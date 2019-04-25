/**
* \file testComputeData.c
* \brief All unit testing functions related to computeData.c file.
* \author Jeremy DALPHIN
* \version 3.0
* \date May 1st, 2019
*
* This file contains all the unit testing functions that were built to test
* the functions defined in the computeData.c file. We use the convention that
* if a function is called myFunction for example, its associated unit-test
* function is called testMyFunction. All the functions defined in this file
* does not have any input argument and does not return any argument (void input
* and output).
*/

#include "test.h"
#include "computeData.h"

////////////////////////////////////////////////////////////////////////////////
// Unit (random) tests on initializeDataStructure of computeData.c file
////////////////////////////////////////////////////////////////////////////////
void testInitializeDataStructure(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthData=0;
    int iRandom=0, i=0, iMax=0, j=0, jMax=0, boolean=0;
    Data *pData=NULL, data;

    char pIntegerToPrint[4][6]={"ndata","niter","nprob","npmat"};
    int *pInteger[4]={&data.ndata,&data.niter,&data.nprob,&data.npmat};
    char ppDoubleToPrint[9][5]={"pnu","pop","vol","d0p","d1p","d2p","rho","tim",
                                                                        "ctim"};
    double **ppDouble[9]={&data.pnu,&data.pop,&data.vol,&data.d0p,&data.d1p,
                                      &data.d2p,&data.rho,&data.tim,&data.ctim};

    // Initializing to zero the data structure
    for (i=0; i<4; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<9; i++)
    {
        *ppDouble[i]=NULL;
    }
    data.opt=NULL;
    data.pprob=NULL;
    data.ppmat=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting initializeDataStructure function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p\n",(void*)pData);
    initializeDataStructure(pData);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pData=&data;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p\n",(void*)pData);
    initializeDataStructure(pData);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random value to the variables
        for (i=0; i<4; i++)
        {
            *pInteger[i]=rand()%161-30;
        }

        for (i=0; i<9; i++)
        {
            lengthData=rand()%200+1;
            if (lengthData>160)
            {
                *ppDouble[i]=NULL;
            }
            else
            {
                *ppDouble[i]=(double*)calloc(lengthData,sizeof(double));
            }
            if (*ppDouble[i]!=NULL)
            {
                jMax=lengthData;
                for (j=0; j<jMax; j++)
                {
                    (*ppDouble[i])[j]=(double)(rand()%6001-3000)/1000.;
                }
            }
        }

        lengthData=rand()%200+1;
        if (lengthData>160)
        {
            pData->opt=NULL;
        }
        else
        {
            pData->opt=(int*)calloc(lengthData,sizeof(int));
        }
        if (pData->opt!=NULL)
        {
            iMax=lengthData;
            for (i=0; i<iMax; i++)
            {
                pData->opt[i]=rand()%161-30;
            }
        }

        lengthData=rand()%200+1;
        if (lengthData>160)
        {
            pData->pprob=NULL;
        }
        else
        {
            pData->pprob=(double**)malloc(lengthData*sizeof(double*));
        }

        lengthData=rand()%200+1;
        if (lengthData>160)
        {
            pData->ppmat=NULL;
        }
        else
        {
            pData->ppmat=
                     (OverlapMatrix**)malloc(lengthData*sizeof(OverlapMatrix*));
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        for (i=0; i<4; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<9; i++)
        {
            fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],(void*)*ppDouble[i]);
        }
        fprintf(stdout,"opt=%p\n",(void*)pData->opt);
        fprintf(stdout,"pprob=%p\n",(void*)pData->pprob);
        fprintf(stdout,"ppmat=%p\n",(void*)pData->ppmat);

        for (i=0; i<9; i++)
        {
            free(*ppDouble[i]);
        }
        free(pData->opt);
        free(pData->pprob);
        free(pData->ppmat);

        // Testing the function
        initializeDataStructure(pData);

        // Checking if the initialization worked
        boolean=0;
        for (i=0; i<4; i++)
        {
            boolean=(boolean || *pInteger[i]!=0);
        }
        for (i=0; i<9; i++)
        {
            boolean=(boolean || *ppDouble[i]!=NULL);
        }
        boolean=(boolean || pData->opt!=NULL);
        boolean=(boolean || pData->pprob!=NULL);
        boolean=(boolean || pData->ppmat!=NULL);

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough initializeDataStructure function ");
            fprintf(stdout,"ended normally, the structure pointed by pData ");
            fprintf(stdout,"has not been initialized properly. One of the ");
            fprintf(stdout,"following variables is not set to zero (or NULL ");
            fprintf(stdout,"for pointers):\n");

            for (i=0; i<4; i++)
            {
                fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
            }
            for (i=0; i<9; i++)
            {
                fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],
                                                           (void*)*ppDouble[i]);
            }
            fprintf(stdout,"opt=%p\n",(void*)pData->opt);
            fprintf(stdout,"pprob=%p\n",(void*)pData->pprob);
            fprintf(stdout,"ppmat=%p\n",(void*)pData->ppmat);
        }

        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
    }

    // End of the tests
    fprintf(stdout,"\nTotal: %d tests (%d succeeded, ",counter,counterSuccess);
    fprintf(stdout,"%d failed) done in ",counterFail);
    time(&endTimer);
    if (difftime(endTimer,startTimer)<60.)
    {
        fprintf(stdout,"%lf seconds.\n",difftime(endTimer,startTimer));
    }
    else
    {
        fprintf(stdout,"%lf minutes.\n",difftime(endTimer,startTimer)/60.);
    }
    fflush(stdout);

    return;
}

////////////////////////////////////////////////////////////////////////////////
// Unit (random) tests on freeDataMemory of computeData.c file
////////////////////////////////////////////////////////////////////////////////
void testFreeDataMemory(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthData, lengthArray=0, lengthMatrix=0;
    int iRandom=0, i=0, iMax=0, j=0, jMax=0, k=0, kMax=0, boolean=0;
    double **pprob=NULL;
    OverlapMatrix **ppOverlapMatrix=NULL;
    Data *pData=NULL, data;

    char pIntegerToPrint[4][6]={"ndata","niter","nprob","npmat"};
    int *pInteger[4]={&data.ndata,&data.niter,&data.nprob,&data.npmat};
    char ppDoubleToPrint[9][5]={"pnu","pop","vol","d0p","d1p","d2p","rho","tim",
                                                                        "ctim"};
    double **ppDouble[9]={&data.pnu,&data.pop,&data.vol,&data.d0p,&data.d1p,
                                      &data.d2p,&data.rho,&data.tim,&data.ctim};

    // Initializing to zero the data structure
    for (i=0; i<4; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<9; i++)
    {
        *ppDouble[i]=NULL;
    }
    data.opt=NULL;
    data.pprob=NULL;
    data.ppmat=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting freeDataMemory function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p\n",(void*)pData);
    freeDataMemory(pData);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pData=&data;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p\n",(void*)pData);
    freeDataMemory(pData);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=-10; iRandom<1; iRandom++)
    {
        for (i=-10; i<1; i++)
        {
            pData->ndata=iRandom;
            pData->npmat=i;
            for (j=0; j<2; j++)
            {
                for (k=0; k<2; k++)
                {
                    if (j)
                    {
                        pData->pprob=(double**)malloc(10*sizeof(double*));
                    }
                    else
                    {
                        pData->pprob=NULL;
                    }

                    if (k)
                    {
                        pData->ppmat=
                             (OverlapMatrix**)malloc(10*sizeof(OverlapMatrix*));
                    }
                    else
                    {
                        pData->ppmat=NULL;
                    }

                    PRINT_TEST_START(counter,expectedValue);
                    fprintf(stdout,"ndata=%d\n",pData->ndata);
                    fprintf(stdout,"npmat=%d\n",pData->npmat);
                    fprintf(stdout,"pprob=%p\n",(void*)pData->pprob);
                    fprintf(stdout,"ppmat=%p\n",(void*)pData->ppmat);
                    freeDataMemory(pData);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
            }
        }
    }

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random value to the variables
        lengthData=rand()%20+1;
        *pInteger[0]=lengthData;
        *pInteger[1]=rand()%161-30;
        *pInteger[2]=rand()%161-30;
        lengthMatrix=rand()%200+1;
        *pInteger[3]=lengthMatrix;

        for (i=0; i<9; i++)
        {
            lengthArray=rand()%200+1;
            if (lengthArray>160)
            {
                *ppDouble[i]=NULL;
            }
            else
            {
                *ppDouble[i]=(double*)calloc(lengthArray,sizeof(double));
            }
            if (*ppDouble[i]!=NULL)
            {
                jMax=lengthArray;
                for (j=0; j<jMax; j++)
                {
                    (*ppDouble[i])[j]=(double)(rand()%6001-3000)/1000.;
                }
            }
        }

        lengthArray=rand()%200+1;
        if (lengthArray>160)
        {
            pData->opt=NULL;
        }
        else
        {
            pData->opt=(int*)calloc(lengthArray,sizeof(int));
        }
        if (pData->opt!=NULL)
        {
            iMax=lengthArray;
            for (i=0; i<iMax; i++)
            {
                pData->opt[i]=rand()%161-30;
            }
        }

        if (lengthData>5 && lengthData<15)
        {
            pData->pprob=NULL;
        }
        else
        {
            pData->pprob=(double**)calloc(lengthData,sizeof(double*));
        }
        if (pData->pprob!=NULL)
        {
            iMax=lengthData;
            for (i=0; i<iMax; i++)
            {
                pprob=&pData->pprob[i];
                lengthArray=rand()%20+1;
                if (lengthArray>15)
                {
                    *pprob=NULL;
                }
                else
                {
                    *pprob=(double*)calloc(lengthArray,sizeof(double));
                }
                if (*pprob!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        (*pprob)[j]=(double)(rand()%6001-3000)/1000.;
                    }
                }
            }
        }

        if (lengthData>10)
        {
            pData->ppmat=NULL;
        }
        else
        {
            pData->ppmat=
                     (OverlapMatrix**)calloc(lengthData,sizeof(OverlapMatrix*));
        }
        if (pData->ppmat!=NULL)
        {
            iMax=lengthData;
            jMax=lengthMatrix;
            for (i=0; i<iMax; i++)
            {
                ppOverlapMatrix=&pData->ppmat[i];
                if (jMax>160)
                {
                    *ppOverlapMatrix=NULL;
                }
                else
                {
                    *ppOverlapMatrix=(OverlapMatrix*)calloc(lengthMatrix,
                                                         sizeof(OverlapMatrix));
                }
                if (*ppOverlapMatrix!=NULL)
                {
                    for (j=0; j<jMax; j++)
                    {
                        (*ppOverlapMatrix)[j].nmat=rand()%161-30;
                        (*ppOverlapMatrix)[j].rhf=rand()%161-30;
                        (*ppOverlapMatrix)[j].sym=rand()%161-30;
                        (*ppOverlapMatrix)[j].id=rand()%161-30;
                        (*ppOverlapMatrix)[j].cmat=
                                               (double)(rand()%6001-3000)/1000.;

                        lengthArray=rand()%20+1;
                        if (lengthArray>15)
                        {
                           (*ppOverlapMatrix)[j].coef=NULL;
                        }
                        else
                        {
                            (*ppOverlapMatrix)[j].coef=
                                    (double*)calloc(lengthArray,sizeof(double));
                        }
                        if ((*ppOverlapMatrix)[j].coef!=NULL)
                        {
                            kMax=lengthArray;
                            for (k=0; k<kMax; k++)
                            {
                                ((*ppOverlapMatrix)[j].coef)[k]=
                                               (double)(rand()%6001-3000)/1000.;
                            }
                        }

                        lengthArray=rand()%20+1;
                        if (lengthArray>15)
                        {
                            (*ppOverlapMatrix)[j].diag=NULL;
                        }
                        else
                        {
                            (*ppOverlapMatrix)[j].diag=
                                            (double complex*)calloc(lengthArray,
                                                        sizeof(double complex));
                        }
                        if ((*ppOverlapMatrix)[j].diag!=NULL)
                        {
                            kMax=lengthArray;
                            for (k=0; k<kMax; k++)
                            {
                                ((*ppOverlapMatrix)[j].diag)[k]=
                                                (double)(rand()%6001-3000)/1000.
                                            +I*(double)(rand()%6001-3000)/1000.;
                            }
                        }

                        lengthArray=rand()%20+1;
                        if (lengthArray>15)
                        {
                            (*ppOverlapMatrix)[j].lvect=NULL;
                        }
                        else
                        {
                            (*ppOverlapMatrix)[j].lvect=
                                            (double complex*)calloc(lengthArray,
                                                        sizeof(double complex));
                        }
                        if ((*ppOverlapMatrix)[j].lvect!=NULL)
                        {
                            kMax=lengthArray;
                            for (k=0; k<kMax; k++)
                            {
                                ((*ppOverlapMatrix)[j].lvect)[k]=
                                                (double)(rand()%6001-3000)/1000.
                                            +I*(double)(rand()%6001-3000)/1000.;
                            }
                        }

                        lengthArray=rand()%20+1;
                        if (lengthArray>15)
                        {
                            (*ppOverlapMatrix)[j].rvect=NULL;
                        }
                        else
                        {
                            (*ppOverlapMatrix)[j].rvect=
                                            (double complex*)calloc(lengthArray,
                                                        sizeof(double complex));
                        }
                        if ((*ppOverlapMatrix)[j].rvect!=NULL)
                        {
                            kMax=lengthArray;
                            for (k=0; k<kMax; k++)
                            {
                                ((*ppOverlapMatrix)[j].rvect)[k]=
                                                (double)(rand()%6001-3000)/1000.
                                            +I*(double)(rand()%6001-3000)/1000.;
                            }
                        }
                    }
                }
            }
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        for (i=0; i<4; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<9; i++)
        {
            fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],(void*)*ppDouble[i]);
        }
        fprintf(stdout,"opt=%p\n",(void*)pData->opt);
        fprintf(stdout,"pprob=%p\n",(void*)pData->pprob);
        fprintf(stdout,"ppmat=%p\n\n",(void*)pData->ppmat);

        if(pData->pprob!=NULL)
        {
            iMax=lengthData;
            for (i=0; i<iMax; i++)
            {
                fprintf(stdout,"pprob=%p ",(void*)pData->pprob[i]);
                fprintf(stdout,"at iteration %d\n",i+1);
            }
            fprintf(stdout,"\n");
        }

        if(pData->ppmat!=NULL)
        {
            iMax=lengthData;
            jMax=lengthMatrix;
            for (i=0; i<iMax; i++)
            {
                ppOverlapMatrix=&pData->ppmat[i];
                if (*ppOverlapMatrix!=NULL)
                {
                    for (j=0; j<jMax; j++)
                    {
                        fprintf(stdout,"OverlapMatrix %d ",j+1);
                        fprintf(stdout,"at iteration %d: ",i+1);
                        fprintf(stdout,"nmat=%d ",(*ppOverlapMatrix)[j].nmat);
                        fprintf(stdout,"rhf=%d ",(*ppOverlapMatrix)[j].rhf);
                        fprintf(stdout,"sym=%d ",(*ppOverlapMatrix)[j].sym);
                        fprintf(stdout,"id=%d ",(*ppOverlapMatrix)[j].id);
                        fprintf(stdout,"cmat=%lf ",(*ppOverlapMatrix)[j].cmat);
                        fprintf(stdout,"coef=%p ",
                                             (void*)(*ppOverlapMatrix)[j].coef);
                        fprintf(stdout,"diag=%p ",
                                             (void*)(*ppOverlapMatrix)[j].diag);
                        fprintf(stdout,"lvect=%p ",
                                            (void*)(*ppOverlapMatrix)[j].lvect);
                        fprintf(stdout,"rvect=%p\n",
                                            (void*)(*ppOverlapMatrix)[j].rvect);
                    }
                    fprintf(stdout,"\n");
                }
            }
            fprintf(stdout,"\n");
        }

        // Testing the function
        freeDataMemory(pData);
        if (iRandom%3)
        {
            freeDataMemory(pData);
        }

        // Checking if it worked
        boolean=0;
        for (i=0; i<9; i++)
        {
            boolean=(boolean || *ppDouble[i]!=NULL);
        }
        boolean=(boolean || pData->opt!=NULL);
        boolean=(boolean || pData->pprob!=NULL);
        boolean=(boolean || pData->ppmat!=NULL);

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough freeDataMemory function ended ");
            fprintf(stdout,"normally, the structure pointed by pData has not ");
            fprintf(stdout,"been freed up properly. One of the following ");
            fprintf(stdout,"pointers is not set to NULL:\n");

            for (i=0; i<9; i++)
            {
                fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],
                                                           (void*)*ppDouble[i]);
            }
            fprintf(stdout,"opt=%p\n",(void*)pData->opt);
            fprintf(stdout,"pprob=%p\n",(void*)pData->pprob);
            fprintf(stdout,"ppmat=%p\n",(void*)pData->ppmat);
        }

        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
    }

    // End of the tests
    fprintf(stdout,"\nTotal: %d tests (%d succeeded, ",counter,counterSuccess);
    fprintf(stdout,"%d failed) done in ",counterFail);
    time(&endTimer);
    if (difftime(endTimer,startTimer)<60.)
    {
        fprintf(stdout,"%lf seconds.\n",difftime(endTimer,startTimer));
    }
    else
    {
        fprintf(stdout,"%lf minutes.\n",difftime(endTimer,startTimer)/60.);
    }
    fflush(stdout);

    return;
}

