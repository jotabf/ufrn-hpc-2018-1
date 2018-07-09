#!/bin/bash
#SBATCH --job-name=MPI_HIST 
#SBATCH --output=Results/S_%j.out
#SBATCH --error=Results/E_%j.err
#SBATCH --exclusive
#SBATCH --hint=compute_bound
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=16
#SBATCH --time=0-00:30
#SBATCH --qos=qos2

module purge; wait
Name=Exec/hist_$SLURM_JOB_ID.out
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK


echo "[P = $SLURM_JOB_NUM_NODES*$SLURM_NTASKS_PER_NODE] [T = $SLURM_CPUS_PER_TASK]"

echo "##EXECUTION##"

module load libraries/openmpi/2.1.1-gnu-6.4; wait
for VERSION in {6.4,7.1,7.3}; do

    echo "##COMPILER## GNU ${VERSION}"

    module load compilers/gnu/$VERSION; wait

    py_out=Results/temp/$SLURM_JOB_ID
    for (( i = 0; i <= 3; i++ )); do
        mpicc -O$i -fopenmp mpi_histograma.c histogram.c -o $Name -std=c99
        wait
        
        for (( j = 0; j < 10; j++ )); do
            mpirun $Name 16 >> $py_out
            wait
        done
        printf "\n" >> $py_out 
        wait
    done

    module unload compilers/gnu/$VERSION; wait

    module load softwares/python/3.6.3-gnu-4.8_sh_lib; wait
    python3 median.py "$py_out"; wait
    rm -r $py_out; wait
    module unload softwares/python/3.6.3-gnu-4.8_sh_lib; wait
done
module unload libraries/openmpi/2.1.1-gnu-6.4; wait


    echo "##COMPILER## INTEL 16"
    module load compilers/intel/16.0.1; wait    

    py_out=Results/temp/$SLURM_JOB_ID.temp
    for (( i = 0; i <= 3; i++ )); do
        mpiicc -O$i -qopenmp mpi_histograma.c histogram.c -o $Name -std=c99
        wait
        
        for (( j = 0; j < 10; j++ )); do
            mpirun $Name 16 >> $py_out
            wait
        done
        printf "\n" >> $py_out 
        wait
    done

    module unload compilers/intel/16.0.1; wait

    module load softwares/python/3.6.3-gnu-4.8_sh_lib; wait
    python3 median.py "$py_out"; wait
    rm -r $py_out; wait
    module unload softwares/python/3.6.3-gnu-4.8_sh_lib; wait

    echo "##COMPILER## INTEL 18"
    module load compilers/intel/2018.2.199; wait
    eval $loadIntelLibs; wait
    #module load libraries/openmpi/3.1.0-intel2018-7.3; wait

    py_out=Results/temp/$SLURM_JOB_ID.temp
    for (( i = 0; i <= 3; i++ )); do
        mpiicc -O$i -qopenmp mpi_histograma.c histogram.c -o $Name -std=c99
        wait
        
        for (( j = 0; j < 10; j++ )); do
            mpirun $Name 16 >> $py_out
            wait
        done
        printf "\n" >> $py_out 
        wait
    done

    module load softwares/python/3.6.3-gnu-4.8_sh_lib; wait
    python3 median.py "$py_out"; wait
    rm -r $py_out; wait
    module unload softwares/python/3.6.3-gnu-4.8_sh_lib; wait

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
#   printf "\n" >> $py_out
#done

#Comando que me salvou quando icc estava de treta comigo
#source /opt/intel/compilers_and_libraries_2016.1.150/linux/bin/compilervars.sh -arch intel64 -platform linux