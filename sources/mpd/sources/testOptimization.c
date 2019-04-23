/**
* \file testOptimization.c
* \brief All unit testing functions related to optimization.c file.
* \author Jeremy DALPHIN
* \version 1.1a
* \date August 1st, 2018
*
* This file contains all the unit testing functions that were built to test
* the functions defined in the optimization.c file. We use the convention that
* if a function is called myFunction for example, its associated unit-test
* function is called testMyFunction. All the functions defined in this file
* does not have any input argument and does not return any argument (void input
* and output).
*/

#include "test.h"
#include "optimization.h"

// Unit (random) tests on initializeDataStructure of optimization.c file
void testInitializeDataStructure(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t iData=0;
    int iRandom=0, i=0, boolean=0;
    Data *pData=NULL, data;

    char pIntegerToPrint[4][6]={"ndata","niter","nprob","nmat"};
    int *pInteger[4]={&data.ndata,&data.niter,&data.nprob,&data.nmat};
    char ppDoubleToPrint[7][5]={"pnu","pop","d0p","d1p","d2p","tim","ctim"};
    double **ppDouble[7]={&data.pnu,&data.pop,&data.d0p,&data.d1p,&data.d2p,
                                                          &data.tim,&data.ctim};

    // Initializing to zero the data structure
    for (i=0; i<4; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<7; i++)
    {
        *ppDouble[i]=NULL;
    }
    data.pprob=NULL;
    data.pmat=NULL;

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
        for (i=0; i<7; i++)
        {
            iData=rand()%20+1;
            if (iData>15)
            {
                *ppDouble[i]=NULL;
            }
            else
            {
                *ppDouble[i]=(double*)calloc(iData,sizeof(double));
            }
        }

        iData=rand()%20+1;
        if (iData>15)
        {
            pData->pprob=NULL;
        }
        else
        {
            pData->pprob=(Probabilities*)calloc(iData,sizeof(Probabilities));
        }

        iData=rand()%20+1;
        if (iData>15)
        {
            pData->pmat=NULL;
        }
        else
        {
            pData->pmat=(OverlapMatrix*)calloc(iData,sizeof(OverlapMatrix));
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        for (i=0; i<4; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<7; i++)
        {
            fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],(void*)*ppDouble[i]);
        }
        fprintf(stdout,"pprob=%p\n",(void*)data.pprob);
        fprintf(stdout,"pmat=%p\n",(void*)data.pmat);

        for (i=0; i<7; i++)
        {
            free(*ppDouble[i]);
        }
        free(data.pprob);
        free(data.pmat);

        // Testing the function
        initializeDataStructure(pData);

        // Checking if the initialization worked
        boolean=0;
        for (i=0; i<4; i++)
        {
            boolean=(boolean || *pInteger[i]!=0);
        }
        for (i=0; i<7; i++)
        {
            boolean=(boolean || *ppDouble[i]!=NULL);
        }
        boolean=(boolean || pData->pprob!=NULL);
        boolean=(boolean || pData->pmat!=NULL);

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
            for (i=0; i<7; i++)
            {
                fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],
                                                           (void*)*ppDouble[i]);
            }
            fprintf(stdout,"pprob=%p\n",(void*)data.pprob);
            fprintf(stdout,"pmat=%p\n",(void*)data.pmat);
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


// Unit (random) tests on freeDataMemory of optimization.c file
void testFreeDataMemory(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t iData, lengthArray=0;
    int iRandom=0, i=0, boolean=0;
    Probabilities *pProbabilities=NULL;
    OverlapMatrix *pOverlapMatrix=NULL;
    Data *pData=NULL, data;

    char pIntegerToPrint[4][6]={"ndata","niter","nprob","nmat"};
    int *pInteger[4]={&data.ndata,&data.niter,&data.nprob,&data.nmat};
    char ppDoubleToPrint[7][5]={"pnu","pop","d0p","d1p","d2p","tim","ctim"};
    double **ppDouble[7]={&data.pnu,&data.pop,&data.d0p,&data.d1p,&data.d2p,
                                                          &data.tim,&data.ctim};

    // Initializing to zero the data structure
    for (i=0; i<4; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<7; i++)
    {
        *ppDouble[i]=NULL;
    }
    data.pprob=NULL;
    data.pmat=NULL;

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
    pData->ndata=-1;
    pData->pprob=(Probabilities*)calloc(6,sizeof(Probabilities));
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p with ndata=%d pprob=",(void*)pData,pData->ndata);
    fprintf(stdout,"%p and pmat=%p\n",(void*)pData->pprob,(void*)pData->pmat);
    freeDataMemory(pData);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pData->pmat=(OverlapMatrix*)calloc(6,sizeof(OverlapMatrix));
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p with ndata=%d pprob=",(void*)pData,pData->ndata);
    fprintf(stdout,"%p and pmat=%p\n",(void*)pData->pprob,(void*)pData->pmat);
    freeDataMemory(pData);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pData->pmat=(OverlapMatrix*)calloc(6,sizeof(OverlapMatrix));
    pData->pprob=(Probabilities*)calloc(6,sizeof(Probabilities));
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p with ndata=%d pprob=",(void*)pData,pData->ndata);
    fprintf(stdout,"%p and pmat=%p\n",(void*)pData->pprob,(void*)pData->pmat);
    freeDataMemory(pData);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p with ndata=%d pprob=",(void*)pData,pData->ndata);
    fprintf(stdout,"%p and pmat=%p\n",(void*)pData->pprob,(void*)pData->pmat);
    freeDataMemory(pData);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);
    pData->ndata=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pData=%p\n",(void*)pData);
    freeDataMemory(pData);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random value to the variables
        iData=rand()%20+1;
        *pInteger[0]=iData;
        for (i=1; i<4; i++)
        {
            *pInteger[i]=rand()%161-30;
        }
        for (i=0; i<7; i++)
        {
            lengthArray=rand()%20+1;
            if (lengthArray>15)
            {
                *ppDouble[i]=NULL;
            }
            else
            {
                *ppDouble[i]=(double*)calloc(lengthArray,sizeof(double));
            }
        }

        if (iData<15 && iData>5)
        {
            pData->pprob=NULL;
        }
        else
        {
            pData->pprob=(Probabilities*)calloc(iData,sizeof(Probabilities));
        }
        if (pData->pprob!=NULL)
        {
            for (i=0; i<pData->ndata; i++)
            {
                pProbabilities=&pData->pprob[i];

                lengthArray=rand()%20+1;
                if (lengthArray>15)
                {
                    pProbabilities->pk=NULL;
                }
                else
                {
                    pProbabilities->pk=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }

                lengthArray=rand()%20+1;
                if (lengthArray>15)
                {
                    pProbabilities->pkl=NULL;
                }
                else
                {
                    pProbabilities->pkl=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }
            }
        }

        if (iData>10)
        {
            pData->pmat=NULL;
        }
        else
        {
            pData->pmat=(OverlapMatrix*)calloc(iData,sizeof(OverlapMatrix));
        }
        if (pData->pmat!=NULL)
        {
            for (i=0; i<pData->ndata; i++)
            {
                pOverlapMatrix=&pData->pmat[i];

                lengthArray=rand()%20+1;
                if (lengthArray>15)
                {
                    pOverlapMatrix->coef=NULL;
                }
                else
                {
                    pOverlapMatrix->coef=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }

                lengthArray=rand()%20+1;
                if (lengthArray>15)
                {
                    pOverlapMatrix->diag=NULL;
                }
                else
                {
                    pOverlapMatrix->diag=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }

                lengthArray=rand()%20+1;
                if (lengthArray>15)
                {
                    pOverlapMatrix->vect=NULL;
                }
                else
                {
                    pOverlapMatrix->vect=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }
            }
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        for (i=0; i<4; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<7; i++)
        {
            fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],(void*)*ppDouble[i]);
        }
        fprintf(stdout,"pprob=%p\n",(void*)data.pprob);
        fprintf(stdout,"pmat=%p\n",(void*)data.pmat);

        if(pData->pprob!=NULL)
        {
            for (i=0; i<pData->ndata; i++)
            {
                pProbabilities=&pData->pprob[i];
                fprintf(stdout,"Probabilities at iteration %d: ",i+1);
                fprintf(stdout,"pk=%p ",(void*)pProbabilities->pk);
                fprintf(stdout,"pkl=%p\n",(void*)pProbabilities->pkl);
            }
        }

        if(pData->pmat!=NULL)
        {
            for (i=0; i<pData->ndata; i++)
            {
                pOverlapMatrix=&pData->pmat[i];
                fprintf(stdout,"OverlapMatrix at iteration %d: ",i+1);
                fprintf(stdout,"coef=%p ",(void*)pOverlapMatrix->coef);
                fprintf(stdout,"diag=%p ",(void*)pOverlapMatrix->diag);
                fprintf(stdout,"vect=%p\n",(void*)pOverlapMatrix->vect);
            }
        }

        // Testing the function
        freeDataMemory(pData);
        if (iRandom%3)
        {
            freeDataMemory(pData);
        }

        // Checking if it worked
        boolean=0;
        for (i=0; i<7; i++)
        {
            boolean=(boolean || *ppDouble[i]!=NULL);
        }
        boolean=(boolean || pData->pprob!=NULL);
        boolean=(boolean || pData->pmat!=NULL);

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough freeDataMemory function ended ");
            fprintf(stdout,"normally, the structure pointed by pData has not ");
            fprintf(stdout,"been freed up properly. One of the following ");
            fprintf(stdout,"pointers is not set to NULL:\n");

            for (i=0; i<4; i++)
            {
                fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
            }
            for (i=0; i<7; i++)
            {
                fprintf(stdout,"%s=%p\n",ppDoubleToPrint[i],
                                                           (void*)*ppDouble[i]);
            }
            fprintf(stdout,"pprob=%p\n",(void*)data.pprob);
            fprintf(stdout,"pmat=%p\n",(void*)data.pmat);
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

