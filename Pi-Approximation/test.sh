#!/bin/bash
#SBATCH --job-name=MPI_PI 
#SBATCH --output=Results/S_%j.out
#SBATCH --error=Results/E_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0-3:30

P=($SLURM_NTASKS_PER_NODE*$SLURM_JOB_NUM_NODES)
Name=Exec/test_$SLURM_JOB_ID

mpiicc mpi_histograma.c -o $Name
wait

./$Name

rm -rv $Name