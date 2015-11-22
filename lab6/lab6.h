#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#define FORMAT_SIZE 2
#define DO_AND_CHECK(COMMAND) \
	err = COMMAND;\
	if(err != SUCCESS) return err;
#define OPERATION_LIST(CODE, NAME) \
	struct spec_ops_t NAME##_ops; \
	NAME##_ops.type = CODE; \
	NAME##_ops.read_spec_head = read_##NAME##_head; \
	NAME##_ops.read_spec_body = read_##NAME##_body; \
	NAME##_ops.write_spec_head = write_##NAME##_head;\
	NAME##_ops.write_spec_body = write_##NAME##_body;


typedef enum img_errors{
	SUCCESS,
	EOPENFILE,
	EREAD,
	EWRITE,
	ENOIMAGE,
	EWRONGHEAD
} img_errors;


