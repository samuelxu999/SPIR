"""
=====
monitor_sensor
=====
Created on May 6, 2017
@author: Xu Ronghua
@Email:  rxu22@binghamton.edu
@TaskDescription: This module read data from sensor and plot animation to monitor sensor data.
"""

import sys
import os
import RS232_module as MyData
import plot_animation as MyPlot
import time
import datetime
import configparser

'''
======================= global variable =======================
'''
logfile=""
log_en=0
MAX_SENSOR=10

'''
Function: Load configuration setting from config.txt file.   
@arguments: 
(out)  config_data         return parsed config data
'''  
def Load_Config():
    #new a RawConfigParser Objects
    myconfig = configparser.RawConfigParser()   
    
    #read data from config file
    configFilePath = r'Config.txt'
    myconfig.read(configFilePath)
    
    #get scantime duration and sensoring interval time.
    myscantime = myconfig.get('Sensor-config', 'SCANTIME')
    mysinterval = myconfig.get('Sensor-config', 'INTERVAL')
    
    config_data={}
    config_data['scantime']=myscantime
    config_data['interval']=mysinterval
    
    #return parsed config data
    return config_data

'''
Function: Save ls_record[] data as report:filename under current directory
@arguments: 
(in)    ls_record:       read ls_record[] to parse need information
(in)    filename:       set report file name
'''
def ExportData(ls_record,filename): 
    if(not os.path.exists(filename)):
        #open file as append mode
        tmp_file = open(filename, "a")
        #add title
        sensor_id=0
        while(sensor_id<len(ls_record)):
            if(sensor_id==0):
                tmp_file.write("\tDate-Time\t\t ")
            else:
                tmp_file.write("S-%s  " %(sensor_id))
            sensor_id+=1
        tmp_file.write("\n")
    else:
        #open file as append mode
        tmp_file = open(filename, "a")
    
    #append record to file
    tmp_id=0
    while(tmp_id<len(ls_record)):
        if(tmp_id==(len(ls_record)-1)):
            tmp_file.write("%s\n" %(ls_record[tmp_id]))
        else:
            tmp_file.write("%s, " %(ls_record[tmp_id]))
        tmp_id+=1
    #close file
    tmp_file.close() 

'''
Function: Monitor sensor data in online mode:
        1) read real-time sensor data vie RS232 communication channel
        2) plot line chart to visualize monitor data
        3) if enable '--log' option, sensor data will be saved as '@Date-@Time.log'
'''
def monitor_online(sensor_id):
    #initialize RS232 object-myserial
    myserial=MyData.RS232_Init()
    print("Application will use port: %s" %(myserial.port))
    
    #Open RS232 port
    MyData.RS232_Open(myserial)    
    if(not myserial.isOpen()):
        print("serial port not open successful, please check port status.")
        exit()
    
    #get config data by user
    myconfigdata=Load_Config()
    scantime= int(myconfigdata['scantime'])
    interval=float(myconfigdata['interval'])
    
    #set log file
    str_time=str(datetime.datetime.now())
    logfile=str_time.split()[0]
    logfile+="-"
    logfile+=str_time.split()[1][:-7]
    logfile=logfile.replace(':', '-')
    logfile+=".log"
    #logfile=datetime.datetime.strptime(str(datetime.datetime.now()), "%H:%M:%S")
    #print(logfile)
    
    #define to set multi-plot mode
    isMultiPlot=False
    #input value type valid, then operate based on parameter type: number or non-number
    if(sensor_id.isdigit()):
        sensor_id=int(sensor_id)
        if(not (sensor_id<=MAX_SENSOR and sensor_id>=1)):
            print("Invalid sensor id, should be range from 1 to %s" %(MAX_SENSOR))
            exit()
    else:
        if(sensor_id!='--'):
            print("Invalid sensor id, should be number, or use -- to display all channels data!")
            exit()
        isMultiPlot=True
    #plot_line()
    MyPlot.plot_online(sensor_id,isMultiPlot,MAX_SENSOR)
    
    #monitor sensor through endless loop
    monitorNum=0
    # we set monitor cycletime is 60s
    while(monitorNum<scantime):
        #data sensoring rate second
        #time.sleep(0.5)
        #read sensor data
        ls_sensor=MyData.Read_Sensor(myserial)
        
        #add sensor data to list for further data analysis
        ls_line=[]
        ls_line.append(str(datetime.datetime.now()))
        ls_line.append(ls_sensor)
        
        #extract sensor data and transfer to uniform format
        ls_record=MyData.ParseLine(ls_line)
        
        #export data to log file
        if(log_en):
            ExportData(ls_record,logfile)
        
        #update_line(hl,ls_record[0],ls_record[1])
        #MyPlot.update_line(monitorNum,ls_record[sensor_id])
        MyPlot.update_line(monitorNum,ls_record,sensor_id,isMultiPlot,MAX_SENSOR)
        
        #plot refresh rate second
        MyPlot.plt.pause(interval)
        
        #add monitor counter
        monitorNum=monitorNum+1
        
    #close RS232 port
    print("Close serial port %s"  %(myserial.port))
    MyData.RS232_Close(myserial)
    if(myserial.isOpen()):
        print("serial port close error!") 

'''
Function:read line contents from file
@arguments: 
(input)  filepath:       input file path
(out)    ls_lines:       return line list object
'''
def ReadLines(filepath):
    #define file handle to open select file
    fname = open(filepath, 'r')    
    #read text by line and saved as array list ls_lines
    ls_lines=fname.readlines()
    #close file
    fname.close()
    return ls_lines

'''
Function: split line string and saved record as array list.
@arguments: 
(in)    ls_line:           input line list to parse need information
(out)   ls_info:           return split line data
'''
def Parselines(ls_line):
    #Define ls_info[] to save split data    
    ls_info=[]
    
    #remove title from ls_line
    del ls_line[0]
    
    # 1.split each line record into ls_info[]
    for tmp_line in ls_line:
        ls_data=[]
        
        tmp_line=tmp_line.replace("\n" , "")            
        #remove empty line
        if(len(tmp_line.split())!=0):
            tmp_data=tmp_line.split(',')
            tmp_id=0
            while(tmp_id<len(tmp_data)):
                if(tmp_id==0):
                    ls_data.append(tmp_data[tmp_id])
                else:
                    #remove space padding in data field
                    ls_data.append(tmp_data[tmp_id].replace(" ",""))
                tmp_id+=1
            ls_info.append(ls_data)

    return ls_info
 
 
'''
Function: Monitor sensor data in offline mode:
        1) read sensor data log
        2) plot line chart to visualize monitor data
'''
def monitor_offline(logname, sensor_id):
    #read log file and save data as ls_line[]
    #print("Extract sensor data from '%s'." %(logname))
    ls_line=ReadLines(logname)    
    
    #handle line list data and save result as ls_info    
    ls_info=Parselines(ls_line)
    #print(ls_info[0])
    
    #input value type valid, then operate based on parameter type: number or non-number
    if(sys.argv[2].isdigit()):      
        if(not (int(sensor_id)<=MAX_SENSOR and int(sensor_id)>=1)):
            print("Invalid sensor id, should be range from 1 to %s" %(MAX_SENSOR))
            exit()
        MyPlot.plot_offline(ls_info,int(sensor_id))
    else:
        if(sensor_id!='--'):
            print("Invalid sensor id, should be number, or use -- to display all channels data!")
            exit() 
        MyPlot.plot_offlineAll(ls_info,MAX_SENSOR)  


'''
Function: Provide command line interface for user
'''        
if __name__ == "__main__":
    if(len(sys.argv)<2):
        print("Usage: %s --online|offline sensor_id(1~%s) --log(Optional)" %(sys.argv[0],MAX_SENSOR))
        exit()
    
    if(sys.argv[1]=="--online"):
        if(len(sys.argv)<3):
           print("1) Usage: %s --online sensor_id(1~%s) --log(Optional)" %(sys.argv[0],MAX_SENSOR))
           exit()
        if((len(sys.argv)==4) and sys.argv[3]=="--log"):
            log_en=1
        sensor_id=sys.argv[2]
        #call online mode
        monitor_online(sensor_id)
    
    elif(sys.argv[1]=="--offline"):
        if(len(sys.argv)<4):
           print("2) Usage: %s --offline sensor_id(1~%s) logname" %(sys.argv[0],MAX_SENSOR))
           exit()
        sensor_id=sys.argv[2]
        logname=sys.argv[3]

        #call offline mode        
        monitor_offline(logname,sensor_id)
    else:
        print("Usage: %s --online|offline --log(Optional)" %(sys.argv[0]))    