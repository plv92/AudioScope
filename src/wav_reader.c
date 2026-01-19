/*
** EPITECH PROJECT, 2026
** AudioScope
** File description:
** wav_reader
*/

#include <stdio.h>
#include <string.h>
#include "wav.h"
#include "open_file.h"

/**
 * @brief Recherche et lit le chunk "data" pour obtenir la taille des données audio.
 *
 * Parcourt les chunks jusqu'à trouver "data" et stocke sa taille dans la structure header.
 *
 * @param file Pointeur vers le fichier ouvert en lecture binaire.
 * @param header Pointeur vers la structure à remplir.
 * @return WAV_OK si le chunk est trouvé et lu, sinon un code d'erreur.
 */
wav_error_t read_data_chunk(FILE *file, wav_header_t *header)
{
    char chunk_id[4];
    uint32_t chunk_size;

    // Search for "data" chunk
    while (1) {
        if (fread(chunk_id, 1, 4, file) != 4)
            return WAV_ERR_IO;
        if (fread(&chunk_size, 4, 1, file) != 1)
            return WAV_ERR_IO;
        if (memcmp(chunk_id, "data", 4) == 0)
            break;
        if (fseek(file, chunk_size, SEEK_CUR) != 0)
            return WAV_ERR_IO;
    }
    header->data_size = chunk_size;
    return WAV_OK;
}

/**
 * @brief Vérifie que le fichier commence par les entêtes "RIFF" et "WAVE".
 *
 * @param file Pointeur vers le fichier ouvert en lecture binaire.
 * @return WAV_OK si les entêtes sont valides, sinon un code d'erreur.
 */
wav_error_t check_riff_wave(FILE *file)
{
    char riff_id[4];
    char wave_id[4];

    if (fread(riff_id, 1, 4, file) != 4)
        return WAV_ERR_IO;
    if (memcmp(riff_id, "RIFF", 4) != 0)
        return WAV_ERR_FORMAT;
    if (fseek(file, 4, SEEK_CUR) != 0) // skip chunk size
        return WAV_ERR_IO;
    if (fread(wave_id, 1, 4, file) != 4)
        return WAV_ERR_IO;
    if (memcmp(wave_id, "WAVE", 4) != 0)
        return WAV_ERR_FORMAT;
    return WAV_OK;
}

/**
 * @brief Recherche et lit le chunk "fmt " pour extraire les informations audio.
 *
 * Parcourt les chunks jusqu'à trouver "fmt ". Remplit la structure header avec
 * le format audio, le nombre de canaux, la fréquence d'échantillonnage et les bits par échantillon.
 *
 * @param file Pointeur vers le fichier ouvert en lecture binaire.
 * @param header Pointeur vers la structure à remplir.
 * @return WAV_OK si le chunk est trouvé et lu, sinon un code d'erreur.
 */
wav_error_t read_fmt_chunk(FILE *file, wav_header_t *header)
{
    char chunk_id[4];
    uint32_t chunk_size, sample_rate;
    uint16_t audio_format, num_channels, bits_per_sample;

    while(1) {
        if (fread(chunk_id, 1, 4, file) != 4)
            return WAV_ERR_IO;
        if (fread(&chunk_size, 4, 1, file) != 1)
            return WAV_ERR_IO;
        if (memcmp(chunk_id, "fmt ", 4) == 0)
            break;
        if (fseek(file, chunk_size, SEEK_CUR) != 0)
            return WAV_ERR_IO;
    }
    if (chunk_size < 16)
        return WAV_ERR_FORMAT;
    if (fread(&audio_format, 2, 1, file) != 1 ||
        fread(&num_channels, 2, 1, file) != 1 ||
        fread(&sample_rate, 4, 1, file) != 1 ||
        fseek(file, 6, SEEK_CUR) != 0 || // skip byte rate
        fread(&bits_per_sample, 2, 1, file) != 1)
        return WAV_ERR_IO;
    if (audio_format != 1) // PCM format
        return WAV_ERR_UNSUPPORTED;
    header->audio_format = audio_format;
    header->num_channels = num_channels;
    header->sample_rate = sample_rate;
    header->bits_per_sample = bits_per_sample;
    if ((chunk_size - 16) > 0 && fseek(file, chunk_size - 16, SEEK_CUR) != 0)
        return WAV_ERR_IO;
    return WAV_OK;
}

/**
 * @brief Parse le header d'un fichier WAV PCM et remplit la structure header.
 *
 * Ouvre le fichier, vérifie les entêtes, lit les chunks "fmt " et "data", puis ferme le fichier.
 *
 * @param filename Nom du fichier WAV à lire.
 * @param header Pointeur vers la structure à remplir.
 * @return WAV_OK si tout s'est bien passé, sinon un code d'erreur.
 */
wav_error_t wav_parse_header(const char *filename, wav_header_t *header)
{
    FILE *file = open_file_read(filename);
    wav_error_t err;

    if (!file)
        return WAV_ERR_FILE;
    err = check_riff_wave(file);
    if (err != WAV_OK) {
        fclose(file);
        return err;
    }
    err = read_fmt_chunk(file, header);
    if (err != WAV_OK) {
        fclose(file);
        return err;
    }
    err = read_data_chunk(file, header);
    if (err != WAV_OK) {
        fclose(file);
        return err;
    }
    fclose(file);
    return WAV_OK;
}