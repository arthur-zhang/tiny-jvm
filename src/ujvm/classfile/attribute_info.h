#pragma once

#include "shared/types.h"
#include "class_reader.h"
#include "shared/string.h"
#include "constant_pool.hpp"
#include "exception_table.h"
#include "element_value.h"
#include "method_parameter.h"

class AttributeInfo {
public:
    u2 attribute_name_index;
    u4 attribute_length;

    AttributeInfo(ClassReader &reader) {
        attribute_name_index = reader.readUInt16();
        attribute_length = reader.readUInt32();
    }

    virtual ~AttributeInfo() = 0;

    virtual void dump(DataOutputStream &os) {
        os.writeUInt16(attribute_name_index);
        os.writeUInt32(attribute_length);
    }

    static AttributeInfo *readAttributeInfo(ClassReader &reader, ConstantPool *constant_pool);

    static u2 attributeName2Tag(u2 attribute_name_index,
                                ConstantPool *constant_pool);
};


class AnnotationDefaultAttribute : public AttributeInfo {
public:
    AnnotationDefaultAttribute(ClassReader &reader) : AttributeInfo(reader) {
        default_value = ElementValue::readElementValue(reader);
    }

    ElementValue *default_value;

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        default_value->dump(os);
    }

    ~AnnotationDefaultAttribute() {
        delete default_value;
    }
};


class BootstrapMethod {
public:
    u2 bootstrap_method_ref;
    u2 num_bootstrap_arguments;
    u2 *bootstrap_arguments = nullptr;

    BootstrapMethod(ClassReader &reader);

    void dump(DataOutputStream &os);

    virtual ~BootstrapMethod();
};

class BootstrapMethodsAttribute : public AttributeInfo {
public:
    BootstrapMethodsAttribute(ClassReader &reader);

    u2 num_bootstrap_methods;
    BootstrapMethod **bootstrap_methods = nullptr;                            // [num_bootstrap_methods];
    void dump(DataOutputStream &os) override;

private:
    virtual ~BootstrapMethodsAttribute();
};

class CodeAttribute : public AttributeInfo {
public:
    u2 max_stack;
    u2 max_locals;

    u4 code_length;
    u1 *code;

    u2 exception_table_length;
    ExceptionTable **exception_table;

    u2 attributes_count;
    AttributeInfo **attributes;

    CodeAttribute(ClassReader &reader, ConstantPool *constantPool);

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(max_stack);
        os.writeUInt16(max_locals);
        os.writeUInt32(code_length);
        os.writeBytes(code, code_length);
        os.writeUInt16(exception_table_length);
        for (int i = 0; i < exception_table_length; i++) {
            exception_table[i]->dump(os);
        }
        os.writeUInt16(attributes_count);
        for (int i = 0; i < attributes_count; i++) {
            attributes[i]->dump(os);
        }
    }

    virtual ~CodeAttribute();
};