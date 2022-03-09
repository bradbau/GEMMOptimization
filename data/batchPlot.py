import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys
import re

kernelNum=5
caseNum=10

x=[]

# plot all

#collect data
numberPattern=r"\d+\.\d*"

perfDataSet=[]
nameList=[]
x=[100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000]

for kernelItem in range(0, kernelNum):
    filename="perf_"+str(kernelItem)+".data"
    kernelName="kernel_"+str(kernelItem)
    nameList.insert(-1, kernelName)


    perfItem=[]
    with open(filename) as f:
        print("open file ", filename)
        lines = f.readlines()
        for line in lines:
            perfItem.insert(-1, float(line))
    
    perfDataSet.insert(-1, perfItem)

# add to dataframe
perfDataDf=pd.DataFrame(perfDataSet, nameList)
print(perfDataDf)
ax = plt.gca()
perfDataDf.plot()
ax.figure.savefig("overview.png")






