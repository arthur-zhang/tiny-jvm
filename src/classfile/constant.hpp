//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_CONSTANT_HPP
#define TINY_JVM_CONSTANT_HPP

#include "class_reader.h"
#include "String.h"
#include "const.h"
#include "data_output_stream.hpp"

class Constant {
public:
    u1 tag;

    Constant(ClassReader &reader, int tag_) : reader(reader), tag(tag_) {
        tag = reader.readUint8();
    }

    virtual void dump(DataOutputStream &os) {
        os.writeUInt8(tag);
    };


protected:
    ClassReader &reader;
};

class CONSTANT_Class_info : public Constant {

public:
    CONSTANT_Class_info(ClassReader &reader) : Constant(reader, CONSTANT_Class) {
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);
        os.writeUInt16(index);
    }

    u2 index;
};

class CONSTANT_String_info : public Constant {            // Class, String

public:
    CONSTANT_String_info(ClassReader &reader) : Constant(reader, CONSTANT_String) {
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);
        os.writeUInt16(index);
    }

    u2 index;
};


class CONSTANT_FMI_info : public Constant {        // Field, Methodref, InterfaceMethodref

public:
    CONSTANT_FMI_info(ClassReader &reader, int tag) : Constant(reader, tag) {
        class_index = reader.readUInt16();
        name_and_type_index = reader.readUInt16();
    }

    u2 class_index;
    u2 name_and_type_index;

    void dump(DataOutputStream &os) override {
        Constant::dump(os);
        os.writeUInt16(class_index);
        os.writeUInt16(name_and_type_index);
    }
};

class ConstantMethodref_info : public CONSTANT_FMI_info {
public:
    ConstantMethodref_info(ClassReader &reader) : CONSTANT_FMI_info(reader, CONSTANT_Methodref) {
    };
};

class ConstantInterfaceMethodref_info : public CONSTANT_FMI_info {
public:
    ConstantInterfaceMethodref_info(ClassReader &reader) : CONSTANT_FMI_info(reader, CONSTANT_InterfaceMethodref) {
    };
};

class ConstantFieldref_info : public CONSTANT_FMI_info {
public:
    ConstantFieldref_info(ClassReader &reader) : CONSTANT_FMI_info(reader, CONSTANT_Fieldref) {
    };
};

class CONSTANT_Integer_info : public Constant {       // Integer
public:
    CONSTANT_Integer_info(ClassReader &reader) : Constant(reader, CONSTANT_Integer) {
        bytes = reader.readUInt32();
    }

    u4 bytes;

    int get_value() {
        return (int) bytes;
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);
        os.writeUInt32(bytes);
    }
};

#define FLOAT_INFINITY            0x7f800000
#define FLOAT_NEGATIVE_INFINITY    0xff800000
#define FLOAT_NAN                0x7f880000
// define by myself
#define DOUBLE_INFINITY            0x7ff0000000000000L
#define DOUBLE_NEGATIVE_INFINITY    0xfff0000000000000L
#define DOUBLE_NAN                0x7ff8000000000000L


// constant pool: MethodHandle
#define REF_getField                    1
#define REF_getStatic                2
#define REF_putField                    3
#define REF_putStatic                4
#define REF_invokeVirtual            5
#define REF_invokeStatic                6
#define REF_invokeSpecial            7
#define REF_newInvokeSpecial            8
#define REF_invokeInterface            9

struct CONSTANT_Float_info : public Constant {       // Float
private:
    u4 bytes;

public:
    CONSTANT_Float_info(ClassReader &reader) : Constant(reader, CONSTANT_Float) {
        bytes = reader.readUInt32();
    }

    float get_value() {
        if (bytes == FLOAT_INFINITY) return FLOAT_INFINITY;
        else if (bytes == FLOAT_NEGATIVE_INFINITY) return FLOAT_NEGATIVE_INFINITY;
        else if ((bytes >= 0x7f800001 && bytes <= 0x7fffffff) || (bytes >= 0xff800001 && bytes <= 0xffffffff))
            return FLOAT_NAN;
        else {
            int s = ((bytes >> 31) == 0) ? 1 : -1;
            int e = ((bytes >> 23) & 0xff);
            int m = (e == 0) ? (bytes & 0x7fffff) << 1 : (bytes & 0x7fffff) | 0x800000;        // $ 4.4.4
            return s * m * pow(2, e - 150);
        }
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);
        os.writeUInt32(bytes);
    }
};

struct CONSTANT_Long_info : public Constant {       // Long
private:
    u4 high_bytes;
    u4 low_bytes;

public:
    CONSTANT_Long_info(ClassReader &reader) : Constant(reader, CONSTANT_Long) {
        high_bytes = reader.readUInt32();
        low_bytes = reader.readUInt32();
    }

    long get_value() {
        return ((long) high_bytes << 32) + low_bytes;
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);

        os.writeUInt32(high_bytes);
        os.writeUInt32(low_bytes);
    }
};

class CONSTANT_Double_info : public Constant {       // Double
private:

    u4 high_bytes;
    u4 low_bytes;

public:
    CONSTANT_Double_info(ClassReader &reader) : Constant(reader, CONSTANT_Double) {
        high_bytes = reader.readUInt32();
        low_bytes = reader.readUInt32();
    }

    double get_value() {
        uint64_t bytes = ((uint64_t) high_bytes << 32) + low_bytes;    // first turns to a Long
        if (bytes == DOUBLE_INFINITY) return DOUBLE_INFINITY;
        else if (bytes == DOUBLE_NEGATIVE_INFINITY) return DOUBLE_NEGATIVE_INFINITY;
        else if ((bytes >= 0x7ff0000000000001L && bytes <= 0x7fffffffffffffffL) ||
                 (bytes >= 0xfff0000000000001L && bytes <= 0xffffffffffffffffL))
            return DOUBLE_NAN;
        else {
            int s = ((bytes >> 63) == 0) ? 1 : -1;
            int e = (int) ((bytes >> 52) & 0x7ffL);
            long m = (e == 0) ? (bytes & 0xfffffffffffffL) << 1 : (bytes & 0xfffffffffffffL) | 0x10000000000000L;
            return s * m * pow(2, e - 1075);
        }
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);

        os.writeUInt32(high_bytes);
        os.writeUInt32(low_bytes);
    }

};

class CONSTANT_NameAndType_info : public Constant {
public:
    u2 name_index;
    u2 descriptor_index;

    CONSTANT_NameAndType_info(ClassReader &reader_) : Constant(reader_, CONSTANT_NameAndType) {
        name_index = reader_.readUInt16();
        descriptor_index = reader_.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);

        os.writeUInt16(name_index);
        os.writeUInt16(descriptor_index);
    }
};

class CONSTANT_Utf8_info : public Constant {       // string literal
public:
    u2 length;
    u1 *bytes = nullptr;

    CONSTANT_Utf8_info(ClassReader &reader) : Constant(reader, CONSTANT_Utf8) {
        length = reader.readUInt16();
        bytes = reader.readBytes(length);
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);

        os.writeUInt16(length);
        os.writeBytes(bytes, length);
    }

    String getConstant() {
        //todo
        return fromBytes(bytes, length);
    }

    ~CONSTANT_Utf8_info();
};

class CONSTANT_MethodHandle_info : public Constant {   // method handler
public:
    CONSTANT_MethodHandle_info(ClassReader &reader) : Constant(reader, CONSTANT_MethodHandle) {
        reference_kind = reader.readUint8();
        reference_index = reader.readUInt16();
    }

    u1 reference_kind;
    u2 reference_index;

    void dump(DataOutputStream &os) override {
        Constant::dump(os);

        os.writeUInt8(reference_kind);
        os.writeUInt16(reference_index);
    }

};

class CONSTANT_MethodType_info : public Constant {   // method type
public:
    CONSTANT_MethodType_info(ClassReader &reader) : Constant(reader, CONSTANT_MethodType) {
        descriptor_index = reader.readUInt16();
    }

    u2 descriptor_index;

    void dump(DataOutputStream &os) override {
        Constant::dump(os);

        os.writeUInt16(descriptor_index);
    }
};

class CONSTANT_InvokeDynamic_info : public Constant {
public:
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;

    CONSTANT_InvokeDynamic_info(ClassReader &reader) : Constant(reader, CONSTANT_InvokeDynamic) {
        bootstrap_method_attr_index = reader.readUInt16();
        name_and_type_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        Constant::dump(os);

        os.writeUInt16(bootstrap_method_attr_index);
        os.writeUInt16(name_and_type_index);
    }

};


static Constant *readConstant(ClassReader &reader) {
    u1 tag = reader.peek1();
    switch (tag) {
        case CONSTANT_Integer:
            return new CONSTANT_Integer_info(reader);
        case CONSTANT_Float:
            return new CONSTANT_Float_info(reader);
        case CONSTANT_Long:
            return new CONSTANT_Long_info(reader);
        case CONSTANT_Double:
            return new CONSTANT_Double_info(reader);
        case CONSTANT_Utf8:
            return new CONSTANT_Utf8_info(reader);
        case CONSTANT_Class:
            return new CONSTANT_Class_info(reader);
        case CONSTANT_String:
            return new CONSTANT_String_info(reader);
        case CONSTANT_Fieldref:
            return new ConstantFieldref_info(reader);
        case CONSTANT_Methodref:
            return new ConstantMethodref_info(reader);
        case CONSTANT_InterfaceMethodref:
            return new ConstantInterfaceMethodref_info(reader);
        case CONSTANT_NameAndType:
            return new CONSTANT_NameAndType_info(reader);
        case CONSTANT_MethodType:
            return new CONSTANT_MethodType_info(reader);
        case CONSTANT_MethodHandle:
            return new CONSTANT_MethodHandle_info(reader);
        case CONSTANT_InvokeDynamic:
            return new CONSTANT_InvokeDynamic_info(reader);
        default:
            std::cout << "can't get here, CONSTANT pool." << std::endl;
            assert(false);
    }
}

#endif //TINY_JVM_CONSTANT_HPP
