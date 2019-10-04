#include "io.h"

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>

static const enum AVCodecID FormatTablep[] =
{
    AV_CODEC_ID_MP3,
    AV_CODEC_ID_PCM_S16LE,
    AV_CODEC_ID_MPEG4,
    AV_CODEC_ID_FLAC,
    AV_CODEC_ID_APE
};

size_type from_file(const char *file_name, char *data)
{
    return 1;
}

int to_file(const char *data, size_type size, const char *file_name, enum FormatID type)
{
    struct AVCodec *codec = get_encoder(type);
    struct AVCodecContext *context = get_encoder_context(codec);
    if(context == NULL)
    {
        return -1;
    }
    context->bit_rate = 64000;
    context->sample_rate = 44100;
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
int to_mpeg(const char *data, size_type size, const char *file_name)
{ return to_file(data, size, file_name, MPEG); }
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
    int result = avcodec_parameters_from_context(audio_stream->codecpar, context);
    if(result < 0)
    {
        avformat_free_context(format);
        return NULL;
    }

    // 打印FormatContext信息
    av_dump_format(format, 0, file_name, 1);

    // 打开文件IO
    if((output_format->flags & AVFMT_NOFILE) > 0)
    {
        result = avio_open(&format->pb, file_name, AVIO_FLAG_WRITE);
        if(result < 0)
        {
            avformat_free_context(format);
            return NULL;
        }
    }

    return output_format;
}

int write(const char *data, size_type size, const char *file_name, struct AVCodecContext *context, struct AVCodec *codec)
{
    if(context == NULL || codec == NULL)
    {
        return -1;
    }
    int result = avcodec_open2(context, codec, NULL);
    if(result < 0)
    {
        return -1;
    }
    struct AVFormatContext *format = get_format_context(file_name);
    if(format == NULL)
    {
        avcodec_free_context(&context);
        return -1;
    }
    struct AVOutputFormat *output_format = get_output_format(file_name, context, format, codec);
    if(output_format == NULL)
    {
        avcodec_free_context(&context);
        return -1;
    }
    struct AVFrame *frame = av_frame_alloc();
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

    size_type per_frame_data_size = (size_type)frame->linesize[0];
    if(AV_CODEC_ID_MP3 == codec->id)
    {
        per_frame_data_size *= 2;
    }
    size_type count = size / per_frame_data_size;
    if(size % (size_type)per_frame_data_size != 0)
    {
        count++;
    }

    int frameCount = 0;
    for (size_type i = 0; i < count; ++i)
    {
        // 创建Packet
        struct AVPacket *packet = av_packet_alloc();
        if (packet == NULL)
        {
            avcodec_free_context(&context);
            return -1;
        }
        av_init_packet(packet);

        if (i == count - 1)
        {
            per_frame_data_size = size % (size_type)per_frame_data_size;
        }

        // 设置数据
        if (codec->id == AV_CODEC_ID_PCM_S16LE)
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
            int64_t channel_layout = av_get_default_channel_layout(2);

            // 格式转换 S16->S16P
            SwrContext *swrContext = swr_alloc_set_opts(NULL, channel_layout, AV_SAMPLE_FMT_S16P, 44100,
                                                        channel_layout, AV_SAMPLE_FMT_S16, 44100, 0, NULL);
            swr_init(swrContext);

            const char *src_data_pointer[1] = {NULL};
            src_data_pointer[0] = &data[per_frame_data_size * i];

            swr_convert(swrContext, frame->data, frame->nb_samples, (const uint8_t **)src_data_pointer, frame->nb_samples);

            swr_free(&swrContext);
//            AVRational rational;
//            rational.den = context->sample_rate;
//            rational.num = 1;
            //frame->pts = av_rescale_q(0, rational, context->time_base);
        }

        frame->pts = frameCount++;
        // 发送Frame
        result = avcodec_send_frame(context, frame);
        if (result < 0)
            continue;

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
        if (result < 0)
            continue;

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

