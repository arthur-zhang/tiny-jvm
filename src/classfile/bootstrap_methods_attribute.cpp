//
// Created by ya on 2022/3/6.
//

#include "bootstrap_methods_attribute.h"

BootstrapMethodsAttribute::BootstrapMethodsAttribute(ClassReader &reader) : AttributeInfo(reader) {
    num_bootstrap_methods = reader.readUInt16();
    if (num_bootstrap_methods != 0)
        bootstrap_methods = new BootstrapMethod *[num_bootstrap_methods];
    for (int pos = 0; pos < num_bootstrap_methods; pos++) {
        bootstrap_methods[pos] = new BootstrapMethod(reader);
    }
}

BootstrapMethodsAttribute::~BootstrapMethodsAttribute() {
    for (int i = 0; i < num_bootstrap_methods; ++i) {
        delete bootstrap_methods[i];
    }
    delete[]bootstrap_methods;
}
