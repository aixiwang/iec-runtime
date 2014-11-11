#include <native/task.h>
#include <native/heap.h>
#include "iocontroller.h"
#include "logger.h"

static RT_TASK io_task;
RT_HEAP io_heap_desc;
char *io_shm;
extern ec_map_t ec_msg;

static void local_di_update(IO_CONFIG *config) {
    LOGGER_DBG("TRACE: io_shm .local_di = %d", *(uint8_t *)&io_shm[LDI_ADDR_OFFSET]);
}

static void local_do_update(IO_CONFIG *config) {
    LOGGER_DBG("TRACE: io_shm .local_do = %d", *(uint8_t *)&io_shm[LDO_ADDR_OFFSET]);
}

static void local_ai_update(IO_CONFIG *config) {
    LOGGER_DBG("TRACE: io_shm .local_ai = %d", *(uint8_t *)&io_shm[LAI_ADDR_OFFSET]);
}

static void local_ao_update(IO_CONFIG *config) {
    LOGGER_DBG("TRACE: io_shm .local_ao = %d", *(uint8_t *)&io_shm[LAO_ADDR_OFFSET]);
}

static void io_update(void *config) {
    IO_CONFIG *io_config = (IO_CONFIG *)config;
	rt_task_set_periodic(NULL, TM_NOW, io_config->update_interval);
	while (1) {
		rt_task_wait_period(NULL);
		if (io_config->ldi_count != 0 ) local_di_update(io_config);
		if (io_config->ldo_count != 0 ) local_do_update(io_config);
		if (io_config->lai_count != 0 ) local_ai_update(io_config);
		if (io_config->lao_count != 0 ) local_ao_update(io_config);
	}
}

static void io_task_create() {
	if (rt_task_create(&io_task, IO_TASK_NAME, 0, IO_TASK_PRIORITY, 0)) {
        LOGGER_ERR(EC_IO_TASK_CREATE, "");
	}
}

void io_task_init(IO_CONFIG *config) {
    int size = config->ldi_count * LDI_WORDSIZE + config->ldo_count * LDO_WORDSIZE +
        config->lai_count * LAI_WORDSIZE + config->lao_count * LAO_WORDSIZE;
	rt_heap_create(&io_heap_desc, "io_shm", size, H_SHARED);
    rt_heap_alloc(&io_heap_desc, size, TM_INFINITE, (void **)&io_shm);
    io_task_create();
}
void io_task_start(IO_CONFIG *config) {
	if (rt_task_start(&io_task, &io_update, (void *)config)) {
        LOGGER_ERR(EC_IO_TASK_START, "");
	}
}

void io_task_delete() {
	if (rt_task_delete(&io_task)) {
	}
}
