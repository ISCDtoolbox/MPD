#ifndef DEF_OPTIMIZATION
#define DEF_OPTIMIZATION

/**
* \file optimization.h
* \brief Macros and prototypes related to the computation of the optimization
*        loop in the MPD algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* This file contains the description of all the preprocessor constant and
* non-static function prototypes that are used to compute the optimization loop
* in the MPD algorithm.
*/

#include "main.h"

/**
* \def INV_PHI
* \brief Constant used in the golden-section search of the \ref optimization
*        function. It is equal to [sqrt(5)-1]/2 = 1/Phi where Phi is the
*        golden number (positive solution of x*x-x+1=0).
*/
#define INV_PHI 0.618033988749894848

/**
* \def INV_PHI2
* \brief Constant used in the golden-section search of the \ref optimization
*        function. It is equal to [3-sqrt(5)]/2 = 1/(Phi*Phi) where Phi is the
*        golden number (positive solution of x*x-x+1=0).
*/
#define INV_PHI2 0.381966011250105152

/* ************************************************************************** */
// Constants defined in the article of Grundmann and Moller (april 1978) for 3D
// numerical integration (formula used by default in Freefem++ software for
// three-dimensional tetrahedral meshes). The 14-point formula (cf.(4.5), p.288)
// is exact on tetrahedra for 5th-order polynomials (sum x^a*y^b*z^c with
// a+b+c<=5) otherwise int_tet(f)=vol(tet)*sum_i [w_i*f(a1i,a2i,a3i,a4i)] +
// Error (of order 6)
/* ************************************************************************** */
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

/* ************************************************************************** */
// Constants defined in the article of Cowper (1973) for two-dimensional
// numerical integration (formula used by default in Freefem++ software for
// two-dimensional triangulated meshes). The 7-point formula (cf.Table I, p.407)
// is exact on triangles for 5th-order polynomials (sum x^a*y^b*z^c with
// a+b+c<=5) otherwise int_tri(f)=area(tri)*sum_i [w_i*f(a1i,a2i,a3i)] +
// Error (of order 6)
/* ************************************************************************** */
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

/* ************************************************************************** */
// Prototypes of non-static functions
/* ************************************************************************** */
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

/**
* \fn int allocateMemoryForData(Parameters* pParameters, Data* pData,
*                                               ChemicalSystem* pChemicalSystem)
* \brief It dynamically allocates memory for the structure pointed by pData
*        according to what is stored in pChemicalSystem and pParameters.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its iter_max variable is used
*                        to dynamically define the size of the arrays in the
*                        structure pointed by pData.
*
* \param[out] pData A pointer that points to the Data structure (defined in
*                   main.h file) of the \ref main function. We aim here to
*                   dynamically allocate some memory for its various variables.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. Its nmorb
*                            variable is used to dynamically define the arrays
*                            contained in the Probabilities and OverlapMatrix
*                            structures pointed by pData->pprob and pData->pmat,
*                            respectively.
*/
int allocateMemoryForData(Parameters* pParameters, Data* pData,
                                               ChemicalSystem* pChemicalSystem);

/**
* \fn int int addLengthForFileName(Parameters* pParameters,
*                                                          int additionalLength)
* \brief It checks that all file names have correct length and if they remains
*        less than the maximal size allowed when additionalLength letters are
*        added.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_length, name_info,
*                        name_data, name_chem, name_mesh, and name_elas
*                        variables are checked and their length are updated
*                        via the standard realloc c-function if additionalLength
*                        letters let their names be greater than the maximal
*                        allowed size stored in pParameters->name_length.
*
* \param[in] additionalLength A positive integer referring to the number of
*                             letters we may want to add to a name without
*                             exceeding the maximal allowed size stored in the
*                             name_length variable of the structure pointed by
*                             pParameters.
*
* The function \ref addLengthForFileName checks that all the file names stored
* in the structure pointed by pParameters are string of correct length (strictly
* less than the pParameters->name_length variable and more than five without
* counting the terminating nul character '\0') and that they ends with the
* correct extension (*.info, *.data, *.chem, *.mesh, and if needed *.elas).
* Then, it looks at the length of these names and if it remains less than
* pParameters->name_length when additionalLength letters are added, nothing is
* done. Otherwise, it dynamically reallocate memory so that each file names can
* store name of size lower than pParameters->name_length+additionalLength.
*/
int addLengthForFileName(Parameters* pParameters, int additionalLength);

/**
* \fn double evaluatingPrimitiveAtVertices(double px, double py, double pz,
*                                          ChemicalSystem* pChemicalSystem,
*                                          MolecularOrbital* pMolecularOrbital,
*                                                                 int primitive)
* \brief It evaluates at the point (px,py,pz) the function associated to a
*        Gaussian-type primitive of pMolecularOrbital described in the structure
*        pointed by pChemicalSystem.
*
* \param[in] px It corresponds to the first coordinate of the point at which
*               we wish to compute the Gaussian-type primitive function.
*
* \param[in] py It corresponds to the second coordinate of the point at which
*               we wish to compute the Gaussian-type primitive function.
*
* \param[in] pz It corresponds to the third coordinate of the point at which
*               we wish to compute the Gaussian-type primitive function.
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
*         default as an 1s-type (see \ref ORB_1S for further details) and a
*         a warning is displayed in the standard output stream.
*
* The \ref evaluatingPrimitiveAtVertices function should be static but has been
* defined as non-static in order to perform unit-tests on it. Moreover, the
* function uses the exponential mathematical function so in order to use this
* function, the math.h file must not be put in comment in the main.h file, and
* the -lm must be set when compiling with gcc (or link properly the math library
* for other compilers and architectures).  
*/
double evaluatingPrimitiveAtVertices(double px, double py, double pz,
                                     ChemicalSystem* pChemicalSystem,
                                     MolecularOrbital* pMolecularOrbital,
                                                                 int primitive);

/**
* \fn double evaluateOrbitalsIJ(double px, double py, double pz,
*                               ChemicalSystem* pChemicalSystem,
*                               MolecularOrbital* pMolecularOrbitalI,
*                                          MolecularOrbital* pMolecularOrbitalJ)
* \brief It evaluates at the point (px,py,pz) the function associated to the
*        product between a molecular orbital pointed by pMolecularOrbitalI
*        and a another one pointed by pMolecularOrbitalJ, both described in the
*        structure pointed by pChemicalSystem.
*
* \param[in] px It corresponds to the first coordinate of the point at which
*               we wish to compute the function.
*
* \param[in] py It corresponds to the second coordinate of the point at which
*               we wish to compute the function.
*
* \param[in] pz It corresponds to the third coordinate of the point at which
*               we wish to compute the function.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. In particular,
*                            we need to access the three coordinates of the
*                            Nucleus structures related to pMolecularOrbitalI
*                            and pMolecularOrbitalJ. For not slowing the
*                            process, we assume here that pChemicalSystem is not
*                            pointing to NULL, as well as its pmorb and pnucl
*                            variables.
*
* \param[in] pMolecularOrbitalI A pointer that points the one of the
*                               MolecularOrbital structure stored in the array
*                               dynamically allocated for the pmorb variable
*                               of the structure pointed by pChemicalSystem.
*                               For not slowing the process, we assume that
*                               pMolecularOrbital is not pointing to NULL, as
*                               well as its coeff, exp, nucl and type variables.
*
* \param[in] pMolecularOrbitalJ A pointer that points to a MolecularOrbital
*                               structure (distinct from pMolecularOrbitalI)
*                               stored in the array dynamically allocated for
*                               the pmorb variable of the structure pointed by
*                               pChemicalSystem. For not slowing the process,
*                               we assume that pMolecularOrbital is not pointing
*                               to NULL, as well as its coeff, exp, nucl and
*                               type variables.
*
* \return It returns the value of the function related to the product of
*         pMolecularOrbitalI by pMolecularOrbitalJ at the point (px,py,pz). For
*         not slowing the process, if the two molecular orbitals have a
*         different spin, the \ref evaluateOrbitalsIJ function immediatly
*         returns zero.
*
* The \ref evaluateOrbitalsIJ function should be static but has been defined as
* non-static in order to perform unit-tests on it. Moreover, it calls the \ref
* evaluatingPrimitiveAtVertices function, which uses the square root
* mathematical function so in order to use this function, the math.h file must
* not be put in comment in the main.h file, and the -lm must be set when
* compiling with gcc (or link properly the math library for other compilers and
* architectures).
*/
double evaluateOrbitalsIJ(double px, double py, double pz,
                          ChemicalSystem* pChemicalSystem,
                          MolecularOrbital* pMolecularOrbitalI,
                                          MolecularOrbital* pMolecularOrbitalJ);

/**
* \fn double evaluateOrbitalsII(double px, double py, double pz,
*                               ChemicalSystem* pChemicalSystem,
*                                          MolecularOrbital* pMolecularOrbitalI)
* \brief It evaluates at the point (px,py,pz) the function associated to the
*        squared value a molecular orbital pointed by pMolecularOrbitalI and
*        described in the structure pointed by pChemicalSystem.
*
* \param[in] px It corresponds to the first coordinate of the point at which
*               we wish to compute the function.
*
* \param[in] py It corresponds to the second coordinate of the point at which
*               we wish to compute the function.
*
* \param[in] pz It corresponds to the third coordinate of the point at which
*               we wish to compute the function.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. In particular,
*                            we need to access the three coordinates of the
*                            Nucleus structure related to pMolecularOrbitalI.
*                            For not slowing the process, we assume here that
*                            pChemicalSystem is not pointing to NULL, as well as
*                            its pmorb and pnucl variables.
*
* \param[in] pMolecularOrbitalI A pointer that points the one of the
*                               MolecularOrbital structure stored in the array
*                               dynamically allocated for the pmorb variable
*                               of the structure pointed by pChemicalSystem.
*                               For not slowing the process, we assume that
*                               pMolecularOrbital is not pointing to NULL, as
*                               well as its coeff, exp, nucl and type variables.
*
* \return It returns the value of the function related to the product of
*         pMolecularOrbitalI by pMolecularOrbitalI at the point (px,py,pz).
*
* The \ref evaluateOrbitalsII function should be static but has been defined as
* non-static in order to perform unit-tests on it. Moreover, it calls the \ref
* evaluatingPrimitiveAtVertices function, which uses the square root
* mathematical function so in order to use this function, the math.h file must
* not be put in comment in the main.h file, and the -lm must be set when
* compiling with gcc (or link properly the math library for other compilers and
* architectures).
*/
double evaluateOrbitalsII(double px, double py, double pz,
                          ChemicalSystem* pChemicalSystem,
                                          MolecularOrbital* pMolecularOrbitalI);

/**
* \fn double computeVolumeTetrahedron(Point* pp1, Point* pp2, Point* pp3,
*                                                                    Point* pp4)
* \brief It computes (six times) the volume of a tetrahedron defined by its
*        four vertices.
*
* \param[in] pp1 A pointer that points to the Point structure of the first
*                vertex associated with the tetrahedron we wish to compute the
*                volume.
*
* \param[in] pp2 A pointer that points to the Point structure of the second
*                vertex associated with the tetrahedron we wish to compute the
*                volume.
*
* \param[in] pp3 A pointer that points to the Point structure of the third
*                vertex associated with the tetrahedron we wish to compute the
*                volume.
*
* \param[in] pp4 A pointer that points to the Point structure of the fourth
*                vertex associated with the tetrahedron we wish to compute the
*                volume.
*
* \return It returns six times the volume of the tetrahedron defined by the four
*         vertices pointed by pp1, pp2, pp3 and pp4. More precisely, we return
*         the volume of the tetrahedron frame (pp1 pp2,pp1 pp3,pp1 pp4) in terms
*         of a determinant (vector product), which corresponds to six times the
*         volume of the tetrahedron.
*
* The \ref computeVolumeTetrahedron function is intended to be used during the
* three-dimensional numerical integration of domains, especially during the
* evaluation of the overlap matrix. For this purpose, we use the 14-point
* formula described in the article of Grundmann and Moller (april 1978,
* cf.(4.5), p.288) which is the method used by default in Freefem++ software.
* In particular, we directly use the weight values (see \ref CST_A, \ref CST_B,
* and \ref CST_C) in the form given in the article, which already includes a
* one-over-six coefficient: 6*\ref CST_A + 4*\ref CST_B + 4*\ref CST_C = 1/6.
* This is the reason why we only compute six times the volume of the tetrahedron
* in the \ref computeVolumeTetrahedron function. This function should be static
* but has been defined as non-static in order to perform unit-test on it. In
* order not to slow the process, let us mention here that we implicitly assume
* here that the input variables pp1, pp2, pp3, and pp4 are not pointing to NULL.
*/
double computeVolumeTetrahedron(Point* pp1, Point* pp2, Point* pp3, Point* pp4);

/**
* \fn int computeOverlapMatrix(Parameters* pParameters, Mesh* pMesh,
*                              Data* pData, ChemicalSystem* pChemicalSystem,
*                                      int labelToAvoid, int iterationInTheLoop)
* \brief It computes on a domain corresponding to all tetrahedra of pMesh that
*        are not labelled labelToAvoid the coefficients of the overlap matrix
*        associated with pChemicalSystem, and the corresponding values are
*        saved in the coef array variable of the OverlapMatrix structure
*        located at pData->pmat[iterationInTheLoop].
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its orb_rhf variable is used
*                        to reduce the computational time when evaluating the
*                        coefficients of a restricted Hartree-Fock chemical
*                        system i.e. when any molecular orbital has a copy
*                        saved in pChemicalSystem but with a different spin.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref
*                  main function. We assume here that its ptet variable
*                  is not pointing to NULL i.e. the mesh is made of
*                  tetrahedra, otherwise an error is returned by the \ref
*                  computeOverlapMatrix function.
*
* \param[out] pData A pointer that points to the Data structure of the \ref
*                   main function. Its pmat variable is used to fill (at the
*                   (iterationInTheLoop)-th position in the array so it must not
*                   point to NULL here) the OverlapMatrix structure with the
*                   coefficients in the array pointed by the corresponding coef
*                   variable.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It is storing
*                            the description of all the MolecularOrbital
*                            structures defined as a sum of Gaussian-type
*                            primitives.
*
* \param[in] labelToAvoid This integer represents the label of the tetrahedra
*                         on which we do not want to compute the integrals
*                         In our situation, we use the mmg3d sofware convention
*                         in level-set mode for the labelling of the structure
*                         pointed by pMesh. In particular, three refers to the
*                         tetrahedra belonging to the interior of the domain,
*                         while two refers to those that are not. Hence, in
*                         order to compute the overlap matrix on the domain
*                         labelToAvoid must be set to 2. If we want to compute
*                         the complement of the domain, then labelToAvoid must
*                         be set to 3. And similarly, if one wants to compute
*                         the overlap matrix on the whole computational box
*                         labelToAvoid must be set to -1. Other values are
*                         forbidden here, and will make the \ref
*                         computeOverlapMatrix function return an error.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations in the optimization loop. Hence,
*                               it must be a non-negative integer (zero refers
*                               to the initial domain after adaptation) not
*                               (strictly) greater than the maximal number of
*                               iterations allowed in the mpd algorithm (stored
*                               in the pParameters->iter_max variable). It
*                               allows the mpd program to save the coefficients
*                               of the overlap matrix at the correct position
*                               in the pData->pmat array of OverlapMatrix
*                               structures.
*
* \return It returns one if the coefficients of the overlap matrix have been
*         successfully computed and stored in the structure pointed by pData,
*         otherwise zero is returned if an error is encountered during the
*         process and in this case, a description of the problem is provided in
*         the standard error stream.
*
* The \ref computeOverlapMatrix function calculates S(Omega)_ij =
* int_Omega Orb(i)*Orb(j), where Orb(i) and Orb(j) are defined in
* pChemicalSystem, and where Omega corresponds to the tetrahedra of pMesh that
* are not labelled labelToAvoid. These coefficients of the overlap matrix are
* stored in the coef variable of the OverlapMatrix structure located at
* pData->pmat[iterationInTheLoop]. The computational time is reduced for
* restricted Hartree-Fock system i.e. when pParameters->orb_rhf is set to one.
* The evaluation of the integral uses a 14-point formula described in the
* article of Grundmann and Moller (april 1978, cf. (4.5), p.288) and which is
* the one used by default in Freefem++ software for computing integrals over
* three-dimensional tetrahedral meshes. In terms of computational time, this
* procedure is the most costly one in the mpd algorithm so a parallelization
* using openmp library is available. Hence, in order to use this function, the
* omp.h file must not be put in comment in the main.h file (and neither does
* the math.h file since some mathematical functions are also needed here).
* Furthermore, the -lm and -fopenmp options must be set when compiling the
* program with gcc (or link correctly the math and openmp libraries for other
* compilers and architectures). Finally, the \ref computeOverlapMatrix function
* should be static but has been defined as non-static in order to perform
* unit-test on it.
*/
int computeOverlapMatrix(Parameters* pParameters, Mesh* pMesh, Data* pData,
                         ChemicalSystem* pChemicalSystem, int labelToAvoid,
                                                        int iterationInTheLoop);
/**
* \fn int convertingType(int type)
* \brief It converts the type as given in the standard nomenclature of the
*        *.wfn format into an integer defined by a+10b+100c, where a, b, and c
*        stands for the number of x, y, and z in the electronic layer in
*        consideration, respectively. For example, the type of the layer
*        \ref ORB_4FXXY is converted into 12, while \ref ORB_3DXZ gives 101.
*
* \param[in] type An integer comprised between one and twenty (i.e. equal to one
*                 of the ORB_* preprocessor constants defined in main.h).
*                 Otherwise, for not slowing the computational time, a warning
*                 message is displayed in the standard output stream and the
*                 integer is treated as if it was equal to the \ref ORB_1S
*                 preprocessor constant.
*
* \return It returns the converted type (or zero i.e. the results for
*         type=\ref ORB_1S if the type variable is not comprised between one
*         and twenty).
*
* The \ref convertingType function should be static but has been defined as
* non-static in order to perform unit-tests on it.
*/
int convertingType(int type);

/**
* \fn double evaluateGaussianIntegral(int exp0, double t0, double t1)
* \brief It evaluates the exact analytical expression of the one-dimensional
*        Gaussian integral from t0 to t1 of t^exp0*exp(-t*t) with respect to
*        the t-variable, and according to the seven different cases for the
*        exponent variable (exp0=0...6).
*
* \param[in] exp0 An integer referring to the exponent present in the integrand
*                 of the one-dimensional Gaussian integral we aim to compute.
*                 It must be comprised between zero and six, otherwise the \ref
*                 evaluateGaussianIntegral function treats it as zero (i.e. the
*                 case of the erf function, up to a sqrt(pi) constant). Hence,
*                 we are only able to compute the integral for integrand of the
*                 form exp(-t^2), t*exp(-t^2), t^2*exp(-t^2), t^3*exp(-t^2),
*                 t^4*exp(-t^2), t^5*exp(-t^2), t^6*exp(-t^2).
*
* \param[in] t0 It corresponds to the starting value of the (oriented) interval
*               on which we wish to compute the one-dimensional integral.
*
* \param[in] t1 It corresponds to the ending value of the (oriented) interval
*               on which we wish to compute the one-dimensional integral.
*
* \return It returns the exact analytical value of the one-dimensional integral
*         from t0 to t1 of t^exp0*exp(-t*t) with respect to the t variable.
*
* In order to compute the exact analytical value of the one-dimension integral,
* we use some standard mathematical function such as the exponential map or the
* Gauss error function erf. Hence, the math.h file must not be put in comment
* in main.h and the -lm option must be set when compiling the program with gcc
* (or link properly the math library for other compilers and architectures).
* Furthermore, the \ref evaluateGaussianIntegral function should be static but
* has been defined as non-static in order to perform unit-tests on it.
*/
double evaluateGaussianIntegral(int exp0, double t0, double t1);

/**
* \fn double evaluateOneIntegralIJ(int expI, int expJ, double cI, double cJ,
*                                                          double t0, double t1)
* \brief It evaluates the exact analytical expression of the one-dimensional
*        integral from t0 to t1 of 2*(t-cI)^expI*(t+cJ)^expJ*exp(-t^2) with
*        respect to the t-variable, and according to the sixteen different cases
*        for the exponent variables (expI=0...3, expJ=0...3).
*
* \param[in] expI An integer referring to the exponent associated with the
*                 the input variable primitiveI of \ref evaluateTripleIntegralIJ
*                 function, and which is present in the integrand of the
*                 one-dimensional integral we aim to compute here. It must be
*                 comprised between zero and three, otherwise the \ref
*                 evaluateOneIntegralIJ function consider it is equal to zero
*                 (case of a 1s-type orbital).
*
* \param[in] expJ An integer referring to the exponent associated with the
*                 the input variable primitiveJ of \ref evaluateTripleIntegralIJ
*                 function, and which is also present in the integrand of the
*                 one-dimensional integral we aim to compute here. It must be
*                 comprised between zero and three, otherwise the \ref
*                 evaluateOneIntegralIJ function consider it is equal to zero
*                 (case of a 1s-type orbital).
*
* \param[in] cI It corresponds to the center obtained after the change of
*               variables performed in the \ref evaluateTripleIntegralIJ
*               function for the nucleus associated with primitiveI, and which
*               is present in the integrand of the one-dimensional integral we
*               aim to compute here.
*
* \param[in] cJ It corresponds to the center obtained after the change of
*               variables performed in the \ref evaluateTripleIntegralIJ
*               function for the nucleus associated with primitiveJ, and which
*               is present in the integrand of the one-dimensional integral we
*               aim to compute here.
*
* \param[in] t0 It corresponds to the starting value of the (oriented) interval
*               on which we wish to compute the one-dimensional integral.
*
* \param[in] t1 It corresponds to the ending value of the (oriented) interval
*               on which we wish to compute the one-dimensional integral.
*
* \return It returns the exact analytical value of the one-dimensional integral
*         from t0 to t1 of 2*(t-cI)^expI*(t+cJ)^expJ*exp(-t^2) with respect to
*         the t variable.
*
* In order to compute the exact analytical value of the one-dimension integral,
* the \ref evaluateOneIntegralIJ function simply expands the polynomial present
* in the integrand and then compute iteratively each mononomial integral thanks
* to the \ref evaluateGaussianIntegral function. This requires the use of some
* standard mathematical function such as the exponential map or the Gauss error
* function erf. Hence, the math.h file must not be put in comment in main.h and
* the -lm option must be set when compiling the program with gcc (or link
* properly the math library for other compilers and architectures). Furthermore,
* the \ref evaluateOneIntegralIJ function should be static but has been defined
* as non-static in order to perform unit-tests on it.
*/
double evaluateOneIntegralIJ(int expI, int expJ, double cI, double cJ,
                                                          double t0, double t1);

/**
* \fn double evaluateTripleIntegralIJ(Parameters* pParameters, Mesh *pMesh,
*                                     ChemicalSystem *pChemicalSystem,
*                                     MolecularOrbital *pMolecularOrbitalI,
*                                     MolecularOrbital *pMolecularOrbitalJ,
*                                     int primitiveI, int primitiveJ,
*                                                               int kHexahedron)
* \brief It evaluates on the kHexahedron of pMesh the exact analytical
*        expression for the three-dimensional integral of the product of
*        primitiveI by primitiveJ, whose descriptions are stored in the
*        structures pointed by pMolecularOrbitalI and pMolecularOrbitalJ in
*        pChemicalSystem, respectively.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its delta_x, delta_y and
*                        delta_z variables are used to compute fastly the
*                        coordinates of integration from the first vertex of
*                        the kHexahedron (rather than look into the coordinate
*                        description of the seventh vertex of kHexahedron),
*                        using here the fact that pMesh is pointing to an
*                        underlying cubic grid structure. For not slowing the
*                        process, we also assume that pParameters is not
*                        pointing to NULL.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function. In particular, we need to access to the three
*                  coordinates of the Point structure associated with the first
*                  vertex of its kHexahedron-th Hexahedron structure. For not
*                  slowing the process, we assume here that pMesh is not
*                  pointing to NULL, as well as its pver and phex variables.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. In particular,
*                            we need to access the three coordinates of the
*                            Nucleus structures related to pMolecularOrbitalI
*                            and pMolecularOrbitalJ. For not slowing the
*                            process, we assume here that pChemicalSystem is not
*                            pointing to NULL, as well as its pmorb and pnucl
*                            variables.
*
* \param[in] pMolecularOrbitalI A pointer that points the one of the
*                               MolecularOrbital structure stored in the array
*                               dynamically allocated for the pmorb variable
*                               of the structure pointed by pChemicalSystem.
*                               For not slowing the process, we assume that
*                               pMolecularOrbital is not pointing to NULL, as
*                               well as its coeff, exp, nucl and type variables.
*
* \param[in] pMolecularOrbitalJ A pointer that points to a MolecularOrbital
*                               structure (that may be identical to the one
*                               pointed by pMolecularOrbitalI) stored in the
*                               array dynamically allocated for the pmorb
*                               variable of the structure pointed by
*                               pChemicalSystem. For not slowing the process,
*                               we assume that pMolecularOrbital is not pointing
*                               to NULL, as well as its coeff, exp, nucl and
*                               type variables.
*
* \param[in] primitiveI It refers to the position of the Gaussian-type primitive
*                       in the array variables coeff, exp, nucl and type of the
*                       structure pointed by pMolecularOrbitalI. We assume that
*                       it is not negative and less than the total number of
*                       Gaussian-type primitives (stored in the
*                       pChemicalSystem->ngauss variable).
*
* \param[in] primitiveJ It refers to the position of the Gaussian-type primitive
*                       in the array variables coeff, exp, nucl and type of the
*                       structure pointed by pMolecularOrbitalJ. We assume that
*                       it is not negative and less than the total number of
*                       Gaussian-type primitives (stored in the
*                       pChemicalSystem->ngauss variable).
*
* \param[in] kHexahedron It refers to the position of the Hexahedron structure
*                        in the array pointed by pMesh->phex, which corresponds
*                        to the hexahedron on which we want to compute the
*                        three-dimensional integral. We assume that it is not
*                        negative and less than the total number of hexahedra in
*                        the mesh (stored in the pMesh->nhex variable).
*
* \return It returns the exact analytical value of the three-dimensional
*         integral of the product of primitiveI by primitiveJ over the
*         kHexahedron. For not slowing the process, if one coefficient related
*         to either primitiveI or primitiveJ is zero, the \ref
*         evaluateTripleIntegralIJ function immediatly returns zero.
*
* The \ref evaluateTripleIntegralIJ function uses standard mathematical function
* (such as sqrt, erf, pow, etc.) in order to compute the integral. Hence, the
* math.h file must not be put in comment in main.h and the -lm option must be
* set when compiling the program with gcc (or link properly the math library
* for other compilers and architectures). Furthermore, the \ref
* evaluateTripleIntegralIJ function should be static but has been defined as
* non-static in order to perform unit-tests on it. In order to get the exact
* expression for the integral, we use the fact that the domain of integration
* is a cube so the separation of variables can be applied on the integrand
* int_kHexahedron primitiveI*primitiveJ =
* int_MoinsX^PlusX (x-xInucl)^sublayerIx*(x-xJnucl)^sublayerJx * ... (similar
* expresion for the y- and z-coordinates). This is followed by a change of
* variables tx=sqrt(expI+expJ)*[x-(expI*xInucl+expJ*xJnucl)/(expI+expJ)] (with
* similar expressions for the y- and z-coordinates). We obtain that the integral
* is the product of three integrals of the form coefficient*int_tMoins^tPlus
* 2*(t-cI)^sublayerI*(t+cJ)^sublayerJ*exp(-t*t), where the coefficient is set to
* exp[expI*expJ*(xInucl-xJnucl)^2/(expI+expJ)] /
* [2*(expI+expJ)^((1+sublayerI+sublayerJ)/2)].
*/
double evaluateTripleIntegralIJ(Parameters* pParameters, Mesh *pMesh,
                                ChemicalSystem *pChemicalSystem,
                                MolecularOrbital *pMolecularOrbitalI,
                                MolecularOrbital *pMolecularOrbitalJ,
                               int primitiveI, int primitiveJ, int kHexahedron);

/**
* \fn int computeOverlapMatrixOnGrid(Parameters* pParameters, Mesh* pMesh,
*                                    Data* pData,
                                     ChemicalSystem* pChemicalSystem,
*                                    int labelToConsiderOrAvoid,
*                                                        int iterationInTheLoop)
* \brief It computes the analytical expresion for the coefficients of the
*        overlap matrix related to pChemicalSystem. The domain of integration
*        Omega corresponds to the hexahedra of pMesh that are NOT labelled
*        labelToConsiderOrAvoid if the iterationInTheLoop is set to zero or if
*        the pParameters->trick_matrix is set to zero, otherwise Omega
*        corresponds to the hexahedra of pMesh labelled labelToConsiderOrAvoid.
*        The corresponding values are saved in the coef array variable of the
*        OverlapMatrix structure located at pData->pmat[iterationInTheLoop].
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its orb_rhf variable is used
*                        to reduce the computational time when evaluating the
*                        coefficients of a restricted Hartree-Fock chemical
*                        system i.e. when any molecular orbital has a copy
*                        saved in pChemicalSystem but with a different spin.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref
*                  main function. We assume here that its phex variable
*                  is not pointing to NULL i.e. the mesh is made of
*                  hexahedra, otherwise an error is returned by the \ref
*                  computeOverlapMatrixOnGrid function.
*
* \param[out] pData A pointer that points to the Data structure of the \ref
*                   main function. Its pmat variable is used to fill (at the
*                   (iterationInTheLoop)-th position in the array so it must not
*                   point to NULL here) the OverlapMatrix structure with the
*                   coefficients in the array pointed by the corresponding coef
*                   variable.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It is storing
*                            the description of all the MolecularOrbital
*                            structures defined as a sum of Gaussian-type
*                            primitives.
*
* \param[in] labelToConsiderOrAvoid This integer represents the label of the
*                                   hexahedra on which we do or not want to
*                                   compute the integrals. More precisely, the
*                                   domain of integration Omega for the
*                                   coefficients of the overlap matrix
*                                   corresponds to the hexahedra of pMesh that
*                                   are NOT labelled labelToConsiderOrAvoid if
*                                   the iterationInTheLoop variable is set to
*                                   zero or if the pParameters->trick_matrix
*                                   variable is set to zero; otherwise Omega
*                                   corresponds to the hexahedra of pMesh
*                                   that are labelled labelToConsiderOrAvoid.
*                                   In our situation, we use the mmg3d sofware
*                                   convention in level-set mode for the
*                                   labelling of the structure pointed by pMesh.
*                                   In particular, 3 refers to the
*                                   hexahedra belonging to the interior of the
*                                   domain, while 2 refers to those that are
*                                   not. Hence, in order to compute the overlap
*                                   matrix on the domain when trick_matrix is
*                                   set to zero, labelToConsiderOrAvoid must be
*                                   set to 2. If we want to compute the
*                                   complement of the domain, then
*                                   labelToConsiderOrAvoid must be set to 3.
*                                   And similarly, if one wants to compute the
*                                   overlap matrix on the whole computational
*                                   box labelToConsiderOrAvoid must be set to
*                                   -1. Finally, in the case where
*                                   pParameters->trick_matrix is set to one,
*                                   we do not aim to recompute the whole overlap
*                                   matrix since pMesh has not changed from the
*                                   last iteration in the optimization loop
*                                   that this assume that we are not
*                                   considering an initial domain i.e. the
*                                   iterationInTheLoop variable is positive).
*                                   For this purpose, we only tag the hexahedra
*                                   on which we wish to compute the integrals
*                                   by changing the sign of their label. Hence,
*                                   -2 will remove the corresponding hexahedra
*                                   from the domain while -3 will add them and
*                                   the \ref computeOverlapMatrixOnGrid will
*                                   compute the resulting coefficients of the
*                                   associated overlap matrix. Other values are
*                                   forbidden here, and will make the \ref
*                                   computeOverlapMatrixOnGrid function return
*                                   an error.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations in the optimization loop. Hence,
*                               it must be a non-negative integer (zero refers
*                               to the initial domain after adaptation) not
*                               (strictly) greater than the maximal number of
*                               iterations allowed in the mpd algorithm (stored
*                               in the pParameters->iter_max variable). It
*                               allows the mpd program to save the coefficients
*                               of the overlap matrix at the correct position
*                               in the pData->pmat array of OverlapMatrix
*                               structures.
*
* \return It returns one if the coefficients of the overlap matrix have been
*         successfully computed and stored in the structure pointed by pData,
*         otherwise zero is returned if an error is encountered during the
*         process and in this case, a description of the problem is provided in
*         the standard error stream.
*
* The \ref computeOverlapMatrixOnGrid function reduces the computational time
* for restricted Hartree-Fock system i.e. when pParameters->orb_rhf is set to
* one. Contrarily to the function \ref computeOverlapMatrix which do the same
* job but for tetrahedral meshes, we can exploit the cubic grid structure in
* order to separate the variables in the integrals and thus compute an exact
* analytical expression for the coefficients of the overlap matrix. In terms of
* computational time, this procedure is the most costly one in the mpd algorithm
* so a parallelization using openmp library is available. Hence, in order to
* use this function, the omp.h file must not be put in comment in main.h (and
* neither does the math.h file since some standard mathematical functions such
* as the exponential map exp or the Gauss error function erf are also needed
* here). Furthermore, the -lm and -fopenmp options must be set when compiling
* the program with gcc (or link correctly the math and openmp libraries for
* other compilers and architectures). Finally, the \ref
* computeOverlapMatrixOnGrid function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int computeOverlapMatrixOnGrid(Parameters* pParameters, Mesh* pMesh,
                               Data* pData, ChemicalSystem* pChemicalSystem,
                               int labelToConsiderOrAvoid,
                                                        int iterationInTheLoop);

/**
* \fn int diagonalizeOverlapMatrix(Parameters* pParameters, Data* pData,
*                                                        int iterationInTheLoop)
* \brief It computes the eigenvalues and eigenvectors of the (square symmetric)
*        matrix whose coefficients are stored in the coef array variable of the
*        OverlapMatrix structure located at pData->pmat[iterationInTheLoop].
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its orb_rhf variable is used
*                        to reduce the computational time when diagonalizing
*                        the overlap matrix, while if its verbose variable is
*                        not set to zero, the computed coefficient, eigenvalues,
*                        and eigenvectors are printed in standard output stream.
*
* \param[in,out] pData A pointer that points to the Data structure of the \ref
*                      main function. Its pmat variable is used to get (at the
*                      (iterationInTheLoop)-th position in the array so it must
*                      not point to NULL here) the OverlapMatrix structure. Its
*                      coef array variable contains the coefficients of the
*                      symmetric (pData->nmat)x(pData->ndata)-matrix while
*                      its diag and vect are used to store the eigenvalues and
*                      eigenvectors of the matrix, respectively.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations in the optimization loop. Hence,
*                               it must be a non-negative integer (zero refers
*                               to the initial domain after adaptation). We
*                               also assume here that it is not be (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm. It allows the mpd
*                               program to load the coefficients and save the
*                               eigenvalues/eigenvectors of the overlap matrix
*                               at the correct position in the pData->pmat array
*                               of OverlapMatrix structures.
*
* \return It returns one if the eigenvalues and eigenvectors of the overlap
*         matrix have been successfully computed and stored in the structure
*         pointed by pData, otherwise zero is returned if an error is
*         encountered during the process.
*
* The \ref diagonalizeOverlapMatrix function diagonalizes the overlap matrix by
* using the dsyev routine of lapacke library (LAPACK interface for c), which
* computes the eigenvalues (and optionally the eigenvectors) associated with a
* real symmetric matrix. In order to use this function, the lapacke.h file must
* not be put in comment in the main.h file. Furthermore, the -llapacke option
* must be set when compiling the program with gcc (or link correctly the
* lapacke library for other compilers and architectures).
*/
int diagonalizeOverlapMatrix(Parameters* pParameters, Data* pData,
                                                        int iterationInTheLoop);

/**
* \fn int computeProbability(Parameters* pParameters, Data* pData,
*                                                        int iterationInTheLoop)
* \brief It computes the different probabilities to find exactly
*        a certain number of electrons in the domain whose values are saved in
*        the pk array variable of the Probabilities structure located at
*        pData->pprob[iterationInTheLoop]. The total population of electrons
*        inside the domain is also computed and saved in
*        pData->pop[iterationInTheLoop], while the probability to find
*        the pParameters->nu_electrons in the domain is saved in
*        pData->pnu[iterationInTheLoop].
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its verbose variable is used
*                        to detail the values of all the computed probabilities
*                        while its nu_electrons variable is used to locate the
*                        probability the user has prescribed.
*
* \param[in,out] pData A pointer that points to the Data structure of the \ref
*                      main function. Its pmat variable is used to get (at the
*                      (iterationInTheLoop)-th position in the array so it must
*                      not point to NULL here) the OverlapMatrix and
*                      Probabilities structures. The corresponding diag array
*                      variable is used to compute the different probabilities
*                      that are then saved in the pk array variable
*                      (calculations are performed on the pkl array variable).
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations in the optimization loop. Hence,
*                               it must be a non-negative integer (zero refers
*                               to the initial domain after adaptation). We
*                               also assume here that it is not be (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable). It allows the
*                               mpd program to load the eigenvalues and save the
*                               probabitilies and population at the correct
*                               position in the pData->pmat and pData->pprob
*                               arrays.
*
* \return It returns one if the eigenvalues and eigenvectors of the overlap
*         matrix have been successfully computed and stored in the structure
*         pointed by pData, otherwise zero is returned if an error is
*         encountered during the process.
*
* The \ref computeProbability function computes the probability P_nu(Omega) to
* find exactly a certain number nu of electrons inside a domain Omega, where
* exactly means that the complement of Omega has to contain the total number ne
* of electrons minus the nu ones. For this purpose, we use the recursive formula
* (5)-(7) of the article of Cancès, Keriven, Lodier, and  Savin (2004, p.374),
* which allows to compute recursively the probabilities in O(ne^2) operations
* thanks to the eigenvalues of the overlap matrix associated with the
* Hartree-Fock chemical system. We also evaluate the total population of
* electrons inside the domain Omega by using the fact that this quantity is
* equal to sum_nu nu*P_nu(Omega). The \ref computeProbability function should be
* static but has been defined as non-static in order to perform unit-test on it.
*/
int computeProbability(Parameters* pParameters, Data* pData,
                                                        int iterationInTheLoop);

/**
* \fn int AddOrRemoveHexahedronToMesh(Mesh* pMesh, int j, int addOrRemove)
* \brief It modifies the label associated with one of the two hexahedra that
*        are having the j-th quadrilateral in common, and corresponding to the
*        boundary of the domain inside the computational box.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function. For not slowing the process, we assume
*                      that its phex and padj variables are not pointing to
*                      NULL i.e. the corresponding array have been filled.
*                      Indeed, the two hexahedra sharing the j-th quadrilateral
*                      are quickly identified only because to the Adjacency
*                      structures built and stored in pMesh->padj.
*
* \param[in] j An integer referring to the position of the array pointed by
*              the padj variable of the structure pointed by pMesh. This
*              array corresponds to the Adjacency structures associated with
*              the quadrliateral that forms the boundary of the domain inside
*              the computational box. Hence, for not slowing the process, we
*              assume here that j is not negative and less than the total number
*              of quadrilaterals representing the boundary of the domain (stored
*              in the pMesh->nadj variable).
*
* \param[in] addOrRemove This integer rules the modification of the label of the
*                        hexahedron concerned. More precisely, if the
*                        addOrRemove variable is set to 1, it adds to the domain
*                        the hexahedron that do not belong to it by changing its
*                        label from 2 to 3 (we recall here that we use the same
*                        convention than the mmg3d software: internal elements
*                        are labelled three otherwise two). If set to -1, it
*                        removes the hexahedron that belongs to the domain by
*                        changing its label from 3 to 2. Finally, if set to -3,
*                        the external hexahedron is added (as if addOrRemove was
*                        set to one) but we use a negative sign to maybe locate
*                        fastly this type of element later in the algorithm.
*                        In particular, this is the trick performed when the
*                        trick_matrix variable of the Parameters structure of
*                        the \ref main function is set to one. Indeed, when the
*                        \ref AddOrRemoveHexahedronToMesh function is used, it
*                        still remains to check if the fact of adding such
*                        elements to the domain will increase the probability.
*                        For this purpose, instead of changing its label from 2
*                        to 3, we change it from 2 to -3, the sign allowing us
*                        to locate fastly this element later in the algorithm
*                        (especially in the \ref computeOverlapMatrixOnGrid
*                        function). Similarly, if addOrRemove is set to -2, the
*                        internal hexahedron is "pendingly" removed i.e. its
*                        label is changed from 3 to -2. We also assume that the
*                        AddOrRemove variable does not take other values than
*                        the ones described above, otherwise a warning is*
*                        displayed in the standard output stream.
*
* \return It returns the position of the hexahedron (for which the label has
*         been modified) in the array phex variable of the structure pointed by
*         pMesh. If the addOrRemove variable is not set to (+/1)one, minus two,
*         or minus three, zero is returned by the \ref
*         AddOrRemoveHexahedronToMesh function and an error is displayed in the
*         standard output stream.
*/
int AddOrRemoveHexahedronToMesh(Mesh* pMesh, int j, int addOrRemove);

/**
* \fn double evaluateShapeGradientAtThePoint(double px, double py, double pz,
*                                            int nProb,
*                                            Probabilities* pProbabilities,
*                                            int nu,
*                                            OverlapMatrix* pOverlapMatrix,
*                                               ChemicalSystem* pChemicalSystem)
* \brief It evaluates at the point (px,py,pz) the shape gradient of the
*        probability (to find exactly nu electrons) thanks to the overlap
*        matrix of pChemicalSystem, and more precisely thanks to the
*        corresponding eigenvalues and eigenvectors stored in the structure
*        pointed by pOverlapMatrix. The calculations are performed recursively,
*        using the pkl (nProb)x(nProb)-array variable of the structure pointed
*        by pProbabilities.
*
* \param[in] px It corresponds to the first coordinate of the point at which
*               we wish to compute the shape gradient.
*
* \param[in] py It corresponds to the second coordinate of the point at which
*               we wish to compute the shape gradient.
*
* \param[in] pz It corresponds to the third coordinate of the point at which
*               we wish to compute the shape gradient.
*
* \param[in] nProb For not slowing the computational time, we assume here that
*                  this integer corresponds to the total number of electrons
*                  in pChemicalSystem plus one i.e. (nProb)x(nProb) is the size
*                  of the matrix stored in the pkl variable of the structure
*                  pointed by pProbabilities.
*
* \param[in] pProbabilities A pointer that points to the Probabilities structure
*                           whose pkl variable will be used to store the
*                           intermediate calculations of the shape gradient.
*                           For not slowing the computational time, we assume it
*                           is not pointing to NULL.
*
* \param[in] nu For not slowing the computational time, we assume here that this
*               integer corresponds to the number of electrons we are looking
*               for (in terms of probability) and that it is positive and
*               (strictly) less that the nProb variable.
*
* \param[in] pOverlapMatrix A pointer that points to the OverlapMatrix structure
*                           whose vect and diag variable will be used to
*                           compute the shape gradient recursively. For not
*                           slowing the computational time, we assume that it is
*                           not pointing to NULL and that the corresponding
*                           overlap matrix has already been diagonalized.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. In particular,
*                            for not slowing the process, we assume here that
*                            its nmorb variable is storing the total number of
*                            electrons i.e. a positive integer equal to the
*                            total number of molecular orbitals (Hartree-Fock)
*                            and that it is not pointing to NULL as well as its
*                            pmorb variable.
*
* \return It returns the value of the shape gradient of the probability at the
*         point (px,py,pz) by using the recursive formula of the article of
*         Cancès, Keriven, Lodier, and Savin (2004, above (15) p.376) which
*         allows to compute recursively the shape gradient in
*         O(pChemicalSystem->nmorb^2) operations thanks to the
*         eigenvalues and eigenvectors of the overlap matrix associated with the
*         Hartree-Fock chemical system.
*
* The \ref evaluateShapeGradientAtThePoint function should be static but has
* been defined as non-static in order to perform unit-tests on it. It also calls
* the \ref evaluateOrbitalsIJ and \ref evaluateOrbitalsII functions which use
* standard mathematical functions such as the exponential map. Hence, in order
* to use this function properly the math.h library must not be put in comment in
* main.h file and the -lm options must be set at compilation with gcc (or link
* properly the math library for other compilers and architectures).
*/

double evaluateShapeGradientAtThePoint(double px, double py, double pz,
                                       int nProb, Probabilities* pProbabilities,
                                       int nu, OverlapMatrix* pOverlapMatrix,
                                               ChemicalSystem* pChemicalSystem);

/**
* \fn int computeShapeGradient(Parameters* pParameters, Mesh* pMesh,
*                              Data* pData, ChemicalSystem* pChemicalSystem,
*                                                        int iterationInTheLoop)
* \brief It computes the shape gradient of the probability at the vertices of
*        pMesh if pParameters->opt_mode is positive, otherwise at the center of
*        the quadrilateral representing the boundary of the internal domain. In
*        the latter case, it also directly modifies the MPD domain depending on
*        the sign of the shape gradient. The gradient evaluation uses the
*        eigenvalues and eigenvectors of the overlap matrix associated with
*        pChemicalSystem and stored in pData->pmat[iterationInTheLoop]. In the
*        case of tetrahedral meshes i.e. if pParamters->opt_mode is positive,
*        it also labels 10 the interior points that belongs to the boundary of
*        the internal domain.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its opt_mode variable is used
*                        to distinguish the tetrahedral mesh from the hexahedral
*                        one.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref
*                  main function. In the case where the mesh is made of
*                  hexahedra, its Adjacency structures are used to locate
*                  fastly the two hexahedra sharing a boundary quadrilateral of
*                  the internal domain, which is modified by the \ref
*                  computeShapeGradient function, depending on the sign
*                  of the computed shape gradient at the center point of the
*                  quadrilateral considered (and also at the center of the
*                  next quadrilateral in the direction of its outer
*                  normal vector).
*
* \param[in,out] pData A pointer that points to the Data structure of the \ref
*                      main function. Its pmat variable is used to get (at the
*                      (iterationInTheLoop)-th position in the array so it must
*                      not point to NULL here) the OverlapMatrix and
*                      Probabilities structures. The corresponding diag and vect
*                      array variables are used to compute the shape gradient
*                      (calculations are performed on the pkl array variable).
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It is storing
*                            the description of all the MolecularOrbital
*                            structures defined as a sum of Gaussian-type
*                            primitives, which are used in the computation of
*                            the shape gradient.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations in the optimization loop. Hence,
*                               it must be a non-negative integer (zero refers
*                               to the initial domain after adaptation). We
*                               also assume here that it is not be (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable). It allows the
*                               mpd program to load the eigenvalues at the
*                               correct position in the pData->pmat array.
*
* \return It returns one if the shape gradient has been successfully computed
*         and stored in the value variable of the pMesh vertices for tetrahedral
*         meshes, while for hexahedral meshes the internal domain has been
*         successively modified according to the sign of the shape gradient.
*         If an error is encountered during the process or if the input
*         variables do not have the expected content, zero is returned by the
*         \ref computeShapeGradient function.
*
* The \ref computeShapeGradient function computes the shape gradient of the
* probability by using the recursive formula of the article of Cancès, Keriven,
* Lodier, and Savin (2004, above (15) p.376) which allows to compute recursively
* the probabilities in O(NumberOfMolecularOrbitals^2) operations thanks to the
* eigenvalues and eigenvectors of the overlap matrix associated with the
* Hartree-Fock chemical system. The \ref computeShapeGradient function should
* be static but has been defined as non-static in order to perform unit-test on
* it. It also calls the \ref evaluateShapeGradientAtThePoint function which uses
* standard mathematical functions such as the exponential map. Hence, in order
* to use this function properly the math.h library must not be put in comment in
* main.h file and the -lm options must be set at compilation with gcc (or link
* properly the math library for other compilers and architectures).
*/
int computeShapeGradient(Parameters* pParameters, Mesh* pMesh, Data* pData,
                         ChemicalSystem* pChemicalSystem,
                                                        int iterationInTheLoop);

/**
* \fn double computeAreaTriangle(Point* pp1, Point* pp2, Point* pp3)
* \brief It computes the area of a triangle defined by its three vertices.
*
* \param[in] pp1 A pointer that points to the Point structure of the first
*                vertex associated with the triangle we wish to compute the
*                area.
*
* \param[in] pp2 A pointer that points to the Point structure of the second
*                vertex associated with the triangle we wish to compute the
*                area.
*
* \param[in] pp3 A pointer that points to the Point structure of the third
*                vertex associated with the triangle we wish to compute the
*                area.
*
*
* \return It returns the area of the triangle defined by the three vertices
*         pointed by pp1, pp2, and pp3. More precisely, we return the half of
*         the norm of the vector product between frame pp1 pp2 and pp1 pp3,
*         which corresponds to the area of the triangle.
*
* The \ref computeAreaTriangle function should be static but has been defined
* as non-static in order to perform unit-test on it. In order not to slow the
* process, let us mention here that we implicitly assume here that the input
* variables pp1, pp2, and pp3 are not pointing to NULL. It also uses the square
* root mathematical function. Hence, in order to use this function properly the
* math.h library must not be put in comment in main.h file and the -lm options
* must be set at compilation with gcc (or link properly the math library for
* other compilers and architectures).
*/
double computeAreaTriangle(Point* pp1, Point* pp2, Point* pp3);

/**
* \fn int computeShapeResidual(Parameters* pParameters, Mesh* pMesh,
*                              Data* pData, ChemicalSystem* pChemicalSystem,
*                                                        int iterationInTheLoop)
* \brief It computes the two-dimensional integral of the square of the shape
*        gradient of the probability (to find pParameters->nu_electrons) over
*        the boundary of the internal domain associated with the structure
*        pointed by pMesh. It corresponds to the shape derivative of the
*        probability taken in the direction of the shape gradient and represents
*        the residual of the first-order optimality conditions for the shape
*        optimization problem. The integral evaluation uses the
*        eigenvalues and eigenvectors of the overlap matrix associated with
*        pChemicalSystem and stored in pData->pmat[iterationInTheLoop].
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its opt_mode variable is used
*                        to distinguish the tetrahedral mesh from the hexahedral
*                        one.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref
*                  main function. In the case where the mesh is made of
*                  hexahedra, its Adjacency structures are used to locate
*                  fastly the boundary quadrilaterals of the internal domain.
*
* \param[in,out] pData A pointer that points to the Data structure of the \ref
*                      main function. Its pmat variable is used to get (at the
*                      (iterationInTheLoop)-th position in the array so it must
*                      not point to NULL here) the OverlapMatrix and
*                      Probabilities structures. The corresponding diag and vect
*                      array variables are used to compute the shape gradient
*                      (calculations are performed on the pkl array variable) at
*                      the interpolation points while the residual is stored in
*                      the pData->d1p[iterationInTheLoop] variable.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It is storing
*                            the description of all the MolecularOrbital
*                            structures defined as a sum of Gaussian-type
*                            primitives, which are used in the computation of
*                            the shape gradient.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations in the optimization loop. Hence,
*                               it must be a non-negative integer (zero refers
*                               to the initial domain after adaptation). We
*                               also assume here that it is not be (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable). It allows the
*                               mpd program to load the eigenvalues at the
*                               correct position in the pData->pmat array.
*
* \return It returns one if the two-dimensional integral has been succesfully
*         computed and stored in the pData->d1p[iterationInTheLoop] variable.
*         Otherwise, zero is returned if an error is encountered during the
*         process.
*
* The \ref computeShapeResidual function computes the two-dimensional integral
* thanks to the interpolation formula of the article of Cowper (1973) for
* triangulated surface (see \ref CST_ONE for further details). In the case of
* hexahedral meshes i.e. if pParameters->opt_mode is not positive, the boundary
* quadrilateral are divided in two triangles. The evaluation of the shape
* gradient is done thanks to the \ref evaluateShapeGradientAtThePoint function.
* The \ref computeShapeResidual function should be static but has been defined
* as non-static in order to perform unit-test on it. It also calls the \ref
* evaluateShapeGradientAtThePoint function which uses standard mathematical
* functions such as the exponential map. Hence, in order to use this function
* properly the math.h library must not be put in comment in main.h file and the
* -lm options must be set at compilation with gcc (or link properly the math
* library for other compilers and architectures).
*/
int computeShapeResidual(Parameters* pParameters, Mesh* pMesh, Data* pData,
                         ChemicalSystem* pChemicalSystem,
                                                        int iterationInTheLoop);
/**
* \fn int shapeDerivative(Parameters* pParameters, Mesh* pMesh, Data* pData,
*                       ChemicalSystem* pChemicalSystem, int iterationInTheLoop)
* \brief It computes the (symmetric) overlap matrix, its eigenvalues and
*        associated eigenvectors, the total population inside the domain, 
*        the probability, its shape gradient, and associated residual at
*        iterationInTheLoop in the optimization loop algorithm. In the case
*        of hexahedral meshes with level-set perturbations, it also modifies the
*        mesh according to the sign of the shape gradient close to the boundary
*        quadrilaterals.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its opt_mode variable is used
*                        to distinguish the tetrahedral meshes (positive value)
*                        from hexahedral ones. In the latter case, if its
*                        trick_matrix variable is set to one (and if the
*                        iterationInTheLoop variable is positive), instead of
*                        recomputing the overlap matrix on the whole domaine at
*                        each iteration, a trick is performed by computing only
*                        the integrals on hexahedra that needs to be removed or
*                        added, and remove or add the obtained quantities from
*                        the older iterationInTheLoop-1)-th corresponding
*                        values.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function. The elements must follows the conventions
*                      used by the mmg3d software i.e. internal elements must be
*                      labelled (+/-)3 whereas elements that do not belongs to
*                      the domain must be labbeled (+/-)2. The boundary elements
*                      that do not belong to the boundary of the computational
*                      box, and which therefore corresponds to the boundary of
*                      the internal domain, must be labelled 10.
*
* \param[out] pData A pointer that points to the Data structure of the \ref
*                   main function. Its pnu variable is used to store the
*                   probability to find pParameters->nu_electrons in the domain
*                   at the (iterationInTheLoop)-th position in the array.
*                   Similarly, its pop and d1p variables are used to store
*                   respectively the total population of electrons inside the
*                   domain, and the shape derivative of the probability taken in
*                   the direction of the shape gradient (int_boundary_Omega
*                   dP/dOmega)^2), and which corresponds to the residual of the
*                   first-order optimility condition to the MPD optimization
*                   problem. Finally, again at the (iterationInTheLoop)-th
*                   position in the array, its pprob (respectively pmat)
*                   variables stores as OverlapMatrix (resp. Probabilities)
*                   structures the (symmetric) overlap matrix coef(ficients),
*                   diag(onal) elements resulting from a diagonalization
*                   process i.e. the overlap matrix eigenvalues, and its 
*                   associated (eigen)vect(ors) (resp. the different computed
*                   p(robabilities to find )k electrons in the domain and a
*                   pkl matrix used to compute recursively the quantities
*                   thanks to the formula of Cancès, Keriven, Lodier, and Savin
*                   (2004)). 
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It is storing
*                            the description of all the MolecularOrbital
*                            structures defined as a sum of Gaussian-type
*                            primitives, which are used in the computation of
*                            the overlap matrix and shape gradient.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations already performed in the
*                               optimization loop. Hence, it must be a
*                               non-negative integer (zero refers to the
*                               initial domain after adaptation) not (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable).
*
* \return It returns one on success otherwise zero is returned if an error is
*         encountered during the process or if the input variables do not have
*         the expected content. In the latter case, a description of the problem
*         is provided in the standard error stream before returning zero.
*
* The \ref shapeDerivative function executes successively the \ref
* computeOverlapMatrix (respectively the \ref computeOverlapMatrixOnGrid) for
* tetrahedral mesh (resp. for hexahedral meshes), \ref diagonalizeOverlapMatrix,
* \ref computeProbability, \ref computeShapeGradient, and \ref
* computeShapeResidual function. We refer to their descriptions for further
* details. The function should be static but has been defined as non-static in
* order to perform unit-test on it. Let us only mention that it uses standard
* mathematical functions, parallelize with openmp the evaluation of the overlap
* matrix, and diagonalize it thanks to the dsyev routine of the lapacke library
* (LAPACK interface for C). Hence, in order to use this function, the -lm 
* -llapacke -fopenmp options must be set at compilation with gcc (or link
* properly the math.h, lapacke.h and omp.h aasociated libraries) and the math.h
* omp.h and lapacke.h files must not be put in comment in main.h file. 
*/
int shapeDerivative(Parameters* pParameters, Mesh* pMesh, Data* pData,
                       ChemicalSystem* pChemicalSystem, int iterationInTheLoop);

/**
* \fn int writingShapeSolFile(Parameters* pParameters, Mesh* pMesh)
* \brief It writes into a (vectorial) *.sol file the values associated with the
*        product of what is stored in pMesh->pver[i].value by the associated
*        normal vector coordinates, where i stands the vertices that belongs to
*        the boundary of the MPD domain; otherwise the values written are zero.
*        The *.sol file name is related to the *.mesh file whose path name is
*        stored in pParameters->name_mesh variable.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable must
*                        refer to the name of an existing *.mesh file otherwise
*                        an error is returned by the \ref writingShapeSolFile
*                        function. We also recall that the name of the *.sol
*                        file is built on the *.mesh file where the '.mesh'
*                        extension has been replaced by the '.sol' one. We also
*                        recall that if a *.sol file with a similar name already
*                        exists, it will be reset and overwritten by the \ref
*                        writingShapeSolFile function.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function. In each Point structure of pMesh->pver that belongs
*                  to the boundary of the MPD domain, the value variable is
*                  intended to store the coefficient that is multiplied by the
*                  coordinate of the normal vector to form the content of the
*                  *.sol file. We emphasize the fact that if its label is set
*                  not to 10 (apart from points on the boundary of the
*                  computational box), an error is returned by the \ref
*                  writingShapeSolFile function. For vertices
*                  that do not belongs to the boundary, we save the value
*                  (0,0,0) and the label is set to zero (apart from those
*                  belonging to the boundary of the the computational box whose
*                  labels remain unchanged throughout the algorithm).
*
* \return It returns one if the vectorial solutions at each vertex were
*         successfully written by using the *.sol format. Otherwise, zero is
*         returned if any error is encountered during the writing process.
*
* The \ref writingShapeSolFile function strongly uses the fact that the normal
* vectors are listed in pMesh->pnorm in increasing order in terms of their
* vertex references. In particular, it returns an error if it is not the case.
* This function should be static but has been defined as non-static in order to
* perform unit-test on it.
*/
int writingShapeSolFile(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int writingObjFile(Parameters* pParameters, Mesh* pMesh)
* \brief It writes into a *.obj file format the three coordinates of the
*        vertices of an internal domain contained in the structure pointed by
*        pMesh, and also the boundary triangles associated to it (labelled 10)
*        with the right vertex connectivity. The name of the *.obj file is
*        related to the associated *.mesh file whose path name is stored in the
*        pParameters->name_mesh variable.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable must
*                        refer to the name of an existing *.mesh file otherwise
*                        an error is returned by the \ref writingObjFile
*                        function. We also recall that the name of the *.obj
*                        file is built on the *.mesh file where the '.mesh'
*                        extension has been replaced by the '.obj' one. We also
*                        recall that if a *.obj file with a similar name already
*                        exists, it will be reset and overwritten by the \ref
*                        writingObjFile function.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function. Each Point structure of pMesh->pver that belongs
*                  to the boundary of the MPD domain (triangles labelled 10)
*                  will be saved according to the *.obj file format ("v x y z"
*                  followed by "f p1 p2 p3").
*
* \return It returns one if the surface mesh associated with the internal domain
*         is successfully written by using the *.obj format. Otherwise, zero is
*         returned if any error is encountered during the writing process.
*
* The \ref writingObjFile function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int writingObjFile(Parameters* pParameters, Mesh* pMesh);

/**
* \fn int saveOrRemoveMeshInTheLoop(Parameters* pParameters, Mesh* pMesh,
*                                   ChemicalSystem* pChemicalSystem,
*                                    int booleanForSave, int iterationInTheLoop)
* \brief It is used at the (iterationInTheLoop)-th iteration in the optimization
*        loop to either remove or save the *.(iterationInTheLoop).mesh file,
*        depending on the fact that booleanForSave is set to zero or not,
*        respectively. Depending on the pParameters->save_type variable, the
*        mesh may be saved also (or only) in the *.cube/ *.obj format, which
*        requires the charge and center coordinates of the nuclei associated
*        with structure pointed by pChemicalSystem.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_mesh variable must
*                        refer to the name of a valid and existing *.mesh file
*                        otherwise an error is returned by the \ref
*                        saveOrRemoveMeshInTheLoop. Its save_type variable
*                        rules if the mesh file is saved in the *.mesh format
*                        (one), *.cube/ *.obj format (zero), or both formats
*                        (two). The *.cube is created for hexahedral meshes
*                        (if Parameters->opt_mode is not positive) whereas the
*                        *.obj format is generated for tetrahedral ones.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function. It contains the values that are intended to be
*                  saved if the booleanForSave variable is set to one. 
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It contains
*                            the charge and center coodinates of the nuclei
*                            that are intended to be saved in the *.cube file
*                            if the if the booleanForSave variable is set to
*                            one (and if the pParameters->save_type is not set
*                            to one). 
*
* \param[in] booleanForSave An integer that refers to the saving or not of the
*                           mesh. If set to 0, the *.(iterationInTheLoop).mesh
*                           file name is built from the *.mesh one stored in
*                           the name_mesh variable of the structure pointed by
*                           pParameters, and it is deleted if it exists.
*                           Otherwise, booleanForSave must be set to one and
*                           in this case, the mesh contained in pMesh is saved
*                           in the *.(iterationInTheLoop).mesh or
*                           *.(iterationInTheLoop).cube format, depending on
*                           the save_type variable of the structure pointed by
*                           pParameters.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations already performed in the
*                               optimization loop. Hence, it must be a
*                               non-negative integer (zero refers to the
*                               initial domain after adaptation) not (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable).
*
* \return It returns one on success otherwise zero is returned if an error is
*         encountered during the process or if the input variables do not have
*         the expected content. In the latter case, a description of the problem
*         is provided in the standard error stream before returning zero.
*
* The \ref saveOrRemoveMeshInTheLoop function should be static but has been
* defined as non-static in order to perform unit-test on it.
*/
int saveOrRemoveMeshInTheLoop(Parameters* pParameters, Mesh* pMesh,
                              ChemicalSystem* pChemicalSystem,
                                    int booleanForSave, int iterationInTheLoop);

/**
* \fn int saveDataInTheLoop(Parameters* pParameters, Mesh* pMesh, Data* pData,
*                           ChemicalSystem* pChemicalSystem,
*                           int iterationInTheLoop,
*                           time_t* pGlobalInitialTimer,
*                              time_t* pStartLocalTimer, time_t* pEndLocalTimer)
* \brief It finishes to evaluate the non-already-computed data at the (end of
*        the) (iterationInTheLoop)-th iteration of the optimization loop, and
*        according to pParameters->save_data, it saves all the calculated values
*        stored in pData by appending them in the *.data file whose name is
*        given by the pParameters->name_data variable. Furthermore, the mesh is
*        saved in an *.(iterationInTheLoop).mesh/ *.(iterationInTheLoop).cube
*        file thanks to the \ref saveOrRemoveMeshInTheLoop function and
*        according to the pParameters->save_mesh variable, the previous
*        *.(iterationInTheLoop-1).mesh/ *.(iterationInTheLoop-1).cube file is
*        deleted thanks to the \ref saveOrRemoveMeshInTheLoop function. We
*        emphasize the fact the mesh and data are automatically saved at
*        initialization i.e. if iterationInTheLoop is set to zero, and the
*        pGlobalInitialTimer, pStartLocalTimer, and pEndLocalTimer variable are
*        used to compute the execution time of the iteration and the cumulated
*        time.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function. Its name_data variable must
*                        refer to the name of a valid *.data file. However, the
*                        saving of data is always performed in appending mode
*                        i.e. if the file does not exists it is created, 
*                        otherwise the data are written right after the already
*                        existing content. Its save_data variable rules the
*                        frequency of the saving. If it is positive and a
*                        multiple of iterationInTheLoop, data are saved;
*                        otherwise they are not. Data are automatically saved
*                        at initialization i.e. if iterationInTheLoop is set to
*                        zero. The mesh at the iterationInTheLoop iteration is
*                        automatically saved in a *.(iterationInTheLoop).mesh/
*                        *.(iterationInTheLoop).cube file thanks to the \ref
*                        saveOrRemoveMeshInTheLoop function. Since the \ref
*                        saveDataInTheLoop function is used at the end of the
*                        optimization loop, this mesh file is in fact containing
*                        the mesh associated with the new domain i.e. the
*                        (iterationInTheLoop+1)-th one. This ensures the user to
*                        get a saving of the mesh in case of crash during the
*                        next loop, which is therefore deleted by this same
*                        function at the end if everything went right. Hence,
*                        in other words, the \ref saveDataInTheLoop function
*                        also deletes the *.(iterationInTheLoop-1).mesh/ 
*                        *.(iterationInTheLoop-1).cube file thanks to the \ref
*                        saveOrRemoveMeshInTheLoop function, except if this
*                        previous mesh wants to be really saved by the user i.e.
*                        if the save_mesh variable of the structure pointed by
*                        pParameters is positive and a multiple of
*                        (iterationInTheLoop-1).
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function. It contains the values that are intended to be
*                  saved at the iterationInTheLoop-th iteration of the
*                  optimization loop.
*
* \param[in] pData A pointer that points to the Data structure of the \ref main
*                  function. In the \ref optimization function, the \ref
*                  saveDataInTheLoop function is placed after the \ref
*                  shapeDerivative one. Hence, the overlap matrix coefficients
*                  (stored in pData->pmat[iterationInTheLoop].coef), eigenvalues
*                  (pData->pmat[iterationInTheLoop].diag), and eigenvectors
*                  (pData->pmat[iterationInTheLoop].vect), the probability to
*                  find exactmy pParameters->nu_electrons inside the domain of
*                  pMesh and pChemicalSystem->nmorb-pParameters->nu_electrons
*                  (pData->pnu[iterationInTheLoop]) and the other computed
*                  probabilities (pData->prob[iterationInTheLoop].pk), the total
*                  population of electrons (pData->pop[iterationInTheLoop])
*                  inside the domain, and the shape derivative residual
*                  (pData->d1p[iterationInTheLoop]) are already computed at the
*                  (iterationInTheLoop)-th iteration. The \ref saveDataInTheLoop
*                  thus finishes to compute the Data structure by evaluating the
*                  probability difference in pData->d0p[iterationInTheLoop] (at
*                  initialization i.e. if iterationInTheLoop is set to zero, we
*                  just initialize it to pData->pnu[iterationInTheLoop]) and
*                  the second order quantity. If the mesh is made of tetrahedra
*                  i.e. if pParameters->opt_mode is positive, the
*                  pData->d2p[iterationInTheLoop] variable stores the absolute 
*                  residual difference, otherwise it stores the second-order
*                  probability difference which is in particular used in the
*                  MPD algorithm to detect oscillations of hexahedral meshes in
*                  the optimization mode where the evolution of the mesh is
*                  locally ruled by the sign of the shape gradient at some
*                  boundary points of the internal domain (for example when
*                  pParameters->opt_mode==-1). Finally, the
*                  pData->tim[iterationInTheLoop] variable is filled with the
*                  execution time of the algorithm at the
*                  (iterationInTheLoop)-th iteration of the optimization loop.
*                  This quantity is computed thanks to the contents pointed by
*                  the input variables pStartLocalTimer and pEndLocalTimer.
*                  Similarly, pData->ctim[iterationInTheLoop] contains the
*                  the cumulated time calculated thanks to pGlobalInitialTimer
*                  and pEndLocalTimer. Apart from the Probabilities and
*                  OverlapMatrix structures, all these described quantities
*                  quantities are saved in the *.data file according to the
*                  pParameters->save_data variable. At initialization, data
*                  are automatically saved and a line specifying the date and
*                  time of the program is saved, as well as the total number
*                  of electrons and the pParameters->nu_electrons one, followed
*                  by a description of the computed initial quantities.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function. It contains
*                            the charge and center coordinates of the nuclei
*                            that may be needed by the \ref
*                            saveOrRemoveMeshInTheLoop function to save the
*                            mesh in a *.cube format, depending on the
*                            pParameters->save_type variable (i.e. if not set
*                            to one). 
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations already performed in the
*                               optimization loop. Hence, it must be a
*                               non-negative integer (zero refers to the
*                               initial domain after adaptation) not (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable).
*
* \param[in] pGlobalInitialTimer A pointer that points to the address of the
*                                globalInitialTimer variable globally defined in
*                                the main.c file to store the time at which the
*                                MPD program is launched, and thus print the
*                                total execution time in any case of error
*                                (even the non-anticipated one, see \ref
*                                endTimerAtExit function). It thus allows the
*                                \ref saveDataInTheLoop function to compute the
*                                cumulated execution time i.e. the total
*                                execution time from the starting of the program
*                                until the current iterationInTheLoop, saved in
*                                the pData->[iterationInTheLoop].ctim variable.
*
* \param[in] pStartLocalTimer A pointer that points to the address of the local
*                             startLocalTimer variable of the \ref main
*                             function, which is used to store successively the
*                             time at which the (iterationInTheLoop)-th
*                             iteration started. It thus allows the \ref
*                             saveDataInTheLoop function to compute the
*                             total execution time of the current
*                             iterationInTheLoop, saved in the
*                             pData->[iterationInTheLoop].tim variable.
*
* \param[in] pEndLocalTimer A pointer that points to the address of the local
*                           endLocalTimer variable of the \ref main function,
*                           which is used to store successively the time at
*                           which the (iterationInTheLoop)-th iteration ended,
*                           and which is calculated by the \ref
*                           saveDataInTheLoop function. It also allows the \ref
*                           saveDataInTheLoop function to compute the total
*                           execution time of the current iterationInTheLoop
*                           and the cumulated one, saved in the 
*                           pData->[iterationInTheLoop].tim and the
*                           pData->[iterationInTheLoop].ctim variables.
*
* \return It returns one on success otherwise zero is returned if an error is
*         encountered during the process or if the input variables do not have
*         the expected content. In the latter case, a description of the problem
*         is provided in the standard error stream before returning zero.
*
* The \ref saveDataInTheLoop function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int saveDataInTheLoop(Parameters* pParameters, Mesh* pMesh, Data* pData,
                      ChemicalSystem* pChemicalSystem, int iterationInTheLoop,
                      time_t* pGlobalInitialTimer, time_t* pStartLocalTimer,
                                                        time_t* pEndLocalTimer);

/**
* \fn int setupInitialData(Parameters* pParameters, Mesh* pMesh, Data* pData,
*                          ChemicalSystem* pChemicalSystem,
*                          time_t* pGlobalInitialTimer,
                               time_t* pStartLocalTimer, time_t* pEndLocalTimer)
* \brief It allocates memory for the saving of pData, enlarge of 10 letters the
*        maximal length allowed for path names, reallocates memory accordingly
*        for the pParameters->name_* variables, write an *.input file
*        recapitulating all the parameters used (both default ones and those
*        prescribed by the user in the *.info (input) file, set up the number
*        of threads/cpu for the parallel calculations (stored in the 
*        pParameters->n_cpu variable), compute all the quantities
*        (probability, ...), save them in the *.data file given by
*        the pParameters->name_data variable, and save also the mesh in a
*        *.0.mesh file. It allows the user to always keep track of all the
*        current parameters with the *.input file (its path name is generated
*        from the *.info one but where the'.info' extension is replace by the
*       'input' one, file is overwritten if it already exists), always get the
*        initial mesh saved, and terminates the setting up of the initialization
*        process of the MPD algorithm.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function.
*
* \param[in] pMesh A pointer that points to the Mesh structure of the \ref main
*                  function.
*
* \param[in] pData A pointer that points to the Data structure of the \ref main
*                  function.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function.
*
* \param[in] pGlobalInitialTimer A pointer that points to the address of the
*                                globalInitialTimer variable globally defined in
*                                the main.c file to store the time at which the
*                                MPD program is launched, and thus print the
*                                total execution time in any case of error
*                                (even the non-anticipated one, see \ref
*                                endTimerAtExit function). It thus allows the
*                                \ref setupInitialData function to compute the
*                                cumulated execution time i.e. the total
*                                execution time from the starting of the program
*                                loading of data, and the end of the
*                                initialization process.
*
* \param[in] pStartLocalTimer A pointer that points to the address of the local
*                             startLocalTimer variable of the \ref main
*                             function, which is used to store the time at which
*                             the loading of data ended and the initialization
*                             step started. It thus allows the \ref
*                             setupInitialData function to compute the
*                             total execution time of the initialization step
*                             without taking into account the initial loading
*                             of data.
*
* \param[in] pEndLocalTimer A pointer that points to the address of the local
*                           endLocalTimer variable of the \ref main function,
*                           which is used to store the time at
*                           which the initialization process ends,
*                           and which is calculated by the \ref
*                           setupInitialData function.
*
* \return It returns one on success otherwise zero is returned if an error is
*         encountered during the process or if the input variables do not have
*         the expected content. In the latter case, a description of the problem
*         is provided in the standard error stream before returning zero.
*
* The \ref setupInitialData function successively executes the \ref
* allocateMemoryForData, \ref addLengthForFileName, \ref writingRestartFile, 
* \ref shapeDerivative, \ref saveDataInTheLoop functions. We refer to their
* descriptions description for further details. We emphasize here the fact that
* the \ref setupInitialData function contains the omp_set_num_threads function
* of the openmp library. Hence, in order to use this function, the omp.h header
* file must not be put in comment in the main.h file. Furthermore, the -fopenmp
* option must be set when compiling the program with gcc (or link correctly the
* openmp library for other compilers and architectures). For not using the
* parallelization used during the computation of the overlap matrix by the \ref
* computeOverlapMatrix or \ref computeOverlapMatrixOnGrid function, just put in
* comment in the \ref setupInitialData function the line containing the
* execution omp_set_num_threads (there should be only one in all *.c/ *.h files
* related to the mpd program and it is contained in this function). In addition,
* let us only mention that the \ref setupInitialData function calls the \ref
* shapeDerivative function, which uses standard mathematical functions,
* parallelize with openmp the evaluation of the overlap matrix, and diagonalize
* it thanks to the dsyev routine of the lapacke library (LAPACK interface for
* C). Hence, in order to use this function, the -lm -llapacke -fopenmp options
* must be set at compilation with gcc (or link properly the math.h, lapacke.h
* and omp.h aasociated libraries) and the math.h omp.h and lapacke.h files must
* of course not be put in comment in the main.h file.
*/
int setupInitialData(Parameters* pParameters, Mesh* pMesh, Data* pData,
                     ChemicalSystem* pChemicalSystem,
                     time_t* pGlobalInitialTimer, time_t* pStartLocalTimer,
                                                        time_t* pEndLocalTimer);

/**
* \fn int exhaustiveSearchAlgorithm(Parameters* pParameters, Mesh* pMesh,
*                                   Data* pData, 
*                                   ChemicalSystem* pChemicalSystem,
*                                                        int iterationInTheLoop)
* \brief It computes the pParameters->opt_mode=-2 of the \ref optimization
*        function. It tries to add or remove the hexahedra that are touching
*        the boundary of the domain. Only perturbations increasing the
*        probability are retained.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function.
*
* \param[out] pData A pointer that points to the Data structure of the \ref main
*                   function.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations already performed in the
*                               optimization loop. Hence, it must be a
*                               non-negative integer (zero refers to the
*                               initial domain after adaptation) not (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable).
*
* \return It returns one on success otherwise zero is returned if an error is
*         encountered during the process.
*
* We mention here that the \ref exhaustiveSearchAlgorithm function calls in
* particular the \ref diagonalizeOverlapMatrix, and \ref computeProbability
* functions, which uses standard mathematical functions, parallelize with openmp
* the evaluation of the overlap matrix, and diagonalize it thanks to the dsyev
* routine of the lapacke library (LAPACK interface for C). Hence, in order to
* use this function, the -lm -llapacke -fopenmp options must be set at
* compilation with gcc (or link properly the math.h, lapacke.h and omp.h
* associated libraries) and the math.h omp.h and lapacke.h files must of course
* not be put in comment in the main.h file.
*/
int exhaustiveSearchAlgorithm(Parameters* pParameters, Mesh* pMesh,
                               Data* pData, ChemicalSystem* pChemicalSystem,
                                                        int iterationInTheLoop);

/**
* \fn int computeLevelSet(Parameters* pParameters, Mesh* pMesh,
*                                                       int iterationInTheLoop)
* \brief It computes the signed distance function i.e. the level-set function
*        with unitary gradient norm of the internal domain of pMesh and saves
*        the values in an *.chi.sol file associated with an *.mesh one. It is
*        ised in the \ref optimization function when pParameters->opt_mode==1.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure (defined in
*                      main.h file) of the \ref main function.
*
* \param[in] iterationInTheLoop An integer that refer to the number of steps
*                               that are already performed in the optimization
*                               loop.
*
* \return It returns one if the signed distance function has been successfully
*         saved in a *.chi.sol file associated with the *.mesh one. Otherwise
*         zero is returned if an error is encountered during the process.
*
* The \ref computeLevelSet relies on the external mshdist and medit softwares,
* which must have been previously installed.
*/
int computeLevelSet(Parameters* pParameters, Mesh* pMesh,
                                                        int iterationInTheLoop);

/**
* \fn int saveTheShapeGradient(Parameters* pParameters, Mesh* pMesh,
*                                                        int iterationInTheLoop)
* \brief It contains successive functions that are used to vizualize and save
*        the shape gradient in the \ref optimization function when
*        pParameters->opt_mode==1.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*
* \param[in] pMesh A pointer that points to the Mesh structure (defined in
*                  main.h file) of the \ref main function.
*
* \param[in] iterationInTheLoop An integer that refer to the number of steps
*                               that are already performed in the optimization
*                               loop.
*
* \return It returns one if the shape gradient has been successfully saved in
*         a *.sol file associated with the *.mesh one. Otherwise zero is
*         returned if an error is encountered during the process.
*
* The \ref saveTheShapeGradient relies on the external medit software, which
* must have been previously installed.
*/
int saveTheShapeGradient(Parameters* pParameters, Mesh* pMesh,
                                                        int iterationInTheLoop);

/**
* \fn int computeEulerianMode(Parameters* pParameters, Mesh* pMesh,
*                                                        int iterationInTheLoop)
* \brief It contains successive functions that are used to specifically compute
*        the Eulerian mode in the \ref optimization function when
*        pParameters->opt_mode==1.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure (defined in
*                      main.h file) of the \ref main function.
*
* \param[in] iterationInTheLoop An integer that refer to the number of steps
*                               that are already performed in the optimization
*                               loop.
*
* \return It returns one if the Eulerian adaptation step has been successfully
*         performed, otherwise zero is returned if an error is encountered 
*         during the process.
*
* The \ref computeEulerianMode relies on the external medit, elastic, mshdist,
* and advect softwares, which must have been previously installed.
*/
int computeEulerianMode(Parameters* pParameters, Mesh* pMesh,
                                                        int iterationInTheLoop);

/**
* \fn int computeLagrangianMode(Parameters* pParameters, Mesh* pMesh,
*                                                        int iterationInTheLoop)
* \brief It contains successive functions that are used to specifically compute
*        the Lagrangian mode in the \ref optimization function when
*        pParameters->opt_mode==1.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure (defined in
*                      main.h file) of the \ref main function.
*
* \param[in] iterationInTheLoop An integer that refer to the number of steps
*                               that are already performed in the optimization
*                               loop.
*
* \return It returns one if the Lagrangian adaptation step has been
*         successfully performed, otherwise zero is returned if an error is
*         encountered during the process.
*
* The \ref computeLagrangianMode relies on the Lagrangian mode of the external
* mmg3d softwares, which must have been previously installed.
*/
int computeLagrangianMode(Parameters* pParameters, Mesh* pMesh,
                                                        int iterationInTheLoop);

/**
* \fn double computeProbabilityAndReloadPreviousMesh(Parameters* pParameters, 
*                                                    Mesh* pMesh, Data* pData,
*                                                    ChemicalSystem*
*                                                    pChemicalSystem,
*                                                        int iterationInTheLoop)
* \brief It computes compute the probability of a given mesh then reload the
*        previous mesh in the structure pointed by pMesh.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function.
*
* \param[out] pData A pointer that points to the Data structure of the \ref main
*                   function.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations already performed in the
*                               optimization loop. Hence, it must be a
*                               non-negative integer (zero refers to the
*                               initial domain after adaptation) not (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable).
*
* \return It returns the value of the probability as a double on success,
*         otherwise -10000. is returned if an error is encountered.
*
* We mention here that the \ref computeProbabilityAndReloadPreviousMesh function
* calls in particular the \ref computeOverlapMatrix, \ref
* diagonalizeOverlapMatrix, and \ref computeProbability functions, which uses
* standard mathematical functions, parallelize with openmp the evaluation of
* the overlap matrix, and diagonalize it thanks to the dsyev routine of the
* lapacke library (LAPACK interface for C). Hence, in order to use this
* function, the -lm -llapacke -fopenmp options must be set at compilation with
* gcc (or link properly the math.h, lapacke.h and omp.h aasociated libraries)
* and the math.h omp.h and lapacke.h files must of course not be put in comment
* in the main.h file.
*/
double computeProbabilityAndReloadPreviousMesh(Parameters* pParameters, 
                                               Mesh* pMesh, Data* pData,
                                               ChemicalSystem* pChemicalSystem,
                                                        int iterationInTheLoop);

/**
* \fn int optimization(Parameters* pParameters, Mesh* pMesh, Data* pData,
*                      ChemicalSystem* pChemicalSystem, int iterationInTheLoop,
*                      time_t* pGlobalInitialTimer, time_t* pStartLocalTimer,
*                                                        time_t* pEndLocalTimer)
* \brief It computes and modifies the shape of the MPD domain according to the
*        shape derivative in order to increase to probability. This is the
*        function used iteratively in the optimization loop of the \ref main
*        function.
*
* \param[in] pParameters A pointer that points to the Parameters structure of
*                        the \ref main function.
*
* \param[in,out] pMesh A pointer that points to the Mesh structure of the \ref
*                      main function.
*
* \param[out] pData A pointer that points to the Data structure of the \ref main
*                   function.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure of the \ref main function.
*
* \param[in] iterationInTheLoop It represents the integer counting the number
*                               of iterations already performed in the
*                               optimization loop. Hence, it must be a
*                               non-negative integer (zero refers to the
*                               initial domain after adaptation) not (strictly)
*                               greater than the maximal number of iterations
*                               allowed in the mpd algorithm (stored in the
*                               pParameters->iter_max variable).
*
* \param[in] pGlobalInitialTimer A pointer that points to the address of the
*                                globalInitialTimer variable globally defined in
*                                the main.c file to store the time at which the
*                                MPD program is launched, and thus print the
*                                total execution time in any case of error
*                                (even the non-anticipated one, see \ref
*                                endTimerAtExit function). It thus allows the
*                                \ref saveDataInTheLoop function to compute the
*                                cumulated execution time i.e. the total
*                                execution time from the starting of the
*                                until the current iterationInTheLoop, saved in
*                                the pData->[iterationInTheLoop].ctim variable.
*
* \param[in] pStartLocalTimer A pointer that points to the address of the local
*                             startLocalTimer variable of the \ref main
*                             function, which is used to store successively the
*                             time at which the (iterationInTheLoop)-th
*                             iteration started. It thus allows the \ref
*                             saveDataInTheLoop function to compute the
*                             total execution time of the current
*                             iterationInTheLoop, saved in the
*                             pData->[iterationInTheLoop].tim variable.
*
* \param[in] pEndLocalTimer A pointer that points to the address of the local
*                           endLocalTimer variable of the \ref main function,
*                           which is used to store successively the time at*
*                           which the (iterationInTheLoop)-th iteration ended,
*                           and which is calculated by the \ref
*                           saveDataInTheLoop function. It also allows the \ref
*                           saveDataInTheLoop function to compute the total
*                           execution time of the current iterationInTheLoop
*                           and the cumulated one, saved in the 
*                           pData->[iterationInTheLoop].tim and the
*                           pData->[iterationInTheLoop].ctim variables.
*
* \return It returns one on success otherwise zero if an error is encountered.
*
* We mention here that the \ref optimization function calls in particular the
* \ref shapeDerivative function, which uses standard mathematical functions,
* parallelize with openmp the evaluation of the overlap matrix, and diagonalize
* it thanks to the dsyev routine of the lapacke library (LAPACK interface for
* C). Hence, in order to use this function, the -lm -llapacke -fopenmp options
* must be set at compilation with gcc (or link properly the math.h, lapacke.h
* and omp.h aasociated libraries) and the math.h omp.h and lapacke.h files must
* of course not be put in comment in the main.h file.
*/
int optimization(Parameters* pParameters, Mesh* pMesh, Data* pData,
                 ChemicalSystem* pChemicalSystem, int iterationInTheLoop,
                 time_t* pGlobalInitialTimer, time_t* pStartLocalTimer,
                                                        time_t* pEndLocalTimer);

#endif

