# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib

import serial
from plot_arrays import *
ser = serial.Serial('COM3',230400)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop
while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\tb: Read Current Sensor (mA) \tc: Encoder Count \td: Angle') # '\t' is a tab
    print('\te: Reset Encoder Count \t\tf: Set PWM \t\tg: Set Current Gains')
    print('\th: Read Current Gains \t\tk: Test Current Gains \tp: Power off')
    print('\tr:Read Mode \t\tq: Quit')

    # read the user's choice
    selection = input('\nENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()); # .encode() turns the string into a char array

    if (selection == 'b'):
        out = float(ser.read_until(b'\n'))
        print('Current (mA): ' + str(out) + '\n')

    elif(selection == 'c'):
        out = int(ser.read_until(b'\n'))
        print("Encoder count: " + str(out) + '\n')

    elif(selection == 'd'):
        out = float(ser.read_until(b'\n'))
        print("Angle (degrees): " + str(out) + '\n')

    elif(selection == 'e'):
        print("Reset Encoder Count\n")

    elif(selection == 'f'):
        pwm_str = input('Set PWM (-100 to 100): ')
        ser.write((str(pwm_str) + '\n').encode())

        pwm_return = ser.read_until(b'\n')
        return_str = int(pwm_return)
        print('PWM set to ' + str(return_str) + '\n')

    elif(selection == 'g'):
        print("Set Current Gains:")
        proportional_gain = float(input('\tEnter Proportional Gain: '))
        integral_gain = float(input('\tEnter Integral Gain: '))
        ser.write((str(proportional_gain) + ' ' + str(integral_gain) + '\n').encode())

        gain_read = ser.read_until(b'\n').decode('utf-8')
        gain_read = gain_read.split()
        proportional_gain = float(gain_read[0])
        integral_gain = float(gain_read[1])
        print("Read Current Gains:")
        print("\tProportional Gain: {}\n\tIntegral Gain: {}\n".format(proportional_gain, integral_gain))

    elif(selection == 'h'):
        print("Read Current Gains:")
        gain_read = ser.read_until(b'\n').decode('utf-8')
        gain_read = gain_read.split()
        proportional_gain = float(gain_read[0])
        integral_gain = float(gain_read[1])
        print("\tProportional Gain: {}\n\tIntegral Gain: {}\n".format(proportional_gain, integral_gain))
    
    elif(selection == 'k'):
        print("Test Current Gains:")
        read_plot_matrix(ser)

        
    elif(selection == 'p'):
        motor_return = ser.read_until(b'\n').decode('utf-8')
        print(str(motor_return) + '\n')
    
    elif(selection == 'r'):
        out = ser.read_until(b'\n').decode('utf-8')
        print('Mode: ' + str(out) + '\n')

    elif(selection == 'x'):
        # inputs
        m_str = input('Enter one number: ')
        n_str = input('Enter another number: ')
        m_int = int(m_str)
        n_int = int(n_str)
        print('Numbers are ' + str(m_int) + ' and ' + str(n_int) + '\n')    
        ser.write((str(m_int) + ' ' + str(n_int) + '\n').encode())  # sent as format "num1  num2"
                                                                    # separated by a string
        m_str = ser.read_until(b'\n')   # read the summed number
        m_int = int(m_str)
        print('Got back: ' + str(m_int) + '\n')

    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True; # exit client
        # be sure to close the port
        ser.close()
    elif (selection == 'r'):
        print('Reset')
    else:
        print('Invalid Selection ' + selection_endline)



