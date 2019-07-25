# mpdProgram
Maximizing Probability Domains

Version 2.0

September 2018

ISCD-Sorbonne Universités

Jeremy DALPHIN

The MPD program aims to compute the shape of a domain that maximizes the probability to find exactly a certain number of electrons inside it, given a chemical electronic structure.

Website of the MPD research project: http://pelikano.free.fr/mpd

Also, an mpdToolbox directory is available in MPD/sources/mpdToolbox (and in MPD/bin/mpdToolbox after installation) with little programs that can handle
tetrehedral mesh operation (intersection, ...), conversion (obj/cube/mesh), and
other features. We refer to the readme.txt file in MPD/sources/mpdToolbox and
to the ones in each program source directory for further details.

In order to compile the mpdProgram, you must have previously installed:
* gcc (version 5.4.0)
* make (version 4.1)
* cmake (version 3.5.1)
* doxygen (version 1.8.11) (optional: it is only necessary if one aims to generate the code documentation also available at http://pelikano.free.fr/mpd/siteInternet/html/index.html )

In order to download the source files of the MPD program, you have two options: either you can download the data compression file at

https://github.com/ISCDtoolbox/MPD/archive/master.zip

and uncompress the downloaded file; or install the git package

* git (version 2.7.4)

Then, open your terminal and locate yourself in the directory where you want to download the files thanks to "ls" and "cd" commands. To clone the files with git, type

	git clone https://github.com/ISCDtoolbox/MPD.git

To enter the MPD/ directory, type

	cd MPD

To allow the execution of the install.sh file, you may have to change the permissions for this file.

	chmod 740 install.sh

Then, to execute the commands inside the install.sh file, just type

	./install.sh

and follow the instructions.

Remark 1: the commented commands written in this install.sh file are originally intended for Ubuntu 16.04.4. Please modify these commands, if you use use for example yum instead of apt for package management (brew for mac), su for sudo, etc.

Remark 2: apart from the installed required libraries (PTHREAD, OPENMP, GLUT, OPENGL, SCOTCH, LAPACKE), the install.sh file will only install the softwares, libraries and include files in the local MPD/ directory.

The mpdProgram will be installed in the newly created bin/ directory. In addition, a certain number of other iscd softwares (https://github.com/ISCDtoolbox) will also be installed.

In the case where the commands inside the install.sh file failed, we list precisely below the needed external libraries (and of course their dependencies, this is normally treated automatically with any package manager):

* libpthread-stubs0-dev (0.3-4) which is the pthread stubs not provided by native libc, development files
* libgomp1 (5.4.0-6ubuntu1~16.04.10) which is the GCC OpenMP (GOMP) support library
* freeglut3-dev (version 2.8.1-2) which is the OpenGL Utility Toolkit development files
* libxmu-dev (version 2:1.1.2-2) which is the X11 miscellaneous utility library (development headers)
* libxi-dev (version 2:1.7.6-1) which is the X11 Input extension library (development headers)
* libptscotch-dev (version 5.1.12b.dfsg-2build4) which is the MPI programs and libraries for graph, mesh and hypergraph partitioning
* liblapacke-dev (3.6.0-2ubuntu2) which is the Library of linear algebra routines 3 - Headers

Moreover, as it can be seen in the sources/ directory, other softwares need to be installed (we still assume here that the commands in install.sh failed):
* medit to vizualize 3D mesh (see https://github.com/ISCDtoolbox/Medit)
* mshdist to redistanciate level-set function (see https://github.com/ISCDtoolbox/Mshdist)
* elastic to solve elasticity on 3D mesh (see https://github.com/ISCDtoolbox/LinearElasticity)
* advect to advect a level-function function thanks to a vector field (see https://github.com/ISCDtoolbox/Advection)
* mmg3d to adapt 3D mesh (see https://github.com/Mmgtools)

Important remark: the version of mmg3d we need for the mpdProgram is a modified version of the official one (and available at https://github.com/dalphin5/mmg/tree/myFeature ). Hence, in the case you have to download manually the git repository from Mmgtools, overwrite the src/mmg3d/* and src/common/* files with the ones given in sources/mmg3d/* and sources/common/* of our MPD git repository, respectively, then cmake the new files in order to get to the right version of mmg3d.

Remark: in order to install the advect, elastic and mshdist software, the Commons library (https://github.com/ISCDtoolbox/Commons) must have been previously installed

Normally, at the end of the execution of the install.sh file, we end up in a newly-created outputFiles/ directory. A quickstart guide is available at 

https://github.com/ISCDtoolbox/MPD/blob/master/mpdProgram_quickstart_guide.pdf

Choose the example you want to try (let us say the terahedral mesh). Type

	cd ./outputFiles/quickstartTests/test1/

Then, execute the program, which takes only one argument, the *.info file, by typing

	../../../bin/mpdProgram quickstart1.info

Similarly, the hexahedra version can be tested by entering the outputFiles/quickstartTests/test0/ directory thanks to the "cd" command and then type

	../../../bin/mpdProgram quickstart0.info

At the end of a test, to vizualize a *.mesh file, you can type for example

	../../../bin/medit ../../../inputFiles/quickstart1.ini.mesh

Thanks to C. Dobrzynski, there is an inline HTML documentation for the medit software available in french (https://www.ljll.math.upmc.fr/frey/logiciels/Docmedit.dir/index.html). There is also a technical report describing its main features (https://www.ljll.math.upmc.fr/frey/publications/RT-0253.pdf).

Let us recall briefly how to use medit.

The F1 key displays the plane cut and the mouse allows to rotate the mesh around it. Type again F1 to remove the plane cut.

If the plane cut is displayed, type F2 to change the mode of rotation. Now the plane cut moves and the mesh is fixed. Type again F2 to remove this mode.
 
By playing with F1 and F2 and the mouse we can rotate the computational box in order to display any part of the mesh (the direction up, down, right, left can also be used).

Type 'h' to get a description of the major shortcuts in the terminal window. Many options are also available by cliking on the right button of the mouse inside the medit window.

Type 'i' to restart the view from the beginning (e.g if you are lost).

Type 'z' the zoom and shift+z (i.e. 'Z') to dezoom.

Type shift+b (i.e. 'B') to display the computational bounding box. Type again shift+b (i.e. 'B') to remove it.

Type shift+a (i.e. 'A') to display the axis of the frame. Type again shift+a (i.e. 'A') to remove them.

Type shift+n (i.e. 'N') to display the normal vectors. Type again shift+n (i.e. 'N') to remove them.

Combine shift and the click on an element of the mesh to display its info in the terminal window. When an element is selected type 'r' to remove all the elements with the same label. type shift+r to display them again.

Combine ctrl and the selection of a rectangular zone with the mouse then type 'z' to zoom on this selected zone.

The main data computed by the mpdProgram are saved in a *.data file. The user can restart a test by taking the *.restart file and change the .restart extension by the .info one. The user can also see in this file all the different type of parameters that the mpdProgram can use. The default values of these parameters are set at the compilation by the preprocessor constants inside the sources/mpd/sources/loadParameters.h file. Everything placed after the (mandatory) end_data keyword in the *.info file is considered as a comment.

The mpdProgram program only takes one input argument: a *.info file (the file must have the .info extension this is important).

In this file (examples are provided in outputFile directory), the minimal
configuration is to prescribe

* a *.wfn/ *.chem file (see example in the inputFiles directory)

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

for optimizing the hexahedral mesh (opt_mode -1 uses the shape gradient to add or remove boundary hexahedra; opt_mode -2 tries to add or remove exhaustively the boundary hexahedra).

Write

	opt_mode 1 

for optimizing the tetrahedral one (opt_mode 2 performs Eulerian (level-set) perturbations; opt_mode 3 performs Lagrangian perturbations; opt_mode 4 defines the new domain as the positive part of the shape gradient of the previous domain in the optimization loop; should only be used for one iteration, very unstable mode, used to catch regions far from nuclei center with very low probability).

Write

	n_cpu 4

for prescribing (for example) 4 threads during the execution of the mpdProgram.

If no internal domain is prescribed inside the mesh (or if no mesh is precribed), the ls_type keyword allows to define an initial cube (ls_type 0) or sphere (ls_type 1) defined by its center
(ls_x, ls_y, and ls_z keywords) and size/radius (ls_r keyword).

Finally, in order to launch the mpdProgram, the line command syntax is the following:

	/*/mpdProgram *.info

where the * symbol must be change depending on your where is located your mpdProgram and what is the name of your *.info file.

We also mention that the mpdProgram can generate *.cube (respectively *.obj) files for vizualizing the boundary of the domain in the hexahedral (resp. tetrahedral) mode. These files can be generated together with the *.mesh files if the line

	save_type 2

appears in the *.info file. These format can be read thanks to the Jmol software (available at http://jmol.sourceforge.net/ ) or also Paraview (https://www.paraview.org/ ). They allow to vizualize the boundary mesh and the molecules together. We quickly describe how to use Jmol. An online documentation for the Console commands are available at https://chemapps.stolaf.edu/jmol/docs/

Once installed, in order to launch the Jmol software (warning: java must have been previously installed), type in a command prompt (we assume the user is located in the Jmol directory)

	java -jar ./Jmol.jar

If it is not the first time you use Jmol, you can also type

	./jmol.sh

The software can also work in ssh with the -X or -Y mode activated (whereas the medit software can fail in ssh with graphical mode actived if one of the two communicating computers has an nvidia cards and the other not). 

Then, click on the upper left pannel named File and open the Console by clicking on it.

Whatever optimization mode you used, the MPD program always produce an *.cube file. This file can be used to display the molecule on Jmol by typing in the Console the following command

	load "pathOfTheFile.cube"

In order to plot a specific domain (more precisely its boundary), you can type in the Console

	isosurface "PathOfTheFile.cube" mesh translucent

or

	isosurface "PathOfTheFile.obj" mesh translucent

depending if you have an *.cube file (hexahedral mesh) or an *.obj one (tetrahedral mesh). The mesh option display the surfacic mesh, where the translucent one makes it a bit transparent. Many option can be done with the mouse and the graphical interface but they can also be done directly with commands in the Console. We detail a few of them.

Type 'rotate x 45' to rotate around the x axis of 45 degrees (idem for y and z).

Similarly, type translate x -10 to translate from the left.

Type 'zoom out' to zoom out or 'zoom 45' to zoom at 45%.

Type 'spacefill off' to remove the atom but keep the interaction between them. 

Finally, in order to save multiple *.jpg photo from a molecule (after having adjusted the view thanks to the previous commands), one can type in the Console (here we take the example of *.obj files)

	for (var i=0; i<=9; i=i+1)
 	  var objName="pathOfTheFile/nameOfTheFileWithoutExtension."+(i)+".obj"
	  isosurface @objName mesh translucent
	  var jpgName="pathOfTheFile/nameOfTheFileWithoutExtension.0"+(i)+".jpg"
 	  write IMAGE 800 600 JPG @jpgName
	end for

	for (var i=10; i<=31; i=i+1)
	  var objName="pathOfTheFile/nameOfTheFileWithoutExtension."+(i)+".obj"
	  isosurface @objName mesh translucent
	  var jpgName="pathOfTheFile/nameOfTheFileWithoutExtension."+(i)+".jpg"
	  write IMAGE 800 600 JPG @jpgName
	end for

The documentation of the mpd code can be generated with doxygen and the Doxyfile in the source/mpd/documentation/ directory. Otherwise, an html version can be found online at

http://pelikano.free.fr/mpd/siteInternet/html/index.html

Some informations are required (follow the instructions given). 

Finally, we recall that a quickstart guide is available at 

https://github.com/ISCDtoolbox/MPD/blob/master/mpdProgram_quickstart_guide.pdf


