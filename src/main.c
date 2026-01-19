/*
** EPITECH PROJECT, 2026
** AudioScope
** File description:
** main
*/

#include <stdio.h>
#include "wav.h"

int main(int ac, char **av)
{
    if (ac < 2) {
        printf("Usage: %s <wav_file>\n", av[0]);
        return 1;
    }
    wav_header_t header;
    wav_error_t err = wav_parse_header(av[1], &header);
    if (err != WAV_OK) {
        printf("Error reading WAV file: %d\n", err);
        return 84;
    }
    printf("WAV File Info:\n");
    printf(" Audio Format: %u\n", header.audio_format);
    printf(" Number of Channels: %u\n", header.num_channels);
    printf(" Sample Rate: %u\n", header.sample_rate);
    printf(" Bits per Sample: %u\n", header.bits_per_sample);
    printf(" Data Size: %u bytes\n", header.data_size);
    return 0;
}
