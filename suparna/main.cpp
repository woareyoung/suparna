#include "io/io.h"
#include "base/sbreak.h"
#include "base/sjoin.h"
#include "base/smerge.h"
#include "base/sspilt.h"
#include <iostream>

#if __GNUC__ == 8 || __GNUC__ < 7
#error not support current gcc version
#elif __GNUC__ > 8
#include <filesystem>
namespace std_fs = std:filesystem;
#elif __GNUC__ > 6
#include <experimental/filesystem>
namespace std_fs = std::experimental::filesystem;
// 需要链接 stdc++fs库，即在编译指令后面添加 -lstdc++fs选项
#endif

static constexpr const char * const file_name = "test.mp3";

int main()
{
    size_type file_size = std_fs::file_size(file_name);
    char *buffer = new char[file_size + 1];
    buffer[file_size] = 0;

    size_type len = from_file(file_name, buffer);
    std::cout << "parse successfully, size = " << len << std::endl;

    system("pause");
    return 0;
}