#pragma once

#include <string>

namespace ujvm::fs {

    class FileHandle {

    public:
        FileHandle();

        ~FileHandle();

        void open(const std::string &path);

        void close();

    private:
        FileHandle(const FileHandle &);
        FileHandle &operator=(const FileHandle &);
        int fd_;
    };
}