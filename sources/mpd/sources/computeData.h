#ifndef DEF_COMPUTEDATA
#define DEF_COMPUTEDATA

/**
* \file computeData.h
* \brief Macros and prototypes related to the computation of the properties that
*        are saved in the Data structure during the MPD algorithm.
* \author Jeremy DALPHIN
* \version 3.0
* \date May 1st, 2019
*
* This file contains the description of all the preprocessor constant and
* non-static function prototypes that are used to compute the properties saved
* in the Data structure during the iterative optimization process of the MPD
* algorithm.
*/

#include "main.h"

// The Bohr radius is the one given in Angstroms (1.e10m) by www.nist.gov
/**
* \def BOHR_RADIUS
* \brief It corresponds to the value of the Bohr radius given in Angstoms
*        (1.e-10m). 
*
* The Bohr radius corresponds to the characteristic length separating the
* electron from the proton. The value given here is the one suggested by the
* National Institute of Standards and Technology (www.nist.gov) of the U.S.
* Department of Commerce.
*/
#define BOHR_RADIUS 0.5291772109217

////////////////////////////////////////////////////////////////////////////////
// Constants defined in the article of Grundmann and Moller (april 1978) for 3D
// numerical integration (formula used by default in Freefem++ software for
// three-dimensional tetrahedral meshes). The 14-point formula (cf.(4.5), p.288)
// is exact on tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with
// a+b+c<=5) otherwise int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] +
// Error (of order 6)
////////////////////////////////////////////////////////////////////////////////
// Weights
/**
* \def CST_A
* \brief First weight defined in the article of Grundmann and Moller (april
*        1978) for three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_A 0.007091003462846911

/**
* \def CST_B
* \brief Second weight defined in the article of Grundmann and Moller (april
*        1978) for three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_B 0.018781320953002642

/**
* \def CST_C
* \brief Third weight defined in the article of Grundmann and Moller (april
*        1978) for three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_C 0.012248840519393658

// Barycentric coordinates
/**
* \def CST_a
* \brief First barycentric coordinate defined in the article of Grundmann and
*        Moller (april 1978) for three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_a 0.454496295874350351

/**
* \def CST_b
* \brief Second barycentric coordinate defined in the article of Grundmann and
*        Moller (april 1978) for three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_b 0.310885919263300610

/**
* \def CST_c
* \brief Third barycentric coordinate defined in the article of Grundmann and
*        Moller (april 1978) for three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_c 0.092735250310891226

/**
* \def CST_aa
* \brief Complement (CST_aa=1/2-CST_a) of the first barycentric coordinate
*        defined in the article of Grundmann and Moller (april 1978) for
*        three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_aa 0.045503704125649649     // (1/2-CST_a)

/**
* \def CST_bb
* \brief Complement (CST_bb=1/3-CST_b) of the second barycentric coordinate
*        defined in the article of Grundmann and Moller (april 1978) for
*        three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_bb 0.067342242210098170     // (1-3*CST_b)

/**
* \def CST_cc
* \brief Complement (CST_cc=1/3-CST_c) of the third barycentric coordinate
*        defined in the article of Grundmann and Moller (april 1978) for
*        three-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for three-dimensional
* tetrahedral meshes. The 14-point formula (cf.(4.5), p.288) is exact on
* tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] + Error (of order 6).
*/
#define CST_cc 0.721794249067326322     // (1-3*CST_c)

////////////////////////////////////////////////////////////////////////////////
// Constants defined in the article of Cowper (1973) for two-dimensional
// numerical integration (formula used by default in Freefem++ software for
// two-dimensional triangulated meshes). The 7-point formula (cf.Table I, p.407)
// is exact on triangles for 5th-order polynomials (sum x^a*y^b*z^c with
// a+b+c<=5) otherwise int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] +
// Error (of order 6)
////////////////////////////////////////////////////////////////////////////////
// Weights
/**
* \def CST_ONE
* \brief First weight (9/40=1-3*CST_TWO-3*CST_THREE) defined in the article of
*        of Cowper (1973) for two-dimensional numerical integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_ONE 0.225    // 9/40=1-3*CST_TWO-3*CST_THREE

/**
* \def CST_TWO
* \brief Second weight ([155-sqrt(15)]/1200 with 18 digits precision) defined
*        in the article of Cowper (1973) for two-dimensional numerical
*        integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_TWO 0.125939180544827153    // [155-sqrt(15)]/1200

/**
* \def CST_THREE
* \brief Third weight ([155+sqrt(15)]/1200 with 18 digits precision) defined
*        in the article of Cowper (1973) for two-dimensional numerical
*        integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_THREE 0.132394152788506181    // [155+sqrt(15)]/1200

// Barycentric coordinates
/**
* \def CST_1
* \brief First barycentric coordinate (1/3 with 18 digits precision) defined
*        in the article of Cowper (1973) for two-dimensional numerical
*        integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_1 0.333333333333333333    // 1/3

/**
* \def CST_2
* \brief Second barycentric coordinate ([6-sqrt(15)]/21 with 18 digits
*        precision) defined in the article of Cowper (1973) for two-dimensional
*        numerical integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_2 0.101286507323456339    // [6-sqrt(15)]/21

/**
* \def CST_3
* \brief Third barycentric coordinate ([6+sqrt(15)]/21 with 18 digits
*        precision) defined in the article of Cowper (1973) for two-dimensional
*        numerical integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_3 0.470142064105115090    // [6+sqrt(15)]/21

/**
* \def CST_22
* \brief Complement (CST_22=1-2*CST_2) of the second barycentric coordinate
*        defined in the article of Cowper (1973) for two-dimensional numerical
*        integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_22 0.797426985353087322     // (1-2*CST_2)

/**
* \def CST_33
* \brief Complement (CST_33=1-2*CST_3) of the third barycentric coordinate
*        defined in the article of Cowper (1973) for two-dimensional numerical
*        integration.
*
* The formula is used by default in Freefem++ software for two-dimensional
* triangulated meshes. The 7-point formula (cf. Table I, p.407) is exact on
* triangles for 5th-order polynomials (sum x^a*y^b*z^c with a+b+c<=5) otherwise
* int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] + Error (of order 6).
*/
#define CST_33 0.059715871789769820     // (1-2*CST_3)

////////////////////////////////////////////////////////////////////////////////
// Prototypes of non-static functions
////////////////////////////////////////////////////////////////////////////////
/**
* \fn void initializeDataStructure(Data* pData)
* \brief It initializes to zero the Data structure pointed by pData.
*
* \param[in] pData A pointer that points to the Data structure (defined in
*                  main.h) of the \ref main function.
*
* The function \ref initializeDataStructure sets to zero all the variables
* of the structure pointed by pData (and pointers are set to NULL). This
* function does not return any value (void output).
*/
void initializeDataStructure(Data* pData);

/**
* \fn void freeDataMemory(Data* pData)
* \brief It frees the memory dynamically allocated for the Data structure
*        pointed by pData.
*
* \param[in] pData A pointer that points to the Data structure (defined in
*                  main.h file) of the \ref main function.
*
* The function \ref freeDataMemory frees the memory dynamically allocated with
* calloc/malloc/realloc for the Data structure pointed by pData. All pointers of
* the Data structure that have been freed up are also set to NULL (but other
* variables than pointers are not reset to zero). This function does not return
* any value (void output).
*/
void freeDataMemory(Data* pData);

#endif

