#pragma once

#include "attribute_info.h"
#include "bootstrap_method.h"

class BootstrapMethodsAttribute : public AttributeInfo {
public:
    BootstrapMethodsAttribute(ClassReader &reader);

    u2 num_bootstrap_methods;
    BootstrapMethod **bootstrap_methods = nullptr;                            // [num_bootstrap_methods];
    void dump(DataOutputStream &os) override;

private:
    virtual ~BootstrapMethodsAttribute();
};

