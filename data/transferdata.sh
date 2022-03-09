#!/bin/bash
#extract performance data from res_i.data
start=$1
end=$2

for ((i=$start;i<=$end;i+=1))
do
        input_file_name="res_${i}.data"
        output_file_name="perf_${i}.data"
        grep "elasped" ${input_file_name} | cut -c 52-60  &> ${output_file_name}
done