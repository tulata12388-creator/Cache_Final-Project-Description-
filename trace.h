#ifndef TRACE_H
#define TRACE_H

#include "cache_config.h"

TraceType parse_trace_line(
    const char *line,
    uint32_t *address,
    int *has_address
);

const char *trace_type_name(TraceType type);

int read_text_file(const char *filename);

#endif