#ifndef POSITION__H__
#define POSITION__H__

#include <xc.h>
#include <stdio.h>
#include "nu32dip.h"
#include "encoder.h"
#include "current.h"
#include "utilities.h"

#define ARRAY_SIZE 2500

int read_position(void);
double read_degrees(void);
void reset_encoder_count(void);
void read_trajectory(void);
void output_position_plot_data(void);

void PositionControl_Startup(void);
void PID_Position_Control(void);

// Setters & Getters
void set_pos_pgain(float pgain);
float get_pos_pgain(void);

void set_pos_igain(float igain);
float get_pos_igain(void);

void set_pos_dgain(float dgain);
float get_pos_dgain(void);

void set_degrees(float degrees);
float get_degrees(void);

int get_pos_count(void);

void reset_pos(void);
void reset_pos_array(void);
#endif