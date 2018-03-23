#!/bin/bash

# Compile sources
g++ speedup.cpp -o speedup -lpthread -std=c++11
if [ $? -ne 0 ]; then
    echo "Failed to compile"
    exit $?;
fi

# Set file name
results_dir='results/'
results_dir+=$(date +%Y%m%d%H%M%S);
output_file=$results_dir'/benchmark_results.csv'
# mkdir $results_dir;

batch_size=30;
# Execute test 30 times
for i in `seq 1 $batch_size`; do
    echo $((100*$i/$batch_size))%;
    # ./speedup >> $output_file;
done