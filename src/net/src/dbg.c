#include "dbg.h"
#include <stdio.h>

void dbg_print (const int module, const int dbg_level, const char * file, const char * func, const int line, const char * fmt, ...) {
    if (module >= dbg_level) {
        static const char * title[] = {
            [DBG_LEVEL_NONE] = "none",
            [DBG_LEVEL_ERROR] = DBG_STYLE_ERROR "error",
            [DBG_LEVEL_WARNING] = DBG_STYLE_WARNING "warning",
            [DBG_LEVEL_INFO] = "info",
        };
        const char * end = file + strlen(file);
        while (end >= file) {
            if (*end == '/') {
                break;
            }
            end --;
        }
        end ++;
        printf ("%s\t(%s\t%s\t%d):", title[dbg_level], end, func, line);

        va_list args;
        va_start (args, fmt);
        vprintf (fmt, args);
        printf (DBG_STYLE_RESET "\n");
        va_end (args);
    }
}


