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

#include "adaptMesh.h"
#include "computeData.h"
//#include "optimization.h"

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
//    testCommentAnormalEnd();
//    testPrintTimer();
//    testEndTimerAtExit();
//    testEndTimerAtError();

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
//    testCheckInputFileName();
//    testCheckForTildeAndReplaceByHomePath();
//    testSetupDefaultParameters();

//    testInitialFileExists();
//    testCloseTheFile();
//    testGetLengthAfterKeywordBeginning();
//    testGetTypeAfterKeyword();
//    testGetLengthAfterKeywordMiddle();
//    testDetectRepetition();
//    testChangeValuesOfParameters();
//    testReadInfoFileAndGetParameters();

//    testCopyFileLocation();
//    testWritingDefaultElasticFile();
//    testCheckValuesOfAllParameters();
//    testLoadParameters();

    return;
}

////////////////////////////////////////////////////////////////////////////////
// Unit (random) tests on commentAnormalEnd of main.c file
////////////////////////////////////////////////////////////////////////////////
void testCommentAnormalEnd(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    int typeOfSignal=0, iRandom=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting commentAnormalEnd function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    commentAnormalEnd(typeOfSignal);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        typeOfSignal=rand()%2001-1000;
        if (typeOfSignal!=SIGINT)
        {
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"typeOfSignal=%d\n",typeOfSignal);
            commentAnormalEnd(typeOfSignal);
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
        }
    }

    for (typeOfSignal=-20; typeOfSignal<=20; typeOfSignal++)
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
// Unit (random) tests on printTimer of main.c file
////////////////////////////////////////////////////////////////////////////////
void testPrintTimer(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    int i=0, iRandom=0;
    time_t finalTimer=0, initialTimer=0;

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
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"Function printTimer returns: ");
    printTimer(finalTimer,initialTimer);
    fprintf(stdout,".\n");
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        initialTimer=rand()%30001-10000;
        finalTimer=rand()%30001-10000;
        PRINT_TEST_START(counter,expectedValue);
        fprintf(stdout,"Function printTimer returns: ");
        printTimer(finalTimer,initialTimer);
        fprintf(stdout,".\n");
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
    }

    for (i=0; i<53; i++)
    {
        PRINT_TEST_START(counter,expectedValue);
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
// Unit (random) tests on endTimerAtExit of main.c file
////////////////////////////////////////////////////////////////////////////////
void testEndTimerAtExit(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    int iRandom=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting endTimerAtExit function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    endTimerAtExit();
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        PRINT_TEST_START(counter,expectedValue);
        endTimerAtExit();
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
// Unit tests on endTimerAtError of main.c file
////////////////////////////////////////////////////////////////////////////////
void testEndTimerAtError(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0; 

    int iRandom=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting endTimerAtError function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"Function endTimerAtError returns: %s",endTimerAtError());
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        PRINT_TEST_START(counter,expectedValue);
        fprintf(stdout,"Function endTimerAtError returns: ");
        fprintf(stdout,"%s",endTimerAtError());
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
// Unit (random) tests on checkStringFromLength of main.c file
////////////////////////////////////////////////////////////////////////////////
void testCheckStringFromLength(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char stringToCheck[16]={'\0'}, *pStringToCheck=NULL;
    int iTest=0, i=0, j=0, k=0, l=0, minimumLength=0, maximumLength=0;
    char pStringToCheckBefore[6]={'a','b','1','1','d','1'};
    char pStringToCheckAfter[6]={'A','1','C','1','\0','\0'};
    int *pIntMaxOrMin[2]={&minimumLength,&maximumLength};

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
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pStringToCheck=stringToCheck;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"stringToCheck=%s\n",pStringToCheck);
    fprintf(stdout,"minimumLength=%d\n",minimumLength);
    fprintf(stdout,"maximumLength=%d\n",maximumLength);
    returnValue=checkStringFromLength(pStringToCheck,minimumLength,
                                                                 maximumLength);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (maximumLength=-10; maximumLength<11; maximumLength++)
    {
        for (minimumLength=-10; minimumLength<11; minimumLength++)
        {
            for (i=0; i<2; i++)
            {
                for (j=-10; j<11; j++)
                {
                    for (k=0; k<6; k++)
                    {
                        for (iTest=0; iTest<2; iTest++)
                        {
                            for (l=0; l<15; l++)
                            {
                                stringToCheck[l]=rand()%90+33;
                            }

                            if (iTest)
                            {
                                for (l=0; l<15; l++)
                                {
                                    if (l<*pIntMaxOrMin[i]+j)
                                    {
                                        if (pStringToCheckBefore[k]!='1')
                                        {
                                            stringToCheck[l]=
                                                        pStringToCheckBefore[k];
                                        }
                                    }
                                    else if (l==*pIntMaxOrMin[i]+j)
                                    {
                                        stringToCheck[l]='\0';
                                    }
                                    else
                                    {
                                        if (pStringToCheckAfter[k]!='1')
                                        {
                                            stringToCheck[l]=
                                                         pStringToCheckAfter[k];
                                        }
                                    }
                                }
                            }
                            else
                            {
                                stringToCheck[rand()%15]='\0';
                            }

                            expectedValue=strlen(stringToCheck)+1;
                            if (minimumLength<1 || maximumLength<minimumLength
                                               || expectedValue>maximumLength ||
                                                    expectedValue<minimumLength)
                            {
                                expectedValue=0;
                            }

                            PRINT_TEST_START(counter,expectedValue);
                            fprintf(stdout,"stringToCheck=%s\n",pStringToCheck);
                            fprintf(stdout,"minimumLength=%d\n",minimumLength);
                            fprintf(stdout,"maximumLength=%d\n",maximumLength);
                            returnValue=checkStringFromLength(pStringToCheck,
                                                              minimumLength,
                                                                 maximumLength);
                            PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                        }
                    }
                }
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

////////////////////////////////////////////////////////////////////////////////
// Unit tests on checkAllPreprocessorConstants of main.c file
////////////////////////////////////////////////////////////////////////////////
void testCheckAllPreprocessorConstants(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char pStringToCheckBefore[6]={'a','b','1','1','d','1'};
    char pStringToCheckAfter[6]={'A','1','C','1','\0','\0'};
    char pathTest[PATH_LENGTH+11]={'\0'};
    char *pPathTest[5]={NULL,NULL,NULL,NULL,NULL};
    char pPathToPrint[5][12]={"pathMedit","pathMmg3d","pathMshdist",
                                                    "pathElastic","pathAdvect"};
    char pPathRef[5][21]={PATH_MEDIT,PATH_MMG3D,PATH_MSHDIST,PATH_ELASTIC,
                                                                   PATH_ADVECT};

    int i=0, j=0, k=0, iTest=0, iPathMedit=0, iPathMmg3d=0, iPathMshdist=0;
    int iPathElastic=0, iPathAdvect=0, optMode=0, verbose=0, nCpu=0;
    int nameLength=0, bohrUnit=0, orbOrtho=0, nX=0, nY=0, nZ=0, lsType=0;
    int trickMatrix=0, approxMode=0, iterIni=0, iterMax=0, saveType=0;
    int saveMesh=0, saveData=0, savePrint=0, saveWhere=0, pathLength=0;
    int hmodeLag=0, nIter=0, noCfl=0, orb1=0, orb2=0, orb3=0, orb4=0, orb5=0;
    int orb6=0, orb7=0, orb8=0, orb9=0, orb10=0, orb11=0, orb12=0, orb13=0;
    int orb14=0, orb15=0, orb16=0, orb17=0, orb18=0, orb19=0, orb20=0;

    double rhoOpt=0., lameInt1=0., lameInt2=0., lameExt1=0., lameExt2=0.;
    double bohrRadius=0., selectOrb=0., selectBox=0., xMin=0., yMin=0., zMin=0.;
    double xMax=0., yMax=0., zMax=0., deltaX=0., deltaY=0., deltaZ=0., lsX=0.;
    double lsY=0., lsZ=0., lsR=0., metCst=0., metErr=0., metMin=0., metMax=0.;
    double iterTolD0P=0., iterTolD1P=0., iterTolD2P=0., hminIso=0., hmaxIso=0.;
    double hausdIso=0., hgradIso=0., hminMet=0., hmaxMet=0., hausdMet=0.;
    double hgradMet=0., hminLs=0., hmaxLs=0., hausdLs=0., hgradLs=0.;
    double hminLag=0., hmaxLag=0., hausdLag=0., hgradLag=0., residual=0.;
    double deltaT=0., cstA=0., cstB=0., cstC=0., csta=0., cstb=0., cstc=0.;
    double cstaa=0., cstbb=0., cstcc=0., cstOne=0., cstTwo=0., cstThree=0.;
    double cst1=0., cst2=0., cst3=0., cst22=0., cst33=0.;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting checkAllPreprocessorConstants function.\n");
    pathTest[PATH_LENGTH+10]='\0';
    for (i=0; i<5; i++)
    {
        pPathRef[i][20]='\0';
    }

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (optMode=-3; optMode<6; optMode++)
    {
        for (verbose=-1; verbose<4; verbose++)
        {
            for (nCpu=-2; nCpu<7; nCpu+=2)
            {
                for (rhoOpt=-1.; rhoOpt<=1.; rhoOpt+=.5)
                {
                    for (nameLength=-50; nameLength<=100; nameLength+=50)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    nameLength=NAME_LENGTH;
                }
                rhoOpt=RHO_OPT;
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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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

    for (bohrUnit=-1; bohrUnit<3; bohrUnit++)
    {
        for (bohrRadius=-BOHR_RADIUS; bohrRadius<=2*BOHR_RADIUS;
                                                        bohrRadius+=BOHR_RADIUS)
        {
            for (selectOrb=-.01; selectOrb<=.02; selectOrb+=.01)
            {
                for (orbOrtho=-1; orbOrtho<=3; orbOrtho++)
                {
                    for (selectBox=-.1; selectBox<=1.1; selectBox+=.05)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    selectBox=SELECT_BOX;
                }
                orbOrtho=ORB_ORTHO;
            }
            selectOrb=SELECT_ORB;
        }
        bohrRadius=BOHR_RADIUS;
    }
    bohrUnit=BOHR_UNIT;

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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                                  verbose,nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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

    for (metCst=-MET_CST; metCst<=2*MET_CST; metCst+=MET_CST)
    {
        for (metErr=-.2; metErr<=.4; metErr+=.2)
        {
            for (metMin=.4; metMin>=-.2; metMin-=.2)
            {
                for (metMax=-.2; metMax<=.4; metMax+=.2)
                {
                    for (trickMatrix=-1; trickMatrix<3; trickMatrix++)
                    {
                        for (approxMode=-1; approxMode<3; approxMode++)
                        {
                            PRINT_TEST_START(counter,expectedValue);
                            returnValue=checkAllPreprocessorConstants(optMode,
                                                                      verbose,
                                                                      nCpu,
                                                                      rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                            PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                        }
                        approxMode=APPROX_MODE;
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

    for (iterIni=10; iterIni>=-10; iterIni-=5)
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
                                                                  verbose,nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
    iterIni=ITER_INI;

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
                                                                  verbose,nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
    for (i=0; i<5; i++)
    {
        pPathRef[i][20]='\0';
    }
    for (pathLength=-PATH_LENGTH+2; pathLength<=PATH_LENGTH+2;
                                                        pathLength+=PATH_LENGTH)
    {
        for (i=-5; i<6; i++)
        {
            for (j=0; j<6; j++)
            {
                for (iTest=0; iTest<2; iTest++)
                {
                    for (k=0; k<PATH_LENGTH+10; k++)
                    {
                        pathTest[k]=rand()%90+33;
                    }

                    if (iTest)
                    {
                        for (k=0; k<PATH_LENGTH+10; k++)
                        {
                            if (k<pathLength+i)
                            {
                                if (pStringToCheckBefore[j]!='1')
                                {
                                    pathTest[k]=pStringToCheckBefore[j];
                                }
                            }
                            else if (k==pathLength+i)
                            {
                                pathTest[k]='\0';
                            }
                            else
                            {
                                if (pStringToCheckAfter[j]!='1')
                                {
                                    pathTest[k]=pStringToCheckAfter[j];
                                }
                            }
                        }
                    }
                    else
                    {
                        pathTest[rand()%(PATH_LENGTH+10)]='\0';
                    }

                    for (k=0; k<5; k++)
                    {
                        pPathTest[k]=NULL;
                    }

                    for (k=0; k<5; k++)
                    {
                        pPathTest[k]=pathTest;
                        PRINT_TEST_START(counter,expectedValue);
                        fprintf(stdout,"pathLength=%d\n",pathLength);
                        fprintf(stdout,"%s=%s\n",pPathToPrint[k],pPathTest[k]);
                        fprintf(stdout,"(%ld characters)\n",
                                                          strlen(pPathTest[k]));
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                        pPathTest[k]=pPathRef[k];
                    }
                }
            }
        }
    }

    for (pathLength=-50; pathLength<=100; pathLength+=50)
    {
        for (iPathMedit=-10; iPathMedit<=10; iPathMedit+=10)
        {
            pPathTest[0]=NULL;
            if (iPathMedit>0)
            {
                pPathTest[0]=pPathRef[0];
            }
            for (iPathMmg3d=-10; iPathMmg3d<=10; iPathMmg3d+=10)
            {
                pPathTest[1]=NULL;
                if (iPathMmg3d>0)
                {
                    pPathTest[1]=pPathRef[1];
                }
                for (iPathMshdist=-10; iPathMshdist<=10; iPathMshdist+=10)
                {
                    pPathTest[2]=NULL;
                    if (iPathMshdist>0)
                    {
                         pPathTest[2]=pPathRef[2];
                    }
                    for (iPathElastic=-10; iPathElastic<=10; iPathElastic+=10)
                    {
                        pPathTest[3]=NULL;
                        if (iPathElastic>0)
                        {
                            pPathTest[3]=pPathRef[3];
                        }
                        for (iPathAdvect=-10; iPathAdvect<=10; iPathAdvect+=10)
                        {
                            pPathTest[4]=NULL;
                            if (iPathAdvect>0)
                            {
                                pPathTest[4]=pPathRef[4];
                            }
                            PRINT_TEST_START(counter,expectedValue);
                            returnValue=checkAllPreprocessorConstants(optMode,
                                                                      verbose,
                                                                      nCpu,
                                                                      rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                              PRINT_TEST_END(counter,counterSuccess,counterFail,
                                                   returnValue,expectedValue,
                                                                      readChar);
                        }
                        pPathTest[4]=pPathRef[4];
                    }
                    pPathTest[3]=pPathRef[3];
                }
                pPathTest[2]=pPathRef[2];
            }
            pPathTest[1]=pPathRef[1];
        }
        pPathTest[0]=pPathRef[0];
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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                              nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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

    for (orb1=-ORB_S; orb1<=ORB_S; orb1+=ORB_S)
    {
        for (orb2=-ORB_PX; orb2<=ORB_PX; orb2+=ORB_PX)
        {
            for (orb3=-ORB_PY; orb3<=ORB_PY; orb3+=ORB_PY)
            {
                for (orb4=-ORB_PZ; orb4<=ORB_PZ; orb4+=ORB_PZ)
                {
                     for (orb5=-ORB_DXX; orb5<=ORB_DXX; orb5+=ORB_DXX)
                     {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb5=ORB_DXX;
                }
                orb4=ORB_PZ;
            }
            orb3=ORB_PY;
        }
        orb2=ORB_PX;
    }
    orb1=ORB_S;

    for (orb6=-ORB_DYY; orb6<=ORB_DYY; orb6+=ORB_DYY)
    {
        for (orb7=-ORB_DZZ; orb7<=ORB_DZZ; orb7+=ORB_DZZ)
        {
            for (orb8=-ORB_DXY; orb8<=ORB_DXY; orb8+=ORB_DXY)
            {
                for (orb9=-ORB_DXZ; orb9<=ORB_DXZ; orb9+=ORB_DXZ)
                {
                    for (orb10=-ORB_DYZ; orb10<=ORB_DYZ; orb10+=ORB_DYZ)
                    {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb10=ORB_DYZ;
                }
                orb9=ORB_DXZ;
            }
            orb8=ORB_DXY;
        }
        orb7=ORB_DZZ;
    }
    orb6=ORB_DYY;

    for (orb11=-ORB_FXXX; orb11<=ORB_FXXX; orb11+=ORB_FXXX)
    {
        for (orb12=-ORB_FYYY; orb12<=ORB_FYYY; orb12+=ORB_FYYY)
        {
            for (orb13=-ORB_FZZZ; orb13<=ORB_FZZZ; orb13+=ORB_FZZZ)
            {
                for (orb14=-ORB_FXXY; orb14<=ORB_FXXY; orb14+=ORB_FXXY)
                {
                    for (orb15=-ORB_FXXZ; orb15<=ORB_FXXZ; orb15+=ORB_FXXZ)
                     {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb15=ORB_FXXZ;
                }
                orb14=ORB_FXXY;
            }
            orb13=ORB_FZZZ;
        }
        orb12=ORB_FYYY;
    }
    orb11=ORB_FXXX;

    for (orb16=-ORB_FYYZ; orb16<=ORB_FYYZ; orb16+=ORB_FYYZ)
    {
        for (orb17=-ORB_FXYY; orb17<=ORB_FXYY; orb17+=ORB_FXYY)
        {
            for (orb18=-ORB_FXZZ; orb18<=ORB_FXZZ; orb18+=ORB_FXZZ)
            {
                for (orb19=-ORB_FYZZ; orb19<=ORB_FYZZ; orb19+=ORB_FYZZ)
                {
                    for (orb20=-ORB_FXYZ; orb20<=ORB_FXYZ; orb20+=ORB_FXYZ)
                     {
                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    orb20=ORB_FXYZ;
                }
                orb19=ORB_FYZZ;
            }
            orb18=ORB_FXZZ;
        }
        orb17=ORB_FXYY;
    }
    orb16=ORB_FYYZ;

    for (cstA=CST_A-1.e-18; cstA<=CST_A+1.e-18; cstA+=1.e-18)
    {
        for (cstB=CST_B-1.e-17; cstB<=CST_B+1.e-17; cstB+=1.e-17)
        {
            for (cstC=CST_C-1.e-18; cstC<=CST_C+1.e-18; cstC+=1.e-18)
            {
                PRINT_TEST_START(counter,expectedValue);
                returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                          rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                                                          rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
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
                PRINT_TEST_START(counter,expectedValue);
                returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                           rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
            }
            cstcc=CST_cc;
        }
        cstbb=CST_bb;
    }
    cstaa=CST_aa;

    for (cstOne=CST_ONE-1.e-16; cstOne<=CST_ONE+1.e-16; cstOne+=1.e-16)
    {
        for (cstTwo=CST_TWO-1.e-16; cstTwo<=CST_TWO+1.e-16; cstTwo+=1.e-16)
        {
            for (cstThree=CST_THREE-1.e-16; cstThree<=CST_THREE+1.e-16;
                                                               cstThree+=1.e-16)
            {
                PRINT_TEST_START(counter,expectedValue);
                returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,
                                                           rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
            }
            cstThree=CST_THREE;
        }
        cstTwo=CST_TWO;
    }
    cstOne=CST_ONE;

    for (cst1=CST_1-1.e-16; cst1<=CST_1+1.e-16; cst1+=1.e-16)
    {
        for (cst2=CST_2-1.e-17; cst2<=CST_2+1.e-17; cst2+=1.e-17)
        {
            for (cst3=CST_3-1.e-16; cst3<=CST_3+1.e-16; cst3+=1.e-16)
            {
                for (cst22=CST_22-1.e-16; cst22<=CST_22+1.e-16; cst22+=1.e-16)
                {
                    for (cst33=CST_33-1.e-17; cst33<=CST_33+1.e-17;
                                                                  cst33+=1.e-17)
                    {
                        expectedValue=0;
                        if (cst1==CST_1 && cst2==CST_2 && cst3==CST_3 &&
                                                 cst22==CST_22 && cst33==CST_33)
                        {
                            expectedValue=1;
                        }

                        PRINT_TEST_START(counter,expectedValue);
                        returnValue=checkAllPreprocessorConstants(optMode,
                                                                  verbose,
                                                                  nCpu,
                                                                  rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
                        PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                    }
                    cst33=CST_33;
                }
                cst22=CST_22;
            }
            cst3=CST_3;
        }
        cst2=CST_2;
    }
    cst1=CST_1;

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    verbose=2;
    returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    verbose=1;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    verbose=0;
    PRINT_TEST_START(counter,expectedValue);
    returnValue=checkAllPreprocessorConstants(optMode,verbose,nCpu,rhoOpt,
                                              nameLength,lameInt1,lameInt2,
                                              lameExt1,lameExt2,bohrUnit,
                                              bohrRadius,selectOrb,orbOrtho,
                                              selectBox,xMin,yMin,zMin,xMax,
                                              yMax,zMax,nX,nY,nZ,deltaX,deltaY,
                                              deltaZ,lsType,lsX,lsY,lsZ,lsR,
                                              metCst,metErr,metMin,metMax,
                                              trickMatrix,approxMode,iterIni,
                                              iterMax,iterTolD0P,iterTolD1P,
                                              iterTolD2P,saveType,saveMesh,
                                              saveData,savePrint,saveWhere,
                                              pathLength,pPathTest[0],
                                              pPathTest[1],pPathTest[2],
                                              pPathTest[3],pPathTest[4],hminIso,
                                              hmaxIso,hausdIso,hgradIso,hminMet,
                                              hmaxMet,hausdMet,hgradMet,hminLs,
                                              hmaxLs,hausdLs,hgradLs,hmodeLag,
                                              hminLag,hmaxLag,hausdLag,hgradLag,
                                              nIter,residual,deltaT,noCfl,orb1,
                                              orb2,orb3,orb4,orb5,orb6,orb7,
                                              orb8,orb9,orb10,orb11,orb12,orb13,
                                              orb14,orb15,orb16,orb17,orb18,
                                              orb19,orb20,cstA,cstB,cstC,csta,
                                              cstb,cstc,cstaa,cstbb,cstcc,
                                              cstOne,cstTwo,cstThree,cst1,cst2,
                                                              cst3,cst22,cst33);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    PRINT_TEST_START(counter,expectedValue);
    returnValue=checkAllPreprocessorConstants(OPT_MODE,VERBOSE,N_CPU,RHO_OPT,
                                       NAME_LENGTH,LAME_INT1,LAME_INT2,
                                       LAME_EXT1,LAME_EXT2,BOHR_UNIT,
                                       BOHR_RADIUS,SELECT_ORB,ORB_ORTHO,
                                       SELECT_BOX,X_MIN,Y_MIN,Z_MIN,X_MAX,Y_MAX,
                                       Z_MAX,N_X,N_Y,N_Z,DELTA_X,DELTA_Y,
                                       DELTA_Z,LS_TYPE,LS_X,LS_Y,LS_Z,LS_R,
                                       MET_CST,MET_ERR,MET_MIN,MET_MAX,
                                       TRICK_MATRIX,APPROX_MODE,ITER_INI,
                                       ITER_MAX,ITER_TOLD0P,ITER_TOLD1P,
                                       ITER_TOLD2P,SAVE_TYPE,SAVE_MESH,
                                       SAVE_DATA,SAVE_PRINT,SAVE_WHERE,
                                       PATH_LENGTH,PATH_MEDIT,PATH_MMG3D,
                                       PATH_MSHDIST,PATH_ELASTIC,PATH_ADVECT,
                                       HMIN_ISO,HMAX_ISO,HAUSD_ISO,HGRAD_ISO,
                                       HMIN_MET,HMAX_MET,HAUSD_MET,HGRAD_MET,
                                       HMIN_LS,HMAX_LS,HAUSD_LS,HGRAD_LS,
                                       HMODE_LAG,HMIN_LAG,HMAX_LAG,HAUSD_LAG,
                                       HGRAD_LAG,N_ITER,RESIDUAL,DELTA_T,NO_CFL,
                                       ORB_S,ORB_PX,ORB_PY,ORB_PZ,ORB_DXX,
                                       ORB_DYY,ORB_DZZ,ORB_DXY,ORB_DXZ,ORB_DYZ,
                                       ORB_FXXX,ORB_FYYY,ORB_FZZZ,ORB_FXXY,
                                       ORB_FXXZ,ORB_FYYZ,ORB_FXYY,ORB_FXZZ,
                                       ORB_FYZZ,ORB_FXYZ,CST_A,CST_B,CST_C,
                                       CST_a,CST_b,CST_c,CST_aa,CST_bb,CST_cc,
                                       CST_ONE,CST_TWO,CST_THREE,CST_1,CST_2,
                                                           CST_3,CST_22,CST_33);
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
// Unit (random) tests on checkInputFileName of main.c file
////////////////////////////////////////////////////////////////////////////////
void testCheckInputFileName(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char fileName[21]={'\0'}, *inputFileName=NULL;
    int iTest=0, i=0, j=0, k=0, l=0, inputOrNot=0, minimumLength=8;
    int maximumLength=0, boolean=0;
    char fileNameBefore[6]={'a','b','1','1','d','1'};
    char fileNameAfter[6]={'A','1','C','1','\0','\0'};
    int *pIntMaxOrMin[2]={&minimumLength,&maximumLength};

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting checkInputFileName function.\n");
    fileName[20]='\0';

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"inputFileName=%p\n",(void*)inputFileName);
    fprintf(stdout,"maximumLength=%d\n",maximumLength);
    returnValue=checkInputFileName(inputFileName,maximumLength);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    inputFileName=fileName;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"inputFileName=%p\n",(void*)inputFileName);
    fprintf(stdout,"maximumLength=%d\n",maximumLength);
    returnValue=checkInputFileName(inputFileName,maximumLength);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (maximumLength=-30; maximumLength<31; maximumLength++)
    {
        for (i=0; i<2; i++)
        {
            for (j=-30; j<31; j++)
            {
                for (k=0; k<6; k++)
                {
                    for (iTest=0; iTest<2; iTest++)
                    {
                        for (inputOrNot=0; inputOrNot<2; inputOrNot++)
                        {
                            for (l=0; l<20; l++)
                            {
                                fileName[l]=rand()%90+33;
                            }

                            if (iTest)
                            {
                                for (l=0; l<20; l++)
                                {
                                    if (l<*pIntMaxOrMin[i]+j)
                                    {
                                        if (fileNameBefore[k]!='1')
                                        {
                                            fileName[l]=fileNameBefore[k];
                                        }
                                    }
                                    else if (l==*pIntMaxOrMin[i]+j)
                                    {
                                        fileName[l]='\0';
                                    }
                                    else
                                    {
                                        if (fileNameBefore[k]!='1')
                                        {
                                            fileName[l]=fileNameAfter[k];
                                        }
                                    }
                                }
                            }
                            else
                            {
                                fileName[rand()%20]='\0';
                            }

                            boolean=strlen(fileName)+1;
                            if (inputOrNot && boolean>6)
                            {
                                fileName[boolean-7]='\0';
                                strcat(fileName,".input");
                            }

                            expectedValue=1;
                            if (maximumLength<minimumLength ||
                                                        boolean>maximumLength ||
                                                        boolean<minimumLength ||
                                                     fileName[boolean-7]!='.' ||
                                                     fileName[boolean-6]!='i' ||
                                                     fileName[boolean-5]!='n' ||
                                                     fileName[boolean-4]!='p' ||
                                                     fileName[boolean-3]!='u' ||
                                                       fileName[boolean-2]!='t')
                            {
                                expectedValue=0;
                            }

                            PRINT_TEST_START(counter,expectedValue);
                            fprintf(stdout,"inputFileName=%s\n",inputFileName);
                            fprintf(stdout,"maximumLength=%d\n",maximumLength);
                            returnValue=checkInputFileName(inputFileName,
                                                                 maximumLength);
                            PRINT_TEST_END(counter,counterSuccess,counterFail,
                                            returnValue,expectedValue,readChar);
                        }
                    }
                }
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
/*
////////////////////////////////////////////////////////////////////////////////
// Unit tests on initialFileExists of main.c file
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
// Unit tests on closeTheFile of main.c file
////////////////////////////////////////////////////////////////////////////////
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
