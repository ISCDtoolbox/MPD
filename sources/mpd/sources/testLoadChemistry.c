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
    int iRandom=0, i=0, iMax=0, boolean=0;
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
        if (pChemicalSystem->pnucl!=NULL)
        {
            iMax=lengthArray;
            for (i=0; i<iMax; i++)
            {
                pChemicalSystem->pnucl[i].x=(double)(rand()%6001-3000)/1000.;
                pChemicalSystem->pnucl[i].y=(double)(rand()%6001-3000)/1000.;
                pChemicalSystem->pnucl[i].z=(double)(rand()%6001-3000)/1000.;
                pChemicalSystem->pnucl[i].charge=rand()%161-30;
            }
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
            pChemicalSystem->pmorb=
                (MolecularOrbital*)malloc(lengthArray*sizeof(MolecularOrbital));
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
            pChemicalSystem->pdet=
                          (Determinant*)malloc(lengthArray*sizeof(Determinant));
        }

        lengthArray=rand()%20+1;
        if (lengthArray>12)
        {
            pChemicalSystem->pmat=NULL;
        }
        else
        {
            pChemicalSystem->pmat=
                      (OverlapMatrix*)malloc(lengthArray*sizeof(OverlapMatrix));
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

////////////////////////////////////////////////////////////////////////////////
// Unit (random) tests on freeChemicalMemory of loadChemistry.c file
////////////////////////////////////////////////////////////////////////////////
void testFreeChemicalMemory(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthMorb=0, lengthDet=0, lengthMatrix=0, lengthArray=0;
    int iRandom=0, i=0, iMax=0, j=0, jMax=0, k=0, l=0, boolean=0;
    MolecularOrbital *pMolecularOrbital=NULL;
    Determinant *pDeterminant=NULL;
    OverlapMatrix *pOverlapMatrix=NULL;
    ChemicalSystem *pChemicalSystem=NULL, chemicalSystem;

    // Initializing the chemical structure
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
    fprintf(stdout,"\nTesting freeChemicalMemory function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p\n",(void*)pChemicalSystem);
    freeChemicalMemory(pChemicalSystem);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pChemicalSystem=&chemicalSystem;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pChemicalSystem=%p\n",(void*)pChemicalSystem);
    freeChemicalMemory(pChemicalSystem);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=-10; iRandom<1; iRandom++)
    {
        for (i=-10; i<1; i++)
        {
            pChemicalSystem->nmorb=iRandom;
            pChemicalSystem->ndet=i;
            for (j=0; j<2; j++)
            {
                for (k=0; k<2; k++)
                {
                    for (l=0; l<2; l++)
                    {
                        if (j)
                        {
                            pChemicalSystem->pmorb=(MolecularOrbital*)
                                            malloc(10*sizeof(MolecularOrbital));
                        }
                        else
                        {
                            pChemicalSystem->pmorb=NULL;
                        }

                        if (k)
                        {
                            pChemicalSystem->pdet=
                                   (Determinant*)malloc(10*sizeof(Determinant));
                        }
                        else
                        {
                            pChemicalSystem->pdet=NULL;
                        }

                        if (l)
                        {
                            pChemicalSystem->pmat=
                               (OverlapMatrix*)malloc(10*sizeof(OverlapMatrix));
                        }
                        else
                        {
                            pChemicalSystem->pmat=NULL;
                        }

                        PRINT_TEST_START(counter,expectedValue);
                        fprintf(stdout,"nmorb=%d\n",pChemicalSystem->nmorb);
                        fprintf(stdout,"ndet=%d\n",pChemicalSystem->ndet);
                        fprintf(stdout,"pmorb=%p\n",
                                                 (void*)pChemicalSystem->pmorb);
                        fprintf(stdout,"pdet=%p\n",
                                                  (void*)pChemicalSystem->pdet);
                        fprintf(stdout,"pmat=%p\n",
                                                  (void*)pChemicalSystem->pmat);
                        freeChemicalMemory(pChemicalSystem);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                }
            }
        }
    }

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
        if (pChemicalSystem->pnucl!=NULL)
        {
            iMax=lengthArray;
            for (i=0; i<iMax; i++)
            {
                pChemicalSystem->pnucl[i].x=(double)(rand()%6001-3000)/1000.;
                pChemicalSystem->pnucl[i].y=(double)(rand()%6001-3000)/1000.;
                pChemicalSystem->pnucl[i].z=(double)(rand()%6001-3000)/1000.;
                pChemicalSystem->pnucl[i].charge=rand()%161-30;
            }
        }
        pChemicalSystem->nprim=rand()%161-30;

        lengthMorb=rand()%20+1;
        pChemicalSystem->nmorb=lengthMorb;
        if (lengthMorb>15)
        {
            pChemicalSystem->pmorb=NULL;
        }
        else
        {
            pChemicalSystem->pmorb=(MolecularOrbital*)calloc(lengthMorb,
                                                      sizeof(MolecularOrbital));
        }
        if (pChemicalSystem->pmorb!=NULL)
        {
            iMax=lengthMorb;
            for (i=0; i<iMax; i++)
            {
                pMolecularOrbital=&pChemicalSystem->pmorb[i];
                pMolecularOrbital->spin=rand()%5-2;
                pMolecularOrbital->ngauss=rand()%161-30;

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
                if (pMolecularOrbital->coeff!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pMolecularOrbital->coeff[j]=
                                               (double)(rand()%6001-3000)/1000.;
                    }
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
                if (pMolecularOrbital->exp!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pMolecularOrbital->exp[j]=
                                               (double)(rand()%6001-3000)/1000.;
                    }
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
                if (pMolecularOrbital->nucl!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pMolecularOrbital->nucl[j]=rand()%161-30;
                    }
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
                if (pMolecularOrbital->type!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pMolecularOrbital->type[j]=rand()%161-30;
                    }
                }

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pMolecularOrbital->pgauss=NULL;
                }
                else
                {
                    pMolecularOrbital->pgauss=(int*)calloc(lengthArray,
                                                                   sizeof(int));
                }
                if (pMolecularOrbital->pgauss!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pMolecularOrbital->pgauss[j]=rand()%161-30;
                    }
                }
            }
        }
        pChemicalSystem->ne=rand()%60-20;
        pChemicalSystem->nu=rand()%60-20;

        lengthDet=rand()%20+1;
        pChemicalSystem->ndet=lengthDet;
        if (lengthDet>5 && lengthDet<15)
        {
            pChemicalSystem->pdet=NULL;
        }
        else
        {
            pChemicalSystem->pdet=(Determinant*)calloc(lengthDet,
                                                           sizeof(Determinant));
        }
        if (pChemicalSystem->pdet!=NULL)
        {
            iMax=lengthDet;
            for (i=0; i<iMax; i++)
            {
                pDeterminant=&pChemicalSystem->pdet[i];
                pDeterminant->rhf=rand()%161-30;
                pDeterminant->cdet=(double)(rand()%6001-3000)/1000.;

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pDeterminant->vmorb=NULL;
                }
                else
                {
                    pDeterminant->vmorb=(int*)calloc(lengthArray,sizeof(int));
                }
                if (pDeterminant->vmorb!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pDeterminant->vmorb[j]=rand()%161-30;
                    }
                }
            }
        }

        lengthMatrix=lengthDet*lengthDet;
        if (lengthMatrix>200)
        {
            pChemicalSystem->pmat=NULL;
        }
        else
        {
            pChemicalSystem->pmat=(OverlapMatrix*)calloc(lengthMatrix,
                                                         sizeof(OverlapMatrix));
        }
        if (pChemicalSystem->pmat!=NULL)
        {
            iMax=lengthMatrix;
            for (i=0; i<iMax; i++)
            {
                pOverlapMatrix=&pChemicalSystem->pmat[i];

                pOverlapMatrix->nmat=rand()%161-30;
                pOverlapMatrix->rhf=rand()%161-30;
                pOverlapMatrix->sym=rand()%161-30;
                pOverlapMatrix->id=rand()%161-30;
                pOverlapMatrix->cmat=(double)(rand()%6001-3000)/1000.;

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pOverlapMatrix->coef=NULL;
                }
                else
                {
                    pOverlapMatrix->coef=(double*)calloc(lengthArray,
                                                                sizeof(double));
                }
                if (pOverlapMatrix->coef!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pOverlapMatrix->coef[j]=
                                               (double)(rand()%6001-3000)/1000.;
                    }
                }

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pOverlapMatrix->diag=NULL;
                }
                else
                {
                    pOverlapMatrix->diag=(double complex*)calloc(lengthArray,
                                                        sizeof(double complex));
                }
                if (pOverlapMatrix->diag!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pOverlapMatrix->diag[j]=(double)(rand()%6001-3000)/1000.
                                            +I*(double)(rand()%6001-3000)/1000.;
                    }
                }

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pOverlapMatrix->lvect=NULL;
                }
                else
                {
                    pOverlapMatrix->lvect=(double complex*)calloc(lengthArray,
                                                        sizeof(double complex));
                }
                if (pOverlapMatrix->lvect!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pOverlapMatrix->lvect[j]=
                                                (double)(rand()%6001-3000)/1000.
                                            +I*(double)(rand()%6001-3000)/1000.;
                    }
                }

                lengthArray=rand()%120+1;
                if (lengthArray>100)
                {
                    pOverlapMatrix->rvect=NULL;
                }
                else
                {
                    pOverlapMatrix->rvect=(double complex*)calloc(lengthArray,
                                                        sizeof(double complex));
                }
                if (pOverlapMatrix->rvect!=NULL)
                {
                    jMax=lengthArray;
                    for (j=0; j<jMax; j++)
                    {
                        pOverlapMatrix->rvect[j]=
                                                (double)(rand()%6001-3000)/1000.
                                            +I*(double)(rand()%6001-3000)/1000.;
                    }
                }
            }
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
        fprintf(stdout,"pmat=%p\n\n",(void*)pChemicalSystem->pmat);

        if (pChemicalSystem->pmorb!=NULL)
        {
            iMax=lengthMorb;
            for (i=0; i<iMax; i++)
            {
                pMolecularOrbital=&pChemicalSystem->pmorb[i];
                fprintf(stdout,"MolecularOrbital %d: ",i+1);
                fprintf(stdout,"spin=%d ",pMolecularOrbital->spin);
                fprintf(stdout,"coeff=%p ",(void*)pMolecularOrbital->coeff);
                fprintf(stdout,"exp=%p ",(void*)pMolecularOrbital->exp);
                fprintf(stdout,"nucl=%p ",(void*)pMolecularOrbital->nucl);
                fprintf(stdout,"type=%p ",(void*)pMolecularOrbital->type);
                fprintf(stdout,"ngauss=%d ",pMolecularOrbital->ngauss);
                fprintf(stdout,"pgauss=%p\n",(void*)pMolecularOrbital->pgauss);
            }
            fprintf(stdout,"\n");
        }

        if (pChemicalSystem->pdet!=NULL)
        {
            iMax=lengthDet;
            for (i=0; i<iMax; i++)
            {
                pDeterminant=&pChemicalSystem->pdet[i];
                fprintf(stdout,"Determinant %d: ",i+1);
                fprintf(stdout,"rhf=%d ",pDeterminant->rhf);
                fprintf(stdout,"cdet=%lf ",pDeterminant->cdet);
                fprintf(stdout,"vmorb=%p\n",(void*)pDeterminant->vmorb);
            }
            fprintf(stdout,"\n");
        }

        if (pChemicalSystem->pmat!=NULL)
        {
            iMax=lengthMatrix;
            for (i=0; i<iMax; i++)
            {
                pOverlapMatrix=&pChemicalSystem->pmat[i];
                fprintf(stdout,"OverlapMatrix %d ",i+1);
                fprintf(stdout,"nmat=%d ",pOverlapMatrix->nmat);
                fprintf(stdout,"rhf=%d ",pOverlapMatrix->rhf);
                fprintf(stdout,"sym=%d ",pOverlapMatrix->sym);
                fprintf(stdout,"id=%d ",pOverlapMatrix->id);
                fprintf(stdout,"cmat=%lf ",pOverlapMatrix->cmat);
                fprintf(stdout,"coef=%p ",(void*)pOverlapMatrix->coef);
                fprintf(stdout,"diag=%p ",(void*)pOverlapMatrix->diag);
                fprintf(stdout,"lvect=%p ",(void*)pOverlapMatrix->lvect);
                fprintf(stdout,"rvect=%p\n",(void*)pOverlapMatrix->rvect);
            }
            fprintf(stdout,"\n");
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
        boolean=(boolean || pChemicalSystem->pdet!=NULL);
        boolean=(boolean || pChemicalSystem->pmat!=NULL);

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

