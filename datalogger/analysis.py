import matplotlib.pyplot as plt
import pandas as pd


#Leser av daten vi hentet for akselerometeret og analyserer og plotter den

xAks = []
yAks = []
zAks = []
xRot = []
yRot = []
zRot = []
t =[-100*10**-3]

vy =[0]
sy = [0]


dt =[]
with open("serial_log.txt", "r") as file:
    # Read and process the file line by line
    for line in file:
        # Strip newline characters and print the line
        lineSplit = line.split(";")
        lineSplit[1] = lineSplit[1].strip()
        dataList = lineSplit[1].split(",")
        # print(dataList)
        xAks.append(float(dataList[0]))
        # if abs(xAks[-1])<0.5:
        #     xAks[-1]=0
        yAks.append(float(dataList[1]))
        zAks.append(float(dataList[2]))
        xRot.append(float(dataList[3]))
        yRot.append(float(dataList[4]))
        zRot.append(float(dataList[5]))
        dt.append(float(dataList[7])*(10**-3))
        t.append(t[-1]+dt[-1])

smoothAksX = []
dtSmoot = []
tSmoot = []
#fjerner store variasjoner
for i in range(0,len(xAks)-10,10):
    sumAksX = 0
    sumDt = 0
    for j in range(10):
        sumAksX+=xAks[i+j]
        sumDt+=dt[i+j]
    sumAksX /=10
    dtSmoot.append(sumDt)
    tSmoot.append(t[i])
    smoothAksX.append(sumAksX)    

#integhrerer
for i in range(len(tSmoot)-1):
    vy.append(vy[-1]+smoothAksX[i]*dtSmoot[i])
    sy.append(sy[-1]+vy[i]*dtSmoot[i])

t.pop(0)
# vy.pop(0)
# sy.pop(0)
# print(xAks)


print(xAks)
#plotter
plt.title("t/sx")
plt.plot(tSmoot,sy)
plt.axhline(y=13,color = "r")
plt.axvline(x=16,color = "r")
plt.show()
plt.title("t/vx")
plt.axvline(x=16,color = "r")
plt.plot(tSmoot,vy)
plt.show()
plt.title("t/xaks")
plt.plot(tSmoot,smoothAksX)
plt.axvline(x=16,color = "r")
# plt.plot(t,yAks)
plt.show()
        
