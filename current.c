#include "current.h"

extern int pwm;

//5KHz ISR
void __ISR(_TIMER_2_VECTOR, IPL5SOFT) CurrentControl(void)
{   
    switch (get_mode()) {

        case IDLE: {
        // Stop sending PWM
        pwm = 0;    // set pwm to 0 so the next PWM mode it doesn't auto-start
        OC1RS = 0;  
        break;
        }

        case PWM: {
            // User set PWM
            if (pwm < 0) {
                LATAbits.LATA1 = 0;
            }
            else {
                LATAbits.LATA1 = 1;
            }
            OC1RS = (unsigned int) (abs(pwm) * PR3/100);
        }
        break;
    }
    
    IFS0bits.T2IF = 0;
}

void CurrentControl_Startup(void) {
    // RPA0 set to OC1
    RPA0R = 5;

    // RA1 is DIR
    TRISAbits.TRISA1 = 0;

    // Timer 2, prescale = 8, 5KHz ISR
    TMR2 = 0;
    T2CONbits.TCKPS = 0b011;
    T2CONbits.TGATE = 0;
    T2CONbits.TCS = 0;
    PR2 = 1199;
    IPC2bits.T2IP = 5;
    IPC2bits.T2IS = 0;

    // Timer 3, prescale = 0, 20KHz PWM
    TMR3 = 0;
    T3CONbits.TCKPS = 0;
    T3CONbits.TGATE = 0;
    T3CONbits.TCS = 0;
    PR3 = 2399;


    // PWM OC1
    OC1CONbits.OCTSEL = 1;  // Timer 3 PWM
    OC1CONbits.OCM = 6;     // PWM w/o Fault

    T2CONbits.ON = 1;
    T3CONbits.ON = 1;
    OC1CONbits.ON = 1;
    
    IEC0bits.T2IE = 1;
}
