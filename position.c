#include "position.h"

void __ISR(_TIMER_1_VECTOR, IPL3SOFT) PositionControl(void) {

}

void PositionControl_Startup(void) {

    // Timer 1, prescale 64
    TMR1 = 0;
    T1CONbits.TCKPS = 0b10;
    T1CONbits.TGATE = 0;
    T1CONbits.TCS = 0;
    PR1 = 3749;
    IPC1bits.T1IP = 3;
    IPC1bits.T1IS = 0;

}