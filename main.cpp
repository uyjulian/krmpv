#include "ncbind/ncbind.hpp"

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
def_mpv_request_event ptr_mpv_request_event;
def_mpv_request_log_messages ptr_mpv_request_log_messages;
def_mpv_wait_event ptr_mpv_wait_event;
def_mpv_wakeup ptr_mpv_wakeup;
def_mpv_set_wakeup_callback ptr_mpv_set_wakeup_callback;
def_mpv_wait_async_requests ptr_mpv_wait_async_requests;
def_mpv_hook_add ptr_mpv_hook_add;
def_mpv_hook_continue ptr_mpv_hook_continue;
def_mpv_stream_cb_add_ro ptr_mpv_stream_cb_add_ro;

class krmpv
{
	static const char *getNarrowString(tTJSVariant * str) {
		ttstr ttbuf(str->AsStringNoAddRef());
		tjs_int len = ttbuf.GetNarrowStrLen();
		char * nstring = new char[len + 1];
		ttbuf.ToNarrowStr(nstring, len);
		return const_cast<const char*>(nstring);
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
		HRESULT res = ip->Seek({ offset }, STREAM_SEEK_SET, &newpos);
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
		info->read_fn = krmpv::read_fn;
		info->seek_fn = krmpv::seek_fn;
		info->close_fn = krmpv::close_fn;
		return ip ? 0 : MPV_ERROR_LOADING_FAILED;
	}
public:
	static tjs_error TJS_INTF_METHOD mpv_client_api_version( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
		*result = static_cast<tTVInteger>(ptr_mpv_client_api_version());
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_error_string( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		*result = ttstr(ptr_mpv_error_string(param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_free( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_free(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_client_name( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_client_name(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_create( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 0) return TJS_E_BADPARAMCOUNT;
		*result = reinterpret_cast<tTVInteger>(ptr_mpv_create());
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_initialize( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_initialize(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_destroy( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_destroy(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_terminate_destroy( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_terminate_destroy(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_create_client( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = reinterpret_cast<tTVInteger>(ptr_mpv_create_client(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()), getNarrowString(param[1])));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_create_weak_client( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = reinterpret_cast<tTVInteger>(ptr_mpv_create_weak_client(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()), getNarrowString(param[1])));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_load_config_file( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_load_config_file(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_get_time_us( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_get_time_us(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_free_node_contents( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_free_node_contents(reinterpret_cast<mpv_node *>((tjs_uintptr_t)param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_set_option( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 4) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_set_option(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]), static_cast<mpv_format>(param[2]->AsInteger()), reinterpret_cast<void *>((tjs_uintptr_t)param[3]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_set_option_string( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 3) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_set_option_string(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]), getNarrowString(param[2]));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_command( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
#if 0
		*result = ptr_mpv_command(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]));
#else
		TVPThrowExceptionMessage(TJS_W("mpv_command is not implemented."));
#endif
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_command_node( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 3) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_command_node(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()), reinterpret_cast<mpv_node *>(param[1]->AsInteger()), reinterpret_cast<mpv_node *>((tjs_uintptr_t)param[2]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_command_string( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_command_string(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_command_node_async( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 3) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_command_node_async(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger(), reinterpret_cast<mpv_node *>(param[2]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_abort_async_command( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_abort_async_command(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger());
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_set_property( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 4) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_set_property(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()), getNarrowString(param[1]), static_cast<mpv_format>(param[2]->AsInteger()), reinterpret_cast<void *>(param[3]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_set_property_string( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 3) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_set_property_string(reinterpret_cast<mpv_handle *>((tjs_uintptr_t)param[0]->AsInteger()), getNarrowString(param[1]), getNarrowString(param[2]));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_set_property_async( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 5) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_set_property_async(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger(), getNarrowString(param[2]), static_cast<mpv_format>(param[3]->AsInteger()), reinterpret_cast<void *>(param[4]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_get_property( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 4) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_get_property(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]), static_cast<mpv_format>(param[2]->AsInteger()), reinterpret_cast<void *>(param[3]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_get_property_string( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ttstr(ptr_mpv_get_property_string(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1])));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_get_property_osd_string( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ttstr(ptr_mpv_get_property_osd_string(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1])));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_get_property_async( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 4) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_get_property_async(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger(), getNarrowString(param[2]), static_cast<mpv_format>(param[3]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_observe_property( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 4) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_observe_property(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger(), getNarrowString(param[2]), static_cast<mpv_format>(param[3]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_unobserve_property( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_unobserve_property(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger());
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_event_name( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		*result = ttstr(ptr_mpv_event_name(static_cast<mpv_event_id>(param[0]->AsInteger())));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_request_event( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 3) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_request_event(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), static_cast<mpv_event_id>(param[1]->AsInteger()), param[2]->AsInteger());
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_request_log_messages( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_request_log_messages(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_wait_event( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = reinterpret_cast<tTVInteger>(ptr_mpv_wait_event(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsReal()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_wakeup( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_wakeup(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_set_wakeup_callback( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 3) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_set_wakeup_callback(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), reinterpret_cast<void (*)(void *)>(param[1]->AsInteger()), reinterpret_cast<void *>(param[2]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_wait_async_requests( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 1) return TJS_E_BADPARAMCOUNT;
		ptr_mpv_wait_async_requests(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()));
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_hook_add( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 4) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_hook_add(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger(), getNarrowString(param[2]), param[3]->AsInteger());
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD mpv_hook_continue( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_hook_continue(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), param[1]->AsInteger());
		return TJS_S_OK;
	}

	static tjs_error TJS_INTF_METHOD krmpv_stream_cb_add_ro( tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(numparams != 2) return TJS_E_BADPARAMCOUNT;
		*result = ptr_mpv_stream_cb_add_ro(reinterpret_cast<mpv_handle *>(param[0]->AsInteger()), getNarrowString(param[1]), nullptr, krmpv::open_fn);
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
	if (!(ptr_mpv_request_event = (def_mpv_request_event)GetProcAddress(dllhandle, "mpv_request_event"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_request_event in mpv-1.dll"));
	if (!(ptr_mpv_request_log_messages = (def_mpv_request_log_messages)GetProcAddress(dllhandle, "mpv_request_log_messages"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_request_log_messages in mpv-1.dll"));
	if (!(ptr_mpv_wait_event = (def_mpv_wait_event)GetProcAddress(dllhandle, "mpv_wait_event"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_wait_event in mpv-1.dll"));
	if (!(ptr_mpv_wakeup = (def_mpv_wakeup)GetProcAddress(dllhandle, "mpv_wakeup"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_wakeup in mpv-1.dll"));
	if (!(ptr_mpv_set_wakeup_callback = (def_mpv_set_wakeup_callback)GetProcAddress(dllhandle, "mpv_set_wakeup_callback"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_set_wakeup_callback in mpv-1.dll"));
	if (!(ptr_mpv_wait_async_requests = (def_mpv_wait_async_requests)GetProcAddress(dllhandle, "mpv_wait_async_requests"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_wait_async_requests in mpv-1.dll"));
	if (!(ptr_mpv_hook_add = (def_mpv_hook_add)GetProcAddress(dllhandle, "mpv_hook_add"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_hook_add in mpv-1.dll"));
	if (!(ptr_mpv_hook_continue = (def_mpv_hook_continue)GetProcAddress(dllhandle, "mpv_hook_continue"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_hook_continue in mpv-1.dll"));
	if (!(ptr_mpv_stream_cb_add_ro = (def_mpv_stream_cb_add_ro)GetProcAddress(dllhandle, "mpv_stream_cb_add_ro"))) TVPThrowExceptionMessage(TJS_W("krmpv: could not find symbol mpv_stream_cb_add_ro in mpv-1.dll"));
	TVPExecuteExpression(
		TJS_W("const ")
		TJS_W("MPV_ERROR_SUCCESS = 0,")
		TJS_W("MPV_ERROR_EVENT_QUEUE_FULL = -1,")
		TJS_W("MPV_ERROR_NOMEM = -2,")
		TJS_W("MPV_ERROR_UNINITIALIZED = -3,")
		TJS_W("MPV_ERROR_INVALID_PARAMETER = -4,")
		TJS_W("MPV_ERROR_OPTION_NOT_FOUND = -5,")
		TJS_W("MPV_ERROR_OPTION_FORMAT = -6,")
		TJS_W("MPV_ERROR_OPTION_ERROR = -7,")
		TJS_W("MPV_ERROR_PROPERTY_NOT_FOUND = -8,")
		TJS_W("MPV_ERROR_PROPERTY_FORMAT = -9,")
		TJS_W("MPV_ERROR_PROPERTY_UNAVAILABLE = -10,")
		TJS_W("MPV_ERROR_PROPERTY_ERROR = -11,")
		TJS_W("MPV_ERROR_COMMAND = -12,")
		TJS_W("MPV_ERROR_LOADING_FAILED = -13,")
		TJS_W("MPV_ERROR_AO_INIT_FAILED = -14,")
		TJS_W("MPV_ERROR_VO_INIT_FAILED = -15,")
		TJS_W("MPV_ERROR_NOTHING_TO_PLAY = -16,")
		TJS_W("MPV_ERROR_UNKNOWN_FORMAT = -17,")
		TJS_W("MPV_ERROR_UNSUPPORTED = -18,")
		TJS_W("MPV_ERROR_NOT_IMPLEMENTED = -19,")
		TJS_W("MPV_ERROR_GENERIC = -20,")
		TJS_W("MPV_FORMAT_NONE = 0,")
		TJS_W("MPV_FORMAT_STRING = 1,")
		TJS_W("MPV_FORMAT_OSD_STRING = 2,")
		TJS_W("MPV_FORMAT_FLAG = 3,")
		TJS_W("MPV_FORMAT_INT64 = 4,")
		TJS_W("MPV_FORMAT_DOUBLE = 5,")
		TJS_W("MPV_FORMAT_NODE = 6,")
		TJS_W("MPV_FORMAT_NODE_ARRAY = 7,")
		TJS_W("MPV_FORMAT_NODE_MAP = 8,")
		TJS_W("MPV_FORMAT_BYTE_ARRAY = 9,")
		TJS_W("MPV_EVENT_NONE = 0,")
		TJS_W("MPV_EVENT_SHUTDOWN = 1,")
		TJS_W("MPV_EVENT_LOG_MESSAGE = 2,")
		TJS_W("MPV_EVENT_GET_PROPERTY_REPLY = 3,")
		TJS_W("MPV_EVENT_SET_PROPERTY_REPLY = 4,")
		TJS_W("MPV_EVENT_COMMAND_REPLY = 5,")
		TJS_W("MPV_EVENT_START_FILE = 6,")
		TJS_W("MPV_EVENT_END_FILE = 7,")
		TJS_W("MPV_EVENT_FILE_LOADED = 8,")
		TJS_W("MPV_EVENT_IDLE = 11,")
		TJS_W("MPV_EVENT_TICK = 14,")
		TJS_W("MPV_EVENT_CLIENT_MESSAGE = 16,")
		TJS_W("MPV_EVENT_VIDEO_RECONFIG = 17,")
		TJS_W("MPV_EVENT_AUDIO_RECONFIG = 18,")
		TJS_W("MPV_EVENT_SEEK = 20,")
		TJS_W("MPV_EVENT_PLAYBACK_RESTART = 21,")
		TJS_W("MPV_EVENT_PROPERTY_CHANGE = 22,")
		TJS_W("MPV_EVENT_QUEUE_OVERFLOW = 24,")
		TJS_W("MPV_EVENT_HOOK = 25,")
		TJS_W("MPV_LOG_LEVEL_NONE = 0,")
		TJS_W("MPV_LOG_LEVEL_FATAL = 10,")
		TJS_W("MPV_LOG_LEVEL_ERROR = 20,")
		TJS_W("MPV_LOG_LEVEL_WARN = 30,")
		TJS_W("MPV_LOG_LEVEL_INFO = 40,")
		TJS_W("MPV_LOG_LEVEL_V = 50,")
		TJS_W("MPV_LOG_LEVEL_DEBUG = 60,")
		TJS_W("MPV_LOG_LEVEL_TRACE = 70,")
		TJS_W("MPV_END_FILE_REASON_EOF = 0,")
		TJS_W("MPV_END_FILE_REASON_STOP = 2,")
		TJS_W("MPV_END_FILE_REASON_QUIT = 3,")
		TJS_W("MPV_END_FILE_REASON_ERROR = 4,")
		TJS_W("MPV_END_FILE_REASON_REDIRECT = 5")
	);
}

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

NCB_POST_REGIST_CALLBACK(unregcb);

NCB_REGISTER_CLASS(krmpv)
{
	RawCallback("mpv_client_api_version", &Class::mpv_client_api_version, TJS_STATICMEMBER);
	RawCallback("mpv_error_string", &Class::mpv_error_string, TJS_STATICMEMBER);
	RawCallback("mpv_free", &Class::mpv_free, TJS_STATICMEMBER);
	RawCallback("mpv_client_name", &Class::mpv_client_name, TJS_STATICMEMBER);
	RawCallback("mpv_create", &Class::mpv_create, TJS_STATICMEMBER);
	RawCallback("mpv_initialize", &Class::mpv_initialize, TJS_STATICMEMBER);
	RawCallback("mpv_destroy", &Class::mpv_destroy, TJS_STATICMEMBER);
	RawCallback("mpv_terminate_destroy", &Class::mpv_terminate_destroy, TJS_STATICMEMBER);
	RawCallback("mpv_create_client", &Class::mpv_create_client, TJS_STATICMEMBER);
	RawCallback("mpv_create_weak_client", &Class::mpv_create_weak_client, TJS_STATICMEMBER);
	RawCallback("mpv_load_config_file", &Class::mpv_load_config_file, TJS_STATICMEMBER);
	RawCallback("mpv_get_time_us", &Class::mpv_get_time_us, TJS_STATICMEMBER);
	RawCallback("mpv_free_node_contents", &Class::mpv_free_node_contents, TJS_STATICMEMBER);
	RawCallback("mpv_set_option", &Class::mpv_set_option, TJS_STATICMEMBER);
	RawCallback("mpv_set_option_string", &Class::mpv_set_option_string, TJS_STATICMEMBER);
	RawCallback("mpv_command", &Class::mpv_command, TJS_STATICMEMBER);
	RawCallback("mpv_command_node", &Class::mpv_command_node, TJS_STATICMEMBER);
	RawCallback("mpv_command_string", &Class::mpv_command_string, TJS_STATICMEMBER);
	RawCallback("mpv_command_node_async", &Class::mpv_command_node_async, TJS_STATICMEMBER);
	RawCallback("mpv_abort_async_command", &Class::mpv_abort_async_command, TJS_STATICMEMBER);
	RawCallback("mpv_set_property", &Class::mpv_set_property, TJS_STATICMEMBER);
	RawCallback("mpv_set_property_string", &Class::mpv_set_property_string, TJS_STATICMEMBER);
	RawCallback("mpv_set_property_async", &Class::mpv_set_property_async, TJS_STATICMEMBER);
	RawCallback("mpv_get_property", &Class::mpv_get_property, TJS_STATICMEMBER);
	RawCallback("mpv_get_property_string", &Class::mpv_get_property_string, TJS_STATICMEMBER);
	RawCallback("mpv_get_property_osd_string", &Class::mpv_get_property_osd_string, TJS_STATICMEMBER);
	RawCallback("mpv_get_property_async", &Class::mpv_get_property_async, TJS_STATICMEMBER);
	RawCallback("mpv_observe_property", &Class::mpv_observe_property, TJS_STATICMEMBER);
	RawCallback("mpv_unobserve_property", &Class::mpv_unobserve_property, TJS_STATICMEMBER);
	RawCallback("mpv_event_name", &Class::mpv_event_name, TJS_STATICMEMBER);
	RawCallback("mpv_request_event", &Class::mpv_request_event, TJS_STATICMEMBER);
	RawCallback("mpv_request_log_messages", &Class::mpv_request_log_messages, TJS_STATICMEMBER);
	RawCallback("mpv_wait_event", &Class::mpv_wait_event, TJS_STATICMEMBER);
	RawCallback("mpv_wakeup", &Class::mpv_wakeup, TJS_STATICMEMBER);
	RawCallback("mpv_set_wakeup_callback", &Class::mpv_set_wakeup_callback, TJS_STATICMEMBER);
	RawCallback("mpv_wait_async_requests", &Class::mpv_wait_async_requests, TJS_STATICMEMBER);
	RawCallback("mpv_hook_add", &Class::mpv_hook_add, TJS_STATICMEMBER);
	RawCallback("mpv_hook_continue", &Class::mpv_hook_continue, TJS_STATICMEMBER);
	RawCallback("krmpv_stream_cb_add_ro", &Class::krmpv_stream_cb_add_ro, TJS_STATICMEMBER);
};
