#include <iostream>

extern "C"
{
#include <libavutil/avutil.h>
}

#include "io/io.h"

int main(int , char *[])
{
    std::cout << "start" << std::endl;
    std::cout << av_version_info() << std::endl;
    std::cout << "finish" << std::endl;
    return 0;
}
