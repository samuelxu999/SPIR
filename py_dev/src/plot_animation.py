"""
=====
Plot_animation
=====
Created on May 6, 2017
@author: Xu Ronghua
@Email:  rxu22@binghamton.edu
@TaskDescription: This module will plot animation for sensor data.
"""

import matplotlib.pyplot as plt
import time

'''
======================= global variable =======================
'''
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.grid()
xdata, ydata = [], []


'''
Function: initialize plot default configuration.   
@arguments: 
(out)  line            return line object 
'''
def init(sensor_id):
    
    #labels
    plt.xlabel("Time slot (Sec)")
    plt.ylabel("Sensor-%s data" %(sensor_id))
    #graph original size
    ax.set_ylim(0, 1200)
    ax.set_xlim(0, 10)
    #graph original size
    del xdata[:]
    del ydata[:]
    line.set_data(xdata, ydata)
    return line,

'''
Function: update line object based on inputed parameter 
@arguments: 
(out)  line            return line object
(out)  line            return line object 
'''
def update_line(data_x,data_y):
    # update the data
    t = data_x
    y = data_y
    xdata.append(t)
    ydata.append(y)
    xmin, xmax = ax.get_xlim()

    if t >= xmax:
        ax.set_xlim(xmin, xmax+10)
        ax.figure.canvas.draw()
    line.set_data(xdata, ydata)

    return line


'''
Function: initialize plot_animation related default setting.   
@arguments: 
'''
def plot_online(sendor_id):
    #Set up plot default setting 
    init(sendor_id)
    #Turn interactive mode on
    plt.ion()
    #Show plot
    plt.show()

'''
Function: split line string and saved record as array list.
@arguments: 
(in)    ls_line:           input line list to parse need information
(out)   ls_info:           return split line data
'''
def plot_offline(ls_records, sensor_id):
    #define list for plot
    x=[]
    SensorData=[]
    
    #prepare data for plot
    i=1
    for record in ls_records:
        x.append(i)
        SensorData.append(record[sensor_id])
        i+=1
    
    #define figure
    #plt.figure(42)
    
    #labels
    plt.xlabel("Time slot (Sec)")
    plt.ylabel("Sensor-%s data" %(sensor_id))
    
    #plot data
    plt.plot(x, SensorData)
    
    #show plot
    plt.show()
    
#if __name__ == "__main__":
#    testfun()
    
    