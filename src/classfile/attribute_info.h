//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_ATTRIBUTE_INFO_H
#define TINY_JVM_ATTRIBUTE_INFO_H

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

    static AttributeInfo *readAttribute(ClassReader &reader, ConstantPool *constant_pool);

    static u2 attributeName2Tag(u2 attribute_name_index,
                                ConstantPool *constant_pool);
};


class ConstantValueAttribute : public AttributeInfo {
public:
    ConstantValueAttribute(ClassReader &reader);

    u2 constant_value_index;
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

    virtual ~CodeAttribute();
};

class StackMapTableAttribute : public AttributeInfo {
public:
    StackMapTableAttribute(ClassReader &reader);

    virtual ~StackMapTableAttribute();

private:
    u1 *bytes;
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
};

class SyntheticAttribute : public AttributeInfo {
public:
    SyntheticAttribute(ClassReader &reader) : AttributeInfo(reader) {}
};

class SignatureAttribute : public AttributeInfo {
public:
    u2 signature_index;

    SignatureAttribute(ClassReader &reader) : AttributeInfo(reader) {
        signature_index = reader.readUInt16();
    }
};

class SourceFileAttribute : public AttributeInfo {
public:
    u2 source_file_index;

    SourceFileAttribute(ClassReader &reader);
};

class SourceDebugExtensionAttribute : public AttributeInfo {
public:
    u1 *debug_extension = nullptr;        // [attribute_length];
    SourceDebugExtensionAttribute(ClassReader &reader) : AttributeInfo(reader) {
        if (attribute_length != 0)
            debug_extension = new u1[attribute_length];
        debug_extension = reader.readBytes(attribute_length);
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
};

class LocalVariable {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
public:
    LocalVariable(ClassReader &reader) {
        start_pc = reader.readUInt16();
        length = reader.readUInt16();
        name_index = reader.readUInt16();
        descriptor_index = reader.readUInt16();
        index = reader.readUInt16();
    }

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
};

class LocalVariableTypeTableAttribute : public AttributeInfo {
public:
    u2 local_variable_type_table_length;
    LocalVariableType **local_variable_type_table = nullptr;// [local_variable_type_table_length]
    LocalVariableTypeTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        local_variable_type_table_length = reader.readUInt16();
        if (local_variable_type_table_length != 0)
            local_variable_type_table = new LocalVariableType *[local_variable_type_table_length];
        for (int pos = 0; pos < local_variable_type_table_length; pos++) {
            local_variable_type_table[pos] = new LocalVariableType(reader);
        }
    }

};

class DeprecatedAttribute : public AttributeInfo {
public:
    DeprecatedAttribute(ClassReader &reader) : AttributeInfo(reader) {
        reader.readBytes(attribute_length);
    }
};

class parameter_annotations_t {    // extract from Runtime_XXX_Annotations_attributes
public:
    parameter_annotations_t(ClassReader &reader) {
        num_annotations = reader.readUInt16();
        if (num_annotations != 0)
            annotations = new AnnotationElementValue *[num_annotations];
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos] = new AnnotationElementValue(reader, '@'); // todo
        }
    }

    u2 num_annotations;
    AnnotationElementValue **annotations = nullptr;    // [num_annotations]
};

class RuntimeVisibleAnnotations_attribute : public AttributeInfo {
public:
    parameter_annotations_t parameter_annotations;

    RuntimeVisibleAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader), parameter_annotations(reader) {
    }
};

class RuntimeInvisibleAnnotations_attribute : public AttributeInfo {
public:
    parameter_annotations_t *parameter_annotations;

    RuntimeInvisibleAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        parameter_annotations = new parameter_annotations_t(reader);
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
};

struct type_annotation {
    // target_type
    struct target_info_t {

        virtual ~target_info_t() {}
    };

    struct type_parameter_target : target_info_t {
        u1 type_parameter_index;

        friend std::istream &operator>>(std::istream &f, type_annotation::type_parameter_target &i);
    };

    struct supertype_target : target_info_t {
        u2 supertype_index;

        friend std::istream &operator>>(std::istream &f, type_annotation::supertype_target &i);
    };

    struct type_parameter_bound_target : target_info_t {
        u1 type_parameter_index;
        u1 bound_index;

        friend std::istream &operator>>(std::istream &f, type_annotation::type_parameter_bound_target &i);
    };

    struct empty_target : target_info_t {
        friend std::istream &operator>>(std::istream &f, type_annotation::empty_target &i);
    };

    struct formal_parameter_target : target_info_t {
        u1 formal_parameter_index;

        friend std::istream &operator>>(std::istream &f, type_annotation::formal_parameter_target &i);
    };

    struct throws_target : target_info_t {
        u2 throws_type_index;

        friend std::istream &operator>>(std::istream &f, type_annotation::throws_target &i);
    };

    struct localvar_target : target_info_t {
        u2 table_length;

        struct table_t {
            u2 start_pc;
            u2 length;
            u2 index;

            friend std::istream &operator>>(std::istream &f, type_annotation::localvar_target::table_t &i);

        } *table = nullptr;                // [table_length];
        friend std::istream &operator>>(std::istream &f, type_annotation::localvar_target &i);

        ~localvar_target();
    };

    struct catch_target : target_info_t {
        u2 exception_table_index;

        friend std::istream &operator>>(std::istream &f, type_annotation::catch_target &i);
    };

    struct offset_target : target_info_t {
        u2 offset;

        friend std::istream &operator>>(std::istream &f, type_annotation::offset_target &i);
    };

    struct type_argument_target : target_info_t {
        u2 offset;
        u1 type_argument_index;

        friend std::istream &operator>>(std::istream &f, type_annotation::type_argument_target &i);
    };

    // type_path
    struct type_path {
        u1 path_length;

        struct path_t {
            u1 type_path_kind;
            u1 type_argument_index;

            friend std::istream &operator>>(std::istream &f, type_annotation::type_path::path_t &i);

        } *path = nullptr;                // [path_length];
        friend std::istream &operator>>(std::istream &f, type_annotation::type_path &i);

        ~type_path();

    };

    // basic
    u1 target_type;
    target_info_t *target_info = nullptr;    // [1]
    type_path target_path;
    AnnotationElementValue *anno = nullptr;                // [1]

    friend std::istream &operator>>(std::istream &f, type_annotation &i);

    ~type_annotation();

};

class RuntimeVisibleTypeAnnotations_attribute : public AttributeInfo {
public:
    RuntimeVisibleTypeAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        reader.readBytes(attribute_length);
    }

    u2 num_annotations;
    type_annotation *annotations = nullptr;                    // [num_annotations];
};

class RuntimeInvisibleTypeAnnotations_attribute : public AttributeInfo {
public:
    RuntimeInvisibleTypeAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        reader.readBytes(attribute_length);
    }

//    u2 num_annotations;
////    type_annotation *annotations = nullptr;					// [num_annotations];
//    friend std::istream & operator >> (std::istream & f, RuntimeInvisibleTypeAnnotations_attribute & i);
//    ~RuntimeInvisibleTypeAnnotations_attribute();
};

/**
 * Represents the default value of a annotation for a method info
 */
class AnnotationDefaultAttribute : public AttributeInfo {
public:
    AnnotationDefaultAttribute(ClassReader &reader) : AttributeInfo(reader) {
        default_value = ElementValue::readElementValue(reader);
    }

    ElementValue *default_value;

    virtual ~AnnotationDefaultAttribute();
};

class BootstrapMethod {
public:
    u2 bootstrap_method_ref;
    u2 num_bootstrap_arguments;
    u2 *bootstrap_arguments = nullptr;

    BootstrapMethod(ClassReader &reader);

    virtual ~BootstrapMethod();
};

class BootstrapMethodsAttribute : public AttributeInfo {
public:
    BootstrapMethodsAttribute(ClassReader &reader);

    u2 num_bootstrap_methods;
    BootstrapMethod **bootstrap_methods = nullptr;                            // [num_bootstrap_methods];
private:
    virtual ~BootstrapMethodsAttribute();
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
};


#endif //TINY_JVM_ATTRIBUTE_INFO_H
