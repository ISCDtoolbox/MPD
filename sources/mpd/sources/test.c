/**
* \file test.c
* \brief All unit testing functions contained in the main.c file.
* \author Jeremy DALPHIN
* \version 3.0
* \date May 1st, 2019
*
* This file contains all the unit testing functions that were built to test
* the functions defined in main.c file. We use the convention that if a function
* is called myFunction for example, its associated unit-test function is called
* testMyFunction. All the functions defined in this file does not have any
* input argument and does not return any argument (void input and output).
*/

#include "main.h"
#include "loadParameters.h"
#include "loadChemistry.h"
#include "loadMesh.h"
//#include "adaptMesh.h"
#include "optimization.h"
#include "test.h"

////////////////////////////////////////////////////////////////////////////////
// The function test is the function of the test.c file that allows to gather
// all the other unit-testing functions that we want to test in the main
// function (other unit-testing functions should be put in comment in test
// function). We use the convention that if a function is called myFunction
// for example, its associated unit-testing function is called testMyFunction.
// It does not have any input argument and does not return any argument
////////////////////////////////////////////////////////////////////////////////
void test(void)
{
    testCommentAnormalEnd();
    testPrintTimer();
    testEndTimerAtExit();
    testEndTimerAtError();

//    testInitializeParameterStructure();
//    testInitializeChemicalStructure();
//    testInitializeDataStructure();
//    testInitializeMeshStructure();

//    testFreeMeshMemory();
//    testFreeDataMemory();
//    testFreeChemicalMemory();
//    testFreeParameterMemory();

//    testCheckStringFromLength();
//    testCheckAllPreprocessorConstants();
//    testInitialFileExists();
//    testCloseTheFile();

//    testSetupDefaultParameters();
//    testGetLengthAfterKeywordBeginning();
//    testGetTypeAfterKeyword();
//    testGetLengthAfterKeywordMiddle();
//    testDetectRepetition();
//    testChangeValuesOfParameters();

//    testReadInfoFileAndGetParameters();
//    testCheckValuesOfAllParameters();
//    testLoadParameters();

    return;
}

////////////////////////////////////////////////////////////////////////////////
// Unit tests on commentAnormalEnd of main.c file
////////////////////////////////////////////////////////////////////////////////
void testCommentAnormalEnd(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    int typeOfSignal=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting commentAnormalEnd function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=1;
    commentAnormalEnd(typeOfSignal);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (typeOfSignal=-20; typeOfSignal<20; typeOfSignal++)
    {
        if (typeOfSignal!=SIGINT)
        {
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"typeOfSignal=%d\n",typeOfSignal);
            commentAnormalEnd(typeOfSignal);
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
        }
    }

// Warning: uncommenting these lines will kill the MPD program after this test
//  typeOfSignal=SIGINT;
//  PRINT_TEST_START(counter,expectedValue);
//  fprintf(stdout,"typeOfSignal=%d\n",typeOfSignal);
//  commentAnormalEnd(typeOfSignal);
//  PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
//                                                                    readChar);

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
// Unit tests on printTimer of main.c file
////////////////////////////////////////////////////////////////////////////////
void testPrintTimer(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    int i=0;
    time_t pFinalTimer[53]={0,1,2,60,61,62,120,121,122,3600,3601,3602,3660,3661,
                            3662,3720,3721,3722,7200,7201,7202,7260,7261,7262,
                            7320,7321,7322,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                               0,0,0,0,0,0,0,0};

    time_t pInitialTimer[53]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,1,2,60,61,62,120,121,122,3600,3601,3602,
                              3660,3661,3662,3720,3721,3722,7200,7201,7202,7260,
                                                      7261,7262,7320,7321,7322};

    char pStringToPrint[53][36]={"(almost) instantaneous","1 second",
                                 "2 seconds","1 minute","1 minute and 1 second",
                                 "1 minute and 2 seconds","2 minutes",
                                 "2 minutes and 1 second",
                                 "2 minutes and 2 seconds","1 hour",
                                 "1 hour and 1 second","1 hour and 2 seconds",
                                 "1 hour and 1 minute",
                                 "1 hour, 1 minute and 1 second",
                                 "1 hour, 1 minute and 2 seconds",
                                 "1 hour and 2 minutes",
                                 "1 hour, 2 minutes and 1 second",
                                 "1 hour, 2 minutes and 2 seconds",
                                 "2 hours","2 hours and 1 second",
                                 "2 hours and 2 seconds","2 hours and 1 minute",
                                 "2 hours, 1 minute and 1 second",
                                 "2 hours, 1 minute and 2 seconds",
                                 "2 hours and 2 minutes",
                                 "2 hours, 2 minutes and 1 second",
                                 "2 hours, 2 minutes and 2 seconds",
                                 "-1 second","-2 seconds","-1 minute",
                                 "-1 minute and -1 second",
                                 "-1 minute and -2 seconds",
                                 "-2 minutes","-2 minutes and -1 second",
                                 "-2 minutes and -2 seconds",
                                 "-1 hour","-1 hour and -1 second",
                                 "-1 hour and -2 seconds",
                                 "-1 hour and -1 minute",
                                 "-1 hour, -1 minute and -1 second",
                                 "-1 hour, -1 minute and -2 seconds",
                                 "-1 hour and -2 minutes",
                                 "-1 hour, -2 minutes and -1 second",
                                 "-1 hour, -2 minutes and -2 seconds",
                                 "-2 hours","-2 hours and -1 second",
                                 "-2 hours and -2 seconds",
                                 "-2 hours and -1 minute",
                                 "-2 hours, -1 minute and -1 second",
                                 "-2 hours, -1 minute and -2 seconds",
                                 "-2 hours and -2 minutes",
                                 "-2 hours, -2 minutes and -1 second",
                                         "-2 hours, -2 minutes and -2 seconds"};

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting printTimer function.\n");

    expectedValue=1;
    for (i=0; i<53; i++)
    {
        PRINT_TEST_START(counter,expectedValue);
        returnValue=1;
        fprintf(stdout,"The program should print '%s': ",pStringToPrint[i]);
        printTimer(pFinalTimer[i],pInitialTimer[i]);
        fprintf(stdout,".\n");
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
// Unit tests on endTimerAtExit of main.c file
////////////////////////////////////////////////////////////////////////////////
void testEndTimerAtExit(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting endTimerAtExit function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=1;
    endTimerAtExit();
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

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
// Unit tests on endTimerAtError of main.c file
////////////////////////////////////////////////////////////////////////////////
void testEndTimerAtError(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0; 

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting endTimerAtError function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=1;
    fprintf(stdout,"Function endTimerAtError returns: %s",endTimerAtError());
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

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
// Unit (random) tests on checkStringFromLength of main.c file
void testCheckStringFromLength(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char stringToCheck[16]={'\0'}, *pStringToCheck=NULL;
    int i=0, iTest=0, minimumLength=0, maximumLength=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting checkStringFromLength function.\n");
    stringToCheck[15]='\0';

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"stringToCheck=%p\n",(void*)pStringToCheck);
    fprintf(stdout,"minimumLength=%d\n",minimumLength);
    fprintf(stdout,"maximumLength=%d\n",maximumLength);
    returnValue=checkStringFromLength(pStringToCheck,minimumLength,
                                                                 maximumLength);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);

    pStringToCheck=stringToCheck;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"stringToCheck=%s\n",pStringToCheck);
    fprintf(stdout,"minimumLength=%d\n",minimumLength);
    fprintf(stdout,"maximumLength=%d\n",maximumLength);
    returnValue=checkStringFromLength(pStringToCheck,minimumLength,
                                                                 maximumLength);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);

    for (minimumLength=-10; minimumLength<11; minimumLength++)
    {
        for (maximumLength=-10; maximumLength<11; maximumLength++)
        {
            for (iTest=0; iTest<32; iTest++)
            {
                switch(iTest)
                {
                    case 0:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-5)
                            {
                                stringToCheck[i]='a';
                            }
                            else if (i==maximumLength-5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='A';
                            }
                        }
                        break;

                    case 1:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-5)
                            {
                                stringToCheck[i]='b';
                            }
                            else if (i==maximumLength-5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 2:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-5)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength-5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='C';
                            }
                        }
                        break;

                    case 3:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-5)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength-5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 4:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-5)
                            {
                                stringToCheck[i]='d';
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 5:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-5)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 6:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-1)
                            {
                                stringToCheck[i]='e';
                            }
                            else if (i==maximumLength-1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='E';
                            }
                        }
                        break;

                    case 7:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-1)
                            {
                                stringToCheck[i]='f';
                            }
                            else if (i==maximumLength-1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 8:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-1)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength-1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='G';
                            }
                        }
                        break;

                    case 9:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-1)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength-1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 10:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-1)
                            {
                                stringToCheck[i]='h';
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 11:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength-1)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 12:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength)
                            {
                                stringToCheck[i]='i';
                            }
                            else if (i==maximumLength)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='I';
                            }
                        }
                        break;

                    case 13:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength)
                            {
                                stringToCheck[i]='j';
                            }
                            else if (i==maximumLength)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 14:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='K';
                            }
                        }
                        break;

                    case 15:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 16:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength)
                            {
                                stringToCheck[i]='l';
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 17:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 18:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+1)
                            {
                                stringToCheck[i]='m';
                            }
                            else if (i==maximumLength+1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='M';
                            }
                        }
                        break;

                    case 19:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+1)
                            {
                                stringToCheck[i]='n';
                            }
                            else if (i==maximumLength+1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 20:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+1)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength+1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='O';
                            }
                        }
                        break;

                    case 21:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+1)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength+1)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 22:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+1)
                            {
                                stringToCheck[i]='p';
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 23:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+1)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 24:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+5)
                            {
                                stringToCheck[i]='q';
                            }
                            else if (i==maximumLength+5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='Q';
                            }
                        }
                        break;

                    case 25:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+5)
                            {
                                stringToCheck[i]='r';
                            }
                            else if (i==maximumLength+5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 26:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+5)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength+5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]='S';
                            }
                        }
                        break;

                    case 27:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+5)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else if (i==maximumLength+5)
                            {
                                stringToCheck[i]='\0';
                            }
                            else
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                        }
                        break;

                    case 28:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+5)
                            {
                                stringToCheck[i]='t';
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 29:
                        for (i=0; i<15; i++)
                        {
                            if (i<maximumLength+5)
                            {
                                stringToCheck[i]=rand()%90+33;
                            }
                            else
                            {
                                stringToCheck[i]='\0';
                            }
                        }
                        break;

                    case 30:
                        for (i=0; i<15; i++)
                        {
                            stringToCheck[i]=rand()%90+33;
                        }
                        break;

                    case 31:
                        for (i=0; i<15; i++)
                        {
                            stringToCheck[i]=rand()%90+33;
                        }
                        stringToCheck[rand()%15]='\0';
                        break;
                }

                expectedValue=strlen(stringToCheck)+1;
                if (minimumLength<1 || maximumLength<minimumLength ||
                     expectedValue>maximumLength || expectedValue<minimumLength)
                {
                    expectedValue=0;
                }

                PRINT_TEST_START(counter,expectedValue);
                fprintf(stdout,"stringToCheck=%s\n",pStringToCheck);
                fprintf(stdout,"minimumLength=%d\n",minimumLength);
                fprintf(stdout,"maximumLength=%d\n",maximumLength);
                returnValue=checkStringFromLength(pStringToCheck,minimumLength,
                                                                 maximumLength);
                PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            }
        }
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

// Unit tests on checkAllPreprocessorConstants of main.c file
void testCheckAllPreprocessorConstants(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char pathTest[PATH_LENGTH+11]={'\0'}, *pPathMedit=NULL, *pPathMmg3d=NULL;
    char *pPathMshdist=NULL, *pPathElastic=NULL, *pPathAdvect=NULL;
    char pathMedit[6]=PATH_MEDIT, pathMmg3d[9]=PATH_MMG3D;
    char pathMshdist[8]=PATH_MSHDIST, pathElastic[8]=PATH_ELASTIC;
    char pathAdvect[7]=PATH_ADVECT;

    int i=0, iPathTest=0, iPathMedit=0, iPathMmg3d=0, iPathMshdist=0;
    int iPathElastic=0, iPathAdvect=0, optMode=0, verbose=0, nCpu=0;
    int nameLength=0, nX=0, nY=0, nZ=0, lsType=0, trickMatrix=0, approxMode=0;
    int iterMax=0, saveType=0, saveMesh=0, saveData=0, savePrint=0, saveWhere=0;
    int pathLength=0, hmodeLag=0, nIter=0, noCfl=0, orb1=0, orb2=0, orb3=0;
    int orb4=0, orb5=0, orb6=0, orb7=0, orb8=0, orb9=0, orb10=0, orb11=0;
    int orb12=0, orb13=0, orb14=0, orb15=0, orb16=0, orb17=0, orb18=0, orb19=0;
    int orb20=0;

    double lameInt1=0., lameInt2=0., lameExt1=0., lameExt2=0., xMin=0., yMin=0.;
    double zMin=0., xMax=0., yMax=0., zMax=0., deltaX=0., deltaY=0., deltaZ=0.;
    double lsX=0., lsY=0., lsZ=0., lsR=0., metCst=0., metErr=0., metMin=0.;
    double metMax=0., iterTolD0P=0., iterTolD1P=0., iterTolD2P=0., hminIso=0.;
    double hmaxIso=0., hausdIso=0., hgradIso=0., hminMet=0., hmaxMet=0.;
    double hausdMet=0., hgradMet=0., hminLs=0., hmaxLs=0., hausdLs=0.;
    double hgradLs=0., hminLag=0., hmaxLag=0., hausdLag=0., hgradLag=0.;
    double residual=0., deltaT=0., cstA=0., cstB=0., cstC=0., csta=0., cstb=0.;
    double cstc=0., cstaa=0., cstbb=0., cstcc=0.;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting checkAllPreprocessorConstants function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,nameLength,
                                              lameInt1,lameInt2,lameExt1,
                                              lameExt2,xMin,yMin,zMin,xMax,yMax,
                                              zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterMax,
                                              iterTolD0P,iterTolD1P,iterTolD2P,
                                              saveType,saveMesh,saveData,
                                              savePrint,saveWhere,pathLength,
                                              pPathMedit,pPathMmg3d,
                                              pPathMshdist,pPathElastic,
                                              pPathAdvect,hminIso,hmaxIso,
                                              hausdIso,hgradIso,hminMet,hmaxMet,
                                              hausdMet,hgradMet,hminLs,hmaxLs,
                                              hausdLs,hgradLs,hmodeLag,hminLag,
                                              hmaxLag,hausdLag,hgradLag,nIter,
                                              residual,deltaT,noCfl,orb1,orb2,
                                              orb3,orb4,orb5,orb6,orb7,orb8,
                                              orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                                   cstb,cstc,cstaa,cstbb,cstcc);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (optMode=-3; optMode<6; optMode++)
    {
        for (verbose=-1; verbose<3; verbose++)
        {
            for (nCpu=-2; nCpu<5; nCpu+=2)
            {
                for (nameLength=-50; nameLength<=100; nameLength+=50)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                nameLength=NAME_LENGTH;
            }
            nCpu=N_CPU;
        }
        verbose=VERBOSE;
    }
    optMode=OPT_MODE;

    for (lameInt1=-30000.; lameInt1<=30000.; lameInt1+=15000.)
    {
        for (lameInt2=-30000.; lameInt2<=30000.; lameInt2+=15000.)
        {
            for (lameExt1=-3000.; lameExt1<=3000.; lameExt1+=1500.)
            {
                for (lameExt2=-3000.; lameExt2<=3000.; lameExt2+=1500.)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                lameExt2=LAME_EXT2;
            }
            lameExt1=LAME_EXT1;
        }
        lameInt2=LAME_INT2;
    }
    lameInt1=LAME_INT1;

    yMin=Y_MIN;
    yMax=Y_MAX;
    nY=N_Y;
    zMin=Z_MIN;
    zMax=Z_MAX;
    nZ=N_Z;
    for (xMin=3.5; xMin>=-3.5; xMin-=1.75)
    {
        for (xMax=-3.5; xMax<=3.5; xMax+=1.75)
        {
            for (nX=-71; nX<=71; nX+=71)
            {
                for (deltaX=-.2; deltaX<=.2; deltaX+=.1)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                deltaX=DELTA_X;
            }
            nX=N_X;
        }
        xMax=X_MAX;
    }
    xMin=X_MIN;

    for (yMin=3.5; yMin>=-3.5; yMin-=1.75)
    {
        for (yMax=-3.5; yMax<=3.5; yMax+=1.75)
        {
            for (nY=-71; nY<=71; nY+=71)
            {
                for (deltaY=-.2; deltaY<=.2; deltaY+=.1)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                deltaY=DELTA_Y;
            }
            nY=N_Y;
        }
        yMax=Y_MAX;
    }
    yMin=Y_MIN;

    for (zMin=3.5; zMin>=-3.5; zMin-=1.75)
    {
        for (zMax=-3.5; zMax<=3.5; zMax+=1.75)
        {
            for (nZ=-71; nZ<=71; nZ+=71)
            {
                for (deltaZ=-.2; deltaZ<=.2; deltaZ+=.1)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                deltaZ=DELTA_Z;
            }
            nZ=N_Z;
        }
        zMax=Z_MAX;
    }
    zMin=Z_MIN;

    for (lsType=-1; lsType<3; lsType++)
    {
        for (lsX=-4.; lsX<=4.; lsX+=2.)
        {
            for (lsY=-4.; lsY<=4.; lsY+=2.)
            {
                for (lsZ=-4.; lsZ<=4.; lsZ+=2.)
                {
                    for (lsR=-1.; lsR<=1.; lsR+=.5)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    lsR=LS_R;
                }
                lsZ=LS_Z;
            }
            lsY=LS_Y;
        }
        lsX=LS_X;
    }
    lsType=LS_TYPE;

    for (metCst=-MET_CST; metCst<=2.*MET_CST; metCst+=MET_CST)
    {
        for (metErr=-.2; metErr<=.4; metErr+=.2)
        {
            for (metMin=.4; metMin>=-.2; metMin-=.2)
            {
                for (metMax=-.2; metMax<=.4; metMax+=.2)
                {
                    for (trickMatrix=-1; trickMatrix<3; trickMatrix++)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    trickMatrix=TRICK_MATRIX;
                }
                metMax=MET_MAX;
            }
            metMin=MET_MIN;
        }
        metErr=MET_ERR;
    }
    metCst=MET_CST;

    for (approxMode=-1; approxMode<3; approxMode++)
    {
        for (iterMax=-10; iterMax<11; iterMax+=5)
        {
            for (iterTolD0P=-.05; iterTolD0P<=.05; iterTolD0P+=.05)
            {
                for (iterTolD1P=-.04; iterTolD1P<=.04; iterTolD1P+=.04)
                {
                    for (iterTolD2P=-.03; iterTolD2P<=.03; iterTolD2P+=.043)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    iterTolD2P=ITER_TOLD2P;
                }
                iterTolD1P=ITER_TOLD1P;
            }
            iterTolD0P=ITER_TOLD0P;
        }
        iterMax=ITER_MAX;
    }
    approxMode=APPROX_MODE;

    for (saveType=-1; saveType<4; saveType++)
    {
        for (saveMesh=-4; saveMesh<=4; saveMesh+=2)
        {
            for (saveData=-4; saveData<=4; saveData+=2)
            {
                for (savePrint=-4; savePrint<=4; savePrint+=2)
                {
                    for (saveWhere=-1; saveWhere<9; saveWhere++)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    saveWhere=SAVE_WHERE;
                }
                savePrint=SAVE_PRINT;
            }
            saveData=SAVE_DATA;
        }
        saveMesh=SAVE_MESH;
    }
    saveType=SAVE_TYPE;

    pathTest[PATH_LENGTH+10]='\0';
    for (pathLength=-PATH_LENGTH+2; pathLength<=PATH_LENGTH+2;
                                                        pathLength+=PATH_LENGTH)
    {
        for (iPathTest=0; iPathTest<32; iPathTest++)
        {
            switch(iPathTest)
            {
                case 0:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-5)
                        {
                            pathTest[i]='a';
                        }
                        else if (i==pathLength-5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='A';
                        }
                    }
                    break;

                case 1:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-5)
                        {
                            pathTest[i]='b';
                        }
                        else if (i==pathLength-5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 2:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-5)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength-5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='C';
                        }
                    }
                    break;

                case 3:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-5)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength-5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 4:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-5)
                        {
                            pathTest[i]='d';
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 5:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-5)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 6:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-1)
                        {
                            pathTest[i]='e';
                        }
                        else if (i==pathLength-1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='E';
                        }
                    }
                    break;

                case 7:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-1)
                        {
                            pathTest[i]='f';
                        }
                        else if (i==pathLength-1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 8:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-1)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength-1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='G';
                        }
                    }
                    break;

                case 9:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-1)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength-1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 10:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-1)
                        {
                            pathTest[i]='h';
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 11:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength-1)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 12:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength)
                        {
                            pathTest[i]='i';
                        }
                        else if (i==pathLength)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='I';
                        }
                    }
                    break;

                case 13:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength)
                        {
                            pathTest[i]='j';
                        }
                        else if (i==pathLength)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 14:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='K';
                        }
                    }
                    break;

                case 15:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 16:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength)
                        {
                            pathTest[i]='l';
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 17:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 18:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+1)
                        {
                            pathTest[i]='m';
                        }
                        else if (i==pathLength+1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='M';
                        }
                    }
                    break;

                case 19:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+1)
                        {
                            pathTest[i]='n';
                        }
                        else if (i==pathLength+1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 20:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+1)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength+1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='O';
                        }
                    }
                    break;

                case 21:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+1)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength+1)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 22:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+1)
                        {
                            pathTest[i]='p';
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 23:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+1)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 24:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+5)
                        {
                            pathTest[i]='q';
                        }
                        else if (i==pathLength+5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='Q';
                        }
                    }
                    break;

                case 25:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+5)
                        {
                            pathTest[i]='r';
                        }
                        else if (i==pathLength+5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 26:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+5)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength+5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]='S';
                        }
                    }
                    break;

                case 27:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+5)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else if (i==pathLength+5)
                        {
                            pathTest[i]='\0';
                        }
                        else
                        {
                            pathTest[i]=rand()%90+33;
                        }
                    }
                    break;

                case 28:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+5)
                        {
                            pathTest[i]='t';
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 29:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        if (i<pathLength+5)
                        {
                            pathTest[i]=rand()%90+33;
                        }
                        else
                        {
                            pathTest[i]='\0';
                        }
                    }
                    break;

                case 30:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        pathTest[i]=rand()%90+33;
                    }
                    break;

                case 31:
                    for (i=0; i<PATH_LENGTH+10;  i++)
                    {
                        pathTest[i]=rand()%90+33;
                    }
                    pathTest[rand()%(PATH_LENGTH+10)]='\0';
                    break;
            }
            pPathMedit=pathTest;
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"pathLength=%d\n",pathLength);
            fprintf(stdout,"pathMedit=%s\n",pPathMedit);
            fprintf(stdout,"(%ld characters)\n",strlen(pPathMedit));
            returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                      nameLength,lameInt1,
                                                      lameInt2,lameExt1,
                                                      lameExt2,xMin,yMin,zMin,
                                                      xMax,yMax,zMax,nX,nY,nZ,
                                                      deltaX,deltaY,deltaZ,
                                                      lsType,lsX,lsY,lsZ,lsR,
                                                      metCst,metErr,metMin,
                                                      metMax,trickMatrix,
                                                      approxMode,iterMax,
                                                      iterTolD0P,iterTolD1P,
                                                      iterTolD2P,saveType,
                                                      saveMesh,saveData,
                                                      savePrint,saveWhere,
                                                      pathLength,pPathMedit,
                                                      pPathMmg3d,pPathMshdist,
                                                      pPathElastic,pPathAdvect,
                                                      hminIso,hmaxIso,hausdIso,
                                                      hgradIso,hminMet,hmaxMet,
                                                      hausdMet,hgradMet,hminLs,
                                                      hmaxLs,hausdLs,hgradLs,
                                                      hmodeLag,hminLag,hmaxLag,
                                                      hausdLag,hgradLag,nIter,
                                                      residual,deltaT,noCfl,
                                                      orb1,orb2,orb3,orb4,orb5,
                                                      orb6,orb7,orb8,orb9,orb10,
                                                      orb11,orb12,orb13,orb14,
                                                      orb15,orb16,orb17,orb18,
                                                      orb19,orb20,cstA,cstB,
                                                      cstC,csta,cstb,cstc,cstaa,
                                                                   cstbb,cstcc);
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            pPathMedit=pathMedit;
            pPathMmg3d=pathTest;
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"pathLength=%d\n",pathLength);
            fprintf(stdout,"pathMmg3d=%s\n",pPathMmg3d);
            fprintf(stdout,"(%ld characters)\n",strlen(pPathMmg3d));
            returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                      nameLength,lameInt1,
                                                      lameInt2,lameExt1,
                                                      lameExt2,xMin,yMin,zMin,
                                                      xMax,yMax,zMax,nX,nY,nZ,
                                                      deltaX,deltaY,deltaZ,
                                                      lsType,lsX,lsY,lsZ,lsR,
                                                      metCst,metErr,metMin,
                                                      metMax,trickMatrix,
                                                      approxMode,iterMax,
                                                      iterTolD0P,iterTolD1P,
                                                      iterTolD2P,saveType,
                                                      saveMesh,saveData,
                                                      savePrint,saveWhere,
                                                      pathLength,pPathMedit,
                                                      pPathMmg3d,pPathMshdist,
                                                      pPathElastic,pPathAdvect,
                                                      hminIso,hmaxIso,hausdIso,
                                                      hgradIso,hminMet,hmaxMet,
                                                      hausdMet,hgradMet,hminLs,
                                                      hmaxLs,hausdLs,hgradLs,
                                                      hmodeLag,hminLag,hmaxLag,
                                                      hausdLag,hgradLag,nIter,
                                                      residual,deltaT,noCfl,
                                                      orb1,orb2,orb3,orb4,orb5,
                                                      orb6,orb7,orb8,orb9,orb10,
                                                      orb11,orb12,orb13,orb14,
                                                      orb15,orb16,orb17,orb18,
                                                      orb19,orb20,cstA,cstB,
                                                      cstC,csta,cstb,cstc,cstaa,
                                                                   cstbb,cstcc);
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            pPathMmg3d=pathMmg3d;
            pPathMshdist=pathTest;
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"pathLength=%d\n",pathLength);
            fprintf(stdout,"pathMshdist=%s\n",pPathMshdist);
            fprintf(stdout,"(%ld characters)\n",strlen(pPathMshdist));
            returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                      nameLength,lameInt1,
                                                      lameInt2,lameExt1,
                                                      lameExt2,xMin,yMin,zMin,
                                                      xMax,yMax,zMax,nX,nY,nZ,
                                                      deltaX,deltaY,deltaZ,
                                                      lsType,lsX,lsY,lsZ,lsR,
                                                      metCst,metErr,metMin,
                                                      metMax,trickMatrix,
                                                      approxMode,iterMax,
                                                      iterTolD0P,iterTolD1P,
                                                      iterTolD2P,saveType,
                                                      saveMesh,saveData,
                                                      savePrint,saveWhere,
                                                      pathLength,pPathMedit,
                                                      pPathMmg3d,pPathMshdist,
                                                      pPathElastic,pPathAdvect,
                                                      hminIso,hmaxIso,hausdIso,
                                                      hgradIso,hminMet,hmaxMet,
                                                      hausdMet,hgradMet,hminLs,
                                                      hmaxLs,hausdLs,hgradLs,
                                                      hmodeLag,hminLag,hmaxLag,
                                                      hausdLag,hgradLag,nIter,
                                                      residual,deltaT,noCfl,
                                                      orb1,orb2,orb3,orb4,orb5,
                                                      orb6,orb7,orb8,orb9,orb10,
                                                      orb11,orb12,orb13,orb14,
                                                      orb15,orb16,orb17,orb18,
                                                      orb19,orb20,cstA,cstB,
                                                      cstC,csta,cstb,cstc,cstaa,
                                                                   cstbb,cstcc);
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            pPathMshdist=pathMshdist;
            pPathElastic=pathTest;
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"pathLength=%d\n",pathLength);
            fprintf(stdout,"pathElastic=%s\n",pPathElastic);
            fprintf(stdout,"(%ld characters)\n",strlen(pPathElastic));
            returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                      nameLength,lameInt1,
                                                      lameInt2,lameExt1,
                                                      lameExt2,xMin,yMin,zMin,
                                                      xMax,yMax,zMax,nX,nY,nZ,
                                                      deltaX,deltaY,deltaZ,
                                                      lsType,lsX,lsY,lsZ,lsR,
                                                      metCst,metErr,metMin,
                                                      metMax,trickMatrix,
                                                      approxMode,iterMax,
                                                      iterTolD0P,iterTolD1P,
                                                      iterTolD2P,saveType,
                                                      saveMesh,saveData,
                                                      savePrint,saveWhere,
                                                      pathLength,pPathMedit,
                                                      pPathMmg3d,pPathMshdist,
                                                      pPathElastic,pPathAdvect,
                                                      hminIso,hmaxIso,hausdIso,
                                                      hgradIso,hminMet,hmaxMet,
                                                      hausdMet,hgradMet,hminLs,
                                                      hmaxLs,hausdLs,hgradLs,
                                                      hmodeLag,hminLag,hmaxLag,
                                                      hausdLag,hgradLag,nIter,
                                                      residual,deltaT,noCfl,
                                                      orb1,orb2,orb3,orb4,orb5,
                                                      orb6,orb7,orb8,orb9,orb10,
                                                      orb11,orb12,orb13,orb14,
                                                      orb15,orb16,orb17,orb18,
                                                      orb19,orb20,cstA,cstB,
                                                      cstC,csta,cstb,cstc,cstaa,
                                                                   cstbb,cstcc);
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            pPathElastic=pathElastic;
            pPathAdvect=pathTest;
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"pathLength=%d\n",pathLength);
            fprintf(stdout,"pathAdvect=%s\n",pPathAdvect);
            fprintf(stdout,"(%ld characters)\n",strlen(pPathAdvect));
            returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                      nameLength,lameInt1,
                                                      lameInt2,lameExt1,
                                                      lameExt2,xMin,yMin,zMin,
                                                      xMax,yMax,zMax,nX,nY,nZ,
                                                      deltaX,deltaY,deltaZ,
                                                      lsType,lsX,lsY,lsZ,lsR,
                                                      metCst,metErr,metMin,
                                                      metMax,trickMatrix,
                                                      approxMode,iterMax,
                                                      iterTolD0P,iterTolD1P,
                                                      iterTolD2P,saveType,
                                                      saveMesh,saveData,
                                                      savePrint,saveWhere,
                                                      pathLength,pPathMedit,
                                                      pPathMmg3d,pPathMshdist,
                                                      pPathElastic,pPathAdvect,
                                                      hminIso,hmaxIso,hausdIso,
                                                      hgradIso,hminMet,hmaxMet,
                                                      hausdMet,hgradMet,hminLs,
                                                      hmaxLs,hausdLs,hgradLs,
                                                      hmodeLag,hminLag,hmaxLag,
                                                      hausdLag,hgradLag,nIter,
                                                      residual,deltaT,noCfl,
                                                      orb1,orb2,orb3,orb4,orb5,
                                                      orb6,orb7,orb8,orb9,orb10,
                                                      orb11,orb12,orb13,orb14,
                                                      orb15,orb16,orb17,orb18,
                                                      orb19,orb20,cstA,cstB,
                                                      cstC,csta,cstb,cstc,cstaa,
                                                                   cstbb,cstcc);
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            pPathAdvect=pathAdvect;
        }
    }

    for (pathLength=-50; pathLength<=100; pathLength+=50)
    {
        for (iPathMedit=-10; iPathMedit<=10; iPathMedit+=10)
        {
            pPathMedit=NULL;
            if (iPathMedit>0)
            {
                pPathMedit=pathMedit;
            }
            for (iPathMmg3d=-10; iPathMmg3d<=10; iPathMmg3d+=10)
            {
                pPathMmg3d=NULL;
                if (iPathMmg3d>0)
                {
                    pPathMmg3d=pathMmg3d;
                }
                for (iPathMshdist=-10; iPathMshdist<=10; iPathMshdist+=10)
                {
                    pPathMshdist=NULL;
                    if (iPathMshdist>0)
                    {
                         pPathMshdist=pathMshdist;
                    }
                    for (iPathElastic=-10; iPathElastic<=10; iPathElastic+=10)
                    {
                        pPathElastic=NULL;
                        if (iPathElastic>0)
                        {
                            pPathElastic=pathElastic;
                        }
                        for (iPathAdvect=-10; iPathAdvect<=10; iPathAdvect+=10)
                        {
                            pPathAdvect=NULL;
                            if (iPathAdvect>0)
                            {
                                pPathAdvect=pathAdvect;
                            }
                            PRINT_TEST_START(counter,expectedValue);
                            returnValue=checkAllPreprocessorConstants(optMode,
                                                                   verbose,
                                                                   nCpu,
                                                                   nameLength,
                                                                   lameInt1,
                                                                   lameInt2,
                                                                   lameExt1,
                                                                   lameExt2,
                                                                   xMin,yMin,
                                                                   zMin,xMax,
                                                                   yMax,zMax,
                                                                   nX,nY,nZ,
                                                                   deltaX,
                                                                   deltaY,
                                                                   deltaZ,
                                                                   lsType,lsX,
                                                                   lsY,lsZ,lsR,
                                                                   metCst,
                                                                   metErr,
                                                                   metMin,
                                                                   metMax,
                                                                   trickMatrix,
                                                                   approxMode,
                                                                   iterMax,
                                                                   iterTolD0P,
                                                                   iterTolD1P,
                                                                   iterTolD2P,
                                                                   saveType,
                                                                   saveMesh,
                                                                   saveData,
                                                                   savePrint,
                                                                   saveWhere,
                                                                   pathLength,
                                                                   pPathMedit,
                                                                   pPathMmg3d,
                                                                   pPathMshdist,
                                                                   pPathElastic,
                                                                   pPathAdvect,
                                                                   hminIso,
                                                                   hmaxIso,
                                                                   hausdIso,
                                                                   hgradIso,
                                                                   hminMet,
                                                                   hmaxMet,
                                                                   hausdMet,
                                                                   hgradMet,
                                                                   hminLs,
                                                                   hmaxLs,
                                                                   hausdLs,
                                                                   hgradLs,
                                                                   hmodeLag,
                                                                   hminLag,
                                                                   hmaxLag,
                                                                   hausdLag,
                                                                   hgradLag,
                                                                   nIter,
                                                                   residual,
                                                                   deltaT,
                                                                   noCfl,orb1,
                                                                   orb2,orb3,
                                                                   orb4,orb5,
                                                                   orb6,orb7,
                                                                   orb8,orb9,
                                                                   orb10,orb11,
                                                                   orb12,orb13,
                                                                   orb14,orb15,
                                                                   orb16,orb17,
                                                                   orb18,orb19,
                                                                   orb20,cstA,
                                                                   cstB,cstC,
                                                                   csta,cstb,
                                                                   cstc,cstaa,
                                                                   cstbb,cstcc);
                              PRINT_TEST_END(counter,counterSuccess,counterFail,
                                                   returnValue,expectedValue,
                                                                      readChar);
                        }
                        pPathAdvect=pathAdvect;
                    }
                    pPathElastic=pathElastic;
                }
                pPathMshdist=pathMshdist;
            }
            pPathMmg3d=pathMmg3d;
        }
        pPathMedit=pathMedit;
    }
    pathLength=PATH_LENGTH;

    for (hminIso=-.04; hminIso<=.04; hminIso+=.04)
    {
        for (hmaxIso=-1.; hmaxIso<=1.; hmaxIso+=1.)
        {
            for (hausdIso=-.01; hausdIso<=.01; hausdIso+=.01)
            {
                for (hgradIso=-1.; hgradIso<=2.; hgradIso+=1.)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                hgradIso=HGRAD_ISO;
            }
            hausdIso=HAUSD_ISO;
        }
        hmaxIso=HMAX_ISO;
    }
    hminIso=HMIN_ISO;

    for (hminMet=-.04; hminMet<=.04; hminMet+=.04)
    {
        for (hmaxMet=-1.; hmaxMet<=1.; hmaxMet+=1.)
        {
            for (hausdMet=-.01; hausdMet<=.01; hausdMet+=.01)
            {
                for (hgradMet=-1.; hgradMet<=2.; hgradMet+=1.)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                hgradMet=HGRAD_MET;
            }
            hausdMet=HAUSD_MET;
        }
        hmaxMet=HMAX_MET;
    }
    hminMet=HMIN_MET;

    for (hminLs=-.04; hminLs<=.04; hminLs+=.04)
    {
        for (hmaxLs=-1.; hmaxLs<=1.; hmaxLs+=1.)
        {
            for (hausdLs=-.01; hausdLs<=.01; hausdLs+=.01)
            {
                for (hgradLs=-1.; hgradLs<=2.; hgradLs+=1.)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                hgradLs=HGRAD_LS;
            }
            hausdLs=HAUSD_LS;
        }
        hmaxLs=HMAX_LS;
    }
    hminLs=HMIN_LS;

    for (hmodeLag=-1; hmodeLag<4; hmodeLag++)
    {
        for (hminLag=-.04; hminLag<=.04; hminLag+=.04)
        {
            for (hmaxLag=-1.; hmaxLag<=1.; hmaxLag+=1.)
            {
                for (hausdLag=-.01; hausdLag<=.01; hausdLag+=.01)
                {
                    for (hgradLag=-1.; hgradLag<=2.; hgradLag+=1.)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                   }
                    hgradLag=HGRAD_LAG;
                }
                hausdLag=HAUSD_LAG;
            }
            hmaxLag=HMAX_LAG;
        }
        hminLag=HMIN_LAG;
    }
    hmodeLag=HMODE_LAG;

    for (nIter=-2; nIter<5; nIter+=2)
    {
        for (residual=-.05; residual<=.05; residual+=.05)
        {
            for (deltaT=-.5; deltaT<=1.; deltaT+=.5)
            {
                for (noCfl=-1; noCfl<3; noCfl++)
                {
                    PRINT_TEST_START(counter,expectedValue);
                    returnValue=checkAllPreprocessorConstants(optMode,verbose,
                                                              nCpu,nameLength,
                                                              lameInt1,lameInt2,
                                                              lameExt1,lameExt2,
                                                              xMin,yMin,zMin,
                                                              xMax,yMax,zMax,nX,
                                                              nY,nZ,deltaX,
                                                              deltaY,deltaZ,
                                                              lsType,lsX,lsY,
                                                              lsZ,lsR,metCst,
                                                              metErr,metMin,
                                                              metMax,
                                                              trickMatrix,
                                                              approxMode,
                                                              iterMax,
                                                              iterTolD0P,
                                                              iterTolD1P,
                                                              iterTolD2P,
                                                              saveType,saveMesh,
                                                              saveData,
                                                              savePrint,
                                                              saveWhere,
                                                              pathLength,
                                                              pPathMedit,
                                                              pPathMmg3d,
                                                              pPathMshdist,
                                                              pPathElastic,
                                                              pPathAdvect,
                                                              hminIso,hmaxIso,
                                                              hausdIso,hgradIso,
                                                              hminMet,hmaxMet,
                                                              hausdMet,hgradMet,
                                                              hminLs,hmaxLs,
                                                              hausdLs,hgradLs,
                                                              hmodeLag,hminLag,
                                                              hmaxLag,hausdLag,
                                                              hgradLag,nIter,
                                                              residual,deltaT,
                                                              noCfl,orb1,orb2,
                                                              orb3,orb4,orb5,
                                                              orb6,orb7,orb8,
                                                              orb9,orb10,orb11,
                                                              orb12,orb13,orb14,
                                                              orb15,orb16,orb17,
                                                              orb18,orb19,orb20,
                                                              cstA,cstB,cstC,
                                                              csta,cstb,cstc,
                                                              cstaa,cstbb,
                                                                         cstcc);
                    PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                }
                noCfl=NO_CFL;
            }
            deltaT=DELTA_T;
        }
        residual=RESIDUAL;
    }
    nIter=N_ITER;

    for (orb1=-ORB_1S; orb1<=ORB_1S; orb1+=ORB_1S)
    {
        for (orb2=-ORB_2PX; orb2<=ORB_2PX; orb2+=ORB_2PX)
        {
            for (orb3=-ORB_2PY; orb3<=ORB_2PY; orb3+=ORB_2PY)
            {
                for (orb4=-ORB_2PZ; orb4<=ORB_2PZ; orb4+=ORB_2PZ)
                {
                     for (orb5=-ORB_3DXX; orb5<=ORB_3DXX; orb5+=ORB_3DXX)
                     {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb5=ORB_3DXX;
                }
                orb4=ORB_2PZ;
            }
            orb3=ORB_2PY;
        }
        orb2=ORB_2PX;
    }
    orb1=ORB_1S;

    for (orb6=-ORB_3DYY; orb6<=ORB_3DYY; orb6+=ORB_3DYY)
    {
        for (orb7=-ORB_3DZZ; orb7<=ORB_3DZZ; orb7+=ORB_3DZZ)
        {
            for (orb8=-ORB_3DXY; orb8<=ORB_3DXY; orb8+=ORB_3DXY)
            {
                for (orb9=-ORB_3DXZ; orb9<=ORB_3DXZ; orb9+=ORB_3DXZ)
                {
                    for (orb10=-ORB_3DYZ; orb10<=ORB_3DYZ; orb10+=ORB_3DYZ)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb10=ORB_3DYZ;
                }
                orb9=ORB_3DXZ;
            }
            orb8=ORB_3DXY;
        }
        orb7=ORB_3DZZ;
    }
    orb6=ORB_3DYY;

    for (orb11=-ORB_4FXXX; orb11<=ORB_4FXXX; orb11+=ORB_4FXXX)
    {
        for (orb12=-ORB_4FYYY; orb12<=ORB_4FYYY; orb12+=ORB_4FYYY)
        {
            for (orb13=-ORB_4FZZZ; orb13<=ORB_4FZZZ; orb13+=ORB_4FZZZ)
            {
                for (orb14=-ORB_4FXXY; orb14<=ORB_4FXXY; orb14+=ORB_4FXXY)
                {
                    for (orb15=-ORB_4FXXZ; orb15<=ORB_4FXXZ; orb15+=ORB_4FXXZ)
                     {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb15=ORB_4FXXZ;
                }
                orb14=ORB_4FXXY;
            }
            orb13=ORB_4FZZZ;
        }
        orb12=ORB_4FYYY;
    }
    orb11=ORB_4FXXX;

    for (orb16=-ORB_4FYYZ; orb16<=ORB_4FYYZ; orb16+=ORB_4FYYZ)
    {
        for (orb17=-ORB_4FXYY; orb17<=ORB_4FXYY; orb17+=ORB_4FXYY)
        {
            for (orb18=-ORB_4FXZZ; orb18<=ORB_4FXZZ; orb18+=ORB_4FXZZ)
            {
                for (orb19=-ORB_4FYZZ; orb19<=ORB_4FYZZ; orb19+=ORB_4FYZZ)
                {
                    for (orb20=-ORB_4FXYZ; orb20<=ORB_4FXYZ; orb20+=ORB_4FXYZ)
                     {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  nameLength,
                                                                  lameInt1,
                                                                  lameInt2,
                                                                  lameExt1,
                                                                  lameExt2,
                                                                  xMin,yMin,
                                                                  zMin,xMax,
                                                                  yMax,zMax,nX,
                                                                  nY,nZ,deltaX,
                                                                  deltaY,deltaZ,
                                                                  lsType,lsX,
                                                                  lsY,lsZ,lsR,
                                                                  metCst,metErr,
                                                                  metMin,metMax,
                                                                  trickMatrix,
                                                                  approxMode,
                                                                  iterMax,
                                                                  iterTolD0P,
                                                                  iterTolD1P,
                                                                  iterTolD2P,
                                                                  saveType,
                                                                  saveMesh,
                                                                  saveData,
                                                                  savePrint,
                                                                  saveWhere,
                                                                  pathLength,
                                                                  pPathMedit,
                                                                  pPathMmg3d,
                                                                  pPathMshdist,
                                                                  pPathElastic,
                                                                  pPathAdvect,
                                                                  hminIso,
                                                                  hmaxIso,
                                                                  hausdIso,
                                                                  hgradIso,
                                                                  hminMet,
                                                                  hmaxMet,
                                                                  hausdMet,
                                                                  hgradMet,
                                                                  hminLs,hmaxLs,
                                                                  hausdLs,
                                                                  hgradLs,
                                                                  hmodeLag,
                                                                  hminLag,
                                                                  hmaxLag,
                                                                  hausdLag,
                                                                  hgradLag,
                                                                  nIter,
                                                                  residual,
                                                                  deltaT,
                                                                  noCfl,orb1,
                                                                  orb2,orb3,
                                                                  orb4,orb5,
                                                                  orb6,orb7,
                                                                  orb8,orb9,
                                                                  orb10,orb11,
                                                                  orb12,orb13,
                                                                  orb14,orb15,
                                                                  orb16,orb17,
                                                                  orb18,orb19,
                                                                  orb20,cstA,
                                                                  cstB,cstC,
                                                                  csta,cstb,
                                                                  cstc,cstaa,
                                                                   cstbb,cstcc);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb20=ORB_4FXYZ;
                }
                orb19=ORB_4FYZZ;
            }
            orb18=ORB_4FXZZ;
        }
        orb17=ORB_4FXYY;
    }
    orb16=ORB_4FYYZ;

    for (cstA=CST_A-1.e-18; cstA<=CST_A+1.e-18; cstA+=1.e-18)
    {
        for (cstB=CST_B-1.e-17; cstB<=CST_B+1.e-17; cstB+=1.e-17)
        {
            for (cstC=CST_C-1.e-18; cstC<=CST_C+1.e-18; cstC+=1.e-18)
            {
                PRINT_TEST_START(counter,expectedValue);
                returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                          nameLength,lameInt1,
                                                          lameInt2,lameExt1,
                                                          lameExt2,xMin,yMin,
                                                          zMin,xMax,yMax,zMax,
                                                          nX,nY,nZ,deltaX,
                                                          deltaY,deltaZ,lsType,
                                                          lsX,lsY,lsZ,lsR,
                                                          metCst,metErr,metMin,
                                                          metMax,trickMatrix,
                                                          approxMode,iterMax,
                                                          iterTolD0P,iterTolD1P,
                                                          iterTolD2P,saveType,
                                                          saveMesh,saveData,
                                                          savePrint,saveWhere,
                                                          pathLength,pPathMedit,
                                                          pPathMmg3d,
                                                          pPathMshdist,
                                                          pPathElastic,
                                                          pathAdvect,hminIso,
                                                          hmaxIso,hausdIso,
                                                          hgradIso,hminMet,
                                                          hmaxMet,hausdMet,
                                                          hgradMet,hminLs,
                                                          hmaxLs,hausdLs,
                                                          hgradLs,hmodeLag,
                                                          hminLag,hmaxLag,
                                                          hausdLag,hgradLag,
                                                          nIter,residual,deltaT,
                                                          noCfl,orb1,orb2,orb3,
                                                          orb4,orb5,orb6,orb7,
                                                          orb8,orb9,orb10,orb11,
                                                          orb12,orb13,orb14,
                                                          orb15,orb16,orb17,
                                                          orb18,orb19,orb20,
                                                          cstA,cstB,cstC,csta,
                                                          cstb,cstc,cstaa,cstbb,
                                                                         cstcc);
                PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            }
            cstC=CST_C;
        }
        cstB=CST_B;
    }
    cstA=CST_A;

    for (csta=CST_a-1.e-16; csta<=CST_a+1.e-16; csta+=1.e-16)
    {
        for (cstb=CST_b-1.e-16; cstb<=CST_b+1.e-16; cstb+=1.e-16)
        {
            for (cstc=CST_c-1.e-17; cstc<=CST_c+1.e-17; cstc+=1.e-17)
            {
                PRINT_TEST_START(counter,expectedValue);
                returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                          nameLength,lameInt1,
                                                          lameInt2,lameExt1,
                                                          lameExt2,xMin,yMin,
                                                          zMin,xMax,yMax,zMax,
                                                          nX,nY,nZ,deltaX,
                                                          deltaY,deltaZ,lsType,
                                                          lsX,lsY,lsZ,lsR,
                                                          metCst,metErr,metMin,
                                                          metMax,trickMatrix,
                                                          approxMode,iterMax,
                                                          iterTolD0P,iterTolD1P,
                                                          iterTolD2P,saveType,
                                                          saveMesh,saveData,
                                                          savePrint,saveWhere,
                                                          pathLength,pPathMedit,
                                                          pPathMmg3d,
                                                          pPathMshdist,
                                                          pPathElastic,
                                                          pathAdvect,hminIso,
                                                          hmaxIso,hausdIso,
                                                          hgradIso,hminMet,
                                                          hmaxMet,hausdMet,
                                                          hgradMet,hminLs,
                                                          hmaxLs,hausdLs,
                                                          hgradLs,hmodeLag,
                                                          hminLag,hmaxLag,
                                                          hausdLag,hgradLag,
                                                          nIter,residual,deltaT,
                                                          noCfl,orb1,orb2,orb3,
                                                          orb4,orb5,orb6,orb7,
                                                          orb8,orb9,orb10,orb11,
                                                          orb12,orb13,orb14,
                                                          orb15,orb16,orb17,
                                                          orb18,orb19,orb20,
                                                          cstA,cstB,cstC,csta,
                                                          cstb,cstc,cstaa,cstbb,
                                                                         cstcc);
                PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            }
            cstc=CST_c;
        }
        cstb=CST_b;
    }
    csta=CST_a;

    for (cstaa=CST_aa-1.e-17; cstaa<=CST_aa+1.e-17; cstaa+=1.e-17)
    {
         for (cstbb=CST_bb-1.e-17; cstbb<=CST_bb+1.e-17; cstbb+=1.e-17)
         {
             for (cstcc=CST_cc-1.e-16; cstcc<=CST_cc+1.e-16; cstcc+=1.e-16)
             {
                 expectedValue=0;
                 if (cstaa==CST_aa && cstbb==CST_bb && cstcc==CST_cc)
                 {
                     expectedValue=1;
                 }
                PRINT_TEST_START(counter,expectedValue);
                returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                          nameLength,lameInt1,
                                                          lameInt2,lameExt1,
                                                          lameExt2,xMin,yMin,
                                                          zMin,xMax,yMax,zMax,
                                                          nX,nY,nZ,deltaX,
                                                          deltaY,deltaZ,lsType,
                                                          lsX,lsY,lsZ,lsR,
                                                          metCst,metErr,metMin,
                                                          metMax,trickMatrix,
                                                          approxMode,iterMax,
                                                          iterTolD0P,iterTolD1P,
                                                          iterTolD2P,saveType,
                                                          saveMesh,saveData,
                                                          savePrint,saveWhere,
                                                          pathLength,pPathMedit,
                                                          pPathMmg3d,
                                                          pPathMshdist,
                                                          pPathElastic,
                                                          pathAdvect,hminIso,
                                                          hmaxIso,hausdIso,
                                                          hgradIso,hminMet,
                                                          hmaxMet,hausdMet,
                                                          hgradMet,hminLs,
                                                          hmaxLs,hausdLs,
                                                          hgradLs,hmodeLag,
                                                          hminLag,hmaxLag,
                                                          hausdLag,hgradLag,
                                                          nIter,residual,deltaT,
                                                          noCfl,orb1,orb2,orb3,
                                                          orb4,orb5,orb6,orb7,
                                                          orb8,orb9,orb10,orb11,
                                                          orb12,orb13,orb14,
                                                          orb15,orb16,orb17,
                                                          orb18,orb19,orb20,
                                                          cstA,cstB,cstC,csta,
                                                          cstb,cstc,cstaa,cstbb,
                                                                         cstcc);
                PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
                }
                cstcc=CST_cc;
        }
        cstbb=CST_bb;
    }
    cstaa=CST_aa;

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,nameLength,
                                              lameInt1,lameInt2,lameExt1,
                                              lameExt2,xMin,yMin,zMin,xMax,yMax,
                                              zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterMax,
                                              iterTolD0P,iterTolD1P,iterTolD2P,
                                              saveType,saveMesh,saveData,
                                              savePrint,saveWhere,pathLength,
                                              pPathMedit,pPathMmg3d,
                                              pPathMshdist,pPathElastic,
                                              pPathAdvect,hminIso,hmaxIso,
                                              hausdIso,hgradIso,hminMet,hmaxMet,
                                              hausdMet,hgradMet,hminLs,hmaxLs,
                                              hausdLs,hgradLs,hmodeLag,hminLag,
                                              hmaxLag,hausdLag,hgradLag,nIter,
                                              residual,deltaT,noCfl,orb1,orb2,
                                              orb3,orb4,orb5,orb6,orb7,orb8,
                                              orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                                   cstb,cstc,cstaa,cstbb,cstcc);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    PRINT_TEST_START(counter,expectedValue);
    returnValue=checkAllPreprocessorConstants(OPT_MODE,VERBOSE,N_CPU,
                                              NAME_LENGTH,LAME_INT1,LAME_INT2,
                                              LAME_EXT1,LAME_EXT2,X_MIN,Y_MIN,
                                              Z_MIN,X_MAX,Y_MAX,Z_MAX,N_X,N_Y,
                                              N_Z,DELTA_X,DELTA_Y,DELTA_Z,
                                              LS_TYPE,LS_X,LS_Y,LS_Z,LS_R,
                                              MET_CST,MET_ERR,MET_MIN,MET_MAX,
                                              TRICK_MATRIX,APPROX_MODE,ITER_MAX,
                                              ITER_TOLD0P,ITER_TOLD1P,
                                              ITER_TOLD2P,SAVE_TYPE,SAVE_MESH,
                                              SAVE_DATA,SAVE_PRINT,SAVE_WHERE,
                                              PATH_LENGTH,PATH_MEDIT,PATH_MMG3D,
                                              PATH_MSHDIST,PATH_ELASTIC,
                                              PATH_ADVECT,HMIN_ISO,HMAX_ISO,
                                              HAUSD_ISO,HGRAD_ISO,HMIN_MET,
                                              HMAX_MET,HAUSD_MET,HGRAD_MET,
                                              HMIN_LS,HMAX_LS,HAUSD_LS,HGRAD_LS,
                                              HMODE_LAG,HMIN_LAG,HMAX_LAG,
                                              HAUSD_LAG,HGRAD_LAG,N_ITER,
                                              RESIDUAL,DELTA_T,NO_CFL,ORB_1S,
                                              ORB_2PX,ORB_2PY,ORB_2PZ,ORB_3DXX,
                                              ORB_3DYY,ORB_3DZZ,ORB_3DXY,
                                              ORB_3DXZ,ORB_3DYZ,ORB_4FXXX,
                                              ORB_4FYYY,ORB_4FZZZ,ORB_4FXXY,
                                              ORB_4FXXZ,ORB_4FYYZ,ORB_4FXYY,
                                              ORB_4FXZZ,ORB_4FYZZ,ORB_4FXYZ,
                                              CST_A,CST_B,CST_C,CST_a,CST_b,
                                                    CST_c,CST_aa,CST_bb,CST_cc);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

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

// Unit tests on initialFileExists of main.c file
void testInitialFileExists(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char *fileLocation=NULL, nameTestShort[9]="test.txt";
    char nameTestLong[18]="test.txtblablabla";
    int nameLength=0;
    FILE* testFile=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting initialFileExists function.\n");

    for (nameLength=-9; nameLength<=18; nameLength+=9)
    {
        expectedValue=-1;
        fileLocation=NULL;
        PRINT_TEST_START(counter,expectedValue);
        fprintf(stdout,"fileLocation=%s\n",fileLocation);
        fprintf(stdout,"nameLength=%d\n",nameLength);
        returnValue=initialFileExists(fileLocation,nameLength);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);

        system("touch test.txt");
        expectedValue=1;
        if (nameLength<9)
        {
            expectedValue=0;
        }
        fileLocation=nameTestShort;
        PRINT_TEST_START(counter,expectedValue);
        fprintf(stdout,"fileLocation=%s\n",fileLocation);
        fprintf(stdout,"nameLength=%d\n",nameLength);
        returnValue=initialFileExists(fileLocation,nameLength);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);

        expectedValue=0;
        if (nameLength>17)
        {
            expectedValue=-1;
        }
        fileLocation=nameTestLong;
        PRINT_TEST_START(counter,expectedValue);
        fprintf(stdout,"fileLocation=%s\n",fileLocation);
        fprintf(stdout,"nameLength=%d\n",nameLength);
        returnValue=initialFileExists(fileLocation,nameLength);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);

        system("rm test.txt");
        expectedValue=-1;
        if (nameLength<9)
        {
            expectedValue=0;
        }
        fileLocation=nameTestShort;
        PRINT_TEST_START(counter,expectedValue);
        fprintf(stdout,"fileLocation=%s\n",fileLocation);
        fprintf(stdout,"nameLength=%d\n",nameLength);
        returnValue=initialFileExists(fileLocation,nameLength);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);

    }

    expectedValue=1;
    system("mkdir testDirectory");
    PRINT_TEST_START(counter,expectedValue);
    testFile=fopen("testDirectory/test.txt","w");
    if (testFile==NULL)
    {
        fprintf(stdout,"\nWarning in testInitialFileExists function: we ");
        fprintf(stdout,"could not write 'Hello World' in ");
        fprintf(stdout,"testDirectory/test.txt file.\n");
        returnValue=-2;
    }
    else
    {
        fprintf(testFile,"Hello World\n");
        fprintf(stdout,"fileLocation=testDirectory/test.txt\nnameLength=23\n");
        returnValue=initialFileExists("testDirectory/test.txt",23);
        fclose(testFile);
        testFile=NULL;
    }
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);
    system("rm testDirectory/test.txt");
    system("rm testDirectory -d");

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

// Unit tests on closeTheFile of main.c file
void testCloseTheFile(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    FILE **pFileToClose=NULL, *fileToClose=NULL;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting closeTheFile function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pFileToClose=%p\n",(void*)pFileToClose);
    closeTheFile(pFileToClose);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pFileToClose=&fileToClose;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pFileToClose=%p\n",(void*)pFileToClose);
    fprintf(stdout,"fileToClose=%p\n",(void*)*pFileToClose);
    closeTheFile(pFileToClose);
    returnValue=1;
    if (fileToClose!=NULL)
    {
        returnValue=0;
    }
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);
    system("touch test.txt");
    PRINT_TEST_START(counter,expectedValue);
    fileToClose=fopen("test.txt","r");
    fprintf(stdout,"pFileToClose=%p\n",(void*)pFileToClose);
    fprintf(stdout,"fileToClose=%p\n",(void*)*pFileToClose);
    if (fileToClose==NULL)
    {
        fprintf(stdout,"\nWarning in testCloseTheFile function: we ");
        fprintf(stdout,"could not open test.txt file.\n");
        returnValue=0;
    }
    else
    {
        fprintf(stdout,"\nOpening test.txt file. ");
        closeTheFile(pFileToClose);
        returnValue=1;
        if (fileToClose!=NULL)
        {
            returnValue=0;
        }
    }
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    PRINT_TEST_START(counter,expectedValue);
    fileToClose=fopen("test.txt","w");
    fprintf(stdout,"fileToClose=%p\n",(void*)*pFileToClose);
    if (fileToClose==NULL)
    {
        fprintf(stdout,"\nWarning in testCloseTheFile function: we ");
        fprintf(stdout,"could not open test.txt file.\n");
        returnValue=0;
    }
    else
    {
        fprintf(stdout,"\nOpening test.txt file. Writing Hello World! in it. ");
        fprintf(fileToClose,"Hello world!\n");
        closeTheFile(pFileToClose);
        returnValue=1;
        if (fileToClose!=NULL)
        {
            returnValue=0;
        }
    }
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    PRINT_TEST_START(counter,expectedValue);
    fileToClose=fopen("test.txt","w");
    fprintf(stdout,"fileToClose=%p\n",(void*)*pFileToClose);
    if (fileToClose==NULL)
    {
        fprintf(stdout,"\nWarning in testCloseTheFile function: we ");
        fprintf(stdout,"could not open test.txt file.\n");
        returnValue=0;
    }
    else
    {
        fprintf(stdout,"\nOpening test.txt file. Writing Hello World! in it. ");
        fprintf(fileToClose,"Hello World \n");
        closeTheFile(pFileToClose);
        closeTheFile(pFileToClose);
        returnValue=1;
        if (fileToClose!=NULL)
        {
            returnValue=0;
        }
    }
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);
    system("rm test.txt");
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"fileToClose=%p\n",(void*)*pFileToClose);
    closeTheFile(pFileToClose);
    returnValue=1;
    if (fileToClose!=NULL)
    {
        returnValue=0;
    }
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    system("touch test.txt");
    PRINT_TEST_START(counter,expectedValue);
    fileToClose=fopen("test.txt","r");
    fprintf(stdout,"fileToClose=%p\n",(void*)*pFileToClose);
    if (fileToClose==NULL)
    {
        fprintf(stdout,"\nWarning in testCloseTheFile function: we ");
        fprintf(stdout,"could not open test.txt file.\n");
        returnValue=0;
    }
    else
    {
        fprintf(stdout,"\nOpening test.txt file. ");
        closeTheFile(pFileToClose);
        returnValue=1;
        if (fileToClose!=NULL)
        {
            returnValue=0;
        }
    }
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);
    system("rm test.txt");

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

