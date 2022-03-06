//
// Created by ya on 2022/3/6.
//
#pragma once

#include "attribute_info.h"
#include "bootstrap_method.h"

class BootstrapMethodsAttribute : public AttributeInfo {
public:
    BootstrapMethodsAttribute(ClassReader &reader);

    u2 num_bootstrap_methods;
    BootstrapMethod **bootstrap_methods = nullptr;                            // [num_bootstrap_methods];

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(num_bootstrap_methods);
        for (int pos = 0; pos < num_bootstrap_methods; pos++) {
            bootstrap_methods[pos]->dump(os);
        }
    }

private:
    virtual ~BootstrapMethodsAttribute();
};

