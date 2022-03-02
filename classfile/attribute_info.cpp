//
// Created by ya on 2022/3/2.
//

#include "attribute_info.h"
#include "const.h"

AttributeInfo *AttributeInfo::parseAttribute(ClassReader &reader, ConstantPool *constant_pool) {
    u2 attribute_name_index = reader.peek2();
    u2 attribute_tag = toAttributeTag(attribute_name_index, constant_pool);
    switch (attribute_tag) {
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
            ExceptionsAttribute *result = new ExceptionsAttribute(reader);
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
            AnnotationDefault_attribute *result = new AnnotationDefault_attribute(reader);
            return result;
        }
        case 21: {
            BootstrapMethods_attribute *result = new BootstrapMethods_attribute(reader);
            return result;
        }
        case 22: {
            MethodParameters_attribute *result = new MethodParameters_attribute(reader);
            return result;
        }
        default: {
            std::cerr << "can't go there! map has not this error tag " << attribute_tag << "!" << std::endl;
            assert(false);
        }
    }
}

AttributeInfo::AttributeInfo(ClassReader &reader) {
    attribute_name_index = reader.readUint16();
    attribute_length = reader.readUint32();
}

u2 AttributeInfo::toAttributeTag(std::uint16_t attribute_name_index, ConstantPool *constant_pool) {
    String str = ((CONSTANT_Utf8_info *) constant_pool->getConstantPool()[attribute_name_index - 1])->getConstant();
    if (attribute_table.find(str) != attribute_table.end()) {
        return attribute_table[str];
    }
    std::cerr << "my jvm don't alloc a new attribute, because we don't have a compiler~" << std::endl;
    assert(false);
};

ElementValue *ElementValue::readElementValue(ClassReader &reader) {
    u1 tag = reader.readUint8();
    switch ((char) tag) {
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
        case 's': {
            return new SimpleElementValue(reader, tag);
        }
        case 'e': {
            return new EnumElementValue(reader, tag);
        }
        case 'c': {
            return new ClassElementValue(reader, tag);
            break;
        }
        case '@': {
            return new AnnotationElementValue(reader, tag);
            break;

        }
        case '[': {
            return new ArrayElementValue(reader, tag);
            break;
        }
        default: {
            std::cerr << "can't get here. in element_value." << std::endl;
            assert(false);
        }
    }
}
