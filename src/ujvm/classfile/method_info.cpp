#include "method_info.h"
#include "attribute_info.h"

MethodInfo::MethodInfo(ClassReader &reader, ConstantPool *constantPool, int a) {
    access_flags = reader.readUInt16();
    name_index = reader.readUInt16();
    descriptor_index = reader.readUInt16();
    attributes_count = reader.readUInt16();
    if (attributes_count != 0)
        attributes = new AttributeInfo *[attributes_count];
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos] = AttributeInfo::readAttributeInfo(reader, constantPool);
    }
}

void MethodInfo::dump(DataOutputStream &os) {

    os.writeUInt16(access_flags);
    os.writeUInt16(name_index);
    os.writeUInt16(descriptor_index);
    os.writeUInt16(attributes_count);
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos]->dump(os);
    }
}
