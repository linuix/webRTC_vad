//
// Created by link on 19-10-31.
//
#define LINUX 0
#define ANDROID 1


#define MY_PLATFORM LINUX

#if MY_PLATFORM == LINUX


enum
{
    OSAL_LOG_DEBUG,
    OSAL_LOG_INFO,
    OSAL_LOG_WARN,
    OSAL_LOG_ERROR,
    OSAL_LOG_FATAL
};

#define OSAL_LOG_MACRO(_level,_format,...)  printf(_format":[file = %s fun name = %s line = %d]\n",##__VA_ARGS__,__FILE__,__func__,__LINE__)
//#define LOGD(_format, ...)  printf(_format, ##__VA_ARGS__)

#else

enum
{
    OSAL_LOG_DEBUG = ANDROID_LOG_DEBUG,
    OSAL_LOG_INFO = ANDROID_LOG_INFO,
    OSAL_LOG_WARN = ANDROID_LOG_WARN,
    OSAL_LOG_ERROR = ANDROID_LOG_ERROR,
    OSAL_LOG_FATAL = ANDROID_LOG_FATAL
};

#define OSAL_LOG(_module, _level, _tag, _format, ...) __android_log_print(_level, _tag, _format, ##__VA_ARGS__)


#define OSAL_LOG_MACRO(_level, _format, ...) \
do \
{ \
    if (OSAL_LOG_LEVEL_ENABLED(_level)) \
    { \
        int OSAL_JOIN(__log_file_name_pos, __LINE__)  = ((int)(std::string(__FILE__).rfind("/"))) + 1; \
        ::std::string OSAL_JOIN(__log_tag, __LINE__) = __PRETTY_FUNCTION__; \
        ::std::string::size_type OSAL_JOIN(__log_tag_start, __LINE__) = ::std::string::npos; \
        ::std::string::size_type OSAL_JOIN(__log_tag_end, __LINE__) =OSAL_JOIN(__log_tag, __LINE__).find_last_of('('); \
        if (::std::string::npos != OSAL_JOIN(__log_tag_end, __LINE__)) \
            OSAL_JOIN(__log_tag, __LINE__).erase(OSAL_JOIN(__log_tag_end, __LINE__)); \
        OSAL_JOIN(__log_tag_start, __LINE__) = OSAL_JOIN(__log_tag, __LINE__).find_last_of(' '); \
        if (::std::string::npos != OSAL_JOIN(__log_tag_start, __LINE__)) \
            OSAL_JOIN(__log_tag, __LINE__).erase(0, OSAL_JOIN(__log_tag_start, __LINE__)+1); \
        OSAL_LOG("",_level, ("TXZ_AUDIO_DECODE::"+ OSAL_JOIN(__log_tag, __LINE__)).c_str(), _format"[%s:%d]", ##__VA_ARGS__, __FILE__+OSAL_JOIN(__log_file_name_pos, __LINE__), __LINE__); \
    } \
} while(0)
#endif



#define LOGD(_format, ...) OSAL_LOG_MACRO(OSAL_LOG_DEBUG, _format, ##__VA_ARGS__)
#define LOGI(_format, ...) OSAL_LOG_MACRO(OSAL_LOG_INFO, _format, ##__VA_ARGS__)
#define LOGW(_format, ...) OSAL_LOG_MACRO(OSAL_LOG_WARN, _format, ##__VA_ARGS__)
#define LOGE(_format, ...) OSAL_LOG_MACRO(OSAL_LOG_ERROR, _format, ##__VA_ARGS__)
#define LOGF(_format, ...) OSAL_LOG_MACRO(OSAL_LOG_FATAL, _format, ##__VA_ARGS__)
