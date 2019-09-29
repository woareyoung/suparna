#include <iostream>

#include <libavutil/avutil.h>


int main(int , char *[])
{
    std::cout << ::av_version_info() << std::endl;
    return 0;
}
