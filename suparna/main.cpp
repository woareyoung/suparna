#include <iostream>

extern "C"
{
#include <libavutil/avutil.h>
}


#include "io/io.h"

int main(int , char *[])
{
    std::cout << av_version_info() << std::endl;
    return 0;
}
