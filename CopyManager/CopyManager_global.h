#ifndef COPYMANAGER_GLOBAL_H
#define COPYMANAGER_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(COPYMANAGER_LIBRARY)
#  define COPYMANAGER_EXPORT Q_DECL_EXPORT
#else
#  define COPYMANAGER_EXPORT Q_DECL_IMPORT
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <string>

namespace CMGlobal {

typedef struct _file_info {
    _file_info() {}
    ~_file_info() {}

    enum Type {
        None = 0,
        File,
        Directory
    };

    _file_info(const _file_info& other) {
        buffer = other.buffer;
        file_size = other.file_size;
        filename = other.filename;
        pathFrom = other.pathFrom;
        pathTo = other.pathTo;
        type = other.type;
    }

    _file_info& operator=(const _file_info& other) {
        if (this == &other) return *this;

        buffer = other.buffer;
        file_size = other.file_size;
        filename = other.filename;
        pathFrom = other.pathFrom;
        pathTo = other.pathTo;
        type = other.type;

        return *this;
    }

    char *buffer = nullptr;
    std::size_t file_size = 0;
    std::string filename;
    std::string pathFrom;
    std::string pathTo;
    Type type = None;

} FileInfo;

}
#endif // COPYMANAGER_GLOBAL_H
