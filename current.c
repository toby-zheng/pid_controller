#include "current.h"

signed int pwm;
static float igain, pgain;
volatile float current_error, i_current_error = 0;

volatile float read_current, ref_current, current;
static int current_count = 0;

float measured_current_array[SAMPLE_NUM];
float ref_array[SAMPLE_NUM];


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
                DIR_BIT = 0;
            }
            else {
                DIR_BIT = 1;
            }
            OC1RS = (unsigned int) (abs(pwm) * PR3/100);
            break;
        }

        case ITEST: {
            if (current_count < 25) {
                ref_current = 200;
            }
            else if (current_count < 50) {
                ref_current = -200;
            }
            else if (current_count < 75) {
                ref_current = 200;
            }
            else if (current_count < 100) {
                ref_current = -200;
            }
            else {
                set_mode(IDLE);
                break;
            }

            PI_Control();
            break;
        }

        case HOLD: {
            reset_current_count();  // prevent overflow
            PI_Control();
            break;
        }

        case TRACK: {
            reset_current_count();  // prevent overflow
            PI_Control();
            break;
        }

        default: {
            break;
        }
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

void PI_Control(void) {

    // Read Current, PI control for desired current
    read_current = INA219_read_current();
    current_error = read_current - ref_current;
    current = igain*i_current_error + pgain*current_error;

    // Bounds
    if (current > 100) {
        current = 100;
    }
    else if (current < -100) {
        current = -100;
    }

    // Direction Bit
    if (current > 0) {
        DIR_BIT = 1;
    }
    else {
        DIR_BIT = 0;
    }

    // Output Compare (PWM)
    OC1RS = (unsigned int) (abs(current) * PR3/100);

    // Save data in arrays for plotting
    measured_current_array[current_count] = read_current;
    ref_array[current_count] = ref_current;

    // Update integral current_error
    i_current_error += current_error;

    current_count ++;
}

void output_current_plot_data(void) {
    char data[50];
    sprintf(data, "%d\r\n", SAMPLE_NUM);
    NU32DIP_WriteUART1(data);
    for(int i = 0; i < SAMPLE_NUM; i++) {
        sprintf(data, "%.5f %.5f\r\n", ref_array[i], measured_current_array[i]);
        NU32DIP_WriteUART1(data);
    }
}

// Setters & Getters
void set_pwm(signed int pwm_input) {
    pwm = pwm_input;
}

signed int get_pwm(void) {
    return pwm;
}

void set_pgain(float pgain_input) {
    pgain = pgain_input;
}

float get_pgain(void) {
    return pgain;
}

void set_igain(float igain_input) {
    igain = igain_input;
}

float get_igain(void) {
    return igain;
}

void set_ref_current(float input_current) {
    ref_current = input_current;
}

float get_ref_current() {
    return ref_current;
}

void reset_ref_current() {
    ref_current = 0;
}

void reset_current_error(void) {
    current_error = 0;
    i_current_error = 0;
}

void reset_current_count(void) {
    current_count = 0;
}