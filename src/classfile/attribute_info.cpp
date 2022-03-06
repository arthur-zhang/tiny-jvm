#include "attribute_info.h"
#include "const.h"
#include "annotation_default.h"
#include "runtime_visible_annotations_attribute.hpp"
#include "bootstrap_methods_attribute.h"
#include "code_attribute.h"
#include "constant_value_attribute.h"
#include "deprecated_attribute.h"
#include "runtime_invisible_annotations_attribute.hpp"
#include "runtime_visible_parameter_annotations_attribute.hpp"
#include "runtime_invisible_parameter_annotations_attribute.hpp"
#include "method_parameter.h"
#include "method_prameters_attribute.h"
#include "runtime_visible_type_annotations_attribute.hpp"
#include "runtime_invisible_type_annotations_attribute.hpp"
#include "stack_map_table_attribute.h"
#include "exception_table_attribute.h"
#include "inner_classes_attribute.h"
#include "enclosing_method_attribute.h"
#include "synthetic_attribute.h"
#include "signature_attribute.hpp"
#include "source_file_attribute.h"
#include "source_debug_extension_attribute.hpp"
#include "line_number_table_attribute.h"
#include "local_variable_table_attribute.h"
#include "local_variable_type_table_attribute.h"

AttributeInfo *AttributeInfo::readAttribute(ClassReader &reader, ConstantPool *constant_pool) {
    u2 attribute_name_index = reader.peek2();
//    cout << "attribute_name_index:" << attribute_name_index << endl;
    u2 tag = attributeName2Tag(attribute_name_index, constant_pool);
//    cout << "tag:" << tag << endl;
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
            RuntimeVisibleAnnotationsAttribute *result = new RuntimeVisibleAnnotationsAttribute(reader);
            return result;
        }
        case 15: {
            RuntimeInvisibleAnnotations_attribute *result = new RuntimeInvisibleAnnotations_attribute(reader);
            return result;
        }
        case 16: {
            RuntimeVisibleParameterAnnotationsAttribute *result = new RuntimeVisibleParameterAnnotationsAttribute(
                    reader);
            return result;
        }
        case 17: {
            RuntimeInvisibleParameterAnnotations_attribute *result = new RuntimeInvisibleParameterAnnotations_attribute(
                    reader);
            return result;
        }
        case 18: {
            RuntimeVisibleTypeAnnotationsAttribute *result = new RuntimeVisibleTypeAnnotationsAttribute(reader);
            return result;
        }
        case 19: {
            RuntimeInvisibleTypeAnnotationsAttribute *result = new RuntimeInvisibleTypeAnnotationsAttribute(
                    reader);
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
}

void AttributeInfo::dump(DataOutputStream &os) {
    os.writeUInt16(attribute_name_index);
    os.writeUInt32(attribute_length);
}









