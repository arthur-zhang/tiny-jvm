//
// Created by ya on 2022/3/2.
//

#include "attribute_info.h"
#include "const.h"

AttributeInfo *AttributeInfo::readAttribute(ClassReader &reader, ConstantPool *constant_pool) {
    u2 attribute_name_index = reader.peek2();
    // Compare strings to find known attribute
    u2 tag = attributeName2Tag(attribute_name_index, constant_pool);
    switch (tag) {
        case 0: {
            ConstantValueAttribute *result = new ConstantValueAttribute(reader);
            return result;
        }
        case 1: {
            CodeAttribute *result = new CodeAttribute(reader, constant_pool);
            return result;
        }
        case 2: {
            StackMapTableAttribute *result = new StackMapTableAttribute(reader);
            return result;
        }
        case 3: {
            ExceptionTableAttribute *result = new ExceptionTableAttribute(reader);
            return result;
        }
        case 4: {
            InnerClassesAttribute *result = new InnerClassesAttribute(reader);
            return result;
        }
        case 5: {
            EnclosingMethodAttribute *result = new EnclosingMethodAttribute(reader);
            return result;
        }
        case 6: {
            SyntheticAttribute *result = new SyntheticAttribute(reader);
            return result;
        }
        case 7: {
            SignatureAttribute *result = new SignatureAttribute(reader);
            return result;
        }
        case 8: {
            SourceFileAttribute *result = new SourceFileAttribute(reader);
            return result;
        }
        case 9: {
            SourceDebugExtensionAttribute *result = new SourceDebugExtensionAttribute(reader);
            return result;
        }
        case 10: {
            LineNumberTableAttribute *result = new LineNumberTableAttribute(reader);
            return result;
        }
        case 11: {
            LocalVariableTableAttribute *result = new LocalVariableTableAttribute(reader);
            return result;
        }
        case 12: {
            LocalVariableTypeTableAttribute *result = new LocalVariableTypeTableAttribute(reader);
            return result;
        }
        case 13: {
            DeprecatedAttribute *result = new DeprecatedAttribute(reader);
            return result;
        }
        case 14: {
            RuntimeVisibleAnnotations_attribute *result = new RuntimeVisibleAnnotations_attribute(reader);
            return result;
        }
        case 15: {
            RuntimeInvisibleAnnotations_attribute *result = new RuntimeInvisibleAnnotations_attribute(reader);
            return result;
        }
        case 16: {
            RuntimeVisibleParameterAnnotations_attribute *result = new RuntimeVisibleParameterAnnotations_attribute(
                    reader);
            return result;
        }
        case 17: {
            RuntimeInvisibleParameterAnnotations_attribute *result = new RuntimeInvisibleParameterAnnotations_attribute(
                    reader);
            return result;
        }
        case 18: {
            RuntimeVisibleTypeAnnotations_attribute *result = new RuntimeVisibleTypeAnnotations_attribute(reader);
            return result;
        }
        case 19: {
            RuntimeInvisibleTypeAnnotations_attribute *result = new RuntimeInvisibleTypeAnnotations_attribute(
                    reader);    // nashi
            return result;
        }
        case 20: {
            AnnotationDefaultAttribute *result = new AnnotationDefaultAttribute(reader);
            return result;
        }
        case 21: {
            BootstrapMethodsAttribute *result = new BootstrapMethodsAttribute(reader);
            return result;
        }
        case 22: {
            MethodParametersAttribute *result = new MethodParametersAttribute(reader);
            return result;
        }
        default: {
            std::cerr << "can't go there! map has not this error tag " << tag << "!" << std::endl;
            assert(false);
        }
    }
}

AttributeInfo::AttributeInfo(ClassReader &reader) {
    attribute_name_index = reader.readUInt16();
    attribute_length = reader.readUInt32();
}

u2 AttributeInfo::attributeName2Tag(std::uint16_t attribute_name_index, ConstantPool *constant_pool) {
    String str = ((CONSTANT_Utf8_info *) constant_pool->getConstantPool()[attribute_name_index - 1])->getConstant();
    if (attribute_table.find(str) != attribute_table.end()) {
        return attribute_table[str];
    }
    std::cerr << "my jvm don't alloc a new attribute, because we don't have a compiler~" << std::endl;
    assert(false);
};

MethodParametersAttribute::MethodParametersAttribute(ClassReader &reader) : AttributeInfo(reader) {
    parameters_count = reader.readUint8();
    if (parameters_count != 0)
        parameters = new MethodParameter *[parameters_count];
    for (int pos = 0; pos < parameters_count; pos++) {
        parameters[pos] = new MethodParameter(reader);
    }
}

MethodParametersAttribute::~MethodParametersAttribute() {
    for (int i = 0; i < parameters_count; ++i) {
        delete parameters[i];
    }
    delete[]parameters;
}

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

BootstrapMethod::BootstrapMethod(ClassReader &reader) {
    bootstrap_method_ref = reader.readUInt16();
    num_bootstrap_arguments = reader.readUInt16();
    if (num_bootstrap_arguments != 0)
        bootstrap_arguments = new u2[num_bootstrap_arguments];
    for (int pos = 0; pos < num_bootstrap_arguments; pos++) {
        bootstrap_arguments[pos] = reader.readUInt16();
    }
}

BootstrapMethod::~BootstrapMethod() {
    delete[]bootstrap_arguments;
}

AnnotationDefaultAttribute::~AnnotationDefaultAttribute() {
    delete default_value;
}

ConstantValueAttribute::ConstantValueAttribute(ClassReader &reader) : AttributeInfo(reader) {
    constant_value_index = reader.readUInt16();
}

SourceFileAttribute::SourceFileAttribute(ClassReader &reader) : AttributeInfo(reader) {
    source_file_index = reader.readUInt16();
}

CodeAttribute::CodeAttribute(ClassReader &reader, ConstantPool *constantPool) : AttributeInfo(reader) {
    max_stack = reader.readUInt16();
    max_locals = reader.readUInt16();
    code_length = reader.readUInt32();
    if (code_length != 0) {
        code = new u1[code_length];
        code = reader.readBytes(code_length);
    }
    exception_table_length = reader.readUInt16();
    if (exception_table_length != 0) {
        exception_table = new ExceptionTable *[exception_table_length];
        for (int pos = 0; pos < exception_table_length; pos++) {
            exception_table[pos] = new ExceptionTable(reader);
        }
    }
    attributes_count = reader.readUInt16();
    if (attributes_count != 0)
        attributes = new AttributeInfo *[attributes_count];
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos] = readAttribute(reader, constantPool);
    }
}

CodeAttribute::~CodeAttribute() {
    delete[] code;
    for (int i = 0; i < attributes_count; ++i) {
        delete attributes[i];
    }
    delete[] attributes;
    for (int i = 0; i < exception_table_length; ++i) {
        delete exception_table[i];
    }
    delete[]exception_table;
}

StackMapTableAttribute::StackMapTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
    // todo later
    bytes = reader.readBytes(attribute_length);
}

StackMapTableAttribute::~StackMapTableAttribute() {
    delete bytes;
}
