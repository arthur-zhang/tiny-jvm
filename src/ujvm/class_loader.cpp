//
// Created by ya on 2022/3/10.
//

#include "class_loader.h"
#include "classpath/system_dictionary.h"
#include "ujvm/runtime/runtime.h"
#include "ujvm/runtime/instance_class.h"
#include "ujvm/runtime/thread.h"

InstanceClassStruct *BootstrapClassLoader::loadClass(u1 *classBytes, size_t classSize) {
    return nullptr;
}


enum ClassSource {
    NOT_FOUND,
    DIR,
    JAR
};
//struct ClassSearchResult final {
//    ClassSource classSource;
//
//};
static vector<strings::String> searchDirList = {
//        L"/Users/arthur/dev/java/ujvm-java/src/main/java",
        L"/Users/arthur/dev/java/ujvm-java/src/main/java",
//        L"/Users/arthur/CLionProjects/ujvm"
};


InstanceClassStruct *loadRawClassX(const strings::String &className) {

    strings::String tmpPath;
    for (const auto &item: searchDirList) {
        tmpPath.clear();
        std::wstringstream filePathBuilder;
        filePathBuilder << item << "/" << className << ".class";
        tmpPath = filePathBuilder.str();
        if (!std::filesystem::exists(tmpPath)) {
            continue;
        }
        auto *cf = new ujvm::ClassFile(strings::toStdString(tmpPath));
        cf->parse();
        return new InstanceClassStruct(cf);
    }

    return nullptr;
}

//ClassSearchResult *searchClass(const strings::String &className) {
//    strings::String fileName = strings::replaceAll(className, L".", L"/");
//    InstanceClassStruct *kClass = loadRawClassX(fileName);
//    if (kClass) {
//        return new ClassSearchResult();
//    }
//
//}

InstanceClassStruct *BootstrapClassLoader::loadClassByName(const strings::String &className) {
    InstanceClassStruct *clazz = SystemDictionary::get()->find(className);
    if (clazz != nullptr) return clazz;
    clazz = loadRawClassX(className);
    if (clazz != nullptr) {
        SystemDictionary::get()->put(className, clazz);
    }
    // call static init method <cinit>

    auto method = clazz->findMethod(L"<clinit>", L"()V");
    if (method != nullptr) {
        Runtime::callStaticMethod(clazz, Threads::currentThread, method);
    }
    return clazz;
}