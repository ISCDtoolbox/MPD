/**
* \file main.c
* \brief Main and shared functions of the MPD program.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* This file contains the \ref main function of the MPD program and also all the
* functions that are shared by all the files related to the MPD program.
*/

/** \mainpage Maximizing optimal probability domain
*
* The MPD program aims to compute the shape of the domain that maximizes the
* probability to find exactly a certain number of electrons inside it, given a
* chemical electronic structure.
*
* In addition, a certain number of other iscd softwares
* (https://github.com/ISCDtoolbox) must have been previously installed
* medit to vizualize 3D mesh (see https://github.com/ISCDtoolbox/Medit)
* mshdist to redistanciate level-set function
* (see https://github.com/ISCDtoolbox/Mshdist)
* elastic to solve elasticity on 3D mesh
* (see https://github.com/ISCDtoolbox/LinearElasticity)
* advect to advect a level-function function thanks to a vector field
* (see https://github.com/ISCDtoolbox/Advection)
* mmg3d_03 to adapt 3D mesh (see https://github.com/Mmgtools)
*
* Important remark: the version of mmg3d we need for the mpdProgram is a
* modified version of the official one.
*/

#include "main.h"
#include "loadParameters.h"
#include "loadChemistry.h"
#include "loadMesh.h"
#include "adaptMesh.h"
#include "optimization.h"
//#include "test.h"

/**
* \var globalInitialTimer
* \brief Global variable used to store the time at which the program starts.
*/
time_t globalInitialTimer=0;

/**
* \def STR_PHASE
* \brief Used to frame some important informations during the MPD algorithm.
*/
#define STR_PHASE "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
%%%%%%%%%%%%%%%%%%%%"

/**
* \def STR_RELEASE
* \brief Used to specify the current date of release of the MPD program.
*/
#define STR_RELEASE "September 1st, 2018"

/**
* \def STR_VERSION
* \brief Used to specify the current version of the MPD program.
*/
#define STR_VERSION "2.0"

/**
* \def STR_COPYRIGHT
* \brief Used to specify the copyright informations of the MPD program.
*/
#define STR_COPYRIGHT "2018, ISCD-Sorbonne Universités"

/**
* \def STR_AUTHOR
* \brief Used to specify the author(s) of the MPD program.
*/
#define STR_AUTHOR "Jeremy DALPHIN"

/* ************************************************************************** */
// Macro functions to initialize and end properly the main function
/* ************************************************************************** */
/**
* \def INITIALIZE_MAIN_STRUCTURES(parameters,chemicalSystem,data,mesh,argc)
* \brief Used to initialize the \ref main function of the MPD program.
*
* It starts the global timer, sets up the tracking of anormal-stop signals via
* the \ref commentAnormalEnd function, forces the program to execute \ref
* endTimerAtExit function when the program stops, prints some basic informations
* about the MPD program, checks that the standard input stream (argc and argv
* arguments of the \ref main function) contain only one argument, and sets to
* zero all the main structures (Parameters, ChemicalSystem, Data, and Mesh).
*/
#define INITIALIZE_MAIN_STRUCTURES(parameters,chemicalSystem,data,mesh,argc)   \
do {                                                                           \
    time(&globalInitialTimer);                                                 \
    atexit(endTimerAtExit);                                                    \
    signal(SIGABRT,commentAnormalEnd);                                         \
    signal(SIGFPE,commentAnormalEnd);                                          \
    signal(SIGILL,commentAnormalEnd);                                          \
    signal(SIGSEGV,commentAnormalEnd);                                         \
    signal(SIGTERM,commentAnormalEnd);                                         \
    signal(SIGINT,commentAnormalEnd);                                          \
    fprintf(stdout,"\n%s\nMPD program starts. ",STR_PHASE);                    \
    fprintf(stdout,"Released version %s ",STR_VERSION);                        \
    fprintf(stdout,"(%s).\nCopyright (c) %s. ",STR_RELEASE,STR_COPYRIGHT);     \
    fprintf(stdout,"Author: %s.\n%s\n",STR_AUTHOR,STR_PHASE);                  \
    if ((argc)!=2)                                                             \
    {                                                                          \
        fprintf(stderr,"\nError encountered at line %d in ",__LINE__);         \
        fprintf(stderr,"%s file on %s",__FILE__,ctime(&globalInitialTimer));   \
        fprintf(stderr,"In main: the MPD program only accept one input ");     \
        fprintf(stderr,"command-line argument (instead of %d); ",(argc)-1);    \
        fprintf(stderr,"the *.info (input) file, which must contain at ");     \
        fprintf(stderr,"least the *.wfn/ *.chem (chemical) file name, ");      \
        fprintf(stderr,"preceded by the 'name_chem' keyword, the ");           \
        fprintf(stderr,"(positive) number of electrons to look for, ");        \
        fprintf(stderr,"preceded by the 'nu_electrons' keyword, and ");        \
        fprintf(stderr,"optionally the *.mesh/ *.cube (mesh) file name to ");  \
        fprintf(stderr,"start with, preceded by the 'name_mesh' keyword. ");   \
        fprintf(stderr,"The *.info file must end with the 'end_data' ");       \
        fprintf(stderr,"keyword and any other informations placed after ");    \
        fprintf(stderr,"will not be read and considered as comments.\n");      \
    }                                                                          \
    initializeParameterStructure(parameters);                                  \
    initializeChemicalStructure(chemicalSystem);                               \
    initializeDataStructure(data);                                             \
    initializeMeshStructure(mesh);                                             \
} while (0)

/**
* \def FREE_AND_RETURN(parameters,chemicalSystem,data,mesh,valueToReturn)
* \brief Used to end the \ref main function of the MPD program.
*
* It frees the memory dynamically allocated for the main structures (Parameters,
* ChemicalSystem, Data, and Mesh) and finally returns the ouput value
* (valueToReturn) of the \ref main function.
*/
#define FREE_AND_RETURN(parameters,chemicalSystem,data,mesh,valueToReturn)     \
do {                                                                           \
    freeMeshMemory(mesh);                                                      \
    freeDataMemory(data);                                                      \
    freeChemicalMemory(chemicalSystem);                                        \
    freeParameterMemory(parameters);                                           \
    return (valueToReturn);                                                    \
} while (0)

/**
* \def PRINT_LOCAL_TIME(nIter,stringToPrint,startLocalTimer,endLocalTimer)
* \brief Used to print the execution associated with a intermediate process
*        inside the \ref main function .
*/
#define PRINT_LOCAL_TIME(nIter,stringToPrint,startLocalTimer,endLocalTimer)    \
do {                                                                           \
    time(&(endLocalTimer));                                                    \
    fprintf(stdout,"\n%s\nITERATION %d COMPLETED: ",stringToPrint,nIter);      \
    printTimer(endLocalTimer,startLocalTimer);                                 \
    fprintf(stdout,".\n");                                                     \
} while (0)

/* ************************************************************************** */
// The function main performs on a given initial domain in a computational box
// (mesh variable) (parameter.)iter_max deformations to maximize the probability
// to find (parameter.)nu_electrons in a chemicalSystem structure. It has the
// usual int argc, char *argv[] command line as input arguments and it returns
// EXIT_FAILURE if an error occurs, otherwise EXIT_SUCCESS is returned. The
// command line must specify the *.info (input) file, which must contain at
// least the *.wfn / *.chem (chemical) file, preceded by 'name_chem' keyword,
// the (positive) number of electrons to look for, preceded by 'nu_electrons'
// keyword, and optionally the *.mesh / *.cube (mesh data) file to start with,
// preceded by 'name_mesh' keyword (if not specify a cube or a sphere is built).
// The *.info file must end with the 'end_data' keyword. Any other informations
// placed after 'end_data' will not be read and considered as comments
/* ************************************************************************** */
int main(int argc, char *argv[])
{
    time_t startLocalTimer=0, endLocalTimer=0;
    int i=0, j=0, optMode=0, iStop=0, jPlus=0, jMoins=0;
    double d0p=0., d1p=0., d2p=0.;
    Parameters parameters;
    ChemicalSystem chemicalSystem;
    Data data;
    Mesh mesh;

    // Initialize the main structures to zero (nothing should be placed before)
    INITIALIZE_MAIN_STRUCTURES(&parameters,&chemicalSystem,&data,&mesh,argc);
    if (argc!=2)
    {
        FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,EXIT_FAILURE);
    }

    // We set the seed for rand() function then we test the selected functions
    //srand(time(NULL)); // Warning: srand(time(NULL)) must be set only one time
    //test();            // It contains all the selected unit-testing functions

    // Check values of all the preprocessor constants
    if (!checkAllPreprocessorConstants(OPT_MODE,VERBOSE,N_CPU,NAME_LENGTH,
                                       LAME_INT1,LAME_INT2,LAME_EXT1,LAME_EXT2,
                                       X_MIN,Y_MIN,Z_MIN,X_MAX,Y_MAX,Z_MAX,N_X,
                                       N_Y,N_Z,DELTA_X,DELTA_Y,DELTA_Z,LS_TYPE,
                                       LS_X,LS_Y,LS_Z,LS_R,MET_CST,MET_ERR,
                                       MET_MIN,MET_MAX,TRICK_MATRIX,APPROX_MODE,
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
                                       ORB_1S,ORB_2PX,ORB_2PY,ORB_2PZ,ORB_3DXX,
                                       ORB_3DYY,ORB_3DZZ,ORB_3DXY,ORB_3DXZ,
                                       ORB_3DYZ,ORB_4FXXX,ORB_4FYYY,ORB_4FZZZ,
                                       ORB_4FXXY,ORB_4FXXZ,ORB_4FYYZ,ORB_4FXYY,
                                       ORB_4FXZZ,ORB_4FYZZ,ORB_4FXYZ,CST_A,
                                       CST_B,CST_C,CST_a,CST_b,CST_c,CST_aa,
                                       CST_bb,CST_cc,CST_ONE,CST_TWO,CST_THREE,
                                       CST_1,CST_2,CST_3,CST_22,CST_33,INV_PHI,
                                                                      INV_PHI2))
    {
        PRINT_ERROR("In main: checkAllPreprocessorConstants function ");
        fprintf(stderr,"returned zero instead of one.\n");
        FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,EXIT_FAILURE);
    }

    // Load parameters from a *.info (input) file pointed by argv[1]
    if (!loadParameters(&parameters,argv[1]))
    {
        PRINT_ERROR("In main: loadParameters function returned zero instead ");
        fprintf(stderr,"of one.\n");
        FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,EXIT_FAILURE);
    }

    // Load chemistry from a *.chem/ *.wfn file pointed by parameters.name_chem
    if (!loadChemistry(&parameters,&chemicalSystem))
    {
        PRINT_ERROR("In main: loadChemistry function returned zero instead ");
        fprintf(stderr,"of one.\n");
        FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,EXIT_FAILURE);
    }

    // Load default mesh or from the *.mesh/ *.cube file of parameters.name_mesh
    switch (loadMesh(&parameters,&mesh))
    {
        case -1:
            if (!writingCubeFile(&parameters,&chemicalSystem,&mesh))
            {
                PRINT_ERROR("In main: writingCubeFile function returned zero ");
                fprintf(stderr,"instead of one.\n");
                FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,
                                                                  EXIT_FAILURE);
            }
            FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,
                                                                  EXIT_SUCCESS);
            break;

        case 1:
            if (!writingCubeFile(&parameters,&chemicalSystem,&mesh))
            {
                PRINT_ERROR("In main: writingCubeFile function returned zero ");
                fprintf(stderr,"instead of one.\n");
                FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,
                                                                  EXIT_FAILURE);
            }

            PRINT_LOCAL_TIME(-1,STR_PHASE,globalInitialTimer,endLocalTimer);
            fprintf(stdout,"PARAMETERS, CHEMISTRY, AND MESH STRUCTURES ");
            fprintf(stdout,"SUCCESSFULLY INITIALIZED.\n%s\n",STR_PHASE);

            // If pParameters->save_print==0, graphic and prompt mode is off
            if (parameters.save_print)
            {
                // Vizualize the mesh of the initial cube with medit (Warning:
                // the medit software must have been previously installed)
                if (!plotMeshWithMeditSoftware(&parameters))
                {
                    PRINT_ERROR("In main: plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,
                                                                  EXIT_FAILURE);
                }
            }
        break;

        default:
            PRINT_ERROR("In main: loadMesh function returned zero instead ");
            fprintf(stderr,"of (+/-)one.\n");
            FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,
                                                                  EXIT_FAILURE);
            break;
    }

    // Prepare the mesh for the optimization loop (initialize the level-set
    // function and perform some mesh adaptations if necessary)
    time(&startLocalTimer);
    if (!adaptMesh(&parameters,&mesh,&chemicalSystem))
    {
        PRINT_ERROR("In main: adaptMesh function returned zero instead of ");
        fprintf(stderr,"one.\n");
        FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,EXIT_FAILURE);
    }

    if (!setupInitialData(&parameters,&mesh,&data,&chemicalSystem,
                           &globalInitialTimer,&startLocalTimer,&endLocalTimer))
    {
        PRINT_ERROR("In main: setupInitialData function returned zero ");
        fprintf(stderr,"instead of one.\n");
        FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,EXIT_FAILURE);
    }

    PRINT_LOCAL_TIME(0,STR_PHASE,startLocalTimer,endLocalTimer);
    fprintf(stdout,"MESH ADAPTED AND INITIAL DOMAIN READY FOR THE ");
    fprintf(stdout,"OPTIMIZATION LOOP.\n%s\n",STR_PHASE);

    // Start the optimization depending on the different mode
    optMode=-1;
    if (!parameters.opt_mode)
    {
        optMode=parameters.opt_mode;
        parameters.opt_mode=-1;
    }

    for (i=1; i<=parameters.iter_max; i++)
    {
        time(&startLocalTimer);

        if (parameters.opt_mode==1 && i%3==0)
        {
            jPlus=0;
            jMoins=0;
            // Check that the shape gradient changes sign to get new domain
            // with opt_mode set to 4
            for (j=0; j<mesh.nver; j++)
            {
                if (mesh.pver[j].value<0.)
                {
                    jMoins++;
                }
                else
                {
                    jPlus++;
                }
            }

            if (jPlus && jMoins)
            {
                optMode=1;
                parameters.opt_mode=4;
            }
        }

        switch (optimization(&parameters,&mesh,&data,&chemicalSystem,i,
                           &globalInitialTimer,&startLocalTimer,&endLocalTimer))
        {
            case 1:
                PRINT_LOCAL_TIME(i,STR_PHASE,startLocalTimer,endLocalTimer);
                fprintf(stdout,"PROBABILITY RESIDUAL: %.8le",data.d1p[i]);
                fprintf(stdout,"\n%s\n",STR_PHASE);
                if (optMode==1 && parameters.opt_mode==4 && i%3==0)
                {
                    optMode=-1;
                    parameters.opt_mode=1;

                    // Rescale the shape gradient by -1 to avoid domaininversion
                    for (j=0; j<mesh.nver; j++)
                    {
                        mesh.pver[j].value*=-1.;
                    }
                }
                break;

            case -1:
                if (optMode==0 && parameters.opt_mode==-1)
                {
                    parameters.opt_mode=-2;
                    i--;
                    iStop=i;
                    d0p=data.d0p[i];
                    d1p=data.d1p[i];
                    d2p=data.d2p[i];
                    data.d0p[i]+=parameters.iter_told0p;
                    data.d1p[i]+=parameters.iter_told1p;
                    data.d2p[i]+=parameters.iter_told2p;

                    // At the transition, undo on labels the shape gradient mode
                    for (j=0; j<mesh.nadj; j++)
                    {
                        if (abs(mesh.phex[mesh.padj[j].hexout-1].label)==3)
                        {
                            mesh.phex[mesh.padj[j].hexout-1].label=2;
                        }
                        if (abs(mesh.phex[mesh.padj[j].hexin-1].label)==2)
                        {
                            mesh.phex[mesh.padj[j].hexin-1].label=3;
                        }
                    }
                }
                else
                {
                    i=parameters.iter_max+1;
                }
                break;

            default:
                PRINT_ERROR("In main: optimization function returned zero ");
                fprintf(stderr,"instead of (+/-)one.\n");
                FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,
                                                                  EXIT_FAILURE);
                break;
        }
    }

    if (!optMode)
    {
        parameters.opt_mode=0;
        data.d0p[iStop]=d0p;
        data.d1p[iStop]=d1p;
        data.d2p[iStop]=d2p;
    }

    FREE_AND_RETURN(&parameters,&chemicalSystem,&data,&mesh,EXIT_SUCCESS);
}

/* ************************************************************************** */
// The function printTimer takes the difference between the finalTimer and
// initialTimer variables and converts it into a human readable time format
// (hour(s), minute(s), second(s), instantaneous), which is finally printed in
// the standard output stream. It has the two time_t variables (initialTimer,
// finalTimer) as input arguments and does not return any argument (void output)
/* ************************************************************************** */
void printTimer(time_t finalTimer, time_t initialTimer)
{
    int totalTime=0, timeH=0, timeM=0, timeS=0;

    totalTime=(int)difftime(finalTimer,initialTimer);
    timeH=totalTime/3600;
    timeM=totalTime%3600;
    timeS=timeM%60;
    timeM/=60;

    if (timeH)
    {
        fprintf(stdout,"%d hour",timeH);
        if (abs(timeH)!=1)
        {
            fprintf(stdout,"s");
        }
        if (timeM && timeS)
        {
            fprintf(stdout,", ");
        }
        else if (timeM || timeS)
        {
            fprintf(stdout," and ");
        }
    }
    if (timeM)
    {
        fprintf(stdout,"%d minute",timeM);
        if (abs(timeM)!=1)
        {
            fprintf(stdout,"s");
        }
        if (timeS)
        {
            fprintf(stdout," and ");
        }
    }
    if (timeS)
    {
        fprintf(stdout,"%d second",timeS);
        if (abs(timeS)!=1)
        {
            fprintf(stdout,"s");
        }
    }
    if (!timeH && !timeM && !timeS)
    {
        fprintf(stdout,"(almost) instantaneous");
    }

    return;
}

/* ************************************************************************** */
// The function endTimerAtExit is only called by atexit i.e. when the program
// will stop. It uses the global variable globalInitialTimer in order to print
// properly the total execution time of the MPD program. It has no argument and
// it returns no argument (void input and output)
/* ************************************************************************** */
void endTimerAtExit(void)
{
    time_t localFinalTimer=0;

    fprintf(stdout,"\n%s\nTotal memory freed up. ",STR_PHASE);
    fprintf(stdout,"MPD program ends.\nTotal execution time: ");

    time(&localFinalTimer);
    printTimer(localFinalTimer,globalInitialTimer);

    fprintf(stdout,".\n%s\n\n",STR_PHASE);
    fflush(stdout);
}

/* ************************************************************************** */
// The function endTimerAtError is called only by PRINT_ERROR when an error
// occurs in the program or by the saveDataInTheLoop function if its input
// iterationInTheLoop variable is set to zero. It has no argument (void input)
// and it returns the time as a string (output of the standard ctim c-function)
/* ************************************************************************** */
char* endTimerAtError(void)
{
    time_t localFinalTimer=0;

    time(&localFinalTimer);

    return ctime(&localFinalTimer);
}

/* ************************************************************************** */
// The function commentAnormalEnd is meant to display an error message in the
// standard output stream when an anormal end of the MPD program occurred. It is
// meant to describe the type of anormal error encountered and it is only called
// when one of the standard signal is caught (SIGABRT, SIGFPE, SIGILL, SIGSEGV,
// SIGTERM, SIGINT). It has the int typeOfSignal variable as input argument and
// it does not return any value (void output)
/* ************************************************************************** */
void commentAnormalEnd(int typeOfSignal)
{
    fprintf(stdout,"\nUnexpected error occurred: ");
    switch (typeOfSignal)
    {
        case SIGABRT:
            fprintf(stdout,"potential lack of memory.\n");
            break;

        case SIGFPE:
            fprintf(stdout,"floating-point exception.\n");
            break;

        case SIGILL:
            fprintf(stdout,"illegal instruction.\n");
            break;

        case SIGSEGV:
            fprintf(stdout,"segmentation fault.\n");
            break;

        case SIGTERM:
        case SIGINT:
            fprintf(stdout,"program killed.\n");
            break;

        default:
            fprintf(stdout,"signal %d caught.\n",typeOfSignal);
            break;
    }
    exit(EXIT_FAILURE);
}

/* ************************************************************************** */
// The function checkStringFromLength evaluates the length (including the
// terminating nul character '\0') of stringTocheck, which must be comprised
// between minimumLength and maximumLength. It has the char* stringTocheck and
// the two int variables (0<minimumLength<=maximumLength) as input arguments and
// it returns zero if an error is encountered, otherwise it returns the
// (positive) length of stringToCheck
/* ************************************************************************** */
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                              int maximumLength)
{
    int i=0;

    // Check if stringToCheck is pointing to NULL
    if (stringToCheck==NULL)
    {
        PRINT_ERROR("In checkStringFromLength: the input (char*) variable ");
        fprintf(stderr,"stringToCheck is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)stringToCheck);
        return 0;
    }

    // Check if the bounds are correct
    if (minimumLength<1 || maximumLength<minimumLength)
    {
        PRINT_ERROR("In checkStringFromLength: the input variable ");
        fprintf(stderr,"minimumLength (=%d) should be a ",minimumLength);
        fprintf(stderr,"positive integer less or equal to the (input) ");
        fprintf(stderr,"variable maximumLength (=%d).\n",maximumLength);
        return 0;
    }

    // Check if stringToCheck is a string of correct length
    i=0;
    while (stringToCheck[i]!='\0' && i<maximumLength)
    {
        i++;
    }
    if (i==maximumLength)
    {
        PRINT_ERROR("In checkStringFromLength: the input (char*) variable ");
        fprintf(stderr,"stringToCheck (=%p) is supposed ",(void*)stringToCheck);
        fprintf(stderr,"to store a name as a string. It seems that ");
        fprintf(stderr,"stringToCheck is not a string (no terminating NUL ");
        fprintf(stderr,"character found) or has a length greater or equal to ");
        fprintf(stderr,"maximumLength (=%d).\n",maximumLength);
        return 0;
    }
    else if (i<minimumLength-1)
    {
        PRINT_ERROR("In checkStringFromLength: the input (char*) variable ");
        fprintf(stderr,"stringToCheck is a string %s of length ",stringToCheck);
        fprintf(stderr,"%d which is strictly less than the minimal size ",i);
        fprintf(stderr,"(=%d) allowed here.\n",minimumLength-1);
        return 0;
    }

    // Add the terminating NUL character '\0' to the length size
    i++;

    return i;
}

/* ************************************************************************** */
// The function checkAllPreprocessorConstants tests if all the preprocessor
// constants are associated with valid values. It has all the preprocessor
// constants as input arguments and returns one on success, otherwise zero
/* ************************************************************************** */
int checkAllPreprocessorConstants(int optMode, int verbose, int nCpu,
                                  int nameLength, double lameInt1,
                                  double lameInt2, double lameExt1,
                                  double lameExt2, double xMin, double yMin,
                                  double zMin, double xMax, double yMax,
                                  double zMax, int nX, int nY,int nZ,
                                  double deltaX, double deltaY, double deltaZ,
                                  int lsType, double lsX, double lsY,
                                  double lsZ, double lsR, double metCst,
                                  double metErr, double metMin, double metMax,
                                  int trickMatrix, int approxMode, int iterMax,
                                  double iterTolD0P, double iterTolD1P,
                                  double iterTolD2P, int saveType, int saveMesh,
                                  int saveData, int savePrint, int saveWhere,
                                  int pathLength, char* pathMedit,
                                  char* pathMmg3d, char* pathMshdist,
                                  char* pathElastic, char* pathAdvect,
                                  double hminIso, double hmaxIso,
                                  double hausdIso, double hgradIso,
                                  double hminMet, double hmaxMet,
                                  double hausdMet, double hgradMet,
                                  double hminLs, double hmaxLs, double hausdLs,
                                  double hgradLs, int hmodeLag, double hminLag,
                                  double hmaxLag, double hausdLag,
                                  double hgradLag, int nIter, double residual,
                                  double deltaT, int noCfl, int orb1, int orb2,
                                  int orb3, int orb4, int orb5, int orb6,
                                  int orb7, int orb8, int orb9, int orb10,
                                  int orb11, int orb12, int orb13, int orb14,
                                  int orb15, int orb16, int orb17, int orb18,
                                  int orb19, int orb20, double cstA,
                                  double cstB, double cstC, double csta,
                                  double cstb, double cstc, double cstaa,
                                  double cstbb, double cstcc, double cstOne,
                                  double cstTwo, double cstThree, double cst1,
                                  double cst2, double cst3, double cst22,
                                    double cst33, double invPhi, double invPhi2)
{
    int boolean=0;
    double dx=0., dy=0., dz=0.;

    if (verbose)
    {
        fprintf(stdout,"\nChecking validity of the preprocessor constants ");
        fprintf(stdout,"defined in all *.h files.\n");
    }

    // Check OPT_MODE
    boolean=(optMode>-3 && optMode<5);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"OPT_MODE=%d can only be set to -2, -1, 0, 1, ",optMode);
        fprintf(stderr,"2, 3, or 4.\nPlease modify the preprocessor constant ");
        fprintf(stderr,"accordingly in loadParameters.h file.\n");
        return 0;
    }

    // Check VERBOSE
    boolean=(verbose==0 || verbose==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"VERBOSE=%d can only be set to 0 or 1.\n",verbose);
        fprintf(stderr,"Please modify the preprocessor constant accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    // Check N_CPU
    boolean=(nCpu>0);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"N_CPU=%d must be a positive integer.\nPlease ",nCpu);
        fprintf(stderr,"modify the preprocessor constant accordingly in ");
        fprintf(stderr,"loadParameters.h file.\n");
        return 0;
    }

    // Check NAME_LENGTH
    boolean=(nameLength>6);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"NAME_LENGTH=%d should be an integer ",nameLength);
        fprintf(stderr,"(strictly) greater than six (to store at least a ");
        fprintf(stderr,"name that contains something more than the *.info ");
        fprintf(stderr,"extension).\nPlease modify the preprocessor constant ");
        fprintf(stderr,"accordingly in loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the Lamé's coefficients
    boolean=(lameInt1>0. && lameInt2>0. && lameExt1>0. && lameExt2>0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: the first and second ");
        fprintf(stderr,"Lame's coefficients must have positive (double) ");
        fprintf(stderr,"values. Expecting\n(LAME_INT1=%lf) > 0.0\n",lameInt1);
        fprintf(stderr,"(LAME_INT2=%lf) > 0.0\n",lameInt2);
        fprintf(stderr,"(LAME_EXT1=%lf) > 0.0\n",lameExt1);
        fprintf(stderr,"(LAME_EXT2=%lf) > 0.0\n",lameExt2);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the computational box
    boolean=(xMin<xMax && yMin<yMax && zMin<zMax && nX>2 && nY>2 && nZ>2);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: incorrect description ");
        fprintf(stderr,"of the cube associated with the computational box. ");
        fprintf(stderr,"Expecting\n(X_MAX=%lf) > (X_MIN=%lf)\n",xMax,xMin);
        fprintf(stderr,"(Y_MAX=%lf) > (Y_MIN=%lf)\n",yMax,yMin);
        fprintf(stderr,"(Z_MAX=%lf) > (Z_MIN=%lf)\n",zMax,zMin);
        fprintf(stderr,"(N_X=%d) > 2\n(N_Y=%d) > 2\n(N_Z=%d) > 2\n",nX,nY,nZ);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    dx=(xMax-xMin)/(double)(nX-1);
    dy=(yMax-yMin)/(double)(nY-1);
    dz=(zMax-zMin)/(double)(nZ-1);

    boolean=(DEF_ABS(deltaX-dx)<1.e-16);
    boolean=(boolean && DEF_ABS(deltaY-dy)<1.e-16);
    boolean=(boolean && DEF_ABS(deltaZ-dz)<1.e-16);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: incorrect ");
        fprintf(stderr,"discretization of the cube associated with the ");
        fprintf(stderr,"computational box. Expecting\n");
        fprintf(stderr,"(DELTA_X=%.18lf) == %.18lf\n",deltaX,dx);
        fprintf(stderr,"(DELTA_Y=%.18lf) == %.18lf\n",deltaY,dy);
        fprintf(stderr,"(DELTA_Z=%.18lf) == %.18lf\n",deltaZ,dz);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the level-set function
    boolean=(lsType==0 || lsType==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"LS_TYPE=%d can only be set to 0 or 1.\nPlease ",lsType);
        fprintf(stderr,"modify the preprocessor constant accordingly in ");
        fprintf(stderr,"loadParameters.h file.\n");
        return 0;
    }

    boolean=(lsX>=xMin && lsX<=xMax);
    boolean=(boolean && lsY>=yMin && lsY<=yMax);
    boolean=(boolean && lsZ>=zMin && lsZ<=zMax);
    if (!boolean && verbose)
    {
        fprintf(stdout,"\nWarning in checkAllPreprocessorConstants function: ");
        fprintf(stdout,"the center (%lf,%lf,%lf) of the initial ",lsX,lsY,lsZ);
        if (lsType)
        {
            fprintf(stdout,"sphere (defined in loadParameters.h file) is not ");
        }
        else
        {
            fprintf(stdout,"cube (defined in loadParameters.h file) is not ");
        }
        fprintf(stdout,"located inside the default computational domain ");
        fprintf(stdout,"[%lf,%lf]x[%lf,%lf]x",xMin,xMax,yMin,yMax);
        fprintf(stdout,"[%lf,%lf].\n",zMin,zMax);
    }

    boolean=(lsR>0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: the ");
        if (lsType)
        {
             fprintf(stderr,"radius LS_R=%lf of the initial sphere must ",lsR);
             fprintf(stderr,"be positive.\nPlease modify the preprocessor ");
        }
        else
        {
             fprintf(stderr,"length LS_R=%lf of the initial cube must ",lsR);
             fprintf(stderr,"be positive.\nPlease modify the preprocessor ");
        }
        fprintf(stderr,"constant accordingly in loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the metric computation
    boolean=(metCst==.28125 && metErr>0. && metMin>0. && metMax>metMin);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(MET_CST=%lf) == 0.28125\n",metCst);
        fprintf(stderr,"(MET_ERR=%lf) > 0.0\n",metErr);
        fprintf(stderr,"(MET_MIN=%lf) > 0.0\n",metMin);
        fprintf(stderr,"(MET_MAX=%lf) > MET_MIN\nPlease modify the ",metMax);
        fprintf(stderr,"preprocessor constants accordingly in ");
        fprintf(stderr,"loadParameters.h file (and in adaptMesh.h file if ");
        fprintf(stderr,"the MET_CST preprocessor constant is not correct).\n");
        return 0;
    }

    // Check preprocessor constants related to the overlap-matrix computation
    boolean=(trickMatrix==0 || trickMatrix==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"TRICK_MATRIX=%d can only be set to 0 or ",trickMatrix);
        fprintf(stderr,"1.\nPlease modify the preprocessor constant ");
        fprintf(stderr,"accordingly in loadParameters.h file.\n");
        return 0;
    }

    boolean=(approxMode==0 || approxMode==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"APPROX_MODE=%d can only be set to 0 or ",approxMode);
        fprintf(stderr,"1.\nPlease modify the preprocessor constant ");
        fprintf(stderr,"accordingly in loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the stop criteria
    boolean=(iterMax>=0 && iterTolD0P>=0. && iterTolD1P>=0. && iterTolD2P>=0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(ITER_MAX=%d) >= 0\n",iterMax);
        fprintf(stderr,"(ITER_TOLD0P=%lf) >= 0.0\n",iterTolD0P);
        fprintf(stderr,"(ITER_TOLD1P=%lf) >= 0.0\n",iterTolD1P);
        fprintf(stderr,"(ITER_TOLD2P=%lf) >= 0.0\n",iterTolD2P);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the saving of data
    boolean=(saveType==0 || saveType==1 || saveType==2);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"SAVE_TYPE=%d can only be set to 0, 1 or 2.\n",saveType);
        fprintf(stderr,"Please modify the preprocessor constant accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    boolean=(saveMesh>=0 && saveData>=0 && savePrint>=0);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"SAVE_MESH=%d, SAVE_DATA=%d, and ",saveMesh,saveData);
        fprintf(stderr,"SAVE_PRINT=%d must be non-negative ",savePrint);
        fprintf(stderr,"integers.\nPlease modify the preprocessor constants ");
        fprintf(stderr,"accordingly in loadParameters.h file.\n");
        return 0;
    }

    boolean=(saveWhere>0 && saveWhere<8);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"SAVE_WHERE=%d can only be set to 0, 1, 2, ",saveWhere);
        fprintf(stderr,"3, 4, 5, 6 or 7.\nPlease modify the preprocessor ");
        fprintf(stderr,"constant accordingly in loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the path specifications
    boolean=checkStringFromLength(pathMedit,2,pathLength);
    boolean=(boolean && checkStringFromLength(pathMmg3d,2,pathLength));
    boolean=(boolean && checkStringFromLength(pathMshdist,2,pathLength));
    boolean=(boolean && checkStringFromLength(pathElastic,2,pathLength));
    boolean=(boolean && checkStringFromLength(pathAdvect,2,pathLength));
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having successively checked that ");
        fprintf(stderr,"one of the preprocessor constants PATH_MEDIT, ");
        fprintf(stderr,"PATH_MMG3D, PATH_MSHDIST, PATH_ELASTIC, or ");
        fprintf(stderr,"PATH_ADVECT is not a string of length strictly less ");
        fprintf(stderr,"than PATH_LENGTH (=%d), which must be an ",pathLength);
        fprintf(stderr,"integer (strictly) greater than one.\nPlease modify ");
        fprintf(stderr,"the preprocessor constants accordingly in ");
        fprintf(stderr,"loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to parameters used in mmg3d
    boolean=(hminIso>0. && hmaxIso>hminIso && hausdIso>0. && hgradIso>1.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(HMIN_ISO=%lf) > 0.0\n",hminIso);
        fprintf(stderr,"(HMAX_ISO=%lf) > HMIN_ISO\n",hmaxIso);
        fprintf(stderr,"(HAUSD_ISO=%lf) > 0.0\n",hausdIso);
        fprintf(stderr,"(HGRAD_ISO=%lf) > 1.0\n",hgradIso);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    boolean=(hminMet>0. && hmaxMet>hminMet && hausdMet>0. && hgradMet>1.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(HMIN_MET=%lf) > 0.0\n",hminMet);
        fprintf(stderr,"(HMAX_MET=%lf) > HMIN_MET\n",hmaxMet);
        fprintf(stderr,"(HAUSD_MET=%lf) > 0.0\n",hausdMet);
        fprintf(stderr,"(HGRAD_MET=%lf) > 1.0\n",hgradMet);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    boolean=(hminLs>0. && hmaxLs>hminLs && hausdLs>0. && hgradLs>1.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(HMIN_LS=%lf) > 0.0\n",hminLs);
        fprintf(stderr,"(HMAX_LS=%lf) > HMIN_LS\n",hmaxLs);
        fprintf(stderr,"(HAUSD_LS=%lf) > 0.0\n",hausdLs);
        fprintf(stderr,"(HGRAD_LS=%lf) > 1.0\n",hgradLs);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    boolean=(hmodeLag==0 || hmodeLag==1 || hmodeLag==2);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"HMODE_LAG=%d can only be set to 0, 1 or 2.\n",hmodeLag);
        fprintf(stderr,"Please modify the preprocessor constant accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    boolean=(hminLag>0. && hmaxLag>hminLag && hausdLag>0. && hgradLag>1.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(HMIN_LAG=%lf) > 0.0\n",hminLag);
        fprintf(stderr,"(HMAX_LAG=%lf) > HMIN_LAG\n",hmaxLag);
        fprintf(stderr,"(HAUSD_LAG=%lf) > 0.0\n",hausdLag);
        fprintf(stderr,"(HGRAD_LAG=%lf) > 1.0\n",hgradLag);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }


    // Check preprocessor constants related to parameters used in mshdist/advect
    boolean=(nIter>=0 && residual>=0. && deltaT>0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(N_ITER=%d) >= 0\n",nIter);
        fprintf(stderr,"(RESIDUAL=%lf) >= 0.0\n",residual);
        fprintf(stderr,"(DELTA_T=%lf) > 0.0\n",deltaT);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in loadParameters.h file.\n");
        return 0;
    }

    boolean=(noCfl==0 || noCfl==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: ");
        fprintf(stderr,"NO_CFL=%d can only be set to 0 or 1.\nPlease ",noCfl);
        fprintf(stderr,"modify the preprocessor constant accordingly in ");
        fprintf(stderr,"loadParameters.h file.\n");
        return 0;
    }

    // Check the preprocessor constants related to the wfn/wfx chemical format
    boolean=(orb1==1 && orb2==2 && orb3==3 && orb4==4 && orb5==5);
    boolean=(boolean && orb6==6 && orb7==7 && orb8==8 && orb9==9 && orb10==10);
    boolean=(boolean && orb11==11 && orb12==12 && orb13==13 && orb14==14);
    boolean=(boolean && orb15==15 && orb16==16 && orb17==17 && orb18==18);
    boolean=(boolean && orb19==19 && orb20==20);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(ORB_1S=%d) == 1\n(ORB_2PX=%d) == 2\n",orb1,orb2);
        fprintf(stderr,"(ORB_2PY=%d) == 3\n(ORB_2PZ=%d) == 4\n",orb3,orb4);
        fprintf(stderr,"(ORB_3DXX=%d) == 5\n(ORB_3DYY=%d) == 6\n",orb5,orb6);
        fprintf(stderr,"(ORB_3DZZ=%d) == 7\n(ORB_3DXY=%d) == 8\n",orb7,orb8);
        fprintf(stderr,"(ORB_3DXZ=%d) == 9\n(ORB_3DYZ=%d) == 10\n",orb9,orb10);
        fprintf(stderr,"(ORB_4FXXX=%d) == 11\n",orb11);
        fprintf(stderr,"(ORB_4FYYY=%d) == 12\n",orb12);
        fprintf(stderr,"(ORB_4FZZZ=%d) == 13\n",orb13);
        fprintf(stderr,"(ORB_4FXXY=%d) == 14\n",orb14);
        fprintf(stderr,"(ORB_4FXXZ=%d) == 15\n",orb15);
        fprintf(stderr,"(ORB_4FYYZ=%d) == 16\n",orb16);
        fprintf(stderr,"(ORB_4FXYY=%d) == 17\n",orb17);
        fprintf(stderr,"(ORB_4FXZZ=%d) == 18\n",orb18);
        fprintf(stderr,"(ORB_4FYZZ=%d) == 19\n",orb19);
        fprintf(stderr,"(ORB_4FXYZ=%d) == 20\nPlease modify the ",orb20);
        fprintf(stderr,"preprocessor constants accordingly in main.h file.\n");
        return 0;
    }

    // Check the preprocessor constants needed for tetrahedral integration
    boolean=(cstA==0.007091003462846911 && cstB==0.018781320953002642);
    boolean=(boolean && cstC==0.012248840519393658);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(CST_A=%.18lf) == 0.007091003462846911\n",cstA);
        fprintf(stderr,"(CST_B=%.18lf) == 0.018781320953002642\n",cstB);
        fprintf(stderr,"(CST_C=%.18lf) == 0.012248840519393658\n",cstC);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in optimization.h file.\n");
        return 0;
    }

    boolean=(csta==0.454496295874350351 && cstb==0.310885919263300610);
    boolean=(boolean && cstc==0.092735250310891226);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(CST_a=%.18lf) == 0.454496295874350351\n",csta);
        fprintf(stderr,"(CST_b=%.18lf) == 0.310885919263300610\n",cstb);
        fprintf(stderr,"(CST_c=%.18lf) == 0.092735250310891226\n",cstc);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in optimization.h file.\n");
        return 0;
    }

    boolean=(cstaa==0.045503704125649649 && cstbb==0.067342242210098170);
    boolean=(boolean && cstcc==0.721794249067326322);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(CST_aa=%.18lf) == 0.045503704125649649\n",cstaa);
        fprintf(stderr,"(CST_bb=%.18lf) == 0.067342242210098170\n",cstbb);
        fprintf(stderr,"(CST_cc=%.18lf) == 0.721794249067326322\n",cstcc);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in optimization.h file.\n");
        return 0;
    }

    // Check the preprocessor constants needed for triangulated integration
    boolean=(cstOne==0.225 && cstTwo==0.125939180544827153);
    boolean=(boolean && cstThree==0.132394152788506181);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(CST_ONE=%.18lf) == 0.225\n",cstOne);
        fprintf(stderr,"(CST_TWO=%.18lf) == 0.125939180544827153\n",cstTwo);
        fprintf(stderr,"(CST_THREE=%.18lf) == 0.132394152788506181\n",cstThree);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in optimization.h file.\n");
        return 0;
    }

    boolean=(cst1==0.333333333333333333 && cst2==0.101286507323456339);
    boolean=(boolean && cst3==0.470142064105115090);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(CST_1=%.18lf) == 0.333333333333333333\n",cst1);
        fprintf(stderr,"(CST_2=%.18lf) == 0.101286507323456339\n",cst2);
        fprintf(stderr,"(CST_3=%.18lf) == 0.470142064105115090\n",cst3);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in optimization.h file.\n");
        return 0;
    }

    boolean=(cst22==0.797426985353087322 && cst33==0.059715871789769820);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(CST_22=%.18lf) == 0.797426985353087322\n",cst22);
        fprintf(stderr,"(CST_33=%.18lf) == 0.059715871789769820\n",cst33);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in optimization.h file.\n");
        return 0;
    }

    boolean=(invPhi==0.618033988749894848 && invPhi2==0.381966011250105152);
    if (!boolean)
    {
        PRINT_ERROR("In checkAllPreprocessorConstants: expecting\n");
        fprintf(stderr,"(INV_PHI=%.18lf) == 0.618033988749894848\n",invPhi);
        fprintf(stderr,"(INV_PHI2=%.18lf) == 0.381966011250105152\n",invPhi2);
        fprintf(stderr,"Please modify the preprocessor constants accordingly ");
        fprintf(stderr,"in optimization.h file.\n");
        return 0;
    }

    if (verbose)
    {
        fprintf(stdout,"Preprocessor constants are ok. Setting default ");
        fprintf(stdout,"values for parameters.\n");
    }

    return 1;
}

/* ************************************************************************** */
// The function initialFileExists checks if a file exists at the location
// pointed by fileLocation, which must be a string of length less than
// nameLength. It has a char* and int variables as input arguments. It returns
// one if the file exists, minus one if it does not, otherwise zero is returned
// if an error is encountered
/* ********************* **************************************************** */
int initialFileExists(char* fileLocation, int nameLength)
{
    FILE *nameFile=NULL;

    // Warning: NULL pointer for fileLocation is not considered here as an error
    if (fileLocation==NULL)
    {
        return -1;    // The file simply does not exist at the given location
    }

    // Check if fileLocation variable is a string of length less than nameLength
    if (!checkStringFromLength(fileLocation,2,nameLength))
    {
        PRINT_ERROR("In initialFileExists: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the input (char*) variable ");
        fprintf(stderr,"fileLocation, which was supposed to store the name ");
        fprintf(stderr,"of a file, is not a string of length (strictly) less ");
        fprintf(stderr,"than %d.\n",nameLength);
        return 0;
    }

    // fopen function returns a FILE pointer, otherwise NULL (file must have
    // been previously created to get a non-NULL pointer in reading mode)
    nameFile=fopen(fileLocation,"r");
    if (nameFile==NULL)
    {
        return -1;
    }

    // fclose function returns zero if the input FILE* variable is successfully
    // closed, otherwise EOF (end-of-file) is returned
    if (fclose(nameFile))
    {
        PRINT_ERROR("In initialFileExists: could not close ");
        fprintf(stderr,"%s file properly.\n",fileLocation);
        nameFile=NULL;
        return 0;
    }
    nameFile=NULL;

    return 1;
}

/* ************************************************************************** */
// The function closeTheFile tries to properly close the FILE* variable via its
// associated pointer pFileToClose. It has the FILE** variable as input argument
// and it does not return any value (void output)
/* ************************************************************************** */
void closeTheFile(FILE** pFileToClose)
{
    if (pFileToClose!=NULL)
    {
        if (*pFileToClose!=NULL)
        {
            // fclose function returns zero if the input FILE* variable is
            // successfully closed, otherwise EOF (end-of-file) is returned
            if (fclose(*pFileToClose))
            {
                fprintf(stdout,"\nWarning in closeTheFile function: ");
                fprintf(stdout,"in addition, we were not able to close ");
                fprintf(stdout,"the file properly.\n");
            }
            else
            {
                fprintf(stdout,"Closing file.\n");
            }
            *pFileToClose=NULL;
        }
    }

    return;
}

/* ************************************************************************** */
// The function copyFileLocation tries to copy a file (using system function)
// located at fileLocation (a path name of length strictly less than nameLength)
// into the location specified at fileLocationForCopy (warning: the file at his
// location must not already exist). It has two char* variables (fileLocation,
// fileLocationForCopy) and an int nameLength variable as input arguments and
// it returns one on success, otherwise zero is returned if an error appears
/* ************************************************************************** */
int copyFileLocation(char* fileLocation, int nameLength,
                                                      char* fileLocationForCopy)
{
    size_t length=0;
    char *commandLine=NULL;

    // Check if the input fileLocation variable refers to a valid file
    if (initialFileExists(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In copyFileLocation: initialFileExists function ");
        fprintf(stderr,"did not return one, which was the expected value ");
        fprintf(stderr,"here, after having checked that the input (char*) ");
        fprintf(stderr,"fileLocation variable does not point to the valid ");
        fprintf(stderr,"path name of an existing file.\n");
        return 0;
    }

    // Check that the input fileLocationForCopy variable is pointing to NULL
    if (fileLocationForCopy==NULL)
    {
        PRINT_ERROR("In copyFileLocation: the input (char*) ");
        fprintf(stderr,"fileLocationForCopy variable is pointing to the ");
        fprintf(stderr,"%p adress.\n",(void*)fileLocationForCopy);
        return 0;
    }

    // Check the input fileLocationForCopy variable does not already refer to an
    // already-existing file (note that if the two input variables fileLocation
    // and fileLocationForCopy are identical, an error will be displayed here)
    if (initialFileExists(fileLocationForCopy,nameLength)!=-1)
    {
        PRINT_ERROR("In copyFileLocation: initialFileExists function ");
        fprintf(stderr,"did not return minus one, which was the expected ");
        fprintf(stderr,"value here, after having checked that the input ");
        fprintf(stderr,"(char*) fileLocationForCopy variable point to the ");
        fprintf(stderr,"name of an already existing file.\n");
        return 0;
    }

    // Dynamically allocate memory for the local commandLine variable
    length=2*nameLength+5;
    commandLine=(char*)calloc(length,sizeof(char));
    if (commandLine==NULL)
    {
        PRINT_ERROR("In copyFileLocation: could not allocate memory for the ");
        fprintf(stderr,"(local) char* commandLine variable.\n");
        return 0;
    }

    // Build the system line command "cp fileLocation fileLocationForCopy"
    // strncpy and strcat returns a pointer to the string (not used here)
    strncpy(commandLine,"cp ",length);
    strcat(commandLine,fileLocation);

    length=strlen(commandLine);
    commandLine[length]=' ';
    commandLine[length+1]='\0';
    strcat(commandLine,fileLocationForCopy);

    // system returns is -1 on error, otherwise the return status of the command
    fprintf(stdout,"\n%s\n",commandLine);
    if (system(commandLine))
    {
        PRINT_ERROR("In copyFileLocation: wrong return of the system ");
        fprintf(stderr,"(standard) c-function.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    // Free the dynamically allocated memory
    free(commandLine);
    commandLine=NULL;

    return 1;
}

/* ************************************************************************** */
// The function renameFileLocation tries to rename a file (using the system
// function) located at fileLocation (a path name of length strictly less than
// nameLength) into the name specified at newFileLocation (warning: the file at
// his location must not already exist). It has two char* variables
// (fileLocation, newFileLocation) and an int nameLength variable as input
// arguments and it returns one on success, otherwise zero if an error occurs
/* ************************************************************************** */
int renameFileLocation(char* fileLocation, int nameLength,
                                                          char* newFileLocation)
{
    size_t length=0;
    char *commandLine=NULL;

    // Check if the input fileLocation variable refers to a valid file
    if (initialFileExists(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In renameFileLocation: initialFileExists function ");
        fprintf(stderr,"did not return one, which was the expected value ");
        fprintf(stderr,"here, after having checked that the input (char*) ");
        fprintf(stderr,"fileLocation variable does not point to the valid ");
        fprintf(stderr,"path name of an existing file.\n");
        return 0;
    }

    // Check that the input newFileLocation variable is pointing to NULL
    if (newFileLocation==NULL)
    {
        PRINT_ERROR("In renameFileLocation: the input (char*) ");
        fprintf(stderr,"newFileLocation variable is pointing to the ");
        fprintf(stderr,"%p adress.\n",(void*)newFileLocation);
        return 0;
    }

    // Check the input newFileLocation variable does not already refer to an
    // already-existing file (note that if the two input variables fileLocation
    // and newFileLocation are identical, an error will be displayed here)
    if (initialFileExists(newFileLocation,nameLength)!=-1)
    {
        PRINT_ERROR("In renameFileLocation: initialFileExists function ");
        fprintf(stderr,"did not return minus one, which was the expected ");
        fprintf(stderr,"value here, after having checked that the input ");
        fprintf(stderr,"(char*) newFileLocation variable point to the name ");
        fprintf(stderr,"of an already existing file.\n");
        return 0;
    }

    // Dynamically allocate memory for the local commandLine variable
    length=2*nameLength+5;
    commandLine=(char*)calloc(length,sizeof(char));
    if (commandLine==NULL)
    {
        PRINT_ERROR("In renameFileLocation: could not allocate memory for ");
        fprintf(stderr,"the (local) char* commandLine variable.\n");
        return 0;
    }

    // Build the system line command "cp fileLocation newFileLocation"
    // strncpy and strcat returns a pointer to the string (not used here)
    strncpy(commandLine,"mv ",length);
    strcat(commandLine,fileLocation);

    length=strlen(commandLine);
    commandLine[length]=' ';
    commandLine[length+1]='\0';
    strcat(commandLine,newFileLocation);

    // system returns is -1 on error, otherwise the return status of the command
    if (commandLine[length-4]=='t')
    {
        fprintf(stdout,"\n%s\n",commandLine);
    }
    if (system(commandLine))
    {
        PRINT_ERROR("In renameFileLocation: wrong return of the system ");
        fprintf(stderr,"(standard) c-function.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    // Free the dynamically allocated memory
    free(commandLine);
    commandLine=NULL;

    return 1;
}

/* ************************************************************************** */
// The function plotMeshWithMeditSoftware tries to execute (thanks to the
// standard system c-function) the external medit software, which must have
// been previously installed and whose path is stored in the
// pParameters->path_medit variable, in order to vizualize a *.mesh file whose
// name is stored in pParameters->name_mesh (warning: the file at this location
// must already exist). It has the Parameters* (defined in main.h) as input
// argument and it returns one on success otherwise zero is returned
/* ************************************************************************** */
int plotMeshWithMeditSoftware(Parameters* pParameters)
{
    size_t length=0;
    char *commandLine=NULL;

    if (pParameters==NULL)
    {
        PRINT_ERROR("In plotMeshWithMeditSoftware: the input pParameters ");
        fprintf(stderr,"variable is pointing to the %p ",(void*)pParameters);
        fprintf(stderr,"address.\n");
        return 0;
    }

    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In plotMeshWithMeditSoftware: getMeshFormat function ");
        fprintf(stderr,"did not return one, which was the expected value ");
        fprintf(stderr,"here, after having checked that the ");
        fprintf(stderr,"pParameters->name_mesh variable is not storing the ");
        fprintf(stderr,"name of a valid and existing *.mesh file.\n");
        return 0;
    }

    if (!checkStringFromLength(pParameters->path_medit,2,
                                                      pParameters->path_length))
    {
        PRINT_ERROR("In plotMeshWithMeditSoftware: checkStringFromLength ");
        fprintf(stderr,"function returned zero instead of one, after having ");
        fprintf(stderr,"checked that the pParameters->path_medit variable ");
        fprintf(stderr,"is not a string of (positive) length (strictly) ");
        fprintf(stderr,"less than %d.\n",pParameters->path_length);
        return 0;
    }

    // Dynamically allocate memory for the local commandLine variable
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=pParameters->name_length+pParameters->path_length+30;
    commandLine=(char*)calloc(length,sizeof(char));
    if (commandLine==NULL)
    {
        PRINT_ERROR("In plotMeshWithMeditSoftware: could not allocate memory ");
        fprintf(stderr,"for the (local) char* commandLine variable.\n");
        return 0;
    }

    // Build the line command pParameters->path_medit + pParameters->name_mesh
    // strlen returns the length of the string, but not including the char '\0'
    // strncpy and strcat returns a pointer to the string (not used here)
    strncpy(commandLine,pParameters->path_medit,length);
    length=strlen(commandLine);
    commandLine[length]=' ';
    commandLine[length+1]='\0';
    strcat(commandLine,pParameters->name_mesh);

    if (!pParameters->verbose)
    {
        strcat(commandLine," >meditOutput.txt 2>&1");
    }

    // system returns is -1 on error, otherwise the return status of the command
    fprintf(stdout,"\n%s\n",commandLine);
    if (system(commandLine))
    {
        PRINT_ERROR("In plotMeshWithMeditSoftware: wrong return of the ");
        fprintf(stderr,"system (standard) c-function.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    if (!pParameters->verbose)
    {
        // remove returns 0 on success, otherwise -1
        if (remove("meditOutput.txt"))
        {
            PRINT_ERROR("In plotMeshWithMeditSoftware: wrong return (=-1) of ");
            fprintf(stderr,"the standard remove c-function in the attempt of ");
            fprintf(stderr,"removing the meditOutput.txt file.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
    }

    // Free the memory allocated for commandLine
    free(commandLine);
    commandLine=NULL;

    return 1;
}

/* ************************************************************************** */
// The function adaptMeshWithMmg3dSoftware tries to execute (thanks to the
// standard system c-function) the external mmg3d software, which must have
// been previously installed and whose path is stored in the
// pParameters->path_mmg3d variable, in order to adapt the mesh according to the
// adaptMode and the parameters stored in the structure pointed by pParameters.
// It has the Parameters* (defined in main.h) and the char[4] adaptMode as
// input arguments and it returns one on success otherwise zero is returned
/* ************************************************************************** */
int adaptMeshWithMmg3dSoftware(Parameters* pParameters, char adaptMode[4])
{
    size_t length=0;
    char *commandLine=NULL, options[120]={'\0'};

    if (pParameters==NULL)
    {
        PRINT_ERROR("In adaptMeshWithMmg3dSoftware: the input pParameters ");
        fprintf(stderr,"variable is pointing to the %p ",(void*)pParameters);
        fprintf(stderr,"address.\n");
        return 0;
    }

    if (!checkStringFromLength(adaptMode,2,4))
    {
        PRINT_ERROR("In adaptMeshWithMmg3dSoftware: checkStringFromLength ");
        fprintf(stderr,"function returned zero instead of one, after having ");
        fprintf(stderr,"checked that the input adaptMode variable is not a");
        fprintf(stderr,"is not a string of (positive) length (strictly) ");
        fprintf(stderr,"less than four.\n");
        return 0;
    }

    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In adaptMeshWithMmg3dSoftware: getMeshFormat function ");
        fprintf(stderr,"did not return one, which was the expected value ");
        fprintf(stderr,"here, after having checked that the ");
        fprintf(stderr,"pParameters->name_mesh variable is not storing the ");
        fprintf(stderr,"name of a valid and existing *.mesh file.\n");
        return 0;
    }

    if (!checkStringFromLength(pParameters->path_mmg3d,2,
                                                      pParameters->path_length))
    {
        PRINT_ERROR("In adaptMeshWithMmg3dSoftware: checkStringFromLength ");
        fprintf(stderr,"function returned zero instead of one, after having ");
        fprintf(stderr,"checked that the pParameters->path_mmg3d variable ");
        fprintf(stderr,"is not of string of (positive) length (strictly) ");
        fprintf(stderr,"less than %d.\n",pParameters->path_length);
        return 0;
    }

    // Dynamically allocate memory for the local commandLine variable
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=3*pParameters->name_length+pParameters->path_length+190;
    commandLine=(char*)calloc(length,sizeof(char));
    if (commandLine==NULL)
    {
        PRINT_ERROR("In adaptMeshWithMmg3dSoftware: could not allocate ");
        fprintf(stderr,"memory for the (local) char* commandLine variable.\n");
        return 0;
    }

    // Build the line command for the mmg3d software, depending on adaptMode
    // strlen returns the length of the string, but not including the char '\0'
    // strncpy and strcat returns a pointer to the string (not used here)
    strncpy(commandLine,pParameters->path_mmg3d,length);
    strcat(commandLine," -in ");
    strcat(commandLine,pParameters->name_mesh);

    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (!strcmp(adaptMode,"iso"))
    {
        strcat(commandLine," -out ");
        strcat(commandLine,pParameters->name_mesh);

        // sprintf returns the total number of characters written is returned
        // not including the char '\0', otherwise a negative number on failure
        if (sprintf(options," -hmin %lf -hmax %lf -hausd %lf -hgrad %lf",
                                                      pParameters->hmin_iso,
                                                      pParameters->hmax_iso,
                                                      pParameters->hausd_iso,
                                                      pParameters->hgrad_iso)<0)
        {
            PRINT_ERROR("In adaptMeshWithMmg3dSoftware: the standard sprintf ");
            fprintf(stderr,"c-function returned an unexpected negative ");
            fprintf(stderr,"value, while attempting to build the mmg3d ");
            fprintf(stderr,"command line for an isotropic mesh adaptation.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
        strcat(commandLine,options);
    }
    else if (!strcmp(adaptMode,"met"))
    {
        strcat(commandLine," -sol ");
        strcat(commandLine,pParameters->name_mesh);

        length=strlen(commandLine);
        commandLine[length-5]='\0';
        strcat(commandLine,".sol -out ");
        strcat(commandLine,pParameters->name_mesh);

        if (sprintf(options," -hmin %lf -hmax %lf -hausd %lf -hgrad %lf",
                                                      pParameters->hmin_met,
                                                      pParameters->hmax_met,
                                                      pParameters->hausd_met,
                                                      pParameters->hgrad_met)<0)
        {
            PRINT_ERROR("In adaptMeshWithMmg3dSoftware: the standard sprintf ");
            fprintf(stderr,"c-function returned an unexpected negative ");
            fprintf(stderr,"value, while attempting to build the mmg3d ");
            fprintf(stderr,"command line for for a metric mesh adaptation.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
        strcat(commandLine,options);
    }
    else if (!strcmp(adaptMode,"ls"))
    {
        strcat(commandLine," -ls -sol ");
        strcat(commandLine,pParameters->name_mesh);

        length=strlen(commandLine);
        commandLine[length-5]='\0';
        strcat(commandLine,".sol -out ");
        strcat(commandLine,pParameters->name_mesh);

        if (sprintf(options," -nr -hmin %lf -hmax %lf -hausd %lf -hgrad %lf",
                                                       pParameters->hmin_ls,
                                                       pParameters->hmax_ls,
                                                       pParameters->hausd_ls,
                                                       pParameters->hgrad_ls)<0)
        {
            PRINT_ERROR("In adaptMeshWithMmg3dSoftware: the standard sprintf ");
            fprintf(stderr,"c-function returned an unexpected negative ");
            fprintf(stderr,"value, while attempting to build the mmg3d ");
            fprintf(stderr,"command line for a level-set mesh adaptation.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
        strcat(commandLine,options);
    }
    else if (!strcmp(adaptMode,"lag"))
    {
        if (sprintf(options," -lag %d",pParameters->hmode_lag)<0)
        {
            PRINT_ERROR("In adaptMeshWithMmg3dSoftware: the standard sprintf ");
            fprintf(stderr,"c-function returned an unexpected negative ");
            fprintf(stderr,"value, while attempting to build the mmg3d ");
            fprintf(stderr,"command line for a Lagrangian mesh adaptation.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
        strcat(commandLine,options);

        strcat(commandLine," -sol ");
        strcat(commandLine,pParameters->name_mesh);

        length=strlen(commandLine);
        commandLine[length-5]='\0';
        strcat(commandLine,".sol -out ");
        strcat(commandLine,pParameters->name_mesh);

        if (sprintf(options," -nr -hmin %lf -hmax %lf -hausd %lf -hgrad %lf",
                                                      pParameters->hmin_lag,
                                                      pParameters->hmax_lag,
                                                      pParameters->hausd_lag,
                                                      pParameters->hgrad_lag)<0)
        {
            PRINT_ERROR("In adaptMeshWithMmg3dSoftware: the standard sprintf ");
            fprintf(stderr,"c-function returned an unexpected negative ");
            fprintf(stderr,"value, while attempting to build the mmg3d ");
            fprintf(stderr,"command line for a Lagrangian mesh adaptation.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
        strcat(commandLine,options);
    }
    else
    {
        PRINT_ERROR("In adaptMeshWithMmg3dSoftware: the input adaptMode ");
        fprintf(stderr,"variable (=%s) can only be set to 'iso', ",adaptMode);
        fprintf(stderr,"'met', 'ls' or 'lag'.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    if (pParameters->opt_mode!=1)
    {
        if (pParameters->verbose)
        {
            strcat(commandLine," -v 1"); // Error detected mmg3d_O3 -v 10 or not
        }
        else
        {
            strcat(commandLine," >mmg3dOutput.txt 2>&1");
        }
    }
    else
    {
        strcat(commandLine," >mmg3dOutput.txt 2>&1");
    }

    // system returns is -1 on error, otherwise the return status of the command
    if (pParameters->opt_mode!=1)
    {
        fprintf(stdout,"\n%s\n",commandLine);
    }
    else
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\n%s\n",commandLine);
        }
        else
        {
            if (!strcmp(adaptMode,"ls"))
            {
                fprintf(stdout,"55 %% done.\n");
            }
            else if (!strcmp(adaptMode,"lag"))
            {
                fprintf(stdout,"10 %% done.\n");
            }
            else
            {
                 fprintf(stdout,"\n%s\n",commandLine);
            }
        }
    }
    if (system(commandLine))
    {
        PRINT_ERROR("In adaptMeshWithMmg3dSoftware: wrong return of the ");
        fprintf(stderr,"system (standard) c-function.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    if (!pParameters->verbose || pParameters->opt_mode==1)
    {
        // remove returns 0 on success, otherwise -1
        if (remove("mmg3dOutput.txt"))
        {
            PRINT_ERROR("In adaptMeshWithMmg3dSoftware: wrong return (=-1) ");
            fprintf(stderr,"of the standard remove c-function in the attempt ");
            fprintf(stderr,"of removing the mmg3dOutput.txt file.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
    }

    // Free the memory allocated for commandLine
    free(commandLine);
    commandLine=NULL;

    return 1;
}

/* ************************************************************************** */
// The function renormalizeWithMshdistSoftware tries to execute (thanks to the
// standard system c-function) the external mshdist software, which must have
// been previously installed and whose path is stored in the
// pParameters->path_mshdist variable, in order to adapt the mesh according to
// the mode and the parameters stored in the structure pointed by pParameters.
// It has the Parameters* (defined in main.h) and the char[4] mode as
// input arguments and it returns one on success otherwise zero is returned
/* ************************************************************************** */
int renormalizeWithMshdistSoftware(Parameters* pParameters, char mode[4])
{
    size_t length=0;
    char *commandLine=NULL, options[80]={'\0'};

    if (pParameters==NULL)
    {
        PRINT_ERROR("In renormalizeWithMshdistSoftware: the input ");
        fprintf(stderr,"pParameters variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pParameters);
        return 0;
    }

    if (!checkStringFromLength(mode,4,4))
    {
        PRINT_ERROR("In renormalizeWithMshdistSoftware: ");
        fprintf(stderr,"checkStringFromLength function returned zero instead ");
        fprintf(stderr,"of one, after having checked that the input mode ");
        fprintf(stderr," variable is not a string made of three characters.\n");
        return 0;
    }

    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In renormalizeWithMshdistSoftware: getMeshFormat ");
        fprintf(stderr,"function did not return one, which was the expected ");
        fprintf(stderr,"value here, after having checked that the ");
        fprintf(stderr,"pParameters->name_mesh variable is not storing the ");
        fprintf(stderr,"name of a valid and existing *.mesh file.\n");
        return 0;
    }

    if (!checkStringFromLength(pParameters->path_mshdist,2,
                                                      pParameters->path_length))
    {
        PRINT_ERROR("In renormalizeWithMshdistSoftware: ");
        fprintf(stderr,"checkStringFromLength function returned zero instead ");
        fprintf(stderr,"of one, after having checked that the ");
        fprintf(stderr,"pParameters->path_mshdist variable is not of string ");
        fprintf(stderr,"of (positive) length (strictly) less than ");
        fprintf(stderr,"%d.\n",pParameters->path_length);
        return 0;
    }

    // Dynamically allocate memory for the local commandLine variable
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=pParameters->name_length+pParameters->path_length+130;
    commandLine=(char*)calloc(length,sizeof(char));
    if (commandLine==NULL)
    {
        PRINT_ERROR("In renormalizeWithMshdistSoftware: could not allocate ");
        fprintf(stderr,"memory for the (local) char* commandLine variable.\n");
        return 0;
    }

    // Build the line command for the mshdist software, depending on adaptMode
    // strlen returns the length of the string, but not including the char '\0'
    // strncpy and strcat returns a pointer to the string (not used here)
    strncpy(commandLine,pParameters->path_mshdist,length);
    length=strlen(commandLine);
    commandLine[length]=' ';
    commandLine[length+1]='\0';
    strcat(commandLine,pParameters->name_mesh);

    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (!strcmp(mode,"dom"))
    {
        strcat(commandLine," -dom ");
    }
    else if (!strcmp(mode,"sol"))
    {
        length=strlen(commandLine);
        commandLine[length]=' ';
        commandLine[length+1]='\0';
    }
    else
    {
        PRINT_ERROR("In renormalizeWithMshdistSoftware: the input mode ");
        fprintf(stderr,"variable (=%s) can only be set to 'sol' or ",mode);
        fprintf(stderr,"'dom'.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    if (pParameters->n_iter>0)
    {
        if (pParameters->residual>0.)
        {
            // sprintf returns the total number of char written is returned
            // not including \0', otherwise a negative number on failure
            if (sprintf(options,"-ncpu %d -it %d -r %lf",pParameters->n_cpu,
                                   pParameters->n_iter,pParameters->residual)<0)
            {
                PRINT_ERROR("In renormalizeWithMshdistSoftware: the standard ");
                fprintf(stderr,"sprintf c-function returned an unexpected ");
                fprintf(stderr,"negative value, while attempting to build ");
                fprintf(stderr,"the mshdist command line for a ");
                fprintf(stderr,"renormalization process.\n");
                free(commandLine);
                commandLine=NULL;
                return 0;
            }
        }
        else
        {
            if (sprintf(options,"-ncpu %d -it %d",pParameters->n_cpu,
                                                         pParameters->n_iter)<0)
            {
                PRINT_ERROR("In renormalizeWithMshdistSoftware: the standard ");
                fprintf(stderr,"sprintf c-function returned an unexpected ");
                fprintf(stderr,"negative value, while attempting to build ");
                fprintf(stderr,"the mshdist command line for a ");
                fprintf(stderr,"renormalization process.\n");
                free(commandLine);
                commandLine=NULL;
                return 0;
            }
        }
    }
    else
    {
        if (pParameters->residual>0.)
        {
            if (sprintf(options,"-ncpu %d -r %lf",pParameters->n_cpu,
                                                       pParameters->residual)<0)
            {
                PRINT_ERROR("In renormalizeWithMshdistSoftware: the standard ");
                fprintf(stderr,"sprintf c-function returned an unexpected ");
                fprintf(stderr,"negative value, while attempting to build ");
                fprintf(stderr,"the mshdist command line for a ");
                fprintf(stderr,"renormalization process.\n");
                free(commandLine);
                commandLine=NULL;
                return 0;
            }
        }
        else
        {
            if (sprintf(options,"-ncpu %d",pParameters->n_cpu)<0)
            {
                PRINT_ERROR("In renormalizeWithMshdistSoftware: the standard ");
                fprintf(stderr,"sprintf c-function returned an unexpected ");
                fprintf(stderr,"negative value, while attempting to build ");
                fprintf(stderr,"the mshdist command line for a ");
                fprintf(stderr,"renormalization process.\n");
                free(commandLine);
                commandLine=NULL;
                return 0;
            }
        }
    }
    strcat(commandLine,options);

    if (pParameters->opt_mode!=1)
    {
        if (pParameters->verbose)
        {
            strcat(commandLine," -v 10");
        }
        else
        {
            strcat(commandLine," >mshdistOutput.txt 2>&1");
        }
    }
    else
    {
        strcat(commandLine," -v 10 >mshdistOutput.txt 2>&1");
    }

    // system returns is -1 on error, otherwise the return status of the command
    if (pParameters->opt_mode!=1)
    {
        fprintf(stdout,"\n%s\n",commandLine);
    }
    else
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\n%s\n",commandLine);
        }
        else
        {
            if (pParameters->n_iter!=10)
            {
                fprintf(stdout,"10 %% done.\n");
            }    
        }
    }

    if (system(commandLine))
    {
        PRINT_ERROR("In renormalizeWithMshdistSoftware: wrong return of the ");
        fprintf(stderr,"system (standard) c-function.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    if (!pParameters->verbose || pParameters->opt_mode==1)
    {
        // remove returns 0 on success, otherwise -1
        if (remove("mshdistOutput.txt"))
        {
            PRINT_ERROR("In renormalizeWithMshdistSoftware: wrong return ");
            fprintf(stderr,"(=-1) of the standard remove c-function in the ");
            fprintf(stderr,"attempt of removing the mshdistOutput.txt file.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
    }

    // Free the memory allocated for commandLine
    free(commandLine);
    commandLine=NULL;

    return 1;
}

/* ************************************************************************** */
// The function extendShapeGradientWithElasticSoftware tries to execute (thanks
// to the standard system c-function) the external elastic software, which must
// have been previously installed and whose path is stored in the
// pParameters->path_elastic variable, in order to extend the shape gradient
// by solving the elasticity equations with dirichlet boundary on the boundary
// of the internal domain tand the parameters stored in the structure pointed
// by pParameters. It has the Parameters* (defined in main.h) as input arguments
// and it returns one on success otherwise zero is returned in case of error
/* ************************************************************************** */
int extendShapeGradientWithElasticSoftware(Parameters* pParameters)
{
    size_t length=0;
    char *commandLine=NULL;

    if (pParameters==NULL)
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: the input ");
        fprintf(stderr,"pParameters variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pParameters);
        return 0;
    }

    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: ");
        fprintf(stderr,"getMeshFormat function did not return one, which was ");
        fprintf(stderr,"the expected value here, after having checked that ");
        fprintf(stderr,"the pParameters->name_mesh variable is not storing ");
        fprintf(stderr,"the name of a valid and existing *.mesh file.\n");
        return 0;
    }

    if (initialFileExists(pParameters->name_elas,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: ");
        fprintf(stderr,"initialFileExists function did not return one, which ");
        fprintf(stderr,"was the expected value here, after having checked ");
        fprintf(stderr,"that the pParameters->name_elas variable is not ");
        fprintf(stderr,"storing the name of a valid and existing *.elas ");
        fprintf(stderr,"file.\n");
        return 0;
    }

    // strlen returns the length of the string, but not including the char '\0'
    length=strlen(pParameters->name_elas);
    if (length<6)
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: ");
        fprintf(stderr,"%s file name should at least ",pParameters->name_elas);
        fprintf(stderr,"contain six characters instead of %ld (in ",length-1);
        fprintf(stderr,"order to end with something more than the *.elas ");
        fprintf(stderr,"extension).\n");
        return 0;
    }

    if (pParameters->name_elas[length-5]!='.' ||
                                        pParameters->name_elas[length-4]!='e' ||
                                        pParameters->name_elas[length-3]!='l' ||
                                        pParameters->name_elas[length-2]!='a' ||
                                        pParameters->name_elas[length-1]!='s' ||
                                           pParameters->name_elas[length]!='\0')
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: the ");
        fprintf(stderr,"name_elas variable (=%s) of ",pParameters->name_elas);
        fprintf(stderr,"the structure pointed by pParameters does not end ");
        fprintf(stderr,"with the '.elas' extension.\n");
        return 0;
    }

    if (!checkStringFromLength(pParameters->path_elastic,2,
                                                      pParameters->path_length))
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: ");
        fprintf(stderr,"checkStringFromLength function returned zero instead ");
        fprintf(stderr,"of one, after having checked that the ");
        fprintf(stderr,"pParameters->path_elastic variable is not of string ");
        fprintf(stderr,"of (positive) length (strictly) less than ");
        fprintf(stderr,"%d.\n",pParameters->path_length);
        return 0;
    }

    // Dynamically allocate memory for commandLine and options variables
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=4*pParameters->name_length+pParameters->path_length+60;
    commandLine=(char*)calloc(length,sizeof(char));
    if (commandLine==NULL)
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: could not ");
        fprintf(stderr,"allocate memory for the (local) char* commandLine ");
        fprintf(stderr,"variable.\n");
        return 0;
    }

    // Build the line command for the mshdist software, depending on adaptMode
    // strncpy and strcat returns a pointer to the string (not used here)
    strncpy(commandLine,pParameters->path_elastic,length);
    length=strlen(commandLine);
    commandLine[length]=' ';
    commandLine[length+1]='\0';
    strcat(commandLine,pParameters->name_mesh);

    strcat(commandLine," -p ");
    strcat(commandLine,pParameters->name_elas);

    strcat(commandLine," -s ");
    strcat(commandLine,pParameters->name_mesh);
    length=strlen(commandLine);
    commandLine[length-5]='\0';
    strcat(commandLine,".sol -o ");
    strcat(commandLine,pParameters->name_mesh);

    length=strlen(commandLine);
    commandLine[length-5]='\0';
    strcat(commandLine,".sol");

    if (pParameters->opt_mode!=1)
    {
        if (pParameters->verbose)
        {
            strcat(commandLine," +v");
        }
        else
        {
            strcat(commandLine," >elasticOutput.txt 2>&1");
        }
    }
    else
    {
        strcat(commandLine," +v >elasticOutput.txt 2>&1");
    }

    // system returns is -1 on error, otherwise the return status of the command
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"\n%s\n",commandLine);
    }
    if (system(commandLine))
    {
        PRINT_ERROR("In extendShapeGradientWithElasticSoftware: wrong ");
        fprintf(stderr,"return of the system (standard) c-function.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    if (!pParameters->verbose || pParameters->opt_mode==1)
    {
        // remove returns 0 on success, otherwise -1
        if (remove("elasticOutput.txt"))
        {
            PRINT_ERROR("In extendShapeGradientWithElasticSoftware: wrong ");
            fprintf(stderr,"return (=-1) of the standard remove c-function ");
            fprintf(stderr,"in the attempt of removing the elasticOutput.txt ");
            fprintf(stderr,"file.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
    }

    // Free the memory allocated for commandLine
    free(commandLine);
    commandLine=NULL;

    return 1;
}

/* ************************************************************************** */
// The function advectLevelSetWithAdvectSoftware tries to execute (thanks to
// the standard system c-function) the external advect software, which must
// have been previously installed and whose path is stored in the
// pParameters->path_advect variable, in order to advect the level-set function
// on the mesh according to the parameters stored in the structure pointed by
// pParameters. It has the Parameters* (defined in main.h) as input arguments
// and it returns one on success otherwise zero is returned
/* ************************************************************************** */
int advectLevelSetWithAdvectSoftware(Parameters* pParameters)
{
    size_t length=0;
    char *commandLine=NULL;
    char options[30]={'0'};

    if (pParameters==NULL)
    {
        PRINT_ERROR("In advectLevelSetWithAdvectSoftware: the input ");
        fprintf(stderr,"pParameters variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pParameters);
        return 0;
    }

    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In advectLevelSetWithAdvectSoftware: getMeshFormat ");
        fprintf(stderr,"function did not return one, which was the expected ");
        fprintf(stderr,"value here, after having checked that the ");
        fprintf(stderr,"pParameters->name_mesh variable is not storing the ");
        fprintf(stderr,"name of a valid and existing *.mesh file.\n");
        return 0;
    }

    if (!checkStringFromLength(pParameters->path_advect,2,
                                                      pParameters->path_length))
    {
        PRINT_ERROR("In advectLevelSetWithAdvectSoftware: ");
        fprintf(stderr,"checkStringFromLength function returned zero instead ");
        fprintf(stderr,"of one, after having checked that the ");
        fprintf(stderr,"pParameters->path_advect variable is not of string ");
        fprintf(stderr,"of (positive) length (strictly) less than ");
        fprintf(stderr,"%d.\n",pParameters->path_length);
        return 0;
    }

    // Dynamically allocate memory for char* variables
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    // strncpy returns a pointer to the string (not used here)
    length=4*pParameters->name_length+pParameters->path_length+100;
    commandLine=(char*)calloc(length,sizeof(char));
    if (commandLine==NULL)
    {
        PRINT_ERROR("In advectLevelSetWithAdvectSoftware: could not ");
        fprintf(stderr,"allocate memory for the (local) char* commandLine ");
        fprintf(stderr,"variable.\n");
        return 0;
    }

    // Build the line command for the advect software
    // strncpy and strcat returns a pointer to the string (not used here)
    strncpy(commandLine,pParameters->path_advect,length);
    length=strlen(commandLine);
    commandLine[length]=' ';
    commandLine[length+1]='\0';
    strcat(commandLine,pParameters->name_mesh);

    strcat(commandLine," -c ");
    strcat(commandLine,pParameters->name_mesh);

    length=strlen(commandLine);
    commandLine[length-5]='\0';
    strcat(commandLine,".chi.sol -s ");
    strcat(commandLine,pParameters->name_mesh);

    length=strlen(commandLine);
    commandLine[length-5]='\0';
    strcat(commandLine,".sol -o ");
    strcat(commandLine,pParameters->name_mesh);

    length=strlen(commandLine);
    commandLine[length-5]='\0';
    strcat(commandLine,".sol");

    // Add or not the advection step
    if (pParameters->delta_t>0.)
    {
        // sprintf returns the total number of characters written is returned
        // not including the char '\0', otherwise a negative number on failure
        if (sprintf(options," -dt %lf",pParameters->delta_t)<0)
        {
            PRINT_ERROR("In advectLevelSetWithAdvectSoftware: the standard ");
            fprintf(stderr,"sprintf c-function returned an unexpected ");
            fprintf(stderr,"negative value, while attempting to build the ");
            fprintf(stderr,"advect command line for advecting the level-set ");
            fprintf(stderr,"function.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
        strcat(commandLine,options);
    }

    // Add or not the nocfl condition
    if (pParameters->no_cfl)
    {
        strcat(commandLine," -nocfl");
    }

    // Choose the verbose mode
    if (pParameters->opt_mode!=1)
    {
        if (pParameters->verbose)
        {
            strcat(commandLine," +v");
        }
        else
        {
            strcat(commandLine," >advectOutput.txt 2>&1");
        }
    }
    else
    {
        strcat(commandLine," +v >advectOutput.txt 2>&1");
    }

    // system returns is -1 on error, otherwise the return status of the command
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"\n%s\n",commandLine);
    }
    if (system(commandLine))
    {
        PRINT_ERROR("In advectLevelSetWithAdvectSoftware: wrong return of ");
        fprintf(stderr,"the system (standard) c-function.\n");
        free(commandLine);
        commandLine=NULL;
        return 0;
    }

    if (!pParameters->verbose || pParameters->opt_mode==1)
    {
        // remove returns 0 on success, otherwise -1
        if (remove("advectOutput.txt"))
        {
            PRINT_ERROR("In extendShapeGradientWithElasticSoftware: wrong ");
            fprintf(stderr,"return (=-1) of the standard remove c-function ");
            fprintf(stderr,"in the attempt of removing the advectOutput.txt ");
            fprintf(stderr,"file.\n");
            free(commandLine);
            commandLine=NULL;
            return 0;
        }
    }

    // Free the memory allocated for commandLine
    free(commandLine);
    commandLine=NULL;

    return 1;
}


