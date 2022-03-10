#include "attribute_info.h"
#include "const.h"
#include "annotation_default.hpp"
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










