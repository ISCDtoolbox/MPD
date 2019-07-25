25/07/2019
The software refineCubeMesh is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input *.mesh file name followed by the output
*.mesh file name.

The first *.mesh file must already exist (since it is read).

Warning: the second *.mesh file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

The program loads the *.mesh file, which is assumed to contain the description
of an hexahedral mesh. Then, it refines the grid dsicretization by two and
save the new mesh in the output *.mesh file.

If an internal domain exists, then the hexahedra must be labelled 3 if they
belongs to the domain, otherwise labelled 2. Similarly, boundary quadrilaterals
are labelled 10 for those belonging to the boundary of the domain. 

Remark: we use the convention of mmg3d software (a version is provided with the
        mpdToolbox MPD/sources/mmg3d or see https://github.com/MmgTools/mmg):
        hexahedra labelled 3 if they belongs to the domain, otherwise 2. These
        values cannot be changed in this version of the program

Remark: the discretization parameters are directly extracted from the
        grid structure of the mesh vertices. The TOLERANCE_DISCRETIZATION
        preprocessor constant rules the tolerance allowed on the variation of
        discretization step between vertices (by default set to 1.e-6)

Remark: do not forget to recompile the program if TOLERANCE_DISCRETIZATION,
        or NAME_SIZE_MAX are changed from their default values.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc refineCubeMesh.c -o refineCubeMesh

and executed (on the provided example) by typing in a command prompt

./refineCubeMesh example.ini.mesh example.out.mesh

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.mesh contains the
hexahedral mesh of the cube [-3.55 3.55]x[-3.55 3.55]x[[-3.37675 3.72325],
a subpart of it meshing a domain. The internal domain has been obtained by
maximizing the probability of exactly finding 8 electrons inside, and 2 outside
for the HF molecule. It is a model in quantum chemistry that allows to
represents in particuler here, the complement of the valence region
representing the interaction between the hydrogen and fluor atoms. This domain
is the one obtained when a certain cubical algorithm oscillates after having
followed the shape gradient.

The example.ini.mesh file and the example.out.mesh produced by the progam
can be vizualized with medit software (https://github.com/ISCDtoolbox/Medit).
A version of medit is also provided with the mpdToolbox (MPD/sources/medit).

The initial mesh and resulting mesh are displayed in example.ini.jpg and
example.out.jpg files, respectively.

