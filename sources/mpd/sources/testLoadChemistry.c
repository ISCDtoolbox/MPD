/**
* \file testLoadChemistry.c
* \brief All unit testing functions related to loadChemistry.c file.
* \author Jeremy DALPHIN
* \version 3.0
* \date May 1st, 2019
*
* This file contains all the unit testing functions that were built to test
* the functions defined in the loadChemistry.c file. We use the convention that
* if a function is called myFunction for example, its associated unit-test
* function is called testMyFunction. All the functions defined in this file
* does not have any input argument and does not return any argument (void input
* and output).
*/

#include "test.h"
#include "loadChemistry.h"

////////////////////////////////////////////////////////////////////////////////
// Unit (random) tests on initializeChemicalStructure of loadChemistry.c file
////////////////////////////////////////////////////////////////////////////////
void testInitializeChemicalStructure(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthArray=0;
    int iRandom=0, boolean=0;
    ChemicalSystem *pChemicalSystem=NULL, chemicalSystem;

    // Initializing to zero the chemical structure
    chemicalSystem.nnucl=0;
    chemicalSystem.pnucl=NULL;
    chemicalSystem.nprim=0;
    chemicalSystem.nmorb=0;
    chemicalSystem.pmorb=NULL;
    chemicalSystem.ne=0;
    chemicalSystem.nu=0;
    chemicalSystem.ndet=0;
    chemicalSystem.pdet=NULL;
    chemicalSystem.pmat=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting initializeChemicalStructure function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p\n",(void*)pChemicalSystem);
    initializeChemicalStructure(pChemicalSystem);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pChemicalSystem=&chemicalSystem;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p\n",(void*)pChemicalSystem);
    initializeChemicalStructure(pChemicalSystem);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random value to the variables
        pChemicalSystem->nnucl=rand()%50-15;
        lengthArray=rand()%20+1;
        if (lengthArray>12)
        {
            pChemicalSystem->pnucl=NULL;
        }
        else
        {
            pChemicalSystem->pnucl=(Nucleus*)calloc(lengthArray,
                                                               sizeof(Nucleus));
        }
        pChemicalSystem->nprim=rand()%161-30;

        pChemicalSystem->nmorb=rand()%60-20;
        lengthArray=rand()%20+1;
        if (lengthArray>12)
        {
            pChemicalSystem->pmorb=NULL;
        }
        else
        {
            pChemicalSystem->pmorb=(MolecularOrbital*)calloc(lengthArray,
                                                      sizeof(MolecularOrbital));
        }
        pChemicalSystem->ne=rand()%60-20;
        pChemicalSystem->nu=rand()%60-20;

        pChemicalSystem->ndet=rand()%60-20;
        lengthArray=rand()%20+1;
        if (lengthArray>12)
        {
            pChemicalSystem->pdet=NULL;
        }
        else
        {
            pChemicalSystem->pdet=(Determinant*)calloc(lengthArray,
                                                           sizeof(Determinant));
        }

        lengthArray=rand()%20+1;
        if (lengthArray>12)
        {
            pChemicalSystem->pmat=NULL;
        }
        else
        {
            pChemicalSystem->pmat=(OverlapMatrix*)calloc(lengthArray,
                                                         sizeof(OverlapMatrix));
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        fprintf(stdout,"nnucl=%d\n",pChemicalSystem->nnucl);
        fprintf(stdout,"pnucl=%p\n",(void*)pChemicalSystem->pnucl);
        fprintf(stdout,"nprim=%d\n",pChemicalSystem->nprim);
        fprintf(stdout,"nmorb=%d\n",pChemicalSystem->nmorb);
        fprintf(stdout,"pmorb=%p\n",(void*)pChemicalSystem->pmorb);
        fprintf(stdout,"ne=%d\n",pChemicalSystem->ne);
        fprintf(stdout,"nu=%d\n",pChemicalSystem->nu);
        fprintf(stdout,"ndet=%d\n",pChemicalSystem->ndet);
        fprintf(stdout,"pdet=%p\n",(void*)pChemicalSystem->pdet);
        fprintf(stdout,"pmat=%p\n",(void*)pChemicalSystem->pmat);

        free(pChemicalSystem->pnucl);
        free(pChemicalSystem->pmorb);
        free(pChemicalSystem->pdet);
        free(pChemicalSystem->pmat);

        // Testing the function
        initializeChemicalStructure(pChemicalSystem);

        // Checking if the initialization worked
        boolean=(pChemicalSystem->nnucl!=0);
        boolean=(boolean || pChemicalSystem->pnucl!=NULL);
        boolean=(boolean || pChemicalSystem->nprim!=0);
        boolean=(boolean || pChemicalSystem->nmorb!=0);
        boolean=(boolean || pChemicalSystem->pmorb!=NULL);
        boolean=(boolean || pChemicalSystem->ne!=0);
        boolean=(boolean || pChemicalSystem->nu!=0);
        boolean=(boolean || pChemicalSystem->ndet!=0);
        boolean=(boolean || pChemicalSystem->pdet!=NULL);
        boolean=(boolean || pChemicalSystem->pmat!=NULL);

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough initializeChemicalStructure function ");
            fprintf(stdout,"ended normally, the structure pointed by ");
            fprintf(stdout,"pChemicalSystem has not been initialized ");
            fprintf(stdout,"properly. One of the following variables is not ");
            fprintf(stdout,"set to zero (or NULL for pointers):\n");

            fprintf(stdout,"nnucl=%d\n",pChemicalSystem->nnucl);
            fprintf(stdout,"pnucl=%p\n",(void*)pChemicalSystem->pnucl);
            fprintf(stdout,"nprim=%d\n",pChemicalSystem->nprim);
            fprintf(stdout,"nmorb=%d\n",pChemicalSystem->nmorb);
            fprintf(stdout,"pmorb=%p\n",(void*)pChemicalSystem->pmorb);
            fprintf(stdout,"ne=%d\n",pChemicalSystem->ne);
            fprintf(stdout,"nu=%d\n",pChemicalSystem->nu);
            fprintf(stdout,"ndet=%d\n",pChemicalSystem->ndet);
            fprintf(stdout,"pdet=%p\n",(void*)pChemicalSystem->pdet);
            fprintf(stdout,"pmat=%p\n",(void*)pChemicalSystem->pmat);
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

/*
////////////////////////////////////////////////////////////////////////////////
// Unit (random) tests on freeChemicalMemory of loadChemistry.c file
////////////////////////////////////////////////////////////////////////////////
void testFreeChemicalMemory(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthArray=0;
    int i=0, iRandom=0, boolean=0;
    MolecularOrbital *pMolecularOrbital=NULL;
    ChemicalSystem *pChemicalSystem=NULL, chemicalSystem;

    // Initializing the chemical structure
    chemicalSystem.nnucl=0;
    chemicalSystem.pnucl=NULL;
    chemicalSystem.ngauss=0;
    chemicalSystem.nmorb=0;
    chemicalSystem.pmorb=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting freeChemicalMemory function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p\n",(void*)pChemicalSystem);
    freeChemicalMemory(pChemicalSystem);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pChemicalSystem=&chemicalSystem;
    chemicalSystem.nmorb=-1;
    chemicalSystem.pmorb=(MolecularOrbital*)calloc(6,sizeof(MolecularOrbital));
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p with ",(void*)pChemicalSystem);
    fprintf(stdout,"nmorb=%d ",pChemicalSystem->nmorb);
    fprintf(stdout,"and pmorb=%p\n",(void*)pChemicalSystem->pmorb);
    freeChemicalMemory(pChemicalSystem);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p with ",(void*)pChemicalSystem);
    fprintf(stdout,"nmorb=%d ",pChemicalSystem->nmorb);
    fprintf(stdout,"and pmorb=%p\n",(void*)pChemicalSystem->pmorb);
    freeChemicalMemory(pChemicalSystem);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    chemicalSystem.nmorb=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p\n",(void*)pChemicalSystem);
    freeChemicalMemory(pChemicalSystem);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random value to the variables
        pChemicalSystem->nnucl=rand()%5-1;
        lengthArray=rand()%10+1;
        if (lengthArray>7)
        {
            pChemicalSystem->pnucl=NULL;
        }
        else
        {
            pChemicalSystem->pnucl=(Nucleus*)calloc(lengthArray,
                                                               sizeof(Nucleus));
        }
        pChemicalSystem->ngauss=rand()%161-30;

        lengthArray=rand()%20+1;
        pChemicalSystem->nmorb=lengthArray;
        if (lengthArray>15)
        {
            pChemicalSystem->pmorb=NULL;
        }
        else
        {
            pChemicalSystem->pmorb=(MolecularOrbital*)calloc(lengthArray,
                                                      sizeof(MolecularOrbital));
        }

        if (pChemicalSystem->pmorb!=NULL)
        {
            for (i=0; i<pChemicalSystem->nmorb; i++)
            {
                pMolecularOrbital=&pChemicalSystem->pmorb[i];
                pMolecularOrbital->spin=rand()%5-2;

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pMolecularOrbital->coeff=NULL;
                }
                else
                {
                    pMolecularOrbital->coeff=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pMolecularOrbital->exp=NULL;
                }
                else
                {
                    pMolecularOrbital->exp=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pMolecularOrbital->nucl=NULL;
                }
                else
                {
                    pMolecularOrbital->nucl=(int*)calloc(lengthArray,
                                                                   sizeof(int));
                }

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pMolecularOrbital->type=NULL;
                }
                else
                {
                    pMolecularOrbital->type=(int*)calloc(lengthArray,
                                                                   sizeof(int));
                }
            }
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        fprintf(stdout,"nnucl=%d\n",pChemicalSystem->nnucl);
        fprintf(stdout,"pnucl=%p\n",(void*)pChemicalSystem->pnucl);
        fprintf(stdout,"ngauss=%d\n",pChemicalSystem->ngauss);
        fprintf(stdout,"nmorb=%d\n",pChemicalSystem->nmorb);
        fprintf(stdout,"pmorb=%p\n",(void*)pChemicalSystem->pmorb);

        if (pChemicalSystem->pmorb!=NULL)
        {
            for (i=0; i<pChemicalSystem->nmorb; i++)
            {
                pMolecularOrbital=&pChemicalSystem->pmorb[i];
                fprintf(stdout,"Orbital %d: ",i+1);
                fprintf(stdout,"spin=%d ",pMolecularOrbital->spin);
                fprintf(stdout,"coeff=%p ",(void*)pMolecularOrbital->coeff);
                fprintf(stdout,"exp=%p ",(void*)pMolecularOrbital->exp);
                fprintf(stdout,"nucl=%p ",(void*)pMolecularOrbital->nucl);
                fprintf(stdout,"type=%p\n",(void*)pMolecularOrbital->type);
            }
        }

        // Testing the function
        freeChemicalMemory(pChemicalSystem);
        if (iRandom%3)
        {
            freeChemicalMemory(pChemicalSystem);
        }

        // Checking if it worked
        boolean=(pChemicalSystem->pnucl!=NULL);
        boolean=(boolean || pChemicalSystem->pmorb!=NULL);

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough freeChemicalMemory function ended ");
            fprintf(stdout,"normally, the structure pointed by ");
            fprintf(stdout,"pChemicalSystem has not been freed up properly. ");
            fprintf(stdout,"One of the following pointers is not set to ");
            fprintf(stdout,"NULL:\n");

            fprintf(stdout,"pnucl=%p\n",(void*)pChemicalSystem->pnucl);
            fprintf(stdout,"pmorb=%p\n",(void*)pChemicalSystem->pmorb);

            if (pChemicalSystem->pmorb!=NULL)
            {
                for (i=0; i<pChemicalSystem->nmorb; i++)
                {
                    pMolecularOrbital=&pChemicalSystem->pmorb[i];
                    fprintf(stdout,"Orbital %d: ",i+1);
                    fprintf(stdout,"spin=%d ",pMolecularOrbital->spin);
                    fprintf(stdout,"coeff=%p ",(void*)pMolecularOrbital->coeff);
                    fprintf(stdout,"exp=%p ",(void*)pMolecularOrbital->exp);
                    fprintf(stdout,"nucl=%p ",(void*)pMolecularOrbital->nucl);
                    fprintf(stdout,"type=%p\n",(void*)pMolecularOrbital->type);
                }
            }
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
*/

