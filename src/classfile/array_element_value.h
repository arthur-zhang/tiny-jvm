
#pragma once

#include "element_value.h"

class ArrayElementValue : public ElementValue {
public:
    u2 num_values;
    ElementValue **values = nullptr;

    ArrayElementValue(ClassReader &reader, u1 tag);

    void dump(DataOutputStream &os) override {
        ElementValue::dump(os);
        os.writeUInt16(num_values);
        for (int pos = 0; pos < num_values; pos++) {
            values[pos]->dump(os);
        }
    }
};

