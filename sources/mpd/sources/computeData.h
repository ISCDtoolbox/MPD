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

