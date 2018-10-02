#ifndef DEF_INITIALIZE_MESH
#define DEF_INITIALIZE_MESH

/**
* \file loadMesh.h
* \brief Prototypes related to the initial loading (or creation) of an initial
*        mesh in the MPD algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* This file contains the description of the all non-static function prototypes
* that are used to initially load the mesh of the MPD algorithm, either from a
* *.mesh/ *.cube file, or from the default/specified parameters if the file does
* not exists.
*/

#include "main.h"

/* ************************************************************************** */
// Prototypes of non-static functions
/* ************************************************************************** */
/**
* \fn void initializeMeshStructure(Mesh* pMesh)
* \brief It initializes to zero the Mesh structure pointed by pMesh.
*
* \param[in] pMesh A pointer that points to the Mesh structure (defined in
*                  main.h) of the \ref main function.
*
* The function \ref initializeMeshStructure sets to zero all the variables
* of the structure pointed by pMesh (and pointers are set to NULL). This
* function does not return any value (void output).
*/
void initializeMeshStructure(Mesh* pMesh);

/**
* \fn void freeMeshMemory(Mesh* pMesh)
* \brief It frees the memory dynamically allocated for the Mesh structure
*        pointed by pMesh.
*
* \param[in] pMesh A pointer that points to the Mesh structure (defined in
*                  main.h file) of the \ref main function.
*
* The function \ref freeMeshMemory frees the memory dynamically allocated with
* calloc/malloc/realloc for the Mesh structure pointed by pMesh. All pointers of
* the Mesh structure that have been freed up are also set to NULL (but other
* variables than pointers are not reset to zero). This function does not return
* any value (void output).
*/
void freeMeshMemory(Mesh* pMesh);

/**
* \fn int initializeCubeDiscretization(Parameters* pParameters, Mesh* pMesh)
* \brief It suggests for approval to the user the mesh discretization of a
*        default initial computational box that will be stored in the structure
*        pointed by pMesh and according to the parameters prescribed in the
*        structure pointed by pParameters. It also evaluates the memory that
*        will be needed for storing such a discretization.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*                        Depending on its opt_mode variable (<=0/>0), its n_x,
*                        n_y and n_z variables are used to estimate the number
*                        of vertices, elements (hexahedra/tetrahdra), and
*                        boundary ones (quadrilaterals/triangles) that are
*                        intended to be stored in the structure pointed by pMesh
*                        in the case the user approve such a discretization
*                        of the cube associated with the initial default
*                        computational box. In addition, if the opt_mode
*                        variable structure pointed by pMesh is positive, some
*                        geometrical quantities are also computed, namely the
*                        number of corners, edges, normal and tangents vectors
*                        related to some boundary elements. The n_x, n_y and
*                        n_z variables of the structure pointed by pParameters
*                        must be (strictly) greater than two in order to have
*                        at least one normal/tangent vectors on the cube faces
*                        associated with the initial default computational box.
*                        Otherwise, an error is returned by the \ref
*                        initializeCubeDiscretization function.
*
* \param[out] pMesh A pointer that points to the Mesh structure (defined in
*                   main.h file) of the \ref main function, that is intended to
*                   be filled with some default parameters related to the mesh
*                   discretization of an initial default comptutational box.
*
* \return It returns one if the user approves the suggested discretization and
*         if the computed parameters are succesffully loaded in the structure
*         pointed by pMesh. It returns minus one if the user refuses to
*         discretize the cube and the mpd program will be next be left.
*         Otherwise, zero is returned if any error is encountered during the
*         process.
*
* The \ref initializeCubeDiscretization function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int initializeCubeDiscretization(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int allocateInitialMeshMemory(Parameters* pParameters, Mesh* pMesh)
* \brief It dynamically allocates memory for the structure pointed by pMesh
*        thanks to the values already loaded in the Mesh structure by the \ref
*        initializeCubeDiscretization function.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*                        Depending on its opt_mode variable (<=0/>0), the number
*                        of elements (hexahedra/tetrahdra), and boundary ones
*                        (quadrilaterals/triangles) of the structure pointed by
*                        pMesh are differently allocated.
*
* \param[in] pMesh A pointer that points to the Mesh structure (defined in
*                   main.h file) of the \ref main function, for which some
*                   memory is intended to be allocated by the \ref
*                   allocateInitialMeshMemory function.
*
* \return It returns one if the memory has been successfully allocated for the
*         structure pointed by pMesh. Otherwise, zero is returned if any error
*         is encountered during the process.
*
* The \ref allocateInitialMeshMemory function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int allocateInitialMeshMemory(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int labelPoint(int i, int j, int k, int nX, int nY, int nZ)
* \brief It is used to compute the label associated with the vertices of the
*        default initial computational box.
*
* \param[in] i The index corresponding to the first-coordinate position of the
*              vertex whose label is intended to be computed. To be more
*              precise, the first coordinate of the points associated with the
*              default initial computational box can be described thanks to the
*              variables of the Parameters structure x_min + i*delta_x for
*              i=0...nX-1. Before using the \ref labelPoint function, please be
*              sure that i lies between zero and nX-1 in order to avoid errors
*              on the vertex labels.
*
* \param[in] j The index corresponding to the second-coordinate position of the
*              vertex whose label is intended to be computed. To be more
*              precise, the second coordinate of the points associated with the
*              default initial computational box can be described thanks to the
*              variables of the Parameters structure y_min + j*delta_y for
*              j=0...nY-1. Before using the \ref labelPoint function, please be
*              sure that j lies between zero and nY-1 in order to avoid errors
*              on the vertex labels.
*
* \param[in] k The index corresponding to the third-coordinate position of the
*              vertex whose label is intended to be computed. To be more
*              precise, the third coordinate of the points associated with the
*              default initial computational box can be described thanks to the
*              variables of the Parameters structure z_min + k*delta_z for
*              k=0...nZ-1. Before using the \ref labelPoint function, please be
*              sure that k lies between zero and nZ-1 in order to avoid errors
*              on the vertex labels.
*
* \param[in] nX The integer corresponding to the (strict) upper bound for the
*               index i. It is intended to store the n_x variables of the
*               Parameters structure. Before using the \ref labelPoint function,
*               please be sure that nX is strictly greater than two in order to
*               avoid errors on the vertex labels.
*
* \param[in] nY The integer corresponding to the (strict) upper bound for the
*               index j. It is intended to store the n_y variables of the
*               Parameters structure. Before using the \ref labelPoint function,
*               please be sure that nY is strictly greater than two in order to
*               avoid errors on the vertex labels.
*
* \param[in] nZ The integer corresponding to the (strict) upper bound for the
*               index k. It is intended to store the n_z variables of the
*               Parameters structure. Before using the \ref labelPoint function,
*               please be sure that nZ is strictly greater than two in order to
*               avoid errors on the vertex labels.
*
* \return It returns the label of the vertex considered. The convention we use
*         for the vertex labels follows the one given for the cube faces. To be
*         more precise, the points that belong to a boundary face of the cube
*         associated with the default initial computational box are labelled
*         according to their face labels. The \ref labelPoint function can thus *         return 1, 2, 3, 4, 5, or 6 in this case. If the point belongs to an
*         egde of the cube, its label follows the rule 10*(label of face 1) +
*         (label of face 2), where we have (label of face 1)<(label of face 2).
*         The \ref labelPoint function can thus return 12, 14, 15, 16, 23, 25,
*         26, 34, 35, 36, 45, or 46 in that case. Finally, if a point is one of
*         the eight corners of the cube, its label follows similarly the rule
*         100*(label of face 1) + 10*(label of face 2) + (label of face 3),
*         where we have (label of face 1)<(label of face 2)<(label of face 3).
*         The \ref labelPoint function can thus return 125, 126, 145, 146, 235,
*         236, 345, or 346 in these situations. In any other situations,
*         provided the input variables have correct values, the \ref labelPoint
*         function returns the label zero.
*
* The \ref labelPoint function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int labelPoint(int i, int j, int k, int nX, int nY, int nZ);

/**
* \fn int meshCube(Parameters* pParameters, Mesh* pMesh)
* \brief According to the parameters stored in the structure pointed by
*        pParameters, it discretizes the cube associated with the corresponding
*        initial default computational box into a mesh which is stored in the
*        structure pointed by pMesh.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*                        Depending on its opt_mode variable (<=0/>0), the number
*                        of elements (hexahedra/tetrahdra), and boundary ones
*                        (quadrilaterals/triangles) of the structure pointed by
*                        pMesh yield to different mesh discretizations of the
*                        initial default computational box.
*
* \param[out] pMesh A pointer that points to the Mesh structure (defined in
*                   main.h file) of the \ref main function, for which some
*                   memory has recently been allocated by the \ref
*                   allocateInitialMeshMemory function, and which is intended
*                   to be filled with the values prescribed by the default
*                   initial mesh discretization of the computational box.
*
* \return It returns one if the structure pointed by pMesh has been successfully
*         filled with the values of the initial mesh discretization. Otherwise,
*         zero is returned if any error is encountered during the process.
*
* The \ref meshCube function should be static but has been defined as non-static
* in order to perform unit-test on it.
*/
int meshCube(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int writingMeshFile(Parameters* pParameters, Mesh* pMesh)
* \brief It uses the data stored in the structures pointed by pParameters,
*        and pMesh in order to save the mesh discretization in the *.mesh
*        format.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        name_mesh variable is expected to store the name of an
*                        *.mesh file which is intended to be written (warning
*                        here: it will reset and overwrite the file if it
*                        already exists). If it is set to NULL, a default name
*                        is generated by changing the *.info extension given in
*                        the name_info variable of the structure pointed by
*                        pParameters into the *.mesh one. If it stores the name
*                        of an *.cube file, the same name is used by changing
*                        the *.cube extension into the *.mesh one.
*
* \param[in] pMesh A pointer that points to the Mesh structure (defined in
*                  main.h file) of the \ref main function. If the opt_mode
*                  variable of the structure pointed by pParameters is not
*                  positive, then the mesh is made of hexahedra , otherwise it
*                  is made of tetrahedra.
*
* \return It returns one if the *.mesh file has been successfully written
*         according to the data stored in the structures pointed by pParameters,
*         and pMesh. In any other situations, an error is displayed in the
*         standard error stream and zero is returned by the \ref writingMeshFile
*         function.
*/
int writingMeshFile(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int getMeshFormat(char* fileLocation, int nameLength)
* \brief It determines if the file located at fileLocation exists and if its
*        name ends by the *.mesh or *.cube extension, with a length (strictly)
*        lower than the nameLength variable.
*
* \param[in] fileLocation A pointer that points to a string that is intended to
*                         store the path name of an existing *.mesh or *.cube
*                         (mesh) file. Its length must not be (strictly) greater
*                         than the nameLength variable (including the
*                         terminating nul character), and lower than six
*                         (to be able to store at least something more than the
*                         *.mesh or *.cube extension). Otherwise, an error is
*                         returned by the \ref getMeshFormat function.
*
* \param[in] nameLength A positive integer representing the maximal length
*                       allowed for the name pointed by the fileLocation
*                       variable (including the terminating nul character). It
*                       must be (strictly) greater than six, otherwise an error
*                       is returned by the \ref getMeshFormat function.
*
* \return It returns one (respectively minus one) if the location pointed by
*         fileLocation corresponds to the name of an existing *.mesh (resp.
*         *.cube) file. In any other situation (i.e. if the input variables do
*         not have the expected content), an error is displayed in the standard
*         error stream and zero is returned by the \ref getMeshFormat
*         function.
*
* The \ref getMeshFormat function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int getMeshFormat(char* fileLocation, int nameLength);

/**
* \fn int readMeshFileAndAllocateMesh(Parameters* pParameters, Mesh* pMesh)
* \brief It loads the values given in a *.mesh file whose path name is stored in
*        the name_mesh variable of the structure pointed by pParameters, and
*        stores them in the variables of the structure pointed by pMesh.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        name_mesh variable is expected to store the name of an
*                        existing *.mesh  file whose associated values are
*                        intended to be loaded and stored in the structure
*                        pointed by pMesh.
*
* \param[out] pMesh A pointer that points to the Mesh structure (defined in
*                   main.h file) of the \ref main function, that is intended to
*                   be filled by the mesh discretization given in the *.mesh file.
*
* \return It returns one if the structure pointed by pMesh has been successfully
*         filled with the values given in the *.mesh file. Otherwise, zero is
*         returned if any error is encountered during the process.

* The function \ref readMeshFileAndAllocateMesh looks at the
* pParameters->name_mesh variable of the structure pointed by pParameters in
* order to successively open the corresponding *.mesh file, which must exists,
* check that it follows the *.mesh format syntax, dynamically allocate some
* memory for the structure pointed by pMesh, and stores in it the mesh data
* contained in the *.mesh file, updating some variables of the structure
* pointed by pParameters and related to the mesh discretization and its bounding
* computational box values.
*/
int readMeshFileAndAllocateMesh(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int updateDiscretizationParameters(Parameters* pParameters, Mesh* pMesh)
* \brief It looks at the mesh discretization which is assumed to have been
*        stored in the structure pointed by pMesh, and update the discretization
*        parameters related the bounding computational box accordingly by
*        modifying the corresponding values stored in the structure pointed by
*        pParameters.
*
* \param[out] pParameters A pointer that points to the Parameters structure
*                         (defined in main.h file) of the \ref main function,
*                         whose values are intended to be updated according to
*                         the mesh discretization stored in the structure
*                         pointed by pMesh. It must not point to NULL otherwise
*                         an error is returned by the \ref
*                         updateDiscretizationParameters function.
*
* \param[in] pMesh A pointer that points to the Mesh structure (defined in
*                  main.h file) of the \ref main function, that is expected to
*                  store an already loaded mesh discretization. Otherwise, an
*                  error is returned by the \ref updateDiscretizationParameters
*                  function.
*
* \return It returns one if the discretization variables of the structure
*         pointed by pParameters have been successfully updates according to
*         the mesh discretization stored in the structure pointed by pMesh.
*         Otherwise, zero is returned if any error is encountered during the
*         process.

* The function \ref updateDiscretizationParameters looks at the mesh
* dicretization recently stored in the structure pointed by pMesh and first
* update the x_min, y_min,  z_min, x_max, y_max, and z_max variables of the
* structure pointed by pParameters, which corresponds to the extremal values
* bounding the computational box. In addition, if the opt_mode variable of the
* structure pointed by pParameters is not positive, the mesh has an hexahedral
* structure and the function \ref updateDiscretizationParameters update the
* discretization parameters stored in the delta_x, delta_y, delta_z, n_x, n_y
* and n_z variables of the structure pointed by pParameters. The \ref
* updateDiscretizationParameters function should be static but has been defined
* as non-static in order to perform unit-test on it.
*/
int updateDiscretizationParameters(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int checkCompatibilityOfChemicalData(char* fileLocation, int nameLength,
*                                                     Nucleus* pNucl, int nNucl)
* \brief It compares the Nucleus data stored in the array (of size nNucl)
*        pointed by pNUcl with the one contained in the *.chem file whose name
*        (of length (strictly) less than nameLength) is stored in the
*        fileLocation variables.
*
* \param[in] fileLocation A pointer that points to a string that is intended to
*                         store the path name of an existing *.chem (chemical)
*                         file. Its length must not be (strictly) greater than
*                         the nameLength variable (including the terminating nul
*                         character), and lower than six (to be able to store
*                         at least something more than the *.chem extension).
*                         Otherwise, an error is returned by the \ref
*                         checkCompatibilityOfChemicalData function.
*
* \param[in] nameLength A positive integer representing the maximal length
*                       allowed for the name pointed by the fileLocation
*                       variable (including the terminating nul character). It
*                       must be (strictly) greater than six, otherwise an error
*                       is returned by the \ref checkCompatibilityOfChemicalData
*                       function.
*
* \param[in] pNucl A pointer that is intended to point to an array of Nucleus
*                  structures that needs to be compared to the one written in
*                  *.chem file stored in the fileLocation variable. Its size is
*                  stored in the nNucl variable.
*
* \param[in] nNucl A positive integer representing the size of the array pointed
*                  by pNucl.
*
* \return It returns one if the Nucleus data stored in the array pointed by
*         pNucl corresponds to the ones written in the *.chem file pointed by
*         the fileLocation variable. In any other situation (i.e. if the input
*         variables do not have the expected content), an error is displayed in
*         the standard error stream and zero is returned by the \ref
*         checkCompatibilityOfChemicalData function.
*
* The \ref checkCompatibilityOfChemicalData function should be static but has
* been defined as non-static in order to perform unit-test on it.
*/
int checkCompatibilityOfChemicalData(char* fileLocation, int nameLength,
                                                     Nucleus* pNucl, int nNucl);

/**
* \fn int readCubeFileAndAllocateMesh(Parameters* pParameters, Mesh* pMesh)
* \brief It loads the values given in a *.cube file whose path name is stored in
*        the name_mesh variable of the structure pointed by pParameters, update
*        the dicretization parameters accordingly, then build and save the
*        corresponding mesh in the structure pointed by pMesh.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        name_mesh variable is expected to store the name of an
*                        existing *.cube file whose associated values are
*                        intended to be loaded and used to build a mesh finallly
*                        stored in the structure pointed by pMesh. Its variables
*                        related to the discretization parameters will also be
*                        updated accordingly.
*
* \param[out] pMesh A pointer that points to the Mesh structure (defined in
*                   main.h file) of the \ref main function, that is intended to
*                   be filled by the mesh discretization constructed from the
*                   *.cube file.
*
* \return It returns one if the structure pointed by pMesh has been successfully
*         filled with the values constructed from the *.cube file. Otherwise, zero
*         is returned if any error is encountered during the process.
*
*/
int readCubeFileAndAllocateMesh(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int loadMesh(Parameters* pParameters, Mesh* pMesh)
* \brief It loads the values given in a *.mesh or *.cube (mesh) file pointed
*        by the name_mesh variable of the structure pointed by pParameters, and
*        stores them in the variables of the structure pointed by pMesh. A
*        default structure is built if no mesh file has been specified,
*        according to the parameters stored in structure pointed by pParameters
*        and an approval of the user is needed to perform the suggested
*        discretization.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        name_mesh variable is expected to store the name of an
*                        existing *.mesh or *.cube file whose associated values
*                        are intended to be loaded and stored in the structure
*                        pointed by pMesh. If no such file has been prescribed,
*                        the name_mesh variable is set to NULL and a default
*                        mesh is meant to be generated according to the other
*                        variables of the Parameters structure. The user has to
*                        approve the suggested discretization for performing it.
*
* \param[out] pMesh A pointer that points to the Mesh structure (defined in
*                   main.h file) of the \ref main function, that is intended to
*                   be filled either by a default mesh or by the one given in
*                   the *.mesh/ *.cube file.
*
* \return It returns one if the structure pointed by pMesh has been successfully
*         initialized to its prescribed values. It returns minus one, if the
*         suggested discretization is not approved by the user and the mpd
*         is left but the default values of the discretization will be
*         nevertheless saved in a default *.cube file. Otherwise, zero is
*         returned if any error is encountered during the process.

* The function \ref loadMesh looks at the pParameters->name_mesh variable. If it
* is set to NULL, depending on the pParamters->opt_mode (<=0/>0), we generate a
* default (hexahedral/tetrahedral) mesh which is stored in the structure
* pointed by pMesh, after approval by the user, and saved in a *.mesh and *.cube
* file. In the case of a tetrahedral mesh i.e. if pParamters->opt_mode is
* positive, an preliminary isotropic mesh adaptation is also performed thanks to
* the mmg3d software, which thus must have been previously installed.
* Otherwise, the function \ref loadMesh simply loads the mesh data from the
* *.mesh/ *.cube file pointed by the pParameters->name_mesh variable, update
* some variables of the structure pointed by pParameters if necessary, and save
* the corresponding values in a *.mesh/ *.cube file. If the user does not
* approve the suggested default discretization, a *.cube file will be
* nevertheless created by the \ref main function in case of future use.
*/
int loadMesh(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int writingCubeFile(Parameters* pParameters,
*                                  ChemicalSystem* pChemicalSystem, Mesh* pMesh)
* \brief It uses the data stored in the structures pointed by pParameters,
*        pChemicalSystem, and pMesh in order to save the mesh discretization in
*        the *.cube format.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        name_mesh variable is expected to store the name of an
*                        *.cube file which is intended to be written (warning
*                        here: it will reset and overwrite the file if it
*                        already exists). If it is set to NULL, a default name
*                        is generated by changing the *.info extension given in
*                        the name_info variable of the structure pointed by
*                        pParameters into the *.cube one. If it stores the name
*                        of an *.mesh file, the same name is used by changing
*                        the *.mesh extension into the *.cube one.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure (defined in main.h file) of the \ref main
*                            function. Its Nucleus structures are used to write
*                            the *.cube file.
*
* \param[in] pMesh A pointer that points to the Mesh structure (defined in
*                   main.h file) of the \ref main function. If the opt_mode
*                   variable of the structure pointed by pParameters is not
*                   positive, then the mesh is made of hexahedra and their label
*                   are then used to write the *.cube file. Otherwise, zero is
*                   written for each corresponding hexahedron's label in the
*                   *.cube file.
*
* \return It returns one if the *.cube file has been successfully written
*         according to the data stored in the structures pointed by pParameters,
*         pChemicalSystem, and pMesh. In any other situations, an error is
*         displayed in the standard error stream and zero is returned by the
*         \ref writingCubeFile function.
*
* Remark: for the moment, it is not possible to write a *.cube file if the data
* contained in the structure pointed by pMesh is made of tetrahedra, i.e. if the
* opt_mode variable of the structure pointed by pParameters is positive. We
* intend to project a tetrahedral mesh into an hexahedral structure in a future
* version of the mpd program.
*/
int writingCubeFile(Parameters* pParameters, ChemicalSystem* pChemicalSystem,
                                                                   Mesh* pMesh);

#endif

