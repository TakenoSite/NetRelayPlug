#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERR,
    LOG_LEVEL_NONE,
} LOG_LEVEL;

typedef const char cchar;

#define COL_DEF "\x1B[0m"   //white
#define COL_RED "\x1B[31m"  //red
#define COL_GRE "\x1B[32m"  //green
#define COL_BLU "\x1B[34m"  //blue
#define COL_YEL "\x1B[33m"  //yellow
#define COL_WHE "\x1B[37m"  //white
#define COL_CYN "\x1B[36m"
#define COL_MAG "\x1B[35m"

void set_log_level(int lvl);

void log_print(LOG_LEVEL lvl, cchar *color, cchar *f, int l, cchar *fmt, ...);

#define log_debug(fmt, args...) log_print(LOG_LEVEL_DEBUG, COL_WHE,  __FILE__, __LINE__, fmt, ##args)
#define log_info(fmt, args...)  log_print(LOG_LEVEL_INFO, COL_GRE, __FILE__, __LINE__, fmt, ##args)
#define log_warning(fmt, args...) log_print(LOG_LEVEL_WARN, COL_CYN, __FILE__, __LINE__, fmt, ##args)
#define log_error(fmt, args...) log_print(LOG_LEVEL_ERR, COL_YEL, __FILE__, __LINE__, fmt, ##args)

void nlog_print(LOG_LEVEL lvl, cchar *color, cchar *f, int l ,size_t len, cchar *fmt, ...);
#define nlog_debug(len, fmt, args...) nlog_print(LOG_LEVEL_DEBUG, COL_WHE,  __FILE__, __LINE__, len, fmt, ##args)
#define nlog_info(len, fmt, args...)  nlog_print(LOG_LEVEL_INFO, COL_GRE, __FILE__, __LINE__, len, fmt, ##args)
#define nlog_warning(len, fmt, args...) nlog_print(LOG_LEVEL_WARN, COL_CYN, __FILE__, __LINE__, len,  fmt, ##args)
#define nlog_error(len, fmt, args...) nlog_print(LOG_LEVEL_ERR, COL_YEL, __FILE__, __LINE__, len, fmt, ##args)


#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */


}
#endif

#endif


