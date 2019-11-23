#ifndef TYPE_DEF_H
#define TYPE_DEF_H

// #ifndef size_type
// #define size_type unsigned long long
// #endif

using size_type = unsigned long long;


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


#endif