#include "nu32dip.h"          // config bits, constants, funcs for startup and UART
#include "encoder.h"
#include "utilities.h"
#include "ina219.h"
#include "current.h"
#include "position.h"
// include other header files here

#define BUF_SIZE 200

static volatile int encoder_count_offset = 0;

int main() 
{
  
  char buffer[BUF_SIZE];
  NU32DIP_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
  NU32DIP_GREEN = 1;  // turn off the LEDs
  NU32DIP_YELLOW = 1;        

  __builtin_disable_interrupts();
  // in future, initialize modules or peripherals here
  UART2_Startup();
  INA219_Startup();
  CurrentControl_Startup();
  __builtin_enable_interrupts();
  set_mode(IDLE);

  while(1)
  {
    NU32DIP_ReadUART1(buffer,BUF_SIZE); // we expect the next character to be a menu command
    NU32DIP_YELLOW = 1;                   // clear the error LED

    switch (buffer[0]) {
      case 'b':                   // read current sensor (mA)
      {
        char m[50];
        float current = INA219_read_current();
        sprintf(m, "%.2f\r\n", current);
        NU32DIP_WriteUART1(m);
        break;
      }

      case 'c':                     // encoder count
      {
        WriteUART2("a");
        while(!get_encoder_flag()){}
        set_encoder_flag(0);
        char m[50];
        int p = get_encoder_count() - encoder_count_offset;
        sprintf(m, "%d\r\n", p);
        NU32DIP_WriteUART1(m);
        break;
      }
      case 'd':                      // degrees of rotation
      {
        WriteUART2("a");
        while(!get_encoder_flag()){}
        set_encoder_flag(0);
        char m[50];
        int p = get_encoder_count() - encoder_count_offset;
        double degrees = ((double) p)/(4.0*COUNTS_PER_REV) * 360; 
        sprintf(m, "%.2f\r\n", degrees);
        NU32DIP_WriteUART1(m);
        break;
      }

      case 'e':                         // reset encoder count 
      {
        encoder_count_offset = get_encoder_count();
        break;
      }

      case 'f':                         // set PWM
      {
        signed int pwm;
        set_mode(PWM);
        NU32DIP_ReadUART1(buffer, BUF_SIZE);
        sscanf(buffer, "%d", &pwm);
        sprintf(buffer, "%d\r\n", pwm);
        set_pwm(pwm);
        pwm = get_pwm();
        NU32DIP_WriteUART1(buffer);
        break;
      }

      case 'g':                         // set current gains
      {
        float pgain_input, igain_input;
        NU32DIP_ReadUART1(buffer, BUF_SIZE);
        sscanf(buffer, "%f %f", &pgain_input, &igain_input);
        set_pgain(pgain_input);
        set_igain(igain_input);
        pgain_input = get_pgain();
        igain_input = get_igain();
        sprintf(buffer, "%.3f %.3f\r\n", pgain_input, igain_input);
        NU32DIP_WriteUART1(buffer);
        break;
      }

      case 'h':                         // get current gains
      {
        float pgain, igain;
        pgain = get_pgain();
        igain = get_igain();
        sprintf(buffer, "%.3f %.3f\r\n", pgain, igain);
        NU32DIP_WriteUART1(buffer);
        break;
      }

      case 'i':                         // set position gains
      {
        float pgain_input, igain_input, dgain_input;
        NU32DIP_ReadUART1(buffer, BUF_SIZE);
        sscanf(buffer, "%f %f %f", &pgain_input, &igain_input, &dgain_input);
        set_pos_pgain(pgain_input);
        set_pos_igain(igain_input);
        set_pos_dgain(dgain_input);
        pgain_input = get_pos_pgain();
        igain_input = get_pos_igain();
        dgain_input = get_pos_dgain();
        sprintf(buffer, "%.3f %.3f %.3f\r\n", pgain_input, igain_input, dgain_input);
        NU32DIP_WriteUART1(buffer);
        break;
      }

      case 'j':                         // get position gains
      {
        float pgain, igain, dgain;
        pgain = get_pos_pgain();
        igain = get_pos_igain();
        dgain = get_pos_dgain();
        sprintf(buffer, "%.3f %.3f %.3f\r\n", pgain, igain, dgain);
        NU32DIP_WriteUART1(buffer);
        break;
      }

      case 'k':                         // test current gains
      {
        float pgain, igain;
        reset_current_count();
        reset_current_error();
        set_mode(ITEST);
        while (get_mode() == ITEST) {}
        pgain = get_pgain();
        igain = get_igain();
        sprintf(buffer, "%.3f %.3f\r\n", pgain, igain);
        NU32DIP_WriteUART1(buffer);
        output_plot_data();
        break;
      }

      case 'l':                         // go to angle (deg)
      {
        reset_pos();
        reset_current_error();
        set_mode(HOLD);
        float degrees;
        NU32DIP_ReadUART1(buffer, BUF_SIZE);
        sscanf(buffer, "%f", degrees);
        set_degrees(degrees);
        break;
      }

      case 'p':                         // Turn off motor
      {
        set_mode(IDLE);
        NU32DIP_WriteUART1("Motor off\r\n");    // write from UART for validation
        break;
      }

      case 'r':
      {
        char m[50];
        int i = get_mode();
        switch (i) {
          case IDLE: { sprintf(m, "IDLE\r\n");  break;}
          case PWM:  {sprintf(m, "PWM\r\n");  break;}
          case ITEST: { sprintf(m, "ITEST\r\n");  break;}
          case HOLD: { sprintf(m, "HOLD\r\n");  break;}
          case TRACK: { sprintf(m, "TRACK\r\n"); break;}
        }
        NU32DIP_WriteUART1(m);
        break;
      }

      case 'x':                       // 
      {
        int m, n;
        NU32DIP_ReadUART1(buffer, BUF_SIZE);
        sscanf(buffer, "%d %d", &m, &n);
        sprintf(buffer, "%d\r\n", m + n);  // returns the sum of the two inputs
        NU32DIP_WriteUART1(buffer);
        break;
      }
      case 'q':
      {
        // handle q for quit. Later you may want to return to IDLE mode here. 
        set_mode(IDLE);
        break;
      }
      default:
      {
        NU32DIP_YELLOW = 0;  // turn on LED2 to indicate an error
        break;
      }
    }
  }
  return 0;
}
