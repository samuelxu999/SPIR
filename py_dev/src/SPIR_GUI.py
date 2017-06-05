"""
=====
SPIR_GUI
=====
Created on June 1, 2017
@author: Xu Ronghua
@Email:  rxu22@binghamton.edu
@TaskDescription: This module provide GUI for user to manage config data and start monitor task.
"""

from PyQt5.QtWidgets import *
from PyQt5.QtGui import QIcon,QFont,QPixmap
from PyQt5.QtCore import QCoreApplication, Qt

import sys
import os
import subprocess
import Utility as MyUtility
import RS232_module as MyData
import monitor_sensor as MySensor
    
class SPIR_FORM(QWidget):
    '''SPIR_FORM construction function'''
    def __init__(self):
        super().__init__()        
        self.initUI()
        
    
    #initialize UI form    
    
    '''Initialize UI components and layout'''
    def initUI(self):
        #================== construct main window form ==================
        self.resize(600, 450)
        #self.setFixedWidth(600)
        #self.setFixedHeight(450)
        
        self.center()
        self.setWindowTitle('SPIR Sensor Management Tools')
        self.setWindowIcon(QIcon('res/BU_icon.jpg')) 
        
        #set tip
        #QToolTip.setFont(QFont('SansSerif', 10))
        
        #set statusBar
        #self.statusBar().showMessage('Ready')
        
        #set menu bar related action
        '''exitAction = QAction(QIcon('res/exit.jpg'), '&Exit', self)        
        exitAction.setShortcut('Ctrl+Q')
        exitAction.setStatusTip('Exit application')
        exitAction.triggered.connect(qApp.quit)
        
        #add menu bar
        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(exitAction)'''
        
        #add toolbar
        '''self.toolbar = self.addToolBar('Exit')
        self.toolbar.addAction(exitAction)'''         
        
        #new userconfig object to manage config data
        self.userconfig = MyUtility.UserConfig()
        
        #=========================== construct widget control =================================
        self.port_Label = QLabel('Port:')
        self.port_Edit = QLineEdit()
        self.port_Edit.setText(self.userconfig.getPort())
        
        self.baudrate_Label = QLabel('Baudrate:')
        self.baudrate_Edit = QLineEdit()
        self.baudrate_Edit.setText(self.userconfig.getBaudrate())
        
        self.scantime_Label = QLabel('Scantime:')
        self.scantime_Edit = QLineEdit()
        self.scantime_Edit.setText(self.userconfig.getScantime())
        
        self.interval_Label = QLabel('Interval:')
        self.interval_Edit = QLineEdit()
        self.interval_Edit.setText(self.userconfig.getInterval())
        
        #for each data in ls_dispchan to check channel enable flag
        ls_dispchan=[]
        sensor_list=str(self.userconfig.getDispchan()).replace(" ","")
        for i in sensor_list[1:-1].split(','):
            ls_dispchan.append(int(i))
        
        #------------ sensor display checkbox ------------------    
        self.sensor1_cb = QCheckBox('Sensor-1')
        if(ls_dispchan[0]==1):
            self.sensor1_cb.setCheckState(Qt.Checked)
        else:
            self.sensor1_cb.setCheckState(Qt.Unchecked)
            
        self.sensor2_cb = QCheckBox('Sensor-2')
        if(ls_dispchan[1]==1):
            self.sensor2_cb.setCheckState(Qt.Checked)
        else:
            self.sensor2_cb.setCheckState(Qt.Unchecked)
            
        self.sensor3_cb = QCheckBox('Sensor-3')
        if(ls_dispchan[2]==1):
            self.sensor3_cb.setCheckState(Qt.Checked)
        else:
            self.sensor3_cb.setCheckState(Qt.Unchecked)
            
        self.sensor4_cb = QCheckBox('Sensor-4')
        if(ls_dispchan[3]==1):
            self.sensor4_cb.setCheckState(Qt.Checked)
        else:
            self.sensor4_cb.setCheckState(Qt.Unchecked)
            
        self.sensor5_cb = QCheckBox('Sensor-5')
        if(ls_dispchan[4]==1):
            self.sensor5_cb.setCheckState(Qt.Checked)
        else:
            self.sensor5_cb.setCheckState(Qt.Unchecked)
            
        self.sensor6_cb = QCheckBox('Sensor-6')
        if(ls_dispchan[5]==1):
            self.sensor6_cb.setCheckState(Qt.Checked)
        else:
            self.sensor6_cb.setCheckState(Qt.Unchecked)
            
        self.sensor7_cb = QCheckBox('Sensor-7')
        if(ls_dispchan[6]==1):
            self.sensor7_cb.setCheckState(Qt.Checked)
        else:
            self.sensor7_cb.setCheckState(Qt.Unchecked)
            
        self.sensor8_cb = QCheckBox('Sensor-8')
        if(ls_dispchan[7]==1):
            self.sensor8_cb.setCheckState(Qt.Checked)
        else:
            self.sensor8_cb.setCheckState(Qt.Unchecked)
            
        self.sensor9_cb = QCheckBox('Sensor-9')
        if(ls_dispchan[8]==1):
            self.sensor9_cb.setCheckState(Qt.Checked)
        else:
            self.sensor9_cb.setCheckState(Qt.Unchecked)
            
        self.sensor10_cb = QCheckBox('Sensor-10')
        if(ls_dispchan[9]==1):
            self.sensor10_cb.setCheckState(Qt.Checked)
        else:
            self.sensor10_cb.setCheckState(Qt.Unchecked)
        
        #--------------------- button -------------------------------    
        save_btn = QPushButton('Save Config',self)
        save_btn.setToolTip('Save your configuration!')
        save_btn.resize(save_btn.sizeHint())
        save_btn.clicked.connect(self.saveConfig)
        
        reload_btn = QPushButton('Reload Config',self)
        reload_btn.setToolTip('Reload your configuration!')
        reload_btn.resize(reload_btn.sizeHint())
        reload_btn.clicked.connect(self.reloadConfig)
        
        connect_btn = QPushButton('Connect',self)
        connect_btn.setToolTip('Connect RS232 port!')
        connect_btn.resize(connect_btn.sizeHint())
        connect_btn.clicked.connect(self.connect)
        
        disconnect_btn = QPushButton('Disconnect', self)
        disconnect_btn.setToolTip('Disconnect RS232 port!')
        disconnect_btn.resize(disconnect_btn.sizeHint())
        disconnect_btn.clicked.connect(self.disconnect)
        
        #connection status to display as bitmap
        self.status_pic = QLabel()
        self.status_pic.setGeometry(0, 0, 100, 25)
        self.conStatusRefresh(False)
        
        #---------------------operation mode ratiobox ------------------------------- 
        self.online_rb = QRadioButton("Online")
        self.online_rb.setChecked(Qt.Checked)
        self.offline_rb = QRadioButton("Offline")
        self.log_cb = QCheckBox('Log Enable')
        self.logpath_Edit = QLineEdit("Input log file path:")
        logname_btn = QPushButton("...", self)
        logname_btn.clicked.connect(self.getLogname)
        
        #--------------------- start monitor button----------------------------------
        monitor_btn = QPushButton('Start', self)
        monitor_btn.setToolTip('Start to monitor sensor.')
        monitor_btn.resize(monitor_btn.sizeHint())
        monitor_btn.clicked.connect(self.startMonitor)
        
        
        #--------------------- group box ------------------------------
        RS232_groupbox = QGroupBox("RS232 Config")
        Scan_groupbox = QGroupBox("Scan Setting")
        Sensor_groupbox = QGroupBox("Sensor Select")
        Setdata_groupbox = QGroupBox("Set Configuration")
        Testdata_groupbox = QGroupBox("Test RS232")
        OpMode_groupbox = QGroupBox("Operation Mode")
        OpCmd_groupbox = QGroupBox("Command")
        
        #============================== layout setting ====================================
        RS232_grid = QGridLayout()
        RS232_grid.setSpacing(5)
        RS232_grid.addWidget(self.port_Label, 1, 0)
        RS232_grid.addWidget(self.port_Edit, 1, 1)
        RS232_grid.addWidget(self.baudrate_Label, 2, 0)
        RS232_grid.addWidget(self.baudrate_Edit, 2, 1)
        
        Scan_grid = QGridLayout()
        Scan_grid.setSpacing(5)
        Scan_grid.addWidget(self.scantime_Label, 1, 0)
        Scan_grid.addWidget(self.scantime_Edit, 1, 1)
        Scan_grid.addWidget(self.interval_Label, 2, 0)
        Scan_grid.addWidget(self.interval_Edit, 2, 1)
        
        Sensor_grid = QGridLayout()
        Sensor_grid.setSpacing(5)
        Sensor_grid.addWidget(self.sensor1_cb, 1, 0)
        Sensor_grid.addWidget(self.sensor2_cb, 1, 1)
        Sensor_grid.addWidget(self.sensor3_cb, 1, 2)
        Sensor_grid.addWidget(self.sensor4_cb, 1, 3)
        Sensor_grid.addWidget(self.sensor5_cb, 1, 4)
        Sensor_grid.addWidget(self.sensor6_cb, 2, 0)
        Sensor_grid.addWidget(self.sensor7_cb, 2, 1)
        Sensor_grid.addWidget(self.sensor8_cb, 2, 2)
        Sensor_grid.addWidget(self.sensor9_cb, 2, 3)
        Sensor_grid.addWidget(self.sensor10_cb, 2, 4)
        
        Setgroup_grid = QGridLayout()
        Setgroup_grid.setSpacing(10)
        Setgroup_grid.addWidget(save_btn, 1, 0)
        Setgroup_grid.addWidget(reload_btn, 1, 1)
        
        Testgroup_grid = QGridLayout()
        Testgroup_grid.setSpacing(20)
        Testgroup_grid.addWidget(connect_btn, 1, 0)
        Testgroup_grid.addWidget(disconnect_btn, 1, 1)
        Testgroup_grid.addWidget(self.status_pic, 1, 2)
        
        OpModegroup_grid = QGridLayout()
        OpModegroup_grid.setSpacing(10)
        OpModegroup_grid.addWidget(self.online_rb, 1, 0)
        OpModegroup_grid.addWidget(self.log_cb, 1, 1, 1, 2)
        OpModegroup_grid.addWidget(self.offline_rb, 2, 0)
        OpModegroup_grid.addWidget(self.logpath_Edit, 2, 1)
        OpModegroup_grid.addWidget(logname_btn, 2, 2)
        
        OpCmdgroup_grid = QGridLayout()
        OpCmdgroup_grid.setSpacing(10)
        OpCmdgroup_grid.addWidget(monitor_btn, 1, 0)
        
        #add grid layout to groupbox control
        RS232_groupbox.setLayout(RS232_grid)
        Scan_groupbox.setLayout(Scan_grid)
        Sensor_groupbox.setLayout(Sensor_grid)
        Setdata_groupbox.setLayout(Setgroup_grid)
        Testdata_groupbox.setLayout(Testgroup_grid)
        OpMode_groupbox.setLayout(OpModegroup_grid)
        OpCmd_groupbox.setLayout(OpCmdgroup_grid)
        
        #new settingLayout for config editor field layout
        settingLayout = QHBoxLayout()
        settingLayout.addWidget(RS232_groupbox, 1)
        settingLayout.addWidget(Scan_groupbox, 1)
        settingLayout.addWidget(Sensor_groupbox, 2)
        
        #new setgroup_Layout for config button field layout
        setgroup_Layout = QHBoxLayout()
        setgroup_Layout.addWidget(Setdata_groupbox, 1)
        setgroup_Layout.addWidget(Testdata_groupbox, 2)
        
        #new modegroup_Layout for operation mode field layout
        modegroup_Layout = QHBoxLayout()
        modegroup_Layout.addWidget(OpMode_groupbox, 3)
        modegroup_Layout.addWidget(OpCmd_groupbox, 1)
        
        #new mainLayout to set whole layout on main form
        mainLayout = QVBoxLayout()
        mainLayout.addLayout(settingLayout, 2)
        mainLayout.addLayout(setgroup_Layout, 1)
        mainLayout.addLayout(modegroup_Layout, 1)
        
        #set mainLayout form form layout
        self.setLayout(mainLayout) 
        
        #show form
        self.show()
    
    ''''Centering window on the screen '''  
    def center(self):        
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())
    
    '''override closeEvent to remind user'''    
    def closeEvent(self, event):
        reply = QMessageBox.question(self, 'Message',
            "Are you sure to quit?", QMessageBox.Yes | 
            QMessageBox.No, QMessageBox.No)

        if reply == QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()
    
    '''Save configuration to Config.txt'''
    def saveConfig(self):      
        sender = self.sender()
        #save config data to Config.txt
        self.userconfig.setPort(self.port_Edit.text())
        self.userconfig.setBaudrate(self.baudrate_Edit.text())
        self.userconfig.setScantime(self.scantime_Edit.text())
        self.userconfig.setInterval(self.interval_Edit.text())
        
        #for each data in ls_dispchan to check channel enable flag
        ls_dispchan=[]
        
        #set config data based on sensor select checkbox status
        if(self.sensor1_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor2_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor3_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor4_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor5_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor6_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor7_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor8_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor9_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
        if(self.sensor10_cb.checkState()==Qt.Checked):
            ls_dispchan.append(1)
        else:
            ls_dispchan.append(0)
            
        self.userconfig.setDispchan(ls_dispchan)
        
        self.reloadConfig()
    
    '''Reload configuration data on UI''' 
    def reloadConfig(self):      
        sender = self.sender()
        #reload config data from Config.txt
        self.port_Edit.setText(self.userconfig.getPort())
        self.baudrate_Edit.setText(self.userconfig.getBaudrate())
        self.scantime_Edit.setText(self.userconfig.getScantime())
        self.interval_Edit.setText(self.userconfig.getInterval())
        
        #for each data in ls_dispchan to check channel enable flag
        ls_dispchan=[]
        sensor_list=str(self.userconfig.getDispchan()).replace(" ","")
        for i in sensor_list[1:-1].split(','):
            ls_dispchan.append(int(i))
        
        #set sensor select checkbox based on config data  
        if(ls_dispchan[0]==1):
            self.sensor1_cb.setCheckState(Qt.Checked)
        else:
            self.sensor1_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[1]==1):
            self.sensor2_cb.setCheckState(Qt.Checked)
        else:
            self.sensor2_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[2]==1):
            self.sensor3_cb.setCheckState(Qt.Checked)
        else:
            self.sensor3_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[3]==1):
            self.sensor4_cb.setCheckState(Qt.Checked)
        else:
            self.sensor4_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[4]==1):
            self.sensor5_cb.setCheckState(Qt.Checked)
        else:
            self.sensor5_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[5]==1):
            self.sensor6_cb.setCheckState(Qt.Checked)
        else:
            self.sensor6_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[6]==1):
            self.sensor7_cb.setCheckState(Qt.Checked)
        else:
            self.sensor7_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[7]==1):
            self.sensor8_cb.setCheckState(Qt.Checked)
        else:
            self.sensor8_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[8]==1):
            self.sensor9_cb.setCheckState(Qt.Checked)
        else:
            self.sensor9_cb.setCheckState(Qt.Unchecked)
            
        if(ls_dispchan[9]==1):
            self.sensor10_cb.setCheckState(Qt.Checked)
        else:
            self.sensor10_cb.setCheckState(Qt.Unchecked)
    
    '''Open RS232 connection'''        
    def connect(self):
        #initialize RS232 object-myserial
        myserial=MyData.RS232_Init()
        if(MyData.portIsUsable(myserial.port)):
            #Open RS232 port
            MyData.RS232_Open(myserial)    
            if(myserial.isOpen()):
                portIsconnect=True
            else:
                portIsconnect=False        
        else:
            portIsconnect=False            
        self.conStatusRefresh(portIsconnect)
    
    '''Close RS232 connection'''    
    def disconnect(self):
        #initialize RS232 object-myserial
        myserial=MyData.RS232_Init()
        if(MyData.portIsUsable(myserial.port)):
            #Open RS232 port
            MyData.RS232_Close(myserial)    
            if(myserial.isOpen()):
                portIsconnect=True
            else:
                portIsconnect=False        
        else:
            portIsconnect=False
        self.conStatusRefresh(portIsconnect)
    
    '''Refresh connection status picture'''
    def conStatusRefresh(self,port_status):
        if(port_status):
            myPixmap = QPixmap('res/conn_font.jpg')
        else:
            myPixmap = QPixmap('res/disconn_font.jpg')
        myScaledPixmap = myPixmap.scaled(self.status_pic.size(), Qt.KeepAspectRatio)
        self.status_pic.setPixmap(myScaledPixmap)
    
    '''Call open file dialog to choose log file'''
    def getLogname(self):
        fname = QFileDialog.getOpenFileName(self, 'Choose log file', os.getcwd(), 'Log files (*.log)')
        #print(fname)
        self.logpath_Edit.setText(fname[0])
     
    '''Start monitor process based on start button clicked action'''   
    def startMonitor(self):
        #check if sensor select is 0: no sensor display
        isNoneSensor=True
        ls_plot=[]
        sensor_id=0
        sensor_list=str(self.userconfig.getDispchan()).replace(" ","")
        for i in sensor_list[1:-1].split(','):
            sensor_id+=1
            if(i=='1'):
                isNoneSensor=False
                ls_plot.append(sensor_id)
        if(isNoneSensor):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Critical)
            msg.setWindowTitle("MessageBox")
            msg.setText("Please choose at least 1 sensor to start monitor!")
            msg.setStandardButtons(QMessageBox.Ok)
            retval = msg.exec_()
            return retval
        
        #call online monitor process
        if(self.online_rb.isChecked()):
            #MySensor.log_en=1, enable logging
            if(self.log_cb.isChecked()): 
                #single monitor               
                if(len(ls_plot)==1):
                    print(subprocess.check_output(['python','./monitor_sensor.py', '--online', str(ls_plot[0]),'--log']))
                #multiple monitor 
                else:
                    print(subprocess.check_output(['python','./monitor_sensor.py', '--online', '--','--log']))
            #MySensor.log_en=0, disable logging
            else: 
                #single monitor 
                if(len(ls_plot)==1):
                    print(subprocess.check_output(['python','./monitor_sensor.py', '--online', str(ls_plot[0])]))
                #multiple monitor
                else:                      
                    print(subprocess.check_output(['python','./monitor_sensor.py', '--online', '--']))
        else:
            #read log path from logpath_Edit
            logname=self.logpath_Edit.text()
            #verify log path
            if(not os.path.exists(logname)):
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Critical)
                msg.setWindowTitle("MessageBox")
                msg.setText("Log file is not existed!")
                msg.setStandardButtons(QMessageBox.Ok)
                retval = msg.exec_()
                return retval
            #call offline monitor process        
            MySensor.monitor_offlineUI(logname)  

if __name__ == "__main__":
    app = QApplication(sys.argv)
    ex = SPIR_FORM()
    sys.exit(app.exec_()) 
