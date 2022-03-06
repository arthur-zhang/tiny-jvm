//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_METHOD_PARAMETER_H
#define TINY_JVM_METHOD_PARAMETER_H

#include "types.hpp"
#include "class_reader.h"
#include "String.h"
#include "constant_pool.hpp"
#include "exception_table.h"
#include "element_value.h"

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

    void dump(DataOutputStream &os) {
        os.writeUInt16(name_index);
        os.writeUInt16(access_flags);
    }
};


#endif //TINY_JVM_METHOD_PARAMETER_H
