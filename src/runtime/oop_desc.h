#pragma once

#include "instance_class.h"
#include "oop_value_type.h"


class InstanceKlass;

class OopDesc {

private:
    InstanceKlass *clazz_ = nullptr;
    OopType oopType_;
public:
    OopDesc(InstanceKlass *clazz, OopType oopType) : clazz_(clazz), oopType_(oopType) {}

    InstanceKlass *getClazz() const {
        return clazz_;
    }

    OopType getOopType() const {
        return oopType_;
    }
};
typedef OopDesc* oop;

class InstanceOopDesc : public OopDesc {

public:
    InstanceOopDesc(InstanceKlass *clazz, jobject ref) : ref_(ref), OopDesc(clazz, OopType::INSTANCE_OOP) {}

    const void *getRef() const {
        return ref_;
    }

private:
    jobject ref_;
};


template<typename T>
class PrimitiveOopDesc : public OopDesc {
public:
    PrimitiveOopDesc(T value) : value_(value), OopDesc(nullptr, OopType::PRIMITIVE_OOP) {}

    virtual ValueType getPrimitiveValueType() const = 0;

    T getValue() const {
        return value_;
    }


private:
    T value_;
};


class IntOopDesc : public PrimitiveOopDesc<jint> {

public:
    explicit IntOopDesc(jint value) : PrimitiveOopDesc(value) {
    }

    ValueType getPrimitiveValueType() const override {
        return ValueType::INT;
    }
};

class LongOopDesc : public PrimitiveOopDesc<jlong> {
public:
    LongOopDesc(jlong value) : PrimitiveOopDesc(value) {}

    ValueType getPrimitiveValueType() const override {
        return ValueType::LONG;
    }
};

class FloatOopDesc : PrimitiveOopDesc<jfloat> {
public:
    FloatOopDesc(jfloat value) : PrimitiveOopDesc(value) {}

    ValueType getPrimitiveValueType() const override {
        return ValueType::FLOAT;
    }
};

class DoubleOopDesc : PrimitiveOopDesc<jdouble> {
public:
    DoubleOopDesc(jdouble value) : PrimitiveOopDesc(value) {}

    ValueType getPrimitiveValueType() const override {
        return ValueType::DOUBLE;
    }
};