/**
* \file computeData.c
* \brief It contains all the functions used to compute properly the different
*        properties that are saved in the Data structure during the MPD
*        algorithm.
* \author Jeremy DALPHIN
* \version 3.0
* \date May 1st, 2019
*
* This file gathers all the functions that are dealing with the Data structure.
* In particular, it contains how to initialize and free the Data structure, as
* well as functions computing the properties that are used to fill its different
* variables during the iterative optimization process of the MPD algorithm. Many
* functions should be static but have been define as non-static for performing
* unit-tests on them.
*/

#include "loadMesh.h"

////////////////////////////////////////////////////////////////////////////////
// The function initializeDataStructure sets to zero all the variables of the
// Data structure (and pointers to NULL). It has the Data* variable (defined in
// main.h) as input argument and it does not return any value (void output)
////////////////////////////////////////////////////////////////////////////////
void initializeDataStructure(Data* pData)
{
    if (pData!=NULL)
    {
        pData->ndata=0;
        pData->niter=0;

        pData->pnu=NULL;
        pData->pop=NULL;
        pData->vol=NULL;

        pData->d0p=NULL;
        pData->d1p=NULL;
        pData->d2p=NULL;

        pData->opt=NULL;
        pData->tim=NULL;
        pData->ctim=NULL;

        pData->nprob=0;
        pData->pprob=NULL;

        pData->npmat=0;
        pData->ppmat=NULL;
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////
// The function freeDataMemory frees the memory dynamically allocated with
// calloc/malloc/realloc for the Data structure (but other variables than
// pointers are not reset to zero). It has the Data* variable (defined in
// main.h) as input argument and it does not return any value (void output)
////////////////////////////////////////////////////////////////////////////////
void freeDataMemory(Data* pData)
{
    int i=0, iMax=0, j=0, jMax=0;
    OverlapMatrix *pOverlapMatrixI=NULL;

    if (pData!=NULL)
    {
        iMax=pData->ndata;
        if (iMax<1 && (pData->pprob!=NULL || pData->ppmat!=NULL))
        {
             fprintf(stdout,"\nWarning in freeDataMemory function: ");
             fprintf(stdout,"%d iterations are saved in pData (input) ",iMax);
             fprintf(stdout,"variable. Some allocated pointers may not be ");
             fprintf(stdout,"freed up correctly.\n");
        }

        free(pData->pnu); // free function does not return any value
        pData->pnu=NULL;

        free(pData->pop);
        pData->pop=NULL;

        free(pData->vol);
        pData->vol=NULL;

        free(pData->d0p);
        pData->d0p=NULL;

        free(pData->d1p);
        pData->d1p=NULL;

        free(pData->d2p);
        pData->d2p=NULL;

        free(pData->opt);
        pData->opt=NULL;

        free(pData->tim);
        pData->tim=NULL;

        free(pData->ctim);
        pData->ctim=NULL;

        if (pData->pprob!=NULL)
        {
            for (i=0; i<iMax; i++)
            {
                free(pData->pprob[i]);
                pData->pprob[i]=NULL;
            }
        }
        free(pData->pprob);
        pData->pprob=NULL;

        if (pData->ppmat!=NULL)
        {
            jMax=pData->npmat;
            if (jMax<1)
            {
                 fprintf(stdout,"\nWarning in freeDataMemory function: ");
                 fprintf(stdout,"%d overlap matrices are saved in the ",jMax);
                 fprintf(stdout,"pData (input) variable. Some allocated ");
                 fprintf(stdout,"pointers may not be freed up correctly.\n");
            }

            for (i=0; i<iMax; i++)
            {
                pOverlapMatrixI=pData->ppmat[i];
                if (pOverlapMatrixI!=NULL)
                {
                    for (j=0; j<jMax; j++)
                    {
                        free(pOverlapMatrixI[j].coef);
                        pOverlapMatrixI[j].coef=NULL;

                        free(pOverlapMatrixI[j].diag);
                        pOverlapMatrixI[j].diag=NULL;

                        free(pOverlapMatrixI[j].lvect);
                        pOverlapMatrixI[j].lvect=NULL;

                        free(pOverlapMatrixI[j].rvect);
                        pOverlapMatrixI[j].rvect=NULL;
                    }
                }
                free(pOverlapMatrixI);
                pOverlapMatrixI=NULL;
            }
        }
        free(pData->ppmat);
        pData->ppmat=NULL;
    }

    return;
}

