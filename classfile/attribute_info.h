//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_ATTRIBUTE_INFO_H
#define TINY_JVM_ATTRIBUTE_INFO_H

#include "types.h"
#include "class_reader.h"
#include "String.h"
#include "constant_pool.h"
#include "exception_table.h"

class AttributeInfo {
public:
    u2 attribute_name_index;
    u4 attribute_length;

    AttributeInfo(ClassReader &reader);

    static AttributeInfo *parseAttribute(ClassReader &reader, ConstantPool *constant_pool);

    static u2 toAttributeTag(u2 attribute_name_index,
                             ConstantPool *constant_pool);
};


class ConstantValueAttribute : public AttributeInfo {
public:
    ConstantValueAttribute(ClassReader &reader) : AttributeInfo(reader) {
        constant_value_index = reader.readUint16();
    }

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

    CodeAttribute(ClassReader &reader, ConstantPool *constantPool) : AttributeInfo(reader) {
        max_stack = reader.readUint16();
        max_locals = reader.readUint16();
        code_length = reader.readUint32();
        if (code_length != 0) {
            code = new u1[code_length];
            code = reader.readBytes(code_length);
        }
        exception_table_length = reader.readUint16();
        if (exception_table_length != 0) {
            exception_table = new ExceptionTable *[exception_table_length];
            for (int pos = 0; pos < exception_table_length; pos++) {
                exception_table[pos] = new ExceptionTable(reader);
            }
        }
        attributes_count = reader.readUint16();
        if (attributes_count != 0)
            attributes = new AttributeInfo *[attributes_count];
        for (int pos = 0; pos < attributes_count; pos++) {
            attributes[pos] = parseAttribute(reader, constantPool);
        }
    }
};

class StackMapTableAttribute : public AttributeInfo {
public:
    StackMapTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        reader.readBytes(attribute_length);
    }
};

class ExceptionsAttribute : public AttributeInfo {
public:
    u2 number_of_exceptions;
    u2 *exception_index_table = nullptr;

    ExceptionsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        number_of_exceptions = reader.readUint16();
        if (number_of_exceptions != 0)
            exception_index_table = new u2[number_of_exceptions];
        for (int pos = 0; pos < number_of_exceptions; pos++) {
            exception_index_table[pos] = reader.readUint16();
        }
    }
};

class InnerClass {
public:
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;

    InnerClass(ClassReader &reader) {
        inner_class_info_index = reader.readUint16();
        outer_class_info_index = reader.readUint16();
        inner_name_index = reader.readUint16();
        inner_class_access_flags = reader.readUint16();
    }
};

class InnerClassesAttribute : public AttributeInfo {
public:
    u2 number_of_classes;
    InnerClass **classes = nullptr;

    InnerClassesAttribute(ClassReader &reader) : AttributeInfo(reader) {
        number_of_classes = reader.readUint16();
        if (number_of_classes != 0)
            classes = new InnerClass *[number_of_classes];
        for (int pos = 0; pos < number_of_classes; pos++) {
            classes[pos] = new InnerClass(reader);
        }
    }

};

class EnclosingMethodAttribute : public AttributeInfo {
public:
    u2 class_index;
    u2 method_index;

    EnclosingMethodAttribute(ClassReader &reader) : AttributeInfo(reader) {
        class_index = reader.readUint16();
        method_index = reader.readUint16();
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
        signature_index = reader.readUint16();
    }
};

class SourceFileAttribute : public AttributeInfo {
public:
    u2 sourcefile_index;

    SourceFileAttribute(ClassReader &reader) : AttributeInfo(reader) {
        sourcefile_index = reader.readUint16();
    }
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
        start_pc = reader.readUint16();
        line_number = reader.readUint16();
    }
};

class LineNumberTableAttribute : public AttributeInfo {
public:
    u2 line_number_table_length;

    LineNumber **line_number_table = nullptr;        // [line_number_table_length]

    LineNumberTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        line_number_table_length = reader.readUint16();
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
        start_pc = reader.readUint16();
        length = reader.readUint16();
        name_index = reader.readUint16();
        descriptor_index = reader.readUint16();
        index = reader.readUint16();
    }

};

class LocalVariableTableAttribute : public AttributeInfo {
public:
    u2 local_variable_table_length;

    LocalVariable **local_variable_table = nullptr;    // [local_variable_table_length]
    LocalVariableTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        local_variable_table_length = reader.readUint16();
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
        start_pc = reader.readUint16();
        length = reader.readUint16();
        name_index = reader.readUint16();
        signature_index = reader.readUint16();
        index = reader.readUint16();
    }
};

class LocalVariableTypeTableAttribute : public AttributeInfo {
public:
    u2 local_variable_type_table_length;
    LocalVariableType **local_variable_type_table = nullptr;// [local_variable_type_table_length]
    LocalVariableTypeTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        local_variable_type_table_length = reader.readUint16();
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


class CodeStub {
public:
    std::vector<u1> stub;

    void inject(u1 code) {
        stub.push_back(code);
    }

    void inject(u2 code) {
        inject((u1) ((code >> 8) & 0xFF));
        inject((u1) (code & 0xFF));
    }

    void inject(u4 code) {
        inject((u1) ((code >> 24) & 0xFF));
        inject((u1) ((code >> 16) & 0xFF));
        inject((u1) ((code >> 8) & 0xFF));
        inject((u1) (code & 0xFF));
    }

    void inject(u1 *bytes, int num) {
        for (int i = 0; i < num; i++) {
            inject((u1) bytes[i]);
        }
    }

    void inject(u2 *bytes, int num) {
        for (int i = 0; i < num; i++) {
            inject((u2) bytes[i]);
        }
    }

    CodeStub &operator+=(const CodeStub &rhs) {
        this->stub.insert(this->stub.end(), rhs.stub.begin(), rhs.stub.end());
        return *this;
    }

    CodeStub operator+(const CodeStub &rhs) {
        CodeStub tmp;
        tmp += *this;
        tmp += rhs;
        return tmp;
    }

    void print() {
        std::wcout << "===---------------- CodeStub -------------------===" << std::endl;
        for (int i = 0; i < stub.size(); i++) {
            std::wcout << std::hex << (unsigned) stub[i] << " ";
            if ((i + 1) % 8 == 0) std::wcout << std::endl;
        }
        std::wcout << std::endl;
        std::wcout << "===---------------------------------------------===" << std::endl;
    }
};

class ElementValue {
public:

    CodeStub stub;

    virtual ~ElementValue() {}
};

class ConstantElementValue : public ElementValue {
public:
    u2 const_value_index;

    ConstantElementValue(ClassReader &reader) {
        const_value_index = reader.readUint16();
        stub.inject(const_value_index);
    }
};

class ClassElementValue : public ElementValue {
public:
    u2 class_info_index;

    ClassElementValue(ClassReader &reader) {
        class_info_index = reader.readUint16();
        stub.inject(class_info_index);
    }

};

class EnumElementValue : public ElementValue {
public:
    EnumElementValue(ClassReader &reader) {
        type_name_index = reader.readUint16();
        const_name_index = reader.readUint16();
        stub.inject(type_name_index);
        stub.inject(const_name_index);
    }

    u2 type_name_index;
    u2 const_name_index;
};


class element_value {
public:
    u1 tag;
    ElementValue *value = nullptr;    // [1]
    CodeStub stub;

    element_value(ClassReader &reader);
};


class ArrayElementValue : public ElementValue {
public:
    u2 num_values;
    element_value **values = nullptr;        // [num_values]

    ArrayElementValue(ClassReader &reader) {
        num_values = reader.readUint16();
        if (num_values !=
            0)        // 这里写成了 values...... 本来就是 nullptr 是 0 ....... 结果调了一个小时...... 一直显示在下边 f >> values[pos] 进入函数中的第一行出错...... 唉（ 还以为是标准库错了（逃 我真是个白痴（打脸
            values = new element_value *[num_values];
        for (int pos = 0; pos < num_values; pos++) {
            values[pos] = new element_value(reader);
        }
        // CodeStub
        stub.inject(num_values);
        for (int pos = 0; pos < num_values; pos++) {
            stub += values[pos]->stub;
        }
    }
};

class ElementValuePair {
public:
    ElementValuePair(ClassReader &reader) {
        element_name_index = reader.readUint16();
//            f >> value;
        value = new element_value(reader);
        // CodeStub
        stub.inject(element_name_index);
        stub += value->stub;
    }

    u2 element_name_index;
    element_value *value;

    CodeStub stub;
};

class AnnotationElementValue : public ElementValue {
public:
    AnnotationElementValue(ClassReader &reader) : ElementValue() {
        type_index = reader.readUint16();
        num_element_value_pairs = reader.readUint16();
        if (num_element_value_pairs != 0)
            element_value_pairs = new ElementValuePair *[num_element_value_pairs];
        for (int pos = 0; pos < num_element_value_pairs; pos++) {
            element_value_pairs[pos] = new ElementValuePair(reader);
        }
        // CodeStub
        stub.inject(type_index);
        stub.inject(num_element_value_pairs);
        for (int pos = 0; pos < num_element_value_pairs; pos++) {
            stub += element_value_pairs[pos]->stub;
        }
    }

    u2 type_index;
    u2 num_element_value_pairs;

    ElementValuePair **element_value_pairs = nullptr;        // [num_element_value_pairs]


};


class parameter_annotations_t {    // extract from Runtime_XXX_Annotations_attributes
public:
    parameter_annotations_t(ClassReader &reader) {
        num_annotations = reader.readUint16();
        if (num_annotations != 0)
            annotations = new AnnotationElementValue *[num_annotations];
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos] = new AnnotationElementValue(reader);
        }
        // CodeStub
        stub.inject(num_annotations);
        for (int pos = 0; pos < num_annotations; pos++) {
            stub += annotations[pos]->stub;
        }
    }

    u2 num_annotations;
    AnnotationElementValue **annotations = nullptr;    // [num_annotations]


    CodeStub stub;
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
    CodeStub stub;

    RuntimeVisibleParameterAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        num_parameters = reader.readUint8();
        if (num_parameters != 0)
            parameter_annotations = new parameter_annotations_t *[num_parameters];
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos] = new parameter_annotations_t(reader);
        }
        // check
        int total_anno_length = 0;
        total_anno_length += 1;
        for (int pos = 0; pos < num_parameters; pos++) {
            total_anno_length += parameter_annotations[pos]->stub.stub.size();
        }
        assert(attribute_length == total_anno_length);
        // CodeStub
        stub.inject(num_parameters);
        for (int pos = 0; pos < num_parameters; pos++) {
            stub += parameter_annotations[pos]->stub;
        }
    }
};

class RuntimeInvisibleParameterAnnotations_attribute : public AttributeInfo {
public:
    u1 num_parameters;
    parameter_annotations_t **parameter_annotations = nullptr;        // [num_parameters];
    friend std::istream &operator>>(std::istream &f, RuntimeInvisibleParameterAnnotations_attribute &i);

    ~RuntimeInvisibleParameterAnnotations_attribute();

    RuntimeInvisibleParameterAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        num_parameters = reader.readUint8();
        if (num_parameters != 0)
            parameter_annotations = new parameter_annotations_t *[num_parameters];
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos] = new parameter_annotations_t(reader);
        }
        // check
        int total_anno_length = 0;
        total_anno_length += 1;
        for (int pos = 0; pos < num_parameters; pos++) {
            total_anno_length += parameter_annotations[pos]->stub.stub.size();
        }
        assert(attribute_length == total_anno_length);
    }
};

struct type_annotation {
    // target_type
    struct target_info_t {
        CodeStub stub;

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

            CodeStub stub;
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

            CodeStub stub;
        } *path = nullptr;                // [path_length];
        friend std::istream &operator>>(std::istream &f, type_annotation::type_path &i);

        ~type_path();

        CodeStub stub;
    };

    // basic
    u1 target_type;
    target_info_t *target_info = nullptr;    // [1]
    type_path target_path;
    AnnotationElementValue *anno = nullptr;                // [1]

    friend std::istream &operator>>(std::istream &f, type_annotation &i);

    ~type_annotation();

    CodeStub stub;
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

class AnnotationDefault_attribute : public AttributeInfo {
public:
    AnnotationDefault_attribute(ClassReader &reader) : AttributeInfo(reader) {
        default_value = new element_value(reader);
        stub += default_value->stub;
    }

    element_value *default_value;

    CodeStub stub;
};

class BootstrapMethods_attribute : public AttributeInfo {
public:
    BootstrapMethods_attribute(ClassReader &reader) : AttributeInfo(reader) {
        num_bootstrap_methods = reader.readUint16();
        if (num_bootstrap_methods != 0)
            bootstrap_methods = new BootstrapMethods_attribute::bootstrap_methods_t *[num_bootstrap_methods];
        for (int pos = 0; pos < num_bootstrap_methods; pos++) {
            bootstrap_methods[pos] = new BootstrapMethods_attribute::bootstrap_methods_t(reader);
        }
    }

    u2 num_bootstrap_methods;

    class bootstrap_methods_t {
    public:
        u2 bootstrap_method_ref;
        u2 num_bootstrap_arguments;
        u2 *bootstrap_arguments = nullptr;                    // [num_bootstrap_arguments];
        bootstrap_methods_t(ClassReader &reader) {
            bootstrap_method_ref = reader.readUint16();
            num_bootstrap_arguments = reader.readUint16();
            if (num_bootstrap_arguments != 0)
                bootstrap_arguments = new u2[num_bootstrap_arguments];
            for (int pos = 0; pos < num_bootstrap_arguments; pos++) {
                bootstrap_arguments[pos] = reader.readUint16();
            }
        }

    private:
        ~bootstrap_methods_t();
    } **bootstrap_methods = nullptr;                            // [num_bootstrap_methods];
};

class MethodParameters_attribute : public AttributeInfo {
public:
    u1 parameters_count;

    class parameters_t {
    public:
        u2 name_index;
        u2 access_flags;

        parameters_t(ClassReader &reader) {
            name_index = reader.readUint16();
            access_flags = reader.readUint16();
        }
    } **parameters = nullptr;                                    // [parameters_count];
    MethodParameters_attribute(ClassReader &reader) : AttributeInfo(reader) {
        parameters_count = reader.readUint8();
        if (parameters_count != 0)
            parameters = new MethodParameters_attribute::parameters_t *[parameters_count];
        for (int pos = 0; pos < parameters_count; pos++) {
            parameters[pos] = new MethodParameters_attribute::parameters_t(reader);
        }
    }

    ~MethodParameters_attribute();
};


#endif //TINY_JVM_ATTRIBUTE_INFO_H
