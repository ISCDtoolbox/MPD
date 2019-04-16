#ifndef DEF_ADAPT_MESH
#define DEF_ADAPT_MESH

/**
* \file adaptMesh.h
* \brief Macros and prototypes related to the mesh adaption process in the MPD
*        algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* This file contains the description of all the preprocessor constants and
* non-static function prototypes that are used to properly adapt and improve
* the mesh in the MPD algorithm.
*/

#include "main.h"

/**
* \def MET_CST
* \brief Dimensional constant used for the metric computation.
*
* It must be equal to 2/9 in 2d, 9/32 in 3d, and more generally to
* [d/(d+1)]^2/2 in any dimension d>=2.
*/
#define MET_CST 0.28125

/**
* \fn int initialDomainInMeshExists(Parameters* pParameters, Mesh* pMesh)
* \brief It checks if the structure pointed by pMesh contains an internal
*        domain via the labels of its faces (then of its elements in the case
*        of hexahedra i.e. if the pParameters->opt_mode variable is not
*        positive).
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its opt_mode variable indicates
*                        if the mesh is tetrahedral (positive values) or
*                        hexahedral (non-positive values) and allows to treat 
*                        the mesh faces as triangles or quadrilaterals.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function. If it contains an internal domain, some of its
*                  faces should be labelled ten. In such a situation, the
*                  elements that belongs to the domain are labelled three,
*                  otherwise two. 
*
* \return It returns one (respectively minus one) if the mesh has an (resp. no)
*         internal domain i.e. if some faces are (resp. not) labelled 10 in the
*         structure pointed by pMesh. In the case of hexahedral mesh i.e. if the
*         pParameters->opt_mode variable is not positive, an additional test is
*         made on the labels of hexahedra (labelled 3 and 2) to see if an
*         internal domain is there but its boundaries have not been set up. In
*         such a situation, the \ref initialDomainInMeshExists function
*         returned two instead of one. This typically happens when an initial
*         *.cube file is loaded by the mpd program. Otherwise, zero is returned
*         by the \ref initialDomainInMeshExists function if an error is
*         encountered during the process.
*/
int initialDomainInMeshExists(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int initializeAdjacency(Parameters* pParameters, Mesh* pMesh)
* \brief It sets up the Adjacency structures associated with the boundary
*        quadrilaterals enclosing the internal domain. We assume here that the
*        structure pointed by pMesh is made of hexahedra (i.e. the
*        pParameters->opt_mode variable is not positive) and that such an
*        internal domain already exists inside the computational box.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its opt_mode variable must be
*                        a non-positive integer i.e. the underlying mesh must be
*                        made of hexahedra in order to set up the \ref Adjacency
*                        structures. Otherwise, an error is returned by the \ref
*                        initializeAdjacency function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function. It is intended to store a uniform grid
*                      mesh where some quadrilaterals (labelled 10) have been
*                      added for specifying the boundary of an internal domain.
*                      Similarly, the hexahedra of the mesh must have been
*                      correctly labelled (3 for those belonging to the internal
*                      domain, 2 for those that are not). Otherwise, an error is
*                      returned by the \ref initializeAdjacency function.
*
* \return It returns one if the \ref Adjacency structures associated with the
*         boundary quadrilaterals enclosing the internal domain have been
*         succesfully set up. Otherwise, zero is returned by the \ref
*         initializeAdjacency function if an error is encountered during the
*         process.
*
* The hexahedral mesh pointed by the pMesh variable may come from previous
* calculations and in particular, the hexahedron's labels may have been set to
* 2 or 3. Similarly, the labels of boundary quadrilaterals enclosing the
* internal domain should be set to 10. The \ref initializeAdjacency function
* should be static but has been defined as non-static in order to perform
* unit-tests on it.
*/
int initializeAdjacency(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int initializeLevelSetFunction(Parameters* pParameters, Mesh* pMesh)
* \brief It generates the signed distance function of an initial sphere or cube
*        depending on the values stored in the structure pointed by pParameters
*        and it updates the labels in the structure pointed by pMesh accordingly
*        (vertices for a tetrahedral mesh and hexahedra for an hexahedral one).
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. The initial internal domain is
*                        ruled by its ls_type, ls_x, ls_y, ls_z, and ls_r
*                        variables. If ls_type is set to 0, then the initial
*                        internal domain is a cube centered at (ls_x,ls_y,ls_z)
*                        and of length ls_r, which must be positive. Otherwise,
*                        ls_type must be set to one and the initial guess for
*                        the MPD domain is a sphere centered at (ls_x,ls_y,ls_z)
*                        and of radius ls_r, which again must be positive.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function. If the opt_mode variable of the structure
*                      pointed by pParameters is positive, the signed distance
*                      function of the initial guess for the MPD domain is
*                      computed at the vertices of the mesh. Otherwise, it is
*                      computed at the center of each hexahedron and depending
*                      on its sign, the hexahedron's label is set to 3
*                      (interior) or 2 (exterior).
*
* \return It returns one if the level-set function of the starting guess for the
*         the MPD domain has been succesfully initialized. Otherwise, zero is
*         returned by the \ref initializeLevelSetFunction function if an error
*         is encountered during the process.
*
* The \ref initializeLevelSetFunction function should be static but has been
* defined as non-static in order to perform unit-tests on it. Moreover, the
* function uses the square root mathematical function so in order to use this
* function, the math.h file must not be put in comment in the main.h file, and
* the -lm option must be set when compiling with gcc (or link properly the math
* library for other compilers and architectures).  
*/
int initializeLevelSetFunction(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int getLevelSetQuadrilaterals(Parameters* pParameters, Mesh* pMesh)
* \brief It modifies the pMesh hexahedral structure so that it contains the
*        quadrilaterals corresponding to the boundary of the internal MPD
*        domain and it updates the Adjacency structures accordingly.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its opt_mode variable must be
*                        a non-positive integer i.e. the underlying mesh must be
*                        made of hexahedra. Otherwise, an error is returned by
*                        the \ref getLevelSetQuadrilaterals function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function. It is intended to store a uniform grid
*                      mesh where some quadrilaterals corresponding to the
*                      internal domain are intended to be added and labelled 10.
*                      Similarly, the hexahedra of the mesh must have been
*                      previously labelled (3 for those belonging to the
*                      internal domain, 2 for those that are not).
*
* \return It returns one if the additional quadrilaterals have been added and
*         if the \ref Adjacency structures associated with the internal domain
*         have been succesfully updated. Otherwise, zero is returned by the \ref
*         getLevelSetQuadrilaterals function if an error is encountered during
*         the process.
*
* The hexahedral mesh pointed by the pMesh variable may come from previous
* calculations and in particular, the hexahedron's labels may have been set to
* (+/-)2 or (+/-)3. Hence, the \ref getLevelSetQuadrilaterals function will
* only consider the positive value corresponding to the labels (see the
* documentation on the trick_matrix variable of the Parameters structure for
* further details).
*/
int getLevelSetQuadrilaterals(Parameters* pParameters, Mesh* pMesh);

/**
* \fn double evaluatingHessianAtVertices(ChemicalSystem* pChemicalSystem,
*                                        MolecularOrbital* pMolecularOrbital,
*                                        Point* pPoint, double gradient[3],
*                                              double hessian[6], int primitive)
* \brief It evaluates the (Gaussian-type function) primitive of
*        pMolecularOrbital defined in pChemicalSystem at pPoint, and it also
*        calculates and stored the values of its gradient and hessian matrix.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. In particular,
*                            we need to access the three coordinates of the
*                            Nucleus structure related to pMolecularOrbital.
*                            For not slowing the process, we assume here that
*                            pChemicalSystem is not pointing to NULL, as well as
*                            its pmorb and pnucl variables.
*
* \param[in] pMolecularOrbital A pointer that points the one of the
*                              MolecularOrbital structure stored in the array
*                              dynamically allocated for the pmorb variable
*                              of the structure pointed by pChemicalSystem.
*                              For not slowing the process, we assume that
*                              pMolecularOrbital is not pointing to NULL, as
*                              well as its coeff, exp, nucl and type variables.
*
* \param[in] pPoint A pointer that points to the Point structure of one of the
*                   vertices associated with the Mesh Structure of the \ref main
*                   function. For not slowing the process, we assume that pPoint
*                   is not pointing to NULL.
*
* \param[out] gradient An three-dimensional array that is intented to store the
*                      gradient values associated with the Gaussian primitive.
*                      For not slowing the process, we assume that the array
*                      pointed by gradient has been initialized to zero before
*                      calling the \ref evaluatingHessianAtVertices function.
*
* \param[out] hessian An six-dimensional array that is intented to store the
*                     values of the Hessian matrix associated with the
*                     Gaussian primitive. Since the Hessian matrix is symmetric,
*                     we only need to store six values instead of nine and we
*                     use the convention: 0->xx; 1->yy; 2->zz; 3->xy,yx;
*                     4->xz,zx; 5->yz,zy. Moreover, for not slowing the
*                     process, we assume that the array pointed by hessian has
*                     been initialized to zero before calling the \ref
*                     evaluatingHessianAtVertices function.
*
* \param[in] primitive An integer referring to the primitive i.e. its position
*                      in the arrays associated with the coeff, exp, nucl, or
*                      type variables of the structure pointed by
*                      pMolecularOrbital. For not slowing the process, we assume
*                      that it is a non-negative integer (strictly) less than
*                      the total number of primitives in pChemicalSystem (stored
*                      in the pChemicalSystem->ngauss variable).
*
* \return It returns the value of the Gaussian-type function. For not slowing
*         the process, if the coefficient of the primitive is zero, the function
*         immediatly returns zero. For similar reasons, if the type of the
*         primitive is not between one and twenty, the type is treated by
*         default as an s-type (see \ref ORB_S for further details) and a
*         a warning is displayed in the standard output stream.
*
* The \ref evaluatingHessianAtVertices function should be static but has been
* defined as non-static in order to perform unit-tests on it. Moreover, the
* function uses the exponential mathematical function so in order to use this
* function, the math.h file must not be put in comment in the main.h file, and
* the -lm must be set when compiling with gcc (or link properly the math library
* for other compilers and architectures).  
*/
double evaluatingHessianAtVertices(ChemicalSystem* pChemicalSystem,
                                   MolecularOrbital* pMolecularOrbital,
                                   Point* pPoint, double gradient[3],
                                              double hessian[6], int primitive);

/**
* \fn int evaluatingMetricOnMesh(Parameters* pParameters, Mesh* pMesh,
*                                               ChemicalSystem* pChemicalSystem)
* \brief It calculates at the vertices of the tetrahedral structure pointed by
*        pMesh the Hessian-based metric associated with every product of
*        orbitals stored in the structure pointed by pChemicalSystem.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its opt_mode variable must be
*                        a positive integer i.e. the underlying mesh must be
*                        made of tetrahedra. Otherwise, an error is returned by
*                        the \ref evaluatingMetricOnMesh function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function. It is intended to store in the value
*                      variable of each Point structures the metric associated
*                      with every product of moelcular orbitals.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. In particular,
*                            if we are dealing with a restricted Hartree-Fock
*                            basis of molecular orbitals, then the computational
*                            time is reduced by not considering the molecular
*                            orbitals of negative spin.
*
* \return It returns one if metric was successfully evaluated at each vertices
*         of the mesh. Otherwise, zero is returned if an error is encountered
*         during the process.
*
* The \ref evaluatingMetricOnMesh function computes at each vertices of the mesh
* the exact analytical Hessian matrix associated with any product of two
* spin-compatible molecular orbitals (expressed as a sum of Gaussian-type
* primitives), which uses some standard functions of the math library (math.h).
* Each (three-dimensional) Hessian matrix is diagonalized by using the dsyev
* routine of lapacke library (LAPACK interface for c), which computes the
* eigenvalues (and optionally the eigenvectors) associated with a real
* symmetric matrix. In order to use this function, the math.h and lapacke.h
* file must not be put in comment in the main.h file. Furthermore, the -lm and
* -llapacke options must be set when compiling the program with gcc (or link
* correctly the math and lapacke library for other compilers and architectures).
*/
int evaluatingMetricOnMesh(Parameters* pParameters, Mesh* pMesh,
                                               ChemicalSystem* pChemicalSystem);

/**
* \fn int writingSolFile(Parameters* pParameters, Mesh* pMesh)
* \brief It writes the values stored in the structure by pMesh in an *.sol file
*        associated with an *.mesh file whose path name is stored in the
*        name_mesh variable of the structure pointed by pParameters.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable must
*                        refer to the name of an existing *.mesh file otherwise
*                        an error is returned by the \ref writingSolFile
*                        function. We also recall that the name of the *.sol
*                        file is built on the *.mesh file where the '.mesh'
*                        extension has been replaced by the '.sol' one. We also
*                        recall that if a *.sol file with a similar name already
*                        exists, it will be reset and overwritten by the \ref
*                        writingSolFile function.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function. In each Point structure belonging to the array
*                  pointed by the pver variable of pMesh, the value variable is
*                  intended to store what will be the content of the *.sol
*                  file.
*
* \return It returns one if the solutions at each vertex were successfully
*         written by using the *.sol format. Otherwise, zero is returned if any
*         error is encountered during the writing process.
*/
int writingSolFile(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int computeMetric(Parameters* pParameters, Mesh* pMesh,
*                       ChemicalSystem* pChemicalSystem, int iterationInTheLoop)
* \brief It evaluates the metric adapted to the molecular orbitals of the
*        structure pointed by pChemicalSystem and it saves the values at the
*        vertices in a file entitled 'metric.sol'.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        met_min, met_max, and met_err also rule the parameters
*                        related to the metric computation of the molecular
*                        orbitals in the case of case of tetrahedral meshes,
*                        i.e. if its opt_mode variable is positive.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure (defined in
*                      main.h file) of the \ref main function, that is intended
*                      to be adapted and prepared for the optimization loop.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It contains
*                            the description of the molecular orbitals as a sum
*                            of Gaussian-type primitives.
*
* \param[in] iterationInTheLoop An integer that refer to the number of steps
*                               that are already performed in the optimization
*                               loop. It is used to vizualize the metric
*                               according to the pParameters->save_print
*                               variable.
*
* \return It returns one if the metric associated with the structure pointed by
*         pMesh has been successfully computed and saved in a metric.sol file.
*         Otherwise, zero is returned if any error is encountered during the
*         process.
*
* The metric computation of the molecular orbitals requires to evaluate and
* diagonalize their Hessian matrix. For this purpose, we use the standard
* mathematical functions and the dsyev routine of lapacke library (LAPACK
* interface for c), which computes the eigenvalues (and optionally the
* eigenvectors) associated with a real symmetric matrix. In order to use these
* function, the math.h and lapacke.h file must not be put in comment in the
* main.h file. Furthermore, the -lm and -llapacke options must be set when
* compiling the program with gcc (or link correctly the math and lapacke library
* for other compilers and architectures).
*/
int computeMetric(Parameters* pParameters, Mesh* pMesh,
                       ChemicalSystem* pChemicalSystem, int iterationInTheLoop);

/**
* \fn int performLevelSetAdaptation(Parameters* pParameters, Mesh* pMesh,
*                                   ChemicalSystem* pChemicalSystem,
*                                                        int iterationInTheLoop)
* \brief It compute the molecular orbitals' metric and performs a mesh
*        adaptation according to the level-set function (that we assume to have
*        been previously computed and saved in a *.sol file associated with the
*        *.mesh one) but also respecting the orbitals' metric. It uses the
*        modified version of the external mmg3d software, that must have been
*        previously installed. Then, it loads the new mesh obtained in the
*        structure pointed by pMesh.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable is
*                        intended to store the name of a valid and existing
*                        *.mesh file in order to be launched by the external
*                        mmg3d sofware which must have been previously
*                        installed, and whose path name must be store in the
*                        path_mmg3d variable of the structure pointed by
*                        pParameters. Otherwise, an error is returned by the
*                        \ref performLevelSetAdaptation function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure (defined in
*                      main.h file) of the \ref main function, that is intended
*                      to be adapted according to both orbitals' metric and
*                      the one associated with the level-set function that
*                      we assume to have been previously saved in a *.sol file
*                      associated with the *.mesh one.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It contains
*                            the description of the molecular orbitals as a sum
*                            of Gaussian-type primitives.
*
* \param[in] iterationInTheLoop An integer that refer to the number of steps
*                               that are already performed in the optimization
*                               loop. It is used to vizualize the adapted mesh
*                               according to the pParameters->save_print
*                               variable.
*
* \return It returns one if the *.mesh file stored in the name_mesh variable of
*         the structure pointed by pParameters has been successfully adapted
*         by the external mmg3d sofware and loaded in the structure pointed by
*         pMesh. In any other situation (i.e. if the input variables do not
*         have the expected content), an error is displayed in the standard
*         error stream and zero is returned by the \ref
*         performLevelSetAdaptation function.
*
* The \ref performLevelSetAdaptation function uses the \ref
* adaptMeshWithMmg3dSoftware function that itself executes the mmg3d sofware
* thank to the standard system c-function. In particular, the mmg3d sofware
* must have been previously installed at a location that must be either given
* by the default \ref PATH_MMG3D preprocessor constant, or by a valid path name
* specified in the input *.info file (preceded by the path_mmg3d keyword).
* Similarly, the mesh to be read must be an existing file written in the *.mesh
* format and stored in the name_mesh variable of the structure pointed by
* pParameters and the corresponding *.sol file must have a similar name
* (replacing the *.mesh extension by the *.sol one).
*/
int performLevelSetAdaptation(Parameters* pParameters, Mesh* pMesh,
                              ChemicalSystem* pChemicalSystem,
                                                        int iterationInTheLoop);

/**
* \fn int adaptMesh(Parameters* pParameters, Mesh* pMesh,
*                                               ChemicalSystem* pChemicalSystem)
* \brief It prepares the mesh for the optimization loop. If the pMesh variable
*        already contains an initial guess for the MPD domain, nothing is done.
*        Otherwise, an initial sphere of cube is built thanks to the structure
*        pointed by pParameters. Moreover, in the case of tetrahedral meshes,
*        it adapts the mesh to the chemistry of the molecular orbitals and also
*        to the geometry of the level-set function interface associated with the
*        initial domain.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        ls_type, ls_x, ls_y, ls_z, and ls_r variables rule the
*                        setting up of the initial domain if it is not already
*                        contained in the structure pointed by pMesh. Its
*                        met_min, met_max, and met_err also rule the parameters
*                        related to the metric computation of the molecular
*                        orbitals in the case of case of tetrahedral meshes,
*                        i.e. if its opt_mode variable is positive.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure (defined in
*                      main.h file) of the \ref main function, that is intended
*                      to be adapted and prepared for the optimization loop if
*                      it is necessary. More precisely, the \ref adaptMesh
*                      function looks at the structure pointed by pMesh and
*                      search for an internal domain (faces labelled 10, and
*                      elements labelled 3 inside the domain otherwise 2). If
*                      such a starting domain is found, nothing is done, apart
*                      from filling the Adjacency structures in the case of
*                      hexahedral meshes i.e. the opt_mode variable of the
*                      structure pointed by pParameters is not positive.
*                      Otherwise, an initial guess for the MPD domain is built
*                      according to the parameters stored in the structure
*                      pointed by pParameters. For hexahedra meshes, the
*                      Adjacency structures are updated; they will be used later
*                      in the optimization loop. In the case of tetrahedral
*                      meshes i.e. the opt_mode variable of the structure
*                      pointed by pParameters is positive, the mesh is adapted
*                      according to the metric associated with any product of
*                      two spin-compatible molecular orbitals stored in the
*                      structure pointed by pChemicalSystem, but also to the
*                      geometry of the level-set function interface associated
*                      with the initial domain.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It contains
*                            the description of the molecular orbitals as a sum
*                            of Gaussian-type primitives. In the case of
*                            tetrahedral meshes, the Hessian matrix associated
*                            any product of two spin-compatible molecular
*                            orbitals will be computed analytically and
*                            diagonalized in order to build properly a metric
*                            associated with chemistry of the system and reduce
*                            the error made on the computation of the overlap
*                            matrix performed later in the \ref optimization
*                            function.
*
* \return It returns one if the structure pointed by pMesh has been successfully
*         adapted and updated for performing the optimization loop. Otherwise,
*         zero is returned if any error is encountered during the process.
*
* The function \ref adaptMesh will use the external mmg3d software if some
* tetrahedral mesh adaptation needs to be performed. In this case, it must have
* been previously installed. Furthermore, the metric computation of the
* molecular orbitals requires to evaluate and diagonalize their Hessian matrix.
* For this purpose, we use the standard mathematical functions and the dsyev
* routine of lapacke library (LAPACK interface for c), which computes the
* eigenvalues (and optionally the eigenvectors) associated with a real
* symmetric matrix. In order to use these function, the math.h and lapacke.h
* file must not be put in comment in the main.h file. Furthermore, the -lm and
* -llapacke options must be set when compiling the program with gcc (or link
* correctly the math and lapacke library for other compilers and architectures).
*/
int adaptMesh(Parameters* pParameters, Mesh* pMesh,
                                               ChemicalSystem* pChemicalSystem);

#endif

