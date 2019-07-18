18/07/2019
The software resizeObjFile is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input name followed by the output name of two
*.obj files (successive lines of 'v x1 x2 x3' followed by some 'f i1 i2 i3'
describing vertex coordinates followed by the enumeration of triangulated
faces).

The first *.obj file must already exist (since it is read).
Warning: the second *.obj file is overwritten it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500.

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

If the program is compiled with the preprocessor constant BOHR_OR_NOT set to 0
(default situation), then the program loads data from the first *.obj file,
then multiply all the point coordinates by the BOHR_RADIUS (0.5291772109217),
to finally write the new resized *.obj file.

Otherwise (i.e. BOHR_OR_NOT is set to 1 during compilation), the program
divides instead of multiplying the point coordinates by BOHR_RADIUS.

Physically, if BOHR_OR_NOT is set to 0 at compilation, it means that the input
*.obj file contains coordinates in Bohrs, which is converted into Angstroms
(1.e-10m) in the ouput *.obj file.

Conversely, if BOHR_OR_NOT is set to 1 at compilation, the program converts
the point coordinates from Angstroms to Bohrs.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc resizedObjFile.c -o resizeObjFile

and executed (on the provided example) by typing in a command prompt

./resizeObjFile example.ini.obj example.out.obj

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.mesh contains the
tetrahedral 3D mesh of an ovaloid-like shape inside a computational box, which
can be vizualized with medit software (https://github.com/ISCDtoolbox/Medit).
A version of medit is also provided with the mpdToolbox (MPD/sources/medit).

The shape of example.mesh corresponds to the domain maximizing the probability
to find two electrons inside it (and two outside), for the molecule HeH2. It
can be interpeted as the representation of the core region of the Helium atom
in HeH2.

The HeH2 molecule can be vizualized with Jmol software
(http://jmol.sourceforge.net/index.fr.html) and the example.cube file
(Warning: Jmol is not provided with the mpdToolbox).

The example.ini.obj file is the input of resizeObjFile program
The example.out.obj file is the output produced by the program

They can be vizualized with Jmol by typing successively in the Jmol console

load "example.cube"
rotate x 45
rotate y 45
rotate z 45
zoom 35
isosurface "example.ini.obj" mesh translucent
write IMAGE 800 600 JPG "example.ini.jpg"
isosurface "example.out.obj" mesh translucent
write IMAGE 800 600 JPG "example.out.jpg"

