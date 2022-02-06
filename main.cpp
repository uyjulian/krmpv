/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2022 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
//   See details of license at "LICENSE"   //
//                                         //
/////////////////////////////////////////////

#include "ncbind/ncbind.hpp"
#include "CharacterSet.h"
#include <vector>
#include <mpv/client.h>
#include <mpv/stream_cb.h>

#define	WM_MPV_WAKEUP (WM_APP+3008)

class KrMpv
{
	int last_error;
	mpv_handle *client;
	iTJSDispatch2 *objthis;
	tTJSVariant window;
	tTJSVariant message_receiver_callback;
	HWND hwnd;

	static int64_t size_fn(void *cookie)
	{
		IStream *ip = reinterpret_cast<IStream *>(cookie);
		STATSTG statstg;
		ip->Stat(&statstg, STATFLAG_NONAME);
		return statstg.cbSize.QuadPart;
	}

	static int64_t read_fn(void *cookie, char *buf, uint64_t nbytes)
	{
		IStream *ip = reinterpret_cast<IStream *>(cookie);
		ULONG readbytes;
		ip->Read(reinterpret_cast<void *>(buf), nbytes, &readbytes);
		return readbytes;
	}

	static int64_t seek_fn(void *cookie, int64_t offset)
	{
		IStream *ip = reinterpret_cast<IStream *>(cookie);
		ULARGE_INTEGER newpos;
		HRESULT res = ip->Seek({ .QuadPart = offset }, STREAM_SEEK_SET, &newpos);
		if (res != S_OK)
			return MPV_ERROR_GENERIC;
		return newpos.QuadPart;
	}

	static void close_fn(void *cookie)
	{
		IStream *ip = reinterpret_cast<IStream *>(cookie);
		ip->Release();
	}

	static int open_fn(void *user_data, char *uri, mpv_stream_cb_info *info)
	{
		std::string tmp_str_utf8;
		tjs_string tmp_str_utf16;
		tmp_str_utf8 = uri;
		tmp_str_utf16 = TJS_W("");
		TVPUtf8ToUtf16(tmp_str_utf16, tmp_str_utf8);
		if (tmp_str_utf16.length() <= 8)
		{
			return MPV_ERROR_LOADING_FAILED;
		}
		IStream *ip = reinterpret_cast<IStream *>(TVPCreateIStream(ttstr(tmp_str_utf16.c_str() + 8 /* skip krmpv:// */), TJS_BS_READ));
		info->cookie = reinterpret_cast<void *>(ip);
		info->size_fn = KrMpv::size_fn;
		info->read_fn = KrMpv::read_fn;
		info->seek_fn = KrMpv::seek_fn;
		info->close_fn = KrMpv::close_fn;
		return ip ? 0 : MPV_ERROR_LOADING_FAILED;
	}

	static tTJSVariant mpv_node_to_tjs_variant(mpv_node *node)
	{
		switch (node->format)
		{
			case MPV_FORMAT_NONE:
				return tTJSVariant();
			case MPV_FORMAT_STRING:
				{
					std::string tmp_str_utf8;
					tjs_string tmp_str_utf16;
					tmp_str_utf8 = node->u.string;
					tmp_str_utf16 = TJS_W("");
					TVPUtf8ToUtf16(tmp_str_utf16, tmp_str_utf8);
					return tTJSVariant(ttstr(tmp_str_utf16.c_str()));
				}
			case MPV_FORMAT_INT64:
				return tTJSVariant((tTVInteger)node->u.int64);
			case MPV_FORMAT_DOUBLE:
				return tTJSVariant((tTVReal)node->u.double_);
			case MPV_FORMAT_FLAG:
				return tTJSVariant((tTVInteger)(node->u.flag ? 1 : 0));
			case MPV_FORMAT_BYTE_ARRAY:
				return tTJSVariant((const tjs_uint8 *)node->u.ba->data, node->u.ba->size);
			case MPV_FORMAT_NODE_ARRAY:
				{
					iTJSDispatch2 *arr = TJSCreateArrayObject();
					if (arr)
					{
						int len = node->u.list->num;
						tTJSVariant tmp;
						for (int i = 0; i < len; i += 1)
						{
							tmp = mpv_node_to_tjs_variant(&node->u.list->values[i]);
							arr->PropSetByNum(TJS_MEMBERENSURE, i, &tmp, arr);
						}
						tTJSVariant ret = tTJSVariant(arr, arr);
						arr->Release();
						return ret;
					}
					return tTJSVariant();
				}
			case MPV_FORMAT_NODE_MAP:
				{
					iTJSDispatch2 *dic = TJSCreateDictionaryObject();
					if (dic)
					{
						int len = node->u.list->num;
						tTJSVariant tmp;
						std::string tmp_key_utf8;
						tjs_string tmp_key_utf16;
						for (int i = 0; i < len; i += 1)
						{
							tmp_key_utf8 = node->u.list->keys[i];
							tmp_key_utf16 = TJS_W("");
							TVPUtf8ToUtf16(tmp_key_utf16, tmp_key_utf8);
							tmp = mpv_node_to_tjs_variant(&node->u.list->values[i]);
							dic->PropSet(TJS_MEMBERENSURE, tmp_key_utf16.c_str(), 0, &tmp, dic);
						}
						tTJSVariant ret = tTJSVariant(dic, dic);
						dic->Release();
						return ret;
					}
					return tTJSVariant();
				}
			default:
				return tTJSVariant(TJS_W("[UNSUPPORTED_MPV_FORMAT]"));
		}
		return tTJSVariant();
	}

	static void tjs_variant_to_mpv_node(tTJSVariant src, mpv_node *node)
	{
		switch (src.Type())
		{
			case tvtVoid:
			{
				node->format = MPV_FORMAT_NONE;
				break;
			}
			case tvtObject:
			{
				node->format = MPV_FORMAT_NONE;
				// TODO: implement
#if 0
				iTJSDispatch2 *obj = src.AsObjectNoAddRef();
				if (obj == NULL)
				{
					node->format = MPV_FORMAT_NONE;
				}
				else if (obj->IsInstanceOf(TJS_IGNOREPROP, NULL, NULL, TJS_W("Array"), obj) == TJS_S_TRUE)
				{
					tTJSVariant tmp;
					tTJSVariantClosure closure = param[i]->AsObjectClosureNoAddRef();
					closure.PropGet(0, TJS_W("count"), 0, &tmp, NULL);
					tjs_int count = tmp;
					for (tjs_int i = 0; i < count; i += 1)
					{

					}
				}
				else
				{

				}
#endif
				break;
			}
			case tvtString:
			{
				// TODO: implement
#if 0
				std::string tmp_str_utf8;
				tjs_string tmp_str_utf16;
				tmp_str_utf8 = "";
				tmp_str_utf16 = src.AsString().c_str();
				TVPUtf16ToUtf8(tmp_str_utf8, tmp_str_utf16);
				node->format = MPV_FORMAT_STRING;
				node->u.string = strdup(tmp_str_utf8.c_str());
#endif
				break;
			}
			case tvtOctet:
			{
				node->format = MPV_FORMAT_BYTE_ARRAY;
				tTJSVariantOctet* octet = src.AsOctetNoAddRef();
				node->u.ba->data = (void *)octet->GetData();
				node->u.ba->size = octet->GetLength();
				break;
			}
			case tvtInteger:
			{
				node->format = MPV_FORMAT_INT64;
				node->u.int64 = src.AsInteger();
				break;
			}
			case tvtReal:
			{
				node->format = MPV_FORMAT_DOUBLE;
				node->u.double_ = src.AsReal();
				break;
			}
			default:
			{
				node->format = MPV_FORMAT_NONE;
				break;
			}
		}
	}

	bool message_receiver(tTVPWindowMessage *msg)
	{
		switch (msg->Msg)
		{
			case WM_MPV_WAKEUP: 
			{
				if (this == (KrMpv*)msg->WParam)
				{
					if (message_receiver_callback.Type() == tvtObject)
					{
						message_receiver_callback.AsObjectClosureNoAddRef().FuncCall(0, NULL, NULL, NULL, 0, NULL, NULL);
					}
					return true;
				}
			}
			break;
		}
		return false;
	}

	static bool __stdcall message_receiver_entry(void *userdata, tTVPWindowMessage *msg)
	{
		return ((KrMpv *)userdata)->message_receiver(msg);
	}

	void setup_message_receiver(bool enable)
	{
		tTJSVariant mode     = enable ? (tTVInteger)(tjs_int)wrmRegister : (tTVInteger)(tjs_int)wrmUnregister;
		tTJSVariant recvfunc = (tTVInteger)(tjs_intptr_t)message_receiver_entry;
		tTJSVariant userdata = (tTVInteger)(tjs_intptr_t)this;
		tTJSVariant *p[] = {&mode, &recvfunc, &userdata};
		if (window.Type() == tvtObject && window.AsObjectClosureNoAddRef().FuncCall(0, TJS_W("registerMessageReceiver"), NULL, NULL, 4, p, NULL) != TJS_S_OK)
		{
			TVPThrowExceptionMessage(TJS_W("Unable to register message receiver"));
		}
	}

	void wakeup_callback(void)
	{
		::PostMessage(hwnd, WM_MPV_WAKEUP, reinterpret_cast<WPARAM>(this), 0);
	}

	static void wakeup_callback_entry(void *userdata)
	{
		((KrMpv *)userdata)->wakeup_callback();
	}

	void setup_wakeup_callback(bool enable)
	{
		if (enable)
		{
			mpv_set_wakeup_callback(client, wakeup_callback_entry, (void *)this);
		}
		else
		{
			mpv_set_wakeup_callback(client, NULL, NULL);
		}
	}

public:

	KrMpv(iTJSDispatch2 *objthis)
	{
		this->objthis = objthis;
		this->client = mpv_create();
		if (!this->client)
		{
			TVPThrowExceptionMessage(TJS_W("Unable to create mpv context"));
		}
	}

	~KrMpv()
	{
		setup_wakeup_callback(false);
		setup_message_receiver(false);
		mpv_destroy(client);
	}

	static tjs_error factory(KrMpv **result, tjs_int numparams, tTJSVariant **params, iTJSDispatch2 *objthis)
	{
		if (result)
		{
			*result = new KrMpv(objthis);
		}
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD script_initialize( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		self->last_error = mpv_initialize(self->client);
		if (self->last_error >= 0)
		{
			self->last_error = mpv_stream_cb_add_ro(self->client, "krmpv", nullptr, KrMpv::open_fn);
		}
		tTJSVariant r = self->last_error < 0 ? tTJSVariant() : (tTJSVariant)1;
		return TJS_S_OK;
	}

	// args: wait in secs (infinite if negative) if mpv doesn't send events earlier.
	static tjs_error TJS_INTF_METHOD script_wait_event( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		double timeout = -1;
		if (numparams >= 1 && param[0]->Type() != tvtVoid)
		{
			timeout = param[0]->AsInteger();
		}
		mpv_event *event = mpv_wait_event(self->client, timeout);

		mpv_node rn;
		mpv_event_to_node(&rn, event);
		tTJSVariant r = mpv_node_to_tjs_variant(&rn);
		if (result)
		{
			*result = r;
		}
		mpv_free_node_contents(&rn);
		return TJS_S_OK;
	}

	// args - command [with arguments] as string
	static tjs_error TJS_INTF_METHOD script_command( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams < 1) return TJS_E_BADPARAMCOUNT;
		std::string tmp_str_utf8;
		tjs_string tmp_str_utf16;
		tmp_str_utf8 = "";
		tmp_str_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str_utf8, tmp_str_utf16);
		self->last_error = mpv_command_string(self->client, tmp_str_utf8.c_str());
		tTJSVariant r = self->last_error < 0 ? tTJSVariant() : (tTJSVariant)1;
		return TJS_S_OK;
	}

	// args - command [with arguments] as string
	static tjs_error TJS_INTF_METHOD script_commandv( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams == 0) return TJS_E_BADPARAMCOUNT;
		std::vector<std::string> argv_storage;
		// First, allocate stuff here (exception can occur here)
		for (tjs_int i = 0; i < numparams; i += 1)
		{
			std::string tmp_str_utf8;
			tjs_string tmp_str_utf16;
			tmp_str_utf8 = "";
			tmp_str_utf16 = ttstr(param[i]->AsStringNoAddRef()).c_str();
			TVPUtf16ToUtf8(tmp_str_utf8, tmp_str_utf16);
			argv_storage.push_back(tmp_str_utf8);
		}
		// Then malloc and pointer assignment can occur here
		const char **argv = (const char **)malloc((numparams + 1) * sizeof(const char *));
		for (tjs_int i = 0; i < numparams; i += 1)
		{
			argv[i] = argv_storage[i].c_str();
		}
		argv[numparams] = NULL;	
		self->last_error = mpv_command(self->client, argv);
		free(argv);
		tTJSVariant r = self->last_error < 0 ? tTJSVariant() : (tTJSVariant)1;
		return TJS_S_OK;
	}

	// TODO: implement
#if 0
	// args: native (array of command and args, similar to commandv) [,def]
	static tjs_error TJS_INTF_METHOD script_command_native( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
		// mpv_command_node(self->client, &cmd, presult_node);
		return TJS_S_OK;
	}
#endif

	// args: name [,def]
	static tjs_error TJS_INTF_METHOD script_get_property_bool( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams < 1) return TJS_E_BADPARAMCOUNT;
		int r;
		std::string tmp_str_utf8;
		tjs_string tmp_str_utf16;
		tmp_str_utf8 = "";
		tmp_str_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str_utf8, tmp_str_utf16);
		self->last_error = mpv_get_property(self->client, tmp_str_utf8.c_str(), MPV_FORMAT_FLAG, &r);
		if (self->last_error >= 0)
		{
			if (result)
			{
				*result = !!r;
			}
		}
		return TJS_S_OK;
	}

	// args: name [,def]
	static tjs_error TJS_INTF_METHOD script_get_property_number( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams < 1) return TJS_E_BADPARAMCOUNT;
		double r;
		std::string tmp_str_utf8;
		tjs_string tmp_str_utf16;
		tmp_str_utf8 = "";
		tmp_str_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str_utf8, tmp_str_utf16);
		self->last_error = mpv_get_property(self->client, tmp_str_utf8.c_str(), MPV_FORMAT_DOUBLE, &r);
		if (self->last_error >= 0)
		{
			if (result)
			{
				*result = r;
			}
		}
		return TJS_S_OK;
	}

	// TODO: implement
#if 0
	// args: name [,def]
	static tjs_error TJS_INTF_METHOD script_get_property_native( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
		const char *name = js_tostring(J, 1);
		mpv_node *presult_node = new_af_mpv_node(af);
		int e = mpv_get_property(self->client, name, MPV_FORMAT_NODE, presult_node);
		if (!pushed_error(J, e, 2))
			pushnode(J, presult_node);
		return TJS_S_OK;
	}
#endif

	// args: name [,def]
	static tjs_error TJS_INTF_METHOD script_get_property( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		char *r = NULL;
		std::string tmp_str1_utf8;
		tjs_string tmp_str1_utf16;
		tmp_str1_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str1_utf8, tmp_str1_utf16);
		self->last_error = mpv_get_property(self->client, tmp_str1_utf8.c_str(), MPV_FORMAT_STRING, &r);
		if (self->last_error >= 0)
		{
			std::string tmp_str2_utf8;
			tjs_string tmp_str2_utf16;
			tmp_str2_utf8 = r;
			TVPUtf8ToUtf16(tmp_str2_utf16, tmp_str2_utf8);
			if (result)
			{
				*result = ttstr(tmp_str2_utf16.c_str());
			}
			mpv_free(r);
		}
		return TJS_S_OK;
	}

	// args: name, string value
	static tjs_error TJS_INTF_METHOD script_set_property( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams < 2) return TJS_E_BADPARAMCOUNT;
		std::string tmp_str1_utf8;
		tjs_string tmp_str1_utf16;
		tmp_str1_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str1_utf8, tmp_str1_utf16);
		std::string tmp_str2_utf8;
		tjs_string tmp_str2_utf16;
		tmp_str2_utf16 = ttstr(param[1]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str2_utf8, tmp_str2_utf16);
		self->last_error = mpv_set_property_string(self->client, tmp_str1_utf8.c_str(), tmp_str2_utf8.c_str());
		tTJSVariant r = self->last_error < 0 ? tTJSVariant() : (tTJSVariant)1;
		if (result)
		{
			*result = r;
		}
		return TJS_S_OK;
	}

	// args: name, boolean
	static tjs_error TJS_INTF_METHOD script_set_property_bool( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams < 2) return TJS_E_BADPARAMCOUNT;
		std::string tmp_str1_utf8;
		tjs_string tmp_str1_utf16;
		tmp_str1_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str1_utf8, tmp_str1_utf16);
		int v = !!param[1]->AsInteger();
		self->last_error = mpv_set_property(self->client, tmp_str1_utf8.c_str(), MPV_FORMAT_FLAG, &v);
		tTJSVariant r = self->last_error < 0 ? tTJSVariant() : (tTJSVariant)1;
		if (result)
		{
			*result = r;
		}
		return TJS_S_OK;
	}

	// args: name, number
	static tjs_error TJS_INTF_METHOD script_set_property_number( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
		std::string tmp_str1_utf8;
		tjs_string tmp_str1_utf16;
		tmp_str1_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str1_utf8, tmp_str1_utf16);
		double v = param[1]->AsReal();
		self->last_error = mpv_set_property(self->client, tmp_str1_utf8.c_str(), MPV_FORMAT_DOUBLE, &v);
		tTJSVariant r = self->last_error < 0 ? tTJSVariant() : (tTJSVariant)1;
		if (result)
		{
			*result = r;
		}
		return TJS_S_OK;
	}

	// TODO: implement
#if 0
	// args: name, native value
	tjs_error TJS_INTF_METHOD script_set_property_native( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
		mpv_node node;
		makenode(af, &node, J, 2);
		mpv_handle *h = self->client;
		int e = mpv_set_property(h, js_tostring(J, 1), MPV_FORMAT_NODE, &node);
		push_status(J, e);
		return TJS_S_OK;
	}
#endif

	// args: none, result in millisec
	static tjs_error TJS_INTF_METHOD script_get_time_ms( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		tTJSVariant r = (tTVReal)(mpv_get_time_us(self->client) / (double)(1000));
		if (result)
		{
			*result = r;
		}
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD script_enable_messages( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams < 1) return TJS_E_BADPARAMCOUNT;
		std::string tmp_str1_utf8;
		tjs_string tmp_str1_utf16;
		tmp_str1_utf16 = ttstr(param[0]->AsStringNoAddRef()).c_str();
		TVPUtf16ToUtf8(tmp_str1_utf8, tmp_str1_utf16);
		self->last_error = mpv_request_log_messages(self->client, tmp_str1_utf8.c_str());
		tTJSVariant r = self->last_error < 0 ? tTJSVariant() : (tTJSVariant)1;
		if (result)
		{
			*result = r;
		}
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD script_last_error( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if (result)
		{
			*result = self->last_error;
		}
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD script_set_wakeup_callback( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, KrMpv *self)
	{
		if(numparams < 2) return TJS_E_BADPARAMCOUNT;
		self->setup_wakeup_callback(false);
		self->setup_message_receiver(false);
		if (param[0]->Type() != tvtObject || param[1]->Type() != tvtObject)
		{
			self->message_receiver_callback = tTJSVariant();
			self->window = tTJSVariant();
			self->hwnd = NULL;
		}
		else
		{
			self->message_receiver_callback = *param[0];
			self->window = *param[1];
			tTJSVariant val;
			self->window.AsObjectClosureNoAddRef().PropGet(0, TJS_W("HWND"), NULL, &val, NULL);
			self->hwnd = reinterpret_cast<HWND>((tjs_intptr_t)(tjs_int64)(val));
			self->setup_message_receiver(true);
			self->setup_wakeup_callback(true);
		}
		return TJS_S_OK;
	}
};

NCB_REGISTER_CLASS(KrMpv)
{
	Factory(&ClassT::factory);
	RawCallback("initialize", &Class::script_initialize, 0);
	// RawCallback("log", &Class::script_log, 0);
	RawCallback("wait_event", &Class::script_wait_event, 0);
	// RawCallback("_request_event", &Class::script__request_event, 0);
	// RawCallback("find_config_file", &Class::script_find_config_file, 0);
	RawCallback("command", &Class::script_command, 0);
	RawCallback("commandv", &Class::script_commandv, 0);
	// RawCallback("command_native", &Class::script_command_native, 0);
	// RawCallback("_command_native_async", &Class::script__command_native_async, 0);
	// RawCallback("_abort_async_command", &Class::script__abort_async_command, 0);
	RawCallback("get_property_bool", &Class::script_get_property_bool, 0);
	RawCallback("get_property_number", &Class::script_get_property_number, 0);
	// RawCallback("get_property_native", &Class::script_get_property_native, 0);
	RawCallback("get_property", &Class::script_get_property, 0);
	// RawCallback("get_property_osd", &Class::script_get_property_osd, 0);
	RawCallback("set_property", &Class::script_set_property, 0);
	RawCallback("set_property_bool", &Class::script_set_property_bool, 0);
	RawCallback("set_property_number", &Class::script_set_property_number, 0);
	// RawCallback("set_property_native", &Class::script_set_property_native, 0);
	// RawCallback("_observe_property", &Class::script__observe_property, 0);
	// RawCallback("_unobserve_property", &Class::script__unobserve_property, 0);
	RawCallback("get_time_ms", &Class::script_get_time_ms, 0);
	// RawCallback("format_time", &Class::script_format_time, 0);
	RawCallback("enable_messages", &Class::script_enable_messages, 0);
	// RawCallback("get_wakeup_pipe", &Class::script_get_wakeup_pipe, 0);
	// RawCallback("_hook_add", &Class::script__hook_add, 0);
	// RawCallback("_hook_continue", &Class::script__hook_continue, 0);
	// RawCallback("input_set_section_mouse_area", &Class::script_input_set_section_mouse_area, 0);
	RawCallback("last_error", &Class::script_last_error, 0);
	// RawCallback("_set_last_error", &Class::script__set_last_error, 0);
	RawCallback("set_wakeup_callback", &Class::script_set_wakeup_callback, 0);
	Variant("MPV_ERROR_SUCCESS", (tTVInteger)MPV_ERROR_SUCCESS);
	Variant("MPV_ERROR_EVENT_QUEUE_FULL", (tTVInteger)MPV_ERROR_EVENT_QUEUE_FULL);
	Variant("MPV_ERROR_NOMEM", (tTVInteger)MPV_ERROR_NOMEM);
	Variant("MPV_ERROR_UNINITIALIZED", (tTVInteger)MPV_ERROR_UNINITIALIZED);
	Variant("MPV_ERROR_INVALID_PARAMETER", (tTVInteger)MPV_ERROR_INVALID_PARAMETER);
	Variant("MPV_ERROR_OPTION_NOT_FOUND", (tTVInteger)MPV_ERROR_OPTION_NOT_FOUND);
	Variant("MPV_ERROR_OPTION_FORMAT", (tTVInteger)MPV_ERROR_OPTION_FORMAT);
	Variant("MPV_ERROR_OPTION_ERROR", (tTVInteger)MPV_ERROR_OPTION_ERROR);
	Variant("MPV_ERROR_PROPERTY_NOT_FOUND", (tTVInteger)MPV_ERROR_PROPERTY_NOT_FOUND);
	Variant("MPV_ERROR_PROPERTY_FORMAT", (tTVInteger)MPV_ERROR_PROPERTY_FORMAT);
	Variant("MPV_ERROR_PROPERTY_UNAVAILABLE", (tTVInteger)MPV_ERROR_PROPERTY_UNAVAILABLE);
	Variant("MPV_ERROR_PROPERTY_ERROR", (tTVInteger)MPV_ERROR_PROPERTY_ERROR);
	Variant("MPV_ERROR_COMMAND", (tTVInteger)MPV_ERROR_COMMAND);
	Variant("MPV_ERROR_LOADING_FAILED", (tTVInteger)MPV_ERROR_LOADING_FAILED);
	Variant("MPV_ERROR_AO_INIT_FAILED", (tTVInteger)MPV_ERROR_AO_INIT_FAILED);
	Variant("MPV_ERROR_VO_INIT_FAILED", (tTVInteger)MPV_ERROR_VO_INIT_FAILED);
	Variant("MPV_ERROR_NOTHING_TO_PLAY", (tTVInteger)MPV_ERROR_NOTHING_TO_PLAY);
	Variant("MPV_ERROR_UNKNOWN_FORMAT", (tTVInteger)MPV_ERROR_UNKNOWN_FORMAT);
	Variant("MPV_ERROR_UNSUPPORTED", (tTVInteger)MPV_ERROR_UNSUPPORTED);
	Variant("MPV_ERROR_NOT_IMPLEMENTED", (tTVInteger)MPV_ERROR_NOT_IMPLEMENTED);
	Variant("MPV_ERROR_GENERIC", (tTVInteger)MPV_ERROR_GENERIC);
	Variant("MPV_FORMAT_NONE", (tTVInteger)MPV_FORMAT_NONE);
	Variant("MPV_FORMAT_STRING", (tTVInteger)MPV_FORMAT_STRING);
	Variant("MPV_FORMAT_OSD_STRING", (tTVInteger)MPV_FORMAT_OSD_STRING);
	Variant("MPV_FORMAT_FLAG", (tTVInteger)MPV_FORMAT_FLAG);
	Variant("MPV_FORMAT_INT64", (tTVInteger)MPV_FORMAT_INT64);
	Variant("MPV_FORMAT_DOUBLE", (tTVInteger)MPV_FORMAT_DOUBLE);
	Variant("MPV_FORMAT_NODE", (tTVInteger)MPV_FORMAT_NODE);
	Variant("MPV_FORMAT_NODE_ARRAY", (tTVInteger)MPV_FORMAT_NODE_ARRAY);
	Variant("MPV_FORMAT_NODE_MAP", (tTVInteger)MPV_FORMAT_NODE_MAP);
	Variant("MPV_FORMAT_BYTE_ARRAY", (tTVInteger)MPV_FORMAT_BYTE_ARRAY);
	Variant("MPV_EVENT_NONE", (tTVInteger)MPV_EVENT_NONE);
	Variant("MPV_EVENT_SHUTDOWN", (tTVInteger)MPV_EVENT_SHUTDOWN);
	Variant("MPV_EVENT_LOG_MESSAGE", (tTVInteger)MPV_EVENT_LOG_MESSAGE);
	Variant("MPV_EVENT_GET_PROPERTY_REPLY", (tTVInteger)MPV_EVENT_GET_PROPERTY_REPLY);
	Variant("MPV_EVENT_SET_PROPERTY_REPLY", (tTVInteger)MPV_EVENT_SET_PROPERTY_REPLY);
	Variant("MPV_EVENT_COMMAND_REPLY", (tTVInteger)MPV_EVENT_COMMAND_REPLY);
	Variant("MPV_EVENT_START_FILE", (tTVInteger)MPV_EVENT_START_FILE);
	Variant("MPV_EVENT_END_FILE", (tTVInteger)MPV_EVENT_END_FILE);
	Variant("MPV_EVENT_FILE_LOADED", (tTVInteger)MPV_EVENT_FILE_LOADED);
	Variant("MPV_EVENT_IDLE", (tTVInteger)MPV_EVENT_IDLE);
	Variant("MPV_EVENT_TICK", (tTVInteger)MPV_EVENT_TICK);
	Variant("MPV_EVENT_CLIENT_MESSAGE", (tTVInteger)MPV_EVENT_CLIENT_MESSAGE);
	Variant("MPV_EVENT_VIDEO_RECONFIG", (tTVInteger)MPV_EVENT_VIDEO_RECONFIG);
	Variant("MPV_EVENT_AUDIO_RECONFIG", (tTVInteger)MPV_EVENT_AUDIO_RECONFIG);
	Variant("MPV_EVENT_SEEK", (tTVInteger)MPV_EVENT_SEEK);
	Variant("MPV_EVENT_PLAYBACK_RESTART", (tTVInteger)MPV_EVENT_PLAYBACK_RESTART);
	Variant("MPV_EVENT_PROPERTY_CHANGE", (tTVInteger)MPV_EVENT_PROPERTY_CHANGE);
	Variant("MPV_EVENT_QUEUE_OVERFLOW", (tTVInteger)MPV_EVENT_QUEUE_OVERFLOW);
	Variant("MPV_EVENT_HOOK", (tTVInteger)MPV_EVENT_HOOK);
	Variant("MPV_LOG_LEVEL_NONE", (tTVInteger)MPV_LOG_LEVEL_NONE);
	Variant("MPV_LOG_LEVEL_FATAL", (tTVInteger)MPV_LOG_LEVEL_FATAL);
	Variant("MPV_LOG_LEVEL_ERROR", (tTVInteger)MPV_LOG_LEVEL_ERROR);
	Variant("MPV_LOG_LEVEL_WARN", (tTVInteger)MPV_LOG_LEVEL_WARN);
	Variant("MPV_LOG_LEVEL_INFO", (tTVInteger)MPV_LOG_LEVEL_INFO);
	Variant("MPV_LOG_LEVEL_V", (tTVInteger)MPV_LOG_LEVEL_V);
	Variant("MPV_LOG_LEVEL_DEBUG", (tTVInteger)MPV_LOG_LEVEL_DEBUG);
	Variant("MPV_LOG_LEVEL_TRACE", (tTVInteger)MPV_LOG_LEVEL_TRACE);
	Variant("MPV_END_FILE_REASON_EOF", (tTVInteger)MPV_END_FILE_REASON_EOF);
	Variant("MPV_END_FILE_REASON_STOP", (tTVInteger)MPV_END_FILE_REASON_STOP);
	Variant("MPV_END_FILE_REASON_QUIT", (tTVInteger)MPV_END_FILE_REASON_QUIT);
	Variant("MPV_END_FILE_REASON_ERROR", (tTVInteger)MPV_END_FILE_REASON_ERROR);
	Variant("MPV_END_FILE_REASON_REDIRECT", (tTVInteger)MPV_END_FILE_REASON_REDIRECT);
};
