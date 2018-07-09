#!/bin/bash
#SBATCH --job-name=MPI_HIST 
#SBATCH --output=Results/S_%j.out
#SBATCH --error=Results/E_%j.err
#SBATCH --exclusive
#SBATCH --hint=compute_bound
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0-00:30
#SBATCH --qos=qos2

COMPILER=mpicc
Name=Exec/hist_$SLURM_JOB_ID.out
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
module load compilers/gnu/7.1; wait
module load libraries/openmpi/2.1.1-gnu-6.4; wait

echo "[P = $SLURM_JOB_NUM_NODES*$SLURM_NTASKS_PER_NODE] [T = $SLURM_CPUS_PER_TASK]"
echo "##COMPILER##"
#$COMPILER -fopenmp mpi_histograma.c histogram.c image.c -o $Name -std=c99; wait
$COMPILER mpi_histograma.c histogram.c image.c -o $Name -std=c99; wait

echo "##EXECUTION##"

py_out=Results/temp/$SLURM_JOB_ID
for (( i = 1; i <= 16; i*=2 )); do
    echo $i
    for (( j = 0; j < 10; j++ )); do
        mpirun $Name $i >> $py_out;
        wait
    done
    printf "\n" >> $py_out 
    wait
done

module load softwares/python/3.6.3-gnu-4.8_sh_lib; wait
python3 median.py "$py_out"; wait
rm -r $py_out; wait

rm -r $Name; wait


#Vtune
#srun amplxe-cl -c advanced-hotspots -r ./Results/VTune/$SLURM_JOB_ID -- ./$Name 16

#py_out=Results/temp/$SLURM_JOB_ID
#for (( i = 1; i <= 16; i*=2 )); do
#    #printf "%i " "$i"
#    for (( j = 0; j < 5; j++ )); do
#        srun $Name $i >> $py_out
#        wait
#    done
#	printf "\n" >> $py_out
#done