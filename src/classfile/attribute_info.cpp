#include "classfile/attribute_info.h"
#include "shared/const.h"

AttributeInfo *AttributeInfo::readAttributeInfo(ClassReader &reader, ConstantPool *constant_pool) {
    u2 attribute_name_index = reader.peek2();
    u2 tag = attributeName2Tag(attribute_name_index, constant_pool);
    switch (tag) {
        case ATTRIBUTE_ConstantValue: {
            ConstantValueAttribute *result = new ConstantValueAttribute(reader);
            return result;
        }
        case ATTRIBUTE_Code: {
            CodeAttribute *result = new CodeAttribute(reader, constant_pool);
            return result;
        }
        case ATTRIBUTE_StackMapTable: {
            StackMapTableAttribute *result = new StackMapTableAttribute(reader);
            return result;
        }
        case ATTRIBUTE_Exceptions: {
            ExceptionTableAttribute *result = new ExceptionTableAttribute(reader);
            return result;
        }
        case ATTRIBUTE_InnerClasses: {
            InnerClassesAttribute *result = new InnerClassesAttribute(reader);
            return result;
        }
        case ATTRIBUTE_EnclosingMethod: {
            EnclosingMethodAttribute *result = new EnclosingMethodAttribute(reader);
            return result;
        }
        case ATTRIBUTE_Synthetic: {
            SyntheticAttribute *result = new SyntheticAttribute(reader);
            return result;
        }
        case ATTRIBUTE_Signature: {
            SignatureAttribute *result = new SignatureAttribute(reader);
            return result;
        }
        case ATTRIBUTE_SourceFile: {
            SourceFileAttribute *result = new SourceFileAttribute(reader);
            return result;
        }
        case ATTRIBUTE_SourceDebugExtension: {
            SourceDebugExtensionAttribute *result = new SourceDebugExtensionAttribute(reader);
            return result;
        }
        case ATTRIBUTE_LineNumberTable: {
            LineNumberTableAttribute *result = new LineNumberTableAttribute(reader);
            return result;
        }
        case ATTRIBUTE_LocalVariableTable: {
            LocalVariableTableAttribute *result = new LocalVariableTableAttribute(reader);
            return result;
        }
        case ATTRIBUTE_LocalVariableTypeTable: {
            LocalVariableTypeTableAttribute *result = new LocalVariableTypeTableAttribute(reader);
            return result;
        }
        case ATTRIBUTE_Deprecated: {
            DeprecatedAttribute *result = new DeprecatedAttribute(reader);
            return result;
        }
        case ATTRIBUTE_RuntimeVisibleAnnotations: {
            RuntimeVisibleAnnotationsAttribute *result = new RuntimeVisibleAnnotationsAttribute(reader);
            return result;
        }
        case ATTRIBUTE_RuntimeInvisibleAnnotations: {
            RuntimeInvisibleAnnotations_attribute *result = new RuntimeInvisibleAnnotations_attribute(reader);
            return result;
        }
        case ATTRIBUTE_RuntimeVisibleParameterAnnotations: {
            RuntimeVisibleParameterAnnotationsAttribute *result = new RuntimeVisibleParameterAnnotationsAttribute(
                    reader);
            return result;
        }
        case ATTRIBUTE_RuntimeInvisibleParameterAnnotations: {
            RuntimeInvisibleParameterAnnotations_attribute *result = new RuntimeInvisibleParameterAnnotations_attribute(
                    reader);
            return result;
        }
        case ATTRIBUTE_RuntimeVisibleTypeAnnotations: {
            RuntimeVisibleTypeAnnotationsAttribute *result = new RuntimeVisibleTypeAnnotationsAttribute(reader);
            return result;
        }
        case ATTRIBUTE_RuntimeInvisibleTypeAnnotations: {
            RuntimeInvisibleTypeAnnotationsAttribute *result = new RuntimeInvisibleTypeAnnotationsAttribute(
                    reader);
            return result;
        }
        case ATTRIBUTE_AnnotationDefault: {
            AnnotationDefaultAttribute *result = new AnnotationDefaultAttribute(reader);
            return result;
        }
        case ATTRIBUTE_BootstrapMethods: {
            BootstrapMethodsAttribute *result = new BootstrapMethodsAttribute(reader);
            return result;
        }
        case ATTRIBUTE_MethodParameters: {
            MethodParametersAttribute *result = new MethodParametersAttribute(reader);
            return result;
        }
        default: {
            std::cerr << "can't go there! map has not this error tag " << tag << "!" << std::endl;
            PANIC("can't go there!");
        }
    }
}

u2 AttributeInfo::attributeName2Tag(u2 attribute_name_index, ConstantPool *constant_pool) {
    strings::String str = ((CONSTANT_Utf8_info *) constant_pool->getConstantPool()[attribute_name_index])->getConstant();
    if (attribute_table.find(str) != attribute_table.end()) {
        return attribute_table[str];
    }
    PANIC("attributeName2Tag failed, %d", attribute_name_index);
}

AttributeInfo::~AttributeInfo() {
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
    if (num_bootstrap_arguments > 0) delete[]bootstrap_arguments;
}

void BootstrapMethod::dump(DataOutputStream &os) {
    os.writeUInt16(bootstrap_method_ref);
    os.writeUInt16(num_bootstrap_arguments);
    for (int i = 0; i < num_bootstrap_arguments; i++) {
        os.writeUInt16(bootstrap_arguments[i]);
    }
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
    if (num_bootstrap_methods <= 0) return;
    for (int i = 0; i < num_bootstrap_methods; ++i) {
        delete bootstrap_methods[i];
    }
    delete[]bootstrap_methods;
}

void BootstrapMethodsAttribute::dump(DataOutputStream &os) {
    AttributeInfo::dump(os);
    os.writeUInt16(num_bootstrap_methods);
    for (int pos = 0; pos < num_bootstrap_methods; pos++) {
        bootstrap_methods[pos]->dump(os);
    }
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
        attributes[pos] = readAttributeInfo(reader, constantPool);
    }
}

CodeAttribute::~CodeAttribute() {
    if (code_length > 0) delete[] code;
    if (attributes_count > 0) {
        for (int i = 0; i < attributes_count; ++i) {
            delete attributes[i];
        }
        delete[] attributes;
    }
    if (exception_table_length > 0) {
        for (int i = 0; i < exception_table_length; ++i) {
            delete exception_table[i];
        }
        delete[]exception_table;
    }
}

ConstantValueAttribute::ConstantValueAttribute(ClassReader &reader) : AttributeInfo(reader) {
    constant_value_index = reader.readUInt16();
}

void ConstantValueAttribute::dump(DataOutputStream &os) {
    AttributeInfo::dump(os);
    os.writeUInt16(constant_value_index);
}

MethodParametersAttribute::MethodParametersAttribute(ClassReader &reader) : AttributeInfo(reader) {
    parameters_count = reader.readUint8();
    if (parameters_count != 0)
        parameters = new MethodParameter *[parameters_count];
    for (int pos = 0; pos < parameters_count; pos++) {
        parameters[pos] = new MethodParameter(reader);
    }
}

MethodParametersAttribute::~MethodParametersAttribute() {
    if (parameters_count <= 0)return;

    for (int i = 0; i < parameters_count; ++i) {
        delete parameters[i];
    }
    delete[]parameters;
}

StackMapTableAttribute::StackMapTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
    // todo later
    bytes = reader.readBytes(attribute_length);
}

void StackMapTableAttribute::dump(DataOutputStream &os) {
    AttributeInfo::dump(os);
    os.writeBytes(bytes, attribute_length);
}

StackMapTableAttribute::~StackMapTableAttribute() {
    if (attribute_length <= 0) return;
    delete[] bytes;
}

ExceptionTable::ExceptionTable(ClassReader &reader) {
    start_pc = reader.readUInt16();
    end_pc = reader.readUInt16();
    handler_pc = reader.readUInt16();
    catch_type = reader.readUInt16();
}

void ExceptionTable::dump(DataOutputStream &os) {
    os.writeUInt16(start_pc);
    os.writeUInt16(end_pc);
    os.writeUInt16(handler_pc);
    os.writeUInt16(catch_type);
}