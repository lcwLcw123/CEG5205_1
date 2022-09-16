from signal import signal
import serial
import io
import pyqtgraph as pg 
import array 
import numpy as np
# import related python packages
import time
import zmq


Data = serial.Serial()
Data.baudrate = 38400
Data.port = 'COM4'
Data.timeout =0.01
Data.open()
# sio  = io.TextIOWrapper(io.BufferedRWPair(Data,Data,1),encoding='ascii',nweline='\r')
# define serial port  

app=pg.mkQApp()
win = pg.GraphicsWindow()
win.setWindowTitle('demo')
win.resize(1600,900)
xLength = 300
fig1 = win.addPlot()
fig1.showGrid(x=True, y=True)
fig1.setRange(xRange=[0,xLength], padding=0)
fig1.setLabel(axis = 'left', text='g')
fig1.setLabel(axis = 'bottom',text='x / point')
fig1.setTitle('acceleration')

curve1 = fig1.plot()
curve2 = fig1.plot()
curve3 = fig1.plot()

data = [np.zeros(xLength).__array__('d'),
        np.zeros(xLength).__array__('d'),
        np.zeros(xLength).__array__('d')] 
# 定义缓冲区长度
buffer_length = 10
edge =2.5
buffer = np.zeros((buffer_length,3)) 
# initial figure  and data which need to save

# def placement(singal,prev_placement):
#     after_placement = prev_placement  

def motivation_test(buffer):
    motivation = ''         
    # 2:b 1:r 0:g
    x_max,y_max,z_max= max(buffer[:,1]),max(buffer[:,0]),max(buffer[:,2])
    x_min,y_min,z_min = min(buffer[:,1]),min(buffer[:,0]),min(buffer[:,2])
    if sum(buffer[:,2])>sum(buffer[:,1])+15:
        motivation = 'zhua'                                     
        if x_max>0.5>y_max and x_max>0.5>z_max-1:
            motivation = 'zhua and left'
        if x_min<-0.5<z_min-1 and x_min<-0.5<y_min :
            motivation = 'zhua and right'
        if z_max-1>0.5>y_max and z_max-1>0.5>x_max:
            motivation = 'zhua and down'
        if z_min-1<-0.5<y_min and z_min-1<-0.5<x_min:
            motivation = 'zhua and up'
        if y_max>0.5>x_max and y_max>0.5>z_max-1:
            motivation = 'zhua and qianjin'       
        if y_min<-0.5<x_min and y_min<-0.5<z_min-1:
            motivation = 'zhua and houtui'
    if sum(buffer[:,2])<sum(buffer[:,1])+15:
        motivation = 'meizhua'
    return motivation

def motivation_test_new(buffer):
    motivation = ''         
    # 2:b 1:r 0:g
    x_max,y_max,z_max= max(buffer[:,1]),max(buffer[:,0]),max(buffer[:,2])
    x_min,y_min,z_min = min(buffer[:,1]),min(buffer[:,0]),min(buffer[:,2])

    if buffer_length-edge<sum(buffer[:,2])<buffer_length+edge:
        motivation = 'stop'
    elif -edge<sum(buffer[:,2])<edge:
        if buffer_length-edge<sum(buffer[:,1])<buffer_length+edge:
            motivation = 'left'
        elif -buffer_length-edge<sum(buffer[:,1])<-buffer_length+edge and -edge<sum(buffer[:,0])<edge:
            motivation = 'right'
        elif -edge<sum(buffer[:,1])<edge and -edge-buffer_length<sum(buffer[:,0])<-buffer_length+edge:
            motivation = 'back'
        elif -edge<sum(buffer[:,1])<edge and buffer_length-edge<sum(buffer[:,0])<buffer_length+edge:
            motivation = 'forward'
    elif (buffer_length)/2-edge<sum(buffer[:,2])<(buffer_length)/2+edge:
        if (buffer_length)/2-edge<sum(buffer[:,1])<(buffer_length)/2+edge:
            motivation = 'down'
        elif -buffer_length-edge<sum(buffer[:,1])<-buffer_length+edge:
            motivation = 'up'
    elif -buffer_length-edge<sum(buffer[:,2])<-buffer_length+edge:
        motivation = 'zhua'
    return motivation

def canFloat(data):
    try:    
        float(data)
        return True
    except:
        return False
# function to judge whether the data is float

def panbie(singal):
    if singal != None and (len(singal)==3) and singal[0] != '' and singal[2] != '':
        if  singal[0] <4 and  singal[1] <4 and  singal[2] <4 :
            return True
    return False
# Identify non-compliance with data requirements, use to exclude noise points and data loss

def dataProcess(data):
    data = str(data)
    dataSet = []
    datapoint = ''
    dot = 0 
    for i in data:
        if i.isdigit() or i == '.' or i == '-':
            datapoint += i
        elif i == "," and canFloat(datapoint):
            dot =dot+1
            dataSet.append(float(datapoint))
            datapoint = ''
    if dot != 3 :return None
    else: return dataSet
# process data 

ax = array.array('d')
ay = array.array('d')
az = array.array('d')


def plotData():
    global signal

    signal = Data.readline()
    signal = dataProcess(signal)
    if panbie(signal):
        for i in range(len(data)):
            if len(data[i]) < xLength :
                data[i].append(signal[i])
            else:
                data[i][:-1] = data[i][1:]
                data[i][-1] = signal[i]
        ax.append(signal[0])
        ay.append(signal[1])
        az.append(signal[2])
        print(signal)
        curve1.setData(data[0],pen=pg.mkPen('g',width=3))
        curve2.setData(data[1],pen=pg.mkPen('r',width=3))
        curve3.setData(data[2],pen=pg.mkPen('b',width=3))
        # buffer[0] = signal
        # buffer[1:,:]= buffer[:-1,:]
        # motivation = motivation_test_new(buffer)
        # print(motivation)
        # message = socket.recv()
        # socket.send_string(motivation)


        # socket.send_string(motivation)
# function to plot data 

# while True:
#     signal = Data.readline()
#     signal = dataProcess(signal)
#     if signal == None or signal[0] == None or signal[2] ==None : continue
#     else: 
#         print(signal)
#     # print(Data)
#     # signal = Data.readline()
#     # print(signal)


timer = pg.QtCore.QTimer()
timer.timeout.connect(plotData)
timer.start(1)
app.exec()

try:
    input('press enter to stop\n')
except (Exception, KeyboardInterrupt):
    pass
#  try to plot figure

# np.savetxt('x_acc.txt',ax)
# np.savetxt('y_acc',ay)
# np.savetxt('z_acc',az)
# to save the data

# app.close()


