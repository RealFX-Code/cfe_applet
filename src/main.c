#include "cfe_types.h"
#include "printf.h"
#include "cfe_string.h"
#include "cfe_api.h"

int conhandle;

void appletmain(unsigned long handle,
                unsigned long ept,
                unsigned long reserved,
                unsigned long seal);

static int console_write(const char *buffer, int length)
{
    int res;

    if (conhandle == -1) return -1;

    for (;;) {
        res = cfe_write(conhandle, (unsigned char *)buffer, length);
        if (res < 0) break;
        buffer += res;
        length -= res;
        if (length == 0) break;
    }

    if (res < 0) return -1;
    return 0;
}

static int console_xprint(const char *str)
{
    int count = 0;
    int len;
    const char *p;

    while ((p = strchr(str, '\n'))) {
        console_write(str, p - str);
        console_write("\r\n", 2);
        count += (p - str);
        str = p + 1;
    }

    len = strlen(str);
    console_write(str, len);
    count += len;

    return count;
}

void appletmain(unsigned long handle,
                unsigned long ept,
                unsigned long reserved,
                unsigned long seal)
{
    void (*reboot)(void) = (void *) (uintptr_t) (int) 0xBFC00000;
    char str[100];

    cfe_init(handle,ept);

    conhandle = cfe_getstdhandle(CFE_STDHANDLE_CONSOLE);

    str[0] = 0;
    cfe_getenv("BOOT_CONSOLE",str,sizeof(str));

    char buf[1024];
    npf_snprintf(buf, sizeof(buf), "Hello, world.  Console = %s\n",str);
    console_xprint(buf);

    npf_snprintf(buf, sizeof(buf), "API Seal = %08X\n",(int)seal);
    console_xprint(buf);

    npf_snprintf(buf, sizeof(buf), "Entrypoint=%08X  Handle=%08X\n",(int)ept,(int)handle);
    console_xprint(buf);

    npf_snprintf(buf, sizeof(buf), "Exiting to CFE\n\n");
    console_xprint(buf);

    cfe_exit(CFE_FLG_WARMSTART,0);

    (*reboot)();

}
