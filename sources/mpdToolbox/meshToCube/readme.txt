24/07/2019
The software meshToCube is a program written in C by Jeremy Dalphin.

It only takes three arguments: the input *.mesh file name followed by the input
*.wfn file and finally the output *.cube file name.

The first *.mesh and *.wfn files must already exist (since they are read).

Warning: the third *.cube file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

The program loads the *.mesh file, which is assumed to contain the description
of an hexahedral mesh. Then, it loads the charge and positions of nucleus
contained in the input *.wfn. Finally, it converts the mesh into grid data
saved in the *.cube file.

If an internal domain exists (hexahedra labelled 3 for those belonging to the
domain, otherwise labelled 2). The final cube labels are set to 1.0
for those belonging to the domain and those outside are tagged directly 0.0

If no internal domain exists, then it tries to look at an associated *.sol
file and if the file exists, it loads the values. Assuming that they represents
the level-set function of a domain computed at the mesh vertices, the grid
labels saved in the output *.cube file are set to 1.0 for negative value of
hexahedral with a sum of eight vertices value negative, otherwise 0.0 in that
case.

Remark: we use the convention of mmg3d software (a version is provided with the
        mpdToolbox MPD/sources/mmg3d or see https://github.com/MmgTools/mmg):
        hexahedra labelled 3 if they belongs to the domain, otherwise 2. The
        values can be changed thanks to the LABEL_INTERIOR preprocessor
        constants.

Remark: the discretization parameters are directly extracted from the
        grid structure of the mesh vertices. The TOLERANCE_DISCRETIZATION
        preprocessor constant rules the tolerance allowed on the variation of
        discretization step between vertices (by default set to 1.e-6)

Remark: do not forget to recompile the program if TOLERANCE_DISCRETIZATION,
        LABEL_INTERIOR, or NAME_SIZE_MAX are changed from their default values.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc meshToCube.c -o meshToCube

and executed (on the provided example) by typing in a command prompt

./meshToCube example.mesh example.wfn example.cube

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.mesh contains the
hexahedral mesh of the cube [-3.55 3.55]x[-3.55 3.55]x[-5.109251,1.990749],
a subpart of it meshing a domain. The internal domain has been obtained by
maximizing the probability of exactly finding 2 electrons inside, and 8 outside
for the HF molecule. It is a model in quantum chemistry that allows to
represents in particuler here, the the valence region representing the
interaction between the hydrogen and fluor atoms.

The example.mesh file can be vizualized with medit software
(https://github.com/ISCDtoolbox/Medit). A version of medit is also provided
with the mpdToolbox (MPD/sources/medit).

The HF molecule with the domain are represented in example.jpg file and the
example.cube file produced by the meshToCube program can be
vizualized with Jmol software (http://jmol.sourceforge.net/index.fr.html)
Warning: Jmol is not provided with the mpdToolbox.

They can be vizualized with Jmol by typing successively in the Jmol console

load "example.cube"
zoom 50
rotate x 90
rotate z 90
isosurface "example.cube" mesh translucent
write IMAGE 800 600 JPG "example.jpg"

////////////////////////////////////////////////////////////////////////////////

Strategy to follow if one aims to computes project a tetrahedral domain on a
hexahedral one.

For example, we aim to convert the tetraToHexa.out.mesh file (up to a projection
of a tetrahedral mesh into an hexahedral one) into a tetraToHexa.out.cube file.

Step 1: generate the level-set function of the domain in tetraToHexa.out.mesh
        This can be done with the help of mshdist software (with -dom option)
        (https://github.com/ISCDtoolbox/Mshdist). A version of mshdist is also
        provided with the mpdToolbox (MPD/sources/mshdist)

./mshdist tetraToHexa.out.mesh -dom -ncpu 1

        This prompt command will generate a tetraToHexa.out.sol file containing
        the level-set function associated with the domain contained in the
        tetraToHexa.out.mesh computed at the mesh vertices.

Step 2: build a hexahedral mesh from a chosen grid discretization saved in a
        example.ini.cube file.

       We give an example of simple *.cube file to generate a cube
       discretization as an hexahedral mesh (do not specify the grid labels
       here):

       The first two lines are considered as comments
       Here is an example of *.cube with prescribed value to change
       1 xMin yMin  zMin
       nX deltaX 0.0    0.0
       nY 0.0    deltaY 0.0
       nZ 0.0    0.0    deltaZ
       1 1.0 0.0 0.0 0.0

        This step can be done with the help of the cubeToMesh software
        (MPD/sources/mpdToolbox/cubeToMesh)

./cubeToMesh tetraToHexa.ini.cube tetraToHexa.ini.mesh

        This prompt command will generate a example.ini.mesh file containing
        the hexahedral mesh specified in the example.ini.cube file.

Step 3: transform the hexahedral domain into a tetrahedral one.
        This step can be done with the help of the hexaToTetra software
        (MPD/sources/mpdToolbox/hexaToTetra)

./hexaToTetra tetraToHexa.ini.mesh tetraToHexa.temp.mesh

        This prompt command will generate a tetraToHexa.temp.mesh file
        containing the tetrahedral mesh associated with our grid discretization.

Step 4: project the tetraToHexa.out.sol values on the vertices of
        tetraToHexa.temp.mesh file. This can be done with the help of the
        mshint software
        https://github.com/ISCDtoolbox/AdaptTools/tree/olderVersionThatWorks).
        A version of mshint is also provided with the mpdToolbox
        (MPD/sources/mpdToolbox/adaptTools/bin/mshint)

./mshint tetraToHexa.out.mesh tetraToHexa.temp.mesh

        This prompt command will generate a tetraToHexa.temp.sol file containing
        the level-set function associated with tetraToHexa.out.mesh projected
        at the vertices of tetraToHexa.temp.mesh (which is also the same
        as for tetraToHexa.ini.mesh, this is the trick here).

        Warning: if the grid chosen in tetraToHexa.ini.cube file is too
                 fine, then mshint may take a lot of time since it is not
                 optimized for this prupose at the origin. Hence, try to
                 chose a reasonable grid.

Step 5: create the final grid data from the knowledge of tetraToHexa.ini.mesh
        tetraToHexa.temp.sol and tetraToHexa.ini.wfn that inteprolate the
        tetrahedral mesh into the grid and saved in a tetraToHexa.out.cube file.

cp tetraToHexa.temp.sol tetraToHexa.ini.sol

./meshToCube tetraToHexa.ini.mesh tetraToHexa.ini.wfn tetraToHexa.out.cube

        This prompt command will generate a tetraToHexa.out.cube file containing
        the projection of the tetrahedral domain of tetraToHexa.out.mesh file
        on the grid prescribed in tetraToHexa.ini.cube file.


The result can be vizualized with Jmol software
(http://jmol.sourceforge.net/index.fr.html)
Warning: Jmol is not provided with the mpdToolbox.

Type successively in the Jmol console

load "tetraToHexa.out.cube"
zoom 50
rotate x 90
rotate z 90
rotate y 45
isosurface "tetraToHexa.out.cube" mesh translucent
write IMAGE 800 600 JPG "tetraToHexa.out.jpg"

A screenshot of the result can be found in the tetraToHexa.out.jpg file.

