#ifndef SBREAK_H
#define SBREAK_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef size_type
#define size_type unsigned long long
#endif

/* 将一段音频拆分成两段
 * @sound  音频段数据
 * @size  音频段数据长度
 * @result  __out__ 保存分割结果
 * @size  __out__ 分割结果的音频数据长度，len = size
 * @return  分割是否成功，失败—— -1， 成功——0
 */
int break_up(const char *sound, size_type size, char *result1, size_type &size1, char *result2, size_type &size2);

#ifdef __cplusplus
}
#endif

#endif // SBREAK_H
