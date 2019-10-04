#ifndef SBREAK_H
#define SBREAK_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned long long size_type;

/* 将一段音频拆分成两段
 * @sound  音频段数据
 * @size  音频段数据长度
 * @result  保存分割结果
 * @size  分割结果的音频数据长度，len = size
 * @return  分割是否成功
 */
int break_up(const char *sound, size_type size, char *result1, size_type &size1, char *result2, size_type &size2);

#ifdef __cplusplus
}
#endif

#endif // SBREAK_H
