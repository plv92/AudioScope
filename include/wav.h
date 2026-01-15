/*
** EPITECH PROJECT, 2026
** AudioScope
** File description:
** wav
*/

#ifndef WAV_H
    #define WAV_H

    #include <stdint.h>

typedef struct wav_header_s {
    char     riff[4];
    uint32_t overall_size;
    char     wave[4];
    char     fmt_chunk_marker[4];
    uint32_t length_of_fmt;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byterate;
    uint16_t block_align;
    uint16_t bits_per_sample;
} wav_header_t;

int read_wav_header(const char *path, wav_header_t *header);

#endif
