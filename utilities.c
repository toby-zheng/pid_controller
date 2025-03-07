#include  "utilities.h"
volatile enum mode_t mode;

int get_mode(void) {
    return mode;
}

void set_mode(int m) {
    mode = m;
}