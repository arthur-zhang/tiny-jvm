//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_INNER_CLASSES_ATTRIBUTE_H
#define TINY_JVM_INNER_CLASSES_ATTRIBUTE_H

#include "attribute_info.h"
#include "inner_class.h"

class InnerClassesAttribute : public AttributeInfo {
public:
    u2 number_of_classes;
    InnerClass **classes = nullptr;

    InnerClassesAttribute(ClassReader &reader) : AttributeInfo(reader) {
        number_of_classes = reader.readUInt16();
        if (number_of_classes != 0)
            classes = new InnerClass *[number_of_classes];
        for (int pos = 0; pos < number_of_classes; pos++) {
            classes[pos] = new InnerClass(reader);
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(number_of_classes);
        for (int i = 0; i < number_of_classes; i++) {
            classes[i]->dump(os);
        }
    }

    virtual ~InnerClassesAttribute() {
        for (int i = 0; i < number_of_classes; ++i) {
            delete classes[i];
        }
        delete[]classes;
    }
};


#endif //TINY_JVM_INNER_CLASSES_ATTRIBUTE_H
