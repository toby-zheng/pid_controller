#ifndef UTILITIES__H__
#define UTILITIES__H__

enum mode_t 
{
    IDLE,
    PWM, 
    ITEST, 
    HOLD, 
    TRACK
};
int get_mode(void);
void set_mode(int m);

#endif