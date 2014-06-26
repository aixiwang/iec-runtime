#include <string.h>
#include "generator.h"
#include "debug.h"

extern int DEBUG_LEVEL;

/* PLC Object File Header Generator */
static void generate_io_refresh_interval(FILE *fp, io_refresh_interval_t interval) {
	PRINT(DEBUG_TRC, "TRACE: io_refresh_interval = %d", interval);
	if (fwrite(&interval, sizeof(io_refresh_interval_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating io refresh interval (%d)...", interval);
	}
}
static void generate_task_count(FILE *fp, task_count_t count) {
	PRINT(DEBUG_TRC, "TRACE: task_count = %d", count);
	if (fwrite(&count, sizeof(task_count_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating task count (%d)...", count);
	}
}
void generate_bin_header(FILE *fp, BIN_HEADER *header) {
	generate_io_refresh_interval(fp, header->io_refresh_interval);
	generate_task_count(fp, header->task_count);
}

/* PLC Object File Body -- Task Property Segment Generator */
static void generate_task_name_size(FILE *fp, task_name_size_t size) {
	PRINT(DEBUG_TRC, "TRACE: task_name_size = %d", size);
	if (fwrite(&size, sizeof(task_name_size_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating size of task name (%d)...", size);
	}
}
static void generate_task_name(FILE *fp, task_name_t *name) {
	PRINT(DEBUG_TRC, "TRACE: task_name = %s", name);
	if (fwrite(name, strlen(name), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating task name (%s)...", name);
	}
}
static void generate_task_priority(FILE *fp, task_priority_t priority) {
	PRINT(DEBUG_TRC, "TRACE: task_priority = %d", priority);
	if (fwrite(&priority, sizeof(task_priority_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating task priority (%d)...", priority);
	}
}
static void generate_task_interval(FILE *fp, task_interval_t interval) {
	PRINT(DEBUG_TRC, "TRACE: task_interval = %d", interval);
	if (fwrite(&interval, sizeof(task_interval_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating task interval (%d)...", interval);
	}
}
static void generate_tds_size(FILE *fp, tds_size_t size) {
	PRINT(DEBUG_TRC, "TRACE: tds_size = %d", size);
	if (fwrite(&size, sizeof(tds_size_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating size of task data segment (%d)...", size);
	}
}
static void generate_inst_count(FILE *fp, inst_count_t count) {
	PRINT(DEBUG_TRC, "TRACE: inst_count = %d", count);
	if (fwrite(&count, sizeof(inst_count_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating instruction count (%d)...", count);
	}
}

void generate_tps(FILE *fp, BIN_TPS *tps) {
	generate_task_name_size(fp, tps->name_size);
	generate_task_name(fp, tps->name);
	generate_task_priority(fp, tps->priority);
	generate_task_interval(fp, tps->interval);
	generate_tds_size(fp, tps->tds_size);
	generate_inst_count(fp, tps->inst_count);
}

/* PLC Object File Body -- Task Data Segment Generator */
void generate_tds(FILE *fp, BIN_TPS *tps, BIN_TDS *tds) {
	if (fwrite(tds, tps->tds_size, 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating task data segment...", 0);
	}
}

/* PLC Object File Body -- Task Code Segment Generator */
static void generate_inst_id(FILE *fp, inst_id_t id) {
	PRINT(DEBUG_TRC, "TRACE: inst_id = %d", id);
	if (fwrite(&id, sizeof(inst_id_t), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating instruction id (%d)...", id);
	}
}
static void generate_inst_arg_va(FILE *fp, inst_arg_va_t arg_va) {
	PRINT(DEBUG_TRC, "TRACE: inst_arg_va = %d", arg_va);
	if (fwrite(&arg_va, sizeof(arg_va), 1, fp) < 1) {
		PRINT(DEBUG_ERR, "ERROR: generating instruction argument virtual address (%d)...", arg_va);
	}
}
static void generate_inst(FILE *fp, BIN_INST *inst, inst_desc_map_t *inst_desc) {
	generate_inst_id(fp, inst->id);
	for (int i = 0; i < (*inst_desc)[inst->id].args_count; ++i) {
		generate_inst_arg_va(fp, inst->arg_va[i]);
	}
}
void generate_tcs(FILE *fp, BIN_TPS *tps, BIN_TCS *tcs, inst_desc_map_t *inst_desc) {
	for (int i = 0; i < tps->inst_count; ++i) {
		generate_inst(fp, &(tcs->inst[i]), inst_desc);
	}
}
