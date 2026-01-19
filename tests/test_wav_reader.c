/*
** EPITECH PROJECT, 2026
** AudioScope
** File description:
** test_wav_reader
*/

#include <criterion/criterion.h>
#include <stdio.h>
#include "wav.h"

Test(wav, invalid_file) {
    wav_header_t header;
    cr_assert(wav_parse_header("fichier_inexistant.wav", &header) == WAV_ERR_FILE);
}

Test(wav, not_a_wav_file) {
    FILE *f = fopen("not_a_wav.txt", "w");
    fprintf(f, "Hello world!\n");
    fclose(f);
    wav_header_t header;
    cr_assert(wav_parse_header("not_a_wav.txt", &header) == WAV_ERR_FORMAT);
    remove("not_a_wav.txt");
}

Test(wav, wrong_riff_header) {
    FILE *f = fopen("bad_riff.wav", "wb");
    fwrite("NOPE", 1, 4, f); // Not RIFF
    int zero = 0;
    fwrite(&zero, 4, 1, f);
    fwrite("WAVE", 1, 4, f);
    fclose(f);
    wav_header_t header;
    cr_assert(wav_parse_header("bad_riff.wav", &header) == WAV_ERR_FORMAT);
    remove("bad_riff.wav");
}

Test(wav, wrong_wave_header) {
    FILE *f = fopen("bad_wave.wav", "wb");
    fwrite("RIFF", 1, 4, f);
    int zero = 0;
    fwrite(&zero, 4, 1, f);
    fwrite("NOPE", 1, 4, f); // Not WAVE
    fclose(f);
    wav_header_t header;
    cr_assert(wav_parse_header("bad_wave.wav", &header) == WAV_ERR_FORMAT);
    remove("bad_wave.wav");
}

Test(wav, unsupported_format) {
    FILE *f = fopen("unsupported_fmt.wav", "wb");
    // RIFF header
    fwrite("RIFF", 1, 4, f);
    int size = 36;
    fwrite(&size, 4, 1, f);
    fwrite("WAVE", 1, 4, f);
    // fmt chunk
    fwrite("fmt ", 1, 4, f);
    int fmt_size = 16;
    fwrite(&fmt_size, 4, 1, f);
    short audio_format = 6; // Not PCM
    short num_channels = 1;
    int sample_rate = 44100;
    int byte_rate = 44100 * 2;
    short block_align = 2;
    short bits_per_sample = 16;
    fwrite(&audio_format, 2, 1, f);
    fwrite(&num_channels, 2, 1, f);
    fwrite(&sample_rate, 4, 1, f);
    fwrite(&byte_rate, 4, 1, f);
    fwrite(&block_align, 2, 1, f);
    fwrite(&bits_per_sample, 2, 1, f);
    // data chunk
    fwrite("data", 1, 4, f);
    int data_size = 0;
    fwrite(&data_size, 4, 1, f);
    fclose(f);
    wav_header_t header;
    cr_assert(wav_parse_header("unsupported_fmt.wav", &header) == WAV_ERR_UNSUPPORTED);
    remove("unsupported_fmt.wav");
}

Test(wav, minimal_valid_wav) {
    FILE *f = fopen("valid.wav", "wb");
    // RIFF header
    fwrite("RIFF", 1, 4, f);
    int size = 36;
    fwrite(&size, 4, 1, f);
    fwrite("WAVE", 1, 4, f);
    // fmt chunk
    fwrite("fmt ", 1, 4, f);
    int fmt_size = 16;
    fwrite(&fmt_size, 4, 1, f);
    short audio_format = 1;
    short num_channels = 1;
    int sample_rate = 44100;
    int byte_rate = 44100 * 2;
    short block_align = 2;
    short bits_per_sample = 16;
    fwrite(&audio_format, 2, 1, f);
    fwrite(&num_channels, 2, 1, f);
    fwrite(&sample_rate, 4, 1, f);
    fwrite(&byte_rate, 4, 1, f);
    fwrite(&block_align, 2, 1, f);
    fwrite(&bits_per_sample, 2, 1, f);
    // data chunk
    fwrite("data", 1, 4, f);
    int data_size = 0;
    fwrite(&data_size, 4, 1, f);
    fclose(f);
    wav_header_t header;
    cr_assert(wav_parse_header("valid.wav", &header) == WAV_OK);
    cr_assert_eq(header.audio_format, 1);
    cr_assert_eq(header.num_channels, 1);
    cr_assert_eq(header.sample_rate, 44100);
    cr_assert_eq(header.bits_per_sample, 16);
    cr_assert_eq(header.data_size, 0);
    remove("valid.wav");
}