24/07/2019
The software vectorFieldNormAtVertices is a program written in C by Jeremy
Dalphin.

It only takes two arguments: the input *.sol file name followed by the output
*.sol file name (enumeration of scalar values associated with mesh vertices).

The first *.sol file must already exist (since it is read) and contain the
three-dimensional values of a vector field computed at the vertices of a mesh.

Warning: the second *.sol file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character). Do not forget to recompile the program
if NAME_SIZE_MAX is changed from its default value.

The program loads the vectorial solutions of the input *.sol file, then computes
the norm of the vector field at the mesh vertices, and finally saves the result
in a scalar output *.sol file.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc vectorFieldNormAtVertices.c -lm -o vectorFieldNormAtVertices

and executed (on the provided example) by typing in a command prompt

./vectorFieldNormAtVertices example.ini.sol example.out.sol

Remark: do not forget to add the math linker (-lm option) with gcc, since the
sqrt function is used by the vectorFieldNormAtVertices program.

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.ini.mesh contains the
tetrahedral 3D mesh of the cube [-3.55 3.55]x[-3.55 3.55]x[-3.37675 3.72325],
a subpart of it meshing a red-blood-cell-like domain. The internal domain has
been obtained by maximizing the probability of exactly finding 8 electrons
inside, and 2 outside for the HF molecule. It is a model in quantum chemistry
that allows to represents in particuler here, the complement of the valence
region representing the intraction between the hydrogen and fluor atoms.

The example.ini.mesh file can be vizualized with medit software
(https://github.com/ISCDtoolbox/Medit). A version of medit is also provided
with the mpdToolbox (MPD/sources/medit).

The mesh is adapted to the metric corresponding to the one associated with the
HF molecule. We can observe that there are a lot of tetrahedra around the
cores and fewer as we get further.

Associated with this mesh, there is a vectorial example.ini.sol file that
contains a vector field computed only the boundary of the internal domain
(zero vector otherwise). It represents the (shape gradient) x (normal vector)
associated with the previously described shape optimization problem.

The example.ini.sol vector filed can be also displayed with medit by simply
typing 'w' after the loading of the mesh.

A screenshot of the mesh together with the vector filed is displayed in the
example.ini.jpg file.

The vectorFieldNormAtVertices program computes the norm of each vectors of
example.ini.sol and write an output file entitled example.out.sol with the
corresponding values.

A copy of example.ini.mesh and entitled example.out.mesh is also available
in order to display the example.out.sol file produced by the program by
simply typing 'm' after the loading of the example.out.mesh file.

A screenshot of the result (zoomed at the interesting region) can be found in
example.out.jpg file.

