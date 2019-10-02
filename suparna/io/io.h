#ifndef IO_H
#define IO_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned long long size_type;

struct AVCodecContext;

enum FormatID
{
    MP3,
    WAV,
    MPEG,
    FLAC,
    APE
};
/* 从文件中读取到音频数据
 * @file_name  文件路径
 * @data  __out__ 用于保存音频数据
 * @return  数据长度
 * finish date:
 */
int from_file(const char *file_name, char *data);

/* 将音频数据输出为文件
 * @data  音频数据
 * @file_name  文件名
 * @size  音频数据长度
 * @type  文件类型
 * @return   成功——1，失败——0
 */
int to_file(const char *data, const char *file_name, size_type size, enum FormatID type);
int to_mp3(const char *data, const char *file_name, size_type size);
int to_wav(const char *data, const char *file_name, size_type size);
int to_mpeg(const char *data, const char *file_name, size_type size);
int to_flac(const char *data, const char *file_name, size_type size);
int to_ape(const char *data, const char *file_name, size_type size);

int write(const char *data, const char *file_name, size_type size, struct AVCodecContext *context);

/* 根据文件类型获取对应的编码类型上下文
 * @type  文件类型
 * @return  AVCodecContext指针
 * finish date: 2019.10.02
 */
struct AVCodecContext* get_encoder_context(enum FormatID type);

#ifdef __cplusplus
}
#endif

#endif // IO_H
