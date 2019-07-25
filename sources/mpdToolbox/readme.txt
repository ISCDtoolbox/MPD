Short description of the softwares in the mpdToolbox

See the readme.txt file of each program for further details.

////////////////////////////////////////////////////////////////////////////////
mshmet software computes the metric of a function computed at the
vertices of a *.mesh file (and associated *.sol file)

./mshmet *.mesh

////////////////////////////////////////////////////////////////////////////////
mshint software interpolates an *.sol file associated with a mesh
(first argument) at the vertices of another mesh (second argument)

./mshint *.mesh *.mesh

////////////////////////////////////////////////////////////////////////////////
buildBoundedDomain extracts the surface of a mesh (first argument),
close it if necessary, and save it in the *.obj format (second argument).
Compute also the volume of the domain obtained

./buildBoundedDomain *.mesh *.obj

////////////////////////////////////////////////////////////////////////////////
buildLevelSetOfHalfSpace computes the level-set function of an half-space in
the x-, y- or z-direction at the vertices of a mesh (first argument) and save
the values in a *.sol file (second argument).

./buildLevelSetOfHalfSpace *.mesh *.sol

////////////////////////////////////////////////////////////////////////////////
cubeToMesh converts the grid data contained in a *.cube file (first argument)
and generates an hexahedral mesh saved in the *.mesh format (second argument).

./cubeToMesh *.cube *.mesh

////////////////////////////////////////////////////////////////////////////////
doubleSphere computes the level-set function of the reunion, intersection,
or symmetric difference of two spheres computed at the vertices of a mesh
(first argument) and saved in an *.sol file (second argument).

./doubleSphere *.mesh *.sol

////////////////////////////////////////////////////////////////////////////////
getIntersection loads the mesh (first argument) and an associated level-set
function (*.sol file associated with the first argument). Then, it can
take the intersection or the reunion or the symmetric difference of this
level-set function with the internal domain encoded in the mesh. The resulting
level-set function is saved in a *.sol file (second argument).

./getIntersection *.ini.mesh *.out.sol

Remark: with mshdist and mshint it is possible to compute the intersection
       (or reunion or symmetric difference) of two domains given as two *.mesh
       files.

////////////////////////////////////////////////////////////////////////////////
hexaToTetra converts a *.mesh file (first argument) containing a 3D-hexahedral
mesh into a tetrahedral one saved in a *.mesh file (second argument).

./hexaToTetra *.mesh *.mesh

////////////////////////////////////////////////////////////////////////////////
labelInverter inverts the labels of the elements (tetrahedra/hexahedra) of a
*.mesh file (first argument), exchanging the internal domain label 3)
with the exterior (label 2). The result is saved in a *.mesh file (second
argument).

./labelInverter *.mesh *.mesh

////////////////////////////////////////////////////////////////////////////////
meshToCube loads the hexahedra mesh of a *.mesh file (first argument) and
the nuclei of a *.wfn file (second argument) in order to write the same mesh
in the *.cube format (third argument).

./meshToCube *.mesh *.wfn *.cube

Remark: with mshdist, mshint, cubeToMesh, and hexaToTetra, it is possible to
        compute the project a tetrahedral domain on a grid. Hence, it is
        possible to convert any *.mesh file in this way into an *.cube file.

////////////////////////////////////////////////////////////////////////////////
meshToObj extracts the surface boundary of an internal domain contained in the
          *.mesh file (first argument) and save the data in the *.obj format
          (second argument).

./meshToObj *.mesh *.obj

////////////////////////////////////////////////////////////////////////////////
objMerge merges the n first *.obj files, n>1, into a common *.obj file (last
argument).

./objMerge *1.obj ... *n.obj *(n+1).obj

////////////////////////////////////////////////////////////////////////////////
refineCubeMesh load the hexahedral mesh of a *.mesh file (first argument)
and refine by two the discretization step, saving the resulting mesh in a
*.mesh file (second argument).

./refineCubeMesh *.mesh *.mesh

////////////////////////////////////////////////////////////////////////////////
resizeObjFile converts the coordinates of a *.obj file (first argument) from
Angstroms into Bohrs (or conversely). The resulting mesh is saved in a *.obj
file (second argument).

./resizeObjFile *.obj *.obj

////////////////////////////////////////////////////////////////////////////////
shapeExtractor extracts the mesh of the domain contained in a *.mesh file
(first argument) and save the resulting mesh in a *.mesh file (second argument).

./shapeExtractor *.mesh *.mesh

////////////////////////////////////////////////////////////////////////////////
vectorFieldNormAtVertices loads the 3D-vectorial values of *.sol file (first
argument) and saves the norm of this vector field into a *.sol file (second
argument).

./vectorFieldNormAtVertices *.sol *.sol

