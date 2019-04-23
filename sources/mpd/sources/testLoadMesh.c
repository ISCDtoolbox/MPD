/**
* \file testLoadMesh.c
* \brief All unit testing functions related to loadMesh.c file.
* \author Jeremy DALPHIN
* \version 1.1a
* \date August 1st, 2018
*
* This file contains all the unit testing functions that were built to test
* the functions defined in the loadMesh.c file. We use the convention that
* if a function is called myFunction for example, its associated unit-test
* function is called testMyFunction. All the functions defined in this file
* does not have any input argument and does not return any argument (void input
* and output).
*/

#include "test.h"
#include "loadMesh.h"

// Unit (random) tests on initializeMeshStructure of loadMesh.c file
void testInitializeMeshStructure(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthArray=0;
    int i=0,iRandom=0, boolean=0;
    Mesh *pMesh=NULL, mesh;

    char pIntegerToPrint[10][6]={"nver","ncor","nnorm","ntan","nedg","ntri",
                                                   "ntet","nqua","nhex","nadj"};
    int *pInteger[10]={&mesh.nver,&mesh.ncor,&mesh.nnorm,&mesh.ntan,&mesh.nedg,
                      &mesh.ntri,&mesh.ntet,&mesh.nqua,&mesh.nhex,&mesh.nadj};

    // Initializing to zero the mesh structure
    for (i=0; i<10; i++)
    {
        *pInteger[i]=0;
    }
    mesh.pver=NULL;
    mesh.pnorm=NULL;
    mesh.ptan=NULL;
    mesh.pedg=NULL;
    mesh.ptri=NULL;
    mesh.ptet=NULL;
    mesh.pqua=NULL;
    mesh.phex=NULL;
    mesh.padj=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting initializeMeshStructure function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pMesh=%p\n",(void*)pMesh);
    initializeMeshStructure(pMesh);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pMesh=&mesh;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pMesh=%p\n",(void*)pMesh);
    initializeMeshStructure(pMesh);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random value to the variables
        for (i=0; i<10; i++)
        {
            *pInteger[i]=rand()%161-30;
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pver=NULL;
        }
        else
        {
            pMesh->pver=(Point*)calloc(lengthArray,sizeof(Point));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pnorm=NULL;
        }
        else
        {
            pMesh->pnorm=(Vector*)calloc(lengthArray,sizeof(Vector));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->ptan=NULL;
        }
        else
        {
            pMesh->ptan=(Vector*)calloc(lengthArray,sizeof(Vector));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pedg=NULL;
        }
        else
        {
            pMesh->pedg=(Edge*)calloc(lengthArray,sizeof(Edge));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->ptri=NULL;
        }
        else
        {
            pMesh->ptri=(Triangle*)calloc(lengthArray,sizeof(Triangle));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->ptet=NULL;
        }
        else
        {
            pMesh->ptet=(Tetrahedron*)calloc(lengthArray,sizeof(Tetrahedron));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pqua=NULL;
        }
        else
        {
            pMesh->pqua=(Quadrilateral*)calloc(lengthArray,
                                                         sizeof(Quadrilateral));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->phex=NULL;
        }
        else
        {
            pMesh->phex=(Hexahedron*)calloc(lengthArray,sizeof(Hexahedron));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->padj=NULL;
        }
        else
        {
            pMesh->padj=(Adjacency*)calloc(lengthArray,sizeof(Adjacency));
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        fprintf(stdout,"pver=%p\n",(void*)pMesh->pver);
        fprintf(stdout,"pnorm=%p\n",(void*)pMesh->pnorm);
        fprintf(stdout,"ptan=%p\n",(void*)pMesh->ptan);
        fprintf(stdout,"pedg=%p\n",(void*)pMesh->pedg);
        fprintf(stdout,"ptri=%p\n",(void*)pMesh->ptri);
        fprintf(stdout,"ptet=%p\n",(void*)pMesh->ptet);
        fprintf(stdout,"pqua=%p\n",(void*)pMesh->pqua);
        fprintf(stdout,"phex=%p\n",(void*)pMesh->phex);
        fprintf(stdout,"padj=%p\n",(void*)pMesh->padj);

        free(pMesh->pver);
        free(pMesh->pnorm);
        free(pMesh->ptan);
        free(pMesh->pedg);
        free(pMesh->ptri);
        free(pMesh->ptet);
        free(pMesh->pqua);
        free(pMesh->phex);
        free(pMesh->padj);

        // Testing the function
        initializeMeshStructure(pMesh);

        // Checking if it worked
        boolean=0;
        for (i=0; i<10; i++)
        {
            boolean=(boolean || *pInteger[i]!=0);
        }
        boolean=(boolean || pMesh->pver!=NULL);
        boolean=(boolean || pMesh->pnorm!=NULL);
        boolean=(boolean || pMesh->ptan!=NULL);
        boolean=(boolean || pMesh->pedg!=NULL);
        boolean=(boolean || pMesh->ptri!=NULL);
        boolean=(boolean || pMesh->ptet!=NULL);
        boolean=(boolean || pMesh->pqua!=NULL);
        boolean=(boolean || pMesh->phex!=NULL);
        boolean=(boolean || pMesh->padj!=NULL);

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough initializeMeshStructure function ");
            fprintf(stdout,"ended normally, the structure pointed by pMesh ");
            fprintf(stdout,"has not been initialized properly. One of the ");
            fprintf(stdout,"following variables is not set to zero (or NULL ");
            fprintf(stdout,"for pointers):\n");

            for (i=0; i<10; i++)
            {
                fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
            }
            fprintf(stdout,"pver=%p\n",(void*)pMesh->pver);
            fprintf(stdout,"pnorm=%p\n",(void*)pMesh->pnorm);
            fprintf(stdout,"ptan=%p\n",(void*)pMesh->ptan);
            fprintf(stdout,"pedg=%p\n",(void*)pMesh->pedg);
            fprintf(stdout,"ptri=%p\n",(void*)pMesh->ptri);
            fprintf(stdout,"ptet=%p\n",(void*)pMesh->ptet);
            fprintf(stdout,"pqua=%p\n",(void*)pMesh->pqua);
            fprintf(stdout,"phex=%p\n",(void*)pMesh->phex);
            fprintf(stdout,"padj=%p\n",(void*)pMesh->padj);
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

// Unit (random) tests on freeMeshMemory of loadMesh.c file
void testFreeMeshMemory(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthArray=0;
    int i=0,iRandom=0, boolean=0;
    Mesh *pMesh=NULL, mesh;

    char pIntegerToPrint[10][6]={"nver","ncor","nnorm","ntan","nedg","ntri",
                                                   "ntet","nqua","nhex","nadj"};
    int *pInteger[10]={&mesh.nver,&mesh.ncor,&mesh.nnorm,&mesh.ntan,&mesh.nedg,
                      &mesh.ntri,&mesh.ntet,&mesh.nqua,&mesh.nhex,&mesh.nadj};

    // Initializing to zero the mesh structure
    for (i=0; i<10; i++)
    {
        *pInteger[i]=0;
    }
    mesh.pver=NULL;
    mesh.pnorm=NULL;
    mesh.ptan=NULL;
    mesh.pedg=NULL;
    mesh.ptri=NULL;
    mesh.ptet=NULL;
    mesh.pqua=NULL;
    mesh.phex=NULL;
    mesh.padj=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting freeMeshMemory function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pMesh=%p\n",(void*)pMesh);
    initializeMeshStructure(pMesh);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pMesh=&mesh;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pMesh=%p\n",(void*)pMesh);
    initializeMeshStructure(pMesh);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random value to the variables
        for (i=0; i<10; i++)
        {
            *pInteger[i]=rand()%161-30;
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pver=NULL;
        }
        else
        {
            pMesh->pver=(Point*)calloc(lengthArray,sizeof(Point));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pnorm=NULL;
        }
        else
        {
            pMesh->pnorm=(Vector*)calloc(lengthArray,sizeof(Vector));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->ptan=NULL;
        }
        else
        {
            pMesh->ptan=(Vector*)calloc(lengthArray,sizeof(Vector));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pedg=NULL;
        }
        else
        {
            pMesh->pedg=(Edge*)calloc(lengthArray,sizeof(Edge));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->ptri=NULL;
        }
        else
        {
            pMesh->ptri=(Triangle*)calloc(lengthArray,sizeof(Triangle));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->ptet=NULL;
        }
        else
        {
            pMesh->ptet=(Tetrahedron*)calloc(lengthArray,sizeof(Tetrahedron));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->pqua=NULL;
        }
        else
        {
            pMesh->pqua=(Quadrilateral*)calloc(lengthArray,
                                                         sizeof(Quadrilateral));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->phex=NULL;
        }
        else
        {
            pMesh->phex=(Hexahedron*)calloc(lengthArray,sizeof(Hexahedron));
        }

        lengthArray=rand()%10000+1;
        if (lengthArray>7000)
        {
            pMesh->padj=NULL;
        }
        else
        {
            pMesh->padj=(Adjacency*)calloc(lengthArray,sizeof(Adjacency));
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        fprintf(stdout,"pver=%p\n",(void*)pMesh->pver);
        fprintf(stdout,"pnorm=%p\n",(void*)pMesh->pnorm);
        fprintf(stdout,"ptan=%p\n",(void*)pMesh->ptan);
        fprintf(stdout,"pedg=%p\n",(void*)pMesh->pedg);
        fprintf(stdout,"ptri=%p\n",(void*)pMesh->ptri);
        fprintf(stdout,"ptet=%p\n",(void*)pMesh->ptet);
        fprintf(stdout,"pqua=%p\n",(void*)pMesh->pqua);
        fprintf(stdout,"phex=%p\n",(void*)pMesh->phex);
        fprintf(stdout,"padj=%p\n",(void*)pMesh->padj);

        // Testing the function
        freeMeshMemory(pMesh);
        if (iRandom%3)
        {
            freeMeshMemory(pMesh);
        }

        // Checking if it worked
        boolean=(pMesh->pver!=NULL);
        boolean=(boolean || pMesh->pnorm!=NULL);
        boolean=(boolean || pMesh->ptan!=NULL);
        boolean=(boolean || pMesh->pedg!=NULL);
        boolean=(boolean || pMesh->ptri!=NULL);
        boolean=(boolean || pMesh->ptet!=NULL);
        boolean=(boolean || pMesh->pqua!=NULL);
        boolean=(boolean || pMesh->phex!=NULL);
        boolean=(boolean || pMesh->padj!=NULL);

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough freeMeshMemory function ended ");
            fprintf(stdout,"normally, the structure pointed by pMesh has not ");
            fprintf(stdout,"been freed up properly. One of the following ");
            fprintf(stdout,"pointers is not set to NULL:\n");

            fprintf(stdout,"pver=%p\n",(void*)pMesh->pver);
            fprintf(stdout,"pnorm=%p\n",(void*)pMesh->pnorm);
            fprintf(stdout,"ptan=%p\n",(void*)pMesh->ptan);
            fprintf(stdout,"pedg=%p\n",(void*)pMesh->pedg);
            fprintf(stdout,"ptri=%p\n",(void*)pMesh->ptri);
            fprintf(stdout,"ptet=%p\n",(void*)pMesh->ptet);
            fprintf(stdout,"pqua=%p\n",(void*)pMesh->pqua);
            fprintf(stdout,"phex=%p\n",(void*)pMesh->phex);
            fprintf(stdout,"padj=%p\n",(void*)pMesh->padj);
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

