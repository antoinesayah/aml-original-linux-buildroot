/**
 * \file feeder.c
 * \brief  Functions of Feeder
 * \version 1.0.0
 * \date 2011-03-08
 */
/* Copyright (C) 2007-2011, Amlogic Inc.
 * All right reserved
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <feeder.h>
#include <audiodsp.h>

/**
 * \brief get audio format
 * \return audio format on success otherwise AUDIO_FORMAT_UNKNOWN
 */
static audio_format_t get_audio_format(void)
{
    int fd;
    char format[21];
    int len;

    format[0] = 0;

    fd = open(FORMAT_PATH, O_RDONLY);
    if (fd < 0) {
        adec_print("amadec device not found");
        return AUDIO_FORMAT_UNKNOWN;
    }

    len = read(fd, format, 20);
    if (len > 0) {
        format[len] = 0;
    }
    if (strncmp(format, "NA", 2) == 0) {
        close(fd);
        return AUDIO_FORMAT_UNKNOWN;
    }

    adec_print("amadec format: %s", format);

    if (strncmp(format, "amadec_mpeg", 11) == 0) {
        close(fd);
        return AUDIO_FORMAT_MPEG;
    }

    if (strncmp(format, "amadec_pcm_s16le", 16) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_PCM_S16LE;
    }

    if (strncmp(format, "amadec_pcm_s16be", 16) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_PCM_S16BE;
    }

    if (strncmp(format, "amadec_pcm_u8", 13) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_PCM_U8;
    }

    if (strncmp(format, "amadec_adpcm", 12) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_ADPCM;
    }

    if (strncmp(format, "amadec_aac", 10) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_AAC;
    }

    if (strncmp(format, "amadec_ac3", 10) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_AC3;
    }

    if (strncmp(format, "amadec_alaw", 11) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_ALAW;
    }

    if (strncmp(format, "amadec_mulaw", 12) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_MULAW;
    }

    if (strncmp(format, "amadec_dts", 10) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_DTS;
    }

    if (strncmp(format, "amadec_flac", 11) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_FLAC;
    }

    if (strncmp(format, "amadec_cook", 11) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_COOK;
    }

    if (strncmp(format, "amadec_amr", 10) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_AMR;
    }

    if (strncmp(format, "amadec_raac", 11) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_RAAC;
    }

    if (strncmp(format, "amadec_wmapro", 13) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_WMAPRO;
    }

    if (strncmp(format, "amadec_wma", 10) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_FORMAT_WMA;
    }

    if (strncmp(format, "amadec_pcm_bluray", 10) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_AFORMAT_PCM_BLURAY;
    }
    if (strncmp(format, "amadec_alac", 11) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_AFORMAT_ALAC;
    }
    if (strncmp(format, "amadec_vorbis", 13) == 0) {
        /*TODO: get format/channel numer/sample rate etc */
        close(fd);
        return AUDIO_AFORMAT_VORBIS;
    }
    close(fd);

    adec_print("audio format unknow.");

    return AUDIO_FORMAT_UNKNOWN;
}

/**
 * \brief init feeder
 * \param audec pointer to audec
 * \return 0 on success otherwise -1 if an error occurred
 */
int feeder_init(aml_audio_dec_t *audec)
{
    int ret;
    dsp_operations_t *dsp_ops;

    dsp_ops = &audec->adsp_ops;

    audec->format = get_audio_format();
    if (audec->format == AUDIO_FORMAT_UNKNOWN) {
        adec_print("Unknown audio format!");
        return -1;
    }

    ret = audiodsp_init(dsp_ops);
    if (ret) {
        adec_print("audio dsp init failed!");
        return -1;
    }

    ret = audiodsp_start(audec);
    if (ret == 0) {
        dsp_ops->dsp_on = 1;
        dsp_ops->dsp_read = audiodsp_stream_read;
        dsp_ops->get_cur_pts = audiodsp_get_pts;
    } else {
        audiodsp_release(dsp_ops);
        dsp_ops->dsp_on = 0;
        dsp_ops->dsp_read = NULL;
        dsp_ops->get_cur_pts = NULL;

        /* TODO: amport init */
    }

    return ret;
}

/**
 * \brief release feeder
 * \param audec pointer to audec
 * \return 0 on success otherwise -1 if an error occurred
 */
int feeder_release(aml_audio_dec_t *audec)
{
    int ret;
    dsp_operations_t *dsp_ops;

    dsp_ops = &audec->adsp_ops;

    ret = audiodsp_stop(dsp_ops);
    if (ret) {
        adec_print("audiodsp stop failed!");
        return -1;
    }

    ret = audiodsp_release(dsp_ops);
    if (ret) {
        adec_print("audiodsp release failed!");
        return -1;
    }

    dsp_ops->dsp_on = 0;
    dsp_ops->dsp_read = NULL;

    return ret;
}