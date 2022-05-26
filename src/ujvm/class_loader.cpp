//
// Created by ya on 2022/3/10.
//

#include "class_loader.h"
#include "classpath/system_dictionary.h"

KClass *ClassLoader::loadClass(u1 *classBytes, size_t classSize) {
    return nullptr;
}

KClass *change_Name_to_CLASS(std::string &packageName, std::string &baseName) {

}

enum ClassSource {
    NOT_FOUND,
    DIR,
    JAR
};
struct ClassSearchResult final {
    ClassSource classSource;

};
static vector<strings::String> searchDirList = {
        L"/Users/arthur/cvt_dev/jvm/wind_jvm/sun_src",
        L"/Users/arthur/cvt_dev/clion/tiny-jvm"
};


KClass *loadRawClassX(strings::String &className) {
    KClass *clazz = SystemDictionary::get()->find(className);
    if (clazz != nullptr) return clazz;


    strings::String tmpPath;
    for (const auto &item: searchDirList) {
        tmpPath.clear();
        std::wstringstream filePathBuilder;
        filePathBuilder << item << "/" << className << ".class";
        tmpPath = filePathBuilder.str();
        if (!std::filesystem::exists(tmpPath)) {
            continue;
        }
        ClassFile *cf = new ujvm::ClassFile(strings::toStdString(tmpPath));
        return new InstanceKClass(cf);
    }

    return nullptr;
}

ClassSearchResult *searchClass(const strings::String &className) {
    strings::String fileName = strings::replaceAll(className, L".", L"/");
    KClass *kClass = loadRawClassX(fileName);
    if (kClass) {
        return new ClassSearchResult();
    }

}

KClass *ClassLoader::loadRawClass(strings::String &name) {

    return nullptr;
}
