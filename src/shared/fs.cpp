//
// Created by ya on 2022/3/11.
//

#include <unistd.h>
#include "fs.h"
#include <fcntl.h>

using namespace ujvm::fs;


FileHandle::FileHandle() {}

FileHandle::~FileHandle() {
    close();
}

void FileHandle::open(const std::string &path) {
    fd_ = ::open(path.c_str(), O_RDWR);
}

void FileHandle::close() {
    ::close(fd_);
}
