#include "position.h"

volatile int encoder_count_offset = 0;

volatile float pos_pgain, pos_igain, pos_dgain, pos_degrees;

volatile float pos_error = 0, i_pos_error = 0, d_pos_error = 0, pos_error_prev = 0 ,pos_current = 0;



void __ISR(_TIMER_1_VECTOR, IPL3SOFT) PositionControl(void) {
    switch (get_mode()) {
        case HOLD: {
            PID_Position_Control();
            break;
        }

        default: {
            break;
        }
    }
    IFS0bits.T1IF = 0;
}

int read_position(void) {
    WriteUART2("a");
    while(!get_encoder_flag()){}
    set_encoder_flag(0);
    int p = get_encoder_count() - encoder_count_offset;
    return p;
}

double read_degrees(void) {
    int pos = read_position();
    double degrees = ((double) pos)/(4.0*COUNTS_PER_REV) * 360.0;   
    return degrees;
}

void reset_encoder_count(void) {
    encoder_count_offset = get_encoder_count();
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

    IFS0bits.T1IF = 0;
    T1CONbits.ON = 1;
    IEC0bits.T1IE = 1;

}

void PID_Position_Control(void) {
    // encoder
    float count, ref_count, curr_error;
    count = read_position();

    // convert degrees to counts
    ref_count = pos_degrees * (4.0 *COUNTS_PER_REV) / 360.0;

    pos_error = (count - ref_count);
    d_pos_error = (pos_error - pos_error_prev)/ (1.0/200.0);

    pos_current = pos_pgain * pos_error + pos_igain * i_pos_error + pos_dgain * d_pos_error;

    set_ref_current(pos_current);

    i_pos_error += pos_error;
    pos_error_prev = pos_error;

}


// Setters & Getters

void set_pos_pgain(float pgain) {
    pos_pgain = pgain;
}

float get_pos_pgain(void) {
    return pos_pgain;
}

void set_pos_igain(float igain) {
    pos_igain = igain;
}

float get_pos_igain(void) {
    return pos_igain;
}

void set_pos_dgain(float dgain) {
    pos_dgain = dgain;
}  

float get_pos_dgain(void) {
    return pos_dgain;
}

void set_degrees(float degrees) {
    pos_degrees = degrees;
}

float get_degrees(void) {
    return pos_degrees;
}

void reset_pos(void) {
    pos_error = 0;
    i_pos_error = 0;
    d_pos_error = 0;
    pos_current = 0;

}