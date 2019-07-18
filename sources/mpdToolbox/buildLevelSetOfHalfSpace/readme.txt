18/07/2019
The software buildLevelSetOfHalfSpace is a program written in C
by Jeremy Dalphin.

It only takes two arguments: the input *.mesh file name followed by the output
*.sol file name (enumeration of scalar values associated with the mesh
vertices).

The first *.mesh file must already exist (since its vertices are read).
Warning: the second *.sol file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

In the default situation, the program simply computes the signed distance
function of the half-space (x>=0) at the provided mesh vertices.

If the XYZ_COORDINATE preprocessor constant is set to 0 (respectively 1 or 2)
at compilation, then the program computes the signed distance of the half-space
(x>=0) (resp. y>=0 or z>=0) at the mesh vertices.

If the COMPLEMENT_OR_NOT preprocessor constant is set to 1, then it is the
complement half-space (xyz<=0) which is considered.

Finally, the preprocessor constant LEVELSET_VALUE (set to 0.0 by default) allows
to consider more general half-spaces (xyz>=LEVELSET_VALUE).

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc buildLevelSetOfHalfSpace.c -o buildLevelSetOfHalfSpace

and executed (on the provided example) by typing in a command prompt

./buildLevelSetOfHalfSpace example.mesh example.sol

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.mesh contains the
tetrahedral 3D mesh of an bowl-like shape half-filled with water, which
can be vizualized with medit software (https://github.com/ISCDtoolbox/Medit).
A version of medit is also provided with the mpdToolbox (MPD/sources/medit).

The *.sol result produced by the program can be also displayed with medit by
simply typing 'm' after the loading of the mesh.

The shape of example.mesh corresponds to the starting domain provided by
Alexiane Plessier for observing the evolution of the inclined flat water
interface with respect to the gravity and the 3D-Navier-Stokes equations inside
a bowl.

The example.jpg represents example.mesh on which is plotted the level-set
function of the upper half-space in the third-coordinate direction (z>=0).

