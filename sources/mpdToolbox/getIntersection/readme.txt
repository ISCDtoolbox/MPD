17/07/2019
The software getIntersection is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input *.mesh file name followed by the output
*.sol file name (enumeration of scalar values associated with the mesh
vertices).

The first *.mesh file must already exist (since its vertices are read)
as well as its associated *.sol file (the same name but with the *.mesh
extension changed by the *.sol one).

Warning: the output *.sol file is overwritten if it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

The program assumes that the input *.mesh file contain a domain inside its
three-dimensional structure (tetrahedra labelled 3 if they belongs to the
domain, otherwise 2; triangles labelles 10 meshing the domain boundary).

It also assumes that the (input) *.sol file associated with the mesh contains
the level-set function of another domain projected on the current one.

Then, the program computes either the reunion, the intersection (default
operator), or the symmetric difference of these two domains via the computation
a level-set function at the mesh vertices, which is saved in the output *.sol
file.

Remark: we use the convention of mmg3d software (a version is provided with the
        mpdToolbox MPD/sources/mmg3d or see https://github.com/MmgTools/mmg):
        tetrahedra labelled 3 if they belongs to the domain, otherwise 2;
        triangles labelles 10 meshing the domain boundary. The values can be
        changed thanks to the LABEL_INTERNAL_TETRA and LABEL_BOUNDARY
        preprocessor constants.

The LS_OPERATION preprocessor constant rule the type of operation to perform
on the level-set functions of the domains:
0=min(reunion) 1=max (intersection), 2=product(symmetric difference).

Remark: do not forget to recompile the program if LABEL_INTERNAL_TETRA,
        LABEL_BOUNDARY, or LS_OPERATION is changed from its default value.

Warning: the level-set function w build is not a signed distance function
         meaning that it does not satisfies the normalization condition:
         || grad(w) || = 1

Remark: the centers and radii of the two spheres can be changed at compilation.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc getIntersection.c -o getIntersection

and executed (on the provided example) by typing in a command prompt

./getIntersection example.mesh exampleIntersection.sol

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.mesh contains the
tetrahedral 3D mesh of a computational box, a subpart of it meshing a domain.
The computational box has been obtained by maximizing the probability of
exactly finding 10 electrons inside it for the water molecule H2O. The
 theoretical solution of such optimization problem is the whole space with
probability one, but numerically, up to 0.995, the solution obtained is the
mesh given by the example.mesh file.

Inside this computational box, there is a certain sub-domain corresponding to
the domain locally maximizing the probability of finding exactly two electrons
inside and eight electrons outside for the water molecule H2O. It is a model
in quantum chemistry that allows to represents in particuler here, the valence
interaction between the oxygen and one hydrogen.

It can be vizualized with medit software (https://github.com/ISCDtoolbox/Medit).
A version of medit is also provided with the mpdToolbox (MPD/sources/medit).

The mesh is adapted to the metric provided in the metric.sol file, and
corresponds to the one associated with the water molecule H2O.

Of course, due to the symmetry of the water molecule H2O, there is another
domain solution of the optimization problem than the one given in the *.mesh
file. It corresponds to the other valence interaction and symmetric with respect
to the plane (z=0). The level-set function of this other domain has been
computed at the mesh vertices and is saved in the example.sol file.

The example.sol can be also displayed with medit by simply typing 'm' after the
loading of the example.mesh file.

A screenshot of the result can be found in example.jpg file.

The program getIntersect can load data from both example.mesh and example.sol
and compute the intersection of these two domains in an output *.sol file.

////////////////////////////////////////////////////////////////////////////////
Strategy to follow if one aims to computes the intersection (or reunion or
symmetric difference) of two domains given as two *.mesh files.

For example, in our example, we have two different *.mesh files representing
the two domains we were taking about previously, and entitled:

valence1.mesh valence2.mesh

We assume to get on valence1.mesh the intersection of the two domains contained
in both files.

Step 1: generate the level-set function of the domain in valence2.mesh
        This can be done with the help of mshdist software (with -dom option)
        (https://github.com/ISCDtoolbox/Mshdist). A version of mshdist is also
        provided with the mpdToolbox (MPD/sources/mshdist)

./mshdist valence2.mesh -dom -ncpu 1

        This prompt command will generate a valence2.sol file containing
        the level-set function associated with the domain in valence2.mesh file.

Step 2: project the valence2.sol values on the vertices of valence1.mesh file.
        This can be done with the help of the mshint software
        https://github.com/ISCDtoolbox/AdaptTools/tree/olderVersionThatWorks).
        A version of mshint is also provided with the mpdToolbox
        (MPD/sources/mpdToolbox/mshint)

./mshint valence2.mesh valence1.mesh

        This prompt command will generate a valence1.sol file containing
        the level-set function associated with the domain in valence2.mesh file
        (but now projected on the mesh vertices given in valence1.mesh file).

Step 3: get the intersection of the domain of valence1.mesh with the one
        inside valence1.sol (containing the one of valence2.mesh). This is
        performed with the getIntersection program.

./getIntersection valence1.mesh valence12.sol

        This prompt command will generate a valence12.sol file containing
        the level-set function associated with the intersection of the two
        domains computed at the vertices of valence1.mesh file.

Step 4: renormalize the level-set function to avoid numerical errors
        at interpolation. This can be done with the help of (another option) of
        the mshdist software (https://github.com/ISCDtoolbox/Mshdist). A version
        of mshdist is also provided with the mpdToolbox (MPD/sources/mshdist).

cp valence1.mesh valence12.mesh

./mshdist valence12.mesh -ncpu 1

        This prompt command will generate a valence12.sol file containing
        the normalized level-set function associated with the intersection of
        the two domains computed at the vertices of valence12.mesh file.

Step 5: interpolate the intersection on the mesh from the level-set function
        and adapt the mesh to the geometry and the water molecule H2O.
        This can be performed thanks to the mmg3d software
        (https://github.com/dalphin5/mmg/tree/myFeature).
        Such version of mmm3d is also provided with the mpdToolbox
        (MPD/sources/mmg3d).

        Remark: this version of mmg3d in not the official one
                (https://github.com/MmgTools/mmg) but a modified version that
                allows to adapt a mesh to both geometry contained in the
                level-set function, but also by respecting a given metric
                outside the boundary points (that must be saved in a file
                metric.sol, the name is important here).

./mmg3d -in valence12.mesh -ls -sol valence12.sol -out valence12.mesh -nr -hmin 0.05 -hmax 1.0 -hausd 0.01 -hgrad 2.0

        This prompt command will generate a new valence12.mesh file containing
        the intersection of the two domains as a subpart of the mesh.
        In addition, the new valence12.mesh file is such that the metric given
        in metric.sol is respected as well as the geometry of the intersection
        of the two domains (the resulting metric in saved in the valence12.sol
        file).

A screenshot of the result can be found in the valence12.jpg file.

Remark: by compiling getIntersection with LS_OPERATION set to 0 or 2, the
        smae procedure allows to get the reunion or the symmetric difference
        of two domains given as subparts of two *.mesh files.

