23/07/2019
The software objMerge is a program written in C by Jeremy Dalphin.

It takes at least three arguments: two *.obj input file names followed by the
output *.obj file name (containing the merged input data). More generally, the
objMerge program contains (n+1) *.obj file names: the first n arguments are the
names of the input *.obj files the user wants to merge together, and the
(n+1)-th argument is the name of the output *.obj file that will contain the
saved merged data. 

The *.obj format is made of successive lines of 'v x1 x2 x3' followed by some
'f i1 i2 i3' describing vertex coordinates followed by the enumeration of
triangulated faces).

The n first *.obj file must already exist (since they are read). Warning: the
last *.obj file is overwritten if it already exits (it is automatically created
if it does not exist). The maximal number of characters allowed in the file
names is 500.

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character, and recompile the objMerge program if
NAME_SIZE_MAX is changed from its default value).

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc objMerge.c -o objMerge

and executed (on the provided example) by typing in a command prompt

./objMerge example1.ini.obj example2.ini.obj example.out.obj

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The files example1.ini.obj and
example2.ini.obj contain the three-dimensional boundary meshes of domains
maximazing the probability to find two electrons inside it (and eight outside),
for the water molecule H2O. The two regions can be interpreted as the two lone
pairs of the oxygen atom.

Other domains of H2O that maximize the probability to find two electrons inside
and eight outside are also given in example3.ini.obj (valence region of the
oxygen-hydrogen interaction), example4.ini.obj (idem with the other hydrogen
atom), and example5.ini.obj (core region of the oxygen).

The H2O molecule can be vizualized with Jmol software
(http://jmol.sourceforge.net/index.fr.html) and the example.cube file
(Warning: Jmol is not provided with the mpdToolbox).

The example1.ini.obj and example2.ini.obj files are the input of objMerge
The example.out.obj file is the output produced by the program

They can be vizualized with Jmol by typing successively in the Jmol console

load "example.cube"
zoom 30
rotate x -75
isosurface "example1.ini.obj" mesh translucent
isosurface "example2.ini.obj" mesh translucent

isosurface "example.out.obj" mesh translucent
write IMAGE 800 600 JPG "example.out.jpg"

