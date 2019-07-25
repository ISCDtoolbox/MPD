24/07/2019
The software hexaToTetra is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input *.mesh file name followed by the output
*.mesh file name.

The first *.mesh file must already exist (since it is read). It is also assumed
that it contains a three-dimensional hexahedral mesh.

Warning: the second *.mesh file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

The program loads the grid data from the input *.mesh file and converts the
mesh into a tetrahedral mesh saved in the output *.mesh file.

Remark: the discretization parameters are directly extracted from the
        grid structure of the mesh vertices. The TOLERANCE_DISCRETIZATION
        preprocessor constant rules the tolerance allowed on the variation of
        discretization step between vertices (by default set to 1.e-6)

Remark: do not forget to recompile the program if TOLERANCE_DISCRETIZATION or
        NAME_SIZE_MAX are changed from their default value.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc hexaToTetra.c -o hexaToTetra

and executed (on the provided example) by typing in a command prompt

./hexaToTetra example.ini.mesh example.out.mesh

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.cube contains the
3D grid mesh of the cube [-3.55 3.55]x[-3.55 3.55]x[-5.10925 1.99075],
a subpart of it meshing a domain. The internal domain has been obtained by
maximizing the probability of exactly finding 2 electrons inside, and 8 outside
for the HF molecule. It is a model in quantum chemistry that allows to
represents in particuler here, the the valence region representing the
interaction between the hydrogen and fluor atoms.

A rough screenshot of the mesh is displayed in the example.jpg file.

The hexaToTetra program converts the hexahedral mesh of example.ini.mesh file
into an tetrahedral one saved in the example.out.mesh file.

The example.ini.mesh and example.out.mesh files can be vizualized with medit
software (https://github.com/ISCDtoolbox/Medit). A version of medit is also
provided with the mpdToolbox (MPD/sources/medit).

