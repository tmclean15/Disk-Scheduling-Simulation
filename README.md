# Introduction

This project contains two programs that provide a command-line interface to simulate
two disk-scheduling algorithms; first come first served, and shortest seek time first. 
It is intended for learning purposes, and for simulating how the number of file requests
affects the performance of these algorithms. The source code is provided for illustrative
purposes.

## Installation

Clone this project to your local machine with the following command:

git clone https://github.com/tmclean15/Disk-Scheduling-Simulation.git

The executables provided in the project were compiled using the Linux GCC compiler,
therefore may not be compatible with all OS architectures. If incompatible, compile the 
source code files with a compiler available for your OS. Suggested for Windows is MinGW,
as it provides a port for the GCC compiler.

## Operation

For each program, the CLI will prompt the user for a number of file requests, and five 
seeds for a random number generator. The program will display the average disk head movement
for five simulations, each run with one of the seeds provided.

