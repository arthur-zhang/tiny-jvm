#pragma once

#include "shared/types.h"
#include "class_reader.h"
#include "shared/jstring.h"
#include "constant_pool.h"
#include "element_value.h"
#include "annotations.h"
#include "data_output_stream.h"

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

class ExceptionTable {
public:
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;

    ExceptionTable(ClassReader &reader);

    void dump(DataOutputStream &os);
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

class ConstantValueAttribute : public AttributeInfo {
public:
    ConstantValueAttribute(ClassReader &reader);

    u2 constant_value_index;

    void dump(DataOutputStream &os) override;
};

class DeprecatedAttribute : public AttributeInfo {
public:
    u1 *bytes;

    DeprecatedAttribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeBytes(bytes, attribute_length);
    }

    virtual ~DeprecatedAttribute() {
        if (attribute_length > 0) delete[]bytes;
    }
};

class EnclosingMethodAttribute : public AttributeInfo {
public:
    u2 class_index;
    u2 method_index;

    EnclosingMethodAttribute(ClassReader &reader) : AttributeInfo(reader) {
        class_index = reader.readUInt16();
        method_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(class_index);
        os.writeUInt16(method_index);
    }
};

class InnerClass {
public:
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;

    InnerClass(ClassReader &reader) {
        inner_class_info_index = reader.readUInt16();
        outer_class_info_index = reader.readUInt16();
        inner_name_index = reader.readUInt16();
        inner_class_access_flags = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(inner_class_info_index);
        os.writeUInt16(outer_class_info_index);
        os.writeUInt16(inner_name_index);
        os.writeUInt16(inner_class_access_flags);
    }
};

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

    ~InnerClassesAttribute() {
        for (int i = 0; i < number_of_classes; ++i) {
            delete classes[i];
        }
        delete[]classes;
    }
};

class ExceptionTableAttribute : public AttributeInfo {
public:
    u2 number_of_exceptions;
    u2 *exception_index_table = nullptr;

    ExceptionTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        number_of_exceptions = reader.readUInt16();
        if (number_of_exceptions != 0)
            exception_index_table = new u2[number_of_exceptions];
        for (int pos = 0; pos < number_of_exceptions; pos++) {
            exception_index_table[pos] = reader.readUInt16();
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(number_of_exceptions);
        for (int i = 0; i < number_of_exceptions; i++) {
            os.writeUInt16(exception_index_table[i]);
        }
    }

    ~ExceptionTableAttribute() {
        if (number_of_exceptions > 0) delete[]exception_index_table;
    }
};

class LineNumber {
public:
    u2 start_pc;
    u2 line_number;

    LineNumber(ClassReader &reader) {
        start_pc = reader.readUInt16();
        line_number = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(line_number);
    };
};

class LineNumberTableAttribute : public AttributeInfo {
public:
    u2 line_number_table_length;

    LineNumber **line_number_table = nullptr;        // [line_number_table_length]

    LineNumberTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        line_number_table_length = reader.readUInt16();
        if (line_number_table_length != 0)
            line_number_table = new LineNumber *[line_number_table_length];
        for (int pos = 0; pos < line_number_table_length; pos++) {
            line_number_table[pos] = new LineNumber(reader);
        }
    }

    virtual ~LineNumberTableAttribute() {
        if (line_number_table_length > 0) {
            for (int pos = 0; pos < line_number_table_length; pos++) {
                delete line_number_table[pos];
            }
            delete[]line_number_table;
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(line_number_table_length);
        for (int i = 0; i < line_number_table_length; i++) {
            line_number_table[i]->dump(os);
        }
    }
};

class LocalVariableType {
public:
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 signature_index;
    u2 index;

    LocalVariableType(ClassReader &reader) {
        start_pc = reader.readUInt16();
        length = reader.readUInt16();
        name_index = reader.readUInt16();
        signature_index = reader.readUInt16();
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(length);
        os.writeUInt16(name_index);
        os.writeUInt16(signature_index);
        os.writeUInt16(index);
    }
};

class LocalVariableTypeTableAttribute : public AttributeInfo {
public:
    u2 local_variable_type_table_length;
    LocalVariableType **local_variable_type_table;

    LocalVariableTypeTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        local_variable_type_table_length = reader.readUInt16();
        if (local_variable_type_table_length != 0)
            local_variable_type_table = new LocalVariableType *[local_variable_type_table_length];
        for (int pos = 0; pos < local_variable_type_table_length; pos++) {
            local_variable_type_table[pos] = new LocalVariableType(reader);
        }
    }

    virtual ~LocalVariableTypeTableAttribute() {
        if (local_variable_type_table_length > 0) {
            for (int pos = 0; pos < local_variable_type_table_length; pos++) {
                delete local_variable_type_table[pos];
            }
            delete[]local_variable_type_table;
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(local_variable_type_table_length);
        for (int pos = 0; pos < local_variable_type_table_length; pos++) {
            local_variable_type_table[pos]->dump(os);
        }
    }
};

class LocalVariable {
public:
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;

    LocalVariable(ClassReader &reader) {
        start_pc = reader.readUInt16();
        length = reader.readUInt16();
        name_index = reader.readUInt16();
        descriptor_index = reader.readUInt16();
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(length);
        os.writeUInt16(name_index);
        os.writeUInt16(descriptor_index);
        os.writeUInt16(index);
    };
};

class LocalVariableTableAttribute : public AttributeInfo {
public:
    u2 local_variable_table_length;

    LocalVariable **local_variable_table = nullptr;    // [local_variable_table_length]
    LocalVariableTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        local_variable_table_length = reader.readUInt16();
        if (local_variable_table_length != 0)
            local_variable_table = new LocalVariable *[local_variable_table_length];
        for (int pos = 0; pos < local_variable_table_length; pos++) {
            local_variable_table[pos] = new LocalVariable(reader);
        }
    }

    virtual ~LocalVariableTableAttribute() {
        if (local_variable_table_length > 0) {
            for (int pos = 0; pos < local_variable_table_length; pos++) {
                delete local_variable_table[pos];
            }
            delete[]local_variable_table;
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(local_variable_table_length);
        for (int pos = 0; pos < local_variable_table_length; pos++) {
            local_variable_table[pos]->dump(os);
        }
    }
};

/**
 * Entry of the parameters table.
 *
 * @see <a href="https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.24">
 * The class File Format : The MethodParameters Attribute</a>
 */
class MethodParameter {
public:
    u2 name_index;
    u2 access_flags;

    MethodParameter(ClassReader &reader) {
        name_index = reader.readUInt16();
        access_flags = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(name_index);
        os.writeUInt16(access_flags);
    }
};

/**
 * This class represents a MethodParameters attribute.
 *
 * @see <a href="https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.24">
 MethodParameters_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 parameters_count;
    {   u2 name_index;
        u2 access_flags;
    } parameters[parameters_count];
}
 */
class MethodParametersAttribute : public AttributeInfo {
public:
    u1 parameters_count;
    MethodParameter **parameters = nullptr;                                    // [parameters_count];

    MethodParametersAttribute(ClassReader &reader);

    ~MethodParametersAttribute();

    void dump(DataOutputStream &os) override {
        os.writeUInt8(parameters_count);
        for (int i = 0; i < parameters_count; ++i) {
            parameters[i]->dump(os);
        }
    }
};

class RuntimeInvisibleAnnotations_attribute : public AttributeInfo {
public:
    Annotations *parameter_annotations;

    RuntimeInvisibleAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        parameter_annotations = new Annotations(reader);
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        parameter_annotations->dump(os);
    }

    virtual ~RuntimeInvisibleAnnotations_attribute() {
        delete parameter_annotations;
    }
};

class RuntimeInvisibleParameterAnnotations_attribute : public AttributeInfo {
public:
    u1 num_parameters;
    Annotations **parameter_annotations = nullptr;        // [num_parameters];

    ~RuntimeInvisibleParameterAnnotations_attribute() {
        for (int pos = 0; pos < num_parameters; pos++) {
            delete parameter_annotations[pos];
        }
        delete parameter_annotations;
    }

    RuntimeInvisibleParameterAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        num_parameters = reader.readUint8();
        if (num_parameters != 0)
            parameter_annotations = new Annotations *[num_parameters];
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos] = new Annotations(reader);
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt8(num_parameters);
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos]->dump(os);
        }
    }
};

class RuntimeInvisibleTypeAnnotationsAttribute : public AttributeInfo {
public:
    u1 *bytes;

    RuntimeInvisibleTypeAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    void dump(DataOutputStream &os) override {
        os.writeBytes(bytes, attribute_length);
    }

    virtual ~RuntimeInvisibleTypeAnnotationsAttribute() {
        if (attribute_length <= 0) return;
        delete[]bytes;
    }
};

class RuntimeVisibleAnnotationsAttribute : public AttributeInfo {
public:
    Annotations *parameter_annotations;

    RuntimeVisibleAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        parameter_annotations = new Annotations(reader);
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        parameter_annotations->dump(os);
    }
};

class RuntimeVisibleParameterAnnotationsAttribute : public AttributeInfo {
public:
    u1 num_parameters;
    Annotations **parameter_annotations = nullptr;        // [num_parameters];

    RuntimeVisibleParameterAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        num_parameters = reader.readUint8();
        if (num_parameters != 0)
            parameter_annotations = new Annotations *[num_parameters];
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos] = new Annotations(reader);
        }
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(num_parameters);
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos]->dump(os);
        }
    }
};

class RuntimeVisibleTypeAnnotationsAttribute : public AttributeInfo {
public:
    u1 *bytes;

    RuntimeVisibleTypeAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    u2 num_annotations;

//    type_annotation *annotations = nullptr;                    // [num_annotations];
    void dump(DataOutputStream &os) override {
        os.writeBytes(bytes, attribute_length);
    }

    virtual ~RuntimeVisibleTypeAnnotationsAttribute() {
        if (attribute_length <= 0) return;
        delete[]bytes;
    }
};

class SignatureAttribute : public AttributeInfo {
public:
    u2 signature_index;

    SignatureAttribute(ClassReader &reader) : AttributeInfo(reader) {
        signature_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(signature_index);
    }
};

class SourceDebugExtensionAttribute : public AttributeInfo {
public:
    u1 *debug_extension = nullptr;

    SourceDebugExtensionAttribute(ClassReader &reader) : AttributeInfo(reader) {
        if (attribute_length != 0) {
            debug_extension = new u1[attribute_length];
            debug_extension = reader.readBytes(attribute_length);
        }
    }

    virtual ~SourceDebugExtensionAttribute() {
        if (attribute_length <= 0) return;
        delete[]debug_extension;
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        if (attribute_length > 0) {
            os.writeBytes(debug_extension, attribute_length);
        }
    }
};

class SourceFileAttribute : public AttributeInfo {
public:
    u2 source_file_index;

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(source_file_index);
    }

    SourceFileAttribute(ClassReader &reader) : AttributeInfo(reader) {
        source_file_index = reader.readUInt16();
    }

    virtual ~SourceFileAttribute() = default;
};

class StackMapTableAttribute : public AttributeInfo {
public:
    StackMapTableAttribute(ClassReader &reader);

    void dump(DataOutputStream &os) override;

    virtual ~StackMapTableAttribute();

private:
    u1 *bytes;
};

class SyntheticAttribute : public AttributeInfo {
public:
    u1 *bytes;

    SyntheticAttribute(ClassReader &reader) : AttributeInfo(reader) {
        if (attribute_length <= 0) return;
        bytes = reader.readBytes(attribute_length);
    }

    virtual ~SyntheticAttribute() {
        if (attribute_length <= 0) return;
        delete[]bytes;
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        if (attribute_length > 0) {
            os.writeBytes(bytes, attribute_length);
        }
    }
};
