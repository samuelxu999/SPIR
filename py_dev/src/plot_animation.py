"""
=====
Plot_animation
=====
Created on May 6, 2017
@author: Xu Ronghua
@Email:  rxu22@binghamton.edu
@TaskDescription: This module will plot animation for sensor data.
"""

import time
import matplotlib.pyplot as plt
import Utility as MyUtility

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

ls_dispchan=[]

#SUB_PLOT_ROW="4"
#SUB_PLOT_COLUMN="2"
INCREAMENT_X=60
INCREAMENT_Y=10
INIT_YLIM=1024
INIT_XLIM=60

'''
Function: Load configuration setting from config.txt file.   
@arguments: 
(out)  config_data         return parsed config data
'''  
def Load_Config():
    #initialize UserConfig()
    userconfig = MyUtility.UserConfig()
    config_data={}
    
    config_data={}
    config_data['dispchan']=str(userconfig.getDispchan()).replace(" ","")
    
    #return parsed config data
    return config_data

'''
Function: initialize multi-plot grid layout.   
@arguments: 
(out)  SUB_PLOT_ROW            return plot grid row count
(out)  SUB_PLOT_COLUMN         return plot grid column count 
'''
def Init_PlotGrid():
    #get config data by user
    myconfigdata=Load_Config()
    max_plot=0
    
    #clear ls_dispchan[]
    del ls_dispchan[:]
    #for each data in list to check channel enable flag
    for i in myconfigdata['dispchan'][1:-1].split(','):
        ls_dispchan.append(int(i))
        if(i=='1'):
            max_plot+=1
    
    #initialize sub-plot grid
    if(max_plot==1):
        SUB_PLOT_ROW="1"
        SUB_PLOT_COLUMN="1"
    elif(max_plot<=2):
        SUB_PLOT_ROW="2"
        SUB_PLOT_COLUMN="1"
    elif(max_plot<=4):
        SUB_PLOT_ROW="2"
        SUB_PLOT_COLUMN="2"
    elif(max_plot<=6):
        SUB_PLOT_ROW="3"
        SUB_PLOT_COLUMN="2"
    else:
        SUB_PLOT_ROW="4"
        SUB_PLOT_COLUMN="2"
    return SUB_PLOT_ROW,SUB_PLOT_COLUMN,max_plot

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
    #ax.set_ylim(0, INIT_YLIM)
    #ax.set_xlim(0, INIT_XLIM)
    
    #graph original size
    xdata, ydata = [], []

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
    SUB_PLOT_ROW,SUB_PLOT_COLUMN,max_plot=Init_PlotGrid()
    
    #used for counting sensor id
    sensor_id=0
    #used for counting sub-plot id
    plot_id=0
        
    while(sensor_id<max_channel):   
        #count sensor_id
        sensor_id+=1
        
        #Skip disabled channel on circuit board
        if(ls_dispchan[sensor_id-1]==0):
            continue
    
        #calculate sub-plot id
        plot_id+=1
        if(plot_id>(int(SUB_PLOT_ROW)*int(SUB_PLOT_COLUMN))):
            break
        #set sub plot layout
        plotnum=SUB_PLOT_ROW+SUB_PLOT_COLUMN+str(plot_id)
        
        #new axis object and add to subplot
        ax = fig.add_subplot(plotnum)
        ax.grid()
        #new line object for axis
        line, = ax.plot([], [], lw=1)
        
        #labels
        plt.xlabel("Scan Number")
        plt.ylabel("Sensor-%s data" %(sensor_id))
        #graph original size
        ax.set_ylim(0, INIT_YLIM)
        ax.set_xlim(0, INIT_XLIM)
        
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
    t = int(data_x)
    y = int(data_y)
    ls_xdata[0].append(t)
    ls_ydata[0].append(y)
    xmin, xmax = ls_ax[0].get_xlim()
    ymin, ymax = ls_ax[0].get_ylim()
    
    #auto expend display boundary based on real value
    if(t==0):
        #initialize y-boundary
        ls_ax[0].set_ylim(y-INCREAMENT_Y, y+INCREAMENT_Y)
        ls_ax[0].figure.canvas.draw()
    else:    
        #rearrange x-boundary
        if t >= xmax:
            ls_ax[0].set_xlim(xmin, xmax+INCREAMENT_X)
            ls_ax[0].figure.canvas.draw()
        #rearrange y-boundary
        if y >= ymax:
            ls_ax[0].set_ylim(ymin, y+INCREAMENT_Y)
            ls_ax[0].figure.canvas.draw()
        if y <= ymin:
            ls_ax[0].set_ylim(y-INCREAMENT_Y, ymax)
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
    SUB_PLOT_ROW,SUB_PLOT_COLUMN,max_plot=Init_PlotGrid()
    
    #used for counting sensor id
    sensor_id=0
    #used for counting sub-plot id
    plot_id=0

    # update the data
    t = data_x
    while(sensor_id<max_channel):   
        #count sensor_id
        sensor_id+=1
        
        
        #Skip disabled channel on circuit board
        if(ls_dispchan[sensor_id-1]==0):
             continue
        
        plot_id+=1
        if(plot_id>(int(SUB_PLOT_ROW)*int(SUB_PLOT_COLUMN))):
            break
        
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
    
    #clear global list
    del ls_ax[:]
    del lines[:]
    del ls_xdata[:]
    del ls_ydata[:]
    
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
    plt.xlabel("Scan Number")
    plt.ylabel("Sensor-%s data" %(sensor_id))
    
    #plot data
    plt.plot(timeslot, SensorData, lw=1.0)
    plt.grid()
    
    #show plot
    plt.show()
    
'''
Function: run on offline mode, plot all channel data on one chart.
@arguments: 
(in)    ls_records:           input line list to parse need information
(in)    max_channel:            limit max channels
'''
def plot_offlineAll(ls_records, max_channel):
    #initialize plot grid
    SUB_PLOT_ROW,SUB_PLOT_COLUMN,max_plot=Init_PlotGrid()
    
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
        
        #Skip disabled channel on circuit board
        if(ls_dispchan[sensor_id-1]==0):
             continue
            
        #prepare data for sub-plot
        i=1
        for record in ls_records:
            timeslot.append(i)
            SensorData.append(record[sensor_id])                
            i+=1
        
        #calculate sub-plot id
        plot_id+=1
        if(plot_id>(int(SUB_PLOT_ROW)*int(SUB_PLOT_COLUMN))):
            break
        
        #set sub plot layout
        plotnum=SUB_PLOT_ROW+SUB_PLOT_COLUMN+str(plot_id)
        
        #-------subplot for each sensor data-------    
        plt.subplot(int(plotnum))
        #labels
        plt.xlabel("Scan Number")
        plt.ylabel("Sensor-%s data" %(sensor_id))    
        #plot data
        plt.plot(timeslot, SensorData, lw=1.0)
        plt.grid()
    #show plot
    plt.show()
    
#if __name__ == "__main__":
#    testfun()
    
    