/**
 * sttyl - is a light weight version of stty, and attempts to mimic some of the
 * functionality provided by stty.
 * date: 3/2/2017
 * author: Tasuku Miura
 */
#include    <ctype.h>
#include    <stdio.h>
#include    <termios.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/ioctl.h>
#include    "sttyltables.h"
#include    "sttylutil.h"
/**
 ** showttyl 
 ** Displays some of current tty settings and capable of setting select number
 ** of modes.
 **/


int main(int argc, char**argv)
{
    struct  termios ttyinfo;               /* this struct holds tty info */
    

    if (tcgetattr(0 , &ttyinfo) == -1 ){   /* get info */
        perror( "cannot get params about stdin");
        exit(1);
    }
                       
    if (argc < 2) {
        display_info(&ttyinfo);
    } else {
        //TODO: the characters after special chars should not trigger invalid
        //response...this needs to be fixed.
        check_valid(argc, argv);
        set_specialchars(&ttyinfo, argc, argv);
        set_modes(&ttyinfo, argc, argv);
    }

   
    return 0;
}
