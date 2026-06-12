/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  API test program				File: test.c
    *
    *  Small program to test CFE's external API
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001,2002,2003
    *  Broadcom Corporation. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions 
    *     as they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of Broadcom Corporation.  The "Broadcom Corporation" 
    *     name may not be used to endorse or promote products derived 
    *     from this software without the prior written permission of 
    *     Broadcom Corporation.
    *  
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT 
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN 
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT,
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF 
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */


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
