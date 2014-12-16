#ifndef __IO_H__
#define __IO_H__

#include "plcmodel.h"

/* I/O Shared Memory Definition */
#define LDI_ADDR_OFFSET 0
#define LDO_ADDR_OFFSET (LDI_ADDR_OFFSET+config->ldi_count*LDI_WORDSIZE)
#define LAI_ADDR_OFFSET (LDO_ADDR_OFFSET+config->ldo_count*LDO_WORDSIZE)
#define LAO_ADDR_OFFSET (LAI_ADDR_OFFSET+config->lai_count*LAI_WORDSIZE)

#define IO_TASK_NAME "io_task"
#define IO_TASK_PRIORITY 99

static void local_di_update(IOConfig *config);
static void local_do_update(IOConfig *config);
static void local_ai_update(IOConfig *config);
static void local_ao_update(IOConfig *config);
static void io_update(void *config);
static void io_task_create();
void io_task_init(IOConfig *config);
void io_task_start(IOConfig *config);
void io_task_delete();
#endif
