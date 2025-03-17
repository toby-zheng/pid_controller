#ifndef POSITION__H__
#define POSITION__H__

#include <xc.h>
#include <stdio.h>
#include "nu32dip.h"
#include "encoder.h"
#include "current.h"
#include "utilities.h"

int read_position(void);
double read_degrees(void);
void reset_encoder_count(void);

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

void reset_pos(void);

#endif