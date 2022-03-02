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
            ConstantValue_attribute *result = new ConstantValue_attribute(reader);
            return result;
        }
        case 1: {
            Code_attribute *result = new Code_attribute(reader, constant_pool);
            return result;
        }
        case 2: {
            StackMapTable_attribute *result = new StackMapTable_attribute(reader);
            return result;
        }
        case 3: {
            Exceptions_attribute *result = new Exceptions_attribute(reader);
            return result;
        }
        case 4: {
            InnerClasses_attribute *result = new InnerClasses_attribute(reader);
            return result;
        }
        case 5: {
            EnclosingMethod_attribute *result = new EnclosingMethod_attribute(reader);
            return result;
        }
        case 6: {
            Synthetic_attribute *result = new Synthetic_attribute(reader);
            return result;
        }
        case 7: {
            Signature_attribute *result = new Signature_attribute(reader);
            return result;
        }
        case 8: {
            SourceFile_attribute *result = new SourceFile_attribute(reader);
            return result;
        }
        case 9: {
            SourceDebugExtension_attribute *result = new SourceDebugExtension_attribute(reader);
            return result;
        }
        case 10: {
            LineNumberTable_attribute *result = new LineNumberTable_attribute(reader);
            return result;
        }
        case 11: {
            LocalVariableTable_attribute *result = new LocalVariableTable_attribute(reader);
            return result;
        }
        case 12: {
            LocalVariableTypeTable_attribute *result = new LocalVariableTypeTable_attribute(reader);
            return result;
        }
        case 13: {
            Deprecated_attribute *result = new Deprecated_attribute(reader);
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
            return nullptr;
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
    } else {
        std::cerr << "my jvm don't alloc a new attribute, because we don't have a compiler~" << std::endl;
        assert(false);
    }
    return 0;
};
