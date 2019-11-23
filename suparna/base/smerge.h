#ifndef SMERGE_H
#define SMERGE_H

#include "../type_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* 合并两段音频，result = sound1 × sound2
 * @sound  音频段数据
 * @size  音频段数据长度
 * @result  __out__ 存放合并后的音频段数据
 * @return  合并后的音频段数据长度，len = size1 > size2 ? size2 : size1
 */
size_type merge(const char *sound1, size_type size1, const char *sound2, size_type size2, char *result);

#ifdef __cplusplus
}
#endif

#endif // SMERGE_H
