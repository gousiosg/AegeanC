1.Preface
This is the implementation of the Aegean C  compiler prepared by 
Aegean_force(4); during the Hardware & Software I course.
The contents of the Aegean C programming language can be found at  
the web page of Prof Diomides Spinellis (http://kerkis.math.aegean.gr/~dspin/comp)

2.Contents
This archive consists of  the sources of the Aegean C language, a sample Aegean C programm 
a simple makefile to help the compiling and installing process, a simple sh script to help
the Aegean C programms compiling, and a .doc file that describes the compiler 
bulding process

3.Compile-Install
*To compile the compiler simply type 
	make
at the command prompt
*To install the compiler and its sources to your home directory, type
	make install
*To clean old core files and recompile type 
	make clean 

4.Running the compiler
To compile an Aegean C programm you can either use the acc program or the
acc.sh  script. We recomend to use the script because it does everything
(compiling and re-compiling the assembly sources with gcc) by its own
The usage of the script is
	./acc.sh input_name.ac output_name
It compiles the Aegean C source to assembly and recompiles the assembly with gcc
to produce the output_name executable.

The usage of acc is the following
	./acc <input_name.ac >output_name.s
	gcc -o executable_name output_name


