#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec  2 13:41:46 2019

@author: lindorfer
"""
import math

file = open("/home/lindorfer/ReWiTech/Masterarbeit/Programs/Daten_MasterProject/Durchschnittswoche/StoreServiceTimes.csv","r")

data = file.read()
data = data.split("\n")

data = sorted(data)
data_ = [d.split(";") for d in data ]
data_.pop(0)
data_.pop(0)

print(data_[3])

def unique(list1): 
  
    # intilize a null list 
    unique_list = [] 
      
    # traverse for all elements 
    for x in list1: 
        # check if exists in unique_list or not 
        if x not in unique_list: 
            unique_list.append(x)

    return unique_list

uniquedata_ = unique(data_)

# data_april = [d for d in data_ if d[0] == "\"April\""]
# print(len(data_april))

stores = list()
stri = "STO_"

for i in range(2,737,1):
    index = stri + "000"[len(str(i)):] + str(i)
    stores.append(["\"" + index + "\"", "\"" + "Mo-Sa" + "\"", "\"" + "01:00" + "\"", "\"" + "00:00" + "\""])



for s in range(len(stores)):
    for ud in uniquedata_:

        if(stores[s][0] == ud[0]):
            stores[s] = ud


# #print(stores)

# demands = list()
# for i in range(6):
#     demands.append(stores.copy())


# for d in data_april:
#     demands[int(d[1])%6][d[2][1:8]] += int(d[4])
#     if(d[2]=="\"STO_143\""):
#         print(int(d[1])%6+1,d[3],d[4])


fr = open("/home/lindorfer/ReWiTech/Masterarbeit/Programs/Daten_MasterProject/Durchschnittswoche/ServiceTimes_all.csv","w")
fr.write("\"Pseudonym\";\"Delivery days\";\"ServiceTimeFrom\";\"ServiceTimeTo\"")
fr.write("\n")

for i,d in enumerate(stores) :
     fr.write(d[0] + ";" + d[1] +";" + d[2] + ";" + d[3] + "\n")

fr.close()