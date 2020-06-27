/*
 * sttyltables.h - declares the arrays related to the different types of flags
 * available in the light weight version of stty.
 * date: 3/2/2017
 * author: Tasuku Miura
 */
#ifndef TABLES_H
#define TABLES_H

#include <termios.h>
#include <string.h>

struct flaginfo {
    int flag;
    char name[256];
};

extern const struct flaginfo local_modes[];
extern const struct flaginfo input_modes[];
extern const struct flaginfo output_modes[];
extern const struct flaginfo control_modes[];
extern const struct flaginfo special_chars[];

#endif
