#ifndef DEF_TEST
#define DEF_TEST

/**
* \file test.h
* \brief Macros used to start/end a test and prototypes of all unit-testing
*        functions.
* \author Jeremy DALPHIN
* \version 3.0
* \date May 1st, 2019
*
* This file contains two macro definition functions in order to start and end a
* test perform inside the unit-testing functions. We then detail all the
* unit-testing function prototypes corresponding to every function used in the
* MPD program.
*/

/**
* \def PRINT_TEST_START(counter,expectedValue)
* \brief Used to initialize the beginning of a test in any unit-testing
*        function.
*
* It prints in the standard output stream the counter variable i.e. number of
* the test the unit-testing function is performing. It also displays the
* expectedValue i.e. the value that the function tested is intended to
* return.
*/
#define PRINT_TEST_START(counter,expectedValue)                                \
do {                                                                           \
    (counter)++;                                                               \
    fprintf(stdout,"\nTest %d starts: ",counter);                              \
    fprintf(stdout,"expecting %d as returnValue.\n",expectedValue);            \
} while(0)

/**
* \def PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
*                                                        expectedValue,readChar)
* \brief Used to sum up the result obtained at the end of a test performed in
*        any unit-testing functions.
*
* It prints in the standard output stream the counter variable i.e. number of
* the test the unit-testing function has finished to perform and the result of
* the test (fail or success). If the returnValue of the function that has been
* tested corresponds to the expectedValue of the test, counterSuccess is
* incremented by one; otherwise, we add one to the counterFail variable and the
* unit-testing function is paused until the 'Enter' keyboard key is pressed
* (i.e. apppears in the standard input stream), which allows the user to
* manipulate external files in order to understand why the test failed.
*/
#define PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,         \
                                                       expectedValue,readChar) \
do {                                                                           \
    if ((returnValue)==(expectedValue))                                        \
    {                                                                          \
        fprintf(stdout,"Test %d succeeded: we got returnValue",counter);       \
        fprintf(stdout,"=%d as expected.\n",returnValue);                      \
        (counterSuccess)++;                                                    \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        fprintf(stdout,"Test %d failed: returnValue",counter);                 \
        fprintf(stdout,"=%d was not the expected value.\n",returnValue);       \
        (counterFail)++;                                                       \
        (readChar)=getchar();                                                  \
        while ((readChar)!='\n' && (readChar)!=EOF)                            \
        {                                                                      \
            (readChar)=getchar();                                              \
        }                                                                      \
    }                                                                          \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// Prototype of the non-static functions of the test.c file
////////////////////////////////////////////////////////////////////////////////
/**
* \fn void test(void)
* \brief It gathers all the other unit-testing functions.
*
* The function \ref test is the function of the test.c file that allows to
* gather all the other unit-testing functions that we want to test in the \ref
* main function (other unit-testing functions should be put in comment in \ref
* test function). We use the convention that if a function is called myFunction
* for example, its associated unit-testing function is called testMyFunction.
* It does not have any input argument and does not return any argument (void
* input and output).
*/
void test(void);

/**
* \fn void testCommentAnormalEnd(void)
* \brief It performs (random) unit tests on the \ref commentAnormalEnd function
*        of the main.c file.
*/
void testCommentAnormalEnd(void);

/**
* \fn void testPrintTimer(void)
* \brief It performs (random) unit tests on the \ref printTimer function of the
*        main.c file.
*/
void testPrintTimer(void);

/**
* \fn void testEndTimerAtExit(void)
* \brief It performs (random) unit tests on the \ref endTimerAtExit function of 
*        the main.c file.
*/
void testEndTimerAtExit(void);

/**
* \fn void testEndTimerAtError(void)
* \brief It performs (random) unit tests on the \ref endTimerAtError function of
*        the main.c file.
*/
void testEndTimerAtError(void);

/**
* \fn void testInitializeParameterStructure(void)
* \brief It performs (random) unit tests on the \ref
*        initializeParameterStructure function of the loadParameters.c file.
*/
void testInitializeParameterStructure(void);

/**
* \fn void testInitializeChemicalStructure(void)
* \brief It performs (random) unit tests on the \ref initializeChemicalStructure
*        function of the loadChemistry.c file.
*/
void testInitializeChemicalStructure(void);

/**
* \fn void testInitializeDataStructure(void)
* \brief It performs (random) unit tests on the \ref initializeDataStructure
*        function of the computeData.c file.
*/
void testInitializeDataStructure(void);

/**
* \fn void testInitializeMeshStructure(void)
* \brief It performs (random) unit tests on the \ref initializeMeshStructure
*        function of the loadMesh.c file.
*/
void testInitializeMeshStructure(void);

/**
* \fn void testFreeMeshMemory(void)
* \brief It performs (random) unit tests on the \ref freeMeshMemory function of
*        the loadMesh.c file.
*/
void testFreeMeshMemory(void);

// Unit (random) tests on freeDataMemory of computeData.c file
/**
* \fn void testFreeDataMemory(void)
* \brief It performs (random) unit tests on the \ref freeDataMemory function of
*        the computeData.c file.
*/
void testFreeDataMemory(void);

/**
* \fn void testFreeChemicalMemory(void)
* \brief It performs (random) unit tests on the \ref freeChemicalMemory function
*        of the loadChemistry.c file.
*/
void testFreeChemicalMemory(void);

/**
* \fn void testFreeParameterMemory(void)
* \brief It performs (random) unit tests on the \ref freeParameterMemory
*        function of the loadParameters.c file.
*/
void testFreeParameterMemory(void);

/**
* \fn static void testCheckStringFromLength(void)
* \brief It performs (random) unit tests on the \ref checkStringFromLength
*        function of the main.c file.
*/
void testCheckStringFromLength(void);

/**
* \fn static void testCheckAllPreprocessorConstants(void)
* \brief It performs unit tests on the \ref checkAllPreprocessorConstants
*        function of the main.c file.
*/
void testCheckAllPreprocessorConstants(void);

/**
* \fn static void testInitialFileExists(void)
* \brief It performs unit tests on the \ref initialFileExists function of the
*        main.c file.
*/
void testInitialFileExists(void);

/**
* \fn void testCheckForTildeAndReplaceByHomePath(void)
* \brief It performs unit (random) tests on the \ref
*        checkForTildeAndReplaceByHomePath function of loadParameters.c file.
*/
void testCheckForTildeAndReplaceByHomePath(void);

/**
* \fn void testSetupDefaultParameters(void)
* \brief It performs unit (random) tests on the \ref setupDefaultParameters
*        function of the loadParameters.c file.
*/
void testSetupDefaultParameters(void);

/**
* \fn static void testCloseTheFile(void)
* \brief It performs unit tests on the \ref closeTheFile function of the main.c
*        file.
*/
void testCloseTheFile(void);

/**
* \fn void testGetLengthAfterKeywordBeginning(void)
* \brief It performs unit (random) tests on the \ref
*        getLengthAfterKeywordBeginning function of the loadParameters.c file.
*/
void testGetLengthAfterKeywordBeginning(void);

/**
* \fn void testGetTypeAfterKeyword(void)
* \brief It performs unit (random) tests on the \ref getTypeAfterKeyword
*        function of the loadParameters.c file.
*/
void testGetTypeAfterKeyword(void);

/**
* \fn void testGetLengthAfterKeywordMiddle(void)
* \brief It performs unit (random) tests on the \ref
*        getLengthAfterKeywordMiddle function of the loadParameters.c file.
*/
void testGetLengthAfterKeywordMiddle(void);

/**
* \fn void testDetectRepetition(void)
* \brief It performs unit (random) tests on the \ref detectRepetition function
*        of the loadParameters.c file.
*/
void testDetectRepetition(void);

/**
* \fn void testChangeValuesOfParameters(void)
* \brief It performs unit (random) tests on the \ref changeValuesOfParameters
*        function of the loadParameters.c file.
*/
void testChangeValuesOfParameters(void);

/**
* \fn void testReadInfoFileAndGetParameters(void)
* \brief It performs unit (random) tests on the \ref
*        readInfoFileAndGetParameters function of the loadParameters.c file.
*/
void testReadInfoFileAndGetParameters(void);

/**
* \fn void testCheckValuesOfAllParameters(void)
* \brief It performs unit (random) tests on the \ref checkValuesOfAllParameters
*        function of the loadParameters.c file.
*/
void testCheckValuesOfAllParameters(void);

#endif

