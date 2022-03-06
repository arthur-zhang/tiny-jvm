#pragma once

#include "class_reader.h"
#include "types.hpp"
#include "data_output_stream.hpp"

class BootstrapMethod {
public:
    u2 bootstrap_method_ref;
    u2 num_bootstrap_arguments;
    u2 *bootstrap_arguments = nullptr;

    BootstrapMethod(ClassReader &reader);

    void dump(DataOutputStream &os);

    virtual ~BootstrapMethod();
};

