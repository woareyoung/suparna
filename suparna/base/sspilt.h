#ifndef SSPILT_H
#define SSPILT_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned long long size_type;

/* 将一段音频分割成两段
 * @sound  音频段数据
 * @size  音频段数据长度
 * @pos  分割的位置
 * @result  保存分割结果
 * @size  分割结果的音频数据长度
 * @return  分割是否成功
 */
int spilt(const char *sound, size_type size, size_type pos, char *result1, size_type &size1, char *result2, size_type &size2);

#ifdef __cplusplus
}
#endif

#endif // SSPILT_H
