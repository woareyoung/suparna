#include "file.h"
#ifndef SUPPORT_FILESYSTEM
#include <cstdio>
#endif

size_type get_file_size(const char *file_name)
{
#ifdef SUPPORT_FILESYSTEM
    size_type file_size = std_fs::file_size(file_name);
#else
    std::ifstream f(file_name);
    if(!f.is_open())
    {
        return 0;
    }
    f.seekg(0, std::ios::end);
    size_type file_size = static_cast<size_type>(f.tellg());
    f.close();
#endif
    return file_size;
}

bool remove_file(const char *file_name)
{
#ifdef SUPPORT_FILESYSTEM
    return std_fs::remove(file_name);
#else
    return remove(file_name) == 0;
#endif
}