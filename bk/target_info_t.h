//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_TARGET_INFO_T_H
#define TINY_JVM_TARGET_INFO_T_H

#include "shared/types.h"
#include "class_reader.h"
#include "constant_pool.hpp"
#include "element_value.h"

class target_info_t {
public:
    target_info_t(ClassReader &reader);

    virtual void dump(DataOutputStream &os);

    virtual ~target_info_t() {}
};

class type_parameter_target : target_info_t {
public:
    u1 type_parameter_index;

    type_parameter_target(ClassReader &reader) : target_info_t(reader) {
        type_parameter_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(type_parameter_index);
    }


};

class supertype_target : target_info_t {
public:
    u2 supertype_index;

    supertype_target(ClassReader &reader) : target_info_t(reader) {
        supertype_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(supertype_index);
    }
};

class type_parameter_bound_target : target_info_t {
public:
    u1 type_parameter_index;
    u1 bound_index;

    type_parameter_bound_target(ClassReader &reader) : target_info_t(reader) {
        type_parameter_index = reader.readUint8();
        bound_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(type_parameter_index);
        os.writeUInt8(bound_index);
    }
};

class empty_target : target_info_t {
};

class formal_parameter_target : target_info_t {

public:
    u1 formal_parameter_index;

    formal_parameter_target(ClassReader &reader) : target_info_t(reader) {
        formal_parameter_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(formal_parameter_index);
    }
};

class throws_target : target_info_t {
public:
    u2 throws_type_index;

    throws_target(ClassReader &reader) : target_info_t(reader) {
        throws_type_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(throws_type_index);
    }
};

class table_t {
public:
    u2 start_pc;
    u2 length;
    u2 index;

    table_t(ClassReader &reader) {
        start_pc = reader.readUInt16();
        length = reader.readUInt16();
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(length);
        os.writeUInt16(index);
    }
};

class localvar_target : target_info_t {
public:
    u2 table_length;
    table_t *table = nullptr;                // [table_length];
    localvar_target(ClassReader &reader) : target_info_t(reader) {
        table_length = reader.readUInt16();
        table = new table_t(reader);
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(table_length);
        table->dump(os);
    }
};

class catch_target : target_info_t {
public:
    u2 exception_table_index;

    catch_target(ClassReader &reader) : target_info_t(reader) {
        exception_table_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(exception_table_index);
    }

};

class offset_target : target_info_t {
public:
    u2 offset;

    offset_target(ClassReader &reader) : target_info_t(reader) {
        offset = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(offset);
    }
};

class type_argument_target : target_info_t {
public:
    u2 offset;
    u1 type_argument_index;

    type_argument_target(ClassReader &reader) : target_info_t(reader) {
        offset = reader.readUInt16();
        type_argument_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt16(offset);
        os.writeUInt16(type_argument_index);
    }
};

class path_t {
public:
    u1 type_path_kind;
    u1 type_argument_index;

    path_t(ClassReader &reader) {
        type_path_kind = reader.readUint8();
        type_argument_index = reader.readUint8();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt8(type_path_kind);
        os.writeUInt8(type_argument_index);
    }
};

class type_path {
public:
    u1 path_length;


    path_t *path = nullptr;                // [path_length];

    type_path(ClassReader &reader) {
        path_length = reader.readUint8();
        path = new path_t(reader);
    }

    void dump(DataOutputStream &os) {
        os.writeUInt8(path_length);
        path->dump(os);
    }
};

class type_annotation {

public:
    u1 target_type;
    target_info_t *target_info = nullptr;    // [1]
    type_path *target_path;
    AnnotationEntry *anno = nullptr;                // [1]

    type_annotation(ClassReader &reader) {
        target_type = reader.readUint8();
        target_info = new target_info_t(reader);
        target_path = new type_path(reader);
        anno = new AnnotationEntry(reader);
    }

    ~type_annotation();

};


#endif //TINY_JVM_TARGET_INFO_T_H
