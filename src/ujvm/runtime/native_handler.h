//
// Created by arthur on 2022/7/13.
//

#pragma once

#include "shared/jstring.h"

#include <unordered_map>
#include "ujvm/native/java_lang_system.h"
using namespace ujvm;
static std::unordered_map<strings::String, std::function<void *(strings::String &)>> registerMap;

void registerNative() {

    registerMap["java/lang/System"] = ;

}
