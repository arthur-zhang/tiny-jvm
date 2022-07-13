//
// Created by ya on 2022/3/10.
//

#include "system_dictionary.h"

bool SystemDictionary::put(const strings::String &name, InstanceClassStruct *clazz) {
    return this->classMap_.insert(std::make_pair(name, clazz)).second;
}

InstanceClassStruct *SystemDictionary::find(const strings::String &name) {

    auto ite = this->classMap_.find(name);
    if (ite == this->classMap_.end()) return nullptr;
    return ite->second;
}

SystemDictionary *SystemDictionary::get() {
    static SystemDictionary dictionary;
    return &dictionary;
}
