#!/bin/bash
# run the test save to res_i.data

rm *.data
#echo "input number" `expr $#-1`
start=$1
end=$2

: '
if [ $# == 2 ]
then
echo "1 args"
start=1
end=$1
elif [ $# ==3 ]
then
echo "2 args"
start=$1
end=$2
else
echo "no args"
start=0
end=15
fi
'

for ((i=$start;i<=$end;i+=1))
do  
    cd ..
    cd src
    
    output_file_name="../data/res_${i}.data"
    echo "executing main $i"
    ./main $i  &> ${output_file_name}
    echo "main $i completed"
    cd ../data
    
done