#include "io/io.h"
#include "base/sbreak.h"
#include "base/sjoin.h"
#include "base/smerge.h"
#include "base/sspilt.h"
#include <iostream>

static constexpr const char * const file_name = "test.mp3";

int main()
{
    std::cout << "process start" << std::endl;
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

    std::cout << "process end" << std::endl;
    system("pause");
    return 0;
}