#include "ncbind/ncbind.hpp"
#include "CharacterSet.h"
#include <vector>

typedef struct mpv_handle mpv_handle;

typedef enum mpv_error {
	MPV_ERROR_SUCCESS = 0,
	MPV_ERROR_EVENT_QUEUE_FULL = -1,
	MPV_ERROR_NOMEM = -2,
	MPV_ERROR_UNINITIALIZED = -3,
	MPV_ERROR_INVALID_PARAMETER = -4,
	MPV_ERROR_OPTION_NOT_FOUND = -5,
	MPV_ERROR_OPTION_FORMAT = -6,
	MPV_ERROR_OPTION_ERROR = -7,
	MPV_ERROR_PROPERTY_NOT_FOUND = -8,
	MPV_ERROR_PROPERTY_FORMAT = -9,
	MPV_ERROR_PROPERTY_UNAVAILABLE = -10,
	MPV_ERROR_PROPERTY_ERROR = -11,
	MPV_ERROR_COMMAND = -12,
	MPV_ERROR_LOADING_FAILED = -13,
	MPV_ERROR_AO_INIT_FAILED = -14,
	MPV_ERROR_VO_INIT_FAILED = -15,
	MPV_ERROR_NOTHING_TO_PLAY = -16,
	MPV_ERROR_UNKNOWN_FORMAT = -17,
	MPV_ERROR_UNSUPPORTED = -18,
	MPV_ERROR_NOT_IMPLEMENTED = -19,
	MPV_ERROR_GENERIC = -20
} mpv_error;

typedef enum mpv_format {
	MPV_FORMAT_NONE = 0,
	MPV_FORMAT_STRING = 1,
	MPV_FORMAT_OSD_STRING = 2,
	MPV_FORMAT_FLAG = 3,
	MPV_FORMAT_INT64 = 4,
	MPV_FORMAT_DOUBLE = 5,
	MPV_FORMAT_NODE = 6,
	MPV_FORMAT_NODE_ARRAY = 7,
	MPV_FORMAT_NODE_MAP = 8,
	MPV_FORMAT_BYTE_ARRAY = 9
} mpv_format;

typedef struct mpv_node {
	union {
		char *string;
		int flag;
		int64_t int64;
		double double_;
		struct mpv_node_list *list;
		struct mpv_byte_array *ba;
	} u;
	mpv_format format;
} mpv_node;

typedef struct mpv_node_list {
	int num;
	mpv_node *values;
	char **keys;
} mpv_node_list;

typedef struct mpv_byte_array {
	void *data;
	size_t size;
} mpv_byte_array;

typedef enum mpv_event_id {
	MPV_EVENT_NONE = 0,
	MPV_EVENT_SHUTDOWN = 1,
	MPV_EVENT_LOG_MESSAGE = 2,
	MPV_EVENT_GET_PROPERTY_REPLY = 3,
	MPV_EVENT_SET_PROPERTY_REPLY = 4,
	MPV_EVENT_COMMAND_REPLY = 5,
	MPV_EVENT_START_FILE	= 6,
	MPV_EVENT_END_FILE = 7,
	MPV_EVENT_FILE_LOADED = 8,
	MPV_EVENT_IDLE = 11,
	MPV_EVENT_TICK = 14,
	MPV_EVENT_CLIENT_MESSAGE = 16,
	MPV_EVENT_VIDEO_RECONFIG = 17,
	MPV_EVENT_AUDIO_RECONFIG = 18,
	MPV_EVENT_SEEK = 20,
	MPV_EVENT_PLAYBACK_RESTART = 21,
	MPV_EVENT_PROPERTY_CHANGE = 22,
	MPV_EVENT_QUEUE_OVERFLOW = 24,
	MPV_EVENT_HOOK = 25,
} mpv_event_id;
typedef struct mpv_event_property {
	const char *name;
	mpv_format format;
	void *data;
} mpv_event_property;
typedef enum mpv_log_level {
	MPV_LOG_LEVEL_NONE = 0,
	MPV_LOG_LEVEL_FATAL = 10,
	MPV_LOG_LEVEL_ERROR = 20,
	MPV_LOG_LEVEL_WARN = 30,
	MPV_LOG_LEVEL_INFO = 40,
	MPV_LOG_LEVEL_V = 50,
	MPV_LOG_LEVEL_DEBUG = 60,
	MPV_LOG_LEVEL_TRACE = 70,
} mpv_log_level;

typedef struct mpv_event_log_message {
	const char *prefix;
	const char *level;
	const char *text;
	mpv_log_level log_level;
} mpv_event_log_message;

typedef enum mpv_end_file_reason {
	MPV_END_FILE_REASON_EOF = 0,
	MPV_END_FILE_REASON_STOP = 2,
	MPV_END_FILE_REASON_QUIT = 3,
	MPV_END_FILE_REASON_ERROR = 4,
	MPV_END_FILE_REASON_REDIRECT = 5,
} mpv_end_file_reason;

typedef struct mpv_event_end_file {
	int reason;
	int error;
} mpv_event_end_file;
typedef struct mpv_event_client_message {
	int num_args;
	const char **args;
} mpv_event_client_message;

typedef struct mpv_event_hook {
	const char *name;
	uint64_t id;
} mpv_event_hook;

typedef struct mpv_event_command {
	mpv_node result;
} mpv_event_command;

typedef struct mpv_event {
	mpv_event_id event_id;
	int error;
	uint64_t reply_userdata;
	void *data;
} mpv_event;

typedef int64_t (*mpv_stream_cb_read_fn)(void *cookie, char *buf, uint64_t nbytes);
typedef int64_t (*mpv_stream_cb_seek_fn)(void *cookie, int64_t offset);
typedef int64_t (*mpv_stream_cb_size_fn)(void *cookie);
typedef void (*mpv_stream_cb_close_fn)(void *cookie);
typedef struct mpv_stream_cb_info {
	void *cookie;
	mpv_stream_cb_read_fn read_fn;
	mpv_stream_cb_seek_fn seek_fn;
	mpv_stream_cb_size_fn size_fn;
	mpv_stream_cb_close_fn close_fn;
} mpv_stream_cb_info;
typedef int (*mpv_stream_cb_open_ro_fn)(void *user_data, char *uri, mpv_stream_cb_info *info);

HINSTANCE dllhandle;
typedef unsigned long (*def_mpv_client_api_version)();
typedef const char * (*def_mpv_error_string)(int error);
typedef void (*def_mpv_free)(void *data);
typedef const char * (*def_mpv_client_name)(mpv_handle *ctx);
typedef mpv_handle * (*def_mpv_create)();
typedef int (*def_mpv_initialize)(mpv_handle *ctx);
typedef void (*def_mpv_destroy)(mpv_handle *ctx);
typedef void (*def_mpv_terminate_destroy)(mpv_handle *ctx);
typedef mpv_handle * (*def_mpv_create_client)(mpv_handle *ctx, const char *name);
typedef mpv_handle * (*def_mpv_create_weak_client)(mpv_handle *ctx, const char *name);
typedef int (*def_mpv_load_config_file)(mpv_handle *ctx, const char *filename);
typedef int64_t (*def_mpv_get_time_us)(mpv_handle *ctx);
typedef void (*def_mpv_free_node_contents)(mpv_node *node);
typedef int (*def_mpv_set_option)(mpv_handle *ctx, const char *name, mpv_format format, void *data);
typedef int (*def_mpv_set_option_string)(mpv_handle *ctx, const char *name, const char *data);
typedef int (*def_mpv_command)(mpv_handle *ctx, const char **args);
typedef int (*def_mpv_command_node)(mpv_handle *ctx, mpv_node *args, mpv_node *result);
typedef int (*def_mpv_command_string)(mpv_handle *ctx, const char *args);
typedef int (*def_mpv_command_node_async)(mpv_handle *ctx, uint64_t reply_userdata, mpv_node *args);
typedef void (*def_mpv_abort_async_command)(mpv_handle *ctx, uint64_t reply_userdata);
typedef int (*def_mpv_set_property)(mpv_handle *ctx, const char *name, mpv_format format, void *data);
typedef int (*def_mpv_set_property_string)(mpv_handle *ctx, const char *name, const char *data);
typedef int (*def_mpv_set_property_async)(mpv_handle *ctx, uint64_t reply_userdata, const char *name, mpv_format format, void *data);
typedef int (*def_mpv_get_property)(mpv_handle *ctx, const char *name, mpv_format format, void *data);
typedef char * (*def_mpv_get_property_string)(mpv_handle *ctx, const char *name);
typedef char * (*def_mpv_get_property_osd_string)(mpv_handle *ctx, const char *name);
typedef int (*def_mpv_get_property_async)(mpv_handle *ctx, uint64_t reply_userdata, const char *name, mpv_format format);
typedef int (*def_mpv_observe_property)(mpv_handle *mpv, uint64_t reply_userdata, const char *name, mpv_format format);
typedef int (*def_mpv_unobserve_property)(mpv_handle *mpv, uint64_t registered_reply_userdata);
typedef const char * (*def_mpv_event_name)(mpv_event_id event);
typedef int (*def_mpv_event_to_node)(mpv_node *dst, mpv_event *src);
typedef int (*def_mpv_request_event)(mpv_handle *ctx, mpv_event_id event, int enable);
typedef int (*def_mpv_request_log_messages)(mpv_handle *ctx, const char *min_level);
typedef mpv_event * (*def_mpv_wait_event)(mpv_handle *ctx, double timeout);
typedef void (*def_mpv_wakeup)(mpv_handle *ctx);
typedef void (*def_mpv_set_wakeup_callback)(mpv_handle *ctx, void (*cb)(void *d), void *d);
typedef void (*def_mpv_wait_async_requests)(mpv_handle *ctx);
typedef int (*def_mpv_hook_add)(mpv_handle *ctx, uint64_t reply_userdata, const char *name, int priority);
typedef int (*def_mpv_hook_continue)(mpv_handle *ctx, uint64_t id);
typedef int (*def_mpv_stream_cb_add_ro)(mpv_handle *ctx, const char *protocol, void *user_data, mpv_stream_cb_open_ro_fn open_fn);

def_mpv_client_api_version ptr_mpv_client_api_version;
def_mpv_error_string ptr_mpv_error_string;
def_mpv_free ptr_mpv_free;
def_mpv_client_name ptr_mpv_client_name;
def_mpv_create ptr_mpv_create;
def_mpv_initialize ptr_mpv_initialize;
def_mpv_destroy ptr_mpv_destroy;
def_mpv_terminate_destroy ptr_mpv_terminate_destroy;
def_mpv_create_client ptr_mpv_create_client;
def_mpv_create_weak_client ptr_mpv_create_weak_client;
def_mpv_load_config_file ptr_mpv_load_config_file;
def_mpv_get_time_us ptr_mpv_get_time_us;
def_mpv_free_node_contents ptr_mpv_free_node_contents;
def_mpv_set_option ptr_mpv_set_option;
def_mpv_set_option_string ptr_mpv_set_option_string;
def_mpv_command ptr_mpv_command;
def_mpv_command_node ptr_mpv_command_node;
def_mpv_command_string ptr_mpv_command_string;
def_mpv_command_node_async ptr_mpv_command_node_async;
def_mpv_abort_async_command ptr_mpv_abort_async_command;
def_mpv_set_property ptr_mpv_set_property;
def_mpv_set_property_string ptr_mpv_set_property_string;
def_mpv_set_property_async ptr_mpv_set_property_async;
def_mpv_get_property ptr_mpv_get_property;
def_mpv_get_property_string ptr_mpv_get_property_string;
def_mpv_get_property_osd_string ptr_mpv_get_property_osd_string;
def_mpv_get_property_async ptr_mpv_get_property_async;
def_mpv_observe_property ptr_mpv_observe_property;
def_mpv_unobserve_property ptr_mpv_unobserve_property;
def_mpv_event_name ptr_mpv_event_name;
def_mpv_event_to_node ptr_mpv_event_to_node;
def_mpv_request_event ptr_mpv_request_event;
def_mpv_request_log_messages ptr_mpv_request_log_messages;
def_mpv_wait_event ptr_mpv_wait_event;
def_mpv_wakeup ptr_mpv_wakeup;
def_mpv_set_wakeup_callback ptr_mpv_set_wakeup_callback;
def_mpv_wait_async_requests ptr_mpv_wait_async_requests;
def_mpv_hook_add ptr_mpv_hook_add;
def_mpv_hook_continue ptr_mpv_hook_continue;
def_mpv_stream_cb_add_ro ptr_mpv_stream_cb_add_ro;

#define mpv_client_api_version ptr_mpv_client_api_version
#define mpv_error_string ptr_mpv_error_string
#define mpv_free ptr_mpv_free
#define mpv_client_name ptr_mpv_client_name
#define mpv_create ptr_mpv_create
#define mpv_initialize ptr_mpv_initialize
#define mpv_destroy ptr_mpv_destroy
#define mpv_terminate_destroy ptr_mpv_terminate_destroy
#define mpv_create_client ptr_mpv_create_client
#define mpv_create_weak_client ptr_mpv_create_weak_client
#define mpv_load_config_file ptr_mpv_load_config_file
#define mpv_get_time_us ptr_mpv_get_time_us
#define mpv_free_node_contents ptr_mpv_free_node_contents
#define mpv_set_option ptr_mpv_set_option
#define mpv_set_option_string ptr_mpv_set_option_string
#define mpv_command ptr_mpv_command
#define mpv_command_node ptr_mpv_command_node
#define mpv_command_string ptr_mpv_command_string
#define mpv_command_node_async ptr_mpv_command_node_async
#define mpv_abort_async_command ptr_mpv_abort_async_command
#define mpv_set_property ptr_mpv_set_property
#define mpv_set_property_string ptr_mpv_set_property_string
#define mpv_set_property_async ptr_mpv_set_property_async
#define mpv_get_property ptr_mpv_get_property
#define mpv_get_property_string ptr_mpv_get_property_string
#define mpv_get_property_osd_string ptr_mpv_get_property_osd_string
#define mpv_get_property_async ptr_mpv_get_property_async
#define mpv_observe_property ptr_mpv_observe_property
#define mpv_unobserve_property ptr_mpv_unobserve_property
#define mpv_event_name ptr_mpv_event_name
#define mpv_event_to_node ptr_mpv_event_to_node
#define mpv_request_event ptr_mpv_request_event
#define mpv_request_log_messages ptr_mpv_request_log_messages
#define mpv_wait_event ptr_mpv_wait_event
#define mpv_wakeup ptr_mpv_wakeup
#define mpv_set_wakeup_callback ptr_mpv_set_wakeup_callback
#define mpv_wait_async_requests ptr_mpv_wait_async_requests
#define mpv_hook_add ptr_mpv_hook_add
#define mpv_hook_continue ptr_mpv_hook_continue
#define mpv_stream_cb_add_ro ptr_mpv_stream_cb_add_ro

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
		if (nbytes != readbytes) {
			return -1;
		}
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
		IStream *ip = reinterpret_cast<IStream *>(TVPCreateIStream(ttstr(uri), TJS_BS_READ));
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
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
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
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
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

static void regcb()
{
	dllhandle = LoadLibrary(TEXT("mpv-1.dll"));
	if (!dllhandle) TVPThrowExceptionMessage(TJS_W("krmpv: couldn't load mpv-1.dll"));
	if (!(ptr_mpv_client_api_version = (def_mpv_client_api_version)GetProcAddress(dllhandle, "mpv_client_api_version"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_client_api_version in mpv-1.dll"));
	if (!(ptr_mpv_error_string = (def_mpv_error_string)GetProcAddress(dllhandle, "mpv_error_string"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_error_string in mpv-1.dll"));
	if (!(ptr_mpv_free = (def_mpv_free)GetProcAddress(dllhandle, "mpv_free"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_free in mpv-1.dll"));
	if (!(ptr_mpv_client_name = (def_mpv_client_name)GetProcAddress(dllhandle, "mpv_client_name"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_client_name in mpv-1.dll"));
	if (!(ptr_mpv_create = (def_mpv_create)GetProcAddress(dllhandle, "mpv_create"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_create in mpv-1.dll"));
	if (!(ptr_mpv_initialize = (def_mpv_initialize)GetProcAddress(dllhandle, "mpv_initialize"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_initialize in mpv-1.dll"));
	if (!(ptr_mpv_destroy = (def_mpv_destroy)GetProcAddress(dllhandle, "mpv_destroy"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_destroy in mpv-1.dll"));
	if (!(ptr_mpv_terminate_destroy = (def_mpv_terminate_destroy)GetProcAddress(dllhandle, "mpv_terminate_destroy"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_terminate_destroy in mpv-1.dll"));
	if (!(ptr_mpv_create_client = (def_mpv_create_client)GetProcAddress(dllhandle, "mpv_create_client"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_create_client in mpv-1.dll"));
	if (!(ptr_mpv_create_weak_client = (def_mpv_create_weak_client)GetProcAddress(dllhandle, "mpv_create_weak_client"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_create_weak_client in mpv-1.dll"));
	if (!(ptr_mpv_load_config_file = (def_mpv_load_config_file)GetProcAddress(dllhandle, "mpv_load_config_file"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_load_config_file in mpv-1.dll"));
	if (!(ptr_mpv_get_time_us = (def_mpv_get_time_us)GetProcAddress(dllhandle, "mpv_get_time_us"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_get_time_us in mpv-1.dll"));
	if (!(ptr_mpv_free_node_contents = (def_mpv_free_node_contents)GetProcAddress(dllhandle, "mpv_free_node_contents"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_free_node_contents in mpv-1.dll"));
	if (!(ptr_mpv_set_option = (def_mpv_set_option)GetProcAddress(dllhandle, "mpv_set_option"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_set_option in mpv-1.dll"));
	if (!(ptr_mpv_set_option_string = (def_mpv_set_option_string)GetProcAddress(dllhandle, "mpv_set_option_string"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_set_option_string in mpv-1.dll"));
	if (!(ptr_mpv_command = (def_mpv_command)GetProcAddress(dllhandle, "mpv_command"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_command in mpv-1.dll"));
	if (!(ptr_mpv_command_node = (def_mpv_command_node)GetProcAddress(dllhandle, "mpv_command_node"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_command_node in mpv-1.dll"));
	if (!(ptr_mpv_command_string = (def_mpv_command_string)GetProcAddress(dllhandle, "mpv_command_string"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_command_string in mpv-1.dll"));
	if (!(ptr_mpv_command_node_async = (def_mpv_command_node_async)GetProcAddress(dllhandle, "mpv_command_node_async"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_command_node_async in mpv-1.dll"));
	if (!(ptr_mpv_abort_async_command = (def_mpv_abort_async_command)GetProcAddress(dllhandle, "mpv_abort_async_command"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_abort_async_command in mpv-1.dll"));
	if (!(ptr_mpv_set_property = (def_mpv_set_property)GetProcAddress(dllhandle, "mpv_set_property"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_set_property in mpv-1.dll"));
	if (!(ptr_mpv_set_property_string = (def_mpv_set_property_string)GetProcAddress(dllhandle, "mpv_set_property_string"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_set_property_string in mpv-1.dll"));
	if (!(ptr_mpv_set_property_async = (def_mpv_set_property_async)GetProcAddress(dllhandle, "mpv_set_property_async"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_set_property_async in mpv-1.dll"));
	if (!(ptr_mpv_get_property = (def_mpv_get_property)GetProcAddress(dllhandle, "mpv_get_property"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_get_property in mpv-1.dll"));
	if (!(ptr_mpv_get_property_string = (def_mpv_get_property_string)GetProcAddress(dllhandle, "mpv_get_property_string"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_get_property_string in mpv-1.dll"));
	if (!(ptr_mpv_get_property_osd_string = (def_mpv_get_property_osd_string)GetProcAddress(dllhandle, "mpv_get_property_osd_string"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_get_property_osd_string in mpv-1.dll"));
	if (!(ptr_mpv_get_property_async = (def_mpv_get_property_async)GetProcAddress(dllhandle, "mpv_get_property_async"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_get_property_async in mpv-1.dll"));
	if (!(ptr_mpv_observe_property = (def_mpv_observe_property)GetProcAddress(dllhandle, "mpv_observe_property"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_observe_property in mpv-1.dll"));
	if (!(ptr_mpv_unobserve_property = (def_mpv_unobserve_property)GetProcAddress(dllhandle, "mpv_unobserve_property"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_unobserve_property in mpv-1.dll"));
	if (!(ptr_mpv_event_name = (def_mpv_event_name)GetProcAddress(dllhandle, "mpv_event_name"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_event_name in mpv-1.dll"));
	if (!(ptr_mpv_event_to_node = (def_mpv_event_to_node)GetProcAddress(dllhandle, "mpv_event_to_node"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_event_to_node in mpv-1.dll"));
	if (!(ptr_mpv_request_event = (def_mpv_request_event)GetProcAddress(dllhandle, "mpv_request_event"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_request_event in mpv-1.dll"));
	if (!(ptr_mpv_request_log_messages = (def_mpv_request_log_messages)GetProcAddress(dllhandle, "mpv_request_log_messages"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_request_log_messages in mpv-1.dll"));
	if (!(ptr_mpv_wait_event = (def_mpv_wait_event)GetProcAddress(dllhandle, "mpv_wait_event"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_wait_event in mpv-1.dll"));
	if (!(ptr_mpv_wakeup = (def_mpv_wakeup)GetProcAddress(dllhandle, "mpv_wakeup"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_wakeup in mpv-1.dll"));
	if (!(ptr_mpv_set_wakeup_callback = (def_mpv_set_wakeup_callback)GetProcAddress(dllhandle, "mpv_set_wakeup_callback"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_set_wakeup_callback in mpv-1.dll"));
	if (!(ptr_mpv_wait_async_requests = (def_mpv_wait_async_requests)GetProcAddress(dllhandle, "mpv_wait_async_requests"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_wait_async_requests in mpv-1.dll"));
	if (!(ptr_mpv_hook_add = (def_mpv_hook_add)GetProcAddress(dllhandle, "mpv_hook_add"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_hook_add in mpv-1.dll"));
	if (!(ptr_mpv_hook_continue = (def_mpv_hook_continue)GetProcAddress(dllhandle, "mpv_hook_continue"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_hook_continue in mpv-1.dll"));
	if (!(ptr_mpv_stream_cb_add_ro = (def_mpv_stream_cb_add_ro)GetProcAddress(dllhandle, "mpv_stream_cb_add_ro"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_stream_cb_add_ro in mpv-1.dll"));
}

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
	Variant("MPV_ERROR_SUCCESS", 0);
	Variant("MPV_ERROR_EVENT_QUEUE_FULL", -1);
	Variant("MPV_ERROR_NOMEM", -2);
	Variant("MPV_ERROR_UNINITIALIZED", -3);
	Variant("MPV_ERROR_INVALID_PARAMETER", -4);
	Variant("MPV_ERROR_OPTION_NOT_FOUND", -5);
	Variant("MPV_ERROR_OPTION_FORMAT", -6);
	Variant("MPV_ERROR_OPTION_ERROR", -7);
	Variant("MPV_ERROR_PROPERTY_NOT_FOUND", -8);
	Variant("MPV_ERROR_PROPERTY_FORMAT", -9);
	Variant("MPV_ERROR_PROPERTY_UNAVAILABLE", -10);
	Variant("MPV_ERROR_PROPERTY_ERROR", -11);
	Variant("MPV_ERROR_COMMAND", -12);
	Variant("MPV_ERROR_LOADING_FAILED", -13);
	Variant("MPV_ERROR_AO_INIT_FAILED", -14);
	Variant("MPV_ERROR_VO_INIT_FAILED", -15);
	Variant("MPV_ERROR_NOTHING_TO_PLAY", -16);
	Variant("MPV_ERROR_UNKNOWN_FORMAT", -17);
	Variant("MPV_ERROR_UNSUPPORTED", -18);
	Variant("MPV_ERROR_NOT_IMPLEMENTED", -19);
	Variant("MPV_ERROR_GENERIC", -20);
	Variant("MPV_FORMAT_NONE", 0);
	Variant("MPV_FORMAT_STRING", 1);
	Variant("MPV_FORMAT_OSD_STRING", 2);
	Variant("MPV_FORMAT_FLAG", 3);
	Variant("MPV_FORMAT_INT64", 4);
	Variant("MPV_FORMAT_DOUBLE", 5);
	Variant("MPV_FORMAT_NODE", 6);
	Variant("MPV_FORMAT_NODE_ARRAY", 7);
	Variant("MPV_FORMAT_NODE_MAP", 8);
	Variant("MPV_FORMAT_BYTE_ARRAY", 9);
	Variant("MPV_EVENT_NONE", 0);
	Variant("MPV_EVENT_SHUTDOWN", 1);
	Variant("MPV_EVENT_LOG_MESSAGE", 2);
	Variant("MPV_EVENT_GET_PROPERTY_REPLY", 3);
	Variant("MPV_EVENT_SET_PROPERTY_REPLY", 4);
	Variant("MPV_EVENT_COMMAND_REPLY", 5);
	Variant("MPV_EVENT_START_FILE", 6);
	Variant("MPV_EVENT_END_FILE", 7);
	Variant("MPV_EVENT_FILE_LOADED", 8);
	Variant("MPV_EVENT_IDLE", 11);
	Variant("MPV_EVENT_TICK", 14);
	Variant("MPV_EVENT_CLIENT_MESSAGE", 16);
	Variant("MPV_EVENT_VIDEO_RECONFIG", 17);
	Variant("MPV_EVENT_AUDIO_RECONFIG", 18);
	Variant("MPV_EVENT_SEEK", 20);
	Variant("MPV_EVENT_PLAYBACK_RESTART", 21);
	Variant("MPV_EVENT_PROPERTY_CHANGE", 22);
	Variant("MPV_EVENT_QUEUE_OVERFLOW", 24);
	Variant("MPV_EVENT_HOOK", 25);
	Variant("MPV_LOG_LEVEL_NONE", 0);
	Variant("MPV_LOG_LEVEL_FATAL", 10);
	Variant("MPV_LOG_LEVEL_ERROR", 20);
	Variant("MPV_LOG_LEVEL_WARN", 30);
	Variant("MPV_LOG_LEVEL_INFO", 40);
	Variant("MPV_LOG_LEVEL_V", 50);
	Variant("MPV_LOG_LEVEL_DEBUG", 60);
	Variant("MPV_LOG_LEVEL_TRACE", 70);
	Variant("MPV_END_FILE_REASON_EOF", 0);
	Variant("MPV_END_FILE_REASON_STOP", 2);
	Variant("MPV_END_FILE_REASON_QUIT", 3);
	Variant("MPV_END_FILE_REASON_ERROR", 4);
	Variant("MPV_END_FILE_REASON_REDIRECT", 5);
};

// We'll develop a TJS wrapper first…
#if 0
class VideoOverlayMpv
{
public:
	void open() const {TVPThrowExceptionMessage(TJS_W("krmpv: open is not implemented"));}

	void play() const {TVPThrowExceptionMessage(TJS_W("krmpv: play is not implemented"));}

	void stop() const {TVPThrowExceptionMessage(TJS_W("krmpv: stop is not implemented"));}

	void close() const {TVPThrowExceptionMessage(TJS_W("krmpv: close is not implemented"));}

	void setPos() const {TVPThrowExceptionMessage(TJS_W("krmpv: setPos is not implemented"));}

	void setSize() const {TVPThrowExceptionMessage(TJS_W("krmpv: setSize is not implemented"));}

	void setBounds() const {TVPThrowExceptionMessage(TJS_W("krmpv: setBounds is not implemented"));}

	void pause() const {TVPThrowExceptionMessage(TJS_W("krmpv: pause is not implemented"));}

	void rewind() const {TVPThrowExceptionMessage(TJS_W("krmpv: rewind is not implemented"));}

	void prepare() const {TVPThrowExceptionMessage(TJS_W("krmpv: prepare is not implemented"));}

	void setSegmentLoop() const {TVPThrowExceptionMessage(TJS_W("krmpv: setSegmentLoop is not implemented"));}

	void cancelSegmentLoop() const {TVPThrowExceptionMessage(TJS_W("krmpv: cancelSegmentLoop is not implemented"));}

	void setPeriodEvent() const {TVPThrowExceptionMessage(TJS_W("krmpv: setPeriodEvent is not implemented"));}

	void cancelPeriodEvent() const {TVPThrowExceptionMessage(TJS_W("krmpv: cancelPeriodEvent is not implemented"));}

	void selectAudioStream() const {TVPThrowExceptionMessage(TJS_W("krmpv: selectAudioStream is not implemented"));}

	void setMixingLayer() const {TVPThrowExceptionMessage(TJS_W("krmpv: setMixingLayer is not implemented"));}

	void resetMixingLayer() const {TVPThrowExceptionMessage(TJS_W("krmpv: resetMixingLayer is not implemented"));}

	void onStatusChanged() const {TVPThrowExceptionMessage(TJS_W("krmpv: onStatusChanged is not implemented"));}

	void onCallbackCommand() const {TVPThrowExceptionMessage(TJS_W("krmpv: onCallbackCommand is not implemented"));}

	void onPeriod() const {TVPThrowExceptionMessage(TJS_W("krmpv: onPeriod is not implemented"));}

	void onFrameUpdate() const {TVPThrowExceptionMessage(TJS_W("krmpv: onFrameUpdate is not implemented"));}

	int position_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: position_get is not implemented")); return 0;}
	void position_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: position_set is not implemented"));}

	int left_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: left_get is not implemented")); return 0;}
	void left_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: left_set is not implemented"));}

	int top_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: top_get is not implemented")); return 0;}
	void top_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: top_set is not implemented"));}

	int width_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: width_get is not implemented")); return 0;}
	void width_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: width_set is not implemented"));}

	int height_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: height_get is not implemented")); return 0;}
	void height_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: height_set is not implemented"));}

	int originalWidth_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: originalWidth_get is not implemented")); return 0;}
	void originalWidth_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: originalWidth_set is not implemented"));}

	int originalHeight_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: originalHeight_get is not implemented")); return 0;}
	void originalHeight_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: originalHeight_set is not implemented"));}

	int visible_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: visible_get is not implemented")); return 0;}
	void visible_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: visible_set is not implemented"));}

	int loop_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: loop_get is not implemented")); return 0;}
	void loop_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: loop_set is not implemented"));}

	int frame_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: frame_get is not implemented")); return 0;}
	void frame_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: frame_set is not implemented"));}

	int fps_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: fps_get is not implemented")); return 0;}
	void fps_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: fps_set is not implemented"));}

	int numberOfFrame_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: numberOfFrame_get is not implemented")); return 0;}
	void numberOfFrame_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: numberOfFrame_set is not implemented"));}

	int totalTime_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: totalTime_get is not implemented")); return 0;}
	void totalTime_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: totalTime_set is not implemented"));}

	int layer1_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: layer1_get is not implemented")); return 0;}
	void layer1_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: layer1_set is not implemented"));}

	int layer2_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: layer2_get is not implemented")); return 0;}
	void layer2_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: layer2_set is not implemented"));}

	int mode_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: mode_get is not implemented")); return 0;}
	void mode_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: mode_set is not implemented"));}

	int playRate_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: playRate_get is not implemented")); return 0;}
	void playRate_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: playRate_set is not implemented"));}

	int segmentLoopStartFrame_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: segmentLoopStartFrame_get is not implemented")); return 0;}
	void segmentLoopStartFrame_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: segmentLoopStartFrame_set is not implemented"));}

	int segmentLoopEndFrame_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: segmentLoopEndFrame_get is not implemented")); return 0;}
	void segmentLoopEndFrame_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: segmentLoopEndFrame_set is not implemented"));}

	int periodEventFrame_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: periodEventFrame_get is not implemented")); return 0;}
	void periodEventFrame_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: periodEventFrame_set is not implemented"));}

	int audioBalance_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: audioBalance_get is not implemented")); return 0;}
	void audioBalance_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: audioBalance_set is not implemented"));}

	int audioVolume_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: audioVolume_get is not implemented")); return 0;}
	void audioVolume_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: audioVolume_set is not implemented"));}

	int numberOfAudioStream_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: numberOfAudioStream_get is not implemented")); return 0;}
	void numberOfAudioStream_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: numberOfAudioStream_set is not implemented"));}

	int enabledAudioStream_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: enabledAudioStream_get is not implemented")); return 0;}
	void enabledAudioStream_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: enabledAudioStream_set is not implemented"));}

	int numberOfVideoStream_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: numberOfVideoStream_get is not implemented")); return 0;}
	void numberOfVideoStream_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: numberOfVideoStream_set is not implemented"));}

	int enabledVideoStream_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: enabledVideoStream_get is not implemented")); return 0;}
	void enabledVideoStream_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: enabledVideoStream_set is not implemented"));}

	int mixingMovieAlpha_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: mixingMovieAlpha_get is not implemented")); return 0;}
	void mixingMovieAlpha_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: mixingMovieAlpha_set is not implemented"));}

	int mixingMovieBGColor_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: mixingMovieBGColor_get is not implemented")); return 0;}
	void mixingMovieBGColor_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: mixingMovieBGColor_set is not implemented"));}

	int contrastRangeMin_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastRangeMin_get is not implemented")); return 0;}
	void contrastRangeMin_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastRangeMin_set is not implemented"));}

	int contrastRangeMax_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastRangeMax_get is not implemented")); return 0;}
	void contrastRangeMax_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastRangeMax_set is not implemented"));}

	int contrastDefaultValue_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastDefaultValue_get is not implemented")); return 0;}
	void contrastDefaultValue_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastDefaultValue_set is not implemented"));}

	int contrastStepSize_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastStepSize_get is not implemented")); return 0;}
	void contrastStepSize_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: contrastStepSize_set is not implemented"));}

	int contrast_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: contrast_get is not implemented")); return 0;}
	void contrast_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: contrast_set is not implemented"));}

	int brightnessRangeMin_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessRangeMin_get is not implemented")); return 0;}
	void brightnessRangeMin_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessRangeMin_set is not implemented"));}

	int brightnessRangeMax_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessRangeMax_get is not implemented")); return 0;}
	void brightnessRangeMax_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessRangeMax_set is not implemented"));}

	int brightnessDefaultValue_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessDefaultValue_get is not implemented")); return 0;}
	void brightnessDefaultValue_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessDefaultValue_set is not implemented"));}

	int brightnessStepSize_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessStepSize_get is not implemented")); return 0;}
	void brightnessStepSize_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: brightnessStepSize_set is not implemented"));}

	int brightness_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: brightness_get is not implemented")); return 0;}
	void brightness_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: brightness_set is not implemented"));}

	int hueRangeMin_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: hueRangeMin_get is not implemented")); return 0;}
	void hueRangeMin_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: hueRangeMin_set is not implemented"));}

	int hueRangeMax_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: hueRangeMax_get is not implemented")); return 0;}
	void hueRangeMax_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: hueRangeMax_set is not implemented"));}

	int hueDefaultValue_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: hueDefaultValue_get is not implemented")); return 0;}
	void hueDefaultValue_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: hueDefaultValue_set is not implemented"));}

	int hueStepSize_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: hueStepSize_get is not implemented")); return 0;}
	void hueStepSize_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: hueStepSize_set is not implemented"));}

	int hue_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: hue_get is not implemented")); return 0;}
	void hue_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: hue_set is not implemented"));}

	int saturationRangeMin_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationRangeMin_get is not implemented")); return 0;}
	void saturationRangeMin_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationRangeMin_set is not implemented"));}

	int saturationRangeMax_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationRangeMax_get is not implemented")); return 0;}
	void saturationRangeMax_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationRangeMax_set is not implemented"));}

	int saturationDefaultValue_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationDefaultValue_get is not implemented")); return 0;}
	void saturationDefaultValue_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationDefaultValue_set is not implemented"));}

	int saturationStepSize_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationStepSize_get is not implemented")); return 0;}
	void saturationStepSize_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: saturationStepSize_set is not implemented"));}

	int saturation_get() const {TVPThrowExceptionMessage(TJS_W("krmpv: saturation_get is not implemented")); return 0;}
	void saturation_set(int x) const {TVPThrowExceptionMessage(TJS_W("krmpv: saturation_set is not implemented"));}
};

NCB_ATTACH_CLASS(VideoOverlayMpv, VideoOverlay)
{
	Method("open", &Class::open);
	Method("play", &Class::play);
	Method("stop", &Class::stop);
	Method("close", &Class::close);
	Method("setPos", &Class::setPos);
	Method("setSize", &Class::setSize);
	Method("setBounds", &Class::setBounds);
	Method("pause", &Class::pause);
	Method("rewind", &Class::rewind);
	Method("prepare", &Class::prepare);
	Method("setSegmentLoop", &Class::setSegmentLoop);
	Method("cancelSegmentLoop", &Class::cancelSegmentLoop);
	Method("setPeriodEvent", &Class::setPeriodEvent);
	Method("cancelPeriodEvent", &Class::cancelPeriodEvent);
	Method("selectAudioStream", &Class::selectAudioStream);
	Method("setMixingLayer", &Class::setMixingLayer);
	Method("resetMixingLayer", &Class::resetMixingLayer);
	Method("onStatusChanged", &Class::onStatusChanged);
	Method("onCallbackCommand", &Class::onCallbackCommand);
	Method("onPeriod", &Class::onPeriod);
	Method("onFrameUpdate", &Class::onFrameUpdate);
	Property("position", &Class::position_get, &Class::position_set);
	Property("left", &Class::left_get, &Class::left_set);
	Property("top", &Class::top_get, &Class::top_set);
	Property("width", &Class::width_get, &Class::width_set);
	Property("height", &Class::height_get, &Class::height_set);
	Property("originalWidth", &Class::originalWidth_get, &Class::originalWidth_set);
	Property("originalHeight", &Class::originalHeight_get, &Class::originalHeight_set);
	Property("visible", &Class::visible_get, &Class::visible_set);
	Property("loop", &Class::loop_get, &Class::loop_set);
	Property("frame", &Class::frame_get, &Class::frame_set);
	Property("fps", &Class::fps_get, &Class::fps_set);
	Property("numberOfFrame", &Class::numberOfFrame_get, &Class::numberOfFrame_set);
	Property("totalTime", &Class::totalTime_get, &Class::totalTime_set);
	Property("layer1", &Class::layer1_get, &Class::layer1_set);
	Property("layer2", &Class::layer2_get, &Class::layer2_set);
	Property("mode", &Class::mode_get, &Class::mode_set);
	Property("playRate", &Class::playRate_get, &Class::playRate_set);
	Property("segmentLoopStartFrame", &Class::segmentLoopStartFrame_get, &Class::segmentLoopStartFrame_set);
	Property("segmentLoopEndFrame", &Class::segmentLoopEndFrame_get, &Class::segmentLoopEndFrame_set);
	Property("periodEventFrame", &Class::periodEventFrame_get, &Class::periodEventFrame_set);
	Property("audioBalance", &Class::audioBalance_get, &Class::audioBalance_set);
	Property("audioVolume", &Class::audioVolume_get, &Class::audioVolume_set);
	Property("numberOfAudioStream", &Class::numberOfAudioStream_get, &Class::numberOfAudioStream_set);
	Property("enabledAudioStream", &Class::enabledAudioStream_get, &Class::enabledAudioStream_set);
	Property("numberOfVideoStream", &Class::numberOfVideoStream_get, &Class::numberOfVideoStream_set);
	Property("enabledVideoStream", &Class::enabledVideoStream_get, &Class::enabledVideoStream_set);
	Property("mixingMovieAlpha", &Class::mixingMovieAlpha_get, &Class::mixingMovieAlpha_set);
	Property("mixingMovieBGColor", &Class::mixingMovieBGColor_get, &Class::mixingMovieBGColor_set);
	Property("contrastRangeMin", &Class::contrastRangeMin_get, &Class::contrastRangeMin_set);
	Property("contrastRangeMax", &Class::contrastRangeMax_get, &Class::contrastRangeMax_set);
	Property("contrastDefaultValue", &Class::contrastDefaultValue_get, &Class::contrastDefaultValue_set);
	Property("contrastStepSize", &Class::contrastStepSize_get, &Class::contrastStepSize_set);
	Property("contrast", &Class::contrast_get, &Class::contrast_set);
	Property("brightnessRangeMin", &Class::brightnessRangeMin_get, &Class::brightnessRangeMin_set);
	Property("brightnessRangeMax", &Class::brightnessRangeMax_get, &Class::brightnessRangeMax_set);
	Property("brightnessDefaultValue", &Class::brightnessDefaultValue_get, &Class::brightnessDefaultValue_set);
	Property("brightnessStepSize", &Class::brightnessStepSize_get, &Class::brightnessStepSize_set);
	Property("brightness", &Class::brightness_get, &Class::brightness_set);
	Property("hueRangeMin", &Class::hueRangeMin_get, &Class::hueRangeMin_set);
	Property("hueRangeMax", &Class::hueRangeMax_get, &Class::hueRangeMax_set);
	Property("hueDefaultValue", &Class::hueDefaultValue_get, &Class::hueDefaultValue_set);
	Property("hueStepSize", &Class::hueStepSize_get, &Class::hueStepSize_set);
	Property("hue", &Class::hue_get, &Class::hue_set);
	Property("saturationRangeMin", &Class::saturationRangeMin_get, &Class::saturationRangeMin_set);
	Property("saturationRangeMax", &Class::saturationRangeMax_get, &Class::saturationRangeMax_set);
	Property("saturationDefaultValue", &Class::saturationDefaultValue_get, &Class::saturationDefaultValue_set);
	Property("saturationStepSize", &Class::saturationStepSize_get, &Class::saturationStepSize_set);
	Property("saturation", &Class::saturation_get, &Class::saturation_set);
};
#endif

NCB_PRE_REGIST_CALLBACK(regcb);

static void unregcb() {
	FreeLibrary(dllhandle);
	ptr_mpv_client_api_version = nullptr;
	ptr_mpv_error_string = nullptr;
	ptr_mpv_free = nullptr;
	ptr_mpv_client_name = nullptr;
	ptr_mpv_create = nullptr;
	ptr_mpv_initialize = nullptr;
	ptr_mpv_destroy = nullptr;
	ptr_mpv_terminate_destroy = nullptr;
	ptr_mpv_create_client = nullptr;
	ptr_mpv_create_weak_client = nullptr;
	ptr_mpv_load_config_file = nullptr;
	ptr_mpv_get_time_us = nullptr;
	ptr_mpv_free_node_contents = nullptr;
	ptr_mpv_set_option = nullptr;
	ptr_mpv_set_option_string = nullptr;
	ptr_mpv_command = nullptr;
	ptr_mpv_command_node = nullptr;
	ptr_mpv_command_string = nullptr;
	ptr_mpv_command_node_async = nullptr;
	ptr_mpv_abort_async_command = nullptr;
	ptr_mpv_set_property = nullptr;
	ptr_mpv_set_property_string = nullptr;
	ptr_mpv_set_property_async = nullptr;
	ptr_mpv_get_property = nullptr;
	ptr_mpv_get_property_string = nullptr;
	ptr_mpv_get_property_osd_string = nullptr;
	ptr_mpv_get_property_async = nullptr;
	ptr_mpv_observe_property = nullptr;
	ptr_mpv_unobserve_property = nullptr;
	ptr_mpv_event_name = nullptr;
	ptr_mpv_event_to_node = nullptr;
	ptr_mpv_request_event = nullptr;
	ptr_mpv_request_log_messages = nullptr;
	ptr_mpv_wait_event = nullptr;
	ptr_mpv_wakeup = nullptr;
	ptr_mpv_set_wakeup_callback = nullptr;
	ptr_mpv_wait_async_requests = nullptr;
	ptr_mpv_hook_add = nullptr;
	ptr_mpv_hook_continue = nullptr;
	ptr_mpv_stream_cb_add_ro = nullptr;
}

NCB_POST_UNREGIST_CALLBACK(unregcb);
