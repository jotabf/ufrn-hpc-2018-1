#!/bin/bash
#SBATCH --job-name=MPI_PI 
#SBATCH --output=Results/S_%j.out
#SBATCH --error=Results/E_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0-3:30

##export OMP_NUM_THREADS=

P=($SLURM_NTASKS_PER_NODE*$SLURM_JOB_NUM_NODES)
Name=Exec/mpi_pi_$SLURM_JOB_ID

echo "Processos = $P"
echo " "

mpiicc mpi_hello.c -o $Name
wait

#1 2 4 8 16 32 64 128 256 512
for (( t = 1; t < 600; t*=2 )); do
	for (( i = 0; i < 10; i++ )); do
		srun $Name $t
		wait
	done
	echo "$t"
	wait
done