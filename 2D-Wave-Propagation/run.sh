#!/bin/bash
#SBATCH --job-name=PROP2D 
#SBATCH --output=Results/S_%j.out
#SBATCH --error=Results/E_%j.err
#SBATCH --exclusive
#SBATCH --hint=compute_bound
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=32
#SBATCH --time=0-00:30
#SBATCH --qos=qos2

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
Name=Exec/mpi_mxm_$SLURM_JOB_ID

module purge; wait
module load compilers/gnu/7.3; wait

#Parametros
Mx=512
My=512
N=512
dx=0.1
dy=0.1
dt=0.04
###########

Median()
{
	module unload compilers/gnu/7.3; wait
	module load softwares/python/3.6.3-gnu-4.8_sh_lib; wait
	python3 median.py "$pyOut"; wait
	rm -r $pyOut; wait
	module unload softwares/python/3.6.3-gnu-4.8_sh_lib; wait
	module load compilers/gnu/7.3; wait
}

if [[ $SLURM_CPUS_PER_TASK > 1 ]]; then
	opemmp=-fopenmp
fi

echo "#[Propagação 2D Paralelo] [T = $SLURM_CPUS_PER_TASK]"

echo "#Compilação"
gcc -lm $opemmp propag_2d_parallel.c -o $Name
wait

echo "#Execução"

echo "#Variando [Mx]"
pyOut=Results/temp/$SLURM_JOB_ID
for (( A = 512; A <= 8192; A*=2 )); do
	for (( i = 0; i < 10; i++ )); do
		srun $Name $A $My $N $dt >> $pyOut
		wait
	done
	printf "\n" >> $pyOut 
	wait
done
Median

echo "#Variando [My]"
pyOut=Results/temp/$SLURM_JOB_ID
for (( A = 512; A <= 8192; A*=2 )); do
	for (( i = 0; i < 10; i++ )); do
		srun $Name $Mx $A $N $dt >> $pyOut
		wait
	done
	printf "\n" >> $pyOut 
	wait
done
Median

echo "#Variando [N]"
pyOut=Results/temp/$SLURM_JOB_ID
for (( A = 512; A <= 8192; A*=2 )); do
	for (( i = 0; i < 10; i++ )); do
		srun $Name $Mx $My $A $dt >> $pyOut
		wait
	done
	printf "\n" >> $pyOut 
	wait
done
Median


rm -r $Name; wait