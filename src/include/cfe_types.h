#ifndef CFE_TYPES_H
#define CFE_TYPES_H

#include <stdint.h>
#include <stddef.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct cons_s {
    char *str;
    int num;
} cons_t;

#endif /* CFE_TYPES_H */