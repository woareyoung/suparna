#ifndef IO_H
#define IO_H

typedef unsigned long long size_type;

enum
{
    MP3,
    WAV,
    MPEG,
    FLAC,
    APE
};
#ifdef __cplusplus
extern "C"
{
#endif

/* 从文件中读取到音频数据
 * @file_name  文件路径
 * @out  __out__ 用于保存音频数据
 * return  数据长度
 * finish date:
 */
int from_file(const char *file_name, char *out);

/* 将音频数据输出为文件
 * @data  音频数据
 * @file_name  文件名
 * @size  音频数据长度
 * @type  文件类型
 * return   成功——1，失败——0
 */
int to_file(const char *data, const char *file_name, size_type size, size_type type);
int to_mp3(const char *data, const char *file_name, size_type size)
{ return to_file(data, file_name, size, MP3); }
int to_wav(const char *data, const char *file_name, size_type size)
{ return to_file(data, file_name, size, WAV); }
int to_mpeg(const char *data, const char *file_name, size_type size)
{ return to_file(data, file_name, size, MPEG); }
int to_flac(const char *data, const char *file_name, size_type size)
{ return to_file(data, file_name, size, FLAC); }
int to_ape(const char *data, const char *file_name, size_type size)
{ return to_file(data, file_name, size, APE); }

#ifdef __cplusplus
}
#endif

#endif // IO_H
