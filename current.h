#ifndef CURRENT__H__
#define CURRENT__H__

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "nu32dip.h"
#include "ina219.h"
#include "utilities.h"

#define SAMPLE_NUM 100
#define DIR_BIT LATAbits.LATA1

void CurrentControl_Startup(void);
void PI_Control(void);
void output_current_plot_data(void);

// Setters & Getters
void set_pwm(signed int pwm_input);
signed int get_pwm(void);

void set_pgain(float pgain_input);
float get_pgain(void);

void set_igain(float igain_input);
float get_igain(void);

void set_ref_current(float current);
float get_ref_current(void);

void reset_ref_current(void);
void reset_current_error(void);
void reset_current_count(void);

#endif
