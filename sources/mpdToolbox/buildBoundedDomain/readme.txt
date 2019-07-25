23/07/2019
The software buildBoundedDomain is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input *.mesh file name followed by the output
*.obj file name.

The first *.mesh file must already exist (since it is read) as well as its
associated *.sol file (same name replacing the *.mesh extension by the
*.sol one).

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

It also assumes that the (input) *.sol file associated with the mesh contains
the level-set function of the current domain (a subpart of the mesh).

Then, the program extracts the domain boundary, closing the surface with the
boundary of the computational box if necessary, and save the triangulated mesh
in the output *.obj format (successive lines of 'v x1 x2 x3' followed by some
'f i1 i2 i3' describing vertex coordinates followed by the enumeration of
triangulated faces).

The volume of the domain inside the computational box is also computed and
displayed in the standard output stream.

Remark: we use the convention of mmg3d software (a version is provided with the
        mpdToolbox MPD/sources/mmg3d or see https://github.com/MmgTools/mmg):
        tetrahedra labelled 3 if they belongs to the domain, otherwise 2;
        triangles labelles 10 meshing the domain boundary. The values can be
        changed thanks to the LABEL_INTERNAL_TETRA and LABEL_BOUNDARY
        preprocessor constants.

If the program is compiled with the preprocessor constant BOHR_OR_NOT set to 1
(default situation), then the program saves the mesh vertices in the output
*.obj file multiplying all the point coordinates by the BOHR_RADIUS (0.5291772109217) preprocessor constant.

Otherwise, BOHR_OR_NOT must be set to 0 and nothing is particularly done.

Physically, if BOHR_OR_NOT is set to 0 at compilation, it means that the input
*.obj file contains coordinates in Bohrs, which is converted into Angstroms
(1.e-10m) in the ouput *.obj file.

Conversely, if BOHR_OR_NOT is set to 1 at compilation, the program converts
the point coordinates from Angstroms to Bohrs.

Remark: do not forget to recompile the program if LABEL_INTERNAL_TETRA,
        LABEL_BOUNDARY, or BOHR_OR_NOT are changed from their default values.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc buildBoundedDomain.c -o buildBoundedDomain

and executed (on the provided example) by typing in a command prompt

./buildBoundedDomain example.mesh example.obj

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.mesh contains the
tetrahedral 3D mesh of an ovaloid-like computational box obtained as the domain
maximizing the probability of finding exactly 14 electrons inside it for the
di-nitrogen molecule N2. The theoretical solution of such optimization problem
is the whole space with probability one, but numerically, up to 0.995, the
solution obtained is the computational box of example.mesh file.

Then, inside this computational box, as a subpart of the mesh (tetrahedra labelled 3 and triangles labelled 10), there is a domain maximizing the
probability of exactly finding 6 electrons inside and 8 electrons outside for
the di-nitrogen molecule N2.

The level-set function of this domain is computed at the mesh vertices and
saved in the example.sol file.

The example.mesh file can be vizualized with medit software
(https://github.com/ISCDtoolbox/Medit). A version of medit is also provided
with the mpdToolbox (MPD/sources/medit).

The associated example.sol file can be also displayed with medit by
simply typing 'm' after the loading of the mesh.

The mesh is adapted to the metric corresponding to the one associated with the
di-nitrogen molecule N2. We can observe that there are a lot of tetrahedra
around the cores and fewer as we get further.

A screenshot of the mesh together with the level-set can be found in
example.ini.jpg file.

The program buildBoundedDomain loads the example.mesh and example.sol files,
then extracts only the internal domain so that example.obj contains only this
triangulated part of the mesh, closing the domain with the boundary of the
computational box if necessary.

The N2 molecule can be vizualized with Jmol software
(http://jmol.sourceforge.net/index.fr.html) and the example.cube file
(Warning: Jmol is not provided with the mpdToolbox).

The example.obj is the output produced by the buildBoundDomain program.

It can be vizualized with Jmol by typing successively in the Jmol console

load "example.cube"
zoom 50
rotate x 90
rotate z 90
isosurface "example.obj" mesh translucent
write IMAGE 800 600 JPG "example.out.jpg"

