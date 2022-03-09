import sys
import pandas as pd
import matplotlib.pyplot as plt

kernelNum=10

n = len(sys.argv)
if n!= 3:
    print("need 2 input")

sample1Idx=int(sys.argv[1])
sample2Idx=int(sys.argv[2])

if sample1Idx > kernelNum or sample2Idx > kernelNum:
    print("sample index out of range")

sample1Filename="perf_"+str(sample1Idx)+".data"
sample2Filename="perf_"+str(sample2Idx)+".data"

sample1Data=pd.read_csv(sample1Filename, sep="\n", header=None)
sample2Data=pd.read_csv(sample2Filename, sep="\n", header=None)



x=np.array([100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000])

length1=sample1Data.shape[0]
length2=sample1Data.shape[0]
xlength=min(length1, length2)-1

plt.plot(x, sample1Data[1:xlength+1] , label="kernel"+str(sample1Idx))
plt.plot(x, sample2Data[1:xlength+1] , label="kernel"+str(sample2Idx))

#plt.legend(loc='upper center')



outputFilename="fig/Compare_"+str(sample1Idx)+"_"+str(sample2Idx)+".png"
plt.savefig(outputFilename)