/*
** EPITECH PROJECT, 2026
** AudioScope
** File description:
** wav
*/

#ifndef WAV_H
    #define WAV_H
    #include <stdint.h>

typedef enum {
    WAV_OK = 0,
    WAV_ERR_FILE = 1,
    WAV_ERR_FORMAT = 2,
    WAV_ERR_UNSUPPORTED = 3,
    WAV_ERR_IO = 4
} wav_error_t;

typedef struct wav_header_s {
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint16_t bits_per_sample;
    uint32_t data_size;
} wav_header_t;

wav_error_t wav_parse_header(const char *filename, wav_header_t *header);

#endif
