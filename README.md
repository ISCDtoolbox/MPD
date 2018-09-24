# MPD
Maximizing optimal probability domain

Version 2.0

September 2018

The MPD program aims to compute the shape of the domain that maximizes the probability to find exactly a certain number of electrons inside it, given a chemical electronic structure.

In order to compile the program, you need to have previously installed:
* gcc (version 5.4.0)
* make (version 4.1)
* cmake (version 3.5.1)
* git (version 2.7.4)

and the libraries:
* liblapacke (3.6.0-2ubuntu2) package containing the Standard C language library for LAPACK (version 3.X).
* lib32gomp1 (5.4.0-6ubuntu1~16.04.6) which an implementation of OpenMP for the C language

In addition, a certain number of other iscd softwares (https://github.com/ISCDtoolbox) must have been previously installed
* medit to vizualize 3D mesh (see https://github.com/ISCDtoolbox/Medit)
* mshdist to redistanciate level-set function (see https://github.com/ISCDtoolbox/Mshdist)
* elastic to solve elasticity on 3D mesh (see https://github.com/ISCDtoolbox/LinearElasticity)
* advect to advect a level-function function thanks to a vector field (see https://github.com/ISCDtoolbox/Advection)
* mmg3d_03 to adapt 3D mesh (see https://github.com/Mmgtools)

Important remark: the version of mmg3d we need for the mpdProgram is a modified version of the official one. Hence, when you have downloaded the git repository from mmgTools, overwrite the src files with the ones given in mmg3d5modified/src then cmake the new files in order to get to good version of mmg3d 

Then, open your terminal and locate yourself at the right position to download the files thanks to "ls" and "cd" command
To clone the files, type

	git clone https://github.com/ISCDtoolbox/MPD.git

To enter the directory, type

	cd MPD

To create a build directory, type

	mkdir build

To enter the build directory, type

	cd build

To create the make file, type

	cmake ..

To compile the mpd program, type

	make

then if it is successfull, type

	make install

(Another more direct possibility is to go inside sources directory and type

	gcc loadParameters.c loadChemistry.c loadMesh.c adaptMesh.c optimization.c main.c -lm -llapacke -fopenmp -Wall -Wextra -pedantic-errors -o mpdProgram

to compile program without cmake and makefile)

Then, you can go inside bin directory by typing

	cd ../bin

and checks that mpdProgram has been created. Now, to first use the mpdProgram, you should place yourself in the outputFiles directory.

The mpdProgram program only takes one input argument: a *.info file (the file must have the *.info extension this is important)

In this file (an example is provided in outputFile directory), the minimal
configuration is to prescribe

	a *.wfn/ *.chem file (see example in the inputFile directory)

	the number of electrons to look for
 
in the following way in the *.info file

	name_chem *.wfn

	nu_electrons 2

	end_data

You can also prescribed a *.mesh/ *.cube file thanks to the name_mesh keyword

The mpd program has also different modes of optimization. It is ruled by the
opt_mode keyword. Positive value assumes that your mesh is made of tetrahedra
whereas non-positive value concerns hexahedral meshes (little cubes). Hence, write in the *.info file

	opt_mode 0

for optimizing the hexahedral mesh or write

	opt_mode 1 

for optimizing the tetrahedral one.

Finally, in order to launch mpdProgram the line command syntax is the following (we assume you are in the outputFiles directory)

	./../bin/mpdProgram ./00essai.info

and checks if it works





 





