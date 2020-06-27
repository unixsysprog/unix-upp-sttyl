/**
 * sttylutil.c - utility functions to show information related to tty settings
 * to terminal.
 * date: 3/2/2017
 * author: Tasuku Miura
 */
#include    "sttylutil.h"
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/ioctl.h>
#include    "sttyltables.h"


/*
 * Prints the usage info.
 */
void print_usage()
{
    printf("Usage: [SETTING] ...\n"
           "Special characters:\n"
           "erase CHAR    CHAR will erase the last character typed\n"
           "intr CHAR     CHAR will send an interrupt signal\n"
           "kill CHAR     CHAR will erase the current line\n"
           "...\n"
           "[control | input | output | local] ...\n"
           "-brkint       turns off brkint\n"
           "brkint        turns on brkint\n"
           "...\n");
}

void showsize()
/*
 * Prints the terminal window size. 
 */
{
    struct winsize winp;                /* This struct holds tty size info */

    if (ioctl(0, TIOCGWINSZ, &winp) < 0) {
        perror("ioctl() failed. ");
        exit(1);
    }
    printf("rows %hu; ",winp.ws_row);
    printf("columns %hu;\n",winp.ws_col);
} 

void showbaud( int thespeed )
/*
 * Prints the speed in english.
 * @arg: thespeed - speed defined in termios.h
 */
{
    printf("speed ");
    switch ( thespeed ){
        case B300:  printf("300 ");    break;
        case B600:  printf("600 ");    break;
        case B1200: printf("1200 ");   break;
        case B1800: printf("1800 ");   break;
        case B2400: printf("2400 ");   break;
        case B4800: printf("4800 ");   break;
        case B9600: printf("9600 ");   break;
        case B19200: printf("19200 "); break;
        case B38400: printf("38400 "); break;
        default:    printf("Fast ");   break;
    }
    printf("baud; ");
}

void showspecial(struct termios *ttyp, const struct flaginfo bits[])
/*
 * Displays the special chars.
 * @arg: ttyp - pointer to termios struct.
 * @arg: bitnames - an array of flag info structs.
 */
{
    int i = 0;
    for (i = 0; bits[i].flag != -1 ; i++) {
        printf("%s = ^%c; ", bits[i].name,
                            ttyp->c_cc[bits[i].flag]-1+'A');
    }
    printf("\n");
}

char* negate_name(char* name)
/*
 * Negates name by prepend "~" to name.
 * @arg: name - name to negate.
 * @ret: retname - negated name.
 */ 
{
    char* retname = malloc(strlen(name) + 2);
    if (retname == NULL) {
        perror("malloc");
        exit(1);
    }

    strcpy(retname, "-");
    strcat(retname, name);
    return retname;
} 

void showflags(struct termios *ttyp, const struct flaginfo bits[], tcflag_t tcf)
/*
 * Displays the flags associated with flaginfo struct.
 * @arg: bitnames - an array of flag info structs
 */
{
    int i = 0;
    for (i = 0; bits[i].flag != -1 ; i++)
        if (tcf & bits[i].flag) {
            printf("%s ", bits[i].name);
        } else {
            char* negname = negate_name((char*)bits[i].name);
            printf("%s ", negname);
            free(negname);
        }
        
    printf("\n");
}

/*
 * Displays the info related to stty, when no argument is specified.
 * @arg: termios_p - a pointer to struct termios.
 */
void display_info(struct termios *ttyp)
{
    showbaud (cfgetospeed( ttyp));    /* get + show baud rate */
    showsize();                         /* show size of tty */
    showspecial(ttyp, special_chars);   
    showflags(ttyp, control_modes, ttyp->c_cflag);
    showflags(ttyp, input_modes, ttyp->c_iflag);
    showflags(ttyp, output_modes, ttyp->c_oflag);
    showflags(ttyp, local_modes, ttyp->c_lflag);
}

/*
 * Sets special chars specified as input arg.
 * @arg: ttyp - pointer to termios struct.
 * @arg: argv - array of command line args.
 */
void set_specialchars(struct termios *ttyp, int argc, char**argv)
{
    int i = 0;
    int j = 0;   

    for (i = 1; i < argc; i++) {
        for (j = 0; special_chars[j].flag != -1; j++) {
            if (strcmp(argv[i], special_chars[j].name) == 0) {
                if (argv[i+1] == NULL) {
                    print_usage();
                    exit(1);
                }

                ttyp->c_cc[special_chars[j].flag] = argv[i+1][0];
                break;
            }
        }
    }
    if (tcsetattr( 0, TCSADRAIN, ttyp) != 0) {
        perror("tcsetattr");
        exit(1);
    }
}

tcflag_t switch_flag(int on, char* n1, char* n2, const struct flaginfo bits[], tcflag_t tcf)
/* Switchs flag on and off depending if user specified on or off. The function
 * iterates through struct to find any matches.
 * @arg: on - flag indicating if mode is on or off.
 * @arg: n1 - name if off was specified.
 * @arg: n2 - name if on was specified.
 * @arg: bits - flaginfo struct.
 * @arg: tcf - flag of mode, where bits contain the relevant information.
 * @ret: tcf - transformed bits, read to use for setting.
 */
{
    int i = 0;
    for (i = 0; bits[i].flag != -1; i++) {
        if (!on) {
            if (strcmp(n1, bits[i].name) == 0) {
                tcf &= ~bits[i].flag;
                break;
            }
        } else {
            if (strcmp(n2, bits[i].name) == 0) {
                tcf |= bits[i].flag;
                break;
            }
        }
    } 

    return tcf;
}

/* Sets the modes specified by the arguments passed on command line.
 * @arg: ttyp - pointer to termios struct.
 * @arg: argc - number of args passed via command line.
 * @arg: argv - array of args of type string.
 */
void set_modes(struct termios *ttyp, int argc, char** argv)
{
    int i = 0;
    int on = 1;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] ==  '-') 
            on = 0;
        
        char* tmp = malloc(strlen(argv[i] - 1));
        if (tmp == NULL) {
            perror("malloc");
            exit(1);
        }
    
        int j = 0;
        for (j = 0; argv[i][j+1] != '\0'; j++)
            tmp[j] = argv[i][j+1];
        tmp[j] = '\0';
           
        ttyp->c_iflag = switch_flag(on, tmp, argv[i], input_modes, ttyp->c_iflag);
        ttyp->c_oflag = switch_flag(on, tmp, argv[i], output_modes, ttyp->c_oflag);
        ttyp->c_lflag = switch_flag(on, tmp, argv[i], local_modes, ttyp->c_lflag);
        ttyp->c_cflag = switch_flag(on, tmp, argv[i], control_modes, ttyp->c_cflag);
        free(tmp);
    }
    if (tcsetattr( 0, TCSADRAIN, ttyp) != 0) {
        perror("tcsetattr");
        exit(1);
    }
}

int check_modes(const struct flaginfo bits[], int found, char* name)
/* Checks to see if name is in bits[]
 * @arg: bits - array of flaginfo structs.
 * @arg: found - state of where or not the argument has been found.
 * @arg: name - name of flag
 * @ret: retfound - returns 1 if found, or the state of found prior to call.
 */
{
    int retfound = found;
    int i = 0;
    for (i = 0; bits[i].flag != -1; i++) {
        if (strcmp(bits[i].name, name) == 0) {
            retfound = 1;
            break;
        }
    }
    return retfound;
}

/* General check to see if any of the arguments are invalid.
 * @arg: argc - number of arguments.
 * @arg: argv - array of arguments.
 * 
 * Note: does not interrupt the flow of the program. Will just output the
 * invalid argument to stdout.
 */
void check_valid(int argc, char** argv)
{
    int i = 0;
    for (i = 1; i < argc; i++) {
        int flag = 0;
        char* tmp = malloc(strlen(argv[i] -1));
        if (tmp == NULL) {
            perror("malloc");
            exit(1);
        }

        if (argv[i][0] == '-') {
            flag = 1;
            int j = 0;
            for (j = 0; argv[i][j+1] != '\0'; j++)
                tmp[j] = argv[i][j+1];
            tmp[j] = '\0';
        }

        char* name;
        if (flag) {
           name = tmp;
        } else {
           name = argv[i];
        }

        int found = 0;
        found = check_modes(special_chars, found, name);
        found = check_modes(input_modes, found, name);
        found = check_modes(output_modes, found, name);
        found = check_modes(control_modes, found, name);
        found = check_modes(local_modes, found, name);

        if (!found)
            printf("invalid argument: %s\n", argv[i]);
        free(tmp);

    }
}

         
