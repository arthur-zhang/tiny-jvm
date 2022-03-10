#pragma once

#include "shared/types.h"
#include "class_reader.h"
#include "data_output_stream.hpp"

class ExceptionTable {
public:
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;

    ExceptionTable(ClassReader &reader);

    void dump(DataOutputStream &os);
};