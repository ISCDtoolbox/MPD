24/07/2019
The software cubeToMesh is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input *.cube file name followed by the output
*.mesh file name.

The first *.cube file must already exist (since it is read).

Warning: the second *.mesh file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

The program loads the grid data from the input *.cube file and converts them
in order to create an hexahedral mesh saved in the output *.mesh file.

If an internal domain exists (cube labelled 1 for those belonging to the domain,
otherwise labelled 0). The final mesh contains as a subpart the boundaries of
this domain. The hexahedra belonging to the domain are labelled
LABEL_INTERNAL_TETRA and those outside are tagged LABEL_EXTERNAL_TETRA. If no
internal domain exists, the hexahedra are labelled zero.

The boundary quadrilaterals of the computational box are labelled from 1 to 6
for each face of the cube created. If an internal domain is detected, the
boundary quadrilaterals of the domain are labelled LABEL_BOUNDARY.

Remark: we use the convention of mmg3d software (a version is provided with the
        mpdToolbox MPD/sources/mmg3d or see https://github.com/MmgTools/mmg):
        hexahedra labelled 3 if they belongs to the domain, otherwise 2;
        boundary quadrilaterals labelled 10 if they belongs to the domain
        boundary. The values can be changed thanks to the LABEL_BOUNDARY,
        LABEL_INTERNAL_TETRA and LABEL_EXTERNAL_TETRA preprocessor constants.

Remark: do not forget to recompile the program if LABEL_INTERNAL_TETRA,
        LABEL_EXTERNAL_TETRA, LABEL_BOUNDARY or NAME_SIZE_MAX are changed from
        their default value.

Remark: the cubeToMesh program can be used to quickly generate an hexahedral
        of a cube with a prescribed discretization. Indeed, the *.cube format
        file is very simple to right (the chemical part is not taken into
        account by the cubeToMesh program) and it is necessary to specify
        labels at the end (in this case hexahedra will be labelled 0).

We give an example of simple *.cube file to generate a cube discretization
as an hexahedral mesh:

The first two lines are considered as comments
Here is an example of *.cube with prescrubed value to change
1 xMin yMin  zMin
nX deltaX 0.0    0.0
nY 0.0    deltaY 0.0
nZ 0.0    0.0    deltaZ
1 1.0 0.0 0.0 0.0

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc cubeToMesh.c -o cubeToMesh

and executed (on the provided example) by typing in a command prompt

./cubeToMesh example.cube example.mesh

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.cube contains the
3D grid mesh of the cube [-3.55 3.55]x[-3.55 3.55]x[-5.109251,1.990749],
a subpart of it meshing a domain. The internal domain has been obtained by
maximizing the probability of exactly finding 2 electrons inside, and 8 outside
for the HF molecule. It is a model in quantum chemistry that allows to
represents in particuler here, the the valence region representing the
interaction between the hydrogen and fluor atoms.

The HF molecule with the domain are represented in example.jpg file and can be
vizualized with Jmol software (http://jmol.sourceforge.net/index.fr.html)
Warning: Jmol is not provided with the mpdToolbox.

They can be vizualized with Jmol by typing successively in the Jmol console

load "example.cube"
zoom 50
rotate x 90
rotate z 90
isosurface "example.cube" mesh translucent
write IMAGE 800 600 JPG "example.jpg"

The cubeToMesh program converts the example.cube file into a example.mesh
containing the hexahedral mesh of the grid data.

The example.mesh file can be vizualized with medit software
(https://github.com/ISCDtoolbox/Medit). A version of medit is also provided
with the mpdToolbox (MPD/sources/medit).

