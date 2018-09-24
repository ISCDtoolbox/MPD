#ifndef DEF_LOAD_PARAMETERS
#define DEF_LOAD_PARAMETERS

/**
* \file loadParameters.h
* \brief Macros and prototypes related to the initial loading of parameters in
*        the MPD algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* This file contains the description of all the preprocessor constants and
* non-static function prototypes that are used to initially load the parameters
* of the MPD algorithm from an *.info (input) file.
*/

#include "main.h"

/* ************************************************************************** */
// Definitions of the different preprocessor constants that are used to
// initialize the default values of the structure Parameters (defined in
// main.h). These default values will be changed if others are specified in the
// *.info (input) file
/* ************************************************************************** */
/**
* \def OPT_MODE
* \brief Used to set the default value for the opt_mode variable of the
*        Parameters structure, which thus rules the default type of
*        optimization performed by the MPD algorithm.
*
* Positive values concern tetrahedral meshes whereas non-positive values concern
* hexahedral meshes. If set to -1/-2, shape-gradient/random perturbations are
* performed on hexahedral meshes in order to maximize the probability. If set
* to 0, the algorithm starts with level-set (global) perturbations, then changes
* to random (local) ones when the hexahedral domain no longer evolves. If set
* to 4, then each new tetrahedral domain represents the positive part of the
* shape gradient associated with the old one in the optimization loop. If set
* to 2/3, then an approach of Eulerian(level-set)/Lagrangian type is performed
* on tetrahedral meshes in order to maximize the probability, by following the
* shape gradient. If set to 1, then all the previous tetrahedral methods are
* combined in order to get a converged MPD domain. Other values are forbidden.
*/
#define OPT_MODE 0

/**
* \def VERBOSE
* \brief Used to set the default value for the verbose variable of the
*        Parameters structure, which thus rules the default type of
*        verbosity performed by the MPD algorithm.
*
* If set to 1, then it prints every execution details in the standard output
* stream; otherwise, it must be set to 0 and very little is shown during the
* execution of the MPD algorithm.
*/
#define VERBOSE 0

/**
* \def N_CPU
* \brief Used to set the default value for the n_cpu variable of the Parameters
*        structure, which thus rules the default number of cpu/threads used by
*        the MPD algorithm.
*
* It represents the number of cpu/threads required or the parallelization by
* openmp in the MPD program. It is also used by mshdist software, which must
* have been previously installed. We must have (int)\ref N_CPU > 0.
*/
#define N_CPU 1

/**
* \def NAME_LENGTH
* \brief Used to set the default value for the name_length variable of the
*        Parameters structure, which thus rules the default maximal length
*        allowed for storing the different file names.
*
* We must have (int)\ref NAME_LENGTH > 6. This condition ensures to store at
* least a name that contains something more than the *.info extension.
*/
#define NAME_LENGTH 101


// Related to the default Lamé's coefficients used to built a default *.elas
// file if needed (may be used only if opt_mode=1/2 in the structure Parameters)
/**
* \def LAME_INT1
* \brief Used to set the default value of Lamé's first coefficient associated
*        with the internal domain in which the probability is maximized.
*
* If the opt_mode variable of the Parameters structure is set to one or two,
* the MPD algorithm will need to launch the elastic software (which must have
* been previously installed in this case). In such a situation, an *.elas file
* is needed for the command line of the elastic software. Such a file can be
* prescribed directly in the *.info (input) file (thanks to the name_elas
* keyword) but if it is not the case, a default *.elas file is built by the MPD
* algorithm, which requires to specify at least Lamé's first and second
* coefficients for the internal domains (labelled three in the MPD algorithm
* and in which the probability is maximized) and for the external ones (labelled
* two in the MPD algorithm and which corresponds to the complement of the
* internal domains in the computational box). Although it will be used only if
* such a situation occurs, we must always have (double)\ref LAME_INT1 > 0.0.
*/
#define LAME_INT1 186000.0

/**
* \def LAME_INT2
* \brief Used to set the default value of Lamé's second coefficient associated
*        with the internal domains of the MPD algorithm.
*
* If the opt_mode variable of the Parameters structure is set to one or two,
* the MPD algorithm will need to launch the elastic software (which must have
* been previously installed in this case). In such a situation, an *.elas file
* is needed for the command line of the elastic software. Such a file can be
* prescribed directly in the *.info (input) file (thanks to the name_elas
* keyword) but if it is not the case, a default *.elas file is built by the MPD
* algorithm, which requires to specify at least Lamé's first and second
* coefficients for the internal domains (labelled three in the MPD algorithm
* and in which the probability is maximized) and for the external ones (labelled
* two in the MPD algorithm and which corresponds to the complement of the
* internal domains in the computational box). Although it will be used only if
* such a situation occurs, we must always have (double)\ref LAME_INT2 > 0.0.
*/
#define LAME_INT2 3400.0

/**
* \def LAME_EXT1
* \brief Used to set the default value of Lamé's first coefficient associated
*        with the external domains of the MPD algorithm.
*
* If the opt_mode variable of the Parameters structure is set to one or two,
* the MPD algorithm will need to launch the elastic software (which must have
* been previously installed in this case). In such a situation, an *.elas file
* is needed for the command line of the elastic software. Such a file can be
* prescribed directly in the *.info (input) file (thanks to the name_elas
* keyword) but if it is not the case, a default *.elas file is built by the MPD
* algorithm, which requires to specify at least Lamé's first and second
* coefficients for the internal domains (labelled three in the MPD algorithm
* and in which the probability is maximized) and for the external ones (labelled
* two in the MPD algorithm and which corresponds to the complement of the
* internal domains in the computational box). Although it will be used only if
* such a situation occurs, we must always have (double)\ref LAME_EXT1 > 0.0.
*/
#define LAME_EXT1 186000.0

/**
* \def LAME_EXT2
* \brief Used to set the default value of Lamé's second coefficient associated
*        with the external domains of the MPD algorithm.
*
* If the opt_mode variable of the Parameters structure is set to one or two,
* the MPD algorithm will need to launch the elastic software (which must have
* been previously installed in this case). In such a situation, an *.elas file
* is needed for the command line of the elastic software. Such a file can be
* prescribed directly in the *.info (input) file (thanks to the name_elas
* keyword) but if it is not the case, a default *.elas file is built by the MPD
* algorithm, which requires to specify at least Lamé's first and second
* coefficients for the internal domains (labelled three in the MPD algorithm
* and in which the probability is maximized) and for the external ones (labelled
* two in the MPD algorithm and which corresponds to the complement of the
* internal domains in the computational box). Although it will be used only if
* such a situation occurs, we must always have (double)\ref LAME_EXT2 > 0.0.
*/
#define LAME_EXT2 3400.0


// Related to the default computational box
/**
* \def X_MIN
* \brief Used to set the default value for the x_min variable of the Parameters
*        structure, which thus rules the default minimal coordinate in the
*        first-coordinate direction.
*
* We must have (double)\ref X_MIN < \ref X_MAX and \ref DELTA_X == (\ref X_MAX -
* \ref X_MIN)/(\ref N_X - 1) (or with at least a difference of less than 1e-16).
*/
#define X_MIN -3.5

/**
* \def Y_MIN
* \brief Used to set the default value for the y_min variable of the Parameters
*        structure, which thus rules the default minimal coordinate in the
*        second-coordinate direction.
*
* We must have (double)\ref Y_MIN < \ref Y_MAX and \ref DELTA_Y == (\ref Y_MAX -
* \ref Y_MIN)/(\ref N_Y - 1) (or with at least a difference of less than 1e-16).
*/
#define Y_MIN -3.5

/**
* \def Z_MIN
* \brief Used to set the default value for the z_min variable of the Parameters
*        structure, which thus rules the default minimal coordinate in the
*        third-coordinate direction.
*
* We must have (double)\ref Z_MIN < \ref Z_MAX and \ref DELTA_Z == (\ref Z_MAX -
* \ref Z_MIN)/(\ref N_Z - 1) (or with at least a difference of less than 1e-16).
*/
#define Z_MIN -3.5

/**
* \def X_MAX
* \brief Used to set the default value for the x_max variable of the Parameters
*        structure, which thus rules the default maximal coordinate in the
*        first-coordinate direction.
*
* We must have (double)\ref X_MAX > \ref X_MIN and \ref DELTA_X == (\ref X_MAX -
* \ref X_MIN)/(\ref N_X - 1) (or with at least a difference of less than 1e-16).
*/
#define X_MAX 3.5

/**
* \def Y_MAX
* \brief Used to set the default value for the y_max variable of the Parameters
*        structure, which thus rules the default maximal coordinate in the
*        second-coordinate direction.
*
* We must have (double)\ref Y_MAX > \ref Y_MIN and \ref DELTA_Y == (\ref Y_MAX -
* \ref Y_MIN)/(\ref N_Y - 1) (or with at least a difference of less than 1e-16).
*/
#define Y_MAX 3.5

/**
* \def Z_MAX
* \brief Used to set the default value for the z_max variable of the Parameters
*        structure, which thus rules the default maximal coordinate in the
*        third-coordinate direction.
*
* We must have (double)\ref Z_MAX > \ref Z_MIN and \ref DELTA_Z == (\ref Z_MAX -
* \ref Z_MIN)/(\ref N_Z - 1) (or with at least a difference of less than 1e-16).
*/
#define Z_MAX 3.5

// The condition N_XYZ > 2 allows at least one normal/tangent vector on the
// cube faces of the default computational box. Odd form should be preferred
// if we have XYZ_MIN=-XYZ_MAX
/**
* \def N_X
* \brief Used to set the default value for the n_x variable of the Parameters
*        structure, which thus rules the default number of points in the
*        first-coordinate direction.
*
* We must have (int)\ref N_X > 2 and \ref DELTA_X == (\ref X_MAX - \ref X_MIN)/
* (\ref N_X - 1) (or with at least a difference of less than 1e-16). The first
* condition allows the existence of at least one normal/tangent vector on the
* cube faces associated with the computational box, while in the second
* condition, odd form should be preferred for \ref N_X if we have
*\ref X_MIN == -\ref X_MAX.
*/
#define N_X 71

/**
* \def N_Y
* \brief Used to set the default value for the n_y variable of the Parameters
*        structure, which thus rules the default number of points in the
*        second-coordinate direction.
*
* We must have (int)\ref N_Y > 2 and \ref DELTA_Y == (\ref Y_MAX - \ref Y_MIN)/
* (\ref N_Y - 1) (or with at least a difference of less than 1e-16). The first
* condition allows the existence of at least one normal/tangent vector on the
* cube faces associated with the computational box, while in the second
* condition, odd form should be preferred for \ref N_Y if we
* have \ref Y_MIN == -\ref Y_MAX.
*/
#define N_Y 71

/**
* \def N_Z
* \brief Used to set the default value for the n_z variable of the Parameters
*        structure, which thus rules the default number of points in the
*        third-coordinate direction.
*
* We must have (int)\ref N_Z > 2 and \ref DELTA_Z == (\ref Z_MAX - \ref Z_MIN)/
* (\ref N_Z - 1) (or with at least a difference of less than 1e-16). The first
* condition allows the existence of at least one normal/tangent vector on the
* cube faces associated with the computational box, while in the second
* condition, odd form should be preferred for \ref N_Z if we
* have \ref Z_MIN == -\ref Z_MAX.
*/
#define N_Z 71

/**
* \def DELTA_X
* \brief Used to set the default value for the delta_x variable of the
*        Parameters structure, which thus rules the default discretization step
*        in the first-coordinate direction.
*
* We must have (double)\ref DELTA_X > 0.0 and (double)\ref DELTA_X == (\ref
* X_MAX - \ref X_MIN)/(\ref N_X - 1) (or with at least a difference of less than
* 1e-16).
*/
#define DELTA_X 0.1

/**
* \def DELTA_Y
* \brief Used to set the default value for the delta_y variable of the
*        Parameters structure, which thus rules the default discretization step
*        in the second-coordinate direction.
*
* We must have (double)\ref DELTA_Y > 0.0 and (double)\ref DELTA_Y == (\ref
* Y_MAX - \ref Y_MIN)/(\ref N_Y - 1) (or with at least a difference of less than
* 1e-16).
*/
#define DELTA_Y 0.1

/**
* \def DELTA_Z
* \brief Used to set the default value for the delta_z variable of the
*        Parameters structure, which thus rules the default discretization step
*        in the third-coordinate direction.
*
* We must have (double)\ref DELTA_Z > 0.0 and (double)\ref DELTA_Z == (\ref
* Z_MAX - \ref Z_MIN)/(\ref N_Z - 1) (or with at least a difference of less than
* 1e-16).
*/
#define DELTA_Z 0.1


// Related to the default level-set function of the initial domain
/**
* \def LS_TYPE
* \brief Used to set the default value for the ls_type variable of the
*        Parameters structure, which thus rules the default shape of the initial
*        internal domain (cube or sphere).
*
* If set to 0, then the initial domain is a cube of center (\ref LS_X, \ref
* LS_Y, \ref LS_Z) and size \ref LS_R. Otherwise, it must be set to 1 and the
* initial domain is a sphere (same center and radius \ref LS_R).
*/
#define LS_TYPE 0

/**
* \def LS_X
* \brief Used to set the default value for the ls_x variable of the Parameters
*        structure, which thus rules the default first coordinate of the
*        cube/sphere center related to the initial domain: (double) \ref LS_X.
*/
#define LS_X 0.0

/**
* \def LS_Y
* \brief Used to set the default value for the ls_y variable of the Parameters
*        structure, which thus rules the default second coordinate of the
*        cube/sphere center related to the initial domain: (double) \ref LS_Y.
*/
#define LS_Y 0.0

/**
* \def LS_Z
* \brief Used to set the default value for the ls_z variable of the Parameters
*        structure, which thus rules the default third coordinate of the
*        cube/sphere center related to the initial domain: (double) \ref LS_Z.
*/
#define LS_Z 0.0

/**
* \def LS_R
* \brief Used to set the default value for the ls_r variable of the Parameters
*        structure, which thus rules the default length/radius of the
*        cube/sphere associated with the initial domain.

* We must have (double)\ref LS_R > 0.0.
*/
#define LS_R 1.0


// Related to the default metric computation (can be used only if
// opt_mode=1/2/3/4 in the structure Parameters)
/**
* \def MET_ERR
* \brief Used to set the default value for the met_err variable of the
*        Parameters structure, which thus rules the default tolerance allowed
*        on the error made during the computation of the adaptation metric.
*
* We must have (double)\ref MET_ERR > 0.0. Although this condition is checked,
* it will only be used if opt_mode==1/2/3/4 in the Parameters structure.
*/
#define MET_ERR 0.01

/**
* \def MET_MIN
* \brief Used to set the default value for the met_min variable of the
*        Parameters structure, which thus rules the default minimum length
*        allowed for edges in the computation of the adaptation metric.
*
* We must have (double)\ref MET_MIN > 0.0 and \ref MET_MIN < \ref MET_MAX.
* Although these conditions are checked, it will only be used if
* opt_mode==1/2/3/4 in the Parameters structure.
*/
#define MET_MIN 0.01

/**
* \def MET_MAX
* \brief Used to set the default value for the met_max variable of the
*        Parameters structure, which thus rules the default maximal length
*        allowed for edges in the computation of the adaptation metric.
*
* We must have (double)\ref MET_MAX > \ref MET_MIN. Although this condition is
* checked, it will only be used if opt_mode==1/2/3/4 in the Parameters
* structure.
*/
#define MET_MAX 1.0


// Related to the default overlap-matrix computation (can be used only if
// opt_mode=-2/-1/0 in the structure Parameters)
/**
* \def TRICK_MATRIX
* \brief Used to set the default value for the trick_matrix variable of the
*        Parameters structure, which thus rules the default computation mode
*        of the overlap matrix for hexahedral meshes.
*
* If set to 0, then the overlap matrix is completely recomputed at each
* iteration. Otherwise, it must be set to 1 and a trick is performed at each
* iteration (except for the first one) on the hexahedron's labels to compute
* only the difference between the old overlap-matrix coefficients and the new
* ones. Although we checked this zero-or-one value condition, it will only be
* used if opt_mode=-2/-1/0 in the Parameters structure.
*/
#define TRICK_MATRIX 1

/**
* \def APPROX_MODE
* \brief Used to set the default value for the approx_mode variable of the
*        Parameters structure, which thus rules the default approximation mode
*        of the overlap matrix coefficients for hexahedral meshes.
*
* If set to 0, then the overlap matrix is computed on hexahedral meshes using
* exact values. Otherwise, it must be set to 1 and the overlap matrix is
* evaluated on hexahedral meshes using approximated values (volume times the
* the integrand value at the cube element center). Although we checked this
* zero-or-one value condition, it will only be used if opt_mode=-2/-1/0 in the
* Parameters structure.
*/
#define APPROX_MODE 0


// Related to the default stop criteria in the optimization loop
/**
* \def ITER_MAX
* \brief Used to set the default value for the iter_max variable of the
*        Parameters structure, which thus rules the default maximal number of
*        iterations allowed in the optimization loop.
*
* We must have (int)\ref ITER_MAX >= 0 (zero means that no optimization loop is
* performed).
*/
#define ITER_MAX 100

/**
* \def ITER_TOLD0P
* \brief Used to set the default value for the iter_told0p variable of the
*        Parameters structure, which thus rules the tolerance allowed between
*        the probabilities of two successive iterations in the optimization
*        loop.
*
* We must have (double)\ref ITER_TOLD0P >= 0.0 (zero means that it is not taken
* into account in the stop criteria of the optimization loop).
*/
#define ITER_TOLD0P 1.0e-6

/**
* \def ITER_TOLD1P
* \brief Used to set the default value for the iter_told1p variable of the
*        Parameters structure, which thus rules the default tolerance allowed
*        on the first-order optimality condition of the optimization problem.
*
* If opt_mode=1/2/3/4 in the Parameters structure, it corresponds to the
* tolerance allowed on the shape derivative of the probability, taken in the
* direction of the shape gradient. Otherwise, we have opt_mode=-2/-1/0 in the
* Parameters structure and it represents the tolerance allowed between the
* probability differences of two successive iterations. We must have (double)
* \ref ITER_TOLD1P >= 0.0 (zero means that it is not taken into account in the
* stop criteria of the optimization loop).
*/
#define ITER_TOLD1P 1.0e-6

/**
* \def ITER_TOLD2P
* \brief Used to set the default value for the iter_told2p variable of the
*        Parameters structure, which thus rules the default tolerance allowed
*        on the second-order optimality condition of the optimization problem.
*
* If opt_mode=1/2/3/4 in the Parameters structure, it corresponds to the
* tolerance allowed between the shape derivatives of two successive iterations,
* both taken in the direction of the shape gradient. Otherwise, we have
* opt_mode=-2/-1/0 in the Parameters structure and it represents the tolerance
* allowed between the second-order probability differences of two successive
* iterations. We must have (double) \ref ITER_TOLD2P >= 0.0 (zero means that it
* is not taken into account in the stop criteria of the optimization loop).
*/
#define ITER_TOLD2P 1.0e-6

// Related to the default saving of data
/**
* \def SAVE_TYPE
* \brief Used to set the default value for the save_type variable of the
*        Parameters structure, which thus rules the default type of output mesh
*        format used by the MPD algorithm.
*
* If set to 1, then any output mesh is saved using the *.mesh format. If set
* to 0, then any output mesh is saved using the *.cube format. Otherwise, it
* must be set to 2 and in this case, any output mesh is saved in both *.cube and
* *.mesh format.
*/
#define SAVE_TYPE 1

/**
* \def SAVE_MESH
* \brief Used to set the default value for the save_mesh variable of the
*        Parameters structure, which thus rules the default frequency at which
*        the mesh is saved in the optimization loop.
*
* We must have (int)\ref SAVE_MESH >= 0 (zero means that the mesh is never saved
* in the optimization loop, and also if \ref SAVE_MESH is (strictly) greater
* than \ref ITER_MAX).
*/
#define SAVE_MESH 0

/**
* \def SAVE_DATA
* \brief Used to set the default value for the save_data variable of the
*        Parameters structure, which thus rules the default frequency at which
*        the data are saved in the optimization loop.
*
* We must have (int)\ref SAVE_DATA >= 0 (zero means that data are never saved
* in the optimization loop, and also if \ref SAVE_DATA is (strictly) greater
* than \ref ITER_MAX). We refer to the Data structure for further details
* concerning what kind of data are saved in the MPD algorithm.
*/
#define SAVE_DATA 1

/**
* \def SAVE_PRINT
* \brief Used to set the default value for the save_print variable of the
*        Parameters structure, which thus rules the default frequency at which
*        the mesh is displayed by medit software in the optimization loop.
*
* We must have (int)\ref SAVE_PRINT >= 0 (zero means that the mesh is never
* plotted in the optimization loop, and also if \ref SAVE_PRINT is (strictly)
* greater than \ref ITER_MAX). We recall that medit software must have been
* previously installed and every time that medit is loaded, the algorithm will
* stop until exit is performed.
*/
#define SAVE_PRINT 0

/**
* \def SAVE_WHERE
* \brief Used to set the default value for the save_where variable of the
*        Parameters structure, which thus rules the default integer referring to
*        which part of the optimization loop the save_print variable of the
*        Parameters structure is vizualizing.
*
* If set to 1, the metric is displayed with the mesh on medit software, which
* must have been previously intalled. If set to 2, the level-set function is
* plotted. If set to 3/4, it is the shape gradient (before/after relabelling).
* If set to 5, the extension of the shape gradient using the elasticy equations
* is vizualized. If set to 6, it is the advection of the level-set function. If
* set to 7, the advected new domain is displayed. Other values are forbidden.
*/
#define SAVE_WHERE 7


// Related to the default path specifications for the external softwares used in
// the MPD algorithm (Warning: medit, mmg3d, mshdist, elastic and advect
// softwares must have been previously installed)
/**
* \def PATH_LENGTH
* \brief Used to set the default value for the path_length variable of the
*        Parameters structure, which thus rules the default maximal length
*        allowed for storing the different path names.
*
* We must have (int)\ref PATH_LENGTH > 1. This condition ensures to store at
* least a name that contains something more than the terminating nul character
* '\0'. In fact, \ref PATH_LENGTH should always be (strictly) greater than the
* maximal size of the strings defined by \ref PATH_MEDIT, \ref PATH_MMG3D, \ref
* PATH_MSHDIST, \ref PATH_ELASTIC, and \ref PATH_ADVECT.
*/
#define PATH_LENGTH 101

/**
* \def PATH_MEDIT
* \brief Used to set the default string content of the array that will be
*        pointed by the path_medit variable of the Parameters structure, which
*        thus rules the default path name for medit software.
*
* (char[])\ref PATH_MEDIT must be a string of length (strictly) lower than \ref
* PATH_LENGTH. We recall that the medit software must have been previously
* installed.
*/
#define PATH_MEDIT "medit"

/**
* \def PATH_MMG3D
* \brief Used to set the default string content of the array that will be
*        pointed by the path_mmg3d variable of the Parameters structure, which
*        thus rules the default path name for mmg3d software.
*
* (char[])\ref PATH_MMG3D must be a string of length (strictly) lower than \ref
* PATH_LENGTH. Although this condition is checked, \ref PATH_MMG3D will only be
* used opt_mode=1/2/3/4 in the Parameters structure. We also recall that the
* mmg3d software must have been previously installed.
*/
#define PATH_MMG3D "mmg3d_O3"

/**
* \def PATH_MSHDIST
* \brief Used to set the default string content of the array that will be
*        pointed by the path_mshdist variable of the Parameters structure, which
*        thus rules the default path name for mshdist software.
*
* (char[])\ref PATH_MSHDIST must be a string of length (strictly) lower than
* \ref PATH_LENGTH. Although this condition is checked, \ref PATH_MSHDIST will
* only be used opt_mode=1/2/4 in the Parameters structure. We also recall that
* the mshdist software must have been previously installed.
*/
#define PATH_MSHDIST "mshdist"

/**
* \def PATH_ELASTIC
* \brief Used to set the default string content of the array that will be
*        pointed by the path_elastic variable of the Parameters structure, which
*        thus rules the default path name for elastic software.
*
* (char[])\ref PATH_ELASTIC must be a string of length (strictly) lower than
* \ref PATH_LENGTH. Although this condition is checked, \ref PATH_ELASTIC will
* only be used opt_mode=1/2 in the Parameters structure. We also recall that the
* elastic software must have been previously installed.
*/
#define PATH_ELASTIC "elastic"

/**
* \def PATH_ADVECT
* \brief Used to set the default string content of the array that will be
*        pointed by the path_advect variable of the Parameters structure, which
*        thus rules the default path name for advect software.
*
* (char[])\ref PATH_ADVECT must be a string of length (strictly) lower than
* \ref PATH_LENGTH. Although this condition is checked, \ref PATH_ADVECT will
* only be used opt_mode=1/2 in the Parameters structure. We also recall that the
* advect software must have been previously installed.
*/
#define PATH_ADVECT "advect"


// Related to the default parameters used by mmg3d to build a default isotropic
// metric (can be used only if opt_mode=1/2/3/4 in the structure Parameters).
// Warning: mmg3d software must have been previously installed
/**
* \def HMIN_ISO
* \brief Used to set the default value for the hmin_iso variable of the
*        Parameters structure, which thus rules the default minimum length
*        allowed for edges in the computation of the default isotropic
*        adaptation metric by mmg3d software.
*
* We must have (double)\ref HMIN_ISO > 0.0 and \ref HMIN_ISO < \ref HMAX_ISO.
* Although these conditions are checked, it will only be used if
* opt_mode=1/2/3/4 in the Parameters structure. We also recall that the mmg3d
* software must have been previously installed.
*/
#define HMIN_ISO 0.1

/**
* \def HMAX_ISO
* \brief Used to set the default value for the hmax_iso variable of the
*        Parameters structure, which thus rules the default maximal length
*        allowed for edges in the computation of the default isotropic
*        adaptation metric by mmg3d software.
*
* We must have (double)\ref HMAX_ISO > \ref HMIN_ISO. Although this condition is
* checked, it will only be used if opt_mode=1/2/3/4 in the Parameters structure.
* We also recall that the mmg3d software must have been previously installed.
*/
#define HMAX_ISO 0.2

/**
* \def HAUSD_ISO
* \brief Used to set the default value for the hausd_iso variable of the
*        Parameters structure, which thus rules the default tolerance allowed on
*        the error measured by the Hausdorff distance between the adapted
*        surface and its ideal representation during the computation of the
*        default isotropic adaptation metric by mmg3d software.
*
* We must have (double)\ref HAUSD_ISO > 0.0. Although this condition is checked,
* it will only be used if opt_mode=1/2/3/4 in the Parameters structure. We also
* recall that the mmg3d software must have been previously installed.
*/
#define HAUSD_ISO 0.01

/**
* \def HGRAD_ISO
* \brief Used to set the default value for the hgrad_iso variable of the
*        Parameters structure, which thus rules the default tolerance allowed
*        for the (gradation) ratio between any two edge lengths sharing one
*        point together during the computation of the default isotropic
*        adaptation metric by mmg3d software.
*
* We must have (double)\ref HGRAD_ISO > 1.0. Although this condition is checked,
* it will only be used if opt_mode=1/2/3/4 in the Parameters structure. We also
* recall that the mmg3d software must have been previously installed.
*/
#define HGRAD_ISO 2.0


// Related to the default parameters used by mmg3d to build the metric related
// to the molecular orbitals (can be used only if opt_mode=1/2/3/4 in the
// structure Parameters). Warning: mmg3d software must have been previously
// installed
/**
* \def HMIN_MET
* \brief Used to set the default value for the hmin_met variable of the
*        Parameters structure, which thus rules the default minimum length
*        allowed for edges in the computation of the molecular-orbitals
*        adaptation metric by mmg3d software.
*
* We must have (double)\ref HMIN_MET > 0.0 and \ref HMIN_MET < \ref HMAX_MET.
* Although these conditions are checked, it will only be used if
* opt_mode=1/2/3/4 in the Parameters structure. We also recall that the mmg3d
* software must have been previously installed.
*/
#define HMIN_MET 0.05

/**
* \def HMAX_MET
* \brief Used to set the default value for the hmax_met variable of the
*        Parameters structure, which thus rules the default maximal length
*        allowed for edges in the computation of the molecular-orbitals
*        adaptation metric by mmg3d software.
*
* We must have (double)\ref HMAX_MET > \ref HMIN_MET. Although this condition is
* checked, it will only be used if opt_mode=1/2/3/4 in the Parameters structure.
* We also recall that the mmg3d software must have been previously installed.
*/
#define HMAX_MET 1.0

/**
* \def HAUSD_MET
* \brief Used to set the default value for the hausd_met variable of the
*        Parameters structure, which thus rules the default tolerance allowed on
*        the error measured by the Hausdorff distance between the adapted
*        surface and its ideal representation during the computation of the
*        molecular-orbitals adaptation metric by mmg3d software.
*
* We must have (double)\ref HAUSD_MET > 0.0. Although this condition is checked,
* it will only be used if opt_mode=1/2/3/4 in the Parameters structure. We also
* recall that the mmg3d software must have been previously installed.
*/
#define HAUSD_MET 0.01

/**
* \def HGRAD_MET
* \brief Used to set the default value for the hgrad_met variable of the
*        Parameters structure, which thus rules the default tolerance allowed
*        for the (gradation) ratio between any two edge lengths sharing one
*        point together during the computation of the molecular-orbitals
*        adaptation metric by mmg3d software.
*
* We must have (double)\ref HGRAD_MET > 1.0. Although this condition is checked,
* it will only be used if opt_mode=1/2/3/4 in the Parameters structure. We also
* recall that the mmg3d software must have been previously installed.
*/
#define HGRAD_MET 2.0


// Related to the default parameters used by the modified version of mmg3d in
// order to build the metric associated with the level-set function but also
// respecting the orbitals' metric (can be used only if opt_mode=1/2/4 in the
// structure Parameters). Warning: the modified version of mmg3d software must
// have been previously installed; its source files are joined with the ones of
// the MPD program)
/**
* \def HMIN_LS
* \brief Used to set the default value for the hmin_ls variable of the
*        Parameters structure, which thus rules the default minimum length
*        allowed for edges in the computation of both level-set function and
*        molecular-orbitals adaptation metric by the modified version of mmg3d
*        software.
*
* We must have (double)\ref HMIN_LS > 0.0 and \ref HMIN_LS < \ref HMAX_LS.
* Although these conditions are checked, it will only be used if opt_mode=1/2/4
* in the Parameters structure. We also recall that the modified version of mmg3d
* software must have been previously installed (its source files are joined with
* the ones of the MPD program).
*/
#define HMIN_LS 0.05

/**
* \def HMAX_LS
* \brief Used to set the default value for the hmax_ls variable of the
*        Parameters structure, which thus rules the default maximal length
*        allowed for edges in the computation of both level-set function and
*        molecular-orbitals adaptation metric by the modified version of mmg3d
*        software.
*
* We must have (double)\ref HMAX_LS > \ref HMIN_LS. Although this condition is
* checked, it will only be used if opt_mode=1/2/4 in the Parameters structure.
* We also recall that the modified version of mmg3d software must have been
* previously installed (its source files are joined with the ones of the MPD
* program).
*/
#define HMAX_LS 1.0

/**
* \def HAUSD_LS
* \brief Used to set the default value for the hausd_ls variable of the
*        Parameters structure, which thus rules the default tolerance allowed on
*        the error measured by the Hausdorff distance between the adapted
*        surface and its ideal representation during the computation of both
*        level-set function and molecular-orbitals adaptation metric by the
*        modified version of mmg3d software.
*
* We must have (double)\ref HAUSD_LS > 0.0. Although this condition is checked,
* it will only be used if opt_mode=1/2/4 in the Parameters structure. We also
* recall that the modified version of mmg3d software must have been previously
* installed (its source files are joined with the ones of the MPD program).
*/
#define HAUSD_LS 0.01

/**
* \def HGRAD_LS
* \brief Used to set the default value for the hgrad_ls variable of the
*        Parameters structure, which thus rules the default tolerance allowed
*        for the (gradation) ratio between any two edge lengths sharing one
*        point together during the computation of both level-set function and
*        molecular-orbitals adaptation metric by the modified version of mmg3d
*        software.
*
* We must have (double)\ref HGRAD_LS > 1.0. Although this condition is checked,
* it will only be used if opt_mode=1/2/4 in the Parameters structure. We also
* recall that the modified version of mmg3d software must have been previously
* installed (its source files are joined with the ones of the MPD program).
*/
#define HGRAD_LS 2.0


// Related to the default parameters used by mmg3d to build the metric in the
// Lagrangian mode (can be used only if opt_mode=1/3 in the structure
// Parameters). Warning: mmg3d software must have been previously installed
/**
* \def HMODE_LAG
* \brief Used to set the default value for the hmode_lag variable of the
*        Parameters structure, which thus rules the default Lagrangian mode
*        used in the computation of the adaptation metric in Lagrangian mode by
*        the mmg3d software.
*
* It set to 0, no local remeshing after displacements is performed by the mmg3d
* software. If set to 1, mmg3d swaps the distorded edges and relocates the
* vertices of distorded tetrahedra. If set to 2, mmg3d splits the points and
* collapses them if necessary, then performs the same types of remeshing
* operations than when \ref HMODE_LAG is set to one. Other values are forbidden.
* Although we check this zero-or-one-or-two value condition, it will be used
* only if opt_mode=1/3 in the Parameters structure. We also recall that the
* mmg3d software must have been previously installed.
*/
#define HMODE_LAG 0

/**
* \def HMIN_LAG
* \brief Used to set the default value for the hmin_lag variable of the
*        Parameters structure, which thus rules the default minimum length
*        allowed for edges in the computation of the adaptation metric in
*        Lagrangian mode by mmg3d software.
*
* We must have (double)\ref HMIN_LAG > 0.0 and \ref HMIN_LAG < \ref HMAX_LAG.
* Although these conditions are checked, it will only be used if opt_mode=1/3
* in the Parameters structure. We also recall that the mmg3d software must have
* been previously installed.
*/
#define HMIN_LAG 0.05

/**
* \def HMAX_LAG
* \brief Used to set the default value for the hmax_lag variable of the
*        Parameters structure, which thus rules the default maximal length
*        allowed for edges in the computation of the adaptation metric in
*        Lagrangian mode by mmg3d software.
*
* We must have (double)\ref HMAX_LAG > \ref HMIN_LAG. Although this condition is
* checked, it will only be used if opt_mode=1/3 in the Parameters structure. We
* also recall that the mmg3d software must have been previously installed.
*/
#define HMAX_LAG 1.0

/**
* \def HAUSD_LAG
* \brief Used to set the default value for the hausd_lag variable of the
*        Parameters structure, which thus rules the default tolerance allowed on
*        the error measured by the Hausdorff distance between the adapted
*        surface and its ideal representation during the computation of the
*        adaptation metric in Lagrangian mode by mmg3d software.
*
* We must have (double)\ref HAUSD_LAG > 0.0. Although this condition is checked,
* it will only be used if opt_mode=1/3 in the Parameters structure. We also
* recall that the mmg3d software must have been previously installed.
*/
#define HAUSD_LAG 0.01

/**
* \def HGRAD_LAG
* \brief Used to set the default value for the hgrad_lag variable of the
*        Parameters structure, which thus rules the default tolerance allowed
*        for the (gradation) ratio between any two edge lengths sharing one
*        point together during the computation of the adaptation metric in
*        Lagrangian mode by mmg3d software.
*
* We must have (double)\ref HGRAD_LAG > 1.0. Although this condition is checked,
* it will only be used if opt_mode=1/3 in the Parameters structure. We also
* recall that the mmg3d software must have been previously installed.
*/
#define HGRAD_LAG 2.0


// Related to the default additional parameters for the command line of mshdist
// and advect softwares in the MPD algorithm (can be used only if opt_mode=1/2/4
// in the structure Parameters). Warning: mshdist and advect softwares must have
// been previously installed
/**
* \def N_ITER
* \brief Used to set the default value for the n_iter variable of the Parameters
*        structure, which thus rules the default maximal number of iterations
*        allowed in the iterative renormalization of the signed distance
*        function by the mshdist software.
*
* We must have (int)\ref N_ITER >= 0 (zero means that no limit is fixed
* concerning the maximal number of iterations). Although this condition is
* checked, it will only be used if opt_mode=1/2/4 in the Parameters structure.
* We also recall that the mshdist software must have been previously installed.
*/
#define N_ITER 0

/**
* \def RESIDUAL
* \brief Used to set the default value for the residual variable of the
*        Parameters structure, which thus rules the default tolerance allowed on
*        the residual, i.e. the value below which the convergence is considered
*        to be achieved by the mshdist software.
*
* We must have (double)\ref RESIDUAL >= 0.0 (zero means that no limit is
* prescribed by the user in mshdist command line). Although this condition is
* checked, it will only be used if opt_mode=1/2/4 in the Parameters structure.
* We also recall that the mshdist software must have been previously installed.
*/
#define RESIDUAL 0.0

/**
* \def DELTA_T
* \brief Used to set the default value for the delta_t variable of the
*        Parameters structure, which thus rules the final time at which the
*        advection equation is expected to be solved by advect software.
*
* We must have (double)\ref DELTA_T > 0.0. Although this condition is checked,
* it will only be used if opt_mode=1/2 in the Parameters structure. We also
* recall that the advect software must have been previously installed.
*/
#define DELTA_T 1.0

/**
* \def NO_CFL
* \brief Used to set the default value for the no_cfl variable of the Parameters
*        structure, which thus rules the default value associated with a
*        potential restriction of the final time at which the advection equation
*        is expected to be solved by advect software.
*
* If set to 0, then a cfl condition is computed, potentially restricting the
* final time (stored in the delta_t variable of the Parameters structure) at
* which the advection equation was meant to be solved by the advect software.
* Otherwise, it must be set to 1 and no restriction is performed on the
* prescribed final time. However, in such a situation, the advect software may
* fail to get a solution or simply alter it due to the numerical instabilities
* produced by an inacurate final time, much larger than the (theoretical) order
* of magnitude given by the cfl condition. Moreover, although this zero-or-one
* value condition is checked, it will only be used if opt_mode=1/2 in the
* Parameters structure. We also recall that the advect software must have been
* previously installed.
*/
#define NO_CFL 1


/* ************************************************************************** */
// Prototypes of non-static functions
/* ************************************************************************** */
/**
* \fn void initializeParameterStructure(Parameters* pParameters)
* \brief It initializes to zero the Parameters structure pointed by pParameters.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function .
*
* The function \ref initializeParameterStructure sets to zero all the variables
* of the structure pointed by pParameters (and pointers are set to NULL). This
* function does not return any value (void output).
*/
void initializeParameterStructure(Parameters* pParameters);

/**
* \fn void freeParameterMemory(Parameters* pParameters)
* \brief It frees the memory dynamically allocated for the Parameter structure
*        pointed by pParameters.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*
* The function \ref freeParameterMemory frees the memory dynamically allocated
* with calloc/malloc/realloc for the Parameters structure pointed by
* pParameters. All pointers of the structure Parameters that have been freed up
* are also set to NULL (but other variables than pointers are not reset to
* zero). This function does not return any value (void output).
*/
void freeParameterMemory(Parameters* pParameters);

/**
* \fn int setupDefaultParameters(Parameters* pParameters, char* nameInfo)
* \brief It initializes all the variable of the structure pointed by pParameters
*       to their associated default values.
*
* \param[out] pParameters A pointer that points to the Parameters structure
*                         (defined in main.h file) of the \ref main function.
*                         The pointer must not be NULL, otherwise an error
*                         is returned by the \ref setupDefaultParameters
*                         function.
*
* \param[in] nameInfo A pointer that points to a string that is intended to
*                     store the path name of an existing *.info (input) file.
*                     Its content will be copied in the name_info variable of
*                     the Parameters structure.
*
* \return It returns one if the structure pointed by pParameters has been
*         successfully initialized to its default values. Otherwise, zero is
*         returned if an error is encountered in the process.

* The function \ref setupDefaultParameters initializes the Parameters structure
* pointed by the pParameters variable to its default values given by the
* preprocessor constants of loadParameters.h file. In particular, it dynamically
* allocates some memory in order to store the content of the string pointed by
* nameInfo into the name_info variable of the Parameters structure. We recall
* that the length of the nameInfo variable must be (strictly) lower than
* NAME_LENGTH. Such operations are also performed in order to set up the path_*
* variables to their default (path-name) contents. The \ref
* setupDefaultParameters function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int setupDefaultParameters(Parameters* pParameters, char* nameInfo);

/**
* \fn int getLengthAfterKeywordBeginning(char keywordBeginning[3], int counter)
* \brief It evaluates the number of characters to read after keywordBeginning,
*        which depends on the different possibilities allowed by its content,
         referring to the (counter)-th keyword of the *.info file.
*
* \param[in] keywordBeginning An array containing two characters and the
*                             terminating nul one '\0'. They represents the
*                             beginning of the (counter)-th keyword whose end
*                             needs to be read properly. This keyword can be the
*                             name of any variables (70 possibilities except
*                             name_info which is replaced by the end_data
*                             keyword, ending the reading in the *.info
*                             file; any other information placed after will not
*                             be read and considered as a comment) stored in
*                             the Parameters structure. The array must contain
*                             two non-nul characters followed by a nul one,
*                             otherwise an error is returned by the \ref
*                             getLengthAfterKeywordBeginning function.
*
* \param[in] counter An integer referring to the line of the *.info file
*                    at which the keyword is read. This integer is mainly use
*                    help the user to locate any syntax error found while
*                    reading the *.info file. More precisely, (counter-1) refers
*                    to the number of keywords that have already been
*                    successfully read and the counter variable to the one whose
*                    first two letters are stored in keywordBeginning. The
*                    integer must be comprised between one and seventy
*                    (case where all the variables of the Parameters structure
*                    are specified, except the name_info one, already storing
*                    the name of the *.info file given in the input command line
*                    of the MPD program, and replaced by the end_data keyword,
*                    which ends the reading in the *.info file; any other
*                    information placed after will not be read and considered
*                    as a comment). Otherwise, an error is returned by the \ref
*                    getLengthAfterKeywordBeginning function.
*
* \return It returns an integer (strictly) comprised between one and twelve.
*         Otherwise, zero is returned if an error is encountered. To be more
*         precise, two is returned if keywordBeginning stores 'n_' (one
*         character is needed to distinguish (n_)c(pu), (n_)x, (n_)y, (n_)z,
*         and (n_)i(ter) keywords in the Parameters structure). Three is
*         returned if keywordBeginning stores 'ls' (two characters are needed
*         to distinguish (ls)_t(ype), (ls)_x, (ls)_y, (ls)_z, and (ls)_r
*         keywords in the Parameters structure). Four is returned if
*         keywordBeginning stores 'x_', 'y_', or 'z_' (three characters are
*         needed to distinguish (x_)min, (x_)max, (y_)min, (y_)max, (z_)min,
*         and (z_)max keywords in the Parameters structure). Five is returned
*         if keywordBeginning stores 'no' (four characters are needed to read
*         (no)_cfl keyword in the Parameters structure). Six is returned if
*         keywordBeginning stores 've', 'nu', 'or', 'de', 'me', or 'hm' (five
*         characters are needed to distinguish (ve)rbose, (nu)_elec(trons),
*         (nu)_spin, (or)b_rhf, (de)lta_x, (de)lta_y, (de)lta_z, (de)lta_t,
*         (me)t_err, (me)t_min, (me)t_max, (hm)in_is(o), (hm)ax_is(o),
*         (hm)in_me(t), (hm)ax_me(t), (hm)in_ls, (hm)ax_ls, (hm)ode_l(ag),
*         (hm)in_la(g), and (hm)ax_la(g) keywords in the Parameters structure).
*         Seven is returned if keywordBeginning stores 'op', 'en' 'it', 'ha',
*         'hg', or 're' (six characters are needed to distinguish (op)t_mode,
*         (it)er_max, (it)er_tol(d0p), (it)er_tol(d1p), (it)er_tol(d2p),
*         (ha)usd_is(o), (ha)usd_me(t), (ha)usd_ls, (ha)usd_la(g),
*         (hg)rad_is(o), (hg)rad_me(t), (hg)rad_ls, (hg)rad_la(g), and
*         (re)sidual keywords in the Parameters structure, but also the
*         (en)d_data keyword). Eight is returned if keywordBeginning stores 'na'
*         or 'sa' (seven characters are needed to distinguish (na)me_leng(th),
*         (na)me_data, (na)me_chem, (na)me_mesh, (na)me_elas, (sa)ve_type,
*         (sa)ve_mesh, (sa)ve_data, (sa)ve_prin(t), and (sa)ve_wher(e) keywords
*         in the Parameters structure). Nine is returned if keywordBeginning
*         stores 'pa' (eight characters are needed to distinguish
*         (pa)th_lengt(h), (pa)th_medit, (pa)th_mmg3d, (pa)th_mshdi(st),
*         (pa)th_elast(ic), and (pa)th_advec(t) keywords in the Parameters
*         structure). Ten is returned if keywordBeginning stores 'ap' (nine
*         characters are needed to read (ap)prox_mode keyword in the Parameters
*         structure). Finally, eleven is returned if keywordBeginning stores
*         'tr' (ten characters are needed to  read (tr)ick_matrix keyword in the
*         Parameters structure). In any other situations (i.e. if the input
*         variables does not have the expected content), an error is displayed
*         in the standard error stream and zero is returned by the \ref
*         getLengthAfterKeywordBeginning function.
*
* The \ref getLengthAfterKeywordBeginning function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int getLengthAfterKeywordBeginning(char keywordBeginning[3], int counter);


/**
* \fn int getTypeAfterKeyword(char keywordMiddle[11], int lengthMiddle,
*                                                                   int counter)
* \brief Depending on the string keywordMiddle of length lengthMiddle, it
*        returns an integer corresponding to the type of variables to be read
*        after the (counter)-th keyword in the *.info file.
*
* \param[in] keywordMiddle An array containing (lengthMiddle-1) non-nul
*                          characters followed by the terminating nul one '\0'.
*                          They represent the second part of the (counter)-th
*                          keyword, whose related value (integer/double/string)
*                          needs to be specified in order to be read after
*                          properly and securely the by fscanf standard
*                          c-function. This keyword can be the name of any
*                          variables (70 possibilities except name_info which
*                          is replaced by the end_data keyword, ending the
*                          reading in the *.info file; any other information
*                          placed after will not be read and considered as a
*                          comment) stored in the Parameters structure. The
*                          array must contain exactly, starting from the
*                          beginning (lengthMiddle-1) non-nul characters
*                          followed by a nul one, otherwise an error is returned
*                          by the \ref getTypeAfterKeyword function.
*
* \param[in] lengthMiddle An integer referring to the length of the
*                         keywordMiddle variable, including the terminating nul
*                         character '\0'. It is intended to store the return
*                         value of the \ref getLengthAfterKeywordBeginning
*                         function. Hence, it must be comprised between two and
*                         eleven, otherwise an error is returned by the \ref
*                         getTypeAfterKeyword function.
*
* \param[in] counter An integer referring to the line of the *.info file
*                    at which the keyword is read. This integer is mainly use
*                    help the user to locate any syntax error found while
*                    reading the *.info file. More precisely, (counter-1) refers
*                    to the number of keywords that have already been
*                    successfully read and the counter variable to the one whose
*                    is being evaluated by the \ref getTypeAfterKeyword
*                    function. The integer must be comprised between one and
*                    seventy (case where all the variables of the Parameters
*                    structure are specified, except the name_info one, already
*                    storing the name of the*.info file given in the input
*                    command line of the MPD program, and replaced by the
*                    end_data keyword, which ends the reading in the *.info
*                    file; any other information placed after will not be read
*                    and considered as a comment). Otherwise, an error is
*                    returned by the \ref getTypeAfterKeyword function.
*
* \return It returns minus one if no type is associated with the keywordMiddle
*         variable, i.e. if the corresponding keyword is the (en)d_data one. It
*         returns one if an integer is expected after, two if it is a double
*         value, and three if it is a string. Otherwise, zero is returned if an
*         error is encountered. To be more precise, minus one is returned if
*         keywordMiddle stores (en)d_data, which would end the reading of the
*         *.info file. One is returned if keywordMiddle stores either
*         (op)t_mode, (ve)rbose, (n_)c(pu), (na)me_leng(th), (nu)_elec(trons),
*         (nu)_spin, (or)b_rhf, (n_)x, (n_)y, (n_)z, (ls)_t(ype),
*         (tr)ick_matrix, (ap)prox_mode, (it)er_max, (sa)ve_type, (sa)ve_mesh,
*         (sa)ve_data, (sa)ve_prin(t), (sa)ve_wher(e), (pa)th_lengt(h),
*         (hm)ode_l(ag), (n_)i(ter) or (no)_cfl to refer to the corresponding
*         integer variables (23) of the Parameters structure. Two is returned if
*         keywordMiddle stores either (x_)min, (x_)max, (y_)min, (y_)max,
*         (z_)min, (z_)max, (de)lta_x, (de)lta_y, (de)lta_z, (ls)_x, (ls)_y,
*         (ls)_z, (ls)_r, (me)t_err, (me)t_min, (me)t_max, (it)er_tol(d0p),
*         (it)er_tol(d1p), (it)er_tol(d2p), (hm)in_is(o), (hm)ax_is(o),
*         (hm)in_me(t), (hm)ax_me(t), (hm)in_ls, (hm)ax_ls, (hm)in_la(g),
*         (hm)ax_la(g), (ha)usd_is(o), (ha)usd_me(t), (ha)usd_ls, (ha)usd_la(g),
*         (hg)rad_is(o), (hg)rad_me(t), (hg)rad_ls, (hg)rad_la(g), (de)lta_t, or
*         (re)sidual to refer to the corresponding double variables (37) of the
*         Parameters structure. Three is returned if keywordMiddle stores either
*         (na)_data, (na)me_chem, (na)me_mesh, (na)me_elas, (pa)th_medit,
*         (pa)th_mmg3d, (pa)th_mshdi(st), (pa)th_elast(ic), or (pa)th_advec(t)
*         to refer to the corresponding string (char*) variables (9) of the
*         Parameters structure, (except the name_info one, already storing the
*         name of the *.info file). In any other situations (i.e. if the input
*         variables does not have the expected content), an error is displayed
*         in the standard error stream and zero is returned by the \ref
*         getTypeAfterKeyword function.
*
* The \ref getTypeAfterKeyword function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int getTypeAfterKeyword(char keywordMiddle[11], int lengthMiddle, int counter);

/**
* \fn int getLengthAfterKeywordMiddle(char keywordMiddle[11], int lengthMiddle,
                                                                    int counter)
* \brief Depending on the string keywordMiddle of length lengthMiddle, it
*        evaluates the number of characters to be read after keywordMiddle so as
*        to complete the reading of the (counter)-th keyword in the *.info file.
*
* \param[in] keywordMiddle An array containing (lengthMiddle-1) non-nul
*                          characters followed by the terminating nul one '\0'.
*                          They represent the second part of the (counter)-th
*                          keyword, whose third and final part needs to be read
*                          properly. This keyword can be the name of any
*                          variables (70 possibilities except name_info which
*                          is replaced by the end_data keyword, ending the
*                          reading in the *.info file; any other information
*                          placed after will not be read and considered as a
*                          comment) stored in the Parameters structure. The
*                          array must contain exactly, starting from the
*                          beginning (lengthMiddle-1) non-nul characters
*                          followed by a nul one, otherwise an error is returned
*                          by the \ref getLengthAfterKeywordMiddle function.
*
* \param[in] lengthMiddle An integer referring to the length of the
*                         keywordMiddle variable, including the terminating nul
*                         character '\0'. It is intended to store the return
*                         value of the \ref getLengthAfterKeywordBeginning
*                         function. Hence, it must be comprised between two and
*                         eleven, otherwise an error is returned by the \ref
*                         getLengthAfterKeywordMiddle function.
*
* \param[in] counter An integer referring to the line of the *.info file
*                    at which the keyword is read. This integer is mainly use
*                    help the user to locate any syntax error found while
*                    reading the *.info file. More precisely, (counter-1) refers
*                    to the number of keywords that have already been
*                    successfully read and the counter variable to the one whose
*                    last letters need to be read properly by the \ref
*                    getLengthAfterKeywordMiddle function. The integer must be
*                    comprised between one and seventy (case where all the
*                    variables of the Parameters structure are specified, except
*                    the name_info one, already storing the name of the*.info
*                    file given in the input command line of the MPD program,
*                    and replaced by the end_data keyword, which ends the
*                    reading in the *.info file; any other information placed
*                    after will not be read and considered as a comment).
*                    Otherwise, an error is returned by the \ref
*                    getLengthAfterKeywordMiddle function.
*
* \return It returns an integer (strictly) comprised between one (nothing more
*         to read) and six. Otherwise, zero is returned if an error is
*         encountered. To be more precise, one is returned if keywordMiddle
*         stores either (op)t_mode, (ve)rbose, (en)d_data (na)me_data,
*         (na)me_chem, (na)me_mesh, (na)me_elas, (nu)_spin, (or)b_rhf, (x_)min,
*         (x_)max, (y_)min, (y_)max, (z_)min, (z_)max, (n_)x, (n_)y, (n_)z,
*         (de)lta_x, (de)lta_y, (de)lta_z, (ls)_x, (ls)_y, (ls)_z, (ls)_r,
*         (me)t_err, (me)t_min, (me)t_max, (tr)ick_matrix, (ap)prox_mode,
*         (it)er_max, (sa)ve_type,(sa)ve_mesh, (sa)ve_data, (pa)th_medit,
*         (pa)th_mmg3d, (hm)_in_ls, (hm)ax_ls, (ha)usd_ls, (hg)rad_ls,
*         (re)sidual, (de)lta_t, or (no)_cfl, which means that nothing needs to
*         be read after in order to complete the reading of the (counter)-th
*         keyword. It means we know if it corresponds to the 'end_data' keyword
*         or to which variables of the Parameters structure it is referring to.
*         Two is returned if keywordMiddle stores either (sa)ve_prin(t),
*         (sa)ve_wher(e), (pa)th_lengt(h), (pa)th_advec(t), (hm)in_is(o),
*         (hm)ax_is(o), (hm)in_me(t), (hm)ax_me(t), (hm)in_la(g), (hm)ax_la(g),
*         (ha)usd_is(o), (ha)usd_me(t), (ha)usd_la(g), (hg)rad_is(o),
*         (hg)rad_me(t), (hg)rad_la(g), which means that one characters needs to
*         be read after in order to complete the reading on the (counter)-th
*         keyword. Three is returned if keywordMiddle stores either (n_)c(pu),
*         (na)me_leng(th), (pa)th_mshdi(st), (pa)th_elast(ic), or (hm)ode_l(ag),
*         which means that two characters needs to be read after in order to
*         complete the reading on the (counter)-th keyword. Four is returned if
*         keywordMiddle stores either (ls)_t(ype), (it)er_tol(d0p),
*         (it)er_tol(d1p), (it)er_tol(d2p), or (n_)i(ter) which means that
*         three characters needs to be read in order to complete the reading of
*         the (counter)-th keyword. Finally, six is returned if (nu)_elec(trons)
*         is stored in keywordMiddle, which means that five letters needs to be
*         read in order to complete the reading of the (counter)-th keyword. In
*         any other situations (i.e. if the input variables does not have the
*         expected content), an error is displayed in the standard error stream
*         and zero is returned by the \ref getLengthAfterKeywordMiddle function.
*
* The \ref getLengthAfterKeywordMiddle function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int getLengthAfterKeywordMiddle(char keywordMiddle[11], int lengthMiddle,
                                                                   int counter);

/**
* \fn int detectRepetition(int repetition[70], char keywordBeginning[3],
*                          char keywordMiddle[11], char keywordEnd[6],
*                                  int lengthMiddle, int lengthEnd, int counter)
* \brief It adds one in the array repetition at the location corresponding to
*        to the (counter)-th keyword of the *.info file, read as the
*        concatenation of keywordBeginning+keywordMiddle(+keywordEnd), where
*        keywordBeginning has length two, keywordMiddle has lengthMiddle and
*        keywordEnd has lengthEnd.
*
* \param[out] repetition An array of seventy integers saving the number of
*                        occurences of the corresponding keywords in the *.info
*                        file. The keyword position in the array is the one
*                        given in the structure Parameters, except for the
*                        end_data keyword which has the position of the
*                        name_info variable, already storing the name of the
*                        *.info file given in the input command line of the
*                        MPD program. We recall that the end_data keyword ends
*                        the reading in the *.info file; any other information
*                        placed after will not be read and considered as a
*                        comment.
*
* \param[in] keywordBeginning An array containing two characters and the
*                             terminating nul one '\0'. They represents the
*                             beginning of the (counter)-th keyword whose
*                             potential repetition in the upper part of the
*                             *.info file is intended to be detected. This
*                             keyword can be the name of any variables (70
*                             possibilities except name_info which is replaced
*                             by the end_data keyword, ending the reading in
*                             the *.info file; any other information placed
*                             after will not be read and considered as a
*                             comment) stored in the Parameters structure. The
*                             array must contain two non-nul characters followed
*                             by a nul one, otherwise an error is returned by
*                             the \ref detectRepetition function.
*
* \param[in] keywordMiddle An array containing (lengthMiddle-1) non-nul
*                          characters followed by the terminating nul one '\0'.
*                          They represent the second part of the (counter)-th
*                          keyword, whose final part is stored in keywordEnd
*                          (only if lengthEnd>1). This keyword can be the name
*                          of any variables (70 possibilities except name_info
*                          which is replaced by the end_data keyword, ending
*                          the reading in the *.info file; any other information
*                          placed after will not be read and considered as a
*                          comment) stored in the Parameters structure. The
*                          array must contain exactly, starting from the
*                          beginning (lengthMiddle-1) non-nul characters
*                          followed by a nul one, otherwise an error is returned
*                          by the \ref detectRepetition function.
*
* \param[in] keywordEnd An array containing (lengthEnd-1) non-nul characters
*                       followed by the terminating nul one '\0'. If
*                       lengthEnd>1, they represent the third part of the
*                       (counter)-th keyword, whose occurence is intented to be
*                       incremented by one in the array repetition. This
*                       keyword can be the name of any variables (70
*                       possibilities except name_info which is replaced by the
*                       end_data keyword, ending the reading in the *.info file;
*                       any other information placed after will not be read and
*                       considered as a comment) stored in the Parameters
*                       structure. The array must contain exactly, starting
*                       from the beginning (lengthEnd-1) non-nul characters
*                       followed by a nul one, otherwise an error is returned by
*                       the \ref detectRepetition function.
*
* \param[in] lengthMiddle An integer referring to the length of the
*                         keywordMiddle variable, including the terminating nul
*                         character '\0'. It is intended to store the return
*                         value of the \ref getLengthAfterKeywordBeginning
*                         function. Hence, it must be comprised between two and
*                         eleven, otherwise an error is returned by the \ref
*                         detectRepetition function.
*
* \param[in] lengthEnd An integer referring to the length of the keywordEnd
*                      variable, including the terminating nul character '\0'.
*                      In particular, if lengthEnd equals one, it means that
*                      keywordEnd is not necessary to read (counter)-th keyword,
*                      which is only given by the concatenation of
*                      keywordBeginning+keyworMiddle. The lengthEnd variable is
*                      intended to store the return value of the \ref
*                      getLengthAfterKeywordMiddle function. Hence, it must be
*                      comprised between one and six, otherwise an error is
*                      returned by the \ref detectRepetition function.
*
* \param[in] counter An integer referring to the line of the *.info file
*                    at which the keyword is read. This integer is mainly use
*                    help the user to locate any syntax error found while
*                    reading the *.info file. More precisely, (counter-1) refers
*                    to the number of keywords that have already been
*                    successfully read and the counter variable to the one *                    whose occurence is intented to be incremented by one in
*                    the array repetition by the \ref detectRepetition function.
*                    The integer must be comprised between one and seventy
*                    (case where all the variables of the Parameters structure
*                    are specified, except the name_info one, already storing
*                    the name of the*.info file given in the input command line
*                    of the MPD program, and replaced by the end_data keyword,
*                    which ends the reading in the *.info file; any other
*                    information placed after will not be read and considered
*                    as a comment). Otherwise, an error is returned by the \ref
*                    detectRepetition function.
*
* \return It returns one if the (counter)-th keyword is successfully identified
*         and its associated location in the array repetition successfully
*         incremented by one. In any other situations (i.e. if the input
*         variables does not have the expected content), an error is displayed
*         in the standard error stream and zero is returned by the \ref
*         detectRepetition function.
*
* The \ref detectRepetition function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int detectRepetition(int repetition[70], char keywordBeginning[3],
                     char keywordMiddle[11], char keywordEnd[6],
                                  int lengthMiddle, int lengthEnd, int counter);

/**
* \fn int changeValuesOfParameters(Parameters* pParameters,
*                                  char keywordBeginning[3],
*                                  char keywordMiddle[11], char keywordEnd[6],
*                                  char* readStringIn, int lengthMiddle,
*                                  int lengthEnd, int counter,
*                                          int readIntegerIn, double readDouble)
* \brief Depending of the type of variables concerned in the structure pointed
*        by pParameters, it saves the value stored either in readIntegerIn,
*        readDouble, or readStringIn in the Parameters structure at the location
*        associated with the (counter)-th keyword in the *.info file, read as
*        the concatenation of , where
*        keywordBeginning has length two, keywordMiddle has lengthMiddle and
*        keywordEnd has lengthEnd.
*
* \param[out] pParameters A pointer that points to the Parameters structure
*                         (defined in main.h file) of the \ref main function.
*                         In this Parameters structure, the location at which
*                         the value (specified by either readIntegerIn,
*                         readDouble, or readStringIn) is intended to be stored
*                         corresponds to the variable name given by the
*                         (counter)-th keyword, read as the concatenation of
*                         keywordBeginning+keywordMiddle(+keywordEnd), except
*                         for the end_data keyword. We recall that the end_data
*                         keyword ends the reading in the *.info file; any other
*                         information placed after will not be read and
*                         considered as a comment. The pointer must not be NULL
*                         and we assume that here all variables of the
*                         Parameters structure have already been initialized to
*                         their default values given by the processor constants
*                         defined in the loadParameters.h file. Otherwise (i.e.
*                         if the name_info and path_* does not have the correct
*                         size) an error is returned by the \ref
*                         changeValuesOfParameters function.
*
* \param[in] keywordBeginning An array containing two characters and the
*                             terminating nul one '\0'. They represents the
*                             beginning of the (counter)-th keyword whose
*                             associated value needs to be properly stored in
*                             the structure pointed by pParameters. This
*                             keyword can be the name of any variables (70
*                             possibilities except name_info, already storing
*                             the name of the *.info file given in the input
*                             command line of the MPD program, and which is
*                             replaced by the end_data keyword, ending the
*                             reading in the *.info file; any other information
*                             placed after will not be read and considered as a
*                             comment) stored in the Parameters structure. The
*                             array must contain two non-nul characters
*                             followed by a nul one, otherwise an error is
*                             returned by the \ref changeValuesOfParameters
*                             function.
*
* \param[in] keywordMiddle An array containing (lengthMiddle-1) non-nul
*                          characters followed by the terminating nul one '\0'.
*                          They represent the second part of the (counter)-th
*                          keyword, whose final part is stored in keywordEnd
*                          (only if lengthEnd>1). This keyword can be the name
*                          of any variables (70 possibilities except name_info
*                          which is replaced by the end_data keyword, ending
*                          the reading in the *.info file; any other information
*                          placed after will not be read and considered as a
*                          comment) stored in the Parameters structure. The
*                          array must contain exactly, starting from the
*                          beginning (lengthMiddle-1) non-nul characters
*                          followed by a nul one, otherwise an error is returned
*                          by the \ref changeValuesOfParameters function.
*
* \param[in] keywordEnd An array containing (lengthEnd-1) non-nul characters
*                       followed by the terminating nul one '\0'. If
*                       lengthEnd>1, they represent the third part of the
*                       (counter)-th keyword, whose associated value needs to
*                       be properly stored in the structure pointed by
*                       pParameters. This keyword can be the name of any
*                       variables (70 possibilities except name_info which is
*                       replaced by the end_data keyword, ending the reading in
*                       the *.info file; any other information placed after will
*                       not be read and considered as a comment) stored in the
*                       Parameters structure. The array must contain exactly,
*                       starting from the beginning (lengthEnd-1) non-nul
*                       characters followed by a nul one, otherwise an error is
*                       returned by the \ref changeValuesOfParameters function.
*
* \param[in] lengthMiddle An integer referring to the length of the
*                         keywordMiddle variable, including the terminating nul
*                         character '\0'. It is intended to store the return
*                         value of the \ref getLengthAfterKeywordBeginning
*                         function. Hence, it must be comprised between two and
*                         eleven, otherwise an error is returned by the \ref
*                         changeValuesOfParameters function.
*
* \param[in] lengthEnd An integer referring to the length of the keywordEnd
*                      variable, including the terminating nul character '\0'.
*                      In particular, if lengthEnd equals one, it means that
*                      keywordEnd is not necessary to read (counter)-th keyword,
*                      which is only given by the concatenation of
*                      keywordBeginning+keyworMiddle. The lengthEnd variable is
*                      intended to store the return value of the \ref
*                      getLengthAfterKeywordMiddle function. Hence, it must be
*                      comprised between one and six, otherwise an error is
*                      returned by the \ref changeValuesOfParameters function.
*
* \param[in] readStringIn A pointer that points to the string that is intended
*                         to be stored. It will only be used if the variables
*                         concerned and related to the (counter)-th keyword are
*                         strings. In this case, the pointer must not be NULL
*                         and the length of readStringIn must be compatible with
*                         the ones imposed by the Parameters structure
*                         (name_length for name_* variables and path_length for
*                         path_* ones). Otherwise, an error is returned by the
*                         \ref changeValuesOfParameters function.
*
* \param[in] counter An integer referring to the line of the *.info file
*                    at which the keyword is read. This integer is mainly use
*                    help the user to locate any syntax error found while
*                    reading the *.info file. More precisely, (counter-1) refers
*                    to the number of keywords that have already been
*                    successfully read and the counter variable to the one, *                    whose values is intended to be stored in the structure
*                    pointed by pParameters. The integer must be comprised
*                    between one and seventy (case where all the variables of
*                    the Parameters structure are specified, except the
*                    name_info one, already storing the name of the*.info file
*                    given in the input command line of the MPD program, and
*                    replaced by the end_data keyword, which ends the reading
*                    in the *.info file; any other information placed after will
*                    not be read and considered as a comment). Otherwise, an
*                    error is returned by the \ref changeValuesOfParameters
*                    function.
*
* \param[in] readIntegerIn An integer that is intended to be stored in the
*                          structure pointed by pParameters. It will only be
*                          used if the variables concerned and related to the
*                          (counter)-th keyword are integers. In this case, the
*                          value stored in readIntegerIn must be compatible with
*                          the ones imposed by the Parameters structure
*                          (name_length for name_* variables and path_length for
*                          path_* ones; the integer values for other variables
*                          are not checked). Otherwise, an error is returned by
*                          the \ref changeValuesOfParameters function.
*
* \param[in] readDouble A double value that is intended to be stored in the
*                       structure pointed by pParameters. It will only be used
*                       if the variables concerned and related to the
*                       (counter)-th keyword are double values. In this case,
*                       the value stored is not checked by the \ref
*                       changeValuesOfParameters function.
*
* \return It returns one if the value of the (counter)-th keyword is
*         successfully stored in the structure pointed by pParameters. In any
*         other situations (i.e. if the input variables does not have the
*         expected content or if the (re)allocation of dynamical memory
*         failed), an error is displayed in the standard error stream and zero
*         is returned by the \ref changeValuesOfParameters function.
*
* The \ref changeValuesOfParameters function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int changeValuesOfParameters(Parameters* pParameters, char keywordBeginning[3],
                             char keywordMiddle[11], char keywordEnd[6],
                             char* readStringIn, int lengthMiddle,
                             int lengthEnd, int counter, int readIntegerIn,
                                                             double readDouble);

/**
* \fn int readInfoFileAndGetParameters(Parameters* pParameters)
* \brief It tries to read the content of an existing the *.info (input) file
*        and it modifies accordingly the (default) values that were stored in
*        the structure pointed by pParameters.
*
* \param[in,out] pParameters A pointer that points to the Parameters structure
*                            (defined in main.h file) of the \ref main function.
*                            It must not point to NULL and its name_info
*                            variable must point to an existing *.info file
*                            name, otherwise an error is returned by the \ref
*                            readInfoFileAndGetParameters function.
*
* \return It returns one if the values of the structure pointed by pParameters
*         have been successfully updated by the ones prescribed in the *.info
*         file, that was stored in the name_info variable of the Parameters
*         structure. In any other situations (i.e. if the input variables does
*         not have the expected content or if the (re)allocation of dynamical
*         memory failed), an error is displayed in the standard error stream and
*         zero is returned by the \ref readInfoFileAndGetParameters function.
*
* The \ref readInfoFileAndGetParameters function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int readInfoFileAndGetParameters(Parameters* pParameters);

/**
* \fn int checkValuesOfAllParameters(Parameters* pParameters)
* \brief It examinates the content stored in the variables of the structure
*        pointed by pParameters and checks if the values that have been loaded
*        from the *.info file are valid in order to proceed further in the MPD
*        algorithm.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. It
*                        must not point to NULL and its variables must store the
*                        valid values as described in the documentation.
*                        Otherwise, an error is returned by the \ref
*                        checkValuesOfAllParameters function. Let us mention
*                        that if a default *.data and *.elas file names are
*                        generated if necessary thanks to the name_info
*                        variable (where the '.info' extension is replaced by
*                        the '.data' and '.elas' one), and the name_data and
*                        and name_elas variable are updated accordingly.
*
* \return It returns one if the values of the structure pointed by pParameters
*         all store valid contents. In any other situations (i.e. if the input
*         variables does not have the expected content), an error is displayed
*         in the standard error stream and zero is returned by the \ref
*         checkValuesOfAllParameters function.
*
* The \ref checkValuesOfAllParameters function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int checkValuesOfAllParameters(Parameters* pParameters);

/**
* \fn int loadParameters(Parameters* pParameters, char* nameInfo)
* \brief It loads the values given in an *.info (input) file pointed by
*        nameInfo, and stores them in the variables of the structure pointed by
*        pParameters. All the variables that have not been assigned are filled
*        with some default values.
*
* \param[out] pParameters A pointer that points to the Parameters structure
*                         (defined in main.h file) of the \ref main function.
*
* \param[in] nameInfo A pointer that points to a string that is intended to
*                     store the path name of an existing *.info (input) file.
*
* \return It returns one if the structure pointed by pParameters has been
*         successfully initialized to its prescribed values. Otherwise, zero is
*         returned if an error is encountered in the process.

* The function \ref loadParameters takes the informations given in an *.info
* (input) file pointed by the nameInfo variable, and combines it with the
* default values given by the preprocessor constants of the loadParameters.h
* file, in order to initialize all the (double/integer/string) variables of the
* Parameters structure of the \ref main function, pointed by the pParameters
* variable. In particular, the *.info file must contain at least (minimal
* configuration) the *.wfn/ *.chem (chemical) file, preceded by the 'name_chem'
* keyword, the (positive) number of electrons to look for, preceded by the
* 'nu_electrons' keyword, and optionally the *.mesh/ *.cube (mesh) file to
* start with, preceded by the 'name_mesh' keyword (if not specify a cube or a
* sphere is built depending on the ls_type variable of the Parameters
* structure).
*/
int loadParameters(Parameters* pParameters, char* nameInfo);

/**
* \fn int writingDefaultElasticFile(Parameters* pParameters)
* \brief It write a default *.elas file if such a file is needed and has not
*        been specified in the *.info (input) file.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*                        if its opt_mode variable is set to one or two,
*                        an *.elas file is needed by the elastic software, which
*                        must have been preivously installed. In the case where
*                        its name_elas variable is set to NULL, the \ref
*                        writingDefaultElasticFile function writes a default
*                        *.elas file using the Lame coefficients given in the
*                        corresponding \ref LAME_INT1, \ref LAME_EXT1, \ref
*                        LAME_INT2, \ref LAME_EXT2 preprocessor constants, and
*                        a default path name from the name_info variable
*                        (replacing the *.info extension by the *.elas one).
*                        Otherwise, an error is returned by the \ref
*                        writingDefaultElasticFile function.
*
* \return It returns one if the default *.elas file has been successfully
*         created and written (and the name_elas variable of the structure
*         pointed by pParameters successfully updated). Otherwise, zero is
*         returned if an error is encountered in the process.
*/
int writingDefaultElasticFile(Parameters* pParameters);

/**
* \fn int writingRestartFile(Parameters* pParameters)
* \brief It write a *.restart file that recalls the values of all parameters,
*        depending on the optimization mode chosen. It is a sort of exhaustive
*        *.info file (following the same syntax) that is used to save the
*        input data before starting the optimization loop in the MPD algorithm.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*                        Depending on its opt_mode variable, the unnecessary
*                        parameters are avoided whereas all the others are
*                        written in the *.restart file whose path name is the
*                        one of pParameters->name_info but where the '.info'
*                        extension has been replace by the '.restart' one.
*
* \return It returns one if the *.restart file has been successfully written.
*         Otherwise, zero is returned if an error is encountered in the process.
*/
int writingRestartFile(Parameters* pParameters);

#endif

