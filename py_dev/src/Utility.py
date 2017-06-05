"""
=====
Utility
=====
Created on June 3, 2017
@author: Xu Ronghua
@Email:  rxu22@binghamton.edu
@TaskDescription: This module provide utility class for user to call function.
"""

import sys
import configparser

'''
Function: Manage configuration setting in config.txt file.   
''' 
class UserConfig(object):
    
    def __init__(self):  
        #new a RawConfigParser Objects
        self.myconfig = configparser.RawConfigParser()
        
        #read data from config file
        self.configFilePath = r'Config.txt'

    
    def getPort(self):
        self.myconfig.read(self.configFilePath)
        myport = self.myconfig.get('RS232-config', 'port')
        return myport
    
    def setPort(self,_data):
        self.myconfig.read(self.configFilePath)
        self.myconfig.set('RS232-config', 'port', _data)
        self.myconfig.write(open(self.configFilePath, 'w'))

    def getBaudrate(self):
        self.myconfig.read(self.configFilePath)
        myport = self.myconfig.get('RS232-config', 'baudrate')
        return myport
    
    def setBaudrate(self,_data):
        self.myconfig.read(self.configFilePath)
        self.myconfig.set('RS232-config', 'baudrate', _data)
        self.myconfig.write(open(self.configFilePath, 'w'))
        
    def getScantime(self):
        self.myconfig.read(self.configFilePath)
        myport = self.myconfig.get('Sensor-config', 'scantime')
        return myport
    
    def setScantime(self,_data):
        self.myconfig.read(self.configFilePath)
        self.myconfig.set('Sensor-config', 'scantime', _data)
        self.myconfig.write(open(self.configFilePath, 'w'))
        
    def getInterval(self):
        self.myconfig.read(self.configFilePath)
        myport = self.myconfig.get('Sensor-config', 'interval')
        return myport
    
    def setInterval(self,_data):
        self.myconfig.read(self.configFilePath)
        self.myconfig.set('Sensor-config', 'interval', _data)
        self.myconfig.write(open(self.configFilePath, 'w'))
        
    def getDispchan(self):
        self.myconfig.read(self.configFilePath)
        myport = self.myconfig.get('Sensor-config', 'disp_chan')
        return myport
    
    def setDispchan(self,_data):
        self.myconfig.read(self.configFilePath)
        self.myconfig.set('Sensor-config', 'disp_chan', _data)
        self.myconfig.write(open(self.configFilePath, 'w'))

if __name__ == "__main__":

    '''userconfig = UserConfig()
    
    print(userconfig.getPort()) 
    print(userconfig.getBaudrate()) 
    print(userconfig.getScantime()) 
    print(userconfig.getInterval())
    
    ls_chan=[1,0,1,0,1,0,1,0,0,0]
    print(ls_chan)
    userconfig.setDispchan(ls_chan)
    print(userconfig.getDispchan())'''
    