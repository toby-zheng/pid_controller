import matplotlib.pyplot as plt
import serial
from statistics import mean 

# Current Arrays
def read_plot_matrix(ser):
    # Include gains on graph
    gain_read = ser.read_until(b'\n').decode('utf-8')
    gain_read = gain_read.split()
    pgain = float(gain_read[0])
    igain = float(gain_read[1])

    n_str = ser.read_until(b'\n'); # get the number of data points to receive
    n_int = int(n_str) # turn it into an int
    print('Data length = ' + str(n_int))
    ref = []
    data = []
    data_received = 0
    while data_received < n_int:
        dat_str = ser.read_until(b'\n'); # get the data as a string, ints seperated by spaces
        dat_int = list(map(float,dat_str.split())) # now the data is a list
        ref.append(dat_int[0])
        data.append(dat_int[1])
        data_received = data_received + 1
    meanzip = zip(ref,data)
    meanlist = []
    for i,j in meanzip:
        meanlist.append(abs(i-j))
    score = mean(meanlist)
    t = range(len(ref)) # index array

    # plot data
    plt.plot(t,ref,'r*-',t,data,'b*-')
    text_str = 'Proportional Gain = ' + str(pgain) + ' (mA^-1)'  + '\nIntegral Gain = ' + str(igain) + ' ((mA*s)^-1)'
    plt.text(0 , -190, text_str, fontsize = 8, bbox = {'facecolor': 'purple', 'alpha': 0.5, 'pad': 10})
    plt.title('Score = ' + str(score))
    plt.ylabel('Current (mA)')
    plt.xlabel('Step')
    plt.show()