#include "../type_def.h"

#if __GNUC__ == 8

#ifdef SUPPORT_FILESYSTEM
#undef SUPPORT_FILESYSTEM
#endif

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
/* 获取文件大小
 * @file_name  文件名
 * @return  文件字节大小
 */
extern size_type get_file_size(const char *file_name);
/* 删除文件
 * @file_name  文件名
 * @return  是否成功，true——成功，false——失败
 */
extern bool remove_file(const char *file_name);

