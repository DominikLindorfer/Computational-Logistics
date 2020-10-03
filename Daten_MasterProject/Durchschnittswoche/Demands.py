#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec  2 13:41:46 2019

@author: lindorfer
"""
import math

file = open("/home/lindorfer/ReWiTech/Masterarbeit/Programs/Daten_MasterProject/Durchschnittswoche/Demand1.csv","r")

data = file.read()
data = data.split("\n")

data = sorted(data)
data_ = [d.split(";") for d in data ]
data_.pop(0)


data_april = [d for d in data_ if d[0] == "\"April\""]

print(len(data_april))

stores = dict()

stri = "STO_"

for i in range(2,737,1):
    index = stri+"000"[len(str(i)):]+str(i)
    stores[index]= 0

#print(stores)

demands = list()
for i in range(6):
    demands.append(stores.copy())


for d in data_april:
    demands[int(d[1])%6][d[2][1:8]] += int(d[4])
    if(d[2]=="\"STO_143\""):
        print(int(d[1])%6+1,d[3],d[4])


fr = open("/home/lindorfer/ReWiTech/Masterarbeit/Programs/Daten_MasterProject/Durchschnittswoche/demands_agg.csv","w")
fr.write("\"Month\";\"Day\";\"SiteName\";\"Temperature\";\"DemandPallets\"")
fr.write("\n")

for i,d in enumerate(demands) :
    print(len(d))
    for k,s in d.items() :
        fr.write("\"April\";"+str(i+1)+";"+"\""+k+"\";\"Ambient\";"+str(math.ceil(float(s)/4))+"\n")
        #fr.write(i+;")

fr.close()