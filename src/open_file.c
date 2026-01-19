/*
** EPITECH PROJECT, 2026
** AudioScope
** File description:
** open_file
*/


#include "open_file.h"

FILE *open_file_read(const char *path)
{
    FILE *file;

    if (!path)
        return NULL;
    file = fopen(path, "rb");
    if (!file)
        return NULL;
    return file;
}