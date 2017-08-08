"""
=====
RS232_module
=====
Created on May 6, 2017
@author: Xu Ronghua
@Email:  rxu22@binghamton.edu
@TaskDescription: This module provides RS232 communication function.
"""

import serial
import time
import array
import Utility as MyUtility

MAX_SENSOR=10

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
    config_data['port']=userconfig.getPort()
    config_data['baudrate']=userconfig.getBaudrate()
    
    #return parsed config data
    return config_data

'''
Function: RS232 initialize to setup default configuration.   
@arguments: 
(out)  ser                return initialized serial object
'''    
def RS232_Init():
    #get config data by user
    myconfigdata=Load_Config()
    
    ser = serial.Serial()
    ser.port = myconfigdata['port']
    ser.baudrate = myconfigdata['baudrate']
    ser.parity = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE
    ser.bytesize = serial.EIGHTBITS
    ser.timeout = None          #block read
    #ser.timeout = 1            #non-block read
    #ser.timeout = 2              #timeout block read
    ser.xonxoff = False     #disable software flow control
    ser.rtscts = False     #disable hardware (RTS/CTS) flow control
    ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
    ser.writeTimeout = 2     #timeout for write
    
    return ser

'''
Function: check whether port is available.   
@arguments: 
(in)  portName        input port name for check
''' 
def portIsUsable(portName):
    try:
       ser = serial.Serial(port=portName)
       return True
    except:
       return False

'''
Function: open RS232 port to set up communication.   
@arguments: 
(in)  obj_serial        input initialized serial object
'''  
def RS232_Open(obj_serial):
    if(obj_serial.isOpen()):
        obj_serial.close()
    obj_serial.open()

'''
Function: close RS232 port to tear down communication.   
@arguments: 
(in)  obj_serial        input initialized serial object
'''     
def RS232_Close(obj_serial):
    if(obj_serial.isOpen()):
        obj_serial.close()

'''
Function: parse received raw data and transfer to uniform data format.   
@arguments: 
(in)  ls_line        input received raw data from ls_line[]
'''  
def ParseLine(ls_line):
    ls_record=[]
    #get time and data list 
    read_time=ls_line[0][:-7]
    read_data=ls_line[1]    
    
    #add time to ls_record
    ls_record.append(read_time)
    
    #for each data to transfer data to decimal format
    for tmp_data in read_data:
        #print("%s %s" %(tmp_data[1:2],tmp_data[2:]))
        #print(int(('0x'+tmp_data[2:]),16))
        ls_record.append(str(int(('0x'+tmp_data[2:]),16)))
    
    return ls_record

'''
Function: read sensor from RS232 channel and return formatted data.   
@arguments: 
(in)   obj_serial         input initialized serial object
(out)  ls_data            return uniform formatted sensor data as ls_data[] 
'''   
def Read_Sensor(obj_serial):       
    ls_data=[]
    #read sensor data for all channel
    sensor_id=0
    while True:
        
        #flush input buffer, discarding all its contents
        obj_serial.flushInput() 
        #flush output buffer, aborting current output
        obj_serial.flushOutput()
    
        #write REQ command:FF 01 id FF
        obj_serial.write(b'\xff\x01')        
        obj_serial.write(array.array('B',[sensor_id]).tostring())
        obj_serial.write(b'\xff')               
        
        #wait for 25ms to read buffered data
        time.sleep(0.025)
        #sensor_data=0x00
        
        req_data=[]
        while obj_serial.inWaiting() > 0:
            #ls_data.append(obj_serial.read(3).hex())
            req_data.append(obj_serial.read(8).hex())
        
        #verify header of packet and extract sensor value
        if(req_data[0][0:8]=='fe01ffff'):
            ls_data.append(req_data[0][8:14])
        else:
            ls_data.append('ff00000')
        
        #ls_data.append(sensor_data)
        
        #switch monitor channel of sensor
        sensor_id = sensor_id + 1
        if (sensor_id > MAX_SENSOR-1):
            break
        
    #return data list
    return ls_data
       
if __name__ == "__main__":
    myserial=RS232_Init()
    RS232_Open(myserial) 
    print(Read_Sensor(myserial))
