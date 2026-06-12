/* cfe_api_int.h - ARM 32-bit version for BCM47081 */
#ifndef CFE_API_INT_H
#define CFE_API_INT_H

#include <stdint.h>

#define CFE_EPTSEAL 0x43464531

#define CFE_CMD_FW_GETINFO      0
#define CFE_CMD_FW_RESTART      1
#define CFE_CMD_FW_BOOT         2
#define CFE_CMD_FW_CPUCTL       3
#define CFE_CMD_FW_GETTIME      4
#define CFE_CMD_FW_MEMENUM      5
#define CFE_CMD_FW_FLUSHCACHE   6
#define CFE_CMD_DEV_GETHANDLE   9
#define CFE_CMD_DEV_ENUM        10
#define CFE_CMD_DEV_OPEN        11
#define CFE_CMD_DEV_INPSTAT     12
#define CFE_CMD_DEV_READ        13
#define CFE_CMD_DEV_WRITE       14
#define CFE_CMD_DEV_IOCTL       15
#define CFE_CMD_DEV_CLOSE       16
#define CFE_CMD_DEV_GETINFO     17
#define CFE_CMD_ENV_ENUM        20
#define CFE_CMD_ENV_GET         22
#define CFE_CMD_ENV_SET         23
#define CFE_CMD_ENV_DEL         24
#define CFE_CMD_MAX             32
#define CFE_CMD_VENDOR_USE      0x8000

/* 32-bit types for ARM */
typedef uint32_t cfe_xuint_t;
typedef int32_t  cfe_xint_t;
typedef int32_t  cfe_xptr_t;

typedef struct xiocb_buffer_s {
    cfe_xuint_t buf_offset;
    cfe_xptr_t  buf_ptr;
    cfe_xuint_t buf_length;
    cfe_xuint_t buf_retlen;
    cfe_xuint_t buf_ioctlcmd;
} xiocb_buffer_t;

#define buf_devflags buf_ioctlcmd

typedef struct xiocb_inpstat_s {
    cfe_xuint_t inp_status;
} xiocb_inpstat_t;

typedef struct xiocb_envbuf_s {
    cfe_xint_t enum_idx;
    cfe_xptr_t name_ptr;
    cfe_xint_t name_length;
    cfe_xptr_t val_ptr;
    cfe_xint_t val_length;
} xiocb_envbuf_t;

typedef struct xiocb_cpuctl_s {
    cfe_xuint_t cpu_number;
    cfe_xuint_t cpu_command;
    cfe_xuint_t start_addr;
    cfe_xuint_t gp_val;
    cfe_xuint_t sp_val;
    cfe_xuint_t a1_val;
} xiocb_cpuctl_t;

typedef struct xiocb_time_s {
    cfe_xint_t ticks;
} xiocb_time_t;

typedef struct xiocb_exitstat_s {
    cfe_xint_t status;
} xiocb_exitstat_t;

typedef struct xiocb_meminfo_s {
    cfe_xint_t  mi_idx;
    cfe_xint_t  mi_type;
    cfe_xuint_t mi_addr;
    cfe_xuint_t mi_size;
} xiocb_meminfo_t;

typedef struct xiocb_fwinfo_s {
    cfe_xint_t fwi_version;
    cfe_xint_t fwi_totalmem;
    cfe_xint_t fwi_flags;
    cfe_xint_t fwi_boardid;
    cfe_xint_t fwi_bootarea_va;
    cfe_xint_t fwi_bootarea_pa;
    cfe_xint_t fwi_bootarea_size;
    cfe_xint_t fwi_reserved1;
    cfe_xint_t fwi_reserved2;
    cfe_xint_t fwi_reserved3;
} xiocb_fwinfo_t;

typedef struct cfe_xiocb_s {
    cfe_xuint_t xiocb_fcode;
    cfe_xint_t  xiocb_status;
    cfe_xint_t  xiocb_handle;
    cfe_xuint_t xiocb_flags;
    cfe_xuint_t xiocb_psize;
    union {
        xiocb_buffer_t   xiocb_buffer;
        xiocb_inpstat_t  xiocb_inpstat;
        xiocb_envbuf_t   xiocb_envbuf;
        xiocb_cpuctl_t   xiocb_cpuctl;
        xiocb_time_t     xiocb_time;
        xiocb_meminfo_t  xiocb_meminfo;
        xiocb_fwinfo_t   xiocb_fwinfo;
        xiocb_exitstat_t xiocb_exitstat;
    } plist;
} cfe_xiocb_t;

#endif /* CFE_API_INT_H */