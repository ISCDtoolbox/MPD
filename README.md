# mpdProgram
Maximizing optimal probability domain

Version 2.0

September 2018

The MPD program aims to compute the shape of the domain that maximizes the probability to find exactly a certain number of electrons inside it, given a chemical electronic structure.

In order to compile the program, it is recommended to have previously installed:
* gcc (version 5.4.0)
* g++ (version 5.4.0)
* make (version 4.1)
* cmake (version 3.5.1)
* git (version 2.7.4)
* doxygen (version 1.8.11) (optional it is used for generating the documentation)

Then, open your terminal and locate yourself at the right position to download the files thanks to "ls" and "cd" command
To clone the files, type

	git clone https://github.com/ISCDtoolbox/MPD.git

To enter the directory, type

	cd MPD

To allow the execution of the install.sh file, you have to change permissions for the install.sh file.

	chmod 740 install.sh

Then, just type

	./install.sh

and follow the instructions. The mpdProgram will be installed in the newly created bin directory. In addition, a certain number of other iscd softwares (https://github.com/ISCDtoolbox) will also be installed.

In the case where the commands inside install.sh failed. We list belows the needed libraries:
* libgomp1 (5.4.0-6ubuntu1~16.04.10) which is the GCC OpenMP (GOMP) support library
* freeglut3-dev (version 2.8.1-2) which is the OpenGL Utility Toolkit development files
* libxmu-dev (version 2:1.1.2-2) which is the X11 miscellaneous utility library (development headers)
* libxi-dev (version 2:1.7.6-1) which is the X11 Input extension library (development headers)
* libptscotch-dev (version 5.1.12b.dfsg-2build4) which is the MPI programs and libraries for graph, mesh and hypergraph partitioning
* liblapacke-dev (3.6.0-2ubuntu2) which is the Library of linear algebra routines 3 - Headers

Moreover, as it can be seen in the sources directory, other softwares need to be installed (we still assume here that the commands in install.sh failed):
* medit to vizualize 3D mesh (see https://github.com/ISCDtoolbox/Medit)
* mshdist to redistanciate level-set function (see https://github.com/ISCDtoolbox/Mshdist)
* elastic to solve elasticity on 3D mesh (see https://github.com/ISCDtoolbox/LinearElasticity)
* advect to advect a level-function function thanks to a vector field (see https://github.com/ISCDtoolbox/Advection)
* mmg3d to adapt 3D mesh (see https://github.com/Mmgtools)

Important remark: the version of mmg3d we need for the mpdProgram is a modified version of the official one. Hence, when you have downloaded the git repository from mmgTools, overwrite the src files with the ones given in sources/* then cmake the new files in order to get to the good version of mmg3d 	


Normally, at the end of the execution of the install.sh file, we end up in a newly-created outputFiles directory. Choose the example you want to try (let us say hexahedra). Type

	cd exampleHexahedra

Then, execute the program, which takes inly one argument, the *.info file

	./../../bin/mpdProgram exHfHex.info

Similarly, the tetrahedra version can be tested by entering the exampleTetrahedra directory thanks to the cd command and then type

	./../../bin/mpdProgram exHfHex.info

At the end of a test, to vizualize a *.mesh file type (for example)

	./../../bin/medit ./../../inputFiles/exHfTetIni.mesh


The data result are saved in a *.data file. The user can restart a test by taking the *.restart and change the .restart extension by the .info one. The user can also see in this file all the different type of parameters that the mpdProgram can use. 


The mpdProgram program only takes one input argument: a *.info file (the file must have the *.info extension this is important)

In this file (examples are provided in outputFile directory), the minimal
configuration is to prescribe

* a *.wfn/ *.chem file (see example in the inputFile directory)

* the number of electrons to look for
 
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

	./*/mpdProgram *.info

and checks if it works

The documentation can be generated with doxygen and the Doxyfile in the source/mpd/documentation directory. Otherwise, an html version can be found online at

http://pelikano.free.fr/mpd/

Informations required (remove all underscore symbols '_' below ):

m_p_d_U_s_e_r 

m_p_d_2_0_1_8_*_0_7_j_d_a_l_p_h_i_n 

