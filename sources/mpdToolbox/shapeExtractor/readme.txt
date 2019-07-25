22/07/2019
The software shapeExtractor is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input *.mesh file name followed by the output
*.mesh file name.

The first *.mesh file must already exist (since it is read).

Warning: the output *.mesh file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

The program assumes that the input *.mesh file contains (strictly) as a subpart
a domain inside its three-dimensional structure (tetrahedra labelled 3 if they
belongs to the domain, otherwise 2; triangles labelles 10 meshing the domain
boundary).

Then, the shapeExtractor program loads the mesh and extracts the internal domain
so that the ouput *.mesh file produced contains only the mesh part of this
internal domain, which becomes the computational box.

The program labels 0 the tetrahedra and 1 the boundary triangles of the output
mesh.

To be more precise, the new (output) mesh will contains only the vertices,
tetrahedra and triangles of the internal domain.

In addition, if the preprocessor constants INCLUDE_NORMALS is set to 1 (default
value), then the normal vectors associated with the internal domain are also
extracted. Otherwise, INCLUDE_NORMALS must be set to zero (default value) and no
other elements of the input mesh structure will appear in the output
extracted mesh.

Warning: if INCLUDE_NORMALS is set to 1, then it assumes that the normal vectors
         are computed on the mesh boundary.

Remark: we use the convention of mmg3d software (a version is provided with the
        mpdToolbox MPD/sources/mmg3d or see https://github.com/MmgTools/mmg):
        tetrahedra labelled 3 if they belongs to the domain, otherwise 2;
        triangles labelles 10 meshing the domain boundary. The values can be
        changed thanks to the LABEL_INTERNAL_TETRA and LABEL_BOUNDARY
        preprocessor constants.

Remark: do not forget to recompile the program if LABEL_INTERNAL_TETRA,
        INCLUDE_NORMALS or LABEL_BOUNDARY are changed from their default value.

Warning: the algorithm used by the shapeExtractor program to build the new mesh
         is done by brute force (during the building of the adjacency table, the
         current value is compared to all previous treated values) and it is by
         no means optimal (building an hash table woul greatly reduce the time
         computation here). Hence, if the mesh is large, it can take a while.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc shapeExtractor.c -o shapeExtractor

and executed (on the provided example) by typing in a command prompt

./shapeExtractor example.ini.mesh example.out.mesh

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.ini.mesh contains the
tetrahedral 3D mesh of the computational box [-6 6]x[-6,6]x[-6,6], a subpart of
it meshing a domain. The internal domain has been obtained by maximizing the
probability of exactly finding 2 electrons inside the cube for the dihydrogen
molecule H2. The theoretical solution of such optimization problem is the whole
space with probability one, but numerically, up to 0.995, the solution obtained
is the internal domain given in the example.ini.mesh file.

The example.ini.mesh file can be vizualized with medit software
(https://github.com/ISCDtoolbox/Medit). A version of medit is also provided
with the mpdToolbox (MPD/sources/medit).

The mesh is adapted to the metric corresponding to the one associated with the
dihydrogen molecule H2. We can observe that there are a lot of tetrahedra around
the cores and fewer as we get further.

A screenshot of the mesh can be found in example.ini.jpg file.

The program shapeExtractor loads the example.ini.mesh file and extracts only the
internal domain so that example.out.mesh contains only this part of the mesh.

