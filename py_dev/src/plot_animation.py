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
#fig, ax = plt.subplots()
fig = plt.figure()
ls_ax=[]

#line, = ax.plot([], [], lw=2)
lines=[]

ls_xdata, ls_ydata = [], []
#xdata, ydata = [], []

SUB_PLOT_ROW="4"
SUB_PLOT_COLUMN="2"
INCREAMENT_X=60
INIT_XLIM=1200
INIT_YLIM=60

'''
Function: initialize single plot default configuration.   
@arguments: 
(out)  line            return line object 
'''
def init_singleplot(sensor_id):
    ax=plt.axes()
    ax = fig.add_subplot(111)
    ax.grid()
    line, = ax.plot([], [], lw=1)
    
    #labels
    plt.xlabel("Time slot")
    plt.ylabel("Sensor-%s data" %(sensor_id))
    #graph original size
    ax.set_ylim(0, INIT_XLIM)
    ax.set_xlim(0, INIT_YLIM)
    
    #graph original size
    xdata, ydata = [], []
    del ls_xdata[:]
    del ls_ydata[:]
    line.set_data(xdata, ydata)
    
    #add local xdata, ydata to global list ls_xdata,ls_ydata 
    ls_xdata.append(xdata)
    ls_ydata.append(ydata)
    
    #add sub-plot to list object
    lines.append(line)
    ls_ax.append(ax)
    #return line,
    
'''
Function: initialize multi-plot default configuration.   
@arguments: 
(in)  max_channel            input max channels limitation
'''
def init_multiplot(max_channel):
    #used for counting sensor id
    sensor_id=0
    #used for counting sub-plot id
    plot_id=0
    
    #clear ls_xdata and ls_ydata
    del ls_xdata[:]
    del ls_ydata[:]
        
    while(sensor_id<max_channel):   
        #count sensor_id
        sensor_id+=1
        
        #Skip channel 6 and 9 on circuit board
        if(sensor_id==7 or sensor_id==10):
            continue
    
        #calculate sub-plot id
        plot_id+=1
        #plotnum=420+plot_id
        plotnum=SUB_PLOT_ROW+SUB_PLOT_COLUMN+str(plot_id)
        
        #new axis object and add to subplot
        ax = fig.add_subplot(plotnum)
        ax.grid()
        #new line object for axis
        line, = ax.plot([], [], lw=1)
        
        #labels
        plt.xlabel("Time slot")
        plt.ylabel("Sensor-%s data" %(sensor_id))
        #graph original size
        ax.set_ylim(0, INIT_XLIM)
        ax.set_xlim(0, INIT_YLIM)
        
        #graph original size
        xdata, ydata = [], []
        line.set_data(xdata, ydata)
        
        #add local xdata, ydata to global list ls_xdata,ls_ydata 
        ls_xdata.append(xdata)
        ls_ydata.append(ydata)
            
        #add sub-plot to list object
        lines.append(line)
        ls_ax.append(ax)

'''
Function: update line object based on inputed parameter 
@arguments: 
(in)  data_x            input new x value
(in)  data_y            input new y value
'''
def update_line(data_x,ls_data,sensor_id,multiplot_flag,max_channel):
    if(multiplot_flag):
        update_multiline(data_x,ls_data,max_channel)
        #print("Stub-update_multiline")
    else:
        update_singleline(data_x,ls_data[sensor_id])
    
'''
Function: update line object based on inputed parameter 
@arguments: 
(in)  data_x            input new x value
(in)  data_y            input new y value
'''
def update_singleline(data_x,data_y):
    # update the data
    t = data_x
    y = data_y
    ls_xdata[0].append(t)
    ls_ydata[0].append(y)
    xmin, xmax = ls_ax[0].get_xlim()
    
    #rearrange x-boundary
    if t >= xmax:
        ls_ax[0].set_xlim(xmin, xmax+INCREAMENT_X)
        ls_ax[0].figure.canvas.draw()
    
    # update line by set updated data list
    lines[0].set_data(ls_xdata[0], ls_ydata[0])

'''
Function: update multiple line objects based on inputed parameter 
@arguments: 
(in)  data_x            input new x value
(in)  ls_data            input updated data value list
(in)  max_channel        input max channels limitation
'''
def update_multiline(data_x,ls_data,max_channel):
    #used for counting sensor id
    sensor_id=0
    #used for counting sub-plot id
    plot_id=0

    # update the data
    t = data_x
    while(sensor_id<max_channel):   
        #count sensor_id
        sensor_id+=1
        
        #Skip channel 6 and 9 on circuit board
        if(sensor_id==7 or sensor_id==10):
            continue
        
        plot_id+=1
        
        y = ls_data[sensor_id]
        ls_xdata[plot_id-1].append(t)
        ls_ydata[plot_id-1].append(y)
        xmin, xmax = ls_ax[plot_id-1].get_xlim()
        
        #rearrange x-boundary
        if t >= xmax:
            ls_ax[plot_id-1].set_xlim(xmin, xmax+INCREAMENT_X)
            ls_ax[plot_id-1].figure.canvas.draw()
        
        # update line by set updated data list
        lines[plot_id-1].set_data(ls_xdata[plot_id-1], ls_ydata[plot_id-1])

'''
Function: initialize plot_animation related default setting.   
@arguments: 
'''
def plot_online(sensor_id,multiplot_flag,max_channel):
    #Set up plot default setting 
    if(multiplot_flag):
        init_multiplot(max_channel)
    else:
        init_singleplot(sensor_id)
    
    #Turn interactive mode on
    plt.ion()
    #Show plot
    plt.show()

'''
Function: run on offline mode, plot selected channel data on one chart.
@arguments: 
(in)    ls_records:           input line list to parse need information
(in)    sensor_id:            input selected sensor id to plot single channel plot
'''
def plot_offline(ls_records, sensor_id):
    #define list for plot
    timeslot=[]
    SensorData=[]
    
    #prepare data for plot
    i=1
    for record in ls_records:
        timeslot.append(i)
        SensorData.append(record[sensor_id])
        i+=1
    
    #define figure
    #plt.figure(42)
    
    #labels
    plt.xlabel("Time slot")
    plt.ylabel("Sensor-%s data" %(sensor_id))
    
    #plot data
    plt.plot(timeslot, SensorData)
    
    #show plot
    plt.show()
    
'''
Function: run on offline mode, plot all channel data on one chart.
@arguments: 
(in)    ls_records:           input line list to parse need information
(in)    max_channel:            limit max channels
'''
def plot_offlineAll(ls_records, max_channel):
    #used for counting sensor id
    sensor_id=0
    #used for counting sub-plot id
    plot_id=0
    while(sensor_id<max_channel):
       
        #define list for sub-plot
        timeslot=[]
        SensorData=[]
        
        #count sensor_id
        sensor_id+=1
        
        #Skip channel 6 and 9 on circuit board
        if(sensor_id==7 or sensor_id==10):
            continue
            
        #prepare data for sub-plot
        i=1
        for record in ls_records:
            timeslot.append(i)
            SensorData.append(record[sensor_id])                
            i+=1
        
        #calculate sub-plot id
        plot_id+=1
        #plotnum=420+plot_id
        plotnum=SUB_PLOT_ROW+SUB_PLOT_COLUMN+str(plot_id)
        
        #-------subplot for each sensor data-------    
        plt.subplot(int(plotnum))
        #labels
        plt.xlabel("Time slot")
        plt.ylabel("Sensor-%s data" %(sensor_id))    
        #plot data
        plt.plot(timeslot, SensorData)
    #show plot
    plt.show()
    
#if __name__ == "__main__":
#    testfun()
    
    