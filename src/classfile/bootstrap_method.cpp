//
// Created by ya on 2022/3/6.
//

#include "bootstrap_method.h"

BootstrapMethod::BootstrapMethod(ClassReader &reader) {
    bootstrap_method_ref = reader.readUInt16();
    num_bootstrap_arguments = reader.readUInt16();
    if (num_bootstrap_arguments != 0)
        bootstrap_arguments = new u2[num_bootstrap_arguments];
    for (int pos = 0; pos < num_bootstrap_arguments; pos++) {
        bootstrap_arguments[pos] = reader.readUInt16();
    }
}

BootstrapMethod::~BootstrapMethod() {
    if (num_bootstrap_arguments > 0) delete[]bootstrap_arguments;
}

void BootstrapMethod::dump(DataOutputStream &os) {
    os.writeUInt16(bootstrap_method_ref);
    os.writeUInt16(num_bootstrap_arguments);
    for (int i = 0; i < num_bootstrap_arguments; i++) {
        os.writeUInt16(bootstrap_arguments[i]);
    }
}
