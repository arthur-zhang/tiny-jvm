#include "method_info.h"
#include "attribute_info.h"
MethodInfo::MethodInfo(ClassReader &reader, ConstantPool *constantPool, int a) {
    access_flags = reader.readUint16();
    name_index = reader.readUint16();
    descriptor_index = reader.readUint16();
    attributes_count = reader.readUint16();
    if (attributes_count != 0)
        attributes = new AttributeInfo *[attributes_count];
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos] = AttributeInfo::readAttribute(reader, constantPool);
    }
}
