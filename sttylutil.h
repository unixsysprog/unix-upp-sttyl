/*
 * sttylutil.h - declares and exposes functions used by other programs related
 * to sttyl.
 * date: 3/2/2017
 * author: Tasuku Miura
 */
#ifndef STTYLUTIL_H
#define STTYLUTIL_H

#include <termios.h>

void print_usage();
void display_info(struct termios*);
void set_specialchars(struct termios*, int, char**);
void set_modes(struct termios*, int, char**);
void check_valid(int argc, char** argv);

#endif
