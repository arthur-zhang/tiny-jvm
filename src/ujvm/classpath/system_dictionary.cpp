//
// Created by ya on 2022/3/10.
//

#include "system_dictionary.h"

bool SystemDictionary::put(const strings::String &name, KClass *clazz) {
    return this->classMap_.insert({name, clazz}).second;
}

KClass *SystemDictionary::find(strings::String &name) {
    return nullptr;
}

SystemDictionary *SystemDictionary::get() {
    static SystemDictionary dictionary;
    return &dictionary;
}
