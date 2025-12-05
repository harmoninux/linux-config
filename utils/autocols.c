/**
 * for Alpine Linux autologin
 */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>

#define ESC "\033"

int main() {
    struct winsize w = { 0, 0, 0, 0 };

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w)) {
        if (errno != EINVAL) {
            return 0;
        }
        memset(&w, 0, sizeof(w));
    }

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 3;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    const char *seq1 = ESC "7" ESC "[r" ESC "[999;999H" ESC "[6n";
    write(2, seq1, strlen(seq1));
    
    alarm(1);

    char buf[32] = {0};
    int n = read(0, buf, sizeof(buf)-1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    struct winsize tw = { 0, 0, 0, 0 };
    if (n > 0) {
        char *p = strchr(buf, '[');
        if (p) {
            unsigned int row = 0, col = 0;
            if (sscanf(p, "[%u;%uR", &row, &col) == 2) {
                tw.ws_row = row;
                tw.ws_col = col;
            }
        }
    }

    const char *seq2 = ESC "8";
    write(2, seq2, strlen(seq2));

    w.ws_col = tw.ws_col;
    ioctl(STDIN_FILENO, TIOCSWINSZ, (char *) &w);

    return 0;
}