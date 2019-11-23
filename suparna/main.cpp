#include "io/io.h"
#include "base/sbreak.h"
#include "base/sjoin.h"
#include "base/smerge.h"
#include "base/sspilt.h"
#include <iostream>

#if __GNUC__ == 8 || __GNUC__ < 7
#error not support current gcc version

#elif __GNUC__ > 8

#ifndef SUPPORT_FILESYSTEM
#define SUPPORT_FILESYSTEM
#endif

#include <filesystem>
namespace std_fs = std:filesystem;

#elif __GNUC__ > 6

#ifndef SUPPORT_FILESYSTEM
#define SUPPORT_FILESYSTEM
#endif

#include <experimental/filesystem>
namespace std_fs = std::experimental::filesystem;
// 需要链接 stdc++fs库，即在编译指令添加 -lstdc++fs选项

#endif

#ifndef SUPPORT_FILESYSTEM
#include <fstream>
#endif

static constexpr const char * const file_name = "test.mp3";

int main()
{
#ifdef SUPPORT_FILESYSTEM
    size_type file_size = std_fs::file_size(file_name);
#else
    std::ifstream f(file_name);
    if(!f.is_open())
    {
        std::cout << "cannot open file" << std::endl;
    }
    f.seekg(0, std::ios::end);
    size_type file_size = static_cast<size_type>(f.tellg());
#endif

    char *buffer = new char[file_size + 1];
    buffer[file_size] = 0;

    size_type len = from_file(file_name, buffer);
    std::cout << "parse successfully, size = " << len << std::endl;

    system("pause");
    return 0;
}