#include "io.h"
#include <iostream>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

static const enum AVCodecID FormatTablep[] =
{
    AV_CODEC_ID_MP3,
    AV_CODEC_ID_PCM_S16LE,
    AV_CODEC_ID_FLAC,
    AV_CODEC_ID_APE
};

static int decode(struct AVCodecContext *context, struct AVPacket *packet, struct AVFrame *frame, char *buff)
{
    char *data_buff = buff;
    // 发送编码数据包给解码器
    if(0 > avcodec_send_packet(context, packet))
    {
        return -1;
    }
    // 读取所有的输出帧
    while(true)
    {
        int ret = avcodec_receive_frame(context, frame);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            break;
        }
        else if(ret < 0)
        {
            return -1;
        }
        int data_size = av_get_bytes_per_sample(context->sample_fmt);
        if (data_size < 0)
        {
            return -1;
        }
        for(int i = 0; i < frame->nb_samples; i++)
        {
            for(int ch = 0; ch < context->channels; ch++)
            {
                memcpy(data_buff, frame->data[ch] + data_size * i, static_cast<size_type>(data_size));
                data_buff += data_size;
            }
        }
    }
    return static_cast<int>(data_buff - buff);
}

size_type from_file(const char *file_name, char *buff)
{
    const struct AVCodec *codec;
    struct AVCodecContext *c = NULL;
    struct AVCodecParserContext *parser = NULL;
    int ret;
    FILE *f;
    uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE], *data;
    size_type data_size, len, result_length = 0;
    struct AVPacket *packet;
    struct AVFrame *decoded_frame = NULL;
    bool invalid = false;

    packet = av_packet_alloc();

    // 查找mp3解码器
    codec = avcodec_find_decoder(AV_CODEC_ID_MP3);
    if(!codec)
    {
        return 0;
    }
    parser = av_parser_init((int)codec->id);
    if(!parser)
    {
        return 0;
    }
    c = avcodec_alloc_context3(codec);
    if(!c)
    {
        return 0;
    }
    // 打开解码器
    if(avcodec_open2(c, codec, NULL) < 0)
    {
        return 0;
    }
    f = fopen(file_name, "rb");
    if(!f)
    {
        return 0;
    }
    // 解码数据
    data = inbuf;
    data_size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);

    while(data_size > 0)
    {
        if(!decoded_frame)
        {
            if(!(decoded_frame = av_frame_alloc()))
            {
                return 0;
            }
        }

        ret = av_parser_parse2(parser, c, &packet->data, &packet->size, (uint8_t *)&data[0], (int)data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if(ret < 0)
        {
            return 0;
        }
        data += ret;
        data_size -= static_cast<size_type>(ret);

        if(packet->size > 0)
        {
            ret = decode(c, packet, decoded_frame, buff);
            if(ret < 0)
            {
                invalid = true;
                break;
            }
            buff += ret;
            result_length += static_cast<size_type>(ret);
        }

        if(data_size < AUDIO_REFILL_THRESH)
        {
            memmove(inbuf, data, data_size);
            data = inbuf;
            len = fread(data + data_size, 1, AUDIO_INBUF_SIZE - data_size, f);
            if (len > 0)
            {
                data_size += len;
            }
        }
    }
    if(invalid == false)
    {
        // 处理最后的数据
        packet->data = NULL;
        packet->size = 0;
        ret = decode(c, packet, decoded_frame, buff);
        if(ret > 0)
        {
            result_length += static_cast<size_type>(ret);
        }

    }
    fclose(f);

    avcodec_free_context(&c);
    av_parser_close(parser);
    av_frame_free(&decoded_frame);
    av_packet_free(&packet);
    return result_length;
}

int to_file(const char *data, size_type size, const char *file_name, enum FormatID type)
{
    struct AVCodec *codec = get_encoder(type);
    struct AVCodecContext *context = get_encoder_context(codec);
    if(context == NULL)
    {
        return -1;
    }
    context->bit_rate = 64000;//波特率
    context->sample_rate = 44100;//采样率
    context->sample_fmt = type == WAV ? AV_SAMPLE_FMT_S16 : AV_SAMPLE_FMT_S16P;
    context->channels = 2;
    context->channel_layout = (unsigned long long)av_get_default_channel_layout(2);
    context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    return write(data, size, file_name, context, codec);
}

int to_mp3(const char *data, size_type size, const char *file_name)
{ return to_file(data, size, file_name, MP3); }
int to_wav(const char *data, size_type size, const char *file_name)
{ return to_file(data, size, file_name, WAV); }
int to_flac(const char *data, size_type size, const char *file_name)
{ return to_file(data, size, file_name, FLAC); }
int to_ape(const char *data, size_type size, const char *file_name)
{ return to_file(data, size, file_name, APE); }

struct AVCodecContext *get_encoder_context(struct AVCodec *coder)
{
    if(coder == NULL) return NULL;
    return avcodec_alloc_context3(coder);
}

struct AVCodec *get_encoder(enum FormatID type)
{
    return avcodec_find_encoder(FormatTablep[type]);
}

struct AVFormatContext *get_format_context(const char *file_name)
{
    struct AVFormatContext *format = NULL;
    // 创建输出 Format 上下文
    int result = avformat_alloc_output_context2(&format, NULL, NULL, file_name);
    if(result < 0)
    {
        return NULL;
    }
    return format;
}

struct AVOutputFormat* get_output_format(const char *file_name,
                                         struct AVCodecContext *context,
                                         struct AVFormatContext *format,
                                         struct AVCodec *codec)
{
    struct AVOutputFormat *output_format = format->oformat;
    // 创建音频流
    struct AVStream *audio_stream = avformat_new_stream(format, codec);
    if(audio_stream == NULL)
    {
        avformat_free_context(format);
        return NULL;
    }

    // 设置流中的参数
    audio_stream->id = (int)format->nb_streams - 1;
    audio_stream->time_base.num = 1;
    audio_stream->time_base.den = 44100;
    if(0 > avcodec_parameters_from_context(audio_stream->codecpar, context))
    {
        avformat_free_context(format);
        return NULL;
    }

    // 打印FormatContext信息
    av_dump_format(format, 0, file_name, 1);

    // 打开文件IO
    if((output_format->flags & AVFMT_NOFILE) > 0)
    {
        if(0 > avio_open(&format->pb, file_name, AVIO_FLAG_WRITE))
        {
            avformat_free_context(format);
            return NULL;
        }
    }

    return output_format;
}

int write(const char *data, size_type size, const char *file_name, struct AVCodecContext *context, struct AVCodec *codec)
{
    struct AVFrame *frame = NULL;
    const char *src_data_pointer[1] = {NULL};
    struct AVPacket *packet = NULL;
    struct AVFormatContext *format = NULL;
    struct AVOutputFormat *output_format = NULL;
    size_type per_frame_data_size, count, i;
    int frame_count = 0, result;
    int64_t channel_layout = 0;
    SwrContext *swrContext = NULL;

    if(context == NULL || codec == NULL)
    {
        return -1;
    }
    result = avcodec_open2(context, codec, NULL);
    if(result < 0)
    {
        return -1;
    }
    format = get_format_context(file_name);
    if(format == NULL)
    {
        avcodec_free_context(&context);
        return -1;
    }
    output_format = get_output_format(file_name, context, format, codec);
    if(output_format == NULL)
    {
        avcodec_free_context(&context);
        return -1;
    }
    frame = av_frame_alloc();
    // 设置Frame的参数
    if((context->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) > 0)
    {
        frame->nb_samples = 10000;
    }
    else
    {
        frame->nb_samples = context->frame_size;
    }
    frame->format = context->sample_fmt;
    frame->channel_layout = context->channel_layout;

    // 申请数据内存
    result = av_frame_get_buffer(frame, 0);
    if(result < 0)
    {
        av_frame_free(&frame);
        avcodec_free_context(&context);
        return -1;
    }

    // 设置Frame为可写
    result = av_frame_make_writable(frame);
    if(result < 0)
    {
        av_frame_free(&frame);
        avcodec_free_context(&context);
        return -1;
    }

    per_frame_data_size = (size_type)frame->linesize[0];
    if(AV_CODEC_ID_MP3 == codec->id)
    {
        per_frame_data_size *= 2;
    }
    count = size / per_frame_data_size;
    if(size % (size_type)per_frame_data_size != 0)
    {
        count++;
    }

    for(i = 0; i < count; ++i)
    {
        // 创建Packet
        packet = av_packet_alloc();
        if(packet == NULL)
        {
            avcodec_free_context(&context);
            return -1;
        }
        av_init_packet(packet);

        if(i == count - 1)
        {
            per_frame_data_size = size % (size_type)per_frame_data_size;
        }

        // 设置数据
        if(codec->id == AV_CODEC_ID_PCM_S16LE)
        {
            // 合成WAV文件
            memset(frame->data[0], 0, per_frame_data_size);
            memcpy(frame->data[0], &(data[per_frame_data_size * i]), per_frame_data_size);
        }
        else
        {
            memset(frame->data[0], 0, (size_type)frame->linesize[0]);
            memset(frame->data[1], 0, (size_type)frame->linesize[0]);
            // 合成MP3文件
            channel_layout = av_get_default_channel_layout(2);

            // 格式转换 S16->S16P
            swrContext = swr_alloc_set_opts(NULL, channel_layout, AV_SAMPLE_FMT_S16P, 44100,
                                                        channel_layout, AV_SAMPLE_FMT_S16, 44100, 0, NULL);
            swr_init(swrContext);

            src_data_pointer[0] = &data[per_frame_data_size * i];

            swr_convert(swrContext, frame->data, frame->nb_samples, (const uint8_t **)src_data_pointer, frame->nb_samples);

            swr_free(&swrContext);
//            AVRational rational;
//            rational.den = context->sample_rate;
//            rational.num = 1;
//            frame->pts = av_rescale_q(0, rational, context->time_base);
        }

        frame->pts = frame_count++;
        // 发送Frame
        result = avcodec_send_frame(context, frame);
        if(result < 0)
        {
            continue;
        }

        // 接收编码后的Packet
        result = avcodec_receive_packet(context, packet);
        if (result < 0)
        {
            av_packet_free(&packet);
            continue;
        }

        // 写入文件
        av_packet_rescale_ts(packet, context->time_base, format->streams[0]->time_base);
        packet->stream_index = 0;
        result = av_interleaved_write_frame(format, packet);
        if(result < 0)
        {
            continue;
        }

        av_packet_free(&packet);
    }

    // 写入文件尾
    av_write_trailer(format);
    // 关闭文件IO
    avio_closep(&format->pb);
    // 释放内存
    av_frame_free(&frame);
    avcodec_free_context(&context);
    avformat_free_context(format);
    return 0;
}
