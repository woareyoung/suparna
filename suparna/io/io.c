#include "io.h"

#include <libavcodec/avcodec.h>

static enum AVCodecID FormatTablep[] =
{
    AV_CODEC_ID_MP3,
    AV_CODEC_ID_PCM_S16LE,
    AV_CODEC_ID_MPEG4,
    AV_CODEC_ID_FLAC,
    AV_CODEC_ID_APE
};

int from_file(const char *file_name, char *data)
{
    return 1;
}

int to_file(const char *data, const char *file_name, size_type size, enum FormatID type)
{
    struct AVCodecContext *context = get_encoder_context(type);
    context->bit_rate = 64000;
    context->sample_rate = 44100;
    write(data, file_name, size, context);
    return 1;
}

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

struct AVCodecContext *get_encoder_context(enum FormatID type)
{
    enum AVCodecID codec_id = FormatTablep[type];
    struct AVCodec *code = avcodec_find_encoder(codec_id);
    if(code == NULL)
    {
        return NULL;
    }
    return avcodec_alloc_context3(code);
}

int write(const char *data, const char *file_name, size_type size, AVCodecContext *context)
{

}
