#!/bin/bash
#SBATCH --job-name=MPI_MXM 
#SBATCH --output=Results/S_%j.out
#SBATCH --error=Results/E_%j.err
#SBATCH --exclusive
#SBATCH --hint=compute_bound
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=8
#SBATCH --time=0-00:30
#SBATCH --qos=qos2

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

P=($SLURM_NTASKS_PER_NODE*$SLURM_JOB_NUM_NODES)
Name=Exec/mpi_mxm_$SLURM_JOB_ID

echo "#[P = $P] [T = $SLURM_CPUS_PER_TASK]"

module load compilers/gnu/7.3; wait
module load libraries/openmpi/2.1.1-gnu-6.4; wait

mpicc  mpi_mxm.c -o $Name -lm -fopenmp
wait

#for size in {300,600,1200,2400,4800}; do
for size in {1,4800}; do
	echo "#Size = $size"
	for (( i = 0; i < 10; i++ )); do
		mpirun $Name $size
		wait
		echo " "
	done
	wait
done