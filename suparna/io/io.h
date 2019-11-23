#ifndef IO_H
#define IO_H

#include "../type_def.h"

struct AVCodecContext;
struct AVCodec;
struct AVFormatContext;
struct AVOutputFormat;

struct PCM
{
    size_type size = 0; //数据长度
    char *data = nullptr;//数据区
    struct AVCodecContext *context = nullptr;//编码器上下文

    ~PCM();
};


enum class FormatID : unsigned char
{
    MP3,
    WAV,
    FLAC,
    APE,
    PCM
};
/* 从文件中读取到音频数据，小端模式
 * @file_name  文件路径，绝对路径或相对路径
 * @data  __out__ 保存音频的pcm数据，用于输出
 * @return  数据长度
 * finish date: 2019.10.06
 */
extern size_type from_file(const char *file_name, char *data);
/* @pcm  PCM结构体，里面会分配data和context的空间
*/
extern void from_file(const char *file_name, struct PCM *pcm);

/* 将音频数据输出为文件
 * @data  音频数据
 * @size  音频数据长度
 * @file_name  文件名
 * @type  文件类型
 * @return   成功——0，失败—— -1
 * finish date: 2019.10.04
 */
extern int to_file(const char *data, size_type size, const char *file_name, enum FormatID type);
extern int to_mp3(const char *data, size_type size, const char *file_name);
extern int to_wav(const char *data, size_type size, const char *file_name);
extern int to_flac(const char *data, size_type size, const char *file_name);
extern int to_ape(const char *data, size_type size, const char *file_name);

/* 将音频数据输出为文件
 * @data  音频数据
 * @size  音频数据长度
 * @file_name  文件名
 * @context  编码器上下文
 * @code  编码器
 * @return   成功——0，失败—— -1
 * finish date: 2019.10.04
 */
extern int write(const char *data, size_type size, const char *file_name, struct AVCodecContext *context, struct AVCodec *code);

/* 根据文件类型获取对应的编码器上下文
 * @type  文件类型
 * @return  AVCodecContext指针
 * finish date: 2019.10.03
 */
extern struct AVCodecContext* get_encoder_context(struct AVCodec *coder);

/* 根据文件类型获取对应的编码器
 * @type  文件类型
 * @return  AVCodec指针
 * finish date: 2019.10.03
 */
extern struct AVCodec* get_encoder(enum FormatID type);
/* 获取输出流格式上下文
 * @file_name  文件名
 * @return  AVFormatContext指针
 * finish date: 2019.10.04
 */
extern struct AVFormatContext* get_format_context(const char *file_name);
/* 获取输出流格式
 * @file_name  文件名
 * @context  编码器上下文
 * @format  格式上下文
 * @codec  编码器
 * finish date: 2019.10.04
 */
extern struct AVOutputFormat* get_output_format(const char *file_name,
                                                struct AVCodecContext *context,
                                                struct AVFormatContext *format,
                                                struct AVCodec *codec);

#endif // IO_H
