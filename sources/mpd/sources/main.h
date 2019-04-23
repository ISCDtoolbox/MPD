#ifndef DEF_MAIN
#define DEF_MAIN

/**
* \file main.h
* \brief Shared macros, structures, and prototypes of the MPD program.
* \author Jeremy DALPHIN
* \version 3.0
* \date May 1st, 2019
*
* This file contains the description of all the different macro functions,
* preprocessor constants, structures, and non-static function prototypes that
* are shared by all files in the MPD program.
*/

////////////////////////////////////////////////////////////////////////////////
// Standard header files to deal respectively with input/output, general basic
// functions, arrays of characters, date/time, different signals, complex
// numbers, math functions, LAPACK and OPENMP libraries
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <complex.h>

//#include <math.h>    // if it is not in comment add -lm with gcc command
//#include <lapacke.h> // if it is not in comment add -llapacke with gcc command

#ifdef _OPENMP
    #include <omp.h> // take it into account by adding -fopenmp with gcc command
#endif

////////////////////////////////////////////////////////////////////////////////
// Macro functions to initialize the diagnostic of an error, to get the
// minimum/maximum of two values, and also the absolute value of a double number
////////////////////////////////////////////////////////////////////////////////
/**
* \def PRINT_ERROR(stringToPrint)
* \brief Used to initialize the diagnostic of an error in the MPD program.
*
* It prints in the standard error stream the date, time, line, and file in which
* the error occurred. An additional stringToPrint message is also displayed,
* usually starting by the name of function in which the problem was found
* (__FUNCTION__ is not standard so it is not used in this program).
*/
#define PRINT_ERROR(stringToPrint)                                             \
do {                                                                           \
        fflush(stdout);                                                        \
        fprintf(stderr,"\nError encountered at line %d in ",__LINE__);         \
        fprintf(stderr,"%s file on %s",__FILE__,endTimerAtError());            \
        fprintf(stderr,"%s",stringToPrint);                                    \
} while (0)

/**
* \def DEF_MIN(a,b)
* \brief Used to compute the minimum of two values (integer/double).
*/
#define DEF_MIN(a,b) (((a) < (b)) ? (a) : (b))

/**
* \def DEF_MAX(a,b)
* \brief Used to compute the maximum of two values (integer/double).
*/
#define DEF_MAX(a,b) (((a) > (b)) ? (a) : (b))

/**
* \def DEF_ABS(a)
* \brief Used to compute the absolute value of a double number.
*/
#define DEF_ABS(a) (((a) > 0.0) ? (a) : (-(a)))

////////////////////////////////////////////////////////////////////////////////
// Preprocessor constants used to distinguish the different types of basis-set
// functions in the expansion of molecular orbitals into a sum of Gaussian-type
// primitives, according to the standard nomenclature (.wfn/.wfx format, outputs
// of Gaussian software)
////////////////////////////////////////////////////////////////////////////////
/**
* \def ORB_S
* \brief Used to distinguish the s-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_S 1

/**
* \def ORB_PX
* \brief Used to distinguish the px-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_PX 2

/**
* \def ORB_PY
* \brief Used to distinguish the py-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_PY 3

/**
* \def ORB_PZ
* \brief Used to distinguish the pz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_PZ 4

/**
* \def ORB_DXX
* \brief Used to distinguish the dxx-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_DXX 5

/**
* \def ORB_DYY
* \brief Used to distinguish the dyy-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_DYY 6

/**
* \def ORB_DZZ
* \brief Used to distinguish the dzz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_DZZ 7

/**
* \def ORB_DXY
* \brief Used to distinguish the dxy-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_DXY 8

/**
* \def ORB_DXZ
* \brief Used to distinguish the dxz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_DXZ 9

/**
* \def ORB_DYZ
* \brief Used to distinguish the dyz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_DYZ 10

/**
* \def ORB_FXXX
* \brief Used to distinguish the fxxx-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FXXX 11

/**
* \def ORB_FYYY
* \brief Used to distinguish the fyyy-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FYYY 12

/**
* \def ORB_FZZZ
* \brief Used to distinguish the fzzz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FZZZ 13

/**
* \def ORB_FXXY
* \brief Used to distinguish the fxxy-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FXXY 14

/**
* \def ORB_FXXZ
* \brief Used to distinguish the fxxz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FXXZ 15

/**
* \def ORB_FYYZ
* \brief Used to distinguish the fyyz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FYYZ 16

/**
* \def ORB_FXYY
* \brief Used to distinguish the fxyy-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FXYY 17

/**
* \def ORB_FXZZ
* \brief Used to distinguish the fxzz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FXZZ 18

/**
* \def ORB_FYZZ
* \brief Used to distinguish the fyzz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FYZZ 19

/**
* \def ORB_FXYZ
* \brief Used to distinguish the fxyz-type basis-set function.
*
* It follows the standard nomenclature (.wfn/.wfx format, outputs of Gaussian
* software) for basis-set functions in the expansion of molecular orbitals into
* a sum of Gaussian-type primitives.
*/
#define ORB_FXYZ 20

/*
// Not used in the current version of the MPD program (but we save the values
// here just in case they might be needed later)
#define ORB_GXXXX 21
#define ORB_GYYYY 22
#define ORB_GZZZZ 23
#define ORB_GXXXY 24
#define ORB_GXXXZ 25
#define ORB_GXYYY 26
#define ORB_GYYYZ 27
#define ORB_GXZZZ 28
#define ORB_GYZZZ 29
#define ORB_GXXYY 30
#define ORB_GXXZZ 31
#define ORB_GYYZZ 32
#define ORB_GXXYZ 33
#define ORB_GXYYZ 34
#define ORB_GXYZZ 35
#define ORB_HZZZZZ 36
#define ORB_HYZZZZ 37
#define ORB_HYYZZZ 38
#define ORB_HYYYZZ 39
#define ORB_HYYYYZ 40
#define ORB_HYYYYY 41
#define ORB_HXZZZZ 42
#define ORB_HXYZZZ 43
#define ORB_HXYYZZ 44
#define ORB_HXYYYZ 45
#define ORB_HXYYYY 46
#define ORB_HXXZZZ 47
#define ORB_HXXYZZ 48
#define ORB_HXXYYZ 49
#define ORB_HXXYYY 50
#define ORB_HXXXZZ 51
#define ORB_HXXXYZ 52
#define ORB_HXXXYY 53
#define ORB_HXXXXZ 54
#define ORB_HXXXXY 55
#define ORB_HXXXXX 56
*/

////////////////////////////////////////////////////////////////////////////////
// Definition of the structure storing the 77 parameters used in the algorithm
////////////////////////////////////////////////////////////////////////////////
/**
* \struct Parameters main.h
* \brief It can store all the different 77 parameters used in the MPD algorithm.
*/
typedef struct {
    int opt_mode;            /*!< This parameter rules the type of optimization
                             *    mode used in the MPD algorithm; positive
                             *    values concern tetrahedral meshes whereas
                             *    non-positive values concern hexahedral meshes;
                             *    if set to minus one/two,
                             *    shape-gradient/exhaustive perturbations are
                             *    performed on hexahedral meshes in order to
                             *    maximize the probability; if set to zero, the
                             *    algorithm starts with level-set (global)
                             *    perturbations, then changes to an exhaustive
                             *    (local) search when the hexahedral domain no
                             *    longer evolves; if set to four, then each new
                             *    tetrahedral domain represents the positive
                             *    part of the shape gradient associated
                             *    with the old one in the optimization loop; if
                             *    set to two/three, then an approach of
                             *    Eulerian(level-set)/Lagrangian type is
                             *    performed on tetrahedral meshes in order to
                             *    maximize the probability, by following the
                             *    shape gradient; if set to one, then all the
                             *    previous tetrahedral methods are combined in
                             *    order to get a converged MPD domain; other
                             *    values are forbidden. */

    int verbose;             /*!< If set to two, then it prints every execution
                             *    details in the standard output stream; if set
                             *    to one, then a little less is displayed;
                             *    otherwise, it must be set to zero and very
                             *    little is shown during the execution of the
                             *    MPD algorithm. */

    int n_cpu;               /*!< Number of cpu/threads required for the
                             *    parallelization by openmp in the MPD program
                             *    (it is also used by mshdist software, which
                             *    must have been previously installed): it must
                             *    be positive. */

    double rho_opt;          /*!< Only used if \ref opt_mode=1/2/3/4: scaling
                             *    factor that is multiplying the shape gradient
                             *    during the optimization algorithm. */


    // Parameters ruling the different file names
    int name_length;         /*!< Maximal length allowed for storing the
                             *    different file names: it must be (strictly)
                             *    greater than seven (to be able to store at
                             *    least a name that contains something more than
                             *    the *.input extension); for any \ref
                             *    name_input, \ref name_result, \ref name_chem,
                             *    \ref name_mesh, and \ref name_elas variable
                             *    that is not pointing to NULL, \ref name_length
                             *    should always correspond to the (common) size
                             *    of the array such a variable is pointing
                             *    to. */

    char* name_input;        /*!< Pointer used to dynamically define the array
                             *    storing the name of the *.input file that
                             *    contains the informations needed for filling
                             *    the Parameters structure; if it is not
                             *    pointing to NULL, the size of the array it is
                             *    pointing to should always correspond to the
                             *    \ref name_length value. */

    char* name_result;       /*!< Pointer used to dynamically define the array
                             *    storing the name of the (output) *.result file
                             *    that will contain a summary of the data saved
                             *    during the MPD algorithm (we refer to the
                             *    description of the Data structure for further
                             *    details); if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref name_length
                             *    value. */

    char* name_chem;         /*!< Pointer used to dynamically define the array
                             *    storing the name of the *.wfn/ *.chem file
                             *    containing the informations needed for filling
                             *    the ChemicalSystem structure; if it is not
                             *    pointing to NULL, the size of the array it is
                             *    pointing to should always correspond to the
                             *    the \ref name_length value. */

    char* name_mesh;         /*!< Pointer used to dynamically define the array
                             *    storing the name of the *.mesh/ *.cube file
                             *    that contains the informations needed for
                             *    filling the Mesh structure; if it is not
                             *    pointing to NULL, the size of the array it is
                             *    pointing to should always correspond to the
                             *    the \ref name_length value. */

    char* name_elas;         /*!< Only used if \ref opt_mode=one/two: pointer
                             *    used to dynamically define the array storing
                             *    the name of the *.elas file that is needed for
                             *    the command line of the elastic software,
                             *    which must have been previously installed; if
                             *    it is not pointing to NULL, the size of the
                             *    array it is pointing to should always
                             *    correspond to the \ref name_length value. */


    // Parameters ruling the system chemistry
    int nu_electrons;        /*!< The number of electrons to look for: it must
                             *    be positive (zero returns an error) and not
                             *    (strictly) greater than the total number of
                             *    electrons in the chemical system stored in
                             *    the \ref ne_electrons variable. */

//  int nu_spin;             /*!< If set to zero, the MPD program will maximize
//                           *    the probability to find exactly \ref
//                           *    nu_electrons in the domain; if set to a
//                           *    negative/positive value that is not (strictly)
//                           *    lower/greater than (-/+) \ref nu_electrons, it
//                           *    also computes the probability to find exactly
//                           *    \ref nu_electrons in the domain, with exactly
//                           *    |\ref nu_spin| electrons having spin down/up
//                           *    and \ref nu_electrons - |\ref nu_spin| having
//                           *    spin up/down; other values are forbidden. */

    int bohr_unit;           /*!< If set to one, then the unit used in the
                             *    *.wfn / *.chem file for locating the centers
                             *    of the nuclei is assumed to be in Bohrs;
                             *    otherwise it must be set to zero (and the
                             *    unit is Angstroms i.e. 1.e_10m). */

    double select_orb;       /*!< If set to a positive value lower than 0.01,
                             *    then a selection of the molecular orbitals
                             *    will be performed during the preprocessing
                             *    (iteration -1) to retain only those that are
                             *    acting inside the computational box with a
                             *    squared L2(R3)-value greater than the one
                             *    given; otherwise it must be set to zero and no
                             *    selection is done. */

    int orb_ortho;           /*!< If set to one, then the molecular orbitals
                             *    are expected to form an L2-orthonormal basis
                             *    basis set over the three-dimensional space;
                             *    otherwise it must be set to zero. */

    int ne_electrons;        /*!< The total number of electrons of the chemical
                             *    system considered inside the computational
                             *    box, which must be positive and equal to the
                             *    ne variable stored in the ChemicalSystem
                             *    structure. */

    int multi_det;           /*!< If set to one, then a multi-determinant wave
                             *    function is considered in the ChemicalSystem
                             *    structure; otherwise it must be set to
                             *    zero. */

    int orb_rhf;             /*!< If set to one, then it means that restricted
                             *    Hartree-Fock orbitals are used (and in
                             *    particular, \ref ne_electrons must be an even
                             *    number in this case), i.e. all molecular
                             *    orbitals have an occupation number that equals
                             *    two in the *.wfn file and must be duplicated;
                             *    otherwise, it must be set to zero. */


    // Parameters ruling the computational box (if a mesh file is not given)
    double select_box;       /*!< If set to zero, then the computational box is
                             *    a cube (\ref x_min,\ref x_max)x (\ref y_min,
                             *   \ref y_max)x(\ref z_min,\ref z_max) discretized
                             *    in (\ref n_x)x(\ref n_y)x(\ref n_z) points
                             *    with a space step precision of
                             *    (\ref delta_x)x(\ref delta_y)x(\ref delta_z);
                             *    if we have \ref select_box >= 0.9 and
                             *    \ref select_box < 1.0, then the default
                             *    computational box is built so that it contains
                             *    all the nuclei given in the .wfn/ *.chem files
                             *    and that the probability to find exactly the
                             *    total number of electrons inside the
                             *    computational box is greater than \ref
                             *    select_box (the theoretical value is one over
                             *    the whole space); if we have \ref
                             *    select_box > 0.0 and \ref select_box <= 0.1,
                             *    then the default computational box is built as
                             *    in the previous mode, but in addition a domain
                             *    maximizing the probability to find exactly the
                             *    total number of electrons is computed inside
                             *    it, up to a tolerance given by \ref
                             *    select_box, and this non-cubical domain is
                             *    finally extracted and considered as the
                             *    computational box at the end of the
                             *    preprocessing step (iteration -1) of the MPD
                             *    algorithm; other values are forbidden. */

    double x_min;            /*!< Minimal coordinate in the first-coordinate
                             *    direction: it must be (strictly) lower than
                             *    \ref x_max and \ref delta_x must be equal to
                             *    (\ref x_max - \ref x_min)/(\ref n_x - 1). */

    double y_min;            /*!< Minimal coordinate in the second-coordinate
                             *    direction: it must be (strictly) lower than
                             *    \ref y_max and \ref delta_y must be equal to
                             *    (\ref y_max - \ref y_min)/(\ref n_y - 1). */

    double z_min;            /*!< Minimal coordinate in the third-coordinate
                             *    direction: it must be (strictly) lower than
                             *    \ref z_max and \ref delta_z must be equal to
                             *    (\ref z_max - \ref z_min)/(\ref n_z - 1). */

    double x_max;            /*!< Maximal coordinate in the first-coordinate
                             *    direction: it must be (strictly) greater than
                             *    \ref x_min and \ref delta_x must be equal to
                             *    (\ref x_max - \ref x_min)/(\ref n_x - 1). */

    double y_max;            /*!< Maximal coordinate in the second-coordinate
                             *    direction: it must be (strictly) greater than
                             *    \ref y_min and \ref delta_y must be equal to
                             *    (\ref y_max - \ref y_min)/(\ref n_y - 1). */

    double z_max;            /*!< Maximal coordinate in the third-coordinate
                             *    direction: it must be (strictly) greater than
                             *    \ref z_min and \ref delta_z must be equal to
                             *    (\ref z_max - \ref z_min)/(\ref n_z - 1). */

    int n_x;                 /*!< Number of points in the first-coordinate
                             *    direction: it must be (strictly) greater than
                             *    two (in order to allow at least one
                             *    normal/tangent vector on the cube faces
                             *    corresponding to the computational box) and
                             *    delta_x must be equal to (\ref x_max - \ref
                             *    x_min)/(\ref n_x - 1). */

    int n_y;                 /*!< Number of points in the second-coordinate
                             *    direction: it must be (strictly) greater than
                             *    two (in order to allow at least one
                             *    normal/tangent vector on the cube faces
                             *    corresponding to the computational box) and
                             *    delta_y must be equal to (\ref y_max - \ref
                             *    y_min)/(\ref n_y - 1). */

    int n_z;                 /*!< Number of points in the third-coordinate
                             *    direction: it must be (strictly) greater than
                             *    two (in order to allow at least one
                             *    normal/tangent vector on the cube faces
                             *    corresponding to the computational box) and
                             *    delta_z must be equal to (\ref z_max - \ref
                             *    z_min)/(\ref n_z - 1). */

    double delta_x;          /*!< Discretization step in the first-coordinate
                             *    direction: it must be positive and \ref
                             *    delta_x must be equal to (\ref x_max - \ref
                             *    x_min)/(\ref n_x - 1). */

    double delta_y;          /*!< Discretization step in the second-coordinate
                             *    direction: it must be positive and \ref
                             *    delta_y must be equal to (\ref y_max - \ref
                             *    y_min)/(\ref n_y - 1). */

    double delta_z;          /*!< Discretization step in the third-coordinate
                             *    direction: it must be positive and \ref
                             *    delta_z must be equal to (\ref z_max - \ref
                             *    z_min)/(\ref n_z - 1). */


    // Parameters ruling the level-set function of the initial domain
    int ls_type;             /*!< If set to zero, then the initial domain is a
                             *    cube of center (\ref ls_x, \ref ls_y, \ref
                             *    ls_z) and size \ref ls_r; otherwise, it must
                             *    be set to one and the initial domain is a
                             *    sphere (same center and radius \ref ls_r). */

    double ls_x;             /*!< First coordinate of the center of the initial
                             *    cube/sphere. */

    double ls_y;             /*!< Second coordinate of the center of the initial
                             *    cube/sphere. */

    double ls_z;             /*!< Third coordinate of the center of the initial
                             *    cube/sphere. */

    double ls_r;             /*!< Length/Radius of the initial cube/sphere:
                             *    it must be positive. */


    // Only used if opt_mode=1/2/3/4: parameters ruling the metric computation
    double met_err;          /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    tolerance allowed on the error made during the
                             *    computation of the adaptation metric; it must
                             *    be positive. */

    double met_min;          /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    minimum length allowed for edges in the
                             *    computation of the adaptation metric; it must
                             *    be positive and (strictly) lower than \ref
                             *    met_max. */

    double met_max;          /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    maximum length allowed for edges in the
                             *    computation of the adaptation metric; it must
                             *    be (strictly) greater than \ref met_min. */


    // Only used if opt_mode=-2/-1/0: parameters associated with the
    // overlap-matrix computation
    int trick_matrix;        /*!< Only used if \ref opt_mode=-(two/one/zero): if
                             *    set to zero, then the overlap matrix is
                             *    completely recomputed at each iteration;
                             *    otherwise, it must be set to one and a trick
                             *    is performed at each iteration (except for the
                             *    first one) on the hexahedra's labels so as to
                             *    compute only the difference between the old
                             *    overlap-matrix coefficients and the new
                             *    ones. */

    int approx_mode;         /*!< Only used if \ref opt_mode=-(two/one/zero): if
                             *    set to zero, then the overlap matrix is
                             *    computed using exact values; otherwise, it
                             *    must be set to one and the overlap matrix is
                             *    evaluated using approximated values (volume
                             *    times the integrand value at the cube element
                             *    center). */


    // Parameters ruling the stop criteria in the optimization loop
    int iter_ini;            /*!< A non-negative integer referring to the
                             *    iteration at which the optimization loop will
                             *    start (in case the MPD program is restarted
                             *    with a different optimization mode for
                             *    example): it must not be greater than the \ref
                             *    iter_max variable. */

    int iter_max;            /*!< Maximum number of iterations allowed in the
                             *    optimization loop: it must not be negative
                             *    (zero means that no optimization loop is
                             *    performed) and not lower than the \ref
                             *    iter_ini variable. */

    double iter_told0p;      /*!< Tolerance allowed between the probabilities
                             *    of two successive iterations: it must not be
                             *    negative (zero means that it is not taken into
                             *    account in the stop criteria of the
                             *    optimization loop). */

    double iter_told1p;      /*!< Tolerance allowed on the shape derivative
                             *    taken in the direction of the shape gradient,
                             *    whose zero value represents the first-order
                             *    optimality condition of the shape optimization
                             *    problem: it must not be  negative (zero means
                             *    that it is not taken into account in the stop
                             *    criteria of the optimization loop). */

    double iter_told2p;      /*!< Tolerance allowed between the shape
                             *    derivatives of two successive iterations (if
                             *    \ref opt_mode=-(two/one/zero), between the
                             *    second-order probability differences of two
                             *    successive iterations): it must not be
                             *    negative (zero means that it is not taken into
                             *    account in the stop criteria of the
                             *    optimization loop). */


    // Parameters ruling the saving of data in the optimization loop
    int save_type;           /*!< If set to one, then the mesh is saved using
                             *    the *.mesh format; if set to zero, then the
                             *    mesh is saved using the *.cube/ *.obj format
                             *    (*.cube for hexahedral meshes and *.obj for
                             *    tetrahedral ones); otherwise, it must be set
                             *    to two, and in this case, the mesh is saved
                             *    in both *.cube/*.obj and *.mesh format. */

    int save_mesh;           /*!< Frequency at which the mesh is saved in the
                             *    optimization loop: it cannot be negative
                             *    (zero means that the mesh is never saved); we
                             *    recall that in any case, the initial mesh
                             *    (iteration 0) and the current mesh are always
                             *    saved in the *.mesh format so that even if the
                             *    MPD algorithm crashes, the program can be
                             *    lauched again thanks to these meshes and the
                             *    *.restart file. */

    int save_data;           /*!< Frequency at which the data are saved in the
                             *    the optimization loop (we refer to the Data
                             *    structure for further details): it must not be
                             *    negative (zero means data are never saved). */

    int save_print;          /*!< Frequency at which the mesh is displayed with
                             *    medit (warning: the software must have been
                             *    previously installed and every time that medit
                             *    is loaded, the algorithm will stop until exit
                             *    is performed): it must not be negative (zero
                             *    means that the mesh is never plotted); we also
                             *    recall that in the case of a positive value, a
                             *    manual confirmation is asked if a default
                             *    computational box has to be built. */

    int save_where;          /*!< Integer referring to the part of the
                             *    optimization loop that \ref save_print is
                             *    vizualizing (all allowed values can be used
                             *    if \ref opt_mode=one/two; in other modes, only
                             *    certain values should be used depending on
                             *    what kind of data is available or not):
                             *    one=metric, two=level-set function,
                             *    three/four=shape gradient (scalar/vectorial
                             *    form), five=extension of the shape gradient,
                             *    six=advection of the level-set function, and
                             *    seven=advected domain; other values are
                             *    forbidden. */

    // Parameters ruling the path names for the external softwares needed in
    // the algorithm (warning: medit, mmg3d, mshdist, elastic and advect must
    // have been previously installed)
    int path_length;         /*!< Maximal length allowed for storing the
                             *    different path names: it must be (strictly)
                             *    greater than one (to be able to store at least
                             *    something more than the terminating nul
                             *    character '\0'); for any \ref path_medit, \ref
                             *    path_mmg3d, \ref path_mshdist, \ref
                             *    path_elastic, \ref path_advect variable that
                             *    is not pointing to NULL, \ref path_length
                             *    should always correspond to the (common) size
                             *    of the array such a variable is pointing
                             *    to. */

    char* path_medit;        /*!< Pointer used to dynamically define the array
                             *    storing the path specification for the medit
                             *    software which must have been previously
                             *    installed; if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref path_length
                             *    value. */

    char* path_mmg3d;        /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    pointer used to dynamically define the array
                             *    storing the path specification for the mmg3d
                             *    software which must have been previously
                             *    installed; if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref path_length
                             *    value. */

    char* path_mshdist;      /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    pointer used to dynamically define the array
                             *    storing the path specification for the mshdist
                             *    software which must have been previously
                             *    installed; if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref path_length
                             *    value. */

    char* path_elastic;      /*!< Only used if \ref opt_mode=one/two: pointer
                             *    used to dynamically define the array storing
                             *    the path specification for the elastic
                             *    software which must have been previously
                             *    installed; if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref path_length
                             *    value. */

    char* path_advect;       /*!< Only used if \ref opt_mode=one/two: pointer
                             *    used to dynamically define the array storing
                             *    the path specification for the advect
                             *    software which must have been previously
                             *    installed; if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref path_length
                             *    value. */


    // Only used if opt_mode=1/2/3/4: parameters used in mmg3d to build the
    // default isotropic metric (warning: mmg3d software must have been
    // previously installed)
    double hmin_iso;         /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    minimum length allowed for edges in the
                             *    computation of the default isotropic
                             *    adaptation metric by mmg3d software, which
                             *    must have been previously installed; it must
                             *    be positive and (strictly) lower than \ref
                             *    hmax_iso. */

    double hmax_iso;         /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    maximum length allowed for edges in the
                             *    computation of the default isotropic
                             *    adaptation metric by mmg3d software, which
                             *    must have been previously installed; it must
                             *    be (strictly) greater than \ref hmin_iso. */

    double hausd_iso;        /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    tolerance allowed on the error measured by the
                             *    Hausdorff distance between the adapted surface
                             *    and its ideal representation during the
                             *    computation of the default isotropic
                             *    adaptation metric by mmg3d software, which
                             *    must have been previously installed; it must
                             *    be positive. */

    double hgrad_iso;        /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    tolerance allowed for the (gradation) ratio
                             *    between any two edge lengths sharing one point
                             *    together during the computation of the default
                             *    isotropic adaptation metric by mmg3d software,
                             *    which must have been previously installed; it
                             *    must be (strictly) greater than one. */


    // Only used if opt_mode=1/2/3/4: parameters used in mmg3d for creating the
    // metric related to the molecular orbitals (warning: mmg3d software must
    // have been previously installed)
    double hmin_met;         /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    minimum length allowed for edges in the
                             *    computation of the molecular-orbitals
                             *    adaptation metric by mmg3d software, which
                             *    must have been previously installed; it must
                             *    be positive and (strictly) lower than \ref
                             *    hmax_met. */

    double hmax_met;         /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    maximum length allowed for edges in the
                             *    computation of the molecular-orbitals
                             *    adaptation metric by mmg3d software, which
                             *    must have been previously installed; it must
                             *    be (strictly) greater than \ref hmin_met. */

    double hausd_met;        /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    tolerance allowed on the error measured by the
                             *    Hausdorff distance between the adapted surface
                             *    and its ideal representation during the
                             *    computation of the molecular-orbitals
                             *    adaptation metric by mmg3d software, which
                             *    must have been previously installed; it must
                             *    be positive. */

    double hgrad_met;        /*!< Only used if \ref opt_mode=one/two/three/four:
                             *    tolerance allowed for the (gradation) ratio
                             *    between any two edge lengths sharing one point
                             *    together during the computation of the
                             *    molecular-orbitals adaptation metric by mmg3d
                             *    software, which must have been previously
                             *    installed; it must be (strictly) greater than
                             *    one. */


    // Only used if opt_mode=1/2/4: parameters used in the modified version of
    // mmg3d for creating the metric associated with the level-set function but
    // also respecting the orbitals' metric (warning: the modified version of
    // the mmg3d software must have been previously installed; its source files
    // are joined with the ones of the MPD program)
    double hmin_ls;          /*!< Only used if \ref opt_mode=one/two/four:
                             *    minimum length allowed for edges in the
                             *    computation of both level-set function and
                             *    molecular-orbitals adaptation metric by the
                             *    modified version of mmg3d software, which must
                             *    have been previously installed (its source
                             *    files are joined with the ones of the MPD
                             *    program); it must be positive and (strictly)
                             *    lower than \ref hmax_ls. */

    double hmax_ls;          /*!< Only used if \ref opt_mode=one/two/four:
                             *    maximum length allowed for edges in the
                             *    computation of both level-set function and
                             *    molecular-orbitals adaptation metric by the
                             *    modified version of mmg3d software, which must
                             *    have been previously installed (its source
                             *    files are joined with the ones of the MPD
                             *    program); it must be (strictly) greater than
                             *    \ref hmin_ls. */

    double hausd_ls;         /*!< Only used if \ref opt_mode=one/two/four:
                             *    tolerance allowed on the error measured by the
                             *    Hausdorff distance between the adapted surface
                             *    and its ideal representation during the
                             *    computation of both level-set function and
                             *    molecular-orbitals adaptation metric by the
                             *    modified version of mmg3d software, which must
                             *    have been previously installed (its source
                             *    files are joined with the ones of the MPD
                             *    program); it must be positive. */

    double hgrad_ls;         /*!< Only used if \ref opt_mode=one/two/four:
                             *    tolerance allowed for the (gradation) ratio
                             *    between any two edge lengths sharing one point
                             *    together during the computation of both
                             *    level-set function and molecular-orbitals
                             *    adaptation metric by the modified version of
                             *    mmg3d software, which must have been
                             *    previously installed (its source files are
                             *    joined with the ones of the MPD program); it
                             *    must be (strictly) greater than one. */


    // Only used if opt_mode=1/3: parameters used in mmg3d for creating the
    // metric in the Lagrangian mode (warning: mmg3d software must have been
    // previously installed)
    int hmode_lag;           /*!< Only used if \ref opt_mode=one/three: type of
                             *    Lagrangian mode used in the computation of
                             *    the adaptation metric in Lagrangian mode by
                             *    the mmg3d software, which must have been
                             *    previously installed; it can be set to zero
                             *    (no local remeshing after displacements),
                             *    one (swap of distorded edges and relocation
                             *    of vertices for distorded tetrahedra), or two
                             *    (idem as the previous (one-)mode but points
                             *    are previously splitted and collapsed if
                             *    necessary); other values are forbidden. */

    double hmin_lag;         /*!< Only used if \ref opt_mode=one/three: minimum
                             *    length allowed for edges in the computation
                             *    of the adaptation metric in Lagrangian mode by
                             *    mmg3d software, which must have been
                             *    previously installed; it must be positive and
                             *    (strictly) lower than \ref hmax_met. */

    double hmax_lag;         /*!< Only used if \ref opt_mode=one/three: maximum
                             *    length allowed for edges in the computation
                             *    of the adaptation metric in Lagrangian mode by
                             *    mmg3d software, which must have been
                             *    previously installed; it must be (strictly)
                             *    greater than \ref hmin_met. */

    double hausd_lag;        /*!< Only used if \ref opt_mode=one/three:
                             *    tolerance allowed on the error measured by the
                             *    Hausdorff distance between the adapted surface
                             *    and its ideal representation during the
                             *    computation of the adaptation metric in
                             *    Lagrangian mode by mmg3d software, which must
                             *    have been previously installed; it must be
                             *    positive. */

    double hgrad_lag;        /*!< Only used if \ref opt_mode=one/three:
                             *    tolerance allowed for the (gradation) ratio
                             *    between any two edge lengths sharing one point
                             *    together during the computation of the
                             *    adaptation metric in Lagrangian mode by mmg3d
                             *    software, which must have been previously
                             *    installed; it must be (strictly) greater than
                             *    one. */


    // Only used if opt_mode=1/2/4: additional parameters needed for the command
    // line of mshdist and advect softwares in the MPD algorithm (warning:
    // mshdist and advect softwares must have been previously installed)
    int n_iter;              /*!< Only used if \ref opt_mode=one/two/four: in
                             *    mshdist command line (software must have been
                             *    previously installed), it represents the
                             *    maximum number of iterations allowed in the
                             *    iterative renormalisation of the signed
                             *    distance function; it must not be negative
                             *    (zero means that no limit is fixed). */

    double residual;         /*!< Only used if \ref opt_mode=one/two/four: in
                             *    mshdist command line (software must have been
                             *    previously installed), it represents the
                             *    tolerance allowed on the residual, i.e. the
                             *    value below which the convergence is
                             *    considered to be achieved; it must not be
                             *    negative (zero means no limit is fixed). */

    double delta_t;          /*!< Only used if \ref opt_mode=one/two: in advect
                             *    command line (software must have been
                             *    previously installed), it represents the final
                             *    time at which the advection equation is
                             *    expected to be solved: it must be positive. */

    int no_cfl;              /*!< Only used if \ref opt_mode=one/two: in advect
                             *    command line (software must have been
                             *    previously installed), if it is set to zero,
                             *    then a cfl condition is computed, restricting
                             *    the final time in the advection equation;
                             *    otherwise, it must be set to one and no
                             *    restriction is performed (but instabilities
                             *    can appear and alter the final solution). */
} Parameters;

////////////////////////////////////////////////////////////////////////////////
// Definitions of the different chemical structures used in the MPD program
////////////////////////////////////////////////////////////////////////////////
/**
* \struct Nucleus main.h
* \brief It can store the position and charge of a nucleus.
*/
typedef struct {
    double x;                /*!< First coordinate of the nucleus center. */
    double y;                /*!< Second coordinate of the nucleus center. */
    double z;                /*!< Third coordinate of the nucleus center. */
    int charge;              /*!< Charge number associated with the nucleus;
                             *    its value should always be positive and not
                             *    (strictly) greater than one hundred. */
} Nucleus;

/**
* \struct MolecularOrbital main.h
* \brief It can store the spin and the basis-set informations of a molecular
*        orbital (i.e. the coefficients, exponents, nuclei, and types defining
*        its Gaussian-type primitives).
*/
typedef struct {
    int spin;                /*!< Spin associated with the molecular orbital;
                             *    its value should be only one (spin up) or
                             *    minus one (spin down). */

    double* coeff;           /*!< Pointer used to dynamically define the array
                             *    storing the coefficients related to the
                             *    expansion of the molecular orbital into a sum
                             *    of (Gaussian-type) primitives; if it is not
                             *    pointing to NULL, the size of the array it is
                             *    pointing to should always correspond to the
                             *    nprim variable of the ChemicalSystem
                             *    structure. */

    double* exp;             /*!< Pointer used to dynamically define the array
                             *    storing the exponents related to the
                             *    expansion of the molecular orbital into a sum
                             *    of (Gaussian-type) primitives; if it is not
                             *    pointing to NULL, the values of the array it
                             *    pointing to should always be positive and its
                             *    size should always correspond to the nprim
                             *    variable of the ChemicalSystem structure. */

    int* nucl;               /*!< Pointer used to dynamically define the array
                             *    storing the center reference of the nuclei
                             *    (i.e. their indices in the array pointed by
                             *    the pnucl variable of the ChemicalSystem
                             *    structure) related to the expansion of the
                             *    molecular orbital into a sum of
                             *    (Gaussian-type) primitives; if it is not
                             *    pointing to NULL, the values of the array it
                             *    pointing to should always be positive and not
                             *    (strictly) greater than the total number of
                             *    nuclei in the chemical system (stored in the
                             *    nnucl variable of the ChemicalSystem
                             *    structure), while its size should always
                             *    correspond to the nprim variable of the
                             *    ChemicalSystem structure. */

    int* type;               /*!< Pointer used to dynamically define the array
                             *    storing the type of Gaussian functions related
                             *    to the expansion of the molecular orbital into
                             *    a sum of (Gaussian-type) primitives; if it is
                             *    not pointing to NULL, the values of the array
                             *    it is pointing to should always be positive
                             *    and not (strictly) greater than twenty (i.e. a
                             *    value referenced between \ref ORB_S and \ref
                             *    ORB_FXYZ), while its size should always
                             *    correspond to the nprim variable of the
                             *    ChemicalSystem structure. */

    int ngauss;              /*!< Number of non-zero primitives associated with
                             *    the current MolecularOrbital structure, which
                             *    should always correspond to the size of the
                             *    array \ref pgauss is pointing to: it should
                             *    always be positive and not greater than the
                             *    total number of primitives (nprim variable)
                             *    stored in the ChemicalSystem structure. */

    int* pgauss;             /*!< Pointer used to dynamically define the array
                             *    storing the position of the non-zero
                             *    primitives in the arrays pointed by \ref
                             *    coeff, \ref exp, \ref nucl, and \ref type;
                             *    if it is not pointing to NULL, its size should
                             *    always be given by the \ref ngauss value. */
} MolecularOrbital;

/**
* \struct Determinant main.h
* \brief It can store all the informations needed to build a determinant
*        of molecular orbitals that can appear in the wave function of the
*        chemical system (pre-factor, molecular orbitals concerned).
*/
typedef struct {
    int rhf;                 /*!< If set to one, then it means that the current
                             *    Determinant structure admits a restricted
                             *    Hartree-Fock-like simplification; otherwise,
                             *    it must be set to zero. */

    double cdet;             /*!< Coefficient acting as a multiplicating
                             *    pre-factor of the Determinant structure in
                             *    the decomposition of the wave function as a
                             *    finite linear combinaison of determinants. */

    int* vmorb;               /*!< Pointer used to dynamically define the array
                              *    storing the vector of molecular orbitals
                              *    constituting the Determinant structure
                              *    referred to as their positions in the array
                              *    pointed by the pmorb variable of the
                              *    ChemicalSystem structure; if it is not
                              *    pointing to NULL, its size should always be
                              *    given by the ne variable of the
                              *    ChemicalSystem structure. */
} Determinant;

/**
* \struct OverlapMatrix main.h
* \brief It can store the principal informations (coefficients, eigenvalues,
*        eigenvectors) related to the different generalized overlap matrices.
*/
typedef struct {
    int nmat;                /*!< Positive integer referring to the size of the
                             *    (nmat)x(nmat)-matrix the current OverlapMatrix
                             *    structure is referring to. */

    int rhf;                 /*!< If set to one, then it means that the current
                             *    OverlapMatrix structure admits a restricted
                             *    Hartree-Fock-like simplification  (and in
                             *    particular, \ref nmat must be an even
                             *    number in this case); otherwise, it must be
                             *    set to zero. */

    double cmat;             /*!< Coefficient acting as a multiplicating
                             *    pre-factor in the decomposition of the
                             *    probability as a finite linear combinaison
                             *    of determinants of generalized overlap
                             *    matrices. */

    double* coef;            /*!< Pointer used to dynamically define the array
                             *    storing the (real) coefficients of the
                             *    current OverlapMatrix structure; if it
                             *    not pointing to NULL, the size of the array it
                             *    is pointing to should always correspond to
                             *    the (\ref nmat )x(\ref nmat ) value.*/

    double complex* diag;    /*!< Pointer used to dynamically define the array
                             *    storing the (complex) eigenvalues of the
                             *    current OverlapMatrix structure (according to
                             *    the generalized Schur decomposition); if it
                             *    not pointing to NULL, the size of the array it
                             *    is pointing to should always correspond to
                             *    the \ref nmat value.*/

    double complex* lvect;   /*!< Pointer used to dynamically define the array
                             *    storing the left eigenvectors of the current
                             *    OverlapMatrix structure (according to the
                             *    generalized Schur decomposition); if it not
                             *    pointing to NULL, the size of the array it is
                             *    pointing to should always correspond to the
                             *    (\ref nmat )x(\ref nmat ) value.*/

    double complex* rvect;   /*!< Pointer used to dynamically define the array
                             *    storing the right eigenvectors of the current
                             *    OverlapMatrix structure (according to the
                             *    generalized Schur decomposition); if it not
                             *    pointing to NULL, the size of the array it is
                             *    pointing to should always correspond to the
                             *    (\ref nmat )x(\ref nmat ) value.*/
} OverlapMatrix;

/**
* \struct ChemicalSystem main.h
* \brief It can store all the chemical informations of the electronic system
*        needed for the MPD algorithm (number of nucleus, primitives and
*        molecular orbitals; description of nuclei, molecular orbitals, and
*       determinants constituting the wave function).
*/
typedef struct {
    int nnucl;               /*!< Number of nuclei in the chemical system: it
                             *    should always be positive; if \ref pnucl is
                             *    not pointing to NULL, \ref nnucl should always
                             *    correspond to the size of the array \ref
                             *    pnucl is pointing to. */

    Nucleus* pnucl;          /*!< Pointer used to dynamically define the array
                             *    storing the nuclei associated with the
                             *    chemical system; if it is not pointing to
                             *    NULL, its size should always be given by the
                             *    \ref nnucl value. */

    int nprim;               /*!< Number of primitives used in the expansion of
                             *    the molecular orbitals into a sum of
                             *    (Gaussian-type) primitives: it should always
                             *    be positive; if \ref pmorb is not pointing to
                             *    NULL, for any coeff, exp, nucl, and type
                             *    variable of the MolecularOrbital structures
                             *    that is not pointing to NULL and stored in the
                             *    array pointed by \ref pmorb, \ref nprim
                             *    should always correspond to the (common) size
                             *    of the array such a variable is pointing
                             *    to. */

    int nmorb;               /*!< Number of molecular orbitals associated with
                             *    the chemical system, which also corresponds
                             *    here to the total number of electrons in the
                             *    chemical system; it should always be positive;
                             *    if \ref pmorb is not pointing to NULL, \ref
                             *    nmorb should always correspond to the size of
                             *    the array \ref pmorb is pointing to. */

    MolecularOrbital* pmorb; /*!< Pointer used to dynamically define the array
                             *    storing the molecular orbitals associated with
                             *    the chemical system; if it is not pointing to
                             *    NULL, its size should always be given by the
                             *    \ref nmorb value. */

    int ne;                  /*!< The total number of electrons of the chemical
                             *    system considered inside the computational
                             *    box, which must be positive. */

    int nu;                  /*!< The number of electrons to look for: it must
                             *    be positive and not greater than \ref ne. */

    int ndet;                /*!< Number of determinants that constitutes the
                             *    wave function, which should always be positive
                             *    and correspond to the size of the array \ref
                             *    pdet is pointing to. */

    Determinant* pdet;       /*!< Pointer used to dynamically define the array
                             *    storing the determinants of the wave function
                             *    that is characterizing the chemical system;
                             *    if it is not pointing to NULL, its size should
                             *    always be given by the \ref ndet value. */

    OverlapMatrix* pmat;     /*!< Pointer used to dynamically define the array
                             *    storing all the informations related to the
                             *    different generalized overlap matrices
                             *    computed over the whole real three-dimensional
                             *    space; if it is not pointing to NULL, its size
                             *    should always be given by the
                             *    (\ref ndet )x(\ref ndet ) value. */
} ChemicalSystem;

////////////////////////////////////////////////////////////////////////////////
// Definition of the structures used to save the data in the MPD algorithm
////////////////////////////////////////////////////////////////////////////////
// /**
//* \struct Probabilities main.h
//* \brief It can store all the different types of probabilities to find exactly
//*        a certain number of electrons in a given domain.
// */
//typedef struct {
//  double* pk;              /*!< Pointer used to dynamically define the array
//                           *    storing at the k-th position the probability
//                           *    to find exactly k electrons in a given domain;
//                           *    if it is not pointing to NULL, the values of
//                           *    the array it is pointing to should never be
//                           *    negative and (strictly) greater than one,
//                           *    while its size should always correspond to the
//                           *    nprob variable of the Data structure. */
//
//  double* pkl;             /*!< Pointer used to dynamically define the array
//                           *    storing the matrix coefficients whose (k,l)-th
//                           *    one represents the (spin-dependant)
//                           *    probability to find exactly (k+l) electrons
//                           *    in a given domain, with exactly k spin-up and
//                           *    l spin-down ones; if it is not pointing to
//                           *    NULL, the values of the array it is pointing
//                           *    to should never be negative and (strictly)
//                           *    greater than one, while its size should always
//                           *    correspond to the (nprob)x(nprob) value, where
//                           *    the nprob variable refers to the one of
//                           *    the Data structure. */
//} Probabilities;

/**
* \struct Data main.h
* \brief It can store all different types of data saved by the MPD algorithm.
*/
typedef struct {
    int ndata;               /*!< Maximum number of iterations saved during the
                             *    optimization algorithm: it should always be
                             *    positive and correspond to the maximum number
                             *    of iterations allowed in the optimization loop
                             *    of the MPD algorithm (stored in the iter_max
                             *    variable of the Parameters structure); for any
                             *    \ref pnu, \ref pop, \ref vol, \ref d0p, \ref
                             *    d1p, \ref d2p, \ref tim, \ref ctim, \ref
                             *    pprob, and \ref ppmat variable that is not
                             *    pointing to NULL, \ref ndata should always
                             *    correspond to the (common) size of the array
                             *    such a variable is pointing to. */

    int niter;               /*!< Number of iterations properly saved in the
                             *    Data Structure since the MPD program is
                             *    usually intended to end with a converged
                             *    domain before the maximum number of iterations
                             *    is reached in the optimization loop; it should
                             *    always be positive and not (strictly) greater
                             *    than the \ref ndata value. */

    double* pnu;             /*!< Pointer used to dynamically define the array
                             *    storing the probability to find exactly the
                             *    number of electrons prescribed by the user
                             *    (stored in the nu_electrons of the Parameters
                             *    structure) in the domain during the iterative
                             *    process; if it is not pointing to NULL, the
                             *    values of the array it is pointing to should
                             *    never be negative and (strictly) greater than
                             *    one, while its size should always correspond
                             *    to the \ref ndata value. */

    double* pop;             /*!< Pointer used to dynamically define the array
                             *    storing the total population of electrons
                             *    inside the domain during the iterative
                             *    process; if it is not pointing to NULL, the
                             *    values of the array it is pointing to should
                             *    never be negative and (strictly) greater than
                             *    the total number of electrons in the chemical
                             *    system (stored in the ne variable of the
                             *    ChemicalSystem structure), while its size
                             *    should always correspond to the \ref ndata
                             *    value. */

    double* vol;             /*!< Pointer used to dynamically define the array
                             *    storing the volume of the domain taken inside
                             *    the computational box during the iterative
                             *    process; if it is not pointing to NULL, the
                             *    values of the array it is pointing to should
                             *    never be negative, while its size should
                             *    always correspond to the \ref ndata value. */

    double* d0p;             /*!< Pointer used to dynamically define the array
                             *    storing the (absolute) probability difference
                             *    during the iterative process; if it is not
                             *    pointing to NULL, the values of the array it
                             *    is pointing to should always be (strictly)
                             *    greater than the iter_told0p variable of the
                             *    Parameters structure, and its size should
                             *    always correspond to the \ref ndata value. */

    double* d1p;             /*!< Pointer used to dynamically define the array
                             *    storing the values of the shape derivative
                             *    (taken in the shape gradient direction)
                             *    associated with the probability during the
                             *    iterative process; if it is not
                             *    pointing to NULL, the values of the array it
                             *    is pointing to should always be (strictly)
                             *    greater than the iter_told1p variable of the
                             *    Parameters structure, and its size should
                             *    always correspond to the \ref ndata value. */

    double* d2p;             /*!< Pointer used to dynamically define the array
                             *    storing the (absolute) shape derivative
                             *    difference during the iterative process; its
                             *    values should always be positive; if it is not
                             *    pointing to NULL, the values of the array it
                             *    is pointing to should always be (strictly)
                             *    greater than the iter_told2p variable of the
                             *    Parameters structure, and its size should
                             *    always correspond to the \ref ndata value. */

    double* tim;             /*!< Pointer used to dynamically define the array
                             *    storing the computational time needed to
                             *    perform the iteration in the optimization
                             *    loop; if it is not pointing to NULL, the
                             *    values of the array it is pointing to should
                             *    always be positive and its size should
                             *    always correspond to the \ref ndata value. */

    double* ctim;            /*!< Pointer used to dynamically define the array
                             *    storing the cumulated computational time
                             *    needed to perform the algorithm until the
                             *    iteration considered in the optimization
                             *    loop; if it is not pointing to NULL, the
                             *    values of the array it is pointing to should
                             *    always be positive and its size should
                             *    always correspond to the \ref ndata value. */

    int nprob;               /*!< Common size the arrays pointed by the \ref
                             *    pprob variable (if not set to NULL); it should
                             *    always be positive and  correspond to the
                             *    total number of electrons plus one (the total
                             *    number of electrons is stored in the ne
                             *    variable of the ChemicalSystem structure). */

    double** pprob;          /*!< Pointer used to dynamically define an array of
                             *    probability array during the iterative
                             *    process, each one storing at the k-th position
                             *    the probability to find exactly k electrons
                             *    inside the current domain; if it is not
                             *    pointing to NULL, the values stored in
                             *    the arrays it is pointing to should never be
                             *    negative and (strictly) greater than one,
                             *    while its size should always correspond to the
                             *    \ref ndata value. */

//  Probabilities* pprob;    /*!< Pointer used to dynamically define the array
//                           *    storing all the different types of
//                           *    probabilities during the iterative process; if
//                           *    it is not pointing to NULL, the size of the
//                           *    array it is pointing to should always
//                           *    correspond to the \ref ndata value. */
//
//  int nmat;                /*!< Common value used to size the arrays pointed
//                           *    by the non-NULL variables coef, diag, lvect
//                           *    and rvect of the OverlapMatrix structures
//                           *    stored in the array pointed by \ref pmat (if
//                           *    \ref pmat is not NULL); it should always be
//                           *    positive and correspond to the value
//                           *    (ndet)x(ndet)x(ne)x(ne) where the ndet and ne
//                           *    variables are stored in the ChemicalSystem
//                           *    structure). */

    OverlapMatrix** ppmat;   /*!< Pointer used to dynamically define an array of
                             *    OverlapMatrix array during the iterative
                             *    process, each one storing all the informations
                             *    related to the different generalized overlap
                             *    matrices; its size should always correspond to
                             *    the \ref ndata value. */
} Data;

////////////////////////////////////////////////////////////////////////////////
// Definitions of the structures used to store the mesh in the MPD program
////////////////////////////////////////////////////////////////////////////////
/**
* \struct Point main.h
* \brief It can store the position, label and a value associated with a point.
*/
typedef struct {
    double x;                /*!< First coordinate of the point. */
    double y;                /*!< Second coordinate of the point. */
    double z;                /*!< Third coordinate of the point. */
    int label;               /*!< Integer value associated with the point. */
    double value;            /*!< Real value associated with the point. */
} Point;

// Only used if opt_mode=1/2/3/4
/**
* \struct Vector main.h
* \brief It can store the point reference and the direction of a vector (only
*        used if opt_mode=one/two/three/four in the Parameters structure).
*/
typedef struct {
    int p;                   /*!< Integer referring to the point at which the
                             *    vector is expressed i.e. its position in the
                             *    array associated with the vertices of the
                             *    mesh: its value should always be positive and
                             *    not (strictly) greater than the total number
                             *    of mesh vertices (stored in the nver variable
                             *    of the Mesh structure). */

    double x;                /*!< First coordinate of the vector. */
    double y;                /*!< Second coordinate of the vector. */
    double z;                /*!< Third coordinate of the vector. */
} Vector;

// Only used if opt_mode=1/2/3/4
/**
* \struct Edge main.h
* \brief It can store the label and two point references of an edge (only
*        used if opt_mode=one/two/three/four in the Parameters structure).
*/
typedef struct {
    int p1;                  /*!< First point of the edge referred to as its
                             *    position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p2;                  /*!< Second point of the edge referred to as its
                             *    position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int label;               /*!< Integer value associated with the edge. */
} Edge;

// Only used if opt_mode=1/2/3/4
/**
* \struct Triangle main.h
* \brief It can store the label and three point references of a triangle (only
*        used if opt_mode=one/two/three/four in the Parameters structure).
*/
typedef struct {
    int p1;                  /*!< First point of the triangle referred to as its
                             *    position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p2;                  /*!< Second point of the triangle referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p3;                  /*!< Third point of the triangle referred to as its
                             *    position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int label;               /*!< Integer value associated with the triangle. */
} Triangle;

// Only used if opt_mode=1/2/3/4
/**
* \struct Tetrahedron main.h
* \brief It can store the label and four point references of a tetrahedron
*        (only used if opt_mode=one/two/three/four in the Parameters structure).
*/
typedef struct {
    int p1;                  /*!< First point of the tetrahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p2;                  /*!< Second point of the tetrahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p3;                  /*!< Third point of the tetrahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p4;                  /*!< Fourth point of the tetrahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int label;               /*!< Integer value associated with the
                             *    tetrahedron. */
} Tetrahedron;

// Only used if opt_mode=-2/-1/0
/**
* \struct Quadrilateral main.h
* \brief It can store the label and four point references of a quadrilateral
*        (only used if opt_mode=-(two/one/zero) in the Parameters structure).
*/
typedef struct {
    int p1;                  /*!< First point of the quadrilateral referred to
                             *    as its position in the array associated with
                             *    the vertices of the mesh; its value should
                             *    always be positive and not (strictly) greater
                             *    than the total number of mesh vertices (stored
                             *    in the nver variable of the Mesh
                             *    structure). */

    int p2;                  /*!< Second point of the quadrilateral referred to
                             *    as its position in the array associated with
                             *    the vertices of the mesh; its value should
                             *    always be positive and not (strictly) greater
                             *    than the total number of mesh vertices (stored
                             *    in the nver variable of the Mesh
                             *    structure). */

    int p3;                  /*!< Third point of the quadrilateral referred to
                             *    as its position in the array associated with
                             *    the vertices of the mesh; its value should
                             *    always be positive and not (strictly) greater
                             *    than the total number of mesh vertices (stored
                             *    in the nver variable of the Mesh
                             *    structure). */

    int p4;                  /*!< Fourth point of the quadrilateral referred to
                             *    as its position in the array associated with
                             *    the vertices of the mesh; its value should
                             *    always be positive and not (strictly) greater
                             *    than the total number of mesh vertices (stored
                             *    in the nver variable of the Mesh
                             *    structure). */

    int label;               /*!< Integer value associated with the
                             *    quadrilateral. */
} Quadrilateral;

/**
* \struct Hexahedron main.h
* \brief It can store the label and eight point references of an hexahedron
*        (only used if opt_mode=-(two/one/zero) in the Parameters structure).
*/
typedef struct {
    int p1;                  /*!< First point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p2;                  /*!< Second point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p3;                  /*!< Third point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p4;                  /*!< Fourth point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p5;                  /*!< Fifth point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p6;                  /*!< Sixth point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p7;                  /*!< Seventh point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

    int p8;                  /*!< Eighth point of the hexahedron referred to as
                             *    its position in the array associated with the
                             *    vertices of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh vertices (stored in
                             *    the nver variable of the Mesh structure). */

   int label;                /*!< Integer value associated with the
                             *    hexahedron. */
} Hexahedron;

// Only used if opt_mode=-2/-1/0: Adjacency structure is used to locate fastly
// the two hexahedra sharing a common quadrilateral which belongs to the
// boundary of a subdomain in the mesh
/**
* \struct Adjacency main.h
* \brief It is used to locate fastly the two hexahedra sharing the same
*        quadrilateral, which belongs to the boundary of a subdomain in the mesh
*        (only used if opt_mode=-(two/one/zero) in the Parameters structure).
*/
typedef struct {
    int quad;                /*!< Integer referring to the position of a
                             *    (boundary) quadrilateral in the array
                             *    associated with the quadrilaterals of the
                             *    mesh; its value should always be positive and
                             *    not (strictly) greater than the total number
                             *    of the mesh quadrilaterals (stored in the nqua
                             *    variable of the Mesh structure). */

    int hexin;               /*!< Integer referring to the position of the
                             *    hexahedron having quadrilateral \ref quad
                             *    among its faces and located inside a subdomain
                             *    of the mesh; its value should always be
                             *    positive and not (strictly) greater than the
                             *    total number of mesh hexahedra (stored in the
                             *    nhex variable of the Mesh structure). */

    int hexout;              /*!< Integer referring to the position of the
                             *    hexahedron having quadrilateral \ref quad
                             *    among its faces and located outside a
                             *    subdomain of the mesh; its value should always
                             *    be positive and not (strictly) greater than
                             *    the total number of mesh hexahedra (stored in
                             *    the nhex variable of the Mesh structure). */
} Adjacency;

/**
* \struct Mesh main.h
* \brief It can store all the mesh informations needed in the MPD algorithm.
*/
typedef struct {
    int nver;                /*!< Number of vertices in the mesh; it should
                             *    always be positive; if \ref pver is not
                             *    pointing to NULL, \ref nver should always
                             *    correspond to the size of the array \ref pver
                             *    is pointing to. */

    Point* pver;             /*!< Pointer used to dynamically define the array
                             *    storing the vertices of the mesh; if it is not
                             *    pointing to NULL, the size of the array it is
                             *    pointing to should always correspond to the
                             *    \ref nver value. */


    // Only used if opt_mode=1/2/3/4 (the mesh is made of vertices, tetrahedra,
    // and triangles)
    int ncor;                /*!< Number of corners in the mesh (only used if
                             *    opt_mode=one/two/three/four in Parameters
                             *    structure); its value should always be
                             *    non-negative and not (strictly) greater than
                             *    the \ref nver value. */

    int nnorm;               /*!< Number of normal vectors defined at some
                             *    (boundary) vertices of the mesh (only used if
                             *    opt_mode=one/two/three/four in Parameters
                             *    structure); its value should always be
                             *    positive and not (strictly) greater than
                             *    the \ref nver value; if \ref pnorm is not
                             *    pointing to NULL, \ref nnorm should always
                             *    correspond to the size of the array \ref
                             *    pnorm is pointing to. */

    Vector* pnorm;           /*!< Pointer used to dynamically define the array
                             *    storing the normal vectors of the mesh (only
                             *    used if opt_mode=one/two/three/four in
                             *    Parameters structure); if it is not pointing
                             *    to NULL, the size of the array it is pointing
                             *    to should always correspond to the \ref nnorm
                             *    value. */

    int ntan;                /*!< Number of tangent vectors defined at some
                             *    (boundary) vertices of the mesh (only used if
                             *    opt_mode=one/two/three/four in Parameters
                             *    structure); its value should always be
                             *    positive and not (strictly) greater than
                             *    the \ref nver value; if \ref ptan is not
                             *    pointing to NULL, \ref ntan should always
                             *    correspond to the size of the array \ref ptan
                             *    is pointing to. */

    Vector* ptan;            /*!< Pointer used to dynamically define the array
                             *    storing the tangent vectors of the mesh (only
                             *    used if opt_mode=one/two/three/four in
                             *    Parameters structure); if it is not pointing
                             *    to NULL, the size of the array it is pointing
                             *    to should always correspond to the \ref ntan
                             *    value. */

    int nedg;                /*!< Number of (boundary) edges in the mesh (only
                             *    used if opt_mode=one/two/three/four in
                             *    Parameters structure); its value should always
                             *    be non-negative and not (strictly) greater
                             *    than the \ref nver value; if \ref pedg is not
                             *    pointing to NULL, \ref nedg should always
                             *    correspond to the size of the array \ref pedg
                             *    is pointing to. */

    Edge* pedg;              /*!< Pointer used to dynamically define the array
                             *    storing the (boundary) edges of the mesh (only
                             *    used if opt_mode=one/two/three/four in
                             *    Parameters structure); if it is not pointing
                             *    to NULL, the size of the array it is pointing
                             *    to should always correspond to the \ref nedg
                             *    value. */

    int ntri;                /*!< Number of (boundary) triangles in the mesh
                             *    (only used if opt_mode=one/two/three/four in
                             *    Parameters structure); its value should always
                             *    be positive and not (strictly) greater than
                             *    the \ref nver value; if \ref ptri is not
                             *    pointing to NULL, \ref ntri should always
                             *    correspond to the size of the array \ref ptri
                             *    is pointing to. */

    Triangle* ptri;          /*!< Pointer used to dynamically define the array
                             *    storing the (boundary) triangles of the mesh
                             *    (only used if opt_mode=one/two/three/four in
                             *    Parameters structure); if it is not pointing
                             *    to NULL, the size of the array it is pointing
                             *    to should always correspond to the \ref ntri
                             *    value. */

    int ntet;                /*!< Number of tetrahedra in the mesh (only used if
                             *    opt_mode=one/two/three/four in Parameters
                             *    structure); its value should always be
                             *    positive and not (strictly) greater than the
                             *    \ref nver value; if \ref ptet is not pointing
                             *    to NULL, \ref ntet should always correspond to
                             *    the size of the array \ref ptet is pointing
                             *    to. */

    Tetrahedron* ptet;       /*!< Pointer used to dynamically define the array
                             *    storing the tetrahedra of the mesh (only used
                             *    if opt_mode=one/two/three/four in Parameters
                             *    structure); if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref ntet value. */

    int ndom;                /*!< Number of boundary triangles in the mesh
                             *    (only used if opt_mode=one/two/three/four in
                             *    Parameters structure) that constitutes
                             *    the boundary of the domain inside the
                             *    computational box; its value should always be
                             *    non-negative and not (strictly) greater than
                             *    the \ref ntri variable; if \ref pdom is not
                             *    pointing to NULL, \ref ndom should always
                             *    correspond to the size of the array \ref pdom
                             *    is pointing to. */

    int* pdom;               /*!< Pointer used to dynamically define the array
                             *    storing the positions of the boundary
                             *    triangles in \ref ptri (only used if
                             *    opt_mode=one/two/three/four in Parameters
                             *    structure) that constitutes the boundary of
                             *    the domain inside the computational box; if it
                             *    is not pointing to NULL, the size of the array
                             *    it is pointing to should always correspond to
                             *    the \ref ndom value. */

    int nbox;                /*!< Number of boundary triangles in the mesh
                             *    (only used if opt_mode=one/two/three/four in
                             *    Parameters structure) that constitutes
                             *    the intersection of the domain with the
                             *    boundary of the computational box (if there is
                             *    one); its value should always be non-negative
                             *    and not (strictly) greater than the \ref ntri
                             *    variable; if \ref pbox is not pointing to
                             *    NULL, \ref nbox should always correspond to
                             *    the size of the array \ref pbox is pointing
                             *    to. */

    int* pbox;               /*!< Pointer used to dynamically define the array
                             *    storing the positions of the boundary
                             *    triangles in \ref ptri (only used if
                             *    opt_mode=one/two/three/four in Parameters
                             *    structure) that constitutes the intersection
                             *    of the domain with the boundary of the
                             *    computational box (if there is one); if it
                             *    is not pointing to NULL, the size of the array
                             *    it is pointing to should always correspond to
                             *    the \ref nbox value. */


    // Only used if opt_mode=-2/-1/0 (the mesh is made of vertices, hexahedra,
    // and quadrilaterals)
    int nqua;                /*!< Number of (boundary) quadrilaterals in the
                             *    mesh (only used if opt_mode=-(two/one/zero) in
                             *    Parameters structure); its value should always
                             *    be positive and not (strictly) greater than
                             *    the \ref nver value; if \ref pqua is not
                             *    pointing to NULL, \ref nqua should always
                             *    correspond to the size of the array \ref pqua
                             *    is pointing to. */

    Quadrilateral* pqua;     /*!< Pointer used to dynamically define the array
                             *    storing the (boundary) quadrilaterals of the
                             *    mesh (only used if opt_mode=-(two/one/zero) in
                             *    Parameters structure); if it is not pointing
                             *    to NULL, the size of the array it is pointing
                             *    to should always correspond to the \ref nqua
                             *    value. */

    int nhex;                /*!< Number of hexahedra in the mesh (only used if
                             *    opt_mode=-(two/one/zero) in Parameters
                             *    structure); its value should always be
                             *    positive and not (strictly) greater than the
                             *    \ref nver value; if \ref phex is not pointing
                             *    to NULL, \ref nhex should always correspond to
                             *    the size of the array \ref phex is pointing
                             *    to. */

    Hexahedron* phex;        /*!< Pointer used to dynamically define the array
                             *    storing the hexahedra of the mesh (only used
                             *    if opt_mode=-(two/one/zero) in Parameters
                             *    structure); if it is not pointing to NULL, the
                             *    size of the array it is pointing to should
                             *    always correspond to the \ref nhex value. */

    int nadj;                /*!< Number of quadrilaterals concerned by the
                             *    subdomain in the mesh (only used if
                             *    opt_mode=-(two/one/zero) in Parameters
                             *    structure); its value should always be
                             *    positive and not (strictly) greater than the
                             *    \ref nver value; if \ref padj is not pointing
                             *    to NULL, \ref nadj should always correspond to
                             *    the size of the array \ref padj is pointing
                             *    to. */

    Adjacency* padj;         /*!< Pointer used to dynamically define the array
                             *    storing the subdomain quadrilaterals of the
                             *    mesh (only used if opt_mode=-(two/one/zero) in
                             *    Parameters structure); if it is not pointing
                             *    to NULL, the size of the array it is pointing
                             *    to should always correspond to the \ref nqua
                             *    value. */
} Mesh;

////////////////////////////////////////////////////////////////////////////////
// Prototype of non-static functions (many functions here sould be static but
// they are defined as non-static in order to unit-test them)
////////////////////////////////////////////////////////////////////////////////
/**
* \fn void printTimer(time_t finalTimer, time_t initialTimer)
* \brief It takes the time difference between the finalTimer and initialTimer
*        variables and converts it into a human readable time format (hour(s),
*        minute(s), second(s), instantaneous), which is finally printed in the
*        standard output stream.
*
* \param[in] finalTimer A time_t variable storing the final time associated with
*                       the execution time of a process that is intended to be
*                       evaluated.
*
* \param[in] initialTimer A time_t variable storing the initial time associated
*                         with the execution time of a process that is intended
*                         to be evaluated.
*
* The function \ref printTimer is usually called when we want to print the
* execution time of a process and it is also called by the \ref endTimerAtExit
* function when the program stops. It returns no argument (void output) and
* This function should be static but it is defined as non-static in order to
* unit-test it.
*/
void printTimer(time_t finalTimer, time_t initialTimer);

/**
* \fn void endTimerAtExit(void)
* \brief Function called by atexit to print the execution time when the program
*        will stop.
*
* The function \ref endTimerAtExit uses the \ref globalInitialTimer variable in
* order to print properly the total execution time of the MPD program. It has
* no argument and it returns no argument (void input and output). This function
* should be static but it is defined as non-static in order to unit-test it.
*/
void endTimerAtExit(void);

/**
* \fn char* endTimerAtError(void)
* \brief Function called only by \ref PRINT_ERROR to print the date and time at
*        which an error occurs in the program or by the \ref saveDataInTheLoop
*        function if its input iterationInTheLoop variable is set to zero so
*        as to print the local date and time at the beginning of the *.result
*        file.
*
* \return Ir returns the local date and time as a string (output of the standard
*          ctim c-function) in a human readable format.

* It has no argument (void input).
*/
char* endTimerAtError(void);

/**
* \fn void commentAnormalEnd(int typeOfSignal)
* \brief It displays an error message in the standard output stream when an
*        anormal end of the MPD program occurred.
*
* \param[in] typeOfSignal It is expected to refer to one of the standard signal
*                         number to be caught (SIGABRT, SIGFPE, SIGILL, SIGSEGV,
*                         SIGTERM, SIGINT). Otherwise, the function will only
*                         display the integer caught in the standard output
*                         stream.
*
* The function \ref commentAnormalEnd is meant to describe the type of anormal
* error encountered when the MPD program anormally stops. It is only called by
* the signal function (cf. \ref INITIALIZE_MAIN_STRUCTURES code for further
* details) when one of a standard signal is caught (SIGABRT, SIGFPE, SIGILL,
* SIGSEGV, SIGTERM, SIGINT). This function returns no argument (void output) and
* should be static but it is defined as non-static in order to unit-test it.
*/
void commentAnormalEnd(int typeOfSignal);

/**
* \fn int checkStringFromLength(char* stringToCheck, int minimumLength,
*                                                             int maximumLength)
* \brief It checks if stringToCheck is pointing to a string with length
*        comprised between minimumLength and maximumLength.
*
* \param[in] stringToCheck It is expected to point to a string of length
*                          comprised between minimumLength and maximumLength
*                          variables. Otherwise (i.e. if stringToCheck==NULL,
*                          or if the terminating nul character '\0' is strictly
*                          located before the minimumLength position of the
*                          array or after the maximumLength one), an error is
*                          returned by \ref checkStringFromLength function.
*
* \param[in] minimumLength Minimal length allowed for the string (including the
*                          terminating nul character '\0'). It must be positive
*                          and not (strictly) greater than the maximumLength
*                          variable, otherwise an error is returned by \ref
*                          checkStringFromLength function.
*
* \param[in] maximumLength Maximal length allowed for the string (including the
*                          terminating nul character '\0'). It must not be
*                          (strictly) lower than minimumLength variable,
*                          otherwise an error is returned by \ref
*                          checkStringFromLength function.
*
* \return It returns zero if an error is encountered, otherwise the (positive)
*         length of stringToCheck is returned (including the terminating nul
*         character '\0').
*
* The function \ref checkStringFromLength evaluates the length (including the
* terminating nul character '\0') of the stringTocheck variable, which must be
* comprised between minimumLength and maximumLength variables (provided that
* 0<minimumLength<=maximumLength).
*/
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                             int maximumLength);

/**
* \fn int checkAllPreprocessorConstants(int optMode, int verbose, int nCpu,
*                                       int nameLength, double lameInt1,
*                                       double lameInt2, double lameExt1,
*                                       double lameExt2, double xMin,
*                                       double yMin, double zMin, double xMax,
*                                       double yMax, double zMax, int nX,
*                                       int nY,int nZ, double deltaX,
*                                       double deltaY, double deltaZ,
*                                       int lsType, double lsX, double lsY,
*                                       double lsZ, double lsR, double metCst,
*                                       double metErr, double metMin,
*                                       double metMax, int trickMatrix,
*                                       int approxMode, int iterMax,
*                                       double iterTolD0P, double iterTolD1P,
*                                       double iterTolD2P, int saveType,
*                                       int saveMesh, int saveData,
*                                       int savePrint, int saveWhere,
*                                       int pathLength, char* pathMedit,
*                                       char* pathMmg3d, char* pathMshdist,
*                                       char* pathElastic, char* pathAdvect,
*                                       double hminIso, double hmaxIso,
*                                       double hausdIso, double hgradIso,
*                                       double hminMet, double hmaxMet,
*                                       double hausdMet, double hgradMet,
*                                       double hminLs, double hmaxLs,
*                                       double hausdLs, double hgradLs,
*                                       int hmodeLag, double hminLag,
*                                       double hmaxLag, double hausdLag,
*                                       double hgradLag, int nIter,
*                                       double residual, double deltaT,
*                                       int noCfl, int orb1, int orb2, int orb3,
*                                       int orb4, int orb5, int orb6, int orb7,
*                                       int orb8, int orb9, int orb10,
*                                       int orb11, int orb12, int orb13,
*                                       int orb14, int orb15, int orb16,
*                                       int orb17, int orb18, int orb19,
*                                       int orb20, double cstA, double cstB,
*                                       double cstC, double csta, double cstb,
*                                       double cstc, double cstaa, double cstbb,
*                                       double cstcc, double cstOne,
*                                       double cstTwo, double cstThree,
*                                       double cst1, double cst2, double cst3,
*                                       double cst22, double cst33,
*                                                 double invPhi, double invPhi2)
* \brief It tests if all the preprocessor constants are valid.
*
* \param[in] optMode It can only be set to -2/-1/0/1/2/3/4 (see \ref OPT_MODE
*                    description for further details). Otherwise, an error is
*                    returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] verbose It can only be set to 0 or 1 (see \ref VERBOSE description
*                    for further details). Otherwise, an error is returned by
*                    \ref checkAllPreprocessorConstants function.
*
* \param[in] nCpu It must be a positive integer (see \ref N_CPU description for
*                 further details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] nameLength It must be an integer (strictly) greater than six (see
*                       \ref NAME_LENGTH description for further details).
*                       Otherwise, an error is returned by
*                       checkAllPreprocessorConstants function.
*
* \param[in] lameInt1 It must be a positive double value (see \ref LAME_INT1
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*                     A typical value is given by 186000.0 for the first Lamé's
*                     coefficient.
*
* \param[in] lameInt2 It must be a positive double value (see \ref LAME_INT2
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*                     A typical value is given by 3400.0 for the second Lamé's
*                     coefficient.
*
* \param[in] lameExt1 It must be a positive double value (see \ref LAME_EXT1
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*                     A typical value is given by 186000.0 for the first Lamé's
*                     coefficient.
*
* \param[in] lameExt2 It must be a positive double value (see \ref LAME_EXT2
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*                     A typical value is given by 3400.0 for the second Lamé's
*                     coefficient.
*
* \param[in] xMin It must be a double value that is (strictly) lower than the
*                 xMax variable (see \ref X_MIN description for further details)
*                 and we must have deltaX equal to (xMax-xMin)/(double)(nX-1).
*                 Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] yMin It must be a double value that is (strictly) lower than the
*                 yMax variable (see \ref Y_MIN description for further details)
*                 and we must have deltaY equal to (yMax-yMin)/(double)(nY-1).
*                 Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] zMin It must be a double value that is (strictly) lower than the
*                 zMax variable (see \ref Z_MIN description for further details)
*                 and we must have deltaZ equal to (zMax-zMin)/(double)(nZ-1).
*                 Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] xMax It must be a double value that is (strictly) greater than the
*                 xMin variable (see \ref X_MAX description for further details)
*                 and we must have deltaX equal to (xMax-xMin)/(double)(nX-1).
*                 Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] yMax It must be a double value that is (strictly) greater than the
*                 yMin variable (see \ref Y_MAX description for further details)
*                 and we must have deltaY equal to (yMax-yMin)/(double)(nY-1).
*                 Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] zMax It must be a double value that is (strictly) greater than the
*                 zMin variable (see \ref Z_MAX description for further details)
*                 and we must have deltaZ equal to (zMax-zMin)/(double)(nZ-1).
*                 Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] nX It must be an integer (strictly) greater than two (see \ref N_X
*               description for further details) and we must have deltaX equal
*               to (xMax-xMin)/(double)(nX-1). Otherwise, an error is returned
*               by \ref checkAllPreprocessorConstants function.
*
* \param[in] nY It must be an integer (strictly) greater than two (see \ref N_Y
*               description for further details) and we must have deltaY equal
*               to (yMax-yMin)/(double)(nY-1). Otherwise, an error is returned
*               by \ref checkAllPreprocessorConstants function.
*
* \param[in] nZ It must be an integer (strictly) greater than two (see \ref N_Z
*               description for further details) and we must have deltaZ equal
*               to (zMax-zMin)/(double)(nZ-1). Otherwise, an error is returned
*               by \ref checkAllPreprocessorConstants function.
*
* \param[in] deltaX It must be equal to (xMax-xMin)/(double)(nX-1) as a double
*                   value (see \ref DELTA_X description for further details) and
*                   the tolerance allowed here for the error on the double
*                   representation of the division must be lower than 1.0e-16
*                   (i.e. a bit higher than the order of magnitude of the
*                   (rounding) machine epsilon. Otherwise, an error is returned
*                   by \ref checkAllPreprocessorConstants function.
*
* \param[in] deltaY It must be equal to (yMax-yMin)/(double)(nY-1) as a double
*                   value (see \ref DELTA_Y description for further details) and
*                   the tolerance allowed here for the error on the double
*                   representation of the division must be lower than 1.0e-16
*                   (i.e. a bit higher than the order of magnitude of the
*                   (rounding) machine epsilon. Otherwise, an error is returned
*                   by \ref checkAllPreprocessorConstants function..
*
* \param[in] deltaZ It must be equal to (zMax-zMin)/(double)(nZ-1) as a double
*                   value (see \ref DELTA_Z description for further details) and
*                   the tolerance allowed here for the error on the double
*                   representation of the division must be lower than 1.0e-16
*                   (i.e. a bit higher than the order of magnitude of the
*                   (rounding) machine epsilon. Otherwise, an error is returned
*                   by \ref checkAllPreprocessorConstants function.
*
* \param[in] lsType It can only be set to 0 or 1 (see \ref LS_TYPE description
*                   for further details). Otherwise, an error is returned by
*                   \ref checkAllPreprocessorConstants function.
*
* \param[in] lsX It can be any double value (see \ref LS_X description for
*                further details) but a warning is displayed if the point
*                (lsX, lsY, lsZ) is not located inside the computational box
*                defined by the cube [xMin,xMax]x[yMin,yMax]x[zMin,zMax].
*
* \param[in] lsY It can be any double value (see \ref LS_Y description for
*                further details) but a warning is displayed if the point
*                (lsX, lsY, lsZ) is not located inside the computational box
*                defined by the cube [xMin,xMax]x[yMin,yMax]x[zMin,zMax].
*
* \param[in] lsZ It can be any double value (see \ref LS_Z description for
*                further details) but a warning is displayed if the point
*                (lsX, lsY, lsZ) is not located inside the computational box
*                defined by the cube [xMin,xMax]x[yMin,yMax]x[zMin,zMax].
*
* \param[in] lsR It must be a positive double value (see \ref LS_R description
*                for further details). Otherwise, an error is returned by \ref
*                checkAllPreprocessorConstants function.
*
* \param[in] metCst It must be exactly equal to 0.28125 (see \ref MET_CST
*                   description for further details). Otherwise, an error is
*                   returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] metErr It must be a positive double value (see \ref MET_ERR
*                   description for further details). Otherwise, an error is
*                   returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] metMin It must be a positive double value that is (strictly) lower
*                   than the metMax variable (see \ref MET_MIN description for
*                   further details). Otherwise, an error is returned by \ref
*                   checkAllPreprocessorConstants function.
*
* \param[in] metMax It must be a double value that is (strictly) greater than
*                   the metMax variable (see \ref MET_MAX description for
*                   further details). Otherwise, an error is returned by  \ref
*                   checkAllPreprocessorConstants function.
*
* \param[in] trickMatrix It can only be set to 0 or 1 (see \ref TRICK_MATRIX
*                        description for further details). Otherwise, an error
*                        is returned by \ref checkAllPreprocessorConstants
*                        function.
*
* \param[in] approxMode It can only be set to 0 or 1 (see \ref APPROX_MODE
*                        description for further details). Otherwise, an error
*                        is returned by \ref checkAllPreprocessorConstants
*                        function.
*
* \param[in] iterMax It must be a non-negative integer (see \ref ITER_MAX
*                    description for further details). Otherwise, an error is
*                    returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] iterTolD0P It must be a non-negative double value (see \ref
*                       ITER_TOLD0P description for further details). Otherwise,
*                       an error is returned by \ref
*                       checkAllPreprocessorConstants function.
*
* \param[in] iterTolD1P It must be a non-negative double value (see \ref
*                       ITER_TOLD1P description for further details). Otherwise,
*                       an error is returned by \ref
*                       checkAllPreprocessorConstants function.
*
* \param[in] iterTolD2P It must be a non-negative double value (see \ref
*                       ITER_TOLD2P description for further details). Otherwise,
*                       an error is returned by \ref
*                       checkAllPreprocessorConstants function.
*
* \param[in] saveType It can only be set to 0, 1 or 2 (see \ref SAVE_TYPE
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] saveMesh It must be a non-negative integer (see \ref SAVE_MESH
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] saveData It must be a non-negative integer (see \ref SAVE_DATA
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] savePrint It must be a non-negative integer (see \ref SAVE_PRINT
*                      description for further details). Otherwise, an error is
*                      returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] saveWhere It can only be set to 1/2/3/4/5/6/7 (see \ref SAVE_WHERE
*                      description for further details). Otherwise, an error is
*                      returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] pathLength It must be an integer (strictly) greater than one (see
*                       \ref PATH_LENGTH description for further details).
*                       Otherwise, an error is returned by \ref
*                       checkAllPreprocessorConstants function.
*
* \param[in] pathMedit It is expected to point to a string of length comprised
*                      between two and the pathLength variable, including the
*                      terminating nul character '\0' (see \ref PATH_MEDIT
*                      description for further details). Otherwise (i.e. either
*                      pathMedit is pointing the the NULL adress, or the
*                      terminating nul character '\0' is strictly located
*                      before the second position of the array or after the
*                      pathLength one), an error is returned by \ref
*                      checkAllPreprocessorConstants function.
*
* \param[in] pathMmg3d It is expected to point to a string of length comprised
*                      between two and the pathLength variable, including the
*                      terminating nul character '\0' (see \ref PATH_MMG3D
*                      description for further details). Otherwise (i.e. either
*                      pathMmg3d is pointing the the NULL adress, or the
*                      terminating nul character '\0' is strictly located
*                      before the second position of the array or after the
*                      pathLength one), an error is returned by \ref
*                      checkAllPreprocessorConstants function.
*
* \param[in] pathMshdist It is expected to point to a string of length comprised
*                        between two and the pathLength variable, including the
*                        terminating nul character '\0' (see \ref PATH_MSHDIST
*                        description for further details). Otherwise (i.e.
*                        either pathMshdist is pointing the the NULL adress, or
*                        the terminating nul character '\0' is strictly located
*                        before the second position of the array or after the
*                        pathLength one), an error is returned by \ref
*                        checkAllPreprocessorConstants function.
*
* \param[in] pathElastic It is expected to point to a string of length comprised
*                        between two and the pathLength variable, including the
*                        terminating nul character '\0' (see \ref PATH_ELASTIC
*                        description for further details). Otherwise (i.e.
*                        either pathElastic is pointing the the NULL adress, or
*                        the terminating nul character '\0' is strictly located
*                        before the second position of the array or after the
*                        pathLength one), an error is returned by \ref
*                        checkAllPreprocessorConstants function.
*
* \param[in] pathAdvect It is expected to point to a string of length comprised
*                       between two and the pathLength variable, including the
*                       terminating nul character '\0' (see \ref PATH_ADVECT
*                       description for further details). Otherwise (i.e.
*                       either pathAdvect is pointing the the NULL adress, or
*                       the terminating nul character '\0' is strictly located
*                       before the second position of the array or after the
*                       pathLength one), an error is returned by \ref
*                       checkAllPreprocessorConstants function.
*
* \param[in] hminIso It must be positive double value that is (strictly) lower
*                    than the hmaxIso variable (see \ref HMIN_ISO description
*                    for further details). Otherwise, an error is returned by
*                    \ref checkAllPreprocessorConstants function.
*
* \param[in] hmaxIso It must be a double value that is (strictly) greater than
*                    the hminIso variable (see \ref HMAX_ISO description for
*                    further details). Otherwise, an error is returned by \ref
*                    checkAllPreprocessorConstants function.
*
* \param[in] hausdIso It must be a positive double value (see \ref HAUSD_ISO
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] hgradIso It must be a double value that is (strictly) greater than
*                     one (see \ref HGRAD_ISO description for further details).
*                     Otherwise, an error is returned by \ref
*                     checkAllPreprocessorConstants function.
*
* \param[in] hminMet It must be positive double value that is (strictly) lower
*                    than the hmaxMet variable (see \ref HMIN_MET description
*                    for further details). Otherwise, an error is returned by
*                    \ref checkAllPreprocessorConstants function.
*
* \param[in] hmaxMet It must be a double value that is (strictly) greater than
*                    the hminMet variable (see \ref HMAX_MET description for
*                    further details). Otherwise, an error is returned by \ref
*                    checkAllPreprocessorConstants function.
*
* \param[in] hausdMet It must be a positive double value (see \ref HAUSD_MET
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] hgradMet It must be a double value that is (strictly) greater than
*                     one (see \ref HGRAD_MET description for further details).
*                     Otherwise, an error is returned by \ref
*                     checkAllPreprocessorConstants function.
*
* \param[in] hminLs It must be positive double value that is (strictly) lower
*                   than the hmaxLs variable (see \ref HMIN_LS description for
*                   further details). Otherwise, an error is returned by \ref
*                   checkAllPreprocessorConstants function.
*
* \param[in] hmaxLs It must be a double value that is (strictly) greater than
*                   the hminLs variable (see \ref HMAX_LS description for
*                   further details). Otherwise, an error is returned by \ref
*                   checkAllPreprocessorConstants function.
*
* \param[in] hausdLs It must be a positive double value (see \ref HAUSD_LS
*                    description for further details). Otherwise, an error is
*                    returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] hgradLs It must be a double value that is (strictly) greater than
*                     one (see \ref HGRAD_LS description for further details).
*                     Otherwise, an error is returned by \ref
*                     checkAllPreprocessorConstants function.
*
* \param[in] hmodeLag It can only be set to 0, 1 or 2 (see \ref HMODE_LAG
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] hminLag It must be positive double value that is (strictly) lower
*                    than the hmaxLag variable (see \ref HMIN_LAG description
*                    for further details). Otherwise, an error is returned by
*                    \ref checkAllPreprocessorConstants function.
*
* \param[in] hmaxLag It must be a double value that is (strictly) greater than
*                    the hminLag variable (see \ref HMAX_LAG description for
*                    further details). Otherwise, an error is returned by \ref
*                    checkAllPreprocessorConstants function.
*
* \param[in] hausdLag It must be a positive double value (see \ref HAUSD_LAG
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] hgradLag It must be a double value that is (strictly) greater than
*                     one (see \ref HGRAD_LAG description for further details).
*                     Otherwise, an error is returned by \ref
*                     checkAllPreprocessorConstants function.
*
* \param[in] nIter It must be a non-negative integer (see \ref N_ITER
*                  description for further details). Otherwise, an error is
*                  returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] residual It must be a positive double value (see \ref RESIDUAL
*                     description for further details). Otherwise, an error is
*                     returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] deltaT It must be a positive double value (see \ref DELTA_T
*                   description for further details). Otherwise, an error is
*                   returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] noCfl It can only be set to 0 or 1 (see \ref NO_CFL description
*                  for further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb1 It must be set to 1 (see \ref ORB_S description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb2 It must be set to 2 (see \ref ORB_PX description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb3 It must be set to 3 (see \ref ORB_PY description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb4 It must be set to 4 (see \ref ORB_PZ description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb5 It must be set to 5 (see \ref ORB_DXX description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb6 It must be set to 6 (see \ref ORB_DYY description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb7 It must be set to 7 (see \ref ORB_DZZ description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb8 It must be set to 8 (see \ref ORB_DXY description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb9 It must be set to 9 (see \ref ORB_DXZ description for further
*                 details). Otherwise, an error is returned by \ref
*                 checkAllPreprocessorConstants function.
*
* \param[in] orb10 It must be set to 10 (see \ref ORB_DYZ description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb11 It must be set to 11 (see \ref ORB_FXXX description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb12 It must be set to 12 (see \ref ORB_FYYY description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb13 It must be set to 13 (see \ref ORB_FZZZ description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb14 It must be set to 14 (see \ref ORB_FXXY description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb15 It must be set to 15 (see \ref ORB_FXXZ description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb16 It must be set to 16 (see \ref ORB_FYYZ description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb17 It must be set to 17 (see \ref ORB_FXYY description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb18 It must be set to 18 (see \ref ORB_FXZZ description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb19 It must be set to 19 (see \ref ORB_FYZZ description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] orb20 It must be set to 20 (see \ref ORB_FXYZ description for
*                  further details). Otherwise, an error is returned by \ref
*                  checkAllPreprocessorConstants function.
*
* \param[in] cstA It must be exactly equal to 0.007091003462846911 (see \ref
*                 CST_A description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstB It must be exactly equal to 0.018781320953002642 (see \ref
*                 CST_B description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstC It must be exactly equal to 0.012248840519393658 (see \ref
*                 CST_C description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] csta It must be exactly equal to 0.454496295874350351 (see \ref
*                 CST_a description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstb It must be exactly equal to 0.310885919263300610 (see \ref
*                 CST_b description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstc It must be exactly equal to 0.092735250310891226 (see \ref
*                 CST_c description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstaa It must be exactly equal to 0.045503704125649649 (see \ref
*                  CST_aa description for further details). Otherwise, an error
*                  is returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstbb It must be exactly equal to 0.067342242210098170 (see \ref
*                  CST_bb description for further details). Otherwise, an error
*                  is returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstcc It must be exactly equal to 0.721794249067326322 (see \ref
*                  CST_cc description for further details). Otherwise, an error
*                  is returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstOne It must be exactly equal to 0.225 (see \ref CST_ONE
*                   description for further details). Otherwise, an error is
*                   returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cstTwo It must be exactly equal to 0.125939180544827153 (see \ref
*                   CST_TWO description for further details). Otherwise, an
*                   error is returned by \ref checkAllPreprocessorConstants
*                   function.
*
* \param[in] cstThree It must be exactly equal to 0.132394152788506181 (see \ref
*                     CST_THREE description for further details). Otherwise, an
*                     error is returned by \ref checkAllPreprocessorConstants
*                     function.
*
* \param[in] cst1 It must be exactly equal to 0.333333333333333333 (see \ref
*                 CST_1 description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cst2 It must be exactly equal to 0.101286507323456339 (see \ref
*                 CST_2 description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cst3 It must be exactly equal to 0.470142064105115090 (see \ref
*                 CST_3 description for further details). Otherwise, an error is
*                 returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cst22 It must be exactly equal to 0.797426985353087322 (see \ref
*                  CST_22 description for further details). Otherwise, an error
*                  is returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] cst33 It must be exactly equal to 0.059715871789769820 (see \ref
*                  CST_33 description for further details). Otherwise, an error
*                  is returned by \ref checkAllPreprocessorConstants function.
*
* \param[in] invPhi It must be exactly equal to 0.618033988749894848 (see \ref
*                   INV_PHI description for further details). Otherwise, an
*                   error is returned by the \ref checkAllPreprocessorConstants
*                   function.
*
* \param[in] invPhi2 It must be exactly equal to 0.381966011250105152 (see \ref
*                    INV_PHI2 description for further details). Otherwise, an
*                    error is returned by the \ref checkAllPreprocessorConstants
*                    function.
*
* \return It returns zero if an error is encountered. Otherwise, one is returned
*         in case of success.
*
* The function \ref checkAllPreprocessorConstants is intended to have all the
* preprocessor constants used in the MPD program as input arguments and it tests
* if their contents (integer/double values, strings) are valid. Since we only
* need to test the values of the preprocessor constants, we could have defined
* the \ref checkAllPreprocessorConstants function without any input argument.
* However, in order to perform unit-tests on this function, we were forced to
* include all the preprocessor constants as input arguments. This allwos us to
* make their values vary during the different unitary tests.
*/
int checkAllPreprocessorConstants(int optMode, int verbose, int nCpu,
                                  int nameLength, double lameInt1,
                                  double lameInt2, double lameExt1,
                                  double lameExt2, double xMin, double yMin,
                                  double zMin, double xMax, double yMax,
                                  double zMax, int nX, int nY,int nZ,
                                  double deltaX, double deltaY, double deltaZ,
                                  int lsType, double lsX, double lsY,
                                  double lsZ, double lsR, double metCst,
                                  double metErr, double metMin, double metMax,
                                  int trickMatrix, int approxMode, int iterMax,
                                  double iterTolD0P, double iterTolD1P,
                                  double iterTolD2P, int saveType, int saveMesh,
                                  int saveData, int savePrint, int saveWhere,
                                  int pathLength, char* pathMedit,
                                  char* pathMmg3d, char* pathMshdist,
                                  char* pathElastic, char* pathAdvect,
                                  double hminIso, double hmaxIso,
                                  double hausdIso, double hgradIso,
                                  double hminMet, double hmaxMet,
                                  double hausdMet, double hgradMet,
                                  double hminLs, double hmaxLs, double hausdLs,
                                  double hgradLs, int hmodeLag, double hminLag,
                                  double hmaxLag, double hausdLag,
                                  double hgradLag, int nIter, double residual,
                                  double deltaT, int noCfl, int orb1, int orb2,
                                  int orb3, int orb4, int orb5, int orb6,
                                  int orb7, int orb8, int orb9, int orb10,
                                  int orb11, int orb12, int orb13, int orb14,
                                  int orb15, int orb16, int orb17, int orb18,
                                  int orb19, int orb20, double cstA,
                                  double cstB, double cstC, double csta,
                                  double cstb, double cstc, double cstaa,
                                  double cstbb, double cstcc, double cstOne,
                                  double cstTwo, double cstThree, double cst1,
                                  double cst2, double cst3, double cst22,
                                   double cst33, double invPhi, double invPhi2);

/**
* \fn int initialFileExists(char* fileLocation, int nameLength)
* \brief It checks if a file exists at the location pointed by fileLocation,
*        which must be a string of length (strictly) less than nameLength.
*
* \param[in] fileLocation It is expected to point to either to NULL or to a
*                         string of length (strictly) less than the nameLength
*                         variable. Otherwise (i.e. if the terminating nul
*                         character '\0' is located after the nameLength
*                         position in the array pointed by fileLocation), an
*                         error is returned by the \ref checkStringFromLength
*                         function.
*
* \param[in] nameLength The maximal length allowed for the string pointed by
*                       fileLocation (including the terminating nul character
*                       '\0'). It must be an integer (strictly) greater than
*                       one. Otherwise, an error is returned by the \ref
*                       checkStringFromLength function.
*
* \return It returns one if the file exists, minus one if it does not (or if
*         the fileLocation variable is pointing to NULL), otherwise zero is
*         returned if an error is encountered.
*
* The function \ref initialFileExists is using the fopen and fclose standard
* c-functions to test (+1/-1) if a file exists at a given location, pointed by
* the fileLocation (input) variable. An error can be returned if we were not
* able to close the file properly, or if the input (char*) fileLocation
* variable is not a string of length (strictly) less than the nameLength (input)
* variable.
*/
int initialFileExists(char* fileLocation, int nameLength);

/**
* \fn void closeTheFile(FILE** pFileToClose)
* \brief It tries to properly close a FILE* variable via its associated pointer
*        pFileToClose.
*
* \param[in] pFileToClose It is a pointer on the FILE* variable that is intended
*            to be closed.
*
* The function \ref closeTheFile is intended to be used in case of error in any
* other function where a file has been opened. It does not return any value
* (void output).
*/
void closeTheFile(FILE** pFileToClose);

/**
* \fn int copyFileLocation(char* fileLocation, int nameLength,
*                                                     char* fileLocationForCopy)
* \brief It tries to copy an existing file located at fileLocation (pointing to
*        a path name of (positive) length (strictly) less than nameLength) into
*        the location specified by the fileLocationForCopy variable.
*
* \param[in] fileLocation It is expected to point to the path name of an
*                         existing file. The corresponding string must have
*                         a positive length (in order to contain at least
*                         something more than the terminating nul character
*                         '\0') which cannot be (strictly) lower than the
*                         nameLength variable. Otherwise, an error is returned
*                         by the \ref copyFileLocation function.
*
* \param[in] nameLength It corresponds to the maximal length allowed for the
*                       string pointed by fileLocation (including the
*                       terminating nul character '\0'). It must be an integer
*                       (strictly) greater than one. Otherwise, an error is
*                       returned by the \ref copyFileLocation function.
*
* \param[in] fileLocationForCopy It is expected to point to the path name of a
*                                non-already-existing file. The corresponding
*                                string must have a positive length (in order to
*                                contain at least something more than the
*                                terminating nul character '\0') which cannot
*                                be (strictly) lower than the nameLength
*                                variable. Otherwise, an error is returned by
*                                the \ref copyFileLocation function.
*
* \return It returns one if the file at fileLocation has been successfully
*         copied at the location given by the fileLocationForCopy variable. In
*         any other situation (i.e. if the input variables do not have the
*         expected content), an error is displayed in the standard error stream
*         and zero is returned by the \ref copyFileLocation function.
*/
int copyFileLocation(char* fileLocation, int nameLength,
                                                     char* fileLocationForCopy);

/**
* \fn int renameFileLocation(char* fileLocation, int nameLength,
*                                                         char* newFileLocation)
* \brief It tries to rename an existing file located at fileLocation (pointing
*        to a path name of (positive) length (strictly) less than nameLength)
*        into the name specified by the newFileLocation variable.
*
* \param[in] fileLocation It is expected to point to the path name of an
*                         existing file. The corresponding string must have
*                         a positive length (in order to contain at least
*                         something more than the terminating nul character
*                         '\0') which cannot be (strictly) lower than the
*                         nameLength variable. Otherwise, an error is returned
*                         by the \ref renameFileLocation function.
*
* \param[in] nameLength It corresponds to the maximal length allowed for the
*                       string pointed by fileLocation (including the
*                       terminating nul character '\0'). It must be an integer
*                       (strictly) greater than one. Otherwise, an error is
*                       returned by the \ref renameFileLocation function.
*
* \param[in] newFileLocation It is expected to point to the path name of a
*                            non-already-existing file. The corresponding
*                            string must have a positive length (in order to
*                            contain at least something more than the
*                            terminating nul character '\0') which cannot be
*                            (strictly) lower than the nameLength variable.
*                            Otherwise, an error is returned by the \ref
*                            renameFileLocation function.
*
* \return It returns one if the file at fileLocation has been successfully
*         renamed at the location given by the newFileLocation variable. In
*         any other situation (i.e. if the input variables do not have the
*         expected content), an error is displayed in the standard error stream
*         and zero is returned by the \ref renameFileLocation function.
*/
int renameFileLocation(char* fileLocation, int nameLength,
                                                         char* newFileLocation);

/**
* \fn int plotMeshWithMeditSoftware(Parameters* pParameters)
* \brief It tries to execute medit software, whose path name is stored in the
*        path_medit variable of the structure pointed by pParameters in order to
*        vizualize the *.mesh file whose name is stored in the name_mesh
*        variable of the structure pointed by pParameters.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable is
*                        intended to store the name of a valid and existing
*                        *.mesh file in order to be launched by the external
*                        medit sofware which must have been previously
*                        installed, and whose path name must be store in the
*                        path_medit variable of the structure pointed by
*                        pParameters. Otherwise, an error is returned by the
*                        \ref plotMeshWithMeditSoftware function.
*
* \return It returns one if the *.mesh file stored in the name_mesh variable of
*         the structure pointed by pParameters has been successfully plotted
*         by the external medit sofware. In any other situation (i.e. if the
*         input variables do not have the expected content), an error is
*         displayed in the standard error stream and zero is returned by the
*         \ref plotMeshWithMeditSoftware function.
*
* The \ref plotMeshWithMeditSoftware function executes the medit sofware thank
* to the standard system c-function. In particular, the medit sofware must have
* been previously installed at a location that must be either given by the
* default \ref PATH_MEDIT preprocessor constant, or by a valid path name
* specified in the *.input file (preceded by the path_medit keyword).
* Similarly, the mesh to be read must be an existing file written in the *.mesh
* format and stored in the name_mesh variable of the structure pointed by
* pParameters.
*/
int plotMeshWithMeditSoftware(Parameters* pParameters);

/**
* \fn int adaptMeshWithMmg3dSoftware(Parameters* pParameters, char adaptMode[4])
* \brief It tries to execute the external mmg3d software in order to adapt the
*        mesh according to the parameters stored in the structure pointed by
*        pParameters and the adaptMode variable.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable is
*                        intended to store the name of a valid and existing
*                        *.mesh file in order to be launched by the external
*                        mmg3d sofware which must have been previously
*                        installed, and whose path name must be store in the
*                        path_mmg3d variable of the structure pointed by
*                        pParameters. Otherwise, an error is returned by the
*                        \ref adaptMeshWithMmg3dSoftware function.
*
* \param[in] adaptMode A string that allows the function to distinguish the type
*                      of adaptation to be performed by the mmg3d software. If
*                      set to "iso" an isotropic metric is performed; if set to
*                      "met" a mesh adaptation according to the metric
*                      (associated to the orbitals) is performed by mmg3d; if
*                      to "ls", a mesh adaptation is performed according the
*                      the level-set function (and also respecting the
*                      orbitals' metric is the modified version of mmg3d is
*                      stored in the path_mmg3d variable of the structure
*                      pointed by pParameters); if set to "lag", a mesh
*                      adaptation is performed by using the Lagrangian mode of
*                      the mmg3d software. Otherwise, an error is returned by
*                      the \ref adaptMeshWithMmg3dSoftware function.
*
* \return It returns one if the *.mesh file stored in the name_mesh variable of
*         the structure pointed by pParameters has been successfully adapted
*         by the external mmg3d sofware. In any other situation (i.e. if the
*         input variables do not have the expected content), an error is
*         displayed in the standard error stream and zero is returned by the
*         \ref adaptMeshWithMmg3dSoftware function.
*
* The \ref adaptMeshWithMmg3dSoftware function executes the mmg3d sofware thank
* to the standard system c-function. In particular, the mmg3d sofware must have
* been previously installed at a location that must be either given by the
* default \ref PATH_MMG3D preprocessor constant, or by a valid path name
* specified in the *.input file (preceded by the path_mmg3d keyword).
* Similarly, the mesh to be read must be an existing file written in the *.mesh
* format and stored in the name_mesh variable of the structure pointed by
* pParameters and the corresponding *.sol file must have a similar name
* (replacing the *.mesh extension by the *.sol one).
*/
int adaptMeshWithMmg3dSoftware(Parameters* pParameters, char adaptMode[4]);

/**
* \fn int renormalizeWithMshdistSoftware(Parameters* pParameters, char mode[4])
* \brief It tries to execute the external mshdist software in order to
*        renormalize a level-set function into a signed distance function
*        according to the parameters stored in the structure pointed by
*        pParameters and the mode variable.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable is
*                        intended to store the name of a valid and existing
*                        *.mesh file in order to be launched by the external
*                        mshdist sofware which must have been previously
*                        installed, and whose path name must be store in the
*                        path_mshdist variable of the structure pointed by
*                        pParameters. Otherwise, an error is returned by the
*                        \ref renormalizeWithMshdistSoftware function.
*
* \param[in] mode A string that allows the function to distinguish the type
*                 of renormalization to be performed by the mshdist software. If
*                 set to "sol", the program looks at an *.sol file associated
*                 with the *.mesh one and assume that it contains the values
*                 of a level-set function stored at the mesh vertices that are
*                 intended to be renormalized thanks to the mshdist algorithm
*                 (i.e. the level-set function satisfies the unitary gradient
*                 norm condition). Otherwise, it must be set to "dom" and the
*                 program directly generates the signed distance function from
*                 the underlying internal domain stored in the mesh by using
*                 the convention of the mmg3d software (tetrahedra labelled 3
*                 belongs to the domain, otherwise labelled 2, internal boundary
*                 triangles labelled 10). If other keywords are used for mode,
*                 an error is returned by \ref renormalizeWithMshdistSoftware
*                 function.
*
* \return It returns one if the *.sol file associated with the *.mesh one stored
*         in the name_mesh variable of the structure pointed by pParameters has
*         been successfully created or modified by the external mshdist sofware.
*         In any other situation (i.e. if the input variables do not have the
*         expected content), an error is displayed in the standard error stream
*         and zero is returned by the \ref renormalizeWithMshdistSoftware
*         function.
*
* The \ref renormalizeWithMshdistSoftware function executes the mshdist sofware
* thank to the standard system c-function. In particular, the mshdist sofware
* must have been previously installed at a location that must be either given
* by the default \ref PATH_MSHDIST preprocessor constant, or by a valid path
* name specified in the *.input file (preceded by the path_mshdist
* keyword). Similarly, the mesh to be read must be an existing file written in
* the *.mesh format and stored in the name_mesh variable of the structure
* pointed by pParameters and when mode="sol" the corresponding *.sol file must
* have a similar name (replacing the *.mesh extension by the *.sol one).
*/
int renormalizeWithMshdistSoftware(Parameters* pParameters, char mode[4]);

/**
* \fn int extendShapeGradientWithElasticSoftware(Parameters* pParameters)
* \brief It tries to execute the external elastic software in order to
*        extend the gradient outside an internal domain by solving the
*        elasticity equations with Dirichlet boundary conditions and thanks to
*        the parameters stored in the structure pointed by pParameters.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh and name_elas
*                        variables are intended to store the names of valid and
*                        existing *.mesh and *.elas file in order to be launched
*                        by the external elastic sofware which must have been
*                        previously installed, and whose path name must be
*                        store in the path_elastic variable of the structure
*                        pointed by pParameters. Otherwise, an error is returned
*                        by the \ref extendShapeGradientWithElasticSoftware
*                        function.
*
* \return It returns one if the vectorial *.sol file (containing the shape
*         gradient at the boundary mesh vertices of the internal domain)
*         associated with the *.mesh one stored in the name_mesh variable of
*         the structure pointed by pParameters has been successfully modified
*         by the external elastic software. In any other situation (i.e. if the
*         input variables do not have the expected content), an error is
*         displayed in the standard error stream and zero is returned by the
*         \ref extendShapeGradientWithElasticSoftware function.
*
* The \ref extendShapeGradientWithElasticSoftware function executes the elastic
* sofware thank to the standard system c-function. In particular, the elastic
* sofware must have been previously installed at a location that must be either
* given by the default \ref PATH_ELASTIC preprocessor constant, or by a valid
* path name specified in the *.input file (preceded by the path_elastic
* keyword). Similarly, the mesh (respectively elastic parameters) to be read
* must be an existing file written in the *.mesh (resp. *.elas) format and
* stored in the name_mesh (resp. name_elas variable of the structure pointed by
* pParameters and the corresponding *.sol file must have a similar name
* (replacing the *.mesh extension by the *.sol one).
*/
int extendShapeGradientWithElasticSoftware(Parameters* pParameters);

/**
* \fn int advectLevelSetWithAdvectSoftware(Parameters* pParameters)
* \brief It tries to execute the external advect software in order to advect
*        a level-set function (stored in a *.chi.sol) associated with an
*        internal domain thanks to (the extension of) the shape gradient
*        (stored in a *.sol file) and the parameters stored in the structure
*        pointed by pParameters.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable is
*                        intended to store the name of a valid and existing
*                        *.mesh file in order to be launched by the external
*                        advect sofware which must have been previously
*                        installed, and whose path name must be store in the
*                        path_advect variable of the structure pointed by
*                        pParameters. Otherwise, an error is returned by the
*                        \ref advectLevelSetWithAdvectSoftware function.
*
* \return It returns one if the *.sol file storing the level-set function
*         has been successfully modified thanks to the *.chi.sol" (vectorial)
*         file by the external advect software. In any other situation (i.e.
*         if the input variables do not have the expected content), an error is
*         displayed in the standard error stream and zero is returned by the
*         \ref advectLevelSetWithAdvectSoftware function.
*
* The \ref advectLevelSetWithAdvectSoftware function executes the advect
* sofware thank to the standard system c-function. In particular, the advect
* sofware must have been previously installed at a location that must be either
* given by the default \ref PATH_ADVECT preprocessor constant, or by a valid
* path name specified in the *.input file (preceded by the path_advect
* keyword). Similarly, the mesh (respectively the level-set function, the shape
* gradient vectorial extension) to be read must be an existing file written in
* the *.mesh (resp. *.sol scalar, *.sol vectorial) format and stored in the
* name_mesh variable (resp. the corresponding *.sol file must have a similar
* name replacing the *.mesh extension by the *.chi.sol, *.sol one).
*/
int advectLevelSetWithAdvectSoftware(Parameters* pParameters);

/**
* \fn int main(int argc, char *argv[])
* \brief Main function of the MPD program.
*
* \param[in] argc The (positive) number of arguments entered in the command line
*                 for executing the MPD program. Here, we must have argc=2, i.e.
*                 the command line must specify only one input argument: the
*                 *.input file used to load the parameters into the Parameters
*                 structure. Otherwise, (zero or more than one argument) an
*                 error is returned by the MPD program.
*
* \param[in] argv An array of pointers (of type char*) that points to the
*                 strings storing the names of the different arguments entered
*                 in the command line for executing the MPD program. Here,
*                 argv[0] will always point to the name of the MPD program,
*                 argv[1] must point to the required name of the *.input file,
*                 and argv[2]=argv[argc] must point to NULL; otherwise, an error
*                 is returned by the MPD program.
*
* \return It returns EXIT_FAILURE if an error is encountered, otherwise
*         EXIT_SUCCESS is returned.
*
* The function \ref main performs on a given initial domain in a computational
* box (mesh variable) at most (parameters.)iter_max deformations in order to
* maximize the probability to find exactly (parameters.)nu_electrons in a
* chemicalSystem structure. The command line must specify the *.input
* file, which must contain at least (minimal configuration) the *.wfn/ *.chem
* (chemical) file, preceded by 'name_chem' keyword, the (positive) number of
* electrons to look for, preceded by 'nu_electrons' keyword, and optionally the
* *.mesh/ *.cube (mesh) file to start with, preceded by 'name_mesh' keyword (if
* not specify a cube or a sphere is built depending on (parameters.)ls_type).
* The *.input file must end with the 'end_data' keyword. Any other informations
* placed after 'end_data' will not be read and considered as comments.
*/

#endif

