/**
* \file testLoadParameters.c
* \brief All unit testing functions related to loadParameters.c file.
* \author Jeremy DALPHIN
* \version 1.1a
* \date August 1st, 2018
*
* This file contains all the unit testing functions that were built to test
* the functions defined in the loadParameters.c file. We use the convention that
* if a function is called myFunction for example, its associated unit-test
* function is called testMyFunction. All the functions defined in this file
* does not have any input argument and does not return any argument (void input
* and output).
*/

#include "test.h"
#include "loadParameters.h"

// Unit (random) tests on initializeParameters of loadParameters.c file
void testInitializeParameterStructure(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthName=0;
    int iRandom=0, i=0, j=0, jMax=0, boolean=0;
    Parameters *pParameters=NULL, parameters;

    char pStringToPrint[10][13]={"name_info","name_data","name_chem",
                                 "name_mesh","name_elas","path_medit",
                                 "path_mmg3d","path_mshdist","path_elastic",
                                                                 "path_advect"};
    char **pString[10]={&parameters.name_info,&parameters.name_data,
                        &parameters.name_chem,&parameters.name_mesh,
                        &parameters.name_elas,&parameters.path_medit,
                        &parameters.path_mmg3d,&parameters.path_mshdist,
                              &parameters.path_elastic,&parameters.path_advect};
    char pIntegerToPrint[23][13]={"name_length","path_length","opt_mode",
                                  "verbose","n_cpu","nu_electrons","nu_spin",
                                  "orb_rhf","n_x","n_y","n_z","ls_type",
                                  "trick_matrix","approx_mode","iter_max",
                                  "save_type","save_mesh","save_data",
                                  "save_print","save_where","hmode_lag",
                                                             "n_iter","no_cfl"};
    int *pInteger[23]={&parameters.name_length,&parameters.path_length,
                       &parameters.opt_mode,&parameters.verbose,
                       &parameters.n_cpu,&parameters.nu_electrons,
                       &parameters.nu_spin,&parameters.orb_rhf,&parameters.n_x,
                       &parameters.n_y,&parameters.n_z,&parameters.ls_type,
                       &parameters.trick_matrix,&parameters.approx_mode,
                       &parameters.iter_max,&parameters.save_type,
                       &parameters.save_mesh,&parameters.save_data,
                       &parameters.save_print,&parameters.save_where,
                       &parameters.hmode_lag,&parameters.n_iter,
                                                            &parameters.no_cfl};
    char pDoubleToPrint[37][12]={"x_min","y_min","z_min","x_max","y_max",
                                 "z_max","delta_x","delta_y","delta_z","ls_x",
                                 "ls_y","ls_z","ls_r","met_err","met_min",
                                 "met_max","iter_told0p","iter_told1p",
                                 "iter_told2p","hmin_iso","hmax_iso",
                                 "hausd_iso","hgrad_iso","hmin_met","hmax_met",
                                 "hausd_met","hgrad_met","hmin_ls","hmax_ls",
                                 "hausd_ls","hgrad_ls","hmin_lag","hmax_lag",
                                  "hausd_lag","hgrad_lag","residual","delta_t"};
    double *pDouble[37]={&parameters.x_min,&parameters.y_min,&parameters.z_min,
                         &parameters.x_max,&parameters.y_max,&parameters.z_max,
                         &parameters.delta_x,&parameters.delta_y,
                         &parameters.delta_z,&parameters.ls_x,&parameters.ls_y,
                         &parameters.ls_z,&parameters.ls_r,&parameters.met_err,
                         &parameters.met_min,&parameters.met_max,
                         &parameters.iter_told0p,&parameters.iter_told1p,
                         &parameters.iter_told2p,&parameters.hmin_iso,
                         &parameters.hmax_iso,&parameters.hausd_iso,
                         &parameters.hgrad_iso,&parameters.hmin_met,
                         &parameters.hmax_met,&parameters.hausd_met,
                         &parameters.hgrad_met,&parameters.hmin_ls,
                         &parameters.hmax_ls,&parameters.hausd_ls,
                         &parameters.hgrad_ls,&parameters.hmin_lag,
                         &parameters.hmax_lag,&parameters.hausd_lag,
                         &parameters.hgrad_lag,&parameters.residual,
                                                           &parameters.delta_t};

    // Initializing to zero the structure of parameters
    for (i=0; i<10; i++)
    {
        *pString[i]=NULL;
    }
    for (i=0; i<23; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<37; i++)
    {
        *pDouble[i]=.0;
    }

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting initializeParameterStructure function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    initializeParameterStructure(pParameters);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pParameters=&parameters;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    initializeParameterStructure(pParameters);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables
        for (i=0; i<10; i++)
        {
            lengthName=rand()%120+1;
            if (lengthName>100)
            {
                *pString[i]=NULL;
            }
            else
            {
                *pString[i]=(char*)calloc(lengthName,sizeof(char));
            }
            if (*pString[i]!=NULL)
            {
                jMax=lengthName;
                for (j=0; j<jMax; j++)
                {
                    (*pString[i])[j]=rand()%90+33;
                }
                (*pString[i])[rand()%lengthName]='\0';
            }
        }
        for (i=0; i<23; i++)
        {
            *pInteger[i]=rand()%121-20;
        }
        for (i=0; i<37; i++)
        {
            *pDouble[i]=(double)(rand()%6001-3000)/1000.;
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the values of the different variables
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%s\n",pStringToPrint[i],*pString[i]);
        }
        for (i=0; i<23; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<37; i++)
        {
            fprintf(stdout,"%s=%lf\n",pDoubleToPrint[i],*pDouble[i]);
        }

        // Free the memory allocated for strings
        for (i=0; i<10; i++)
        {
            free(*pString[i]);
        }

        // Testing the function
        initializeParameterStructure(pParameters);

        // Checking if the initialization worked
        boolean=0;
        for (i=0; i<10; i++)
        {
            boolean=(boolean || *pString[i]!=NULL);
        }
        for (i=0; i<23; i++)
        {
            boolean=(boolean || *pInteger[i]!=0);
        }
        for (i=0; i<37; i++)
        {
            boolean=(boolean || *pDouble[i]!=.0);
        }

        returnValue=1;
        if (boolean)
        {
            returnValue=0;
            fprintf(stdout,"\nAlthough initializeParameterStructure function ");
            fprintf(stdout,"ended normally, the structure pointed by ");
            fprintf(stdout,"pParameters has not been initialized properly. ");
            fprintf(stdout,"One of the following variables is not set to ");
            fprintf(stdout,"zero (or NULL for pointers):\n");

            for (i=0; i<10; i++)
            {
                fprintf(stdout,"%s=%p\n",pStringToPrint[i],*pString[i]);
            }
            for (i=0; i<23; i++)
            {
                fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
            }
            for (i=0; i<37; i++)
            {
                fprintf(stdout,"%s=%lf\n",pDoubleToPrint[i],*pDouble[i]);
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

// Unit (random) tests on freeParameterMemory of loadParameters.c
void testFreeParameterMemory(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthName=0;
    int iRandom=0, i=0, j=0, jMax=0, boolean=0;
    Parameters *pParameters=NULL, parameters;

    char pStringToPrint[10][13]={"name_info","name_data","name_chem",
                                 "name_mesh","name_elas","path_medit",
                                 "path_mmg3d","path_mshdist","path_elastic",
                                                                 "path_advect"};
    char **pString[10]={&parameters.name_info,&parameters.name_data,
                        &parameters.name_chem,&parameters.name_mesh,
                        &parameters.name_elas,&parameters.path_medit,
                        &parameters.path_mmg3d,&parameters.path_mshdist,
                              &parameters.path_elastic,&parameters.path_advect};
    char pIntegerToPrint[23][13]={"name_length","path_length","opt_mode",
                                  "verbose","n_cpu","nu_electrons","nu_spin",
                                  "orb_rhf","n_x","n_y","n_z","ls_type",
                                  "trick_matrix","approx_mode","iter_max",
                                  "save_type","save_mesh","save_data",
                                  "save_print","save_where","hmode_lag",
                                                             "n_iter","no_cfl"};
    int *pInteger[23]={&parameters.name_length,&parameters.path_length,
                       &parameters.opt_mode,&parameters.verbose,
                       &parameters.n_cpu,&parameters.nu_electrons,
                       &parameters.nu_spin,&parameters.orb_rhf,&parameters.n_x,
                       &parameters.n_y,&parameters.n_z,&parameters.ls_type,
                       &parameters.trick_matrix,&parameters.approx_mode,
                       &parameters.iter_max,&parameters.save_type,
                       &parameters.save_mesh,&parameters.save_data,
                       &parameters.save_print,&parameters.save_where,
                       &parameters.hmode_lag,&parameters.n_iter,
                                                            &parameters.no_cfl};
    char pDoubleToPrint[37][12]={"x_min","y_min","z_min","x_max","y_max",
                                 "z_max","delta_x","delta_y","delta_z","ls_x",
                                 "ls_y","ls_z","ls_r","met_err","met_min",
                                 "met_max","iter_told0p","iter_told1p",
                                 "iter_told2p","hmin_iso","hmax_iso",
                                 "hausd_iso","hgrad_iso","hmin_met","hmax_met",
                                 "hausd_met","hgrad_met","hmin_ls","hmax_ls",
                                 "hausd_ls","hgrad_ls","hmin_lag","hmax_lag",
                                  "hausd_lag","hgrad_lag","residual","delta_t"};
    double *pDouble[37]={&parameters.x_min,&parameters.y_min,&parameters.z_min,
                         &parameters.x_max,&parameters.y_max,&parameters.z_max,
                         &parameters.delta_x,&parameters.delta_y,
                         &parameters.delta_z,&parameters.ls_x,&parameters.ls_y,
                         &parameters.ls_z,&parameters.ls_r,&parameters.met_err,
                         &parameters.met_min,&parameters.met_max,
                         &parameters.iter_told0p,&parameters.iter_told1p,
                         &parameters.iter_told2p,&parameters.hmin_iso,
                         &parameters.hmax_iso,&parameters.hausd_iso,
                         &parameters.hgrad_iso,&parameters.hmin_met,
                         &parameters.hmax_met,&parameters.hausd_met,
                         &parameters.hgrad_met,&parameters.hmin_ls,
                         &parameters.hmax_ls,&parameters.hausd_ls,
                         &parameters.hgrad_ls,&parameters.hmin_lag,
                         &parameters.hmax_lag,&parameters.hausd_lag,
                         &parameters.hgrad_lag,&parameters.residual,
                                                           &parameters.delta_t};

    // Initializing to zero the structure of parameters
    for (i=0; i<10; i++)
    {
        *pString[i]=NULL;
    }
    for (i=0; i<23; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<37; i++)
    {
        *pDouble[i]=.0;
    }

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting freeParameterMemory function.\n");

    expectedValue=1;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    freeParameterMemory(pParameters);
    returnValue=1;
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pParameters=&parameters;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    freeParameterMemory(pParameters);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables
        for (i=0; i<10; i++)
        {
            lengthName=rand()%120+1;
            if (lengthName>100)
            {
                *pString[i]=NULL;
            }
            else
            {
                *pString[i]=(char*)calloc(lengthName,sizeof(char));
            }
            if (*pString[i]!=NULL)
            {
                jMax=lengthName;
                for (j=0; j<jMax; j++)
                {
                    (*pString[i])[j]=rand()%90+33;
                }
                (*pString[i])[rand()%lengthName]='\0';
            }
        }
        for (i=0; i<23; i++)
        {
            *pInteger[i]=rand()%121-20;
        }
        for (i=0; i<37; i++)
        {
            *pDouble[i]=(double)(rand()%6001-3000)/1000.;
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the values of the different variables
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%s\n",pStringToPrint[i],*pString[i]);
        }
        for (i=0; i<23; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<37; i++)
        {
            fprintf(stdout,"%s=%lf\n",pDoubleToPrint[i],*pDouble[i]);
         }

         // Testing the function
         freeParameterMemory(pParameters);
         if (rand()%3)
         {
             freeParameterMemory(pParameters);
         }

         // Checking if it worked
         boolean=0;
         for (i=0; i<10; i++)
         {
             boolean=(boolean || *pString[i]!=NULL);
          }

          returnValue=1;
          if (boolean)
          {
              returnValue=0;
              fprintf(stdout,"\nAlthough freeParameterMemory function ended ");
              fprintf(stdout,"normally, the structure pointed by pParameters ");
              fprintf(stdout,"has not been freed up properly. One of the ");
              fprintf(stdout,"following pointers is not set to NULL:\n");

              for (i=0; i<10; i++)
              {
                  fprintf(stdout,"%s=%p\n",pStringToPrint[i],*pString[i]);
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

// Unit (random) tests on setupDefaultParameters of loadParameters.c
void testSetupDefaultParameters(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthName=0;
    char *nameInfo=NULL, nameTest[NAME_LENGTH+11]={'\0'};
    int iRandom=0, i=0, j=0, boolean=0, iLength=0;
    Parameters *pParameters=NULL, parameters;

    char pStringToPrint[10][13]={"name_info","name_data","name_chem",
                                 "name_mesh","name_elas","path_medit",
                                 "path_mmg3d","path_mshdist","path_elastic",
                                                                 "path_advect"};
    char **pString[10]={&parameters.name_info,&parameters.name_data,
                        &parameters.name_chem,&parameters.name_mesh,
                        &parameters.name_elas,&parameters.path_medit,
                        &parameters.path_mmg3d,&parameters.path_mshdist,
                              &parameters.path_elastic,&parameters.path_advect};
    char pPathReference[5][PATH_LENGTH]={PATH_MEDIT,PATH_MMG3D,PATH_MSHDIST,
                                                      PATH_ELASTIC,PATH_ADVECT};
    char *pStringReference[10]={nameTest,NULL,NULL,NULL,NULL,pPathReference[0],
                                pPathReference[1],pPathReference[2],
                                           pPathReference[3],pPathReference[4]};
    char pIntegerToPrint[23][13]={"name_length","path_length","opt_mode",
                                  "verbose","n_cpu","nu_electrons","nu_spin",
                                  "orb_rhf","n_x","n_y","n_z","ls_type",
                                  "trick_matrix","approx_mode","iter_max",
                                  "save_type","save_mesh","save_data",
                                  "save_print","save_where","hmode_lag",
                                                             "n_iter","no_cfl"};
    int *pInteger[23]={&parameters.name_length,&parameters.path_length,
                       &parameters.opt_mode,&parameters.verbose,
                       &parameters.n_cpu,&parameters.nu_electrons,
                       &parameters.nu_spin,&parameters.orb_rhf,&parameters.n_x,
                       &parameters.n_y,&parameters.n_z,&parameters.ls_type,
                       &parameters.trick_matrix,&parameters.approx_mode,
                       &parameters.iter_max,&parameters.save_type,
                       &parameters.save_mesh,&parameters.save_data,
                       &parameters.save_print,&parameters.save_where,
                       &parameters.hmode_lag,&parameters.n_iter,
                                                            &parameters.no_cfl};
    int pIntegerReference[23]={NAME_LENGTH,PATH_LENGTH,OPT_MODE,VERBOSE,N_CPU,0,
                              0,0,N_X,N_Y,N_Z,LS_TYPE,TRICK_MATRIX,APPROX_MODE,
                              ITER_MAX,SAVE_TYPE,SAVE_MESH,SAVE_DATA,SAVE_PRINT,
                                            SAVE_WHERE,HMODE_LAG,N_ITER,NO_CFL};
    char pDoubleToPrint[37][12]={"x_min","y_min","z_min","x_max","y_max",
                                 "z_max","delta_x","delta_y","delta_z","ls_x",
                                 "ls_y","ls_z","ls_r","met_err","met_min",
                                 "met_max","iter_told0p","iter_told1p",
                                 "iter_told2p","hmin_iso","hmax_iso",
                                 "hausd_iso","hgrad_iso","hmin_met","hmax_met",
                                 "hausd_met","hgrad_met","hmin_ls","hmax_ls",
                                 "hausd_ls","hgrad_ls","hmin_lag","hmax_lag",
                                  "hausd_lag","hgrad_lag","residual","delta_t"};
    double *pDouble[37]={&parameters.x_min,&parameters.y_min,&parameters.z_min,
                         &parameters.x_max,&parameters.y_max,&parameters.z_max,
                         &parameters.delta_x,&parameters.delta_y,
                         &parameters.delta_z,&parameters.ls_x,&parameters.ls_y,
                         &parameters.ls_z,&parameters.ls_r,&parameters.met_err,
                         &parameters.met_min,&parameters.met_max,
                         &parameters.iter_told0p,&parameters.iter_told1p,
                         &parameters.iter_told2p,&parameters.hmin_iso,
                         &parameters.hmax_iso,&parameters.hausd_iso,
                         &parameters.hgrad_iso,&parameters.hmin_met,
                         &parameters.hmax_met,&parameters.hausd_met,
                         &parameters.hgrad_met,&parameters.hmin_ls,
                         &parameters.hmax_ls,&parameters.hausd_ls,
                         &parameters.hgrad_ls,&parameters.hmin_lag,
                         &parameters.hmax_lag,&parameters.hausd_lag,
                         &parameters.hgrad_lag,&parameters.residual,
                                                           &parameters.delta_t};
    double pDoubleReference[37]={X_MIN,Y_MIN,Z_MIN,X_MAX,Y_MAX,Z_MAX,DELTA_X,
                                 DELTA_Y,DELTA_Z,LS_X,LS_Y,LS_Z,LS_R,MET_ERR,
                                 MET_MIN,MET_MAX,ITER_TOLD0P,ITER_TOLD1P,
                                 ITER_TOLD2P,HMIN_ISO,HMAX_ISO,HAUSD_ISO,
                                 HGRAD_ISO,HMIN_MET,HMAX_MET,HAUSD_MET,
                                 HGRAD_MET,HMIN_LS,HMAX_LS,HAUSD_LS,HGRAD_LS,
                                 HMIN_LAG,HMAX_LAG,HAUSD_LAG,HGRAD_LAG,RESIDUAL,
                                                                       DELTA_T};

    // Initializing to zero the structure of parameters
    for (i=0; i<10; i++)
    {
        *pString[i]=NULL;
    }
    for (i=0; i<23; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<37; i++)
    {
        *pDouble[i]=.0;
    }

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting setupDefaultParameters function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    fprintf(stdout,"nameTest=%p\n",(void*)nameInfo);
    returnValue=setupDefaultParameters(pParameters,nameInfo);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pParameters=&parameters;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    fprintf(stdout,"nameTest=%p\n",(void*)nameInfo);
    returnValue=setupDefaultParameters(pParameters,nameInfo);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    nameInfo=nameTest;
    pParameters=NULL;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    fprintf(stdout,"nameTest=%p\n",(void*)nameInfo);
    returnValue=setupDefaultParameters(pParameters,nameInfo);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pParameters=&parameters;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    fprintf(stdout,"nameTest=%p\n",(void*)nameInfo);
    returnValue=setupDefaultParameters(pParameters,nameInfo);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    nameTest[NAME_LENGTH+10]='\0';
    for (iLength=-NAME_LENGTH; iLength<=NAME_LENGTH; iLength+=NAME_LENGTH)
    {
        for (i=0; i<32; i++)
        {
            switch(i)
            {
                case 0:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-4)
                        {
                            nameTest[j]='a';
                        }
                        else if (j==iLength-4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='A';
                        }
                    }
                    break;

                case 1:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-4)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength-4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='B';
                        }
                    }
                    break;

                case 2:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-4)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength-4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 3:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-4)
                        {
                            nameTest[j]='c';
                        }
                        else if (j==iLength-4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 4:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-4)
                        {
                            nameTest[j]='d';
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 5:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-4)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 6:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-1)
                        {
                            nameTest[j]='e';
                        }
                        else if (j==iLength-1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='E';
                        }
                    }
                    break;

                case 7:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-1)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength-1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='F';
                        }
                    }
                    break;

                case 8:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-1)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength-1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 9:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-1)
                        {
                            nameTest[j]='g';
                        }
                        else if (j==iLength-1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 10:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-1)
                        {
                            nameTest[j]='h';
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 11:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength-1)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 12:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength)
                        {
                            nameTest[j]='i';
                        }
                        else if (j==iLength)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='I';
                        }
                    }
                    break;

                case 13:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='J';
                        }
                    }
                    break;

                case 14:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 15:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength)
                        {
                            nameTest[j]='k';
                        }
                        else if (j==iLength)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 16:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength)
                        {
                            nameTest[j]='l';
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 17:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 18:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+1)
                        {
                            nameTest[j]='m';
                        }
                        else if (j==iLength+1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='M';
                        }
                    }
                    break;

                case 19:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+1)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength+1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='N';
                        }
                    }
                    break;

                case 20:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+1)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength+1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 21:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+1)
                        {
                            nameTest[j]='o';
                        }
                        else if (j==iLength+1)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 22:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+1)
                        {
                            nameTest[j]='p';
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 23:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+1)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 24:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+4)
                        {
                            nameTest[j]='q';
                        }
                        else if (j==iLength+4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='Q';
                        }
                    }
                    break;

                case 25:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+4)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength+4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]='R';
                        }
                    }
                    break;

                case 26:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+4)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else if (j==iLength+4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 27:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+4)
                        {
                            nameTest[j]='s';
                        }
                        else if (j==iLength+4)
                        {
                            nameTest[j]='\0';
                        }
                        else
                        {
                            nameTest[j]=rand()%90+33;
                        }
                    }
                    break;

                case 28:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+4)
                        {
                            nameTest[j]='t';
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 29:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        if (j<iLength+4)
                        {
                            nameTest[j]=rand()%90+33;
                        }
                        else
                        {
                            nameTest[j]='\0';
                        }
                    }
                    break;

                case 30:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        nameTest[j]=rand()%90+33;
                    }
                    break;

                case 31:
                    for (j=0; j<NAME_LENGTH+10; j++)
                    {
                        nameTest[j]=rand()%90+33;
                    }
                    nameTest[rand()%(NAME_LENGTH+10)]='\0';
                    break;
            }
            expectedValue=1;
            lengthName=strlen(nameTest);
            if (lengthName<6 || lengthName>=NAME_LENGTH)
            {
                expectedValue=0;
            }
            PRINT_TEST_START(counter,expectedValue);
            fprintf(stdout,"nameTest=%s\n",nameTest);
            fprintf(stdout,"(%ld characters)\n",lengthName);
            returnValue=setupDefaultParameters(pParameters,nameInfo);
            if (returnValue)
            {
                for (j=0; j<10; j++)
                {
                    free(*pString[j]);
                    *pString[j]=NULL;
                }
            }
            PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);
        }
    }

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables
        for (i=0; i<NAME_LENGTH+10; i++)
        {
             nameTest[i]=rand()%90+33;
        }
        nameTest[rand()%(NAME_LENGTH+10)]='\0';

        for (i=0; i<10; i++)
        {
            lengthName=rand()%120+1;
            if (lengthName>100)
            {
                *pString[i]=NULL;
            }
            else
            {
                *pString[i]=(char*)calloc(lengthName,sizeof(char));
            }
            if (*pString[i]!=NULL)
            {
                iLength=lengthName;
                for (j=0; j<iLength; j++)
                {
                    (*pString[i])[j]=rand()%90+33;
                }
                (*pString[i])[rand()%lengthName]='\0';
            }
        }
        for (i=0; i<23; i++)
        {
            *pInteger[i]=rand()%221-20;
        }
        for (i=0; i<37; i++)
        {
            *pDouble[i]=(double)(rand()%6001-3000)/1000.;
        }

        expectedValue=0;
        if (strlen(nameTest)<NAME_LENGTH && strlen(nameTest)>5)
        {
            expectedValue=1;
        }
        PRINT_TEST_START(counter,expectedValue);

        // Printing the values of the different variables
        fprintf(stdout,"nameTest=%s\n",nameInfo);
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%s\n",pStringToPrint[i],*pString[i]);
        }
        for (i=0; i<23; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<37; i++)
        {
            fprintf(stdout,"%s=%lf\n",pDoubleToPrint[i],*pDouble[i]);
        }

        // Free the memory allocated for strings
        for (i=0; i<10; i++)
        {
            free(*pString[i]);
        }

        // Testing the function
        returnValue=setupDefaultParameters(pParameters,nameInfo);

        // Checking if the setting up of default values worked
        boolean=0;
        for (i=0; i<10; i++)
        {
            if (*pString[i]==NULL || pStringReference[i]==NULL)
            {
                boolean=(boolean || *pString[i]!=pStringReference[i]);
            }
            else
            {
                boolean=(boolean || strcmp(*pString[i],pStringReference[i]));
            }
        }
        for (i=0; i<23; i++)
        {
            boolean=(boolean || *pInteger[i]!=pIntegerReference[i]);
        }
        for (i=0; i<37; i++)
        {
            boolean=(boolean || *pDouble[i]!=pDoubleReference[i]);
        }

        if (boolean && returnValue)
        {
            if (expectedValue)
            {
                returnValue=0;
            }
            fprintf(stdout,"\nAlthough setupDefaultParameters function ended ");
            fprintf(stdout,"normally, the pParameters structure has not been ");
            fprintf(stdout,"initialized properly. One of the following ");
            fprintf(stdout,"variables is not set to its default value ");
            fprintf(stdout,"(preprocessor constants in loadParameter.h):\n");

            for (i=0; i<10; i++)
            {
                fprintf(stdout,"(%s=%s) =",pStringToPrint[i],*pString[i]);
                fprintf(stdout,"= %s\n",pStringReference[i]);
            }
            for (i=0; i<23; i++)
            {
                fprintf(stdout,"(%s=%d) =",pIntegerToPrint[i],*pInteger[i]);
                fprintf(stdout,"= %d\n",pIntegerReference[i]);
            }
            for (i=0; i<37; i++)
            {
                fprintf(stdout,"(%s=%lf) =",pDoubleToPrint[i],*pDouble[i]);
                fprintf(stdout,"= %lf\n",pDoubleReference[i]);
            }
        }

        // Free the memory allocated for strings in case of success
        if (returnValue)
        {
            for (i=0; i<10; i++)
            {
                free(*pString[i]);
                *pString[i]=NULL;
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

// Unit (random) tests on getLengthAfterKeywordBeginning of loadParameters.c
void testGetLengthAfterKeywordBeginning(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char pKeywordBeginning[23][3]={"op","ve","n_","na","en","nu","or","x_","y_",
                                   "z_","de","ls","me","tr","ap","it","sa","pa",
                                                      "hm","ha","hg","re","no"};
    char keywordBeginning[3]={'\0'};
    int i=0, iRandom=0, booleanForString=0, counterLine=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting getLengthAfterKeywordBeginning function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"keywordBeginning=%s\n",keywordBeginning);
    fprintf(stdout,"counter=%d\n",counterLine);
    returnValue=getLengthAfterKeywordBeginning(keywordBeginning,counterLine);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
            // Giving random values to the variables
            if (iRandom>30000)
            {
                strncpy(keywordBeginning,pKeywordBeginning[rand()%23],3);
            }
            else
            {
                for (i=0; i<3; i++)
                {
                    keywordBeginning[i]=rand()%90+33;
                }
                if (rand()%3==0)
                {
                    keywordBeginning[rand()%3]='\0';
                }
                else if (rand()%3==1)
                {
                    keywordBeginning[2]='\0';
                }
            }
            counterLine=rand()%91-10;

            // Setting up the expected value
            booleanForString=(keywordBeginning[0]=='\0');
            booleanForString=(booleanForString || keywordBeginning[1]=='\0');
            booleanForString=(booleanForString || keywordBeginning[2]=='\0');

            if (!booleanForString)
            {
                expectedValue=0;
            }
            else
            {
                if (counterLine<1 || counterLine>70)
                {
                    expectedValue=0;
                }
                else if (!strcmp(keywordBeginning,"n_"))
                {
                    expectedValue=2;
                }
                else if (!strcmp(keywordBeginning,"ls"))
                {
                    expectedValue=3;
                }
                else if (!strcmp(keywordBeginning,"x_") ||
                                  !strcmp(keywordBeginning,"y_") ||
                                                 !strcmp(keywordBeginning,"z_"))
                {
                    expectedValue=4;
                }
                else if (!strcmp(keywordBeginning,"no"))
                {
                    expectedValue=5;
                }
                else if (!strcmp(keywordBeginning,"ve") ||
                              !strcmp(keywordBeginning,"nu") ||
                                   !strcmp(keywordBeginning,"or") ||
                                        !strcmp(keywordBeginning,"de") ||
                                             !strcmp(keywordBeginning,"me") ||
                                                 !strcmp(keywordBeginning,"hm"))
                {
                    expectedValue=6;
                }
                else if (!strcmp(keywordBeginning,"op") ||
                              !strcmp(keywordBeginning,"en") ||
                                   !strcmp(keywordBeginning,"it") ||
                                        !strcmp(keywordBeginning,"ha") ||
                                             !strcmp(keywordBeginning,"hg") ||
                                                 !strcmp(keywordBeginning,"re"))
                {
                    expectedValue=7;
                }
                else if (!strcmp(keywordBeginning,"na") ||
                                                 !strcmp(keywordBeginning,"sa"))
                {
                    expectedValue=8;
                }
                else if (!strcmp(keywordBeginning,"pa"))
                {
                    expectedValue=9;
                }
                else if (!strcmp(keywordBeginning,"ap"))
                {
                    expectedValue=10;
                }
                else if (!strcmp(keywordBeginning,"tr"))
                {
                    expectedValue=11;
                }
                else
                {
                    expectedValue=0;
                }
            }

            PRINT_TEST_START(counter,expectedValue);

            // Printing the values of the different variables
            if (!booleanForString)
            {
                fprintf(stdout,"keywordBeginning=%c",keywordBeginning[0]);
                fprintf(stdout,"%c",keywordBeginning[1]);
                fprintf(stdout,"%c\n",keywordBeginning[2]);
            }
            else
            {
                fprintf(stdout,"keywordBeginning=%s\n",keywordBeginning);
            }
            fprintf(stdout,"counter=%d\n",counterLine);
            returnValue=getLengthAfterKeywordBeginning(keywordBeginning,
                                                                   counterLine);
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

// Unit (random) tests on getTypeAfterKeyword of loadParameters.c
void testGetTypeAfterKeyword(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char pKeywordMiddle[64][11]={"d_data","me_data","me_chem","me_mesh",
                                 "me_elas","th_medit","th_mmg3d","th_mshdi",
                                 "th_elast","th_advec","me_leng","th_lengt",
                                 "t_mode","rbose","c","_elec","_spin","b_rhf",
                                 "x","y","z","_t","ick_matrix","prox_mode",
                                 "er_max", "ve_type","ve_mesh","ve_data",
                                 "ve_prin","ve_wher","ode_l","i","_cfl","min",
                                 "max","lta_x","lta_y","lta_z","_x","_y","_z",
                                 "_r","t_err","t_min","t_max","er_tol", "in_is",
                                 "ax_is","in_me","ax_me","in_ls","ax_ls",
                                 "in_la","ax_la","usd_is","usd_me","usd_ls",
                                 "usd_la","rad_is","rad_me","rad_ls","rad_la",
                                                              "sidual","lta_t"};
    char keywordMiddle[11]={'\0'};
    int booleanForKeyword=0, booleanForInteger=0, booleanForDouble=0;
    int booleanForString=0;
    int  i=0, iRandom=0, lengthMiddle=0, counterLine=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting getTypeAfterKeyword function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"keywordMiddle=%s\n",keywordMiddle);
    fprintf(stdout,"lengthMiddle=%d\n",lengthMiddle);
    fprintf(stdout,"counter=%d\n",counterLine);
    returnValue=getTypeAfterKeyword(keywordMiddle,lengthMiddle,counterLine);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables
        if (iRandom>30000)
        {
            strncpy(keywordMiddle,pKeywordMiddle[rand()%64],11);
            lengthMiddle=strlen(keywordMiddle)+1;
            if (rand()%2)
            {
                lengthMiddle=rand()%31-10;
            }
        }
        else
        {
            for (i=0; i<11; i++)
            {
                keywordMiddle[i]=rand()%90+33;
            }
            lengthMiddle=rand()%31-10;
            if (rand()%2)
            {
                keywordMiddle[rand()%11]='\0';
                if (rand()%3)
                {
                    lengthMiddle=strlen(keywordMiddle)+1;
                }
            }
        }
        counterLine=rand()%91-10;

        // Setting up the expected value
        booleanForKeyword=0;
        for (i=0; i<11; i++)
        {
            if (keywordMiddle[i]=='\0')
            {
                booleanForKeyword=1;
            }
        }

        if (!booleanForKeyword)
        {
            expectedValue=0;
        }
        else
        {
            booleanForString=0;
            for (i=1; i<10; i++)
            {
                booleanForString=(booleanForString ||
                                      !strcmp(keywordMiddle,pKeywordMiddle[i]));
            }
            booleanForInteger=0;
            for (i=10; i<33; i++)
            {
                booleanForInteger=(booleanForInteger ||
                                      !strcmp(keywordMiddle,pKeywordMiddle[i]));
            }
            booleanForDouble=0;
            for (i=33; i<64; i++)
            {
                booleanForDouble=(booleanForDouble ||
                                      !strcmp(keywordMiddle,pKeywordMiddle[i]));
            }

            if (lengthMiddle<2 || lengthMiddle>11 ||
                                     lengthMiddle!=(int)strlen(keywordMiddle)+1)
            {
                expectedValue=0;
            }
            else if (counterLine<1 || counterLine>70)
            {
                expectedValue=0;
            }
            else if (!strcmp(keywordMiddle,pKeywordMiddle[0]))
            {
                expectedValue=-1;
            }
            else if (booleanForInteger)
            {
                expectedValue=1;
            }
            else if (booleanForDouble)
            {
                expectedValue=2;
            }
            else if (booleanForString)
            {
                expectedValue=3;
            }
            else
            {
                expectedValue=0;
            }
        }

        PRINT_TEST_START(counter,expectedValue);
        if (!booleanForKeyword)
        {
            fprintf(stdout,"keywordMiddle=");
            for (i=0; i<11; i++)
            {
                fprintf(stdout,"%c",keywordMiddle[i]);
            }
        }
        else
        {
            fprintf(stdout,"keywordMiddle=%s",keywordMiddle);
        }
        fprintf(stdout,"\nlengthMiddle=%d\n",lengthMiddle);
        fprintf(stdout,"counter=%d\n",counterLine);
        returnValue=getTypeAfterKeyword(keywordMiddle,lengthMiddle,counterLine);
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

// Unit (random) tests on getLengthAfterKeywordMiddle of loadParameters.c
void testGetLengthAfterKeywordMiddle(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char pKeywordMiddle[64][11]={"t_mode","rbose","d_data","me_data","me_chem",
                                 "me_mesh","me_elas","_spin","b_rhf","min",
                                 "max","x","y","z","lta_x","lta_y","lta_z",
                                 "_x","_y","_z","_r","t_err","t_min","t_max",
                                 "ick_matrix","prox_mode","er_max","ve_type",
                                 "ve_mesh","ve_data","th_medit","th_mmg3d",
                                 "in_ls","ax_ls","usd_ls","rad_ls","sidual",
                                 "lta_t","_cfl","ve_prin","ve_wher","th_lengt",
                                 "th_advec","in_is","ax_is","in_me","ax_me",
                                 "in_la","ax_la","usd_is","usd_me","usd_la",
                                 "rad_is","rad_me","rad_la","c","me_leng",
                                 "th_mshdi","th_elast","ode_l","_t","er_tol",
                                                                   "i","_elec"};
    char keywordMiddle[11]={'\0'};
    int booleanForKeyword=0, booleanReturn1=0, booleanReturn2=0;
    int booleanReturn3=0, booleanReturn4=0, i=0, iRandom=0, lengthMiddle=0;
    int counterLine=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting getLengthAfterKeywordMiddle function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"keywordMiddle=%s\n",keywordMiddle);
    fprintf(stdout,"lengthMiddle=%d\n",lengthMiddle);
    fprintf(stdout,"counter=%d\n",counterLine);
    returnValue=getLengthAfterKeywordMiddle(keywordMiddle,lengthMiddle,
                                                                   counterLine);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables
        if (iRandom>30000)
        {
            strncpy(keywordMiddle,pKeywordMiddle[rand()%64],11);
            lengthMiddle=strlen(keywordMiddle)+1;
            if (rand()%2)
            {
                lengthMiddle=rand()%31-10;
            }
        }
        else
        {
            for (i=0; i<11; i++)
            {
                keywordMiddle[i]=rand()%90+33;
            }
            lengthMiddle=rand()%31-10;
            if (rand()%2)
            {
                keywordMiddle[rand()%11]='\0';
                if (rand()%3)
                {
                        lengthMiddle=strlen(keywordMiddle)+1;
                }
            }
        }
        counterLine=rand()%91-10;

        // Setting up the expected value
        booleanForKeyword=0;
        for (i=0; i<11; i++)
        {
            if (keywordMiddle[i]=='\0')
            {
                booleanForKeyword=1;
            }
        }

        if (!booleanForKeyword)
        {
            expectedValue=0;
        }
        else
        {
            booleanReturn1=0;
            for (i=0; i<39; i++)
            {
                booleanReturn1=(booleanReturn1 ||
                                      !strcmp(keywordMiddle,pKeywordMiddle[i]));
            }
            booleanReturn2=0;
            for (i=39; i<55; i++)
            {
                booleanReturn2=(booleanReturn2 ||
                                      !strcmp(keywordMiddle,pKeywordMiddle[i]));
            }
            booleanReturn3=0;
            for (i=55; i<60; i++)
            {
                booleanReturn3=(booleanReturn3 ||
                                      !strcmp(keywordMiddle,pKeywordMiddle[i]));
            }
            booleanReturn4=0;
            for (i=60; i<63; i++)
            {
                booleanReturn4=(booleanReturn4 ||
                                      !strcmp(keywordMiddle,pKeywordMiddle[i]));
            }

            if (lengthMiddle<2 || lengthMiddle>11 ||
                                     lengthMiddle!=(int)strlen(keywordMiddle)+1)
            {
                expectedValue=0;
            }
            else if (counterLine<1 || counterLine>70)
            {
                expectedValue=0;
            }
            else if (!strcmp(keywordMiddle,pKeywordMiddle[63]))
            {
                printf("\n%s\n",pKeywordMiddle[63]);
                expectedValue=6;
            }
            else if (booleanReturn4)
            {
                expectedValue=4;
            }
            else if (booleanReturn3)
            {
                expectedValue=3;
            }
            else if (booleanReturn2)
            {
                expectedValue=2;
            }
            else if (booleanReturn1)
            {
                expectedValue=1;
            }
            else
            {
                expectedValue=0;
            }
        }

        PRINT_TEST_START(counter,expectedValue);
        if (!booleanForKeyword)
        {
            fprintf(stdout,"keywordMiddle=");
            for (i=0; i<11; i++)
            {
            fprintf(stdout,"%c",keywordMiddle[i]);
            }
        }
        else
        {
            fprintf(stdout,"keywordMiddle=%s",keywordMiddle);
        }
        fprintf(stdout,"\nlengthMiddle=%d\n",lengthMiddle);
        fprintf(stdout,"counter=%d\n",counterLine);
        returnValue=getLengthAfterKeywordMiddle(keywordMiddle,lengthMiddle,
                                                                   counterLine);
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

// Unit (random) tests on detectRepetition of loadParameters.c
void testDetectRepetition(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char pKeyword[70][13]={"opt_mode","verbose","n_cpu","name_length",
                           "end_data","name_data","name_chem","name_mesh",
                           "name_elas","nu_electrons","nu_spin","orb_rhf",
                           "x_min","x_max","y_min","y_max","z_min","z_max",
                           "n_x","n_y","n_z","delta_x","delta_y","delta_z",
                           "ls_type","ls_x","ls_y","ls_z","ls_r","met_err",
                           "met_min","met_max","trick_matrix","approx_mode",
                           "iter_max","iter_told0p","iter_told1p","iter_told2p",
                           "save_type","save_mesh","save_data","save_print",
                           "save_where","path_length","path_medit","path_mmg3d",
                           "path_mshdist","path_elastic","path_advect",
                           "hmin_iso","hmax_iso","hmin_met","hmax_met",
                           "hmin_ls","hmax_ls","hmode_lag","hmin_lag",
                           "hmax_lag","hausd_iso","hausd_met","hausd_ls",
                           "hausd_lag","hgrad_iso","hgrad_met","hgrad_ls",
                            "hgrad_lag","n_iter","residual","delta_t","no_cfl"};
    char pKeywordBeginning[23][3]={"op","ve","n_","na","en","nu","or","x_","y_",
                                   "z_","de","ls","me","tr","ap","it","sa","pa",
                                                      "hm","ha","hg","re","no"};
    char pKeywordMiddle[64][11]={"t_mode","rbose","d_data","me_data","me_chem",
                                 "me_mesh","me_elas","_spin","b_rhf","min",
                                 "max","x","y","z","lta_x","lta_y","lta_z",
                                 "_x","_y","_z","_r","t_err","t_min","t_max",
                                 "ick_matrix","prox_mode","er_max","ve_type",
                                 "ve_mesh","ve_data","th_medit","th_mmg3d",
                                 "in_ls","ax_ls","usd_ls","rad_ls","sidual",
                                 "lta_t","_cfl","ve_prin","ve_wher","th_lengt",
                                 "th_advec","in_is","ax_is","in_me","ax_me",
                                 "in_la","ax_la","usd_is","usd_me","usd_la",
                                 "rad_is","rad_me","rad_la","c","me_leng",
                                 "th_mshdi","th_elast","ode_l","_t","er_tol",
                                                                   "i","_elec"};
    char pKeywordEnd[16][6]={"t","e","h","o","g","pu","th","st","ic","ag","ype",
                                               "d0p","d1p","d2p","ter","trons"};
    char keyword[20]={'\0'}, keywordBeginning[3]={'\0'};
    char keywordMiddle[11]={'\0'}, keywordEnd[6]={'\0'};
    int booleanForKeywordBeginning=0, booleanForKeywordMiddle=0;
    int booleanForKeywordEnd=0, booleanForKeyword=0, booleanForRepetition=0;
    int repetitionOld[70]={0}, repetition[70]={0}, i=0, iRandom=0;
    int lengthMiddle=0, lengthEnd=0, counterLine=0;

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting detectRepetition function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"repetition=");
    for (i=0; i<70; i++)
    {
       fprintf(stdout,"%d",repetition[i]);
    }
    fprintf(stdout,"\nkeywordBeginning=%s\n",keywordBeginning);
    fprintf(stdout,"keywordMiddle=%s\n",keywordMiddle);
    fprintf(stdout,"keywordEnd=%s\n",keywordEnd);
    fprintf(stdout,"lengthMiddle=%d\n",lengthMiddle);
    fprintf(stdout,"lengthEnd=%d\n",lengthEnd);
    fprintf(stdout,"counter=%d\n",counterLine);
    returnValue=detectRepetition(repetition,keywordBeginning,keywordMiddle,
                                 keywordEnd,lengthMiddle,lengthEnd,counterLine);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables
        if (iRandom>20000)
        {
            strncpy(keywordBeginning,pKeywordBeginning[rand()%23],3);
            strncpy(keywordMiddle,pKeywordMiddle[rand()%64],11);
            strncpy(keywordEnd,pKeywordEnd[rand()%16],6);
            lengthMiddle=strlen(keywordMiddle)+1;
            lengthEnd=strlen(keywordEnd)+1;

            if (iRandom<21000)
            {
                lengthMiddle=rand()%31-10;
                lengthEnd=1;
            }
            else if (iRandom<22000)
            {
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<23000)
            {
                lengthMiddle=rand()%31-10;
            }
            else if (iRandom<24000)
            {
                lengthEnd=1;
            }
            else if (iRandom<25000)
            {
                lengthEnd=rand()%31-10;
            }
        }
        else
        {
            for (i=0; i<3; i++)
            {
                keywordBeginning[i]=rand()%90+33;
            }
            for (i=0; i<11; i++)
            {
                keywordMiddle[i]=rand()%90+33;
            }
            for (i=0; i<6; i++)
            {
                keywordEnd[i]=rand()%90+33;
            }

            if (iRandom<1000)
            {
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<2000)
            {
                keywordBeginning[rand()%3]='\0';
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<3000)
            {
                keywordBeginning[2]='\0';
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<4000)
            {
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<5000)
            {
                keywordBeginning[rand()%3]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=(int)strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<6000)
            {
                keywordBeginning[2]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<7000)
            {
                lengthMiddle=rand()%31-10;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<8000)
            {
                keywordBeginning[rand()%3]='\0';
                lengthMiddle=rand()%31-10;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<9000)
            {
                keywordBeginning[2]='\0';
                lengthMiddle=rand()%31-10;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<10000)
            {
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<11000)
            {
                keywordBeginning[rand()%3]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<12000)
            {
                keywordBeginning[2]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else
            {
                keywordBeginning[2]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
            }
        }
        counterLine=rand()%91-10;
        for (i=0; i<70; i++)
        {
            repetition[i]=rand()%5-2;
            repetitionOld[i]=repetition[i];
        }

        // Setting up the expected value
        booleanForKeywordBeginning=0;
        for (i=0; i<3; i++)
        {
            if (keywordBeginning[i]=='\0')
            {
                booleanForKeywordBeginning=1;
            }

        }
        booleanForKeywordMiddle=0;
        for (i=0; i<11; i++)
        {
            if (keywordMiddle[i]=='\0')
            {
                booleanForKeywordMiddle=1;
            }
        }
        booleanForKeywordEnd=0;
        for (i=0; i<6; i++)
        {
            if (keywordEnd[i]=='\0')
            {
                booleanForKeywordEnd=1;
            }
        }

        if (!booleanForKeywordBeginning || !booleanForKeywordMiddle ||
                                                          !booleanForKeywordEnd)
        {
            expectedValue=0;
        }
        else
        {
            booleanForKeyword=0;
            strncpy(keyword,keywordBeginning,20);
            strcat(keyword,keywordMiddle);
            for (i=0; i<70; i++)
            {
                booleanForKeyword=(booleanForKeyword ||
                                                  !strcmp(keyword,pKeyword[i]));
            }
            if (!booleanForKeyword)
            {
                strcat(keyword,keywordEnd);
                for (i=0; i<70; i++)
                {
                    booleanForKeyword=(booleanForKeyword ||
                                                  !strcmp(keyword,pKeyword[i]));
                }
            }

            if (lengthMiddle<2 || lengthMiddle>11 ||
                                     lengthMiddle!=(int)strlen(keywordMiddle)+1)
            {
                expectedValue=0;
            }
            else if (lengthEnd<1 || lengthEnd>6 || counterLine<1 ||
                                                                 counterLine>70)
            {
                expectedValue=0;
            }
            else if (lengthEnd>1 && lengthEnd!=(int)strlen(keywordEnd)+1)
            {
                expectedValue=0;
            }
            else if (booleanForKeyword)
            {
                expectedValue=1;
            }
            else
            {
                expectedValue=0;
            }
        }

        PRINT_TEST_START(counter,expectedValue);

        // Print the values of the different variables
        fprintf(stdout,"repetition=");
        for (i=0; i<70; i++)
        {
           fprintf(stdout,"%d",repetition[i]);
        }
        fprintf(stdout,"\nkeywordBeginning=");
        if (!booleanForKeywordBeginning)
        {
            for (i=0; i<3; i++)
            {
               fprintf(stdout,"%c",keywordBeginning[i]);
            }
        }
        else
        {
            fprintf(stdout,"%s",keywordBeginning);
        }
        fprintf(stdout,"\nkeywordMiddle=");
        if (!booleanForKeywordMiddle)
        {
            for (i=0; i<11; i++)
            {
               fprintf(stdout,"%c",keywordMiddle[i]);
            }
        }
        else
        {
            fprintf(stdout,"%s",keywordMiddle);
        }
        fprintf(stdout,"\nkeywordEnd=");
        if (!booleanForKeywordEnd)
        {
            for (i=0; i<6; i++)
            {
               fprintf(stdout,"%c",keywordEnd[i]);
            }
        }
        else
        {
            fprintf(stdout,"%s",keywordEnd);
        }
        fprintf(stdout,"\nlengthMiddle=%d\n",lengthMiddle);
        fprintf(stdout,"lengthEnd=%d\n",lengthEnd);
        fprintf(stdout,"counter=%d\n",counterLine);

        // Testing the function
        returnValue=detectRepetition(repetition,keywordBeginning,keywordMiddle,
                                     keywordEnd,lengthMiddle,lengthEnd,
                                                                   counterLine);

        // Check that repetition has been correctly increased
        if (!booleanForKeywordBeginning || !booleanForKeywordMiddle ||
                                                          !booleanForKeywordEnd)
        {
            booleanForKeyword=0;
            for (i=0; i<70; i++)
            {
                if (repetition[i]!=repetitionOld[i])
                {
                    booleanForKeyword=1;
                }
            }
        }
        else
        {
            booleanForKeyword=70;
            strncpy(keyword,keywordBeginning,20);
            strcat(keyword,keywordMiddle);
            for (i=0; i<70; i++)
            {
                if (!strcmp(keyword,pKeyword[i]))
                {
                    booleanForKeyword=i;
                }
            }
            if (booleanForKeyword==70)
            {
                strcat(keyword,keywordEnd);
                for (i=0; i<70; i++)
                {
                    if (!strcmp(keyword,pKeyword[i]))
                    {
                        booleanForKeyword=i;
                    }
                }
            }

            booleanForRepetition=0;
            for (i=0; i<booleanForKeyword; i++)
            {
                if (repetition[i]!=repetitionOld[i])
                {
                    booleanForRepetition=1;
                }
            }
            if (booleanForKeyword<70)
            {
                if (repetition[booleanForKeyword]!=
                                   repetitionOld[booleanForKeyword]+returnValue)
                {
                    booleanForRepetition=1;
                }
                for (i=booleanForKeyword+1; i<70; i++)
                {
                    if (repetition[i]!=repetitionOld[i])
                    {
                        booleanForRepetition=1;
                    }
                }
            }

            if (booleanForRepetition)
            {
                fprintf(stdout,"\nAlthough detectRepetition function ended ");
                fprintf(stdout,"normally, the repetition variable has not ");
                fprintf(stdout,"been correctly incremented by ");
                fprintf(stdout,"detectRepetition function:\nindex concerned: ");
                fprintf(stdout,"%d\nrepetition=",booleanForKeyword+1);

                for (i=0; i<70; i++)
                {
                    fprintf(stdout,"%d",repetitionOld[i]);
                }
                fprintf(stdout," (before)\nrepetition=");
                for (i=0; i<70; i++)
                {
                    fprintf(stdout,"%d",repetition[i]);
                }
                fprintf(stdout," (after)\n");

               expectedValue=1;
               if (returnValue)
               {
                    expectedValue=0;
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

// Unit (random) tests on changeValuesOfParameters of loadParameters.c
void testChangeValuesOfParameters(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthName=0;
    char keyword[20]={'\0'}, keywordBeginning[3]={'\0'};
    char keywordMiddle[11]={'\0'}, keywordEnd[6]={'\0'};
    char saveString[10][101]={""}, *readStringIn=NULL;
    int booleanForKeywordBeginning=0, booleanForKeywordMiddle=0;
    int booleanForKeywordEnd=0, booleanForKeyword=0, booleanForName=0;
    int booleanForPath=0, booleanForReturnValue=0, iRandom=0, i=0, j=0;
    int lengthMiddle=0, lengthEnd=0, counterLine=0, readIntegerIn=0;
    double readDouble=0.;
    Parameters *pParameters=NULL, parameters, parametersOld;

    char pStringToPrint[10][13]={"name_info","name_data","name_chem",
                                 "name_mesh","name_elas","path_medit",
                                 "path_mmg3d","path_mshdist","path_elastic",
                                                                 "path_advect"};
    char **pString[10]={&parameters.name_info,&parameters.name_data,
                        &parameters.name_chem,&parameters.name_mesh,
                        &parameters.name_elas,&parameters.path_medit,
                        &parameters.path_mmg3d,&parameters.path_mshdist,
                              &parameters.path_elastic,&parameters.path_advect};
    char **pStringOld[10]={&parametersOld.name_info,&parametersOld.name_data,
                           &parametersOld.name_chem,&parametersOld.name_mesh,
                           &parametersOld.name_elas,&parametersOld.path_medit,
                           &parametersOld.path_mmg3d,
                           &parametersOld.path_mshdist,
                           &parametersOld.path_elastic,
                                                    &parametersOld.path_advect};
    char pIntegerToPrint[23][13]={"name_length","path_length","opt_mode",
                                  "verbose","n_cpu","nu_electrons","nu_spin",
                                  "orb_rhf","n_x","n_y","n_z","ls_type",
                                  "trick_matrix","approx_mode","iter_max",
                                  "save_type","save_mesh","save_data",
                                  "save_print","save_where","hmode_lag",
                                                             "n_iter","no_cfl"};
    int *pInteger[23]={&parameters.name_length,&parameters.path_length,
                       &parameters.opt_mode,&parameters.verbose,
                       &parameters.n_cpu,&parameters.nu_electrons,
                       &parameters.nu_spin,&parameters.orb_rhf,&parameters.n_x,
                       &parameters.n_y,&parameters.n_z,&parameters.ls_type,
                       &parameters.trick_matrix,&parameters.approx_mode,
                       &parameters.iter_max,&parameters.save_type,
                       &parameters.save_mesh,&parameters.save_data,
                       &parameters.save_print,&parameters.save_where,
                       &parameters.hmode_lag,&parameters.n_iter,
                                                            &parameters.no_cfl};
    int *pIntegerOld[23]={&parametersOld.name_length,&parametersOld.path_length,
                          &parametersOld.opt_mode,&parametersOld.verbose,
                          &parametersOld.n_cpu,&parametersOld.nu_electrons,
                          &parametersOld.nu_spin,&parametersOld.orb_rhf,
                          &parametersOld.n_x,&parametersOld.n_y,
                          &parametersOld.n_z,&parametersOld.ls_type,
                          &parametersOld.trick_matrix,
                          &parametersOld.approx_mode,&parametersOld.iter_max,
                          &parametersOld.save_type,&parametersOld.save_mesh,
                          &parametersOld.save_data,&parametersOld.save_print,
                          &parametersOld.save_where,&parametersOld.hmode_lag,
                                   &parametersOld.n_iter,&parametersOld.no_cfl};
    char pDoubleToPrint[37][12]={"x_min","y_min","z_min","x_max","y_max",
                                 "z_max","delta_x","delta_y","delta_z","ls_x",
                                 "ls_y","ls_z","ls_r","met_err","met_min",
                                 "met_max","iter_told0p","iter_told1p",
                                 "iter_told2p","hmin_iso","hmax_iso",
                                 "hausd_iso","hgrad_iso","hmin_met","hmax_met",
                                 "hausd_met","hgrad_met","hmin_ls","hmax_ls",
                                 "hausd_ls","hgrad_ls","hmin_lag","hmax_lag",
                                  "hausd_lag","hgrad_lag","residual","delta_t"};
    double *pDouble[37]={&parameters.x_min,&parameters.y_min,&parameters.z_min,
                         &parameters.x_max,&parameters.y_max,&parameters.z_max,
                         &parameters.delta_x,&parameters.delta_y,
                         &parameters.delta_z,&parameters.ls_x,&parameters.ls_y,
                         &parameters.ls_z,&parameters.ls_r,&parameters.met_err,
                         &parameters.met_min,&parameters.met_max,
                         &parameters.iter_told0p,&parameters.iter_told1p,
                         &parameters.iter_told2p,&parameters.hmin_iso,
                         &parameters.hmax_iso,&parameters.hausd_iso,
                         &parameters.hgrad_iso,&parameters.hmin_met,
                         &parameters.hmax_met,&parameters.hausd_met,
                         &parameters.hgrad_met,&parameters.hmin_ls,
                         &parameters.hmax_ls,&parameters.hausd_ls,
                         &parameters.hgrad_ls,&parameters.hmin_lag,
                         &parameters.hmax_lag,&parameters.hausd_lag,
                         &parameters.hgrad_lag,&parameters.residual,
                                                           &parameters.delta_t};
    double *pDoubleOld[37]={&parametersOld.x_min,&parametersOld.y_min,
                            &parametersOld.z_min,&parametersOld.x_max,
                            &parametersOld.y_max,&parametersOld.z_max,
                            &parametersOld.delta_x,&parametersOld.delta_y,
                            &parametersOld.delta_z,&parametersOld.ls_x,
                            &parametersOld.ls_y,&parametersOld.ls_z,
                            &parametersOld.ls_r,&parametersOld.met_err,
                            &parametersOld.met_min,&parametersOld.met_max,
                            &parametersOld.iter_told0p,
                            &parametersOld.iter_told1p,
                            &parametersOld.iter_told2p,&parametersOld.hmin_iso,
                            &parametersOld.hmax_iso,&parametersOld.hausd_iso,
                            &parametersOld.hgrad_iso,&parametersOld.hmin_met,
                            &parametersOld.hmax_met,&parametersOld.hausd_met,
                            &parametersOld.hgrad_met,&parametersOld.hmin_ls,
                            &parametersOld.hmax_ls,&parametersOld.hausd_ls,
                            &parametersOld.hgrad_ls,&parametersOld.hmin_lag,
                            &parametersOld.hmax_lag,&parametersOld.hausd_lag,
                            &parametersOld.hgrad_lag,&parametersOld.residual,
                                                        &parametersOld.delta_t};
    char pKeyword[70][13]={"opt_mode","verbose","n_cpu","name_length",
                           "end_data","name_data","name_chem","name_mesh",
                           "name_elas","nu_electrons","nu_spin","orb_rhf",
                           "x_min","x_max","y_min","y_max","z_min","z_max",
                           "n_x","n_y","n_z","delta_x","delta_y","delta_z",
                           "ls_type","ls_x","ls_y","ls_z","ls_r","met_err",
                           "met_min","met_max","trick_matrix","approx_mode",
                           "iter_max","iter_told0p","iter_told1p","iter_told2p",
                           "save_type","save_mesh","save_data","save_print",
                           "save_where","path_length","path_medit","path_mmg3d",
                           "path_mshdist","path_elastic","path_advect",
                           "hmin_iso","hmax_iso","hmin_met","hmax_met",
                           "hmin_ls","hmax_ls","hmode_lag","hmin_lag",
                           "hmax_lag","hausd_iso","hausd_met","hausd_ls",
                           "hausd_lag","hgrad_iso","hgrad_met","hgrad_ls",
                            "hgrad_lag","n_iter","residual","delta_t","no_cfl"};
    char pKeywordBeginning[23][3]={"op","ve","n_","na","en","nu","or","x_","y_",
                                   "z_","de","ls","me","tr","ap","it","sa","pa",
                                                      "hm","ha","hg","re","no"};
    char pKeywordMiddle[64][11]={"t_mode","rbose","d_data","me_data","me_chem",
                                 "me_mesh","me_elas","_spin","b_rhf","min",
                                 "max","x","y","z","lta_x","lta_y","lta_z",
                                 "_x","_y","_z","_r","t_err","t_min","t_max",
                                 "ick_matrix","prox_mode","er_max","ve_type",
                                 "ve_mesh","ve_data","th_medit","th_mmg3d",
                                 "in_ls","ax_ls","usd_ls","rad_ls","sidual",
                                 "lta_t","_cfl","ve_prin","ve_wher","th_lengt",
                                 "th_advec","in_is","ax_is","in_me","ax_me",
                                 "in_la","ax_la","usd_is","usd_me","usd_la",
                                 "rad_is","rad_me","rad_la","c","me_leng",
                                 "th_mshdi","th_elast","ode_l","_t","er_tol",
                                                                   "i","_elec"};
    char pKeywordEnd[16][6]={"t","e","h","o","g","pu","th","st","ic","ag","ype",
                                               "d0p","d1p","d2p","ter","trons"};

    // Initializing to zero the structure of parameters and parametersOld
    for (i=0; i<10; i++)
    {
        *pString[i]=NULL;
        *pStringOld[i]=NULL;
        strncpy(saveString[i],"",101);
    }
    for (i=0; i<23; i++)
    {
        *pInteger[i]=0;
        *pIntegerOld[i]=0;
    }
    for (i=0; i<37; i++)
    {
        *pDouble[i]=.0;
        *pDoubleOld[i]=.0;
    }

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting changeValuesOfParameters function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    fprintf(stdout,"keywordBeginning=%s\n",keywordBeginning);
    fprintf(stdout,"keywordMiddle=%s\n",keywordMiddle);
    fprintf(stdout,"keywordEnd=%s\n",keywordEnd);
    fprintf(stdout,"readStringIn=%p\n",(void*)readStringIn);
    fprintf(stdout,"lengthMiddle=%d\n",lengthMiddle);
    fprintf(stdout,"lengthEnd=%d\n",lengthEnd);
    fprintf(stdout,"counter=%d\n",counterLine);
    fprintf(stdout,"readIntegerIn=%d\n",readIntegerIn);
    fprintf(stdout,"readDouble=%lf\n",readDouble);
    returnValue=changeValuesOfParameters(pParameters,keywordBeginning,
                                         keywordMiddle,keywordEnd,
                                         readStringIn,lengthMiddle,lengthEnd,
                                          counterLine,readIntegerIn,readDouble);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pParameters=&parameters;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    fprintf(stdout,"keywordBeginning=%s\n",keywordBeginning);
    fprintf(stdout,"keywordMiddle=%s\n",keywordMiddle);
    fprintf(stdout,"keywordEnd=%s\n",keywordEnd);
    fprintf(stdout,"readStringIn=%p\n",(void*)readStringIn);
    fprintf(stdout,"lengthMiddle=%d\n",lengthMiddle);
    fprintf(stdout,"lengthEnd=%d\n",lengthEnd);
    fprintf(stdout,"counter=%d\n",counterLine);
    fprintf(stdout,"readIntegerIn=%d\n",readIntegerIn);
    fprintf(stdout,"readDouble=%lf\n",readDouble);
    returnValue=changeValuesOfParameters(pParameters,keywordBeginning,
                                         keywordMiddle,keywordEnd,
                                         readStringIn,lengthMiddle,lengthEnd,
                                          counterLine,readIntegerIn,readDouble);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables
        pParameters->name_length=rand()%120+1;
        pParameters->path_length=rand()%120+1;
        for (i=0; i<10; i++)
        {
            lengthName=rand()%120+1;
            if (lengthName>100)
            {
                *pString[i]=NULL;
            }
            else
            {
                if (i<5)
                {
                    lengthName=pParameters->name_length;
                }
                else
                {
                    lengthName=pParameters->path_length;
                }
                *pString[i]=(char*)calloc(lengthName,sizeof(char));
            }
            if (*pString[i]!=NULL)
            {
                for (j=0; j<(int)lengthName; j++)
                {
                    (*pString[i])[j]=rand()%90+33;
                }
                (*pString[i])[rand()%lengthName]='\0';
            }
        }
        // Starting from 2 because path_length, name_length already initialized
        for (i=2; i<23; i++)
        {
            *pInteger[i]=rand()%121-20;
        }
        for (i=0; i<37; i++)
        {
            *pDouble[i]=(double)(rand()%6001-3000)/1000.;
        }

        if (iRandom>20000)
        {
            strncpy(keywordBeginning,pKeywordBeginning[rand()%23],3);
            strncpy(keywordMiddle,pKeywordMiddle[rand()%64],11);
            strncpy(keywordEnd,pKeywordEnd[rand()%16],6);
            lengthMiddle=strlen(keywordMiddle)+1;
            lengthEnd=strlen(keywordEnd)+1;

            if (iRandom<21000)
            {
                lengthMiddle=rand()%31-10;
                lengthEnd=1;
            }
            else if (iRandom<22000)
            {
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<23000)
            {
                lengthMiddle=rand()%31-10;
            }
            else if (iRandom<24000)
            {
                lengthEnd=1;
            }
            else if (iRandom<25000)
            {
                lengthEnd=rand()%31-10;
            }
        }
        else
        {
            for (i=0; i<3; i++)
            {
                keywordBeginning[i]=rand()%90+33;
                keywordMiddle[i]=rand()%90+33;
                keywordEnd[i]=rand()%90+33;
            }
            for (i=3; i<6; i++)
            {
                keywordMiddle[i]=rand()%90+33;
                keywordEnd[i]=rand()%90+33;
            }
            for (i=6; i<11; i++)
            {
                keywordMiddle[i]=rand()%90+33;
            }

            if (iRandom<1000)
            {
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<2000)
            {
                keywordBeginning[rand()%3]='\0';
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<3000)
            {
                keywordBeginning[2]='\0';
                lengthMiddle=rand()%31-10;
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<4000)
            {
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<5000)
            {
                keywordBeginning[rand()%3]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<6000)
            {
                keywordBeginning[2]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                lengthEnd=rand()%31-10;
            }
            else if (iRandom<7000)
            {
                lengthMiddle=rand()%31-10;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                     lengthEnd=1;
                }
            }
            else if (iRandom<8000)
            {
                keywordBeginning[rand()%3]='\0';
                lengthMiddle=rand()%31-10;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<9000)
            {
                keywordBeginning[2]='\0';
                lengthMiddle=rand()%31-10;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<10000)
            {
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<11000)
            {
                keywordBeginning[rand()%3]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else if (iRandom<12000)
            {
                keywordBeginning[2]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                if (rand()%2)
                {
                    lengthMiddle=rand()%31-10;
                }
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
                if (rand()%3==0)
                {
                    lengthEnd=rand()%31-10;
                }
                else if (rand()%3==1)
                {
                    lengthEnd=1;
                }
            }
            else
            {
                keywordBeginning[2]='\0';
                keywordMiddle[rand()%11]='\0';
                lengthMiddle=strlen(keywordMiddle)+1;
                keywordEnd[rand()%6]='\0';
                lengthEnd=strlen(keywordEnd)+1;
            }
        }
        counterLine=rand()%91-10;

        lengthName=rand()%110+1;
        if (lengthName>100)
        {
            readStringIn=NULL;
        }
        else
        {
            readStringIn=(char*)calloc(lengthName,sizeof(char));
        }
        if (readStringIn!=NULL)
        {
            for (i=0; i<(int)lengthName; i++)
            {
                readStringIn[i]=rand()%90+33;
            }
            readStringIn[rand()%lengthName]='\0';
        }
        readIntegerIn=rand()%101-20;
        readDouble=(double)(rand()%6001-3000)/1000.;

        // Copying the values of pParameters into parametersOld
        for (i=0; i<10; i++)
        {
            *pStringOld[i]=*pString[i];
            strncpy(saveString[i],"(NULL)",101);
            if (*pString[i]!=NULL)
            {
                strncpy(saveString[i],*pString[i],101);
            }
        }
        for (i=0; i<23; i++)
        {
            *pIntegerOld[i]=*pInteger[i];
        }
        for (i=0; i<37; i++)
        {
            *pDoubleOld[i]=*pDouble[i];
        }

        // Setting up the expected value
        booleanForKeywordBeginning=0;
        for (i=0; i<3; i++)
        {
            if (keywordBeginning[i]=='\0')
            {
                booleanForKeywordBeginning=1;
            }
        }
        booleanForKeywordMiddle=0;
        for (i=0; i<11; i++)
        {
            if (keywordMiddle[i]=='\0')
            {
                booleanForKeywordMiddle=1;
            }
        }
        booleanForKeywordEnd=0;
        for (i=0; i<6; i++)
        {
            if (keywordEnd[i]=='\0')
            {
                booleanForKeywordEnd=1;
            }
        }

        if (!booleanForKeywordBeginning || !booleanForKeywordMiddle ||
                                                          !booleanForKeywordEnd)
        {
            expectedValue=0;
        }
        else
        {
            booleanForName=0;
            if (readStringIn!=NULL)
            {
                booleanForName=((int)strlen(readStringIn)<
                                                     pParameters->name_length &&
                                                        strlen(readStringIn)>5);
            }
            booleanForPath=0;
            if (readStringIn!=NULL)
            {
                booleanForPath=((int)strlen(readStringIn)<
                                                     pParameters->path_length &&
                                                        strlen(readStringIn)>0);
            }

            booleanForKeyword=0;
            strncpy(keyword,keywordBeginning,20);
            strcat(keyword,keywordMiddle);
            for (i=0; i<70; i++)
            {
                switch(i)
                {
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        booleanForKeyword=(booleanForKeyword ||
                                                   (!strcmp(keyword,pKeyword[i])
                                                       && booleanForName
                                                       && *pString[i-4]==NULL));
                        break;

                    case 44:
                    case 45:
                        booleanForKeyword=(booleanForKeyword ||
                                                   (!strcmp(keyword,pKeyword[i])
                                                            && booleanForPath));
                        break;

                    default:
                        booleanForKeyword=(booleanForKeyword ||
                                                  !strcmp(keyword,pKeyword[i]));
                        break;
                }
            }
            if (!booleanForKeyword)
            {
                strcat(keyword,keywordEnd);
                for (i=0; i<70; i++)
                {
                    switch(i)
                    {
                        case 3:
                            booleanForKeyword=(booleanForKeyword ||
                                                (!strcmp(keyword,pKeyword[i]) &&
                                      readIntegerIn>=pParameters->name_length));
                            break;

                        case 5:
                        case 6:
                        case 7:
                        case 8:
                            booleanForKeyword=(booleanForKeyword ||
                                                   (!strcmp(keyword,pKeyword[i])
                                                       && booleanForName
                                                       && *pString[i-4]==NULL));
                         break;

                        case 43:
                            booleanForKeyword=(booleanForKeyword ||
                                                (!strcmp(keyword,pKeyword[i]) &&
                                      readIntegerIn>=pParameters->path_length));
                             break;

                        case 44:
                        case 45:
                        case 46:
                        case 47:
                        case 48:
                            booleanForKeyword=(booleanForKeyword ||
                                                   (!strcmp(keyword,pKeyword[i])
                                                            && booleanForPath));
                            break;

                        default:
                            booleanForKeyword=(booleanForKeyword ||
                                                  !strcmp(keyword,pKeyword[i]));
                            break;
                    }
                }
            }

            if (pParameters->name_info==NULL || pParameters->path_medit==NULL ||
                                                pParameters->path_mmg3d==NULL ||
                                              pParameters->path_mshdist==NULL ||
                                              pParameters->path_elastic==NULL ||
                                                 pParameters->path_advect==NULL)
            {
                expectedValue=0;
            }
            else if (pParameters->name_length<7 || pParameters->path_length<2 ||
                                             strlen(pParameters->name_info)<6 ||
                                             !strlen(pParameters->path_medit) ||
                                             !strlen(pParameters->path_mmg3d) ||
                                           !strlen(pParameters->path_mshdist) ||
                                           !strlen(pParameters->path_elastic) ||
                                              !strlen(pParameters->path_advect))
            {
                expectedValue=0;
            }
            else if (lengthMiddle<2 || lengthMiddle>11 ||
                                     lengthMiddle!=(int)strlen(keywordMiddle)+1)
            {
                expectedValue=0;
            }
            else if (lengthEnd<1 || lengthEnd>6 || counterLine<1 ||
                                                                 counterLine>70)
            {
                expectedValue=0;
            }
            else if (lengthEnd>1 && lengthEnd!=(int)strlen(keywordEnd)+1)
            {
                expectedValue=0;
            }
            else if (booleanForKeyword)
            {
                expectedValue=1;
            }
            else
            {
                expectedValue=0;
            }
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the value of the different variables
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%s\n",pStringToPrint[i],*pString[i]);
        }
        for (i=0; i<23; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<37; i++)
        {
            fprintf(stdout,"%s=%lf\n",pDoubleToPrint[i],*pDouble[i]);
        }
        fprintf(stdout,"keywordBeginning=");
        if (!booleanForKeywordBeginning)
        {
            for (i=0; i<3; i++)
            {
                fprintf(stdout,"%c",keywordBeginning[i]);
            }
        }
        else
        {
            fprintf(stdout,"%s",keywordBeginning);
        }
        fprintf(stdout,"\nkeywordMiddle=");
        if (!booleanForKeywordMiddle)
        {
            for (i=0; i<11; i++)
            {
                fprintf(stdout,"%c",keywordMiddle[i]);
            }
        }
        else
        {
            fprintf(stdout,"%s",keywordMiddle);
        }
        fprintf(stdout,"\nkeywordEnd=");
        if (!booleanForKeywordEnd)
        {
            for (i=0; i<6; i++)
            {
                fprintf(stdout,"%c",keywordEnd[i]);
            }
        }
        else
        {
            fprintf(stdout,"%s",keywordEnd);
        }
        fprintf(stdout,"\nreadStringIn=%s\n",readStringIn);
        fprintf(stdout,"(%ld characters)\n",lengthName);
        fprintf(stdout,"lengthMiddle=%d\n",lengthMiddle);
        fprintf(stdout,"lengthEnd=%d\n",lengthEnd);
        fprintf(stdout,"counter=%d\n",counterLine);
        fprintf(stdout,"readIntegerIn=%d\n",readIntegerIn);
        fprintf(stdout,"readDouble=%lf\n",readDouble);

        // Testing the function
        returnValue=changeValuesOfParameters(pParameters,keywordBeginning,
                                             keywordMiddle,keywordEnd,
                                             readStringIn,lengthMiddle,
                                             lengthEnd,counterLine,
                                                      readIntegerIn,readDouble);

        // Check that p(P)arameters has been correctly updated
        if (returnValue)
        {
            booleanForReturnValue=0;
            for (i=0; i<10; i++)
            {
                if (*pString[i]==NULL || *pStringOld[i]==NULL)
                {
                    booleanForReturnValue+=(*pString[i]==*pStringOld[i]);
                }
                else
                {
                    if (*pString[i]==*pStringOld[i])
                    {
                        booleanForReturnValue+=(!strcmp(*pString[i],
                                                               *pStringOld[i]));
                    }
                    else
                    {
                        booleanForReturnValue+=(!strcmp(*pString[i],
                                                                saveString[i]));
                    }
                }
            }
            for (i=0; i<23; i++)
            {
                booleanForReturnValue+=(*pInteger[i]==*pIntegerOld[i]);
            }
            for (i=0; i<37; i++)
            {
                booleanForReturnValue+=(*pDouble[i]==*pDoubleOld[i]);
            }
            strncpy(keyword,keywordBeginning,20);
            strcat(keyword,keywordMiddle);
            for (i=1; i<10; i++)
            {
                if (!strcmp(keyword,pStringToPrint[i]))
                {
                    if (*pString[i]==NULL || readStringIn==NULL)
                    {
                        booleanForReturnValue+=(*pString[i]==readStringIn);
                    }
                    else
                    {
                        booleanForReturnValue+=(!strcmp(*pString[i],
                                                                 readStringIn));
                    }
                }
            }
            for (i=0; i<23; i++)
            {
                if (!strcmp(keyword,pIntegerToPrint[i]))
                {
                    booleanForReturnValue+=(*pInteger[i]==readIntegerIn);
                }
            }
            for (i=0; i<37; i++)
            {
                if (!strcmp(keyword,pDoubleToPrint[i]))
                {
                    booleanForReturnValue+=(*pDouble[i]==readDouble);
                }
            }
            if (booleanForReturnValue<70)
            {
                strcat(keyword,keywordEnd);
                for (i=1; i<10; i++)
                {
                    if (!strcmp(keyword,pStringToPrint[i]))
                    {
                        if (*pString[i]==NULL || readStringIn==NULL)
                        {
                             booleanForReturnValue+=(*pString[i]==readStringIn);
                        }
                        else
                        {
                            booleanForReturnValue+=(!strcmp(*pString[i],
                                                                 readStringIn));
                        }
                    }
                }
                for (i=0; i<23; i++)
                {
                    if (!strcmp(keyword,pIntegerToPrint[i]))
                    {
                        booleanForReturnValue+=(*pInteger[i]==readIntegerIn);
                    }
                }
                for (i=0; i<37; i++)
                {
                    if (!strcmp(keyword,pDoubleToPrint[i]))
                    {
                        booleanForReturnValue+=(*pDouble[i]==readDouble);
                    }
                }
            }
            if (booleanForReturnValue<70)
            {
                if (expectedValue)
                {
                    returnValue=0;
                }
                fprintf(stdout,"\nAlthough changeValuesOfParameters function ");
                fprintf(stdout,"ended normally, the variables of the ");
                fprintf(stdout,"structure pointed by pParameters have not ");
                fprintf(stdout,"been correctly updated:\n");
                for (i=0; i<10; i++)
                {
                    fprintf(stdout,"%s=%s\n",pStringToPrint[i],*pString[i]);
                }
                for (i=0; i<23; i++)
                {
                    fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
                }
                for (i=0; i<37; i++)
                {
                    fprintf(stdout,"%s=%lf\n",pDoubleToPrint[i],*pDouble[i]);
                }
                fprintf(stdout,"keywordBeginning=");
                if (!booleanForKeywordBeginning)
                {
                    for (i=0; i<3; i++)
                    {
                        fprintf(stdout,"%c",keywordBeginning[i]);
                    }
                }
                else
                {
                    fprintf(stdout,"%s",keywordBeginning);
                }
                fprintf(stdout,"\nkeywordMiddle=");
                if (!booleanForKeywordMiddle)
                {
                    for (i=0; i<11; i++)
                    {
                        fprintf(stdout,"%c",keywordMiddle[i]);
                    }
                }
                else
                {
                    fprintf(stdout,"%s",keywordMiddle);
                }
                fprintf(stdout,"\nkeywordEnd=");
                if (!booleanForKeywordEnd)
                {
                    for (i=0; i<6; i++)
                    {
                        fprintf(stdout,"%c",keywordEnd[i]);
                    }
                }
                else
                {
                    fprintf(stdout,"%s",keywordEnd);
                }
                fprintf(stdout,"\nreadStringIn=%s\n",readStringIn);
                fprintf(stdout,"(%ld characters)\n",lengthName);
                fprintf(stdout,"lengthMiddle=%d\n",lengthMiddle);
                fprintf(stdout,"lengthEnd=%d\n",lengthEnd);
                fprintf(stdout,"counter=%d\n",counterLine);
                fprintf(stdout,"readIntegerIn=%d\n",readIntegerIn);
                fprintf(stdout,"readDouble=%lf\n",readDouble);
            }
        }

        // Free the memory allocated for strings
        for (i=0; i<10; i++)
        {
            free(*pString[i]);
            *pString[i]=NULL;
        }
        free(readStringIn);
        readStringIn=NULL;

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

// Unit (random) tests on readInfoFileAndGetParameters of loadParameters.c
void testReadInfoFileAndGetParameters(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    char keyword[20]={'\0'}, pStringToPrint[80][20]={""};
    char pValueToPrint[80][20]={""}, pSpaceStringValue[80][20]={""};
    char pSpaceValueString[80][20]={""}, *stringToPrint=NULL;
    char pSaveString[10][121]={""};
    size_t lengthName=0;
    int iRandom=0, i=0, iMax=0, j=0, k=0, integerToPrint=0, booleanForEndData=0;
    int booleanForName=0, booleanForPath=0, repetition[72]={0};
    double doubleToPrint=.0;
    FILE *infoFile=NULL;
    Parameters *pParameters=NULL, parameters, parametersRef;

    char pStringName[10][20]={"name_info","name_data","name_chem","name_mesh",
                              "name_elas","path_medit","path_mmg3d",
                                   "path_mshdist","path_elastic","path_advect"};
    char **pString[10]={&parameters.name_info,&parameters.name_data,
                        &parameters.name_chem,&parameters.name_mesh,
                        &parameters.name_elas,&parameters.path_medit,
                        &parameters.path_mmg3d,&parameters.path_mshdist,
                              &parameters.path_elastic,&parameters.path_advect};
    char **pStringRef[10]={&parametersRef.name_info,&parametersRef.name_data,
                           &parametersRef.name_chem,&parametersRef.name_mesh,
                           &parametersRef.name_elas,&parametersRef.path_medit,
                           &parametersRef.path_mmg3d,
                           &parametersRef.path_mshdist,
                           &parametersRef.path_elastic,
                                                    &parametersRef.path_advect};
    char pIntegerName[23][20]={"name_length","path_length","opt_mode","verbose",
                               "n_cpu","nu_electrons","nu_spin","orb_rhf","n_x",
                               "n_y","n_z","ls_type","trick_matrix",
                               "approx_mode","iter_max","save_type","save_mesh",
                               "save_data","save_print","save_where",
                                                 "hmode_lag","n_iter","no_cfl"};
    int *pInteger[23]={&parameters.name_length,&parameters.path_length,
                       &parameters.opt_mode,&parameters.verbose,
                       &parameters.n_cpu,&parameters.nu_electrons,
                       &parameters.nu_spin,&parameters.orb_rhf,&parameters.n_x,
                       &parameters.n_y,&parameters.n_z,&parameters.ls_type,
                       &parameters.trick_matrix,&parameters.approx_mode,
                       &parameters.iter_max,&parameters.save_type,
                       &parameters.save_mesh,&parameters.save_data,
                       &parameters.save_print,&parameters.save_where,
                       &parameters.hmode_lag,&parameters.n_iter,
                                                            &parameters.no_cfl};
    int *pIntegerRef[23]={&parametersRef.name_length,&parametersRef.path_length,
                          &parametersRef.opt_mode,&parametersRef.verbose,
                          &parametersRef.n_cpu,&parametersRef.nu_electrons,
                          &parametersRef.nu_spin,&parametersRef.orb_rhf,
                          &parametersRef.n_x,&parametersRef.n_y,
                          &parametersRef.n_z,&parametersRef.ls_type,
                          &parametersRef.trick_matrix,
                          &parametersRef.approx_mode,&parametersRef.iter_max,
                          &parametersRef.save_type,&parametersRef.save_mesh,
                          &parametersRef.save_data,&parametersRef.save_print,
                          &parametersRef.save_where,&parametersRef.hmode_lag,
                                   &parametersRef.n_iter,&parametersRef.no_cfl};
    char pDoubleName[37][20]={"x_min","x_max","y_min","y_max","z_min","z_max",
                              "delta_x","delta_y","delta_z","ls_x","ls_y",
                              "ls_z","ls_r","met_err","met_min","met_max",
                              "iter_told0p","iter_told1p","iter_told2p",
                              "hmin_iso","hmax_iso","hausd_iso","hgrad_iso",
                              "hmin_met","hmax_met","hausd_met","hgrad_met",
                              "hmin_ls","hmax_ls","hausd_ls","hgrad_ls",
                              "hmin_lag","hmax_lag","hausd_lag","hgrad_lag",
                                                          "residual","delta_t"};
    double *pDouble[37]={&parameters.x_min,&parameters.x_max,&parameters.y_min,
                         &parameters.y_max,&parameters.z_min,&parameters.z_max,
                         &parameters.delta_x,&parameters.delta_y,
                         &parameters.delta_z,&parameters.ls_x,&parameters.ls_y,
                         &parameters.ls_z,&parameters.ls_r,&parameters.met_err,
                         &parameters.met_min,&parameters.met_max,
                         &parameters.iter_told0p,&parameters.iter_told1p,
                         &parameters.iter_told2p,&parameters.hmin_iso,
                         &parameters.hmax_iso,&parameters.hausd_iso,
                         &parameters.hgrad_iso,&parameters.hmin_met,
                         &parameters.hmax_met,&parameters.hausd_met,
                         &parameters.hgrad_met,&parameters.hmin_ls,
                         &parameters.hmax_ls,&parameters.hausd_ls,
                         &parameters.hgrad_ls,&parameters.hmin_lag,
                         &parameters.hmax_lag,&parameters.hausd_lag,
                         &parameters.hgrad_lag,&parameters.residual,
                                                           &parameters.delta_t};
    double *pDoubleRef[37]={&parametersRef.x_min,&parametersRef.x_max,
                            &parametersRef.y_min,&parametersRef.y_max,
                            &parametersRef.z_min,&parametersRef.z_max,
                            &parametersRef.delta_x,&parametersRef.delta_y,
                            &parametersRef.delta_z,&parametersRef.ls_x,
                            &parametersRef.ls_y,&parametersRef.ls_z,
                            &parametersRef.ls_r,&parametersRef.met_err,
                            &parametersRef.met_min,&parametersRef.met_max,
                            &parametersRef.iter_told0p,
                            &parametersRef.iter_told1p,
                            &parametersRef.iter_told2p,&parametersRef.hmin_iso,
                            &parametersRef.hmax_iso,&parametersRef.hausd_iso,
                            &parametersRef.hgrad_iso,&parametersRef.hmin_met,
                            &parametersRef.hmax_met,&parametersRef.hausd_met,
                            &parametersRef.hgrad_met,&parametersRef.hmin_ls,
                            &parametersRef.hmax_ls,&parametersRef.hausd_ls,
                            &parametersRef.hgrad_ls,&parametersRef.hmin_lag,
                            &parametersRef.hmax_lag,&parametersRef.hausd_lag,
                            &parametersRef.hgrad_lag,&parametersRef.residual,
                                                        &parametersRef.delta_t};
    char pKeyword[71][20]={"not A Good Keyword","end_data","name_data",
                           "name_chem","name_mesh","name_elas","path_medit",
                           "path_mmg3d","path_mshdist","path_elastic",
                           "path_advect","name_length","path_length","opt_mode",
                           "verbose","n_cpu","nu_electrons","nu_spin","orb_rhf",
                           "n_x","n_y","n_z","ls_type","trick_matrix",
                           "approx_mode","iter_max","save_type","save_mesh",
                           "save_data","save_print","save_where","hmode_lag",
                           "n_iter","no_cfl","x_min","x_max","y_min","y_max",
                           "z_min","z_max","delta_x","delta_y","delta_z","ls_x",
                           "ls_y","ls_z","ls_r","met_err","met_min","met_max",
                           "iter_told0p","iter_told1p","iter_told2p","hmin_iso",
                           "hmax_iso","hausd_iso","hgrad_iso","hmin_met",
                           "hmax_met","hausd_met","hgrad_met","hmin_ls",
                           "hmax_ls","hausd_ls","hgrad_ls","hmin_lag",
                           "hmax_lag","hausd_lag","hgrad_lag","residual",
                                                                     "delta_t"};
    char pSpace[35][20]={""," ","  ","   ","\n"," \n","\n "," \n ","  \n",
                         "  \n ","  \n  "," "," \n  ","\n  ","\n\n"," \n\n",
                         "\n\n "," \n\n ","\n \n "," \n \n"," "," \n \n ",
                         "  \n\n","\n\n  "," \n\n  ","  \n\n ","  \n\n  ",
                         "  \n \n","\n \n  "," \n  \n ","  \n  \n  "," \n  \n",
                                               "\n  \n ","\n\n\n","\n\n\n\n\n"};
    char pIntegerToPrint[28][20]={"0.0","10e","9.82","-19873","-0151","-1",
                                  "-01","0","0000","1","001","987","-987383038",
                                  "1029","0012837","0111","012745","5456480",
                                  "9","-7","089","-00001","-00","89","-238",
                                                     "2394002","200002","2939"};
    char pDoubleToPrint[2192][20]={"-e00","-E00","-e+00","-E+00","-e-00",
                                   "-E-00","-e01","-E01","-e+01","-E+01",
                                   "-e-01","-E-01","-e010","-E010","-e+010",
                                   "-E+010","-e-010","-E-010","-e10","-E10",
                                   "-e+10","-E+10","-e-10","-E-10","-e2","-E2",
                                   "-e+2","-E+2","-e-2","-E-2","-e0","-E0",
                                   "-e+0","-E+0","-e-0","-E-0","-e","-E","-e+",
                                   "-E+","-e-","-E-","-","e00","E00","e+00",
                                   "E+00","e-00","E-00","e01","E01","e+01",
                                   "E+01","e-01","E-01","e010","E010","e+010",
                                   "E+010","e-010","E-010","e10","E10","e+10",
                                   "E+10","e-10","E-10","e2","E2","e+2","E+2",
                                   "e-2","E-2","e0","E0","e+0","E+0","e-0",
                                   "E-0","e","E","e+","E+","e-","E-","+e00",
                                   "+E00","+e+00","+E+00","+e-00","+E-00",
                                   "+e01","+E01","+e+01","+E+01","+e-01",
                                   "+E-01","+e010","+E010","+e+010","+E+010",
                                   "+e-010","+E-010","+e10","+E10","+e+10",
                                   "+E+10","+e-10","+E-10","+e2","+E2","+e+2",
                                   "+E+2","+e-2","+E-2","+e0","+E0","+e+0",
                                   "+E+0","+e-0","+E-0","+e","+E","+e+","+E+",
                                   "+e-","+E-","+",
                                   "-45.00e00","-45.00E00","-45.00e+00",
                                   "-45.00E+00","-45.00e-00","-45.00E-00",
                                   "-45.00e01","-45.00E01","-45.00e+01",
                                   "-45.00E+01","-45.00e-01","-45.00E-01",
                                   "-45.00e010","-45.00E010","-45.00e+010",
                                   "-45.00E+010","-45.00e-010","-45.00E-010",
                                   "-45.00e10","-45.00E10","-45.00e+10",
                                   "-45.00E+10","-45.00e-10","-45.00E-10",
                                   "-45.00e2","-45.00E2","-45.00e+2",
                                   "-45.00E+2","-45.00e-2","-45.00E-2",
                                   "-45.00e0","-45.00E0","-45.00e+0",
                                   "-45.00E+0","-45.00e-0","-45.00E-0",
                                   "-45.00e","-45.00E","-45.00e+","-45.00E+",
                                   "-45.00e-","-45.00E-","-45.00","-045.0e00",
                                   "-045.0E00","-045.0e+00","-045.0E+00",
                                   "-045.0e-00","-045.0E-00","-045.0e01",
                                   "-045.0E01","-045.0e+01","-045.0E+01",
                                   "-045.0e-01","-045.0E-01","-045.0e010",
                                   "-045.0E010","-045.0e+010","-045.0E+010",
                                   "-045.0e-010","-045.0E-010","-045.0e10",
                                   "-045.0E10","-045.0e+10","-045.0E+10",
                                   "-045.0e-10","-045.0E-10","-045.0e2",
                                   "-045.0E2","-045.0e+2","-045.0E+2",
                                   "-045.0e-2","-045.0E-2","-045.0e0",
                                   "-045.0E0","-045.0e+0","-045.0E+0",
                                   "-045.0e-0","-045.0E-0","-045.0e","-045.0E",
                                   "-045.0e+","-045.0E+","-045.0e-","-045.0E-",
                                   "-045.0","-45.e00","-45.E00","-45.e+00",
                                   "-45.E+00","-45.e-00","-45.E-00","-45.e01",
                                   "-45.E01","-45.e+01","-45.E+01","-45.e-01",
                                   "-45.E-01","-45.e010","-45.E010","-45.e+010",
                                   "-45.E+010","-45.e-010","-45.E-010",
                                   "-45.e10","-45.E10","-45.e+10","-45.E+10",
                                   "-45.e-10","-45.E-10","-45.e2","-45.E2",
                                   "-45.e+2","-45.E+2","-45.e-2","-45.E-2",
                                   "-45.e0","-45.E0","-45.e+0","-45.E+0",
                                   "-45.e-0","-45.E-0","-45.e","-45.E","-45.e+",
                                   "-45.E+","-45.e-","-45.E-","-45.",
                                   "-0045.e00","-0045.E00","-0045.e+00",
                                   "-0045.E+00","-0045.e-00","-0045.E-00",
                                   "-0045.e01","-0045.E01","-0045.e+01",
                                   "-0045.E+01","-0045.e-01","-0045.E-01",
                                   "-0045.e010","-0045.E010","-0045.e+010",
                                   "-0045.E+010","-0045.e-010","-0045.E-010",
                                   "-0045.e10","-0045.E10","-0045.e+10",
                                   "-0045.E+10","-0045.e-10","-0045.E-10",
                                   "-0045.e2","-0045.E2","-0045.e+2",
                                   "-0045.E+2","-0045.e-2","-0045.E-2",
                                   "-0045.e0","-0045.E0","-0045.e+0",
                                   "-0045.E+0","-0045.e-0","-0045.E-0",
                                   "-0045.e","-0045.E","-0045.e+","-0045.E+",
                                   "-0045.e-","-0045.E-","-0045.","-45.54e00",
                                   "-45.54E00","-45.54e+00","-45.54E+00",
                                   "-45.54e-00","-45.54E-00","-45.54e01",
                                   "-45.54E01","-45.54e+01","-45.54E+01",
                                   "-45.54e-01","-45.54E-01","-45.54e010",
                                   "-45.54E010","-45.54e+010","-45.54E+010",
                                   "-45.54e-010","-45.54E-010","-45.54e10",
                                   "-45.54E10","-45.54e+10","-45.54E+10",
                                   "-45.54e-10","-45.54E-10","-45.54e2",
                                   "-45.54E2","-45.54e+2","-45.54E+2",
                                   "-45.54e-2","-45.54E-2","-45.54e0",
                                   "-45.54E0","-45.54e+0","-45.54E+0",
                                   "-45.54e-0","-45.54E-0","-45.54e","-45.54E",
                                   "-45.54e+","-45.54E+","-45.54e-","-45.54E-",
                                   "-45.54","-045.054e00","-045.054E00",
                                   "-045.054e+00","-045.054E+00","-045.054e-00",
                                   "-045.054E-00","-045.054e01","-045.054E01",
                                   "-045.054e+01","-045.054E+01","-045.054e-01",
                                   "-045.054E-01","-045.054e010","-045.054E010",
                                   "-045.054e+010","-045.054E+010",
                                   "-045.054e-010","-045.054E-010",
                                   "-045.054e10","-045.054E10","-045.054e+10",
                                   "-045.054E+10","-045.054e-10","-045.054E-10",
                                   "-045.054e2","-045.054E2","-045.054e+2",
                                   "-045.054E+2","-045.054e-2","-045.054E-2",
                                   "-045.054e0","-045.054E0","-045.054e+0",
                                   "-045.054E+0","-045.054e-0","-045.054E-0",
                                   "-045.054e","-045.054E","-045.054e+",
                                   "-045.054E+","-045.054e-","-045.054E-",
                                   "-045.054","-000.054e00","-000.054E00",
                                   "-000.054e+00","-000.054E+00","-000.054e-00",
                                   "-000.054E-00","-000.054e01","-000.054E01",
                                   "-000.054e+01","-000.054E+01","-000.054e-01",
                                   "-000.054E-01","-000.054e010","-000.054E010",
                                   "-000.054e+010","-000.054E+010",
                                   "-000.054e-010","-000.054E-010",
                                   "-000.054e10","-000.054E10","-000.054e+10",
                                   "-000.054E+10","-000.054e-10","-000.054E-10",
                                   "-000.054e2","-000.054E2","-000.054e+2",
                                   "-000.054E+2","-000.054e-2","-000.054E-2",
                                   "-000.054e0","-000.054E0","-000.054e+0",
                                   "-000.054E+0","-000.054e-0","-000.054E-0",
                                   "-000.054e","-000.054E","-000.054e+",
                                   "-000.054E+","-000.054e-","-000.054E-",
                                   "-000.054","-0.054e00","-0.054E00",
                                   "-0.054e+00","-0.054E+00","-0.054e-00",
                                   "-0.054E-00","-0.054e01","-0.054E01",
                                   "-0.054e+01","-0.054E+01","-0.054e-01",
                                   "-0.054E-01","-0.054e010","-0.054E010",
                                   "-0.054e+010","-0.054E+010","-0.054e-010",
                                   "-0.054E-010","-0.054e10","-0.054E10",
                                   "-0.054e+10","-0.054E+10","-0.054e-10",
                                   "-0.054E-10","-0.054e2","-0.054E2",
                                   "-0.054e+2","-0.054E+2","-0.054e-2",
                                   "-0.054E-2","-0.054e0","-0.054E0",
                                   "-0.054e+0","-0.054E+0","-0.054e-0",
                                   "-0.054E-0","-0.054e","-0.054E","-0.054e+",
                                   "-0.054E+","-0.054e-","-0.054E-","-0.054",
                                   "-.054e00","-.054E00","-.054e+00",
                                   "-.054E+00","-.054e-00","-.054E-00",
                                   "-.054e01","-.054E01","-.054e+01",
                                   "-.054E+01","-.054e-01","-.054E-01",
                                   "-.054e010","-.054E010","-.054e+010",
                                   "-.054E+010","-.054e-010","-.054E-010",
                                   "-.054e10","-.054E10","-.054e+10",
                                   "-.054E+10","-.054e-10","-.054E-10",
                                   "-.054e2","-.054E2","-.054e+2","-.054E+2",
                                   "-.054e-2","-.054E-2","-.054e0","-.054E0",
                                   "-.054e+0","-.054E+0","-.054e-0","-.054E-0",
                                   "-.054e","-.054E","-.054e+","-.054E+",
                                   "-.054e-","-.054E-","-.054","-45e00",
                                   "-45E00","-45e+00","-45E+00","-45e-00",
                                   "-45E-00","-45e01","-45E01","-45e+01",
                                   "-45E+01","-45e-01","-45E-01","-45e010",
                                   "-45E010","-45e+010","-45E+010","-45e-010",
                                   "-45E-010","-45e10","-45E10","-45e+10",
                                   "-45E+10","-45e-10","-45E-10","-45e2",
                                   "-45E2","-45e+2","-45E+2","-45e-2","-45E-2",
                                   "-45e0","-45E0","-45e+0","-45E+0","-45e-0",
                                   "-45E-0","-45e","-45E","-45e+","-45E+",
                                   "-45e-","-45E-","-45","-0045e00","-0045E00",
                                   "-0045e+00","-0045E+00","-0045e-00",
                                   "-0045E-00","-0045e01","-0045E01",
                                   "-0045e+01","-0045E+01","-0045e-01",
                                   "-0045E-01","-0045e010","-0045E010",
                                   "-0045e+010","-0045E+010","-0045e-010",
                                   "-0045E-010","-0045e10","-0045E10",
                                   "-0045e+10","-0045E+10","-0045e-10",
                                   "-0045E-10","-0045e2","-0045E2",
                                   "-0045e+2","-0045E+2","-0045e-2","-0045E-2",
                                   "-0045e0","-0045E0","-0045e+0","-0045E+0",
                                   "-0045e-0","-0045E-0","-0045e","-0045E",
                                   "-0045e+","-0045E+","-0045e-","-0045E-",
                                   "-0045","45.00e00","45.00E00","45.00e+00",
                                   "45.00E+00","45.00e-00","45.00E-00",
                                   "45.00e01","45.00E01","45.00e+01",
                                   "45.00E+01","45.00e-01","45.00E-01",
                                   "45.00e010","45.00E010","45.00e+010",
                                   "45.00E+010","45.00e-010","45.00E-010",
                                   "45.00e10","45.00E10","45.00e+10",
                                   "45.00E+10","45.00e-10","45.00E-10",
                                   "45.00e2","45.00E2","45.00e+2","45.00E+2",
                                   "45.00e-2","45.00E-2","45.00e0","45.00E0",
                                   "45.00e+0","45.00E+0","45.00e-0","45.00E-0",
                                   "45.00e","45.00E","45.00e+","45.00E+",
                                   "45.00e-","45.00E-","45.00","045.0e00",
                                   "045.0E00","045.0e+00","045.0E+00",
                                   "045.0e-00","045.0E-00","045.0e01",
                                   "045.0E01","045.0e+01","045.0E+01",
                                   "045.0e-01","045.0E-01","045.0e010",
                                   "045.0E010","045.0e+010","045.0E+010",
                                   "045.0e-010","045.0E-010","045.0e10",
                                   "045.0E10","045.0e+10","045.0E+10",
                                   "045.0e-10","045.0E-10","045.0e2","045.0E2",
                                   "045.0e+2","045.0E+2","045.0e-2","045.0E-2",
                                   "045.0e0","045.0E0","045.0e+0","045.0E+0",
                                   "045.0e-0","045.0E-0","045.0e","045.0E",
                                   "045.0e+","045.0E+","045.0e-","045.0E-",
                                   "045.0","45.e00","45.E00","45.e+00",
                                   "45.E+00","45.e-00","45.E-00","45.e01",
                                   "45.E01","45.e+01","45.E+01","45.e-01",
                                   "45.E-01","45.e010","45.E010","45.e+010",
                                   "45.E+010","45.e-010","45.E-010","45.e10",
                                   "45.E10","45.e+10","45.E+10","45.e-10",
                                   "45.E-10","45.e2","45.E2","45.e+2","45.E+2",
                                   "45.e-2","45.E-2","45.e0","45.E0","45.e+0",
                                   "45.E+0","45.e-0","45.E-0","45.e","45.E",
                                   "45.e+","45.E+","45.e-","45.E-","45.",
                                   "0045.e00","0045.E00","0045.e+00",
                                   "0045.E+00","0045.e-00","0045.E-00",
                                   "0045.e01","0045.E01","0045.e+01",
                                   "0045.E+01","0045.e-01","0045.E-01",
                                   "0045.e010","0045.E010","0045.e+010",
                                   "0045.E+010","0045.e-010","0045.E-010",
                                   "0045.e10","0045.E10","0045.e+10",
                                   "0045.E+10","0045.e-10","0045.E-10",
                                   "0045.e2","0045.E2","0045.e+2","0045.E+2",
                                   "0045.e-2","0045.E-2","0045.e0","0045.E0",
                                   "0045.e+0","0045.E+0","0045.e-0","0045.E-0",
                                   "0045.e","0045.E","0045.e+","0045.E+",
                                   "0045.e-","0045.E-","0045.","45.54e00",
                                   "45.54E00","45.54e+00","45.54E+00",
                                   "45.54e-00","45.54E-00","45.54e01",
                                   "45.54E01","45.54e+01","45.54E+01",
                                   "45.54e-01","45.54E-01","45.54e010",
                                   "45.54E010","45.54e+010","45.54E+010",
                                   "45.54e-010","45.54E-010","45.54e10",
                                   "45.54E10","45.54e+10","45.54E+10",
                                   "45.54e-10","45.54E-10","45.54e2","45.54E2",
                                   "45.54e+2","45.54E+2","45.54e-2","45.54E-2",
                                   "45.54e0","45.54E0","45.54e+0","45.54E+0",
                                   "45.54e-0","45.54E-0","45.54e","45.54E",
                                   "45.54e+","45.54E+","45.54e-","45.54E-",
                                   "45.54","045.054e00","045.054E00",
                                   "045.054e+00","045.054E+00","045.054e-00",
                                   "045.054E-00","045.054e01","045.054E01",
                                   "045.054e+01","045.054E+01","045.054e-01",
                                   "045.054E-01","045.054e010","045.054E010",
                                   "045.054e+010","045.054E+010","045.054e-010",
                                   "045.054E-010","045.054e10","045.054E10",
                                   "045.054e+10","045.054E+10","045.054e-10",
                                   "045.054E-10","045.054E2","045.054e+2",
                                   "045.054E+2","045.054e-2","045.054e2",
                                   "045.054E-2","045.054e0","045.054E0",
                                   "045.054e+0","045.054E+0","045.054e-0",
                                   "045.054E-0","045.054e","045.054E",
                                   "045.054e+","045.054E+","045.054e-",
                                   "045.054E-","045.054","000.054e00",
                                   "000.054E00","000.054e+00","000.054E+00",
                                   "000.054e-00","000.054E-00","000.054e01",
                                   "000.054E01","000.054e+01","000.054E+01",
                                   "000.054e-01","000.054E-01","000.054e010",
                                   "000.054E010","000.054e+010","000.054E+010",
                                   "000.054e-010","000.054E-010","000.054e10",
                                   "000.054E10","000.054e+10","000.054E+10",
                                   "000.054e-10","000.054E-10","000.054e2",
                                   "000.054E2","000.054e+2","000.054E+2",
                                   "000.054e-2","000.054E-2","000.054e0",
                                   "000.054E0","000.054e+0","000.054E+0",
                                   "000.054e-0","000.054E-0","000.054e",
                                   "000.054E","000.054e+","000.054E+",
                                   "000.054e-","000.054E-","000.054","0.054e00",
                                   "0.054E00","0.054e+00","0.054E+00",
                                   "0.054e-00","0.054E-00","0.054e01",
                                   "0.054E01","0.054e+01","0.054E+01",
                                   "0.054e-01","0.054E-01","0.054e010",
                                   "0.054E010","0.054e+010","0.054E+010",
                                   "0.054e-010","0.054E-010","0.054e10",
                                   "0.054E10","0.054e+10","0.054E+10",
                                   "0.054e-10","0.054E-10","0.054e2","0.054E2",
                                   "0.054e+2","0.054E+2","0.054e-2","0.054E-2",
                                   "0.054e0","0.054E0","0.054e+0","0.054E+0",
                                   "0.054e-0","0.054E-0","0.054e","0.054E",
                                   "0.054e+","0.054E+","0.054e-","0.054E-",
                                   "0.054",".054e00",".054E00",".054e+00",
                                   ".054E+00",".054e-00",".054E-00",".054e01",
                                   ".054E01",".054e+01",".054E+01",".054e-01",
                                   ".054E-01",".054e010",".054E010",".054e+010",
                                   ".054E+010",".054e-010",".054E-010",
                                   ".054e10",".054E10",".054e+10",".054E+10",
                                   ".054e-10",".054E-10",".054e2",".054E2",
                                   ".054e+2",".054E+2",".054e-2",".054E-2",
                                   ".054e0",".054E0",".054e+0",".054E+0",
                                   ".054e-0",".054E-0",".054e",".054E",".054e+",
                                   ".054E+",".054e-",".054E-",".054","45e00",
                                   "45E00","45e+00","45E+00","45e-00","45E-00",
                                   "45e01","45E01","45e+01","45E+01","45e-01",
                                   "45E-01","45e010","45E010","45e+010",
                                   "45E+010","45e-010","45E-010","45e10",
                                   "45E10","45e+10","45E+10","45e-10","45E-10",
                                   "45e2","45E2","45e+2","45E+2","45e-2",
                                   "45E-2","45e0","45E0","45e+0","45E+0",
                                   "45e-0","45E-0","45e","45E","45e+","45E+",
                                   "45e-","45E-","45","0045e00","0045E00",
                                   "0045e+00","0045E+00","0045e-00","0045E-00",
                                   "0045e01","0045E01","0045e+01","0045E+01",
                                   "0045e-01","0045E-01","0045e010","0045E010",
                                   "0045e+010","0045E+010","0045e-010",
                                   "0045E-010","0045e10","0045E10","0045e+10",
                                   "0045E+10","0045e-10","0045E-10","0045e2",
                                   "0045E2","0045e+2","0045E+2","0045e-2",
                                   "0045E-2","0045e0","0045E0","0045e+0",
                                   "0045E+0","0045e-0","0045E-0","0045e",
                                   "0045E","0045e+","0045E+","0045e-","0045E-",
                                   "0045","+45.00e00","+45.00E00","+45.00e+00",
                                   "+45.00E+00","+45.00e-00","+45.00E-00",
                                   "+45.00e01","+45.00E01","+45.00e+01",
                                   "+45.00E+01","+45.00e-01","+45.00E-01",
                                   "+45.00e010","+45.00E010","+45.00e+010",
                                   "+45.00E+010","+45.00e-010","+45.00E-010",
                                   "+45.00e10","+45.00E10","+45.00e+10",
                                   "+45.00E+10","+45.00e-10","+45.00E-10",
                                   "+45.00e2","+45.00E2","+45.00e+2",
                                   "+45.00E+2","+45.00e-2","+45.00E-2",
                                   "+45.00e0","+45.00E0","+45.00e+0",
                                   "+45.00E+0","+45.00e-0","+45.00E-0",
                                   "+45.00e","+45.00E","+45.00e+","+45.00E+",
                                   "+45.00e-","+45.00E-","+45.00","+045.0e00",
                                   "+045.0E00","+045.0e+00","+045.0E+00",
                                   "+045.0e-00","+045.0E-00","+045.0e01",
                                   "+045.0E01","+045.0e+01","+045.0E+01",
                                   "+045.0e-01","+045.0E-01","+045.0e010",
                                   "+045.0E010","+045.0e+010","+045.0E+010",
                                   "+045.0e-010","+045.0E-010","+045.0e10",
                                   "+045.0E10","+045.0e+10","+045.0E+10",
                                   "+045.0e-10","+045.0E-10","+045.0e2",
                                   "+045.0E2","+045.0e+2","+045.0E+2",
                                   "+045.0e-2","+045.0E-2","+045.0e0",
                                   "+045.0E0","+045.0e+0","+045.0E+0",
                                   "+045.0e-0","+045.0E-0","+045.0e","+045.0E",
                                   "+045.0e+","+045.0E+","+045.0e-","+045.0E-",
                                   "+045.0","+45.e00","+45.E00","+45.e+00",
                                   "+45.E+00","+45.e-00","+45.E-00","+45.e01",
                                   "+45.E01","+45.e+01","+45.E+01","+45.e-01",
                                   "+45.E-01","+45.e010","+45.E010","+45.e+010",
                                   "+45.E+010","+45.e-010","+45.E-010",
                                   "+45.e10","+45.E10","+45.e+10","+45.E+10",
                                   "+45.e-10","+45.E-10","+45.e2","+45.E2",
                                   "+45.e+2","+45.E+2","+45.e-2","+45.E-2",
                                   "+45.e0","+45.E0","+45.e+0","+45.E+0",
                                   "+45.e-0","+45.E-0","+45.e","+45.E","+45.e+",
                                   "+45.E+","+45.e-","+45.E-","+45.",
                                   "+0045.e00","+0045.E00","+0045.e+00",
                                   "+0045.E+00","+0045.e-00","+0045.E-00",
                                   "+0045.e01","+0045.E01","+0045.e+01",
                                   "+0045.E+01","+0045.e-01","+0045.E-01",
                                   "+0045.e010","+0045.E010","+0045.e+010",
                                   "+0045.E+010","+0045.e-010","+0045.E-010",
                                   "+0045.e10","+0045.E10","+0045.e+10",
                                   "+0045.E+10","+0045.e-10","+0045.E-10",
                                   "+0045.e2","+0045.E2","+0045.e+2",
                                   "+0045.E+2","+0045.e-2","+0045.E-2",
                                   "+0045.e0","+0045.E0","+0045.e+0",
                                   "+0045.E+0","+0045.e-0","+0045.E-0",
                                   "+0045.e","+0045.E","+0045.e+","+0045.E+",
                                   "+0045.e-","+0045.E-","+0045.","+45.54e00",
                                   "+45.54E00","+45.54e+00","+45.54E+00",
                                   "+45.54e-00","+45.54E-00","+45.54e01",
                                   "+45.54E01","+45.54e+01","+45.54E+01",
                                   "+45.54e-01","+45.54E-01","+45.54e010",
                                   "+45.54E010","+45.54e+010","+45.54E+010",
                                   "+45.54e-010","+45.54E-010","+45.54e10",
                                   "+45.54E10","+45.54e+10","+45.54E+10",
                                   "+45.54e-10","+45.54E-10","+45.54e2",
                                   "+45.54E2","+45.54e+2","+45.54E+2",
                                   "+45.54e-2","+45.54E-2","+45.54e0",
                                   "+45.54E0","+45.54e+0","+45.54E+0",
                                   "+45.54e-0","+45.54E-0","+45.54e","+45.54E",
                                   "+45.54e+","+45.54E+","+45.54e-","+45.54E-",
                                   "+45.54","+045.054e00","+045.054E00",
                                   "+045.054e+00","+045.054E+00","+045.054e-00",
                                   "+045.054E-00","+045.054e01","+045.054E01",
                                   "+045.054e+01","+045.054E+01","+045.054e-01",
                                   "+045.054E-01","+045.054e010","+045.054E010",
                                   "+045.054e+010","+045.054E+010",
                                   "+045.054e-010","+045.054E-010",
                                   "+045.054e10","+045.054E10","+045.054e+10",
                                   "+045.054E+10","+045.054e-10","+045.054E-10",
                                   "+045.054e2","+045.054E2","+045.054e+2",
                                   "+045.054E+2","+045.054e-2","+045.054E-2",
                                   "+045.054e0","+045.054E0","+045.054e+0",
                                   "+045.054E+0","+045.054e-0","+045.054E-0",
                                   "+045.054e","+045.054E","+045.054e+",
                                   "+045.054E+","+045.054e-","+045.054E-",
                                   "+045.054","+000.054e00","+000.054E00",
                                   "+000.054e+00","+000.054E+00","+000.054e-00",
                                   "+000.054E-00","+000.054e01","+000.054E01",
                                   "+000.054e+01","+000.054E+01","+000.054e-01",
                                   "+000.054E-01","+000.054e010","+000.054E010",
                                   "+000.054e+010","+000.054E+010",
                                   "+000.054e-010","+000.054E-010",
                                   "+000.054e10","+000.054E10","+000.054e+10",
                                   "+000.054E+10","+000.054e-10","+000.054E-10",
                                   "+000.054e2","+000.054E2","+000.054e+2",
                                   "+000.054E+2","+000.054e-2","+000.054E-2",
                                   "+000.054e0","+000.054E0","+000.054e+0",
                                   "+000.054E+0","+000.054e-0","+000.054E-0",
                                   "+000.054e","+000.054E","+000.054e+",
                                   "+000.054E+","+000.054e-","+000.054E-",
                                   "+000.054","+0.054e00","+0.054E00",
                                   "+0.054e+00","+0.054E+00","+0.054e-00",
                                   "+0.054E-00","+0.054e01","+0.054E01",
                                   "+0.054e+01","+0.054E+01","+0.054e-01",
                                   "+0.054E-01","+0.054e010","+0.054E010",
                                   "+0.054e+010","+0.054E+010","+0.054e-010",
                                   "+0.054E-010","+0.054e10","+0.054E10",
                                   "+0.054e+10","+0.054E+10","+0.054e-10",
                                   "+0.054E-10","+0.054e2","+0.054E2",
                                   "+0.054e+2","+0.054E+2","+0.054e-2",
                                   "+0.054E-2","+0.054e0","+0.054E0",
                                   "+0.054e+0","+0.054E+0","+0.054e-0",
                                   "+0.054E-0","+0.054e","+0.054E","+0.054e+",
                                   "+0.054E+","+0.054e-","+0.054E-","+0.054",
                                   "+.054e00","+.054E00","+.054e+00",
                                   "+.054E+00","+.054e-00","+.054E-00",
                                   "+.054e01","+.054E01","+.054e+01",
                                   "+.054E+01","+.054e-01","+.054E-01",
                                   "+.054e010","+.054E010","+.054e+010",
                                   "+.054E+010","+.054e-010","+.054E-010",
                                   "+.054e10","+.054E10","+.054e+10",
                                   "+.054E+10","+.054e-10","+.054E-10",
                                   "+.054e2","+.054E2","+.054e+2","+.054E+2",
                                   "+.054e-2","+.054E-2","+.054e0","+.054E0",
                                   "+.054e+0","+.054E+0","+.054e-0","+.054E-0",
                                   "+.054e","+.054E","+.054e+","+.054E+",
                                   "+.054e-","+.054E-","+.054","+45e00",
                                   "+45E00","+45e+00","+45E+00","+45e-00",
                                   "+45E-00","+45e01","+45E01","+45e+01",
                                   "+45E+01","+45e-01","+45E-01","+45e010",
                                   "+45E010","+45e+010","+45E+010","+45e-010",
                                   "+45E-010","+45e10","+45E10","+45e+10",
                                   "+45E+10","+45e-10","+45E-10","+45e2",
                                   "+45E2","+45e+2","+45E+2","+45e-2","+45E-2",
                                   "+45e0","+45E0","+45e+0","+45E+0","+45e-0",
                                   "+45E-0","+45e","+45E","+45e+","+45E+",
                                   "+45e-","+45E-","+45","+0045e00","+0045E00",
                                   "+0045e+00","+0045E+00","+0045e-00",
                                   "+0045E-00","+0045e01","+0045E01",
                                   "+0045e+01","+0045E+01","+0045e-01",
                                   "+0045E-01","+0045e010","+0045E010",
                                   "+0045e+010","+0045E+010","+0045e-010",
                                   "+0045E-010","+0045e10","+0045E10",
                                   "+0045e+10","+0045E+10","+0045e-10",
                                   "+0045E-10","+0045e2","+0045E2","+0045e+2",
                                   "+0045E+2","+0045e-2","+0045E-2","+0045e0",
                                   "+0045E0","+0045e+0","+0045E+0","+0045e-0",
                                   "+0045E-0","+0045e","+0045E","+0045e+",
                                   "+0045E+","+0045e-","+0045E-","+0045",
                                   "00.00e00","00.00E00","00.00e+00",
                                   "00.00E+00","00.00e-00","00.00E-00",
                                   "00.00e01","00.00E01","00.00e+01",
                                   "00.00E+01","00.00e-01","00.00E-01",
                                   "00.00e010","00.00E010","00.00e+010",
                                   "00.00E+010","00.00e-010","00.00E-010",
                                   "00.00e10","00.00E10","00.00e+10",
                                   "00.00E+10","00.00e-10","00.00E-10",
                                   "00.00e2","00.00E2","00.00e+2","00.00E+2",
                                   "00.00e-2","00.00E-2","00.00e0","00.00E0",
                                   "00.00e+0","00.00E+0","00.00e-0","00.00E-0",
                                   "00.00e","00.00E","00.00e+","00.00E+",
                                   "00.00e-","00.00E-","00.00",".0e00",".0E00",
                                   ".0e+00",".0E+00",".0e-00",".0E-00",".0e01",
                                   ".0E01",".0e+01",".0E+01",".0e-01",".0E-01",
                                   ".0e010",".0E010",".0e+010",".0E+010",
                                   ".0e-010",".0E-010",".0e10",".0E10",".0e+10",
                                   ".0E+10",".0e-10",".0E-10",".0e2",".0E2",
                                   ".0e+2",".0E+2",".0e-2",".0E-2",".0e0",
                                   ".0E0",".0e+0",".0E+0",".0e-0",".0E-0",".0e",
                                   ".0E",".0e+",".0E+",".0e-",".0E-",".0",
                                   "0.e00","0.E00","0.e+00","0.E+00","0.e-00",
                                   "0.E-00","0.e01","0.E01","0.e+01","0.E+01",
                                   "0.e-01","0.E-01","0.e010","0.E010",
                                   "0.e+010","0.E+010","0.e-010","0.E-010",
                                   "0.e10","0.E10","0.e+10","0.E+10","0.e-10",
                                   "0.E-10","0.e2","0.E2","0.e+2","0.E+2",
                                   "0.e-2","0.E-2","0.e0","0.E0","0.e+0",
                                   "0.E+0","0.e-0","0.E-0","0.e","0.E","0.e+",
                                   "0.E+","0.e-","0.E-","0.","00e00","00E00",
                                   "00e+00","00E+00","00e-00","00E-00","00e01",
                                   "00E01","00e+01","00E+01","00e-01","00E-01",
                                   "00e010","00E010","00e+010","00E+010",
                                   "00e-010","00E-010","00e10","00E10","00e+10",
                                   "00E+10","00e-10","00E-10","00e2","00E2",
                                   "00e+2","00E+2","00e-2","00E-2","00e0",
                                   "00E0","00e+0","00E+0","00e-0","00E-0","00e",
                                   "00E","00e+","00E+","00e-","00E-","00",
                                   "0e00","0E00","0e+00","0E+00","0e-00",
                                   "0E-00","0e01","0E01","0e+01","0E+01",
                                   "0e-01","0E-01","0e010","0E010","0e+010",
                                   "0E+010","0e-010","0E-010","0e10","0E10",
                                   "0e+10","0E+10","0e-10","0E-10","0e2","0E2",
                                   "0e+2","0E+2","0e-2","0E-2","0e0","0E0",
                                   "0e+0","0E+0","0e-0","0E-0","0e","0E","0e+",
                                   "0E+","0e-","0E-","0","-00.00e00",
                                   "-00.00E00","-00.00e+00","-00.00E+00",
                                   "-00.00e-00","-00.00E-00","-00.00e01",
                                   "-00.00E01","-00.00e+01","-00.00E+01",
                                   "-00.00e-01","-00.00E-01","-00.00e010",
                                   "-00.00E010","-00.00e+010","-00.00E+010",
                                   "-00.00e-010","-00.00E-010","-00.00e10",
                                   "-00.00E10","-00.00e+10","-00.00E+10",
                                   "-00.00e-10","-00.00E-10","-00.00e2",
                                   "-00.00E2","-00.00e+2","-00.00E+2",
                                   "-00.00e-2","-00.00E-2","-00.00e0",
                                   "-00.00E0","-00.00e+0","-00.00E+0",
                                   "-00.00e-0","-00.00E-0","-00.00e","-00.00E",
                                   "-00.00e+","-00.00E+","-00.00e-","-00.00E-",
                                   "-00.00","-.0e00","-.0E00","-.0e+00",
                                   "-.0E+00","-.0e-00","-.0E-00","-.0e01",
                                   "-.0E01","-.0e+01","-.0E+01","-.0e-01",
                                   "-.0E-01","-.0e010","-.0E010","-.0e+010",
                                   "-.0E+010","-.0e-010","-.0E-010","-.0e10",
                                   "-.0E10","-.0e+10","-.0E+10","-.0e-10",
                                   "-.0E-10","-.0e2","-.0E2","-.0e+2","-.0E+2",
                                   "-.0e-2","-.0E-2","-.0e0","-.0E0","-.0e+0",
                                   "-.0E+0","-.0e-0","-.0E-0","-.0e","-.0E",
                                   "-.0e+","-.0E+","-.0e-","-.0E-","-.0",
                                   "-0.e00","-0.E00","-0.e+00","-0.E+00",
                                   "-0.e-00","-0.E-00","-0.e01","-0.E01",
                                   "-0.e+01","-0.E+01","-0.e-01","-0.E-01",
                                   "-0.e010","-0.E010","-0.e+010","-0.E+010",
                                   "-0.e-010","-0.E-010","-0.e10","-0.E10",
                                   "-0.e+10","-0.E+10","-0.e-10","-0.E-10",
                                   "-0.e2","-0.E2","-0.e+2","-0.E+2","-0.e-2",
                                   "-0.E-2","-0.e0","-0.E0","-0.e+0","-0.E+0",
                                   "-0.e-0","-0.E-0","-0.e","-0.E","-0.e+",
                                   "-0.E+","-0.e-","-0.E-","-0.","-00e00",
                                   "-00E00","-00e+00","-00E+00","-00e-00",
                                   "-00E-00","-00e01","-00E01","-00e+01",
                                   "-00E+01","-00e-01","-00E-01","-00e010",
                                   "-00E010","-00e+010","-00E+010","-00e-010",
                                   "-00E-010","-00e10","-00E10","-00e+10",
                                   "-00E+10","-00e-10","-00E-10","-00e2",
                                   "-00E2","-00e+2","-00E+2","-00e-2","-00E-2",
                                   "-00e0","-00E0","-00e+0","-00E+0","-00e-0",
                                   "-00E-0","-00e","-00E","-00e+","-00E+",
                                   "-00e-","-00E-","-00","-0e00","-0E00",
                                   "-0e+00","-0E+00","-0e-00","-0E-00","-0e01",
                                   "-0E01","-0e+01","-0E+01","-0e-01","-0E-01",
                                   "-0e010","-0E010","-0e+010","-0E+010",
                                   "-0e-010","-0E-010","-0e10","-0E10","-0e+10",
                                   "-0E+10","-0e-10","-0E-10","-0e2","-0E2",
                                   "-0e+2","-0E+2","-0e-2","-0E-2","-0e0",
                                   "-0E0","-0e+0","-0E+0","-0e-0","-0E-0","-0e",
                                   "-0E","-0e+","-0E+","-0e-","-0E-","-0",
                                   "+00.00e00","+00.00E00","+00.00e+00",
                                   "+00.00E+00","+00.00e-00","+00.00E-00",
                                   "+00.00e01","+00.00E01","+00.00e+01",
                                   "+00.00E+01","+00.00e-01","+00.00E-01",
                                   "+00.00e010","+00.00E010","+00.00e+010",
                                   "+00.00E+010","+00.00e-010","+00.00E-010",
                                   "+00.00e10","+00.00E10","+00.00e+10",
                                   "+00.00E+10","+00.00e-10","+00.00E-10",
                                   "+00.00e2","+00.00E2","+00.00e+2",
                                   "+00.00E+2","+00.00e-2","+00.00E-2",
                                   "+00.00e0","+00.00E0","+00.00e+0",
                                   "+00.00E+0","+00.00e-0","+00.00E-0",
                                   "+00.00e","+00.00E","+00.00e+","+00.00E+",
                                   "+00.00e-","+00.00E-","+00.00","+.0e00",
                                   "+.0E00","+.0e+00","+.0E+00","+.0e-00",
                                   "+.0E-00","+.0e01","+.0E01","+.0e+01",
                                   "+.0E+01","+.0e-01","+.0E-01","+.0e010",
                                   "+.0E010","+.0e+010","+.0E+010","+.0e-010",
                                   "+.0E-010","+.0e10","+.0E10","+.0e+10",
                                   "+.0E+10","+.0e-10","+.0E-10","+.0e2",
                                   "+.0E2","+.0e+2","+.0E+2","+.0e-2","+.0E-2",
                                   "+.0e0","+.0E0","+.0e+0","+.0E+0","+.0e-0",
                                   "+.0E-0","+.0e","+.0E","+.0e+","+.0E+",
                                   "+.0e-","+.0E-","+.0","+0.e00","+0.E00",
                                   "+0.e+00","+0.E+00","+0.e-00","+0.E-00",
                                   "+0.e01","+0.E01","+0.e+01","+0.E+01",
                                   "+0.e-01","+0.E-01","+0.e010","+0.E010",
                                   "+0.e+010","+0.E+010","+0.e-010","+0.E-010",
                                   "+0.e10","+0.E10","+0.e+10","+0.E+10",
                                   "+0.e-10","+0.E-10","+0.e2","+0.E2","+0.e+2",
                                   "+0.E+2","+0.e-2","+0.E-2","+0.e0","+0.E0",
                                   "+0.e+0","+0.E+0","+0.e-0","+0.E-0","+0.e",
                                   "+0.E","+0.e+","+0.E+","+0.e-","+0.E-","+0.",
                                   "+00e00","+00E00","+00e+00","+00E+00",
                                   "+00e-00","+00E-00","+00e01","+00E01",
                                   "+00e+01","+00E+01","+00e-01","+00E-01",
                                   "+00e010","+00E010","+00e+010","+00E+010",
                                   "+00e-010","+00E-010","+00e10","+00E10",
                                   "+00e+10","+00E+10","+00e-10","+00E-10",
                                   "+00e2","+00E2","+00e+2","+00E+2","+00e-2",
                                   "+00E-2","+00e0","+00E0","+00e+0","+00E+0",
                                   "+00e-0","+00E-0","+00e","+00E","+00e+",
                                   "+00E+","+00e-","+00E-","+00","+0e00",
                                   "+0E00","+0e+00","+0E+00","+0e-00","+0E-00",
                                   "+0e01","+0E01","+0e+01","+0E+01","+0e-01",
                                   "+0E-01","+0e010","+0E010","+0e+010",
                                   "+0E+010","+0e-010","+0E-010","+0e10",
                                   "+0E10","+0e+10","+0E+10","+0e-10","+0E-10",
                                   "+0e2","+0E2","+0e+2","+0E+2","+0e-2",
                                   "+0E-2","+0e0","+0E0","+0e+0","+0E+0",
                                   "+0e-0","+0E-0","+0e","+0E","+0e+","+0E+",
                                                            "+0e-","+0E-","+0"};

    // Initializing to zero the structure of parameters and parametersRef
    for (i=0; i<10; i++)
    {
        *pString[i]=NULL;
        *pStringRef[i]=NULL;
    }
    for (i=0; i<23; i++)
    {
        *pInteger[i]=0;
        *pIntegerRef[i]=0;
    }
    for (i=0; i<37; i++)
    {
        *pDouble[i]=.0;
        *pDoubleRef[i]=.0;
    }

    // Test starts
    time(&startTimer);
    fprintf(stdout,"\nTesting readInfoFileAndGetParameters function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    returnValue=readInfoFileAndGetParameters(pParameters);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pParameters=&parameters;
    PRINT_TEST_START(counter,expectedValue);
    fprintf(stdout,"pParameters=%p\n",(void*)pParameters);
    returnValue=readInfoFileAndGetParameters(pParameters);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    for (iRandom=0; iRandom<50000; iRandom++)
    {
        // Giving random values to the variables of the structure pParameters
        pParameters->name_length=rand()%120+1;
        pParameters->path_length=rand()%120+1;
        for (i=0; i<10; i++)
        {
            lengthName=rand()%120+1;
            if (lengthName>100)
            {
                *pString[i]=NULL;
            }
            else
            {
                lengthName=pParameters->path_length;
                if (i<5)
                {
                    lengthName=pParameters->name_length;
                }
                *pString[i]=(char*)calloc(lengthName,sizeof(char));
            }
            if (*pString[i]!=NULL)
            {
                for (j=0; j<(int)lengthName; j++)
                {
                    (*pString[i])[j]=rand()%90+33;
                }
                (*pString[i])[rand()%lengthName]='\0';
                if (i==0 && iRandom>10000)
                {
                    strncpy(*pString[0],"test.info",lengthName);
                }
            }
        }
        // Starting from 2 because path_length, name_length already initialized
        for (i=2; i<23; i++)
        {
            *pInteger[i]=rand()%121-20;
        }
        for (i=0; i<37; i++)
        {
            *pDouble[i]=(double)(rand()%60001-30000)/10000.;
        }

        // Copying the values of pParameters into parametersRef
        for (i=0; i<10; i++)
        {
            *pStringRef[i]=*pString[i];
            strncpy(pSaveString[i],"(NULL)",121);
            if (*pString[i]!=NULL)
            {
                strncpy(pSaveString[i],*pString[i],121);
            }
        }
        for (i=0; i<23; i++)
        {
            *pIntegerRef[i]=*pInteger[i];
        }
        for (i=0; i<37; i++)
        {
            *pDoubleRef[i]=*pDouble[i];
        }

        // pStringToPrint, pSpaceStringValue, pSpaceValueString, pValueToPrint
        expectedValue=1;
        booleanForEndData=0;
        booleanForName=0;
        booleanForPath=0;

        for (i=0; i<80; i++)
        {
            strncpy(pStringToPrint[i],"xxxxxxxxxxxxxxxxxxx",20);
            strncpy(pSpaceStringValue[i],"xxxxxxxxxxxxxxxxxxx",20);
            strncpy(pValueToPrint[i],"xxxxxxxxxxxxxxxxxxx",20);
            strncpy(pSpaceValueString[i],"xxxxxxxxxxxxxxxxxxx",20);
        }
        for (i=0; i<72; i++)
        {
            repetition[i]=0;
        }

        iMax=rand()%79;
        for (i=0; i<iMax; i++)
        {
            // Set up keyword thanks to the integer j
            j=rand()%71;
            stringToPrint=pKeyword[j];
            if (iRandom<12000 && rand()%3==0)
            {
                for (k=0; k<20; k++)
                {
                    keyword[k]=rand()%90+33;
                }
                keyword[rand()%20]='\0';
                stringToPrint=keyword;
                for (j=0; j<71; j++)
                {
                    if (!strcmp(stringToPrint,pKeyword[j]))
                    {
                        break;
                    }
                }
            }
            strncpy(pStringToPrint[i],stringToPrint,20);

            if (!booleanForEndData)
            {
                repetition[j]++;
            }
            if (j==1)
            {
                booleanForEndData=1;
            }

            // Set up space between keyword and value
            stringToPrint=pSpace[1];
            if (rand()%3==0)
            {
                stringToPrint=pSpace[rand()%35];
            }
            strncpy(pSpaceStringValue[i],stringToPrint,20);

            // Set up value associated with keyword
            if (iRandom<20000)
            {
                if (rand()%2)
                {
                    if (rand()%3==0)
                    {
                        k=rand()%2192;
                        stringToPrint=pDoubleToPrint[k];
                        strncpy(pValueToPrint[i],stringToPrint,20);
                    }
                    else
                    {
                        k=2192;
                        doubleToPrint=(double)(rand()%6001-3000)/1000.;
                        if (sprintf(pValueToPrint[i],"%lf",doubleToPrint)<0)
                        {
                            k=2193;
                            strncpy(pValueToPrint[i],pKeyword[0],20);
                        }
                    }
                }
                else
                {
                    if (rand()%3==0)
                    {
                        k=rand()%28;
                        stringToPrint=pIntegerToPrint[k];
                        strncpy(pValueToPrint[i],stringToPrint,20);
                        k=-k;
                    }
                    else
                    {
                        k=-28;
                        integerToPrint=rand()%201-100;
                        if (sprintf(pValueToPrint[i],"%d",integerToPrint)<0)
                        {
                            k=-29;
                            strncpy(pValueToPrint[i],pKeyword[0],20);
                        }
                    }
                }
            }
            else
            {
                if (j>10 && j<34)
                {
                    k=rand()%28;
                    stringToPrint=pIntegerToPrint[k];
                    k=-k;
                }
                else if (j>33 && j<71)
                {
                    k=rand()%2192;
                    stringToPrint=pDoubleToPrint[k];
                }
                else
                {
                    for (k=0; k<20; k++)
                    {
                        keyword[k]=rand()%90+33;
                    }
                    keyword[rand()%19+1]='\0';
                    stringToPrint=keyword;
                    k=-30;
                }
                strncpy(pValueToPrint[i],stringToPrint,20);
            }

            // Set up space between value and keyword
            stringToPrint=pSpace[5];
            if (rand()%3==0)
            {
                stringToPrint=pSpace[rand()%35];
            }
            strncpy(pSpaceValueString[i],stringToPrint,20);

            // Set up the expected value
            if (!booleanForEndData)
            {
                if (j==0 || j==71)
                {
                    expectedValue=0;
                }
                else if (j>1 && j<6)
                {
                    if (pSpaceValueString[i][0]=='\0')
                    {
                        expectedValue=0;
                    }
                    else if (strlen(pValueToPrint[i])<6 ||
                                                 (int)strlen(pValueToPrint[i])>=
                             DEF_MAX(booleanForName,pParameters->name_length) ||
                                                            *pString[j-1]!=NULL)
                    {
                        expectedValue=0;
                    }
                }
                else if (j>5 && j<11)
                {
                    if (pSpaceValueString[i][0]=='\0')
                    {
                        expectedValue=0;
                    }
                    else if ((int)strlen(pValueToPrint[i])>=
                             DEF_MAX(booleanForPath,pParameters->path_length) ||
                                                            *pString[j-1]==NULL)
                    {
                        expectedValue=0;
                    }
                }
                else if (j>10 && j<34)
                {
                    if (k>-3 || k<-28)
                    {
                        switch(k)
                        {
                            case 557:
                            case 600:
                            case 1030:
                            case 1073:
                            case 1503:
                            case 1546:
                            case 1718:
                            case 1761:
                            case 1933:
                            case 1976:
                            case 2148:
                            case 2191:
                                break;

                            default:
                                expectedValue=0;
                                break;
                        }
                    }
                    else if (j==11)
                    {
                        integerToPrint=atoi(pValueToPrint[i]);
                        if (integerToPrint<pParameters->name_length)
                        {
                            expectedValue=0;
                        }
                        else
                        {
                            booleanForName=integerToPrint;
                        }
                    }
                    else if (j==12)
                    {
                        integerToPrint=atoi(pValueToPrint[i]);
                        if (integerToPrint<pParameters->path_length)
                        {
                            expectedValue=0;
                        }
                        else
                        {
                            booleanForPath=integerToPrint;
                        }
                    }
                }
                else if (j>33 && j<71)
                {
                    if (k==-29 || k==2193)
                    {
                        expectedValue=0;
                    }
                    else if (!k && !strcmp(pValueToPrint[i],pDoubleToPrint[0]))
                    {
                        expectedValue=0;
                    }
                    else if (k>0 && k<128)
                    {
                        expectedValue=0;
                    }
                }
            }

            // Update the Ref structure according to the file to be written
            if (!booleanForEndData)
            {
                if (j>1 && j<11)
                {
                    strncpy(pSaveString[j-1],pValueToPrint[i],121);
                }
                else if (j>10 && j<34)
                {
                    integerToPrint=atoi(pValueToPrint[i]);
                    *pIntegerRef[j-11]=integerToPrint;
                }
                else if (j>33 && j<71)
                {
                    doubleToPrint=atof(pValueToPrint[i]);
                    *pDoubleRef[j-34]=doubleToPrint;
                }
            }
        }

        if (rand()%5==0)
        {
            booleanForEndData=1;
            strncpy(pStringToPrint[iMax],"end_data",20);
        }

        // More checks to set up the final expectedValue
        for (i=1; i<71; i++)
        {
            if (repetition[i]>1)
            {
                expectedValue=0;
            }
        }
        if (pParameters->name_info==NULL || pParameters->path_length<1)
        {
            expectedValue=0;
        }
        else if (strcmp(pParameters->name_info,"test.info") ||
                              pParameters->name_length<10 || !booleanForEndData)
        {
            expectedValue=0;
        }
        else if (pParameters->path_medit==NULL || pParameters->path_mmg3d==NULL
                                             || pParameters->path_mshdist==NULL
                                             || pParameters->path_elastic==NULL
                                              || pParameters->path_advect==NULL)
        {
            expectedValue=0;
        }
        else if (!strlen(pParameters->path_medit) ||
                                             !strlen(pParameters->path_mmg3d) ||
                                           !strlen(pParameters->path_mshdist) ||
                                           !strlen(pParameters->path_elastic) ||
                                              !strlen(pParameters->path_advect))
        {
            expectedValue=0;
        }

        PRINT_TEST_START(counter,expectedValue);

        // Writing the random test.info file
        infoFile=fopen("test.info","w+");
        if (infoFile==NULL)
        {
            fprintf(stdout,"\nWarning in testReadInfoFileAndGetParameters ");
            fprintf(stdout,"function: could not create the test.info file.\n");
            expectedValue=-1;
        }
        else
        {
            if (rand()%20==0)
            {
                stringToPrint=pSpace[rand()%35];
                fprintf(infoFile,"%s",stringToPrint);
            }
            for (i=0; i<iMax; i++)
            {
                fprintf(infoFile,"%s%s",pStringToPrint[i],pSpaceStringValue[i]);
                fprintf(infoFile,"%s%s",pValueToPrint[i],pSpaceValueString[i]);
            }
            if (!strcmp(pStringToPrint[iMax],"end_data"))
            {
                fprintf(infoFile,"%s",pStringToPrint[i]);
            }
            if (fclose(infoFile))
            {
                fprintf(stdout,"\nWarning in ");
                fprintf(stdout,"testReadInfoFileAndGetParameters function: ");
                fprintf(stderr," could not close the test.info file ");
                fprintf(stderr,"properly.\n");
                expectedValue=-1;
            }
            infoFile=NULL;
        }


        // Printing the value of the different variables
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%s\n",pStringName[i],*pString[i]);
        }
        for (i=0; i<23; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerName[i],*pInteger[i]);
        }
        for (i=0; i<37; i++)
        {
            fprintf(stdout,"%s=%lf\n",pDoubleName[i],*pDouble[i]);
        }

        // Testing the function
        returnValue=readInfoFileAndGetParameters(pParameters);

        // Check that the parameters have been corrected loaded
        if (returnValue)
        {
            for (i=0; i<10; i++)
            {
                if (*pString[i]==NULL)
                {
                    if (*pStringRef[i]!=NULL)
                    {
                        expectedValue=0;
                    }
                }
                else
                {
                    if (strcmp(*pString[i],pSaveString[i]))
                    {
                       expectedValue=0;
                    }
                }
            }
            for (i=0; i<23; i++)
            {
                if (*pInteger[i]!=*pIntegerRef[i])
                {
                    expectedValue=0;
                }
            }
            for (i=0; i<37; i++)
            {
                if (*pDouble[i]!=*pDoubleRef[i])
                {
                    expectedValue=0;
                }
            }
        }

        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,
                                                        expectedValue,readChar);

        // Free the memory allocated for strings
        for (i=0; i<10; i++)
        {
            free(*pString[i]);
            *pString[i]=NULL;
        }
    }
    system("rm test.info");

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

// Unit (random) tests on checkValuesOfAllParameters of loadParameters.c file
void testCheckValuesOfAllParameters(void)
{
    time_t startTimer=0, endTimer=0;
    int returnValue=0, expectedValue=0, counter=0, counterSuccess=0;
    int counterFail=0, readChar=0;

    size_t lengthName=0;
    char stringCommandLine[DEF_MAX(NAME_LENGTH,PATH_LENGTH)+10]="";
    int iRandom=0, i=0, j=0, boolean=0, booleanFile[5]={0};
    double delta=0.;
    Parameters *pParameters=NULL, parameters;

    char pStringToPrint[10][13]={"name_info","name_data","name_chem",
                                 "name_mesh","name_elas","path_medit",
                                 "path_mmg3d","path_mshdist","path_elastic",
                                                                 "path_advect"};
    char **pString[10]={&parameters.name_info,&parameters.name_data,
                        &parameters.name_chem,&parameters.name_mesh,
                        &parameters.name_elas,&parameters.path_medit,
                        &parameters.path_mmg3d,&parameters.path_mshdist,
                              &parameters.path_elastic,&parameters.path_advect};
    char pIntegerToPrint[23][13]={"name_length","path_length","opt_mode",
                                  "verbose","orb_rhf","ls_type","trick_matrix",
                                  "approx_mode","no_cfl","n_cpu","nu_electrons",
                                  "nu_spin","n_x","n_y","n_z","save_type",
                                  "hmode_lag","iter_max","save_mesh",
                                  "save_data","save_print","n_iter",
                                                                  "save_where"};
    int *pInteger[23]={&parameters.name_length,&parameters.path_length,
                       &parameters.opt_mode,&parameters.verbose,
                       &parameters.orb_rhf,&parameters.ls_type,
                       &parameters.trick_matrix,&parameters.approx_mode,
                       &parameters.no_cfl,&parameters.n_cpu,
                       &parameters.nu_electrons,&parameters.nu_spin,
                       &parameters.n_x,&parameters.n_y,&parameters.n_z,
                       &parameters.save_type,&parameters.hmode_lag,
                       &parameters.iter_max,&parameters.save_mesh,
                       &parameters.save_data,&parameters.save_print,
                                     &parameters.n_iter,&parameters.save_where};
    int pIntegerValue[23][17]={{NAME_LENGTH,NAME_LENGTH,NAME_LENGTH+1,
                                NAME_LENGTH+1,NAME_LENGTH+10,NAME_LENGTH+10,
                                NAME_LENGTH+50,NAME_LENGTH+50,7,8,17,57,0,
                                              NAME_LENGTH-1,6,NAME_LENGTH-8,-1},
                               {PATH_LENGTH,PATH_LENGTH,PATH_LENGTH+1,
                                PATH_LENGTH+1,PATH_LENGTH+10,PATH_LENGTH+10,
                                PATH_LENGTH+50,PATH_LENGTH+50,2,3,12,52,0,
                                              PATH_LENGTH-1,1,PATH_LENGTH-8,-6},
                               {-2,-1,0,OPT_MODE,1,2,3,4,-10,-3,5,10,
                                OPT_MODE+10,OPT_MODE-10,OPT_MODE+5,
                                                               OPT_MODE-5,1000},
                               {0,1,0,1,0,1,0,1,-10,-4,-3,-2,-1,2,3,4,10},
                               {0,1,0,1,0,1,0,1,-10,-4,-3,-2,-1,2,3,4,10},
                               {0,1,0,1,0,1,0,1,-10,-4,-3,-2,-1,2,3,4,10},
                               {0,1,0,1,0,1,0,1,-10,-4,-3,-2,-1,2,3,4,10},
                               {0,1,0,1,0,1,0,1,-10,-4,-3,-2,-1,2,3,4,10},
                               {0,1,0,1,0,1,0,1,-10,-4,-3,-2,-1,2,3,4,10},
                               {1,2,2,3,4,5,6,7,0,-1,-2,-3,-4,-5,-6,-7,-10},
                               {1,2,2,3,4,5,6,7,0,-1,-2,-3,-4,-5,-6,-7,-10},
                               {-1,0,1,-1,0,1,0,0,-20,-15,-10,-9,20,15,10,9,8},
                               {3,4,5,6,7,8,9,10,2,1,0,-1,-2,-3,-4,-5,-10},
                               {3,4,5,6,7,8,9,10,2,1,0,-1,-2,-3,-4,-5,-10},
                               {3,4,5,6,7,8,9,10,2,1,0,-1,-2,-3,-4,-5,-10},
                               {0,1,2,2,1,0,0,1,-1,-2,-3,-10,3,4,5,6,10},
                               {0,1,2,2,1,0,0,1,-1,-2,-3,-10,3,4,5,6,10},
                               {0,1,2,3,4,5,6,10,-1,-2,-3,-4,-5,-6,-7,-10,-100},
                               {0,1,2,3,4,5,6,10,-1,-2,-3,-4,-5,-6,-7,-10,-100},
                               {0,1,2,3,4,5,6,10,-1,-2,-3,-4,-5,-6,-7,-10,-100},
                               {0,1,2,3,4,5,6,10,-1,-2,-3,-4,-5,-6,-7,-10,-100},
                               {0,1,2,3,4,5,6,10,-1,-2,-3,-4,-5,-6,-7,-10,-100},
                               {1,2,3,4,5,6,7,SAVE_WHERE,0,-1,-2,-3,8,9,10,
                                                  SAVE_WHERE-10,SAVE_WHERE+10}};

    char pDoubleToPrint[37][12]={"x_min","y_min","z_min","x_max","y_max",
                                 "z_max","delta_x","delta_y","delta_z","ls_x",
                                 "ls_y","ls_z","ls_r","met_err","met_min",
                                 "hausd_iso","hmin_iso","hausd_met","hmin_met",
                                 "hausd_ls","hmin_ls","hausd_lag","hmin_lag",
                                 "delta_t","iter_told0p","iter_told1p",
                                 "iter_told2p","residual","hgrad_iso",
                                 "hgrad_met","hgrad_ls","hgrad_lag","met_max",
                                    "hmax_iso","hmax_met","hmax_ls","hmax_lag"};
    double *pDouble[37]={&parameters.x_min,&parameters.y_min,&parameters.z_min,
                         &parameters.x_max,&parameters.y_max,&parameters.z_max,
                         &parameters.delta_x,&parameters.delta_y,
                         &parameters.delta_z,&parameters.ls_x,&parameters.ls_y,
                         &parameters.ls_z,&parameters.ls_r,&parameters.met_err,
                         &parameters.met_min,&parameters.hausd_iso,
                         &parameters.hmin_iso,&parameters.hausd_met,
                         &parameters.hmin_met,&parameters.hausd_ls,
                         &parameters.hmin_ls,&parameters.hausd_lag,
                         &parameters.hmin_lag,&parameters.delta_t,
                         &parameters.iter_told0p,&parameters.iter_told1p,
                         &parameters.iter_told2p,&parameters.residual,
                         &parameters.hgrad_iso,&parameters.hgrad_met,
                         &parameters.hgrad_ls,&parameters.hgrad_lag,
                         &parameters.met_max,&parameters.hmax_iso,
                         &parameters.hmax_met,&parameters.hmax_ls,
                                                          &parameters.hmax_lag};
    double pDoubleValue[37][17]={{X_MIN,Y_MIN,Z_MIN,X_MIN+1.,Y_MIN+1.,Z_MIN+1.,
                                  -1.4321,-1.2345,-X_MIN,-Y_MIN,-Z_MIN,12.2,
                                            23.3456,6.5432,100.,1234.11,12.e10},
                                 {X_MIN,Y_MIN,Z_MIN,X_MIN+1.,Y_MIN+1.,Z_MIN+1.,
                                  -1.4321,-1.2345,-X_MIN,-Y_MIN,-Z_MIN,12.2,
                                            23.3456,6.5432,100.,1234.11,12.e10},
                                 {X_MIN,Y_MIN,Z_MIN,X_MIN+1.,Y_MIN+1.,Z_MIN+1.,
                                  -1.4321,-1.2345,-X_MIN,-Y_MIN,-Z_MIN,12.2,
                                            23.3456,6.5432,100.,1234.11,12.e10},
                                 {X_MAX,Y_MAX,Z_MAX,X_MAX,Y_MAX,Z_MAX,X_MAX,
                                  Y_MAX,-X_MAX,-Y_MAX,-Z_MAX,-12.2,-23.3456,
                                                -6.5432,-100.,-1234.11,-12.e10},
                                 {X_MAX,Y_MAX,Z_MAX,X_MAX,Y_MAX,Z_MAX,X_MAX,
                                  Y_MAX,-X_MAX,-Y_MAX,-Z_MAX,-12.2,-23.3456,
                                                -6.5432,-100.,-1234.11,-12.e10},
                                 {X_MAX,Y_MAX,Z_MAX,X_MAX,Y_MAX,Z_MAX,X_MAX,
                                  Y_MAX,-X_MAX,-Y_MAX,-Z_MAX,-12.2,-23.3456,
                                                -6.5432,-100.,-1234.11,-12.e10},
                                 {DELTA_X,DELTA_Y,DELTA_Z,
                                  (X_MAX-X_MIN)/(double)(N_X-1),
                                  (Y_MAX-Y_MIN)/(double)(N_Y-1),
                                  (Z_MAX-Z_MIN)/(double)(N_Z-1),DELTA_X+1.e-17,
                                  DELTA_Y-1.e-17,-DELTA_X,-DELTA_Y,-DELTA_Z,0.,
                                                     100.,1.e-14,-.023,-1.,0.2},
                                 {DELTA_X,DELTA_Y,DELTA_Z,
                                  (X_MAX-X_MIN)/(double)(N_X-1),
                                  (Y_MAX-Y_MIN)/(double)(N_Y-1),
                                  (Z_MAX-Z_MIN)/(double)(N_Z-1),DELTA_X+1.e-17,
                                  DELTA_Y-1.e-17,-DELTA_X,-DELTA_Y,-DELTA_Z,0.,
                                                     100.,1.e-14,-.023,-1.,0.2},

                                 {DELTA_X,DELTA_Y,DELTA_Z,
                                  (X_MAX-X_MIN)/(double)(N_X-1),
                                  (Y_MAX-Y_MIN)/(double)(N_Y-1),
                                  (Z_MAX-Z_MIN)/(double)(N_Z-1),DELTA_X+1.e-17,
                                  DELTA_Y-1.e-17,-DELTA_X,-DELTA_Y,-DELTA_Z,0.,
                                                     100.,1.e-14,-.023,-1.,0.2},
                                 {X_MIN+1.,X_MAX-1.,Y_MIN+1.,Y_MAX-1.,Z_MIN+1.,
                                  Z_MAX-1.,0.,LS_X,X_MIN-1.,X_MAX+1.,Y_MIN-1.,
                                  Y_MAX+1.,Z_MIN-1.,Z_MAX+1.,-100.,10.23654e2,
                                                                   -5565.32e-3},
                                 {X_MIN+1.,X_MAX-1.,Y_MIN+1.,Y_MAX-1.,Z_MIN+1.,
                                  Z_MAX-1.,0.,LS_Y,X_MIN-1.,X_MAX+1.,Y_MIN-1.,
                                  Y_MAX+1.,Z_MIN-1.,Z_MAX+1.,-100.,10.23654e2,
                                                                   -5565.32e-3},
                                 {X_MIN+1.,X_MAX-1.,Y_MIN+1.,Y_MAX-1.,Z_MIN+1.,
                                  Z_MAX-1.,0.,LS_Z,X_MIN-1.,X_MAX+1.,Y_MIN-1.,
                                  Y_MAX+1.,Z_MIN-1.,Z_MAX+1.,-100.,10.23654e2,
                                                                   -5565.32e-3},
                                 {LS_R,1.e-10,MET_ERR,MET_MIN,HMIN_ISO,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,-MET_MAX,-HAUSD_MET,
                                                            -HMIN_LS,-HMAX_LAG},
                                 {LS_R,1.e-10,MET_ERR,MET_MIN,HMIN_ISO,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,-MET_MAX,-HAUSD_MET,
                                                            -HMIN_LS,-HMAX_LAG},
                                 {1.e-10,MET_ERR,MET_MIN,HMIN_ISO,HMIN_LAG,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,MET_MAX,-HAUSD_MET,-HMIN_LS,
                                                                      HMAX_LAG},
                                 {LS_R,1.e-10,MET_ERR,MET_MIN,HMIN_ISO,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,-MET_MAX,-HAUSD_MET,
                                                            -HMIN_LS,-HMAX_LAG},
                                 {1.e-10,MET_ERR,MET_MIN,HMIN_ISO,HMIN_LAG,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,MET_MAX,-HAUSD_MET,-HMIN_LS,
                                                                      HMAX_LAG},
                                 {LS_R,1.e-10,MET_ERR,MET_MIN,HMIN_ISO,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,-MET_MAX,-HAUSD_MET,
                                                            -HMIN_LS,-HMAX_LAG},
                                 {1.e-10,MET_ERR,MET_MIN,HMIN_ISO,HMIN_LAG,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,MET_MAX,-HAUSD_MET,-HMIN_LS,
                                                                      HMAX_LAG},
                                 {LS_R,1.e-10,MET_ERR,MET_MIN,HMIN_ISO,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,-MET_MAX,-HAUSD_MET,
                                                            -HMIN_LS,-HMAX_LAG},
                                 {1.e-10,MET_ERR,MET_MIN,HMIN_ISO,HMIN_LAG,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,MET_MAX,-HAUSD_MET,-HMIN_LS,
                                                                      HMAX_LAG},
                                 {LS_R,1.e-10,MET_ERR,MET_MIN,HMIN_ISO,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,-MET_MAX,-HAUSD_MET,
                                                            -HMIN_LS,-HMAX_LAG},
                                 {1.e-10,MET_ERR,MET_MIN,HMIN_ISO,HMIN_LAG,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,MET_MAX,-HAUSD_MET,-HMIN_LS,
                                                                      HMAX_LAG},
                                 {DELTA_T,1.e-10,MET_ERR,MET_MIN,HMIN_ISO,
                                  HAUSD_LAG,HMIN_LS,HMIN_MET,0.,-LS_R,-1.e-10,
                                  -MET_ERR,-MET_MIN,-MET_MAX,-HAUSD_MET,
                                                            -HMIN_LS,-HMAX_LAG},
                                 {0.,ITER_TOLD0P,ITER_TOLD1P,ITER_TOLD2P,
                                  RESIDUAL,0.2,10.,1.e-5,-ITER_TOLD0P,
                                  -ITER_TOLD1P,-ITER_TOLD2P,-RESIDUAL,-0.2,-10.,
                                                      -1.e-5,-1.e-10,-2.235e23},
                                 {0.,ITER_TOLD0P,ITER_TOLD1P,ITER_TOLD2P,
                                  RESIDUAL,0.2,10.,1.e-5,-ITER_TOLD0P,
                                  -ITER_TOLD1P,-ITER_TOLD2P,-RESIDUAL,-0.2,-10.,
                                                      -1.e-5,-1.e-10,-2.235e23},
                                 {0.,ITER_TOLD0P,ITER_TOLD1P,ITER_TOLD2P,
                                  RESIDUAL,0.2,10.,1.e-5,-ITER_TOLD0P,
                                  -ITER_TOLD1P,-ITER_TOLD2P,-RESIDUAL,-0.2,-10.,
                                                      -1.e-5,-1.e-10,-2.235e23},
                                 {0.,ITER_TOLD0P,ITER_TOLD1P,ITER_TOLD2P,
                                  RESIDUAL,0.2,10.,1.e-5,-ITER_TOLD0P,
                                  -ITER_TOLD1P,-ITER_TOLD2P,-RESIDUAL,-0.2,-10.,
                                                      -1.e-5,-1.e-10,-2.235e23},
                                 {HGRAD_ISO,HGRAD_MET,HGRAD_LS,HGRAD_LAG,10.,
                                  1.2345e23,11.2345e-1,1.1,1.,0.,-1.e-3,-10.,
                                  -0.9999999,-HGRAD_ISO,-HGRAD_MET,-HGRAD_LS,
                                                                    -HGRAD_LAG},
                                 {HGRAD_ISO,HGRAD_MET,HGRAD_LS,HGRAD_LAG,10.,
                                  1.2345e23,11.2345e-1,1.1,1.,0.,-1.e-3,-10.,
                                  -0.9999999,-HGRAD_ISO,-HGRAD_MET,-HGRAD_LS,
                                                                    -HGRAD_LAG},
                                 {HGRAD_ISO,HGRAD_MET,HGRAD_LS,HGRAD_LAG,10.,
                                  1.2345e23,11.2345e-1,1.1,1.,0.,-1.e-3,-10.,
                                  -0.9999999,-HGRAD_ISO,-HGRAD_MET,-HGRAD_LS,
                                                                    -HGRAD_LAG},
                                 {HGRAD_ISO,HGRAD_MET,HGRAD_LS,HGRAD_LAG,10.,
                                  1.2345e23,11.2345e-1,1.1,1.,0.,-1.e-3,-10.,
                                  -0.9999999,-HGRAD_ISO,-HGRAD_MET,-HGRAD_LS,
                                                                    -HGRAD_LAG},
                                 {MET_MAX,HMAX_ISO+1.,HMAX_MET,HMAX_LS,HMAX_LAG,
                                  10.,1.2356,1.56e23,MET_MIN,HMIN_ISO,HMIN_MET,
                                    HMIN_LS,HMIN_LAG,0.,-1.,-MET_MAX,-HMIN_ISO},
                                 {MET_MAX,HMAX_ISO+1.,HMAX_MET,HMAX_LS,HMAX_LAG,
                                  10.,1.2356,1.56e23,MET_MIN,HMIN_ISO,HMIN_MET,
                                    HMIN_LS,HMIN_LAG,0.,-1.,-MET_MAX,-HMIN_ISO},
                                 {MET_MAX,HMAX_ISO+1.,HMAX_MET,HMAX_LS,HMAX_LAG,
                                  10.,1.2356,1.56e23,MET_MIN,HMIN_ISO,HMIN_MET,
                                    HMIN_LS,HMIN_LAG,0.,-1.,-MET_MAX,-HMIN_ISO},
                                 {MET_MAX,HMAX_ISO+1.,HMAX_MET,HMAX_LS,HMAX_LAG,
                                  10.,1.2356,1.56e23,MET_MIN,HMIN_ISO,HMIN_MET,
                                    HMIN_LS,HMIN_LAG,0.,-1.,-MET_MAX,-HMIN_ISO},
                                 {MET_MAX,HMAX_ISO+1.,HMAX_MET,HMAX_LS,HMAX_LAG,
                                  10.,1.2356,1.56e23,MET_MIN,HMIN_ISO,HMIN_MET,
                                   HMIN_LS,HMIN_LAG,0.,-1.,-MET_MAX,-HMIN_ISO}};

    // Initializing to zero the structure of parameters
    for (i=0; i<10; i++)
    {
        *pString[i]=NULL;
    }
    for (i=0; i<23; i++)
    {
        *pInteger[i]=0;
    }
    for (i=0; i<37; i++)
    {
        *pDouble[i]=.0;
    }

    // Test starts
    time(&startTimer);
    printf("\nTesting checkValuesOfAllParameters function.\n");

    expectedValue=0;
    PRINT_TEST_START(counter,expectedValue);
    printf("pParameters=%p\n",(void*)pParameters);
    returnValue=checkValuesOfAllParameters(pParameters);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);

    pParameters=&parameters;
    PRINT_TEST_START(counter,expectedValue);
    printf("pParameters=%p\n",(void*)pParameters);
    returnValue=checkValuesOfAllParameters(pParameters);
    PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                      readChar);
    for (iRandom=0; iRandom<50000; iRandom++)
    {
        expectedValue=1;
        strncpy(stringCommandLine,"touch ",DEF_MAX(NAME_LENGTH,PATH_LENGTH)+10);
        for (i=0; i<5; i++)
        {
            booleanFile[i]=0;
        }

        // Set the integer values of pParameters
        for (i=0; i<23; i++)
        {
            if (rand()%100)
            {
                *pInteger[i]=pIntegerValue[i][rand()%8];
            }
            else
            {
                *pInteger[i]=pIntegerValue[i][rand()%9+8];
                expectedValue=0;
            }
        }

        // Set the double values of pParameters
        for (i=0; i<37; i++)
        {
            if (rand()%100)
            {
                *pDouble[i]=pDoubleValue[i][rand()%8];
                if (rand()%100 && (i==6 || i==7 || i==8))
                {
                    *pDouble[i]=(*pDouble[i-3]-*pDouble[i-6])/(double)
                                                             (*pInteger[i+6]-1);
                }
            }
            else
            {
                *pDouble[i]=pDoubleValue[i][rand()%9+8];
        printf("\nCOUCOUC=%d\n",i);
                if (i!=9 && i!=10 && i!=11)
                {
                    expectedValue=0;
                }
            }
        }

        // Set the name_* string variables of the structure Parameters
        for (i=0; i<5; i++)
        {
            lengthName=rand()%(*pInteger[0]+50)+1;
            if ((int)lengthName>*pInteger[0]+25)
            {
                *pString[i]=NULL;
            }
            else
            {
                *pString[i]=(char*)calloc(lengthName,sizeof(char));
            }
            if (*pString[i]!=NULL)
            {
                for (j=0; j<(int)lengthName; j++)
                {
                   (*pString[i])[j]=rand()%26+97;
                }
                (*pString[i])[rand()%lengthName]='\0';
                if ((int)strlen(*pString[i])>=*pInteger[0])
                {
                    expectedValue=0;
                }
                else if ((int)strlen(*pString[i])<6 && (i==0 || i==1))
                {
                    expectedValue=0;
                }
                else if (i==4 && *pInteger[2]!=1 && *pInteger[2]!=2)
                {
                    expectedValue=0;
                }
                else if (rand()%100 && strlen(*pString[i])>0)
                {
                    strcat(stringCommandLine,*pString[i]);
                    system(stringCommandLine);
                    booleanFile[i]=1;
                    stringCommandLine[6]='\0';
                }
                else if (i==0 || i==2 || i==3)
                {
                    expectedValue=0;
                }
                else if (i==4 && (*pInteger[2]==1 || *pInteger[2]==1))
                {
                    expectedValue=0;
                }
            }
            else if (i==0 || i==2)
            {
                expectedValue=0;
            }
        }

        // Set the path_* string variables of the structure Parameters
        for (i=5; i<10; i++)
        {
            lengthName=rand()%(*pInteger[1]+50)+1;
            if ((int)lengthName>*pInteger[1]+25)
            {
                *pString[i]=NULL;
            }
            else
            {
                *pString[i]=(char*)calloc(lengthName,sizeof(char));
            }
            if (*pString[i]!=NULL)
            {
                for (j=0; j<(int)lengthName; j++)
                {
                   (*pString[i])[j]=rand()%26+97;
                }
                (*pString[i])[rand()%lengthName]='\0';
                if (!strlen(*pString[i]))
                {
                    expectedValue=0;
                }
            }
        }

        // Set the expectedValue to zero if additional conditions hold
        boolean=(*pInteger[6]!=TRICK_MATRIX || *pInteger[7]!=APPROX_MODE);
        if (boolean && *pInteger[2]>0)
        {
            expectedValue=0;
        }
        else if (*pInteger[8]!=NO_CFL && *pInteger[2]!=1 && *pInteger[2]!=2)
        {
            expectedValue=0;
        }
        else if (*pInteger[16]!=HMODE_LAG && *pInteger[2]!=1 && *pInteger[2]!=3)
        {
            expectedValue=0;
        }
        else if (*pInteger[21]!=N_ITER && *pInteger[2]!=1 && *pInteger[2]!=2 &&
                                                                *pInteger[2]!=4)
        {
            expectedValue=0;
        }

        boolean=(*pDouble[13]!=MET_ERR || *pDouble[14]!=MET_MIN);
        boolean=(boolean ||  *pDouble[32]!=MET_MAX);
        if (boolean && *pInteger[2]<=0)
        {
            expectedValue=0;
        }

        boolean=(*pDouble[16]!=HMIN_ISO || *pDouble[33]!=HMAX_ISO);
        boolean=(boolean || *pDouble[15]!=HAUSD_ISO || *pDouble[28]!=HGRAD_ISO);
        boolean=(boolean || *pDouble[18]!=HMIN_MET || *pDouble[33]!=HMAX_MET);
        boolean=(boolean || *pDouble[17]!=HAUSD_MET || *pDouble[29]!=HGRAD_MET);
        if (boolean && *pInteger[2]<=0)
        {
            expectedValue=0;
        }

        boolean=(*pDouble[20]!=HMIN_LS || *pDouble[35]!=HMAX_LS);
        boolean=(boolean || *pDouble[19]!=HAUSD_LS || *pDouble[30]!=HGRAD_LS);
        if (boolean && *pInteger[2]!=1 && *pInteger[2]!=2 && *pInteger[2]!=4)
        {
            expectedValue=0;
        }

        boolean=(*pDouble[22]!=HMIN_LAG || *pDouble[36]!=HMAX_LAG);
        boolean=(boolean || *pDouble[21]!=HAUSD_LAG || *pDouble[31]!=HGRAD_LAG);
        if (boolean && *pInteger[2]!=1 && *pInteger[2]!=3)
        {
            expectedValue=0;
        }
        else if (*pDouble[23]!=DELTA_T && *pInteger[2]!=1 && *pInteger[2]!=2)
        {
            expectedValue=0;
        }
        else if (*pDouble[27]!=RESIDUAL && *pInteger[2]!=1 && *pInteger[2]!=2 &&
                                                                *pInteger[2]!=4)
        {
            expectedValue=0;
        }

        for (i=6; i<9; i++)
        {
            delta=(*pDouble[i-3]-*pDouble[i-6])/(double)(*pInteger[i+6]-1);
            boolean=(DEF_ABS(*pDouble[i]-delta)<1.e-16);
            if (!boolean)
            {
                expectedValue=0;
            }
        }

        for (i=5; i<10; i++)
        {
            if (*pString[i]==NULL)
            {
                expectedValue=0;
            }
            else if ((int)strlen(*pString[i])>=*pInteger[1])
            {
                expectedValue=0;
            }
            else if (i==6 && *pInteger[2]<=0 && strcmp(*pString[i],PATH_MMG3D))
            {
                expectedValue=0;
            }
            else if (i==7 && *pInteger[2]!=1 && *pInteger[2]!=2 &&
                            *pInteger[2]!=4 && strcmp(*pString[i],PATH_MSHDIST))
            {
                expectedValue=0;
            }
            else if (i==8 && *pInteger[2]!=1 && *pInteger[2]!=2 &&
                                               strcmp(*pString[i],PATH_ELASTIC))
            {
                expectedValue=0;
            }
            else if (i==9 && *pInteger[2]!=1 && *pInteger[2]!=2 &&
                                                strcmp(*pString[i],PATH_ADVECT))
            {
                expectedValue=0;
            }
        }

        PRINT_TEST_START(counter,expectedValue);

        // Printing the values of the different variables
        for (i=0; i<10; i++)
        {
            fprintf(stdout,"%s=%s\n",pStringToPrint[i],*pString[i]);
            if (*pString[i]!=NULL)
            {
                fprintf(stdout,"(%ld characters)\n",strlen(*pString[i]));
            }
        }
        for (i=0; i<23; i++)
        {
            fprintf(stdout,"%s=%d\n",pIntegerToPrint[i],*pInteger[i]);
        }
        for (i=0; i<37; i++)
        {
            fprintf(stdout,"%s=%le\n",pDoubleToPrint[i],*pDouble[i]);
        }

        // Testing the function
        returnValue=checkValuesOfAllParameters(pParameters);

        // Remove the created files
        strncpy(stringCommandLine,"rm ",DEF_MAX(NAME_LENGTH,PATH_LENGTH)+10);
        for (i=0; i<5; i++)
        {
            if (booleanFile[i])
            {
                strcat(stringCommandLine,*pString[i]);
                printf("\n%s\n",stringCommandLine);
                system(stringCommandLine);
                stringCommandLine[3]='\0';
            }
        }

        // Free the memory allocated for string
        for (i=0; i<10; i++)
        {
            free(*pString[i]);
            *pString[i]=NULL;
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
static void testLoadParameters(void)
{
        time_t startTimer=0, endTimer=0;
        int returnValue=0, expectedValue=0, counter=0, counterSuccess=0, counterFail=0, readChar=0;

        size_t lengthName=0;
        int iRandom=0, i=0, iMax=0, j=0, k=0, integerToPrint=0, booleanForEndData=0;
        int booleanForName=0, booleanForPath=0, repetition[62]={0};
        double doubleToPrint=.0;
        FILE *infoFile=NULL;
        Parameters *pParameters=NULL, parameters, parametersRef;

        char pStringName[9][20]={"name_info","name_chem","name_mesh","name_elas",
                                        "path_medit","path_mmg3d","path_mshdist","path_elastic",
                                                                                     "path_advect"};
        char **pString[9]={&parameters.name_info,&parameters.name_chem,&parameters.name_mesh,
                           &parameters.name_elas,&parameters.path_medit,&parameters.path_mmg3d,
                           &parameters.path_mshdist,&parameters.path_elastic,
                                                                           &parameters.path_advect};
        char **pStringRef[9]={&parametersRef.name_info,&parametersRef.name_chem,
                              &parametersRef.name_mesh,&parametersRef.name_elas,
                              &parametersRef.path_medit,&parametersRef.path_mmg3d,
                              &parametersRef.path_mshdist,&parametersRef.path_elastic,
                                                                        &parametersRef.path_advect};
        char pIntegerName[20][20]={"name_length","path_length","ls_mode","verbose","orb_spin",
                                      "nu_electrons","n_cpu","n_x","n_y","n_z","n_iter","no_cfl",
                                      "ls_type","trick_matrix","iter_max","save_type","save_mesh",
                                                             "save_data","save_print","save_where"};
        int *pInteger[20]={&parameters.name_length,&parameters.path_length,&parameters.ls_mode,
                           &parameters.verbose,&parameters.orb_spin,&parameters.nu_electrons,
                           &parameters.n_cpu,&parameters.n_x,&parameters.n_y,&parameters.n_z,
                           &parameters.n_iter,&parameters.no_cfl,&parameters.ls_type,
                           &parameters.trick_matrix,&parameters.iter_max,&parameters.save_type,
                           &parameters.save_mesh,&parameters.save_data,&parameters.save_print,
                                                                            &parameters.save_where};
        int *pIntegerRef[20]={&parametersRef.name_length,&parametersRef.path_length,
                              &parametersRef.ls_mode,&parametersRef.verbose,
                              &parametersRef.orb_spin,&parametersRef.nu_electrons,
                              &parametersRef.n_cpu,&parametersRef.n_x,&parametersRef.n_y,
                              &parametersRef.n_z,&parametersRef.n_iter,&parametersRef.no_cfl,
                              &parametersRef.ls_type,&parametersRef.trick_matrix,
                              &parametersRef.iter_max,&parametersRef.save_type,
                              &parametersRef.save_mesh,&parametersRef.save_data,
                                               &parametersRef.save_print,&parametersRef.save_where};
        char pDoubleName[31][20]={"x_min","y_min","z_min","x_max","y_max","z_max","delta_x",
                                     "delta_y","delta_z","delta_t","ls_x","ls_y","ls_z","ls_r",
                                     "met_err","met_min","met_max","iter_told0p","iter_told1p",
                                     "hmin_iso","hmax_iso","hausd_iso","hgrad_iso","hmin_met",
                                     "hmax_met","hausd_met","hgrad_met","hmin_ls","hmax_ls",
                                                                             "hausd_ls","hgrad_ls"};
        double *pDouble[31]={&parameters.x_min,&parameters.y_min,&parameters.z_min,
                             &parameters.x_max,&parameters.y_max,&parameters.z_max,
                             &parameters.delta_x,&parameters.delta_y,&parameters.delta_z,
                             &parameters.delta_t,&parameters.ls_x,&parameters.ls_y,&parameters.ls_z,
                             &parameters.ls_r,&parameters.met_err,&parameters.met_min,
                             &parameters.met_max,&parameters.iter_told0p,&parameters.iter_told1p,
                             &parameters.hmin_iso,&parameters.hmax_iso,&parameters.hausd_iso,
                             &parameters.hgrad_iso,&parameters.hmin_met,&parameters.hmax_met,
                             &parameters.hausd_met,&parameters.hgrad_met,&parameters.hmin_ls,
                                     &parameters.hmax_ls,&parameters.hausd_ls,&parameters.hgrad_ls};
        double *pDoubleRef[31]={&parametersRef.x_min,&parametersRef.y_min,&parametersRef.z_min,
                                &parametersRef.x_max,&parametersRef.y_max,&parametersRef.z_max,
                                &parametersRef.delta_x,&parametersRef.delta_y,
                                &parametersRef.delta_z,&parametersRef.delta_t,&parametersRef.ls_x,
                                &parametersRef.ls_y,&parametersRef.ls_z,&parametersRef.ls_r,
                                &parametersRef.met_err,&parametersRef.met_min,
                                &parametersRef.met_max,&parametersRef.iter_told0p,
                                &parametersRef.iter_told1p,&parametersRef.hmin_iso,
                                &parametersRef.hmax_iso,&parametersRef.hausd_iso,
                                &parametersRef.hgrad_iso,&parametersRef.hmin_met,
                                &parametersRef.hmax_met,&parametersRef.hausd_met,
                                &parametersRef.hgrad_met,&parametersRef.hmin_ls,
                                &parametersRef.hmax_ls,&parametersRef.hausd_ls,
                                                                           &parametersRef.hgrad_ls};
        char pKeyword[61][20]={"not A Good Keyword","end_data","name_chem","name_mesh","name_elas",
                               "path_medit","path_mmg3d","path_mshdist","path_elastic",
                               "path_advect","name_length","path_length","ls_mode","verbose",
                               "orb_spin","nu_electrons","n_cpu","n_x","n_y","n_z","n_iter",
                               "no_cfl","ls_type","trick_matrix","iter_max","save_type","save_mesh",
                               "save_data","save_print","save_where","x_min","y_min","z_min",
                               "x_max","y_max","z_max","delta_x","delta_y","delta_z","delta_t",
                               "ls_x","ls_y","ls_z","ls_r","met_err","met_min","met_max",
                               "iter_told0p","iter_told1p","hmin_iso","hmax_iso","hausd_iso",
                               "hgrad_iso","hmin_met","hmax_met","hausd_met","hgrad_met","hmin_ls",
                                                                   "hmax_ls","hausd_ls","hgrad_ls"};
        char pSpace[35][20]={""," ","  ","   ","\n"," \n","\n "," \n ","  \n","  \n ","  \n  "," ",
                             " \n  ","\n  ","\n\n"," \n\n","\n\n "," \n\n ","\n \n "," \n \n"," ",
                             " \n \n ","  \n\n","\n\n  "," \n\n  ","  \n\n ","  \n\n  ","  \n \n",
                             "\n \n  "," \n  \n ","  \n  \n  "," \n  \n","\n  \n ","\n\n\n",
                                                                                      "\n\n\n\n\n"};
        char pIntegerToPrint[28][20]={"0.0","10e","9.82","-19873","-0151","-1","-01","0","0000","1",
                                      "001","987","-987383038","1029","0012837","0111","012745",
                                      "5456480","9","-7","089","-00001","-00","89","-238","2394002",
                                                                                   "200002","2939"};
        char pDoubleToPrint[2192][20]={"-e00","-E00","-e+00","-E+00","-e-00","-E-00","-e01","-E01",
                                       "-e+01","-E+01","-e-01","-E-01","-e010","-E010","-e+010",
                                       "-E+010","-e-010","-E-010","-e10","-E10","-e+10","-E+10",
                                       "-e-10","-E-10","-e2","-E2","-e+2","-E+2","-e-2","-E-2",
                                       "-e0","-E0","-e+0","-E+0","-e-0","-E-0","-e","-E","-e+",
                                       "-E+","-e-","-E-","-",

                                       "e00","E00","e+00","E+00","e-00","E-00","e01","E01","e+01",
                                       "E+01","e-01","E-01","e010","E010","e+010","E+010","e-010",
                                       "E-010","e10","E10","e+10","E+10","e-10","E-10","e2","E2",
                                       "e+2","E+2","e-2","E-2","e0","E0","e+0","E+0","e-0","E-0",
                                       "e","E","e+","E+","e-","E-",

                                       "+e00","+E00","+e+00","+E+00","+e-00","+E-00","+e01","+E01",
                                       "+e+01","+E+01","+e-01","+E-01","+e010","+E010","+e+010",
                                       "+E+010","+e-010","+E-010","+e10","+E10","+e+10","+E+10",
                                       "+e-10","+E-10","+e2","+E2","+e+2","+E+2","+e-2","+E-2",
                                       "+e0","+E0","+e+0","+E+0","+e-0","+E-0","+e","+E","+e+",
                                       "+E+","+e-","+E-","+",

                                       "-45.00e00","-45.00E00","-45.00e+00","-45.00E+00",
                                       "-45.00e-00","-45.00E-00","-45.00e01","-45.00E01",
                                       "-45.00e+01","-45.00E+01","-45.00e-01","-45.00E-01",
                                       "-45.00e010","-45.00E010","-45.00e+010","-45.00E+010",
                                       "-45.00e-010","-45.00E-010","-45.00e10","-45.00E10",
                                       "-45.00e+10","-45.00E+10","-45.00e-10","-45.00E-10",
                                       "-45.00e2","-45.00E2","-45.00e+2","-45.00E+2","-45.00e-2",
                                       "-45.00E-2","-45.00e0","-45.00E0","-45.00e+0","-45.00E+0",
                                       "-45.00e-0","-45.00E-0","-45.00e","-45.00E","-45.00e+",
                                       "-45.00E+","-45.00e-","-45.00E-","-45.00",

                                       "-045.0e00","-045.0E00","-045.0e+00","-045.0E+00",
                                       "-045.0e-00","-045.0E-00","-045.0e01","-045.0E01",
                                       "-045.0e+01","-045.0E+01","-045.0e-01","-045.0E-01",
                                       "-045.0e010","-045.0E010","-045.0e+010","-045.0E+010",
                                       "-045.0e-010","-045.0E-010","-045.0e10","-045.0E10",
                                       "-045.0e+10","-045.0E+10","-045.0e-10", "-045.0E-10",
                                       "-045.0e2","-045.0E2","-045.0e+2","-045.0E+2","-045.0e-2",
                                       "-045.0E-2","-045.0e0","-045.0E0","-045.0e+0","-045.0E+0",
                                       "-045.0e-0","-045.0E-0","-045.0e","-045.0E","-045.0e+",
                                       "-045.0E+","-045.0e-","-045.0E-","-045.0",

                                       "-45.e00","-45.E00","-45.e+00","-45.E+00","-45.e-00",
                                       "-45.E-00","-45.e01","-45.E01","-45.e+01","-45.E+01",
                                       "-45.e-01","-45.E-01","-45.e010","-45.E010","-45.e+010",
                                       "-45.E+010","-45.e-010","-45.E-010","-45.e10","-45.E10",
                                       "-45.e+10","-45.E+10","-45.e-10","-45.E-10","-45.e2",
                                       "-45.E2","-45.e+2","-45.E+2","-45.e-2","-45.E-2","-45.e0",
                                       "-45.E0","-45.e+0","-45.E+0","-45.e-0","-45.E-0","-45.e",
                                       "-45.E","-45.e+","-45.E+","-45.e-","-45.E-","-45.",

                                       "-0045.e00","-0045.E00","-0045.e+00","-0045.E+00",
                                       "-0045.e-00","-0045.E-00","-0045.e01","-0045.E01",
                                       "-0045.e+01","-0045.E+01","-0045.e-01","-0045.E-01",
                                       "-0045.e010","-0045.E010","-0045.e+010","-0045.E+010",
                                       "-0045.e-010","-0045.E-010","-0045.e10","-0045.E10",
                                       "-0045.e+10","-0045.E+10","-0045.e-10","-0045.E-10",
                                       "-0045.e2","-0045.E2","-0045.e+2","-0045.E+2","-0045.e-2",
                                       "-0045.E-2","-0045.e0","-0045.E0","-0045.e+0","-0045.E+0",
                                       "-0045.e-0","-0045.E-0","-0045.e","-0045.E","-0045.e+",
                                       "-0045.E+","-0045.e-","-0045.E-","-0045.",

                                       "-45.54e00","-45.54E00","-45.54e+00","-45.54E+00",
                                       "-45.54e-00","-45.54E-00","-45.54e01","-45.54E01",
                                       "-45.54e+01","-45.54E+01","-45.54e-01","-45.54E-01",
                                       "-45.54e010","-45.54E010","-45.54e+010","-45.54E+010",
                                       "-45.54e-010","-45.54E-010","-45.54e10","-45.54E10",
                                       "-45.54e+10","-45.54E+10","-45.54e-10","-45.54E-10",
                                       "-45.54e2","-45.54E2","-45.54e+2","-45.54E+2","-45.54e-2",
                                       "-45.54E-2","-45.54e0","-45.54E0","-45.54e+0","-45.54E+0",
                                       "-45.54e-0","-45.54E-0","-45.54e","-45.54E","-45.54e+",
                                       "-45.54E+","-45.54e-","-45.54E-","-45.54",

                                       "-045.054e00","-045.054E00","-045.054e+00","-045.054E+00",
                                       "-045.054e-00","-045.054E-00","-045.054e01","-045.054E01",
                                       "-045.054e+01","-045.054E+01","-045.054e-01","-045.054E-01",
                                       "-045.054e010","-045.054E010","-045.054e+010",
                                       "-045.054E+010","-045.054e-010","-045.054E-010",
                                       "-045.054e10","-045.054E10","-045.054e+10","-045.054E+10",
                                       "-045.054e-10","-045.054E-10","-045.054e2","-045.054E2",
                                       "-045.054e+2","-045.054E+2","-045.054e-2","-045.054E-2",
                                       "-045.054e0","-045.054E0","-045.054e+0","-045.054E+0",
                                       "-045.054e-0","-045.054E-0","-045.054e","-045.054E",
                                       "-045.054e+","-045.054E+","-045.054e-","-045.054E-",
                                       "-045.054",

                                       "-000.054e00","-000.054E00","-000.054e+00","-000.054E+00",
                                       "-000.054e-00","-000.054E-00","-000.054e01","-000.054E01",
                                       "-000.054e+01","-000.054E+01","-000.054e-01","-000.054E-01",
                                       "-000.054e010","-000.054E010","-000.054e+010",
                                       "-000.054E+010","-000.054e-010","-000.054E-010",
                                       "-000.054e10","-000.054E10","-000.054e+10","-000.054E+10",
                                       "-000.054e-10","-000.054E-10","-000.054e2","-000.054E2",
                                       "-000.054e+2","-000.054E+2","-000.054e-2","-000.054E-2",
                                       "-000.054e0","-000.054E0","-000.054e+0","-000.054E+0",
                                       "-000.054e-0","-000.054E-0","-000.054e","-000.054E",
                                       "-000.054e+","-000.054E+","-000.054e-","-000.054E-",
                                       "-000.054",

                                       "-0.054e00","-0.054E00","-0.054e+00","-0.054E+00",
                                       "-0.054e-00","-0.054E-00","-0.054e01","-0.054E01",
                                       "-0.054e+01","-0.054E+01","-0.054e-01","-0.054E-01",
                                       "-0.054e010","-0.054E010","-0.054e+010","-0.054E+010",
                                       "-0.054e-010","-0.054E-010","-0.054e10","-0.054E10",
                                       "-0.054e+10","-0.054E+10","-0.054e-10","-0.054E-10",
                                       "-0.054e2","-0.054E2","-0.054e+2","-0.054E+2","-0.054e-2",
                                       "-0.054E-2","-0.054e0","-0.054E0","-0.054e+0","-0.054E+0",
                                       "-0.054e-0","-0.054E-0","-0.054e","-0.054E","-0.054e+",
                                       "-0.054E+","-0.054e-","-0.054E-","-0.054",

                                       "-.054e00","-.054E00","-.054e+00","-.054E+00","-.054e-00",
                                       "-.054E-00","-.054e01","-.054E01","-.054e+01","-.054E+01",
                                       "-.054e-01","-.054E-01","-.054e010","-.054E010","-.054e+010",
                                       "-.054E+010","-.054e-010","-.054E-010","-.054e10","-.054E10",
                                       "-.054e+10","-.054E+10","-.054e-10","-.054E-10","-.054e2",
                                       "-.054E2","-.054e+2","-.054E+2","-.054e-2","-.054E-2",
                                       "-.054e0","-.054E0","-.054e+0","-.054E+0","-.054e-0",
                                       "-.054E-0","-.054e","-.054E","-.054e+","-.054E+","-.054e-",
                                       "-.054E-","-.054",

                                       "-45e00","-45E00","-45e+00","-45E+00","-45e-00","-45E-00",
                                       "-45e01","-45E01","-45e+01","-45E+01","-45e-01","-45E-01",
                                       "-45e010","-45E010","-45e+010","-45E+010","-45e-010",
                                       "-45E-010","-45e10","-45E10","-45e+10","-45E+10","-45e-10",
                                       "-45E-10","-45e2","-45E2","-45e+2","-45E+2","-45e-2",
                                       "-45E-2","-45e0","-45E0","-45e+0","-45E+0","-45e-0","-45E-0",
                                       "-45e","-45E","-45e+","-45E+","-45e-","-45E-","-45",

                                       "-0045e00","-0045E00","-0045e+00","-0045E+00","-0045e-00",
                                       "-0045E-00","-0045e01","-0045E01","-0045e+01","-0045E+01",
                                       "-0045e-01","-0045E-01","-0045e010","-0045E010","-0045e+010",
                                       "-0045E+010","-0045e-010","-0045E-010","-0045e10","-0045E10",
                                       "-0045e+10","-0045E+10","-0045e-10","-0045E-10","-0045e2",
                                       "-0045E2","-0045e+2","-0045E+2","-0045e-2","-0045E-2",
                                       "-0045e0","-0045E0","-0045e+0","-0045E+0","-0045e-0",
                                       "-0045E-0","-0045e","-0045E","-0045e+","-0045E+","-0045e-",
                                       "-0045E-","-0045",

                                       "45.00e00","45.00E00","45.00e+00","45.00E+00","45.00e-00",
                                       "45.00E-00","45.00e01","45.00E01","45.00e+01","45.00E+01",
                                       "45.00e-01","45.00E-01","45.00e010","45.00E010","45.00e+010",
                                       "45.00E+010","45.00e-010","45.00E-010","45.00e10","45.00E10",
                                       "45.00e+10","45.00E+10","45.00e-10","45.00E-10","45.00e2",
                                       "45.00E2","45.00e+2","45.00E+2","45.00e-2","45.00E-2",
                                       "45.00e0","45.00E0","45.00e+0","45.00E+0","45.00e-0",
                                       "45.00E-0","45.00e","45.00E","45.00e+","45.00E+","45.00e-",
                                       "45.00E-","45.00",

                                       "045.0e00","045.0E00","045.0e+00","045.0E+00","045.0e-00",
                                       "045.0E-00","045.0e01","045.0E01","045.0e+01","045.0E+01",
                                       "045.0e-01","045.0E-01","045.0e010","045.0E010","045.0e+010",
                                       "045.0E+010","045.0e-010","045.0E-010","045.0e10","045.0E10",
                                       "045.0e+10","045.0E+10","045.0e-10","045.0E-10","045.0e2",
                                       "045.0E2","045.0e+2","045.0E+2","045.0e-2","045.0E-2",
                                       "045.0e0","045.0E0","045.0e+0","045.0E+0","045.0e-0",
                                       "045.0E-0","045.0e","045.0E","045.0e+","045.0E+","045.0e-",
                                       "045.0E-","045.0","45.e00","45.E00","45.e+00","45.E+00",

                                       "45.e-00","45.E-00","45.e01","45.E01","45.e+01","45.E+01",
                                       "45.e-01","45.E-01","45.e010","45.E010","45.e+010",
                                       "45.E+010","45.e-010","45.E-010","45.e10","45.E10","45.e+10",
                                       "45.E+10","45.e-10","45.E-10","45.e2","45.E2","45.e+2",
                                       "45.E+2","45.e-2","45.E-2","45.e0","45.E0","45.e+0","45.E+0",
                                       "45.e-0","45.E-0","45.e","45.E","45.e+","45.E+","45.e-",
                                       "45.E-","45.",

                                       "0045.e00","0045.E00","0045.e+00","0045.E+00","0045.e-00",
                                       "0045.E-00","0045.e01","0045.E01","0045.e+01","0045.E+01",
                                       "0045.e-01","0045.E-01","0045.e010","0045.E010","0045.e+010",
                                       "0045.E+010","0045.e-010","0045.E-010","0045.e10","0045.E10",
                                       "0045.e+10","0045.E+10","0045.e-10","0045.E-10","0045.e2",
                                       "0045.E2","0045.e+2","0045.E+2","0045.e-2","0045.E-2",
                                       "0045.e0","0045.E0","0045.e+0","0045.E+0","0045.e-0",
                                       "0045.E-0","0045.e","0045.E","0045.e+","0045.E+","0045.e-",
                                       "0045.E-","0045.",

                                       "45.54e00","45.54E00","45.54e+00","45.54E+00","45.54e-00",
                                       "45.54E-00","45.54e01","45.54E01","45.54e+01","45.54E+01",
                                       "45.54e-01","45.54E-01","45.54e010","45.54E010","45.54e+010",
                                       "45.54E+010","45.54e-010","45.54E-010","45.54e10","45.54E10",
                                       "45.54e+10","45.54E+10","45.54e-10","45.54E-10","45.54e2",
                                       "45.54E2","45.54e+2","45.54E+2","45.54e-2","45.54E-2",
                                       "45.54e0","45.54E0","45.54e+0","45.54E+0","45.54e-0",
                                       "45.54E-0","45.54e","45.54E","45.54e+","45.54E+","45.54e-",
                                       "45.54E-","45.54",

                                       "045.054e00","045.054E00","045.054e+00","045.054E+00",
                                       "045.054e-00","045.054E-00","045.054e01","045.054E01",
                                       "045.054e+01","045.054E+01","045.054e-01","045.054E-01",
                                       "045.054e010","045.054E010","045.054e+010","045.054E+010",
                                       "045.054e-010","045.054E-010","045.054e10","045.054E10",
                                       "045.054e+10","045.054E+10","045.054e-10","045.054E-10",
                                       "045.054E2","045.054e+2","045.054E+2","045.054e-2",
                                       "045.054e2","045.054E-2","045.054e0","045.054E0",
                                       "045.054e+0","045.054E+0","045.054e-0","045.054E-0",
                                       "045.054e","045.054E","045.054e+","045.054E+","045.054e-",
                                       "045.054E-","045.054",

                                       "000.054e00","000.054E00","000.054e+00","000.054E+00",
                                       "000.054e-00","000.054E-00","000.054e01","000.054E01",
                                       "000.054e+01","000.054E+01","000.054e-01","000.054E-01",
                                       "000.054e010","000.054E010","000.054e+010","000.054E+010",
                                       "000.054e-010","000.054E-010","000.054e10","000.054E10",
                                       "000.054e+10","000.054E+10","000.054e-10","000.054E-10",
                                       "000.054e2","000.054E2","000.054e+2","000.054E+2",
                                       "000.054e-2","000.054E-2","000.054e0","000.054E0",
                                       "000.054e+0","000.054E+0","000.054e-0","000.054E-0",
                                       "000.054e","000.054E","000.054e+","000.054E+","000.054e-",
                                       "000.054E-","000.054",

                                       "0.054e00","0.054E00","0.054e+00","0.054E+00","0.054e-00",
                                       "0.054E-00","0.054e01","0.054E01","0.054e+01","0.054E+01",
                                       "0.054e-01","0.054E-01","0.054e010","0.054E010","0.054e+010",
                                       "0.054E+010","0.054e-010","0.054E-010","0.054e10","0.054E10",
                                       "0.054e+10","0.054E+10","0.054e-10","0.054E-10","0.054e2",
                                       "0.054E2","0.054e+2","0.054E+2","0.054e-2","0.054E-2",
                                       "0.054e0","0.054E0","0.054e+0","0.054E+0","0.054e-0",
                                       "0.054E-0","0.054e","0.054E","0.054e+","0.054E+","0.054e-",
                                       "0.054E-","0.054",".054e00",".054E00",".054e+00",".054E+00",

                                       ".054e-00",".054E-00",".054e01",".054E01",".054e+01",
                                       ".054E+01",".054e-01",".054E-01",".054e010",".054E010",
                                       ".054e+010",".054E+010",".054e-010",".054E-010",".054e10",
                                       ".054E10",".054e+10",".054E+10",".054e-10",".054E-10",
                                       ".054e2",".054E2",".054e+2",".054E+2",".054e-2",".054E-2",
                                       ".054e0",".054E0",".054e+0",".054E+0",".054e-0",".054E-0",
                                       ".054e",".054E",".054e+",".054E+",".054e-",".054E-",".054",

                                       "45e00","45E00","45e+00","45E+00","45e-00","45E-00","45e01",
                                       "45E01","45e+01","45E+01","45e-01","45E-01","45e010",
                                       "45E010","45e+010","45E+010","45e-010","45E-010","45e10",
                                       "45E10","45e+10","45E+10","45e-10","45E-10","45e2","45E2",
                                       "45e+2","45E+2","45e-2","45E-2","45e0","45E0","45e+0",
                                       "45E+0","45e-0","45E-0","45e","45E","45e+","45E+","45e-",
                                       "45E-","45",

                                       "0045e00","0045E00","0045e+00","0045E+00","0045e-00",
                                       "0045E-00","0045e01","0045E01","0045e+01","0045E+01",
                                       "0045e-01","0045E-01","0045e010","0045E010","0045e+010",
                                       "0045E+010","0045e-010","0045E-010","0045e10","0045E10",
                                       "0045e+10","0045E+10","0045e-10","0045E-10","0045e2",
                                       "0045E2","0045e+2","0045E+2","0045e-2","0045E-2","0045e0",
                                       "0045E0","0045e+0","0045E+0","0045e-0","0045E-0","0045e",
                                       "0045E","0045e+","0045E+","0045e-","0045E-","0045",

                                       "+45.00e00","+45.00E00","+45.00e+00","+45.00E+00",
                                       "+45.00e-00","+45.00E-00","+45.00e01","+45.00E01",
                                       "+45.00e+01","+45.00E+01","+45.00e-01","+45.00E-01",
                                       "+45.00e010","+45.00E010","+45.00e+010","+45.00E+010",
                                       "+45.00e-010","+45.00E-010","+45.00e10","+45.00E10",
                                       "+45.00e+10","+45.00E+10","+45.00e-10","+45.00E-10",
                                       "+45.00e2","+45.00E2","+45.00e+2","+45.00E+2","+45.00e-2",
                                       "+45.00E-2","+45.00e0","+45.00E0","+45.00e+0","+45.00E+0",
                                       "+45.00e-0","+45.00E-0","+45.00e","+45.00E","+45.00e+",
                                       "+45.00E+","+45.00e-","+45.00E-","+45.00",

                                       "+045.0e00","+045.0E00","+045.0e+00","+045.0E+00",
                                       "+045.0e-00","+045.0E-00","+045.0e01","+045.0E01",
                                       "+045.0e+01","+045.0E+01","+045.0e-01","+045.0E-01",
                                       "+045.0e010","+045.0E010","+045.0e+010","+045.0E+010",
                                       "+045.0e-010","+045.0E-010","+045.0e10","+045.0E10",
                                       "+045.0e+10","+045.0E+10","+045.0e-10","+045.0E-10",
                                       "+045.0e2","+045.0E2","+045.0e+2","+045.0E+2","+045.0e-2",
                                       "+045.0E-2","+045.0e0","+045.0E0","+045.0e+0","+045.0E+0",
                                       "+045.0e-0","+045.0E-0","+045.0e","+045.0E","+045.0e+",
                                       "+045.0E+","+045.0e-","+045.0E-","+045.0",

                                       "+45.e00","+45.E00","+45.e+00","+45.E+00","+45.e-00",
                                       "+45.E-00","+45.e01","+45.E01","+45.e+01","+45.E+01",
                                       "+45.e-01","+45.E-01","+45.e010","+45.E010","+45.e+010",
                                       "+45.E+010","+45.e-010","+45.E-010","+45.e10","+45.E10",
                                       "+45.e+10","+45.E+10","+45.e-10","+45.E-10","+45.e2",
                                       "+45.E2","+45.e+2","+45.E+2","+45.e-2","+45.E-2","+45.e0",
                                       "+45.E0","+45.e+0","+45.E+0","+45.e-0","+45.E-0","+45.e",
                                       "+45.E","+45.e+","+45.E+","+45.e-","+45.E-","+45.",

                                       "+0045.e00","+0045.E00","+0045.e+00","+0045.E+00",
                                       "+0045.e-00","+0045.E-00","+0045.e01","+0045.E01",
                                       "+0045.e+01","+0045.E+01","+0045.e-01","+0045.E-01",
                                       "+0045.e010","+0045.E010","+0045.e+010","+0045.E+010",
                                       "+0045.e-010","+0045.E-010","+0045.e10","+0045.E10",
                                       "+0045.e+10","+0045.E+10","+0045.e-10","+0045.E-10",
                                       "+0045.e2","+0045.E2","+0045.e+2","+0045.E+2","+0045.e-2",
                                       "+0045.E-2","+0045.e0","+0045.E0","+0045.e+0","+0045.E+0",
                                       "+0045.e-0","+0045.E-0","+0045.e","+0045.E","+0045.e+",
                                       "+0045.E+","+0045.e-","+0045.E-","+0045.",

                                       "+45.54e00","+45.54E00","+45.54e+00","+45.54E+00",
                                       "+45.54e-00","+45.54E-00","+45.54e01","+45.54E01",
                                       "+45.54e+01","+45.54E+01","+45.54e-01","+45.54E-01",
                                       "+45.54e010","+45.54E010","+45.54e+010","+45.54E+010",
                                       "+45.54e-010","+45.54E-010","+45.54e10","+45.54E10",
                                       "+45.54e+10","+45.54E+10","+45.54e-10","+45.54E-10",
                                       "+45.54e2","+45.54E2","+45.54e+2","+45.54E+2","+45.54e-2",
                                       "+45.54E-2","+45.54e0","+45.54E0","+45.54e+0","+45.54E+0",
                                       "+45.54e-0","+45.54E-0","+45.54e","+45.54E","+45.54e+",
                                       "+45.54E+","+45.54e-","+45.54E-","+45.54",

                                       "+045.054e00","+045.054E00","+045.054e+00","+045.054E+00",
                                       "+045.054e-00","+045.054E-00","+045.054e01","+045.054E01",
                                       "+045.054e+01","+045.054E+01","+045.054e-01","+045.054E-01",
                                       "+045.054e010","+045.054E010","+045.054e+010",
                                       "+045.054E+010","+045.054e-010","+045.054E-010",
                                       "+045.054e10","+045.054E10","+045.054e+10","+045.054E+10",
                                       "+045.054e-10","+045.054E-10","+045.054e2","+045.054E2",
                                       "+045.054e+2","+045.054E+2","+045.054e-2","+045.054E-2",
                                       "+045.054e0","+045.054E0","+045.054e+0","+045.054E+0",
                                       "+045.054e-0","+045.054E-0","+045.054e","+045.054E",
                                       "+045.054e+","+045.054E+","+045.054e-","+045.054E-",
                                       "+045.054",

                                       "+000.054e00","+000.054E00","+000.054e+00","+000.054E+00",
                                       "+000.054e-00","+000.054E-00","+000.054e01","+000.054E01",
                                       "+000.054e+01","+000.054E+01","+000.054e-01","+000.054E-01",
                                       "+000.054e010","+000.054E010","+000.054e+010",
                                       "+000.054E+010","+000.054e-010","+000.054E-010",
                                       "+000.054e10","+000.054E10","+000.054e+10","+000.054E+10",
                                       "+000.054e-10","+000.054E-10","+000.054e2","+000.054E2",
                                       "+000.054e+2","+000.054E+2","+000.054e-2","+000.054E-2",
                                       "+000.054e0","+000.054E0","+000.054e+0","+000.054E+0",
                                       "+000.054e-0","+000.054E-0","+000.054e","+000.054E",
                                       "+000.054e+","+000.054E+","+000.054e-","+000.054E-",
                                       "+000.054",

                                       "+0.054e00","+0.054E00","+0.054e+00","+0.054E+00",
                                       "+0.054e-00","+0.054E-00","+0.054e01","+0.054E01",
                                       "+0.054e+01","+0.054E+01","+0.054e-01","+0.054E-01",
                                       "+0.054e010","+0.054E010","+0.054e+010","+0.054E+010",
                                       "+0.054e-010","+0.054E-010","+0.054e10","+0.054E10",
                                       "+0.054e+10","+0.054E+10","+0.054e-10","+0.054E-10",
                                       "+0.054e2","+0.054E2","+0.054e+2","+0.054E+2","+0.054e-2",
                                       "+0.054E-2","+0.054e0","+0.054E0","+0.054e+0","+0.054E+0",
                                       "+0.054e-0","+0.054E-0","+0.054e","+0.054E","+0.054e+",
                                       "+0.054E+","+0.054e-","+0.054E-","+0.054",

                                       "+.054e00","+.054E00","+.054e+00","+.054E+00","+.054e-00",
                                       "+.054E-00","+.054e01","+.054E01","+.054e+01","+.054E+01",
                                       "+.054e-01","+.054E-01","+.054e010","+.054E010","+.054e+010",
                                       "+.054E+010","+.054e-010","+.054E-010","+.054e10","+.054E10",
                                       "+.054e+10","+.054E+10","+.054e-10","+.054E-10","+.054e2",
                                       "+.054E2","+.054e+2","+.054E+2","+.054e-2","+.054E-2",
                                       "+.054e0","+.054E0","+.054e+0","+.054E+0","+.054e-0",
                                       "+.054E-0","+.054e","+.054E","+.054e+","+.054E+","+.054e-",
                                       "+.054E-","+.054",

                                       "+45e00","+45E00","+45e+00","+45E+00","+45e-00","+45E-00",
                                       "+45e01","+45E01","+45e+01","+45E+01","+45e-01","+45E-01",
                                       "+45e010","+45E010","+45e+010","+45E+010","+45e-010",
                                       "+45E-010","+45e10","+45E10","+45e+10","+45E+10","+45e-10",
                                       "+45E-10","+45e2","+45E2","+45e+2","+45E+2","+45e-2",
                                       "+45E-2","+45e0","+45E0","+45e+0","+45E+0","+45e-0","+45E-0",
                                       "+45e","+45E","+45e+","+45E+","+45e-","+45E-","+45",

                                       "+0045e00","+0045E00","+0045e+00","+0045E+00","+0045e-00",
                                       "+0045E-00","+0045e01","+0045E01","+0045e+01","+0045E+01",
                                       "+0045e-01","+0045E-01","+0045e010","+0045E010","+0045e+010",
                                       "+0045E+010","+0045e-010","+0045E-010","+0045e10","+0045E10",
                                       "+0045e+10","+0045E+10","+0045e-10","+0045E-10","+0045e2",
                                       "+0045E2","+0045e+2","+0045E+2","+0045e-2","+0045E-2",
                                       "+0045e0","+0045E0","+0045e+0","+0045E+0","+0045e-0",
                                       "+0045E-0","+0045e","+0045E","+0045e+","+0045E+","+0045e-",
                                       "+0045E-","+0045",

                                       "00.00e00","00.00E00","00.00e+00","00.00E+00","00.00e-00",
                                       "00.00E-00","00.00e01","00.00E01","00.00e+01","00.00E+01",
                                       "00.00e-01","00.00E-01","00.00e010","00.00E010","00.00e+010",
                                       "00.00E+010","00.00e-010","00.00E-010","00.00e10","00.00E10",
                                       "00.00e+10","00.00E+10","00.00e-10","00.00E-10","00.00e2",
                                       "00.00E2","00.00e+2","00.00E+2","00.00e-2","00.00E-2",
                                       "00.00e0","00.00E0","00.00e+0","00.00E+0","00.00e-0",
                                       "00.00E-0","00.00e","00.00E","00.00e+","00.00E+","00.00e-",
                                       "00.00E-","00.00",

                                       ".0e00",".0E00",".0e+00",".0E+00",".0e-00",".0E-00",".0e01",
                                       ".0E01",".0e+01",".0E+01",".0e-01",".0E-01",".0e010",
                                       ".0E010",".0e+010",".0E+010",".0e-010",".0E-010",".0e10",
                                       ".0E10",".0e+10",".0E+10",".0e-10",".0E-10",".0e2",".0E2",
                                       ".0e+2",".0E+2",".0e-2",".0E-2",".0e0",".0E0",".0e+0",
                                       ".0E+0",".0e-0",".0E-0",".0e",".0E",".0e+",".0E+",".0e-",
                                       ".0E-",".0",

                                       "0.e00","0.E00","0.e+00","0.E+00","0.e-00","0.E-00","0.e01",
                                       "0.E01","0.e+01","0.E+01","0.e-01","0.E-01","0.e010",
                                       "0.E010","0.e+010","0.E+010","0.e-010","0.E-010","0.e10",
                                       "0.E10","0.e+10","0.E+10","0.e-10","0.E-10","0.e2","0.E2",
                                       "0.e+2","0.E+2","0.e-2","0.E-2","0.e0","0.E0","0.e+0",
                                       "0.E+0","0.e-0","0.E-0","0.e","0.E","0.e+","0.E+","0.e-",
                                       "0.E-","0.",

                                       "00e00","00E00","00e+00","00E+00","00e-00","00E-00","00e01",
                                       "00E01","00e+01","00E+01","00e-01","00E-01","00e010",
                                       "00E010","00e+010","00E+010","00e-010","00E-010","00e10",
                                       "00E10","00e+10","00E+10","00e-10","00E-10","00e2","00E2",
                                       "00e+2","00E+2","00e-2","00E-2","00e0","00E0","00e+0",
                                       "00E+0","00e-0","00E-0","00e","00E","00e+","00E+","00e-",
                                       "00E-","00",

                                       "0e00","0E00","0e+00","0E+00","0e-00","0E-00","0e01","0E01",
                                       "0e+01","0E+01","0e-01","0E-01","0e010","0E010","0e+010",
                                       "0E+010","0e-010","0E-010","0e10","0E10","0e+10","0E+10",
                                       "0e-10","0E-10","0e2","0E2","0e+2","0E+2","0e-2","0E-2",
                                       "0e0","0E0","0e+0","0E+0","0e-0","0E-0","0e","0E","0e+",
                                       "0E+","0e-","0E-","0",

                                       "-00.00e00","-00.00E00","-00.00e+00","-00.00E+00",
                                       "-00.00e-00","-00.00E-00","-00.00e01","-00.00E01",
                                       "-00.00e+01","-00.00E+01","-00.00e-01","-00.00E-01",
                                       "-00.00e010","-00.00E010","-00.00e+010","-00.00E+010",
                                       "-00.00e-010","-00.00E-010","-00.00e10","-00.00E10",
                                       "-00.00e+10","-00.00E+10","-00.00e-10","-00.00E-10",
                                       "-00.00e2","-00.00E2","-00.00e+2","-00.00E+2","-00.00e-2",
                                       "-00.00E-2","-00.00e0","-00.00E0","-00.00e+0","-00.00E+0",
                                       "-00.00e-0","-00.00E-0","-00.00e","-00.00E","-00.00e+",
                                       "-00.00E+","-00.00e-","-00.00E-","-00.00",

                                       "-.0e00","-.0E00","-.0e+00","-.0E+00","-.0e-00","-.0E-00",
                                       "-.0e01","-.0E01","-.0e+01","-.0E+01","-.0e-01","-.0E-01",
                                       "-.0e010","-.0E010","-.0e+010","-.0E+010","-.0e-010",
                                       "-.0E-010","-.0e10","-.0E10","-.0e+10","-.0E+10","-.0e-10",
                                       "-.0E-10","-.0e2","-.0E2","-.0e+2","-.0E+2","-.0e-2",
                                       "-.0E-2","-.0e0","-.0E0","-.0e+0","-.0E+0","-.0e-0","-.0E-0",
                                       "-.0e","-.0E","-.0e+","-.0E+","-.0e-","-.0E-","-.0",

                                       "-0.e00","-0.E00","-0.e+00","-0.E+00","-0.e-00","-0.E-00",
                                       "-0.e01","-0.E01","-0.e+01","-0.E+01","-0.e-01","-0.E-01",
                                       "-0.e010","-0.E010","-0.e+010","-0.E+010","-0.e-010",
                                       "-0.E-010","-0.e10","-0.E10","-0.e+10","-0.E+10","-0.e-10",
                                       "-0.E-10","-0.e2","-0.E2","-0.e+2","-0.E+2","-0.e-2",
                                       "-0.E-2","-0.e0","-0.E0","-0.e+0","-0.E+0","-0.e-0","-0.E-0",
                                       "-0.e","-0.E","-0.e+","-0.E+","-0.e-","-0.E-","-0.",

                                       "-00e00","-00E00","-00e+00","-00E+00","-00e-00","-00E-00",
                                       "-00e01","-00E01","-00e+01","-00E+01","-00e-01","-00E-01",
                                       "-00e010","-00E010","-00e+010","-00E+010","-00e-010",
                                       "-00E-010","-00e10","-00E10","-00e+10","-00E+10","-00e-10",
                                       "-00E-10","-00e2","-00E2","-00e+2","-00E+2","-00e-2",
                                       "-00E-2","-00e0","-00E0","-00e+0","-00E+0","-00e-0","-00E-0",
                                       "-00e","-00E","-00e+","-00E+","-00e-","-00E-","-00",

                                       "-0e00","-0E00","-0e+00","-0E+00","-0e-00","-0E-00","-0e01",
                                       "-0E01","-0e+01","-0E+01","-0e-01","-0E-01","-0e010",
                                       "-0E010","-0e+010","-0E+010","-0e-010","-0E-010","-0e10",
                                       "-0E10","-0e+10","-0E+10","-0e-10","-0E-10","-0e2","-0E2",
                                       "-0e+2","-0E+2","-0e-2","-0E-2","-0e0","-0E0","-0e+0",
                                       "-0E+0","-0e-0","-0E-0","-0e","-0E","-0e+","-0E+","-0e-",
                                       "-0E-","-0",

                                       "+00.00e00","+00.00E00","+00.00e+00","+00.00E+00",
                                       "+00.00e-00","+00.00E-00","+00.00e01","+00.00E01",
                                       "+00.00e+01","+00.00E+01","+00.00e-01","+00.00E-01",
                                       "+00.00e010","+00.00E010","+00.00e+010","+00.00E+010",
                                       "+00.00e-010","+00.00E-010","+00.00e10","+00.00E10",
                                       "+00.00e+10","+00.00E+10","+00.00e-10","+00.00E-10",
                                       "+00.00e2","+00.00E2","+00.00e+2","+00.00E+2","+00.00e-2",
                                       "+00.00E-2","+00.00e0","+00.00E0","+00.00e+0","+00.00E+0",
                                       "+00.00e-0","+00.00E-0","+00.00e","+00.00E","+00.00e+",
                                       "+00.00E+","+00.00e-","+00.00E-","+00.00",

                                       "+.0e00","+.0E00","+.0e+00","+.0E+00","+.0e-00","+.0E-00",
                                       "+.0e01","+.0E01","+.0e+01","+.0E+01","+.0e-01","+.0E-01",
                                       "+.0e010","+.0E010","+.0e+010","+.0E+010","+.0e-010",
                                       "+.0E-010","+.0e10","+.0E10","+.0e+10","+.0E+10","+.0e-10",
                                       "+.0E-10","+.0e2","+.0E2","+.0e+2","+.0E+2","+.0e-2",
                                       "+.0E-2","+.0e0","+.0E0","+.0e+0","+.0E+0","+.0e-0","+.0E-0",
                                       "+.0e","+.0E","+.0e+","+.0E+","+.0e-","+.0E-","+.0",

                                       "+0.e00","+0.E00","+0.e+00","+0.E+00","+0.e-00","+0.E-00",
                                       "+0.e01","+0.E01","+0.e+01","+0.E+01","+0.e-01","+0.E-01",
                                       "+0.e010","+0.E010","+0.e+010","+0.E+010","+0.e-010",
                                       "+0.E-010","+0.e10","+0.E10","+0.e+10","+0.E+10","+0.e-10",
                                       "+0.E-10","+0.e2","+0.E2","+0.e+2","+0.E+2","+0.e-2",
                                       "+0.E-2","+0.e0","+0.E0","+0.e+0","+0.E+0","+0.e-0","+0.E-0",
                                       "+0.e","+0.E","+0.e+","+0.E+","+0.e-","+0.E-","+0.",

                                       "+00e00","+00E00","+00e+00","+00E+00","+00e-00","+00E-00",
                                       "+00e01","+00E01","+00e+01","+00E+01","+00e-01","+00E-01",
                                       "+00e010","+00E010","+00e+010","+00E+010","+00e-010",
                                       "+00E-010","+00e10","+00E10","+00e+10","+00E+10","+00e-10",
                                       "+00E-10","+00e2","+00E2","+00e+2","+00E+2","+00e-2",
                                       "+00E-2","+00e0","+00E0","+00e+0","+00E+0","+00e-0","+00E-0",
                                       "+00e","+00E","+00e+","+00E+","+00e-","+00E-","+00",

                                       "+0e00","+0E00","+0e+00","+0E+00","+0e-00","+0E-00","+0e01",
                                       "+0E01","+0e+01","+0E+01","+0e-01","+0E-01","+0e010",
                                       "+0E010","+0e+010","+0E+010","+0e-010","+0E-010","+0e10",
                                       "+0E10","+0e+10","+0E+10","+0e-10","+0E-10","+0e2","+0E2",
                                       "+0e+2","+0E+2","+0e-2","+0E-2","+0e0","+0E0","+0e+0",
                                       "+0E+0","+0e-0","+0E-0","+0e","+0E","+0e+","+0E+","+0e-",
                                                                                       "+0E-","+0"};
        char keyword[20]={'\0'}, pStringToPrint[80][20]={""}, pValueToPrint[80][20]={""};
        char pSpaceStringValue[80][20]={""}, pSpaceValueString[80][20]={""}, *stringToPrint=NULL;
        char pSaveString[9][121]={""}, *nameInfo=NULL, nameTest[NAME_LENGTH+10]="";

        // Initializing to zero the structure of parameters and parametersRef
        for (i=0; i<9; i++)
        {
                *pString[i]=NULL;
                *pStringRef[i]=NULL;
        }
        for (i=0; i<20; i++)
        {
                *pInteger[i]=0;
                *pIntegerRef[i]=0;
        }
        for (i=0; i<31; i++)
        {
                *pDouble[i]=.0;
                *pDoubleRef[i]=.0;
        }

        // Test starts
        time(&startTimer);
        printf("\nTesting loadParameters function.\n");

        expectedValue=0;
        PRINT_TEST_START(counter,expectedValue);
        printf("pParameters=%p\nnameTest=%p\n",pParameters,nameInfo);
        returnValue=loadParameters(pParameters,nameInfo);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,readChar);

        pParameters=&parameters;
        PRINT_TEST_START(counter,expectedValue);
        printf("pParameters=%p\nnameTest=%p\n",pParameters,nameInfo);
        returnValue=loadParameters(pParameters,nameInfo);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,readChar);

        nameInfo=nameTest;
        pParameters=NULL;
        PRINT_TEST_START(counter,expectedValue);
        printf("pParameters=%p\nnameTest=%p\n",pParameters,nameInfo);
        returnValue=loadParameters(pParameters,nameInfo);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,readChar);

        pParameters=&parameters;
        PRINT_TEST_START(counter,expectedValue);
        printf("pParameters=%p\nnameTest=%p\n",pParameters,nameInfo);
        returnValue=loadParameters(pParameters,nameInfo);
        PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,readChar);

        for (i=0; i<10; i++)
        {
                switch(i)
                {
                        case 0: // Warning here: we must have NAME_LENGTH>4
                                for (j=0; j<NAME_LENGTH-4; j++)
                                {
                                        nameTest[j]='a';
                                }
                                nameTest[NAME_LENGTH-4]='\0';
                                for (j=NAME_LENGTH-3; j<NAME_LENGTH+10; j++)
                                {
                                        nameTest[j]='A';
                                }
                                break;

                        case 1: // Warning here: we must have NAME_LENGTH>4
                                for (j=0; j<NAME_LENGTH-4; j++)
                                {
                                        nameTest[j]='b';
                                }
                                for (j=NAME_LENGTH-4; j<NAME_LENGTH+10; j++)
                                {
                                        nameTest[j]='\0';
                                }
                                break;

                        case 2:
                                for (j=0; j<NAME_LENGTH-1; j++)
                                {
                                        nameTest[j]='c';
                                }
                                nameTest[NAME_LENGTH-1]='\0';
                                for (j=NAME_LENGTH; j<NAME_LENGTH+10; j++)
                                {
                                        nameTest[j]='C';
                                }
                                break;

                        case 3:
                                for (j=0; j<NAME_LENGTH-1; j++)
                                {
                                        nameTest[j]='d';
                                }
                                for (j=NAME_LENGTH-1; j<NAME_LENGTH+10; j++)
                                {
                                         nameTest[j]='\0';
                                }
                                break;

                        case 4:
                                for (j=0; j<NAME_LENGTH; j++)
                                {
                                        nameTest[j]='e';
                                }
                                nameTest[NAME_LENGTH]='\0';
                                for (j=NAME_LENGTH+1; j<NAME_LENGTH+10; j++)
                                {
                                         nameTest[j]='E';
                                }
                                break;

                         case 5:
                                for (j=0; j<NAME_LENGTH; j++)
                                {
                                        nameTest[j]='f';
                                }
                                for (j=NAME_LENGTH; j<NAME_LENGTH+10; j++)
                                {
                                         nameTest[j]='\0';
                                }
                                break;

                        case 6:
                                for (j=0; j<NAME_LENGTH+1; j++)
                                {
                                        nameTest[j]='g';
                                }
                                nameTest[NAME_LENGTH+1]='\0';
                                for (j=NAME_LENGTH+2; j<NAME_LENGTH+10; j++)
                                {
                                         nameTest[j]='G';
                                }
                                break;

                         case 7:
                                for (j=0; j<NAME_LENGTH+1; j++)
                                {
                                        nameTest[j]='h';
                                }
                                for (j=NAME_LENGTH+1; j<NAME_LENGTH+10; j++)
                                {
                                         nameTest[j]='\0';
                                }
                                break;

                        case 8:
                                for (j=0; j<NAME_LENGTH+9; j++)
                                {
                                         nameTest[j]='i';
                                }
                                nameTest[NAME_LENGTH+9]='\0';
                                break;

                        case 9:
                                strncpy(nameTest,"test.info",NAME_LENGTH+10);
                                break;
                }

                PRINT_TEST_START(counter,expectedValue);
                printf("nameTest=%s\n",nameTest);
                returnValue=loadParameters(pParameters,nameInfo);
                for (j=0; j<9; j++)
                {
                        free(*pString[j]);
                }
                PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                                          readChar);
        }

        for (iRandom=0; iRandom<50000; iRandom++)
        {
                // Giving random values to the variables of the structure pParameters
                for (i=0; i<9; i++)
                {
                        lengthName=rand()%120+1;
                        if (lengthName>100)
                        {
                                *pString[i]=NULL;
                        }
                        else
                        {
                                *pString[i]=(char*)calloc(lengthName,sizeof(char));
                        }
                        if (*pString[i]!=NULL)
                        {
                                for (j=0; j<lengthName; j++)
                                {
                                        (*pString[i])[j]=rand()%90+33;
                                }
                                if (rand()%2)
                                {
                                        (*pString[i])[rand()%lengthName]='\0';
                                }
                        }
                }
                for (i=0; i<20; i++)
                {
                        *pInteger[i]=rand()%121-20;
                }
                for (i=0; i<31; i++)
                {
                        *pDouble[i]=(double)(rand()%60001-30000)/10000.;
                }

                // Fill pStringToPrint, pSpaceStringValue, pSpaceValueString, and pValueToPrint
                expectedValue=1;
                booleanForEndData=0;
                booleanForName=0;
                booleanForPath=0;

                for (i=0; i<80; i++)
                {
                        strncpy(pStringToPrint[i],"xxxxxxxxxxxxxxxxxxx",20);
                        strncpy(pSpaceStringValue[i],"xxxxxxxxxxxxxxxxxxx",20);
                        strncpy(pValueToPrint[i],"xxxxxxxxxxxxxxxxxxx",20);
                        strncpy(pSpaceValueString[i],"xxxxxxxxxxxxxxxxxxx",20);
                }
                for (i=0; i<62; i++)
                {
                        repetition[i]=0;
                }

                iMax=rand()%79;
                for (i=0; i<iMax; i++)
                {
                        // Set up keyword thanks to the integer j
                        j=rand()%61;
                        stringToPrint=pKeyword[j];
                        if (iRandom<12000 && rand()%3==0)
                        {
                                for (k=0; k<20; k++)
                                {
                                        keyword[k]=rand()%90+33;
                                }
                                keyword[rand()%20]='\0';
                                stringToPrint=keyword;
                                for (j=0; j<61; j++)
                                {
                                        if (!strcmp(stringToPrint,pKeyword[j]))
                                        {
                                                break;
                                        }
                                }
                        }
                        strncpy(pStringToPrint[i],stringToPrint,20);

                        if (!booleanForEndData)
                        {
                                 repetition[j]++;
                        }
                        if (j==1)
                        {
                                booleanForEndData=1;
                        }

                        // Set up space between keyword and value
                        stringToPrint=pSpace[1];
                        if (rand()%3==0)
                        {
                                stringToPrint=pSpace[rand()%35];
                        }
                        strncpy(pSpaceStringValue[i],stringToPrint,20);

                        // Set up value associated with keyword
                        if (iRandom<20000)
                        {
                                if (rand()%2)
                                {
                                        if (rand()%3==0)
                                        {
                                                k=rand()%2192;
                                                stringToPrint=pDoubleToPrint[k];
                                                strncpy(pValueToPrint[i],stringToPrint,20);
                                        }
                                        else
                                        {
                                                k=2192;
                                                doubleToPrint=(double)(rand()%6001-3000)/1000.;
                                                if (sprintf(pValueToPrint[i],"%lf",doubleToPrint)<0)
                                                {
                                                        k=2193;
                                                        strncpy(pValueToPrint[i],pKeyword[0],20);
                                                }
                                        }
                                }
                                else
                                {
                                        if (rand()%3==0)
                                        {
                                                k=rand()%28;
                                                stringToPrint=pIntegerToPrint[k];
                                                strncpy(pValueToPrint[i],stringToPrint,20);
                                                k=-k;
                                        }
                                        else
                                        {
                                                k=-28;
                                                integerToPrint=rand()%201-100;
                                                if (sprintf(pValueToPrint[i],"%d",integerToPrint)<0)
                                                {
                                                        k=-29;
                                                        strncpy(pValueToPrint[i],pKeyword[0],20);
                                                }
                                        }
                                }
                        }
                        else
                        {
                                if (j>9 && j<30)
                                {
                                        k=rand()%28;
                                        stringToPrint=pIntegerToPrint[k];
                                        k=-k;
                                }
                                else if (j>29 && j<61)
                                {
                                        k=rand()%2192;
                                        stringToPrint=pDoubleToPrint[k];
                                }
                                else
                                {
                                        for (k=0; k<20; k++)
                                        {
                                                keyword[k]=rand()%90+33;
                                        }
                                        keyword[rand()%19+1]='\0';
                                        stringToPrint=keyword;
                                        k=-30;
                                }
                                strncpy(pValueToPrint[i],stringToPrint,20);
                        }

                        // Set up space between value and keyword
                        stringToPrint=pSpace[5];
                        if (rand()%3==0)
                        {
                                stringToPrint=pSpace[rand()%35];
                        }
                        strncpy(pSpaceValueString[i],stringToPrint,20);

                        // Set up the expected value
                        if (!booleanForEndData)
                        {
                                if (j==0 || j==61)
                                {
                                        expectedValue=0;
                                }
                                else if (j>1 && j<5)
                                {
                                        if (pSpaceValueString[i][0]=='\0')
                                        {
                                                expectedValue=0;
                                        }
                                        else if (strlen(pValueToPrint[i])<6 ||
                                                   strlen(pValueToPrint[i])>=DEF_MAX(booleanForName,
                                                   pParameters->name_length) || *pString[j-1]!=NULL)
                                        {
                                                expectedValue=0;
                                        }
                                }
                                else if (j>4 && j<10)
                                {
                                        if (pSpaceValueString[i][0]=='\0')
                                        {
                                                expectedValue=0;
                                        }
                                        else if (strlen(pValueToPrint[i])>=
                                                 DEF_MAX(booleanForPath,pParameters->path_length) ||
                                                                                *pString[j-1]==NULL)
                                        {
                                                expectedValue=0;
                                        }
                                }
                                else if (j>9 && j<30)
                                {
                                        if (k>-3 || k<-28)
                                        {
                                                switch(k)
                                                {
                                                        case 557:
                                                        case 600:
                                                        case 1030:
                                                        case 1073:
                                                        case 1503:
                                                        case 1546:
                                                        case 1718:
                                                        case 1761:
                                                        case 1933:
                                                        case 1976:
                                                        case 2148:
                                                        case 2191:
                                                                break;

                                                        default:
                                                                expectedValue=0;
                                                                break;
                                                }
                                        }
                                        else if (j==10)
                                        {
                                                integerToPrint=atoi(pValueToPrint[i]);
                                                if (integerToPrint<pParameters->name_length)
                                                {
                                                        expectedValue=0;
                                                }
                                                else
                                                {
                                                        booleanForName=integerToPrint;
                                                }
                                        }
                                        else if (j==11)
                                        {
                                                integerToPrint=atoi(pValueToPrint[i]);
                                                if (integerToPrint<pParameters->path_length)
                                                {
                                                        expectedValue=0;
                                                }
                                                else
                                                {
                                                        booleanForPath=integerToPrint;
                                                }
                                        }
                                }
                                else if (j>29 && j<61)
                                {
                                        if (k==-29 || k==2193)
                                        {
                                                expectedValue=0;
                                        }
                                        else if (!k && !strcmp(pValueToPrint[i],pDoubleToPrint[0]))
                                        {
                                                expectedValue=0;
                                        }
                                        else if (k>0 && k<128)
                                        {
                                                expectedValue=0;
                                        }
                                }
                        }

                        // Update the Ref reference structure according to the file to be written
                        if (!booleanForEndData)
                        {
                                if (j>1 && j<10)
                                {
                                        strncpy(pSaveString[j-1],pValueToPrint[i],121);
                                }
                                else if (j>9 && j<30)
                                {
                                        integerToPrint=atoi(pValueToPrint[i]);
                                        *pIntegerRef[j-10]=integerToPrint;
                                }
                                else if (j>29 && j<61)
                                {
                                        doubleToPrint=atof(pValueToPrint[i]);
                                        *pDoubleRef[j-30]=doubleToPrint;
                                }
                        }
                }

                if (rand()%5==0)
                {
                        booleanForEndData=1;
                        strncpy(pStringToPrint[iMax],"end_data",20);
                }

                PRINT_TEST_START(counter,expectedValue);

                // Writing the random test.info file
                infoFile=fopen("test.info","w+");
                if (infoFile==NULL)
                {
                        printf("\nError: could not create the test.info file.\n");
                        expectedValue=-1;
                }
                else
                {
                        if (rand()%20==0)
                        {
                                stringToPrint=pSpace[rand()%35];
                                fprintf(infoFile,"%s",stringToPrint);
                        }
                        for (i=0; i<iMax; i++)
                        {
                                fprintf(infoFile,"%s%s",pStringToPrint[i],pSpaceStringValue[i]);
                                fprintf(infoFile,"%s%s",pValueToPrint[i],pSpaceValueString[i]);
                        }
                        if (!strcmp(pStringToPrint[iMax],"end_data"))
                        {
                                fprintf(infoFile,"%s",pStringToPrint[i]);
                        }
                        if (fclose(infoFile))
                        {
                                printf("\nError: could not close the test.info file properly.\n");
                                expectedValue=-1;
                        }
                        infoFile=NULL;
                }

                // Printing the value of the different variables
                for (i=0; i<9; i++)
                {
                        printf("%s=%s\n",pStringName[i],*pString[i]);
                }
                for (i=0; i<20; i++)
                {
                        printf("%s=%d\n",pIntegerName[i],*pInteger[i]);
                }
                for (i=0; i<31; i++)
                {
                        printf("%s=%lf\n",pDoubleName[i],*pDouble[i]);
                }

                // Free the memory allocated for pParameters
                for (i=0; i<9; i++)
                {
                        free(*pString[i]);
                }

                // Testing the function
                returnValue=loadParameters(pParameters,nameInfo);
*/
/*
                // Check that the parameters have been corrected loaded
                if (returnValue)
                {
                        for (i=0; i<9; i++)
                        {
                                if (*pString[i]==NULL)
                                {
                                        if (*pStringRef[i]!=NULL)
                                        {
                                                expectedValue=0;
                                        }
                                }
                                else
                                {
                                        if (strcmp(*pString[i],pSaveString[i]))
                                        {
                                               expectedValue=0;
                                        }
                                }
                        }
                        for (i=0; i<20; i++)
                        {
                                if (*pInteger[i]!=*pIntegerRef[i])
                                {
                                        expectedValue=0;
                                }
                        }
                        for (i=0; i<31; i++)
                        {
                                if (*pDouble[i]!=*pDoubleRef[i])
                                {
                                        expectedValue=0;
                                }
                        }
                }
*/
/*
                PRINT_TEST_END(counter,counterSuccess,counterFail,returnValue,expectedValue,
                                                                                          readChar);

                // Free the memory allocated for strings
                for (i=0; i<9; i++)
                {
                        free(*pString[i]);
                        *pString[i]=NULL;
                }
        }
        system("rm test.info");

        // End of the tests
        printf("\nTotal: %d tests (%d succeeded, %d failed) ",counter,counterSuccess,counterFail);
        time(&endTimer);
        if (difftime(endTimer,startTimer)<60.)
        {
                printf("done in %lf seconds.\n",difftime(endTimer,startTimer));
        }
        else
        {
                printf("done in %lf minutes.\n",difftime(endTimer,startTimer)/60.);
        }

        return;
}
*/

