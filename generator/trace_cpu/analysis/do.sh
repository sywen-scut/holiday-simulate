#./corr -n barnes_xy -i 100000 -f 0.1
interval=10000
freq=0.5
for sets in barnes_xy blackscholes_xy canneal_xy dedup_xy ferret_xy fluidanimate_xy freqmine_xy raytrace_xy streamcluster_xy swaptions_xy swaptions_xy
do
        ./corr_cpu -n $sets -i $interval -f $freq
done

