#pragma once

#include "types.hpp"
#include "class_reader.h"
#include "String.h"
#include "constant_pool.hpp"
#include "exception_table.h"
#include "element_value.h"

class AttributeInfo {
public:
    u2 attribute_name_index;
    u4 attribute_length;

    AttributeInfo(ClassReader &reader);

    ~AttributeInfo() {}//todo

    virtual void dump(DataOutputStream &os) = 0;

    static AttributeInfo *readAttribute(ClassReader &reader, ConstantPool *constant_pool);

    static u2 attributeName2Tag(u2 attribute_name_index,
                                ConstantPool *constant_pool);
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

    virtual ~ExceptionTableAttribute() {
        delete[]exception_index_table;
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

    virtual ~InnerClassesAttribute() {
        for (int i = 0; i < number_of_classes; ++i) {
            delete classes[i];
        }
        delete[]classes;
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

class SyntheticAttribute : public AttributeInfo {
public:
    u1 *bytes;

    SyntheticAttribute(ClassReader &reader) : AttributeInfo(reader) {
        if (attribute_length > 0) {
            bytes = reader.readBytes(attribute_length);
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        if (attribute_length > 0) {
            os.writeBytes(bytes, attribute_length);
        }
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

class SourceFileAttribute : public AttributeInfo {
public:
    u2 source_file_index;

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(source_file_index);
    }

    SourceFileAttribute(ClassReader &reader);
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

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        if (attribute_length > 0) {
            os.writeBytes(debug_extension, attribute_length);
        }
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

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(line_number_table_length);
        for (int i = 0; i < line_number_table_length; i++) {
            line_number_table[i]->dump(os);
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

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(local_variable_table_length);
        for (int pos = 0; pos < local_variable_table_length; pos++) {
            local_variable_table[pos]->dump(os);
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

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(local_variable_type_table_length);
        for (int pos = 0; pos < local_variable_type_table_length; pos++) {
            local_variable_type_table[pos]->dump(os);
        }
    }
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
};

class parameter_annotations_t {
public:
    parameter_annotations_t(ClassReader &reader) {
        num_annotations = reader.readUInt16();
        if (num_annotations != 0)
            annotations = new AnnotationEntry *[num_annotations];
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos] = new AnnotationEntry(reader); // todo
        }
    }

    u2 num_annotations;
    AnnotationEntry **annotations = nullptr;    // [num_annotations]

    void dump(DataOutputStream &os) {
        os.writeUInt16(num_annotations);
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos]->dump(os);
        }
    }
};

class RuntimeInvisibleAnnotations_attribute : public AttributeInfo {
public:
    parameter_annotations_t *parameter_annotations;

    RuntimeInvisibleAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        parameter_annotations = new parameter_annotations_t(reader);
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        parameter_annotations->dump(os);
    }
};

class RuntimeVisibleParameterAnnotations_attribute : public AttributeInfo {
public:
    u1 num_parameters;
    parameter_annotations_t **parameter_annotations = nullptr;        // [num_parameters];

    RuntimeVisibleParameterAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        num_parameters = reader.readUint8();
        if (num_parameters != 0)
            parameter_annotations = new parameter_annotations_t *[num_parameters];
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos] = new parameter_annotations_t(reader);
        }
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(num_parameters);
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos]->dump(os);
        }
    }
};

class RuntimeInvisibleParameterAnnotations_attribute : public AttributeInfo {
public:
    u1 num_parameters;
    parameter_annotations_t **parameter_annotations = nullptr;        // [num_parameters];

    ~RuntimeInvisibleParameterAnnotations_attribute();

    RuntimeInvisibleParameterAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        num_parameters = reader.readUint8();
        if (num_parameters != 0)
            parameter_annotations = new parameter_annotations_t *[num_parameters];
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos] = new parameter_annotations_t(reader);
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

class target_info_t {
public:
    target_info_t(ClassReader &reader);

    virtual void dump(DataOutputStream &os);

    virtual ~target_info_t() {}
};

class type_parameter_target : target_info_t {
public:
    u1 type_parameter_index;

    type_parameter_target(ClassReader &reader) : target_info_t(reader) {
        type_parameter_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(type_parameter_index);
    }


};

class supertype_target : target_info_t {
public:
    u2 supertype_index;

    supertype_target(ClassReader &reader) : target_info_t(reader) {
        supertype_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(supertype_index);
    }
};

class type_parameter_bound_target : target_info_t {
public:
    u1 type_parameter_index;
    u1 bound_index;

    type_parameter_bound_target(ClassReader &reader) : target_info_t(reader) {
        type_parameter_index = reader.readUint8();
        bound_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(type_parameter_index);
        os.writeUInt8(bound_index);
    }
};

class empty_target : target_info_t {
};

class formal_parameter_target : target_info_t {

public:
    u1 formal_parameter_index;

    formal_parameter_target(ClassReader &reader) : target_info_t(reader) {
        formal_parameter_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(formal_parameter_index);
    }
};

class throws_target : target_info_t {
public:
    u2 throws_type_index;

    throws_target(ClassReader &reader) : target_info_t(reader) {
        throws_type_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(throws_type_index);
    }
};

class table_t {
public:
    u2 start_pc;
    u2 length;
    u2 index;

    table_t(ClassReader &reader) {
        start_pc = reader.readUInt16();
        length = reader.readUInt16();
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(length);
        os.writeUInt16(index);
    }
};

class localvar_target : target_info_t {
public:
    u2 table_length;
    table_t *table = nullptr;                // [table_length];
    localvar_target(ClassReader &reader) : target_info_t(reader) {
        table_length = reader.readUInt16();
        table = new table_t(reader);
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(table_length);
        table->dump(os);
    }
};

class catch_target : target_info_t {
public:
    u2 exception_table_index;

    catch_target(ClassReader &reader) : target_info_t(reader) {
        exception_table_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(exception_table_index);
    }

};

class offset_target : target_info_t {
public:
    u2 offset;

    offset_target(ClassReader &reader) : target_info_t(reader) {
        offset = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(offset);
    }
};

class type_argument_target : target_info_t {
public:
    u2 offset;
    u1 type_argument_index;

    type_argument_target(ClassReader &reader) : target_info_t(reader) {
        offset = reader.readUInt16();
        type_argument_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(offset);
        os.writeUInt16(type_argument_index);
    }
};

class path_t {
public:
    u1 type_path_kind;
    u1 type_argument_index;

    path_t(ClassReader &reader) {
        type_path_kind = reader.readUint8();
        type_argument_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt8(type_path_kind);
        os.writeUInt8(type_argument_index);
    }
};


class type_path {
public:
    u1 path_length;


    path_t *path = nullptr;                // [path_length];

    type_path(ClassReader &reader) {
        path_length = reader.readUint8();
        path = new path_t(reader);
    }

    void dump(DataOutputStream &os) {
        os.writeUInt8(path_length);
        path->dump(os);
    }
};

class type_annotation {

public:
    u1 target_type;
    target_info_t *target_info = nullptr;    // [1]
    type_path *target_path;
    AnnotationEntry *anno = nullptr;                // [1]

    type_annotation(ClassReader &reader) {
        target_type = reader.readUint8();
        target_info = new target_info_t(reader);
        target_path = new type_path(reader);
        anno = new AnnotationEntry(reader);
    }

    ~type_annotation();

};

class RuntimeVisibleTypeAnnotations_attribute : public AttributeInfo {
public:
    u1 *bytes;

    RuntimeVisibleTypeAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    u2 num_annotations;
    type_annotation *annotations = nullptr;                    // [num_annotations];
    void dump(DataOutputStream &os) override {
        os.writeBytes(bytes, attribute_length);
    }
};

// todo
class RuntimeInvisibleTypeAnnotations_attribute : public AttributeInfo {
public:
    u1 *bytes;

    RuntimeInvisibleTypeAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    void dump(DataOutputStream &os) override {
        os.writeBytes(bytes, attribute_length);
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


