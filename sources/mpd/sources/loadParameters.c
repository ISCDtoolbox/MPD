/**
* \file loadParameters.c
* \brief It contains all the functions used to load properly the parameters
*        from a *.input file (and use the default values if they are not
*        specified) in the MPD algorithm.
* \author Jeremy DALPHIN
* \version 3.0
* \date August 1st, 2019
*
* The main function of this file is called \ref loadParameters and many other
* functions should be static but have been defined as non-static for performing
* unit-tests on them.
*/

#include "loadParameters.h"

////////////////////////////////////////////////////////////////////////////////
// The function initializeParameterStructure sets to zero all the variables of
// the structure Parameters (and pointers to NULL). It has the Parameters*
// variable (defined in main.h) as input argument and it does not return value
////////////////////////////////////////////////////////////////////////////////
void initializeParameterStructure(Parameters* pParameters)
{
    // 81 parameters
    if (pParameters!=NULL)
    {
        pParameters->opt_mode=0;
        pParameters->verbose=0;
        pParameters->n_cpu=0;
        pParameters->rho_opt=0.;

        pParameters->name_length=0;
        pParameters->name_input=NULL;
        pParameters->name_result=NULL;
        pParameters->name_chem=NULL;
        pParameters->name_mesh=NULL;
        pParameters->name_elas=NULL;

        pParameters->nu_electrons=0;
        pParameters->bohr_unit=0;
        pParameters->select_orb=0.;
        pParameters->orb_ortho=0;
        pParameters->ne_electrons=0;
        pParameters->multi_det=0;
        pParameters->orb_rhf=0;

        pParameters->select_box=0.;
        pParameters->x_min=0.;
        pParameters->y_min=0.;
        pParameters->z_min=0.;

        pParameters->x_max=0.;
        pParameters->y_max=0.;
        pParameters->z_max=0.;

        pParameters->n_x=0;
        pParameters->n_y=0;
        pParameters->n_z=0;

        pParameters->delta_x=0.;
        pParameters->delta_y=0.;
        pParameters->delta_z=0.;

        pParameters->ls_ini=0;
        pParameters->ls_type=0;
        pParameters->ls_x=0.;
        pParameters->ls_y=0.;
        pParameters->ls_z=0.;
        pParameters->ls_rx=0.;
        pParameters->ls_ry=0.;
        pParameters->ls_rz=0.;

        pParameters->met_err=0.;
        pParameters->met_min=0.;
        pParameters->met_max=0.;

        pParameters->trick_matrix=0;
        pParameters->approx_mode=0;

        pParameters->iter_ini=0;
        pParameters->iter_max=0;
        pParameters->iter_told0p=0.;
        pParameters->iter_told1p=0.;
        pParameters->iter_told2p=0.;

        pParameters->save_type=0;
        pParameters->save_mesh=0;
        pParameters->save_data=0;
        pParameters->save_print=0;
        pParameters->save_where=0;

        pParameters->path_length=0;
        pParameters->path_medit=NULL;
        pParameters->path_mmg3d=NULL;
        pParameters->path_mshdist=NULL;
        pParameters->path_elastic=NULL;
        pParameters->path_advect=NULL;

        pParameters->hmin_iso=0.;
        pParameters->hmax_iso=0.;
        pParameters->hausd_iso=0.;
        pParameters->hgrad_iso=0.;

        pParameters->hmin_met=0.;
        pParameters->hmax_met=0.;
        pParameters->hausd_met=0.;
        pParameters->hgrad_met=0.;

        pParameters->hmin_ls=0.;
        pParameters->hmax_ls=0.;
        pParameters->hausd_ls=0.;
        pParameters->hgrad_ls=0.;

        pParameters->hmode_lag=0;
        pParameters->hmin_lag=0.;
        pParameters->hmax_lag=0.;
        pParameters->hausd_lag=0.;
        pParameters->hgrad_lag=0.;

        pParameters->memory=0;
        pParameters->n_iter=0;
        pParameters->residual=0.;
        pParameters->delta_t=0.;
        pParameters->no_cfl=0;
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////
// The function freeParameterMemory frees the memory dynamically allocated with
// calloc/malloc/realloc for the structure Parameters (but other variables than
// pointers are not reset to zero). It has the Parameters* variable (defined in
// main.h) as input argument and it does not return any value
////////////////////////////////////////////////////////////////////////////////
void freeParameterMemory(Parameters* pParameters)
{
    if (pParameters!=NULL)
    {
        // The free function does not return any value (void output)
        free(pParameters->name_input);
        pParameters->name_input=NULL;

        free(pParameters->name_result);
        pParameters->name_result=NULL;

        free(pParameters->name_chem);
        pParameters->name_chem=NULL;

        free(pParameters->name_mesh);
        pParameters->name_mesh=NULL;

        free(pParameters->name_elas);
        pParameters->name_elas=NULL;

        free(pParameters->path_medit);
        pParameters->path_medit=NULL;

        free(pParameters->path_mmg3d);
        pParameters->path_mmg3d=NULL;

        free(pParameters->path_mshdist);
        pParameters->path_mshdist=NULL;

        free(pParameters->path_elastic);
        pParameters->path_elastic=NULL;

        free(pParameters->path_advect);
        pParameters->path_advect=NULL;
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////
// The function checkForTildeAndReplaceByHomePath evaluates if pStringToCheck
// is pointing to a string of (positive) length (including the terminating nul
// character '\0') less than maximumLength thanks to the checkStringFromLength
// function. Then, it looks if the string pointed by pStringToCheck begins with
// the usual shortcut '~/' for the home path directory. If it is the case, then
// it recovers the full path for the home directory and replace it inside the
// string pointed by pStringToCheck, adding some extra length if needed. It has
// the char** pStringTocheck and the int variable maximumLength (>1) as input
// arguments. It returns zero if an error is encountered, otherwise it returns
// the (positive) length of the new updated string pointed by pStringToCheck
// (including the terminating character '\0')
////////////////////////////////////////////////////////////////////////////////
int checkForTildeAndReplaceByHomePath(char** pStringToCheck, int maximumLength)
{
    size_t lengthString=0, lengthPath=0, lengthTotal=0;
    char *pathName=NULL, *newString=NULL;
    int readChar=0, i=0, iMax=0, j=0, jMax=0;
    FILE *fileName=NULL;

    // Check if pStringToCheck is pointing to NULL
    if (pStringToCheck==NULL)
    {
        PRINT_ERROR("In checkForTildeAndReplaceByHomePath: the input ");
        fprintf(stderr,"(char**) variable ");
        fprintf(stderr,"pStringToCheck=%p ",(void*)pStringToCheck);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check the length of the string pointed by pStringToCheck
    lengthString=checkStringFromLength(*pStringToCheck,2,maximumLength);
    if (!lengthString)
    {
        PRINT_ERROR("In checkForTildeAndReplaceByHomePath: ");
        fprintf(stderr,"checkStringFromLength function returned zero, which ");
        fprintf(stderr,"is not the expected value here.\n");
        return 0;
    }

    // Check for the '~/' home path directory
    if ((*pStringToCheck)[0]=='~' && (*pStringToCheck)[1]=='/')
    {
        // Remove getHomePath.txt if the file already exists
        i=initialFileExists("getHomePath.txt",16);
        if (abs(i)!=1)
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: ");
            fprintf(stderr,"initialFileExists function returned zero instead ");
            fprintf(stderr,"of (+/-) one, while attempting to check if the ");
            fprintf(stderr,"getHomePath.txt file already exists.\n");
            return 0;
        }
        else if (i==1)
        {
            // remove returns 0 on success, otherwise -1
            if (remove("getHomePath.txt"))
            {
                PRINT_ERROR("In checkForTildeAndReplaceByHomePath: wrong ");
                fprintf(stderr,"return (=-1) of the standard remove ");
                fprintf(stderr,"c-function in the attempt of removing the ");
                fprintf(stderr,"getHomePath.txt file.\n");
                return 0;
            }
        }

        // system returns is -1 on error, otherwise the return command status
        if (system("echo $HOME >getHomePath.txt"))
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: wrong return ");
            fprintf(stderr,"(=-1) of the standard system c-function in the ");
            fprintf(stderr,"attempt of writing the home directory path name ");
            fprintf(stderr,"in the getHomePath.txt file.\n");
            return 0;
        }

        // fopen function returns a FILE pointer, otherwise NULL (file must have
        // been previously created to get a non-NULL pointer in reading mode)
        fileName=fopen("getHomePath.txt","r");
        if (fileName==NULL)
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: we were not ");
            fprintf(stderr,"able to open properly the getHomePath.txt file.\n");
            return 0;
        }

        // Count the number of characters saved in getHomePath.txt
        // fgetc returns the char read as an unsigned char cast to an int or EOF
        lengthPath=0;
        readChar=fgetc(fileName);
        while (readChar!='\n' && readChar!=EOF)
        {
            lengthPath++;
            readChar=fgetc(fileName);
        }

        // fclose function returns zero if the input FILE* variable is
        // successfully closed, otherwise EOF (end-of-file) is returned
        if (fclose(fileName))
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: we were not ");
            fprintf(stderr,"able to close properly the getHomePath.txt ");
            fprintf(stderr,"file.\n");
            fileName=NULL;
            return 0;
        }
        fileName=NULL;

        // Allocate memory for the (new) path name if the length is positive
        if (!lengthPath)
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: the ");
            fprintf(stderr,"getHomePath.txt file does not contain any home ");
            fprintf(stderr,"directory path name.\n");
            return 0;
        }
        lengthTotal=lengthPath+lengthString-1;

        // calloc returns a pointer to the allocated memory, otherwise NULL
        pathName=(char*)calloc(lengthTotal,sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: could not ");
            fprintf(stderr,"allocate memory for the local (char*) pathName ");
            fprintf(stderr,"variable.\n");
            return 0;
        }

        // Read the path name and store it in pathName
        fileName=fopen("getHomePath.txt","r");
        if (fileName==NULL)
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: we were not ");
            fprintf(stderr,"able to (re-)open properly the getHomePath.txt ");
            fprintf(stderr,"file.\n");
            free(pathName);
            pathName=NULL;
            return 0;
        }

        i=0;
        iMax=lengthPath;
        readChar=fgetc(fileName);
        while (readChar!='\n' && readChar!=EOF && i<iMax)
        {
            pathName[i]=(char)readChar;
            readChar=fgetc(fileName);
            i++;
        }
        pathName[i]='\0';

        if (fclose(fileName))
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: we were not ");
            fprintf(stderr,"able to close (again) the getHomePath.txt file.\n");
            free(pathName);
            pathName=NULL;
            fileName=NULL;
            return 0;
        }
        fileName=NULL;

        if (i!=iMax || pathName[0]!='/')
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: something ");
            fprintf(stderr,"went wrong in the reading of the home directory ");
            fprintf(stderr,"path name written in the getHomePath.txt file; ");
            fprintf(stderr,"expecting %d characters instead of %d and ",iMax,i);
            fprintf(stderr,"the string %s has been read for the ",pathName);
            fprintf(stderr,"home directory path name (which should also ");
            fprintf(stderr,"starts with the '/' character).\n");
            free(pathName);
            pathName=NULL;
            return 0;
        }

        if (remove("getHomePath.txt"))
        {
            PRINT_ERROR("In checkForTildeAndReplaceByHomePath: wrong return ");
            fprintf(stderr,"(=-1) of the standard remove c-function in the ");
            fprintf(stderr,"attempt of removing the getHomePath.txt file.\n");
            free(pathName);
            pathName=NULL;
            return 0;
        }

        // Add the old path name to the remaining part of the pathName variable
        jMax=lengthTotal;
        for (j=i; j<jMax; j++)
        {
            pathName[j]=(*pStringToCheck)[j-i+1];
        }
        pathName[jMax-1]='\0';

        // realloc returns a pointer to the newly allocated memory, or NULL
        if (jMax>maximumLength)
        {
            newString=(char*)realloc(*pStringToCheck,lengthTotal*sizeof(char));
            if (newString==NULL)
            {
                PRINT_ERROR("In checkForTildeAndReplaceByHomePath: could not ");
                fprintf(stderr,"reallocate memory for the string pointed by ");
                fprintf(stderr,"the input (char**) pStringToCheck variable.\n");
                free(pathName);
                pathName=NULL;
                return 0;
            }
            *pStringToCheck=newString;

            // strncpy function returns a pointer to the string (not used here)
            strncpy(*pStringToCheck,pathName,lengthTotal);
        }
        else
        {
            strncpy(*pStringToCheck,pathName,maximumLength);
        }
        lengthString=lengthTotal;

        // Free the memory allocated for pathName
        free(pathName);
        pathName=NULL;
    }

    return lengthString;
}

////////////////////////////////////////////////////////////////////////////////
// The function setupDefaultParameters fills all the variables of the structure
// pointed by pParameters to their default values (given by the preprocessor
// constants of loadParameters.h). The nameInputFile variable, which should
// contain the name of the *.input file, is stored in the name_input variable of
// the structure Parameters. The function has the Parameters* variable (defined
// in main.h) and the char* nameInputFile variable as input arguments and it
// returns zero if an error occurred, otherwise one is returned for success
////////////////////////////////////////////////////////////////////////////////
int setupDefaultParameters(Parameters* pParameters, char* nameInputFile)
{
    char *pathName=NULL;
    int lengthName=0, i=0;

    // Check if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: the (input) variable ");
        fprintf(stderr,"pParameters=%p does not point to ",(void*)pParameters);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the nameInputFile variable
    if (!checkInputFileName(nameInputFile,NAME_LENGTH))
    {
        PRINT_ERROR("In setupDefaultParameters: checkInputFileName function ");
        fprintf(stderr,"returned zero instead of one, after having checked ");
        fprintf(stderr,"that the input (char*) variable called ");
        fprintf(stderr,"nameInputFile, which was supposed to store the name ");
        fprintf(stderr,"of the *.input file, is not a string of length ");
        fprintf(stderr,"strictly less than %d (and strictly more ",NAME_LENGTH);
        fprintf(stderr,"than 7) that contains a (non-empty) name ended by ");
        fprintf(stderr,"the '.input' file extension.\n");
        return 0;
    }

    // Setting up the default values to the variables
    pParameters->opt_mode=OPT_MODE;
    pParameters->verbose=VERBOSE;
    pParameters->n_cpu=N_CPU;
    pParameters->rho_opt=RHO_OPT;
    pParameters->name_length=NAME_LENGTH;

    // calloc function returns a pointer to the allocated memory, otherwise NULL
    pParameters->name_input=(char*)calloc(NAME_LENGTH,sizeof(char));
    if (pParameters->name_input==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the (char*) pParameters->name_input variable.\n");
        return 0;
    }

    // strncpy function returns a pointer to the string (not used here)
    strncpy(pParameters->name_input,nameInputFile,NAME_LENGTH);

    // Check for './' at the beginning of the string of pParameters->name_input
    if (pParameters->name_input[0]=='.' && pParameters->name_input[1]=='/')
    {
        for (i=2; i<NAME_LENGTH; i++)
        {
            pParameters->name_input[i-2]=pParameters->name_input[i];
        }
        pParameters->name_input[NAME_LENGTH-2]='\0';
        pParameters->name_input[NAME_LENGTH-1]='\0';
    }

    if (pParameters->verbose>0)
    {
        fprintf(stdout,"\nPrescribed values for parameters will be loaded ");
        fprintf(stdout,"from the %s file.",pParameters->name_input);
    }

    pParameters->name_result=NULL;
    pParameters->name_chem=NULL;
    pParameters->name_mesh=NULL;
    pParameters->name_elas=NULL;

    pParameters->nu_electrons=0;
    pParameters->bohr_unit=BOHR_UNIT;
    pParameters->select_orb=SELECT_ORB;
    pParameters->orb_ortho=ORB_ORTHO;

    pParameters->ne_electrons=0;
    pParameters->multi_det=0;
    pParameters->orb_rhf=0;

    pParameters->select_box=SELECT_BOX;
    pParameters->x_min=X_MIN;
    pParameters->y_min=Y_MIN;
    pParameters->z_min=Z_MIN;

    pParameters->x_max=X_MAX;
    pParameters->y_max=Y_MAX;
    pParameters->z_max=Z_MAX;

    pParameters->n_x=N_X;
    pParameters->n_y=N_Y;
    pParameters->n_z=N_Z;

    pParameters->delta_x=DELTA_X;
    pParameters->delta_y=DELTA_Y;
    pParameters->delta_z=DELTA_Z;

    pParameters->ls_ini=0;
    pParameters->ls_type=LS_TYPE;
    pParameters->ls_x=LS_X;
    pParameters->ls_y=LS_Y;
    pParameters->ls_z=LS_Z;
    pParameters->ls_rx=LS_RX;
    pParameters->ls_ry=LS_RY;
    pParameters->ls_rz=LS_RZ;

    pParameters->met_err=MET_ERR;
    pParameters->met_min=MET_MIN;
    pParameters->met_max=MET_MAX;

    pParameters->trick_matrix=TRICK_MATRIX;
    pParameters->approx_mode=APPROX_MODE;

    pParameters->iter_ini=ITER_INI;
    pParameters->iter_max=ITER_MAX;
    pParameters->iter_told0p=ITER_TOLD0P;
    pParameters->iter_told1p=ITER_TOLD1P;
    pParameters->iter_told2p=ITER_TOLD2P;

    pParameters->save_type=SAVE_TYPE;
    pParameters->save_mesh=SAVE_MESH;
    pParameters->save_data=SAVE_DATA;
    pParameters->save_print=SAVE_PRINT;
    pParameters->save_where=SAVE_WHERE;

    pParameters->path_length=PATH_LENGTH;
    pParameters->path_medit=(char*)calloc(pParameters->path_length,
                                                                  sizeof(char));
    if (pParameters->path_medit==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the (char*) pParameters->path_medit variable.\n");
        return 0;
    }
    strncpy(pParameters->path_medit,PATH_MEDIT,pParameters->path_length);
    lengthName=checkForTildeAndReplaceByHomePath(&pParameters->path_medit,
                                                      pParameters->path_length);
    if (!lengthName)
    {
        PRINT_ERROR("In setupDefaultParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_medit variable.\n");
        return 0;
    }
    else if (lengthName>pParameters->path_length)
    {
        pParameters->path_length=lengthName;
    }

    pParameters->path_mmg3d=(char*)calloc(pParameters->path_length,
                                                                  sizeof(char));
    if (pParameters->path_mmg3d==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the (char*) pParameters->path_mmg3d variable.\n");
        return 0;
    }
    strncpy(pParameters->path_mmg3d,PATH_MMG3D,pParameters->path_length);
    lengthName=checkForTildeAndReplaceByHomePath(&pParameters->path_mmg3d,
                                                      pParameters->path_length);
    if (!lengthName)
    {
        PRINT_ERROR("In setupDefaultParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_mmg3d variable.\n");
        return 0;
    }
    else if (lengthName>pParameters->path_length)
    {
        pathName=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_medit ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_medit=pathName;

        // Update with the new length
        pParameters->path_length=lengthName;
    }

    pParameters->path_mshdist=(char*)calloc(pParameters->path_length,
                                                                  sizeof(char));
    if (pParameters->path_mshdist==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the (char*) pParameters->path_mshdist variable.\n");
        return 0;
    }
    strncpy(pParameters->path_mshdist,PATH_MSHDIST,pParameters->path_length);
    lengthName=checkForTildeAndReplaceByHomePath(&pParameters->path_mshdist,
                                                      pParameters->path_length);
    if (!lengthName)
    {
        PRINT_ERROR("In setupDefaultParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_mshdist variable.\n");
        return 0;
    }
    else if (lengthName>pParameters->path_length)
    {
        pathName=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_medit ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_medit=pathName;

        pathName=(char*)realloc(pParameters->path_mmg3d,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_mmg3d ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_mmg3d=pathName;

        // Update with the new length
        pParameters->path_length=lengthName;
    }

    pParameters->path_elastic=(char*)calloc(pParameters->path_length,
                                                                  sizeof(char));
    if (pParameters->path_elastic==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the (char*) pParameters->path_elastic variable.\n");
        return 0;
    }
    strncpy(pParameters->path_elastic,PATH_ELASTIC,pParameters->path_length);
    lengthName=checkForTildeAndReplaceByHomePath(&pParameters->path_elastic,
                                                      pParameters->path_length);
    if (!lengthName)
    {
        PRINT_ERROR("In setupDefaultParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_elastic variable.\n");
        return 0;
    }
    else if (lengthName>pParameters->path_length)
    {
        pathName=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_medit ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_medit=pathName;

        pathName=(char*)realloc(pParameters->path_mmg3d,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_mmg3d ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_mmg3d=pathName;

        pathName=(char*)realloc(pParameters->path_mshdist,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_mshdist ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_mshdist=pathName;

        // Update with the new length
        pParameters->path_length=lengthName;
    }

    pParameters->path_advect=(char*)calloc(pParameters->path_length,
                                                                  sizeof(char));
    if (pParameters->path_advect==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the (char*) pParameters->path_advect variable.\n");
        return 0;
    }
    strncpy(pParameters->path_advect,PATH_ADVECT,pParameters->path_length);
    lengthName=checkForTildeAndReplaceByHomePath(&pParameters->path_advect,
                                                      pParameters->path_length);
    if (!lengthName)
    {
        PRINT_ERROR("In setupDefaultParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_advect variable.\n");
        return 0;
    }
    else if (lengthName>pParameters->path_length)
    {
        pathName=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_medit ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_medit=pathName;

        pathName=(char*)realloc(pParameters->path_mmg3d,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_mmg3d ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_mmg3d=pathName;

        pathName=(char*)realloc(pParameters->path_mshdist,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_mshdist ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_mshdist=pathName;

        pathName=(char*)realloc(pParameters->path_elastic,
                                                       lengthName*sizeof(char));
        if (pathName==NULL)
        {
            PRINT_ERROR("In setupDefaultParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->path_elastic ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->path_elastic=pathName;

        // Update with the new length
        pParameters->path_length=lengthName;
    }

    pParameters->hmin_iso=HMIN_ISO;
    pParameters->hmax_iso=HMAX_ISO;
    pParameters->hausd_iso=HAUSD_ISO;
    pParameters->hgrad_iso=HGRAD_ISO;

    pParameters->hmin_met=HMIN_MET;
    pParameters->hmax_met=HMAX_MET;
    pParameters->hausd_met=HAUSD_MET;
    pParameters->hgrad_met=HGRAD_MET;

    pParameters->hmin_ls=HMIN_LS;
    pParameters->hmax_ls=HMAX_LS;
    pParameters->hausd_ls=HAUSD_LS;
    pParameters->hgrad_ls=HGRAD_LS;

    pParameters->hmode_lag=HMODE_LAG;
    pParameters->hmin_lag=HMIN_LAG;
    pParameters->hmax_lag=HMAX_LAG;
    pParameters->hausd_lag=HAUSD_LAG;
    pParameters->hgrad_lag=HGRAD_LAG;

    pParameters->memory=MEMORY;
    pParameters->n_iter=N_ITER;
    pParameters->residual=RESIDUAL;
    pParameters->delta_t=DELTA_T;
    pParameters->no_cfl=NO_CFL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The function getLengthAfterKeywordBeginning returns the number of characters
// to read at line counter (including the terminating nul one '\0') depending
// on the different possibilities for keywordBeginning. It has a char[3]
// keywordBeginning variable and an int counter variable as input arguments and
// it returns an integer between two and nine, or eleven, otherwise zero is
// returned if an error is encountered
////////////////////////////////////////////////////////////////////////////////
int getLengthAfterKeywordBeginning(char keywordBeginning[3], int counter)
{
    int returnValue=0;

    // Check that counter is between 1 and 81 (total number of keywords)
    if (counter<1 || counter>81)
    {
        PRINT_ERROR("In getLengthAfterKeywordBeginning: the input variable ");
        fprintf(stderr,"counter, corresponding to the %d-th keyword ",counter);
        fprintf(stderr,"read, should be an integer between 1 and 81 (the ");
        fprintf(stderr,"total number of different possible keywords).\n");
        return 0;
    }

    // Check that keywordBeginning is a string made of two characters
    if (!checkStringFromLength(keywordBeginning,3,3))
    {
        PRINT_ERROR("In getLengthAfterKeywordBeginning: ");
        fprintf(stderr,"checkStringFromLength function returned zero, which ");
        fprintf(stderr,"is not the expected value here, after having checked ");
        fprintf(stderr,"that the input char[3] variable keywordBeginning ");
        fprintf(stderr,"is not a string made of two characters, which were ");
        fprintf(stderr,"supposed to be the first two ones of the ");
        fprintf(stderr,"%d-th keyword read.\n",counter);
        return 0;
    }

    // Get the number of characters (including the nul one '\0') to be read next
    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (!strcmp(keywordBeginning,"n_"))
    {
       returnValue=2;
    }
    else if (!strcmp(keywordBeginning,"ls"))
    {
       returnValue=3;
    }
    else if (!strcmp(keywordBeginning,"x_") || !strcmp(keywordBeginning,"y_") ||
                                                 !strcmp(keywordBeginning,"z_"))
    {
       returnValue=4;
    }
    else if (!strcmp(keywordBeginning,"no") || !strcmp(keywordBeginning,"me"))
    {
       returnValue=5;
    }
    else if (!strcmp(keywordBeginning,"ve") || !strcmp(keywordBeginning,"rh") ||
             !strcmp(keywordBeginning,"or") || !strcmp(keywordBeginning,"de") ||
                                                 !strcmp(keywordBeginning,"hm"))
    {
       returnValue=6;
    }
    else if (!strcmp(keywordBeginning,"op") || !strcmp(keywordBeginning,"en") ||
             !strcmp(keywordBeginning,"it") || !strcmp(keywordBeginning,"ha") ||
               !strcmp(keywordBeginning,"hg") || !strcmp(keywordBeginning,"re"))
    {
       returnValue=7;
    }
    else if (!strcmp(keywordBeginning,"na") || !strcmp(keywordBeginning,"bo") ||
               !strcmp(keywordBeginning,"mu") || !strcmp(keywordBeginning,"sa"))
    {
       returnValue=8;
    }
    else if (!strcmp(keywordBeginning,"se") || !strcmp(keywordBeginning,"pa"))
    {
       returnValue=9;
    }
    else if (!strcmp(keywordBeginning,"ap"))
    {
       returnValue=10;
    }
    else if (!strcmp(keywordBeginning,"nu") || !strcmp(keywordBeginning,"ne") ||
                                                 !strcmp(keywordBeginning,"tr"))
    {
       returnValue=11;
    }
    else
    {
        PRINT_ERROR("In getLengthAfterKeywordBeginning: the first two ");
        fprintf(stderr,"letters read (=%s) for the ",keywordBeginning);
        fprintf(stderr,"%d-th keyword do not correspond to the ",counter);
        fprintf(stderr,"beginning of any known keyword for the structure ");
        fprintf(stderr,"Parameters.\n");
        returnValue=0;
    }

    return returnValue;
}

////////////////////////////////////////////////////////////////////////////////
// The function getTypeAfterKeyword, depending on the different cases for
// keywordMiddle (a string of length lengthMiddle including '\0') returns the
// type (-1= no type (end_data keyword), 1=integer, 2=double, 3=string) of
// variables to be read after by fscanf at line counter. It has the char[11]
// keywordMiddle variable, and two int variables (lengthMiddle and counter) as
// input arguments and returns -1, 1, 2, or 3, otherwise zero in case of error
////////////////////////////////////////////////////////////////////////////////
int getTypeAfterKeyword(char keywordMiddle[11], int lengthMiddle, int counter)
{
    int returnValue=0, boolean1=0, boolean2=0, boolean3=0;

    // Check that counter is between 1 and 81 (total number of keywords)
    if (counter<1 || counter>81)
    {
        PRINT_ERROR("In getTypeAfterKeyword: the input variable counter, ");
        fprintf(stderr,"corresponding to the %d-th keyword read, ",counter);
        fprintf(stderr,"should be an integer between 1 and 81 (the total ");
        fprintf(stderr,"number of different possible keywords).\n");
        return 0;
    }

    // Check that lengthMiddle stores keywordMiddle's size (between 2 and 11)
    if (lengthMiddle>11 || lengthMiddle<2)
    {
        PRINT_ERROR("In getTypeAfterKeyword: the input lengthMiddle variable ");
        fprintf(stderr,"(=%d) should be an integer comprised ",lengthMiddle);
        fprintf(stderr,"between two and eleven, corresponding to the length ");
        fprintf(stderr,"of the next part of the %d-th keyword read.\n",counter);
        return 0;
    }

    // Check keywordMiddle is a string of lengthMiddle char. (including '\0')
    if (!checkStringFromLength(keywordMiddle,lengthMiddle,lengthMiddle))
    {
        PRINT_ERROR("In getTypeAfterKeyword: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the input char[11] ");
        fprintf(stderr,"variable keywordMiddle is not a string made of ");
        fprintf(stderr,"%d characters, which were supposed to ",lengthMiddle-1);
        fprintf(stderr,"be the next ones read on the %d-th keyword.\n",counter);
        return 0;
    }

    // Distinguish the different cases depending on the content of keywordMiddle
    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    boolean1=(!strcmp(keywordMiddle,"t_mode"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"rbose"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"c"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_leng"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_electrons"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"hr_unit"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"b_ort"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lti_det"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"b_rhf"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"x"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"y"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"z"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_i"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_t"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ick_matrix"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"prox_mode"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"er_ini"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"er_max"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_type"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_mesh"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_data"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_prin"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_wher"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"th_lengt"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ode_l"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"mory"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"i"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_cfl"));

    boolean2=(!strcmp(keywordMiddle,"o_opt"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lect_orb"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lect_box"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"min"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"max"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lta_x"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lta_y"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lta_z"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_x"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_y"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_z"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_r"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_er"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_mi"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_ma"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"er_tol"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"in_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ax_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"in_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ax_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"in_ls"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ax_ls"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"in_la"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ax_la"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"usd_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"usd_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"usd_ls"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"usd_la"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"rad_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"rad_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"rad_ls"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"rad_la"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"sidual"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lta_t"));

    boolean3=(!strcmp(keywordMiddle,"me_resu"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"me_chem"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"me_mesh"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"me_elas"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_medit"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_mmg3d"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_mshdi"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_elast"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_advec"));

    if (!strcmp(keywordMiddle,"d_data"))
    {
        returnValue=-1;
    }
    else if (boolean1)
    {
        returnValue=1;
    }
    else if (boolean2)
    {
        returnValue=2;
    }
    else if (boolean3)
    {
        returnValue=3;
    }
    else
    {
        PRINT_ERROR("In getTypeAfterKeyword: the ");
        fprintf(stderr,"%d next letters read ",lengthMiddle-1);
        fprintf(stderr,"(=%s) after the first two ones of the ",keywordMiddle);
        fprintf(stderr,"%d-th keyword do not correspond to the ",counter);
        fprintf(stderr,"beginning of any known keyword for the structure ");
        fprintf(stderr,"Parameters.\n");
        returnValue=0;
    }

    return returnValue;
}

////////////////////////////////////////////////////////////////////////////////
// The function getLengthAfterKeywordMiddle, depending on the different cases
// for keywordMiddle (a string of length lengthMiddle including '\0') evaluates
// the number of characters (including '\0') to be read after keywordMiddle in
// order to complete the reading of the (counter)-th keyword. It has a char[11]
// keywordMiddle, and two int variables (lengthMiddle and counter) as input
// arguments and it returns 1, 2, 3, or 4, otherwise zero in case of error
////////////////////////////////////////////////////////////////////////////////
int getLengthAfterKeywordMiddle(char keywordMiddle[11], int lengthMiddle,
                                                                    int counter)
{
    int returnValue=0, boolean1=0, boolean2=0, boolean3=0, boolean4=0;

    // Check that counter is between 1 and 81 (total number of keywords)
    if (counter<1 || counter>81)
    {
        PRINT_ERROR("In getLengthAfterKeywordMiddle: the input variable ");
        fprintf(stderr,"counter, corresponding to the %d-th keyword ",counter);
        fprintf(stderr,"read, should be an integer between 1 and 81 (the ");
        fprintf(stderr,"total number of different possible keywords).\n");
        return 0;
    }

    // Check that lengthMiddle stores keywordMiddle's size (between 2 and 11)
    if (lengthMiddle>11 || lengthMiddle<2)
    {
        PRINT_ERROR("In getLengthAfterKeywordMiddle: the input lengthMiddle ");
        fprintf(stderr,"variable (=%d) should be an integer ",lengthMiddle);
        fprintf(stderr,"comprised between two and eleven, corresponding to ");
        fprintf(stderr,"the length of next part of the %d-th keyword ",counter);
        fprintf(stderr,"read.\n");
        return 0;
    }

    // Check keywordMiddle is a string of lengthMiddle char. (including '\0')
    if (!checkStringFromLength(keywordMiddle,lengthMiddle,lengthMiddle))
    {
        PRINT_ERROR("In getLengthAfterKeywordMiddle: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the input ");
        fprintf(stderr,"char[11] variable keywordMiddle is not a string ");
        fprintf(stderr,"made of %d characters, which were ",lengthMiddle-1);
        fprintf(stderr,"supposed to be the next ones read on the ");
        fprintf(stderr,"%d-th keyword.\n",counter);
        return 0;
    }

    // Distinguish the different cases depending on the content of keywordMiddle
    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    boolean1=(!strcmp(keywordMiddle,"t_mode"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"rbose"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"o_opt"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"d_data"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_chem"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_mesh"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_elas"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_electrons"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"hr_unit"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lect_orb"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lti_det"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"b_rhf"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lect_box"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"min"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"max"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"x"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"y"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"z"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lta_x"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lta_y"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lta_z"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_x"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_y"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_z"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ick_matrix"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"prox_mode"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"er_ini"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"er_max"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_type"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_mesh"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_data"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"th_medit"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"th_mmg3d"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"in_ls"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ax_ls"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"usd_ls"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"rad_ls"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"mory"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"sidual"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lta_t"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_cfl"));

    boolean2=(!strcmp(keywordMiddle,"_r"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_er"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_ma"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_mi"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ve_prin"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ve_wher"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"th_lengt"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"th_advec"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"in_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ax_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"in_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ax_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"in_la"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"ax_la"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"usd_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"usd_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"usd_la"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"rad_is"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"rad_me"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"rad_la"));

    boolean3=(!strcmp(keywordMiddle,"c"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"me_leng"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"me_resu"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"b_ort"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"_i"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_mshdi"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_elast"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"ode_l"));

    boolean4=(!strcmp(keywordMiddle,"_t"));
    boolean4=(boolean4 || !strcmp(keywordMiddle,"er_tol"));
    boolean4=(boolean4 || !strcmp(keywordMiddle,"i"));

    if (boolean4)
    {
        returnValue=4;
    }
    else if (boolean3)
    {
        returnValue=3;
    }
    else if (boolean2)
    {
        returnValue=2;
    }
    else if (boolean1)
    {
        returnValue=1;
    }
    else
    {
        PRINT_ERROR("In getLengthAfterKeywordMiddle: the ");
        fprintf(stderr,"%d next letters read ",lengthMiddle-1);
        fprintf(stderr,"(=%s) after the first two ones of the ",keywordMiddle);
        fprintf(stderr,"%d-th keyword do not correspond to the ",counter);
        fprintf(stderr,"beginning of any known keyword for the structure ");
        fprintf(stderr,"Parameters.\n");
        returnValue=0;
    }

    return returnValue;
}

////////////////////////////////////////////////////////////////////////////////
// The function detectRepetition simply adds one in the array repetition at the
// location corresponding to the (counter)-th keyword in *.input file read as
// the concatenation of the strings keywordBeginning+keywordMiddle(+keywordEnd)
// where keywordMiddle has size lengthMiddle and keywordEnd has size lengthEnd
// It has the int[81] repetition, three char[] variables (keywordBeginning[3],
// keywordMiddle[11], and keywordEnd[4]), and three int variables (lengthMiddle,
// lengthEnd, and counter) as input arguments and it returns one on success,
// otherwise zero is returned if an error occurred
////////////////////////////////////////////////////////////////////////////////
int detectRepetition(int repetition[81], char keywordBeginning[3],
                     char keywordMiddle[11], char keywordEnd[4],
                                   int lengthMiddle, int lengthEnd, int counter)
{
    // Check that counter is between 1 and 81 (total number of keywords)
    if (counter<1 || counter>81)
    {
        PRINT_ERROR("In detectRepetition: the input variable counter ");
        fprintf(stderr,"corresponding to the %d-th keyword read, ",counter);
        fprintf(stderr,"should be an integer between 1 and and 81 (the total ");
        fprintf(stderr,"number of different possible keywords).\n");
        return 0;
    }

    // Check that keywordBeginning is a string made of two characters
    if (!checkStringFromLength(keywordBeginning,3,3))
    {
        PRINT_ERROR("In detectRepetition: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the input char[3] variable ");
        fprintf(stderr,"keywordBeginning is not a string made of two ");
        fprintf(stderr,"characters, which were supposed to be the first two ");
        fprintf(stderr,"ones of the %d-th keyword read.\n",counter);
        return 0;
    }

    // Check that lengthMiddle stores keywordMiddle's size (between 2 and 11)
    if (lengthMiddle>11 || lengthMiddle<2)
    {
        PRINT_ERROR("In detectRepetition: the input lengthMiddle variable ");
        fprintf(stderr,"(=%d) should be an integer comprised ",lengthMiddle);
        fprintf(stderr,"between two and eleven, corresponding to the length ");
        fprintf(stderr,"of next part of the %d-th keyword read.\n",counter);
        return 0;
    }

    // Check keywordMiddle is a string of lengthMiddle char. (including '\0')
    if (!checkStringFromLength(keywordMiddle,lengthMiddle,lengthMiddle))
    {
        PRINT_ERROR("In detectRepetition: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the input char[11] ");
        fprintf(stderr,"variable keywordMiddle is not a string made of ");
        fprintf(stderr,"%d characters, which were supposed to ",lengthMiddle-1);
        fprintf(stderr,"be the next ones read on the %d-th keyword.\n",counter);
        return 0;
    }

    // Check that lengthEnd stores keywordEnd's size (between 1 and 4)
    if (lengthEnd<1 || lengthEnd>4)
    {
        PRINT_ERROR("In detectRepetition: the input lengthEnd variable ");
        fprintf(stderr,"(=%d) should be an integer comprised ",lengthEnd);
        fprintf(stderr,"between one and four, corresponding to the length ");
        fprintf(stderr,"of last part of the %d-th keyword read.\n",counter);
        return 0;
    }

    // Check keywordEnd is a string of lengthEnd characters (including '\0')
    if (lengthEnd>1)
    {
        if (!checkStringFromLength(keywordEnd,lengthEnd,lengthEnd))
        {
            PRINT_ERROR("In detectRepetition: checkStringFromLength function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, after having checked that the input ");
            fprintf(stderr,"char[4] variable keywordMEnd is not a string ");
            fprintf(stderr,"made of %d characters, which were ",lengthEnd-1);
            fprintf(stderr,"supposed to be the last ones read on the ");
            fprintf(stderr,"%d-th keyword.\n",counter);
            return 0;
        }
    }

    // Distinguishing the 81 different cases and excluding non-valid keywords
    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (!strcmp(keywordBeginning,"op") && !strcmp(keywordMiddle,"t_mode"))
    {
        repetition[0]++;
    }
    else if (!strcmp(keywordBeginning,"ve") && !strcmp(keywordMiddle,"rbose"))
    {
        repetition[1]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"c") &&
                                                       !strcmp(keywordEnd,"pu"))
    {
        repetition[2]++;
    }
    else if (!strcmp(keywordBeginning,"rh") && !strcmp(keywordMiddle,"o_opt"))
    {
        repetition[3]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_leng")
                                                    && !strcmp(keywordEnd,"th"))
    {
        repetition[4]++;
    }
    else if (!strcmp(keywordBeginning,"en") && !strcmp(keywordMiddle,"d_data"))
    {
        repetition[5]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_resu")
                                                    && !strcmp(keywordEnd,"lt"))
    {
        repetition[6]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_chem"))
    {
        repetition[7]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_mesh"))
    {
        repetition[8]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_elas"))
    {
        repetition[9]++;
    }
    else if (!strcmp(keywordBeginning,"nu") &&
                                            !strcmp(keywordMiddle,"_electrons"))
    {
        repetition[10]++;
    }
    else if (!strcmp(keywordBeginning,"bo") && !strcmp(keywordMiddle,"hr_unit"))
    {
        repetition[11]++;
    }
    else if (!strcmp(keywordBeginning,"se") &&
                                              !strcmp(keywordMiddle,"lect_orb"))
    {
        repetition[12]++;
    }
    else if (!strcmp(keywordBeginning,"or") && !strcmp(keywordMiddle,"b_ort")
                                                    && !strcmp(keywordEnd,"ho"))
    {
        repetition[13]++;
    }
    else if (!strcmp(keywordBeginning,"ne") &&
                                            !strcmp(keywordMiddle,"_electrons"))
    {
        repetition[14]++;
    }
    else if (!strcmp(keywordBeginning,"mu") && !strcmp(keywordMiddle,"lti_det"))
    {
        repetition[15]++;
    }
    else if (!strcmp(keywordBeginning,"or") && !strcmp(keywordMiddle,"b_rhf"))
    {
        repetition[16]++;
    }
    else if (!strcmp(keywordBeginning,"se") &&
                                              !strcmp(keywordMiddle,"lect_box"))
    {
        repetition[17]++;
    }
    else if (!strcmp(keywordBeginning,"x_") && !strcmp(keywordMiddle,"min"))
    {
        repetition[18]++;
    }
    else if (!strcmp(keywordBeginning,"x_") && !strcmp(keywordMiddle,"max"))
    {
        repetition[19]++;
    }
    else if (!strcmp(keywordBeginning,"y_") && !strcmp(keywordMiddle,"min"))
    {
        repetition[20]++;
    }
    else if (!strcmp(keywordBeginning,"y_") && !strcmp(keywordMiddle,"max"))
    {
        repetition[21]++;
    }
    else if (!strcmp(keywordBeginning,"z_") && !strcmp(keywordMiddle,"min"))
    {
        repetition[22]++;
    }
    else if (!strcmp(keywordBeginning,"z_") && !strcmp(keywordMiddle,"max"))
    {
        repetition[23]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"x"))
    {
        repetition[24]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"y"))
    {
        repetition[25]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"z"))
    {
        repetition[26]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_x"))
    {
        repetition[27]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_y"))
    {
        repetition[28]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_z"))
    {
        repetition[29]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_i") &&
                                                       !strcmp(keywordEnd,"ni"))
    {
        repetition[30]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_t") &&
                                                      !strcmp(keywordEnd,"ype"))
    {
        repetition[31]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_x"))
    {
        repetition[32]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_y"))
    {
        repetition[33]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_z"))
    {
        repetition[34]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r")
                                                     && !strcmp(keywordEnd,"x"))
    {
        repetition[35]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r")
                                                     && !strcmp(keywordEnd,"y"))
    {
        repetition[36]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r")
                                                     && !strcmp(keywordEnd,"z"))
    {
        repetition[37]++;
    }

    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_er")
                                                     && !strcmp(keywordEnd,"r"))
    {
        repetition[38]++;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_mi")
                                                     && !strcmp(keywordEnd,"n"))
    {
        repetition[39]++;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_ma")
                                                     && !strcmp(keywordEnd,"x"))
    {
        repetition[40]++;
    }
    else if (!strcmp(keywordBeginning,"tr") &&
                                            !strcmp(keywordMiddle,"ick_matrix"))
    {
        repetition[41]++;
    }
    else if (!strcmp(keywordBeginning,"ap") &&
                                             !strcmp(keywordMiddle,"prox_mode"))
    {
        repetition[42]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_ini"))
    {
        repetition[43]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_max"))
    {
        repetition[44]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d0p"))
    {
        repetition[45]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d1p"))
    {
        repetition[46]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d2p"))
    {
        repetition[47]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_type"))
    {
        repetition[48]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_mesh"))
    {
        repetition[49]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_data"))
    {
        repetition[50]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_prin")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[51]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_wher")
                                                     && !strcmp(keywordEnd,"e"))
    {
        repetition[52]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_lengt")
                                                     && !strcmp(keywordEnd,"h"))
    {
        repetition[53]++;
    }
    else if (!strcmp(keywordBeginning,"pa") &&
                                              !strcmp(keywordMiddle,"th_medit"))
    {
        repetition[54]++;
    }
    else if (!strcmp(keywordBeginning,"pa") &&
                                              !strcmp(keywordMiddle,"th_mmg3d"))
    {
        repetition[55]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_mshdi")
                                                    && !strcmp(keywordEnd,"st"))
    {
        repetition[56]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_elast")
                                                    && !strcmp(keywordEnd,"ic"))
    {
        repetition[57]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_advec")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[58]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_is") &&
                                                        !strcmp(keywordEnd,"o"))
    {
        repetition[59]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_is") &&
                                                        !strcmp(keywordEnd,"o"))
    {
        repetition[60]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_me") &&
                                                        !strcmp(keywordEnd,"t"))
    {
        repetition[61]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_me") &&
                                                        !strcmp(keywordEnd,"t"))
    {
        repetition[62]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_ls"))
    {
        repetition[63]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_ls"))
    {
        repetition[64]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ode_l") &&
                                                       !strcmp(keywordEnd,"ag"))
    {
        repetition[65]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_la") &&
                                                        !strcmp(keywordEnd,"g"))
    {
        repetition[66]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_la") &&
                                                        !strcmp(keywordEnd,"g"))
    {
        repetition[67]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_is")
                                                     && !strcmp(keywordEnd,"o"))
    {
        repetition[68]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_me")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[69]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_ls"))
    {
        repetition[70]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_la")
                                                     && !strcmp(keywordEnd,"g"))
    {
        repetition[71]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_is")
                                                     && !strcmp(keywordEnd,"o"))
    {
        repetition[72]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_me")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[73]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_ls"))
    {
        repetition[74]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_la")
                                                     && !strcmp(keywordEnd,"g"))
    {
        repetition[75]++;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"mory"))
    {
        repetition[76]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"i") &&
                                                      !strcmp(keywordEnd,"ter"))
    {
        repetition[77]++;
    }
    else if (!strcmp(keywordBeginning,"re") && !strcmp(keywordMiddle,"sidual"))
    {
        repetition[78]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_t"))
    {
        repetition[79]++;
    }
    else if (!strcmp(keywordBeginning,"no") && !strcmp(keywordMiddle,"_cfl"))
    {
        repetition[80]++;
    }
    else
    {
        PRINT_ERROR("In detectRepetition: the string ");
        fprintf(stderr,"(%s%s",keywordBeginning,keywordMiddle);
        if (lengthEnd>1)
        {
            fprintf(stderr,"%s",keywordEnd);
        }
        fprintf(stderr,") associated with the %d-th keyword read ",counter);
        fprintf(stderr,"does not correspond to any known keyword of the ");
        fprintf(stderr,"structure Parameters.\n");
        return 0;
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The function changeValuesOfParameters saves the value stored either in
// readIntegerIn, readDouble, or readStringIn in the structure pointed by
// pParameters, depending on the type of the (counter)-th keyword in the *.input
// file, read as a concatenation of keywordBeginning+keywordMiddle(+keywordEnd)
// where keywordMiddle has size lengthMiddle, and keywordEnd has size lengthEnd.
// It has the Parameters* variable (defined in main.h), 3 char[] variables
// (keywordbeginning[3], keywordMiddle[11], and keywordEnd[4]), a char*
// readStringIn, four int variables (lengthMiddle, lengthEnd, counter, and
// readIntegerIn), and a double readDouble variable as input arguments, and it
// returns one on success, otherwise zero is returned if an error is encountered
////////////////////////////////////////////////////////////////////////////////
int changeValuesOfParameters(Parameters* pParameters, char keywordBeginning[3],
                             char keywordMiddle[11], char keywordEnd[4],
                             char* readStringIn, int lengthMiddle,
                             int lengthEnd, int counter, int readIntegerIn,
                                                              double readDouble)
{
    size_t size=0;
    char *stringCopy=NULL;
    int boolean=0;

    // Test if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In changeValuesOfParameters: the (input) variable ");
        fprintf(stderr,"pParameters=%p does not point to ",(void*)pParameters);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check that pParameters stores correct strings for name_input and path_*
    boolean=checkStringFromLength(pParameters->name_input,8,
                                                      pParameters->name_length);
    boolean=(boolean && checkStringFromLength(pParameters->path_medit,2,
                                                     pParameters->path_length));
    boolean=(boolean && checkStringFromLength(pParameters->path_mmg3d,2,
                                                     pParameters->path_length));
    boolean=(boolean && checkStringFromLength(pParameters->path_mshdist,2,
                                                     pParameters->path_length));
    boolean=(boolean && checkStringFromLength(pParameters->path_elastic,2,
                                                     pParameters->path_length));
    boolean=(boolean && checkStringFromLength(pParameters->path_advect,2,
                                                     pParameters->path_length));
    if (!boolean)
    {
        PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having successively checked that ");
        fprintf(stderr,"at least one of the (char*) variables name_input, ");
        fprintf(stderr,"path_medit, path_mmg3d, path_mshdist, path_elastic, ");
        fprintf(stderr,"or path_advect of the structure pointed by the input ");
        fprintf(stderr,"pParameters variable is not a string with the ");
        fprintf(stderr,"correct length.\n");
        return 0;
    }

    // Check that counter is between 1 and 81 (total number of keywords)
    if (counter<1 || counter>81)
    {
        PRINT_ERROR("In changeValuesOfParameters: the input variable counter ");
        fprintf(stderr,"corresponding to the %d-th keyword read, ",counter);
        fprintf(stderr,"should be an integer between 1 and and 81 (the total ");
        fprintf(stderr,"number of different possible keywords).\n");
        return 0;
    }

    //  Check that keywordBeginning is a string made of two characters
    if (!checkStringFromLength(keywordBeginning,3,3))
    {
        PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the input  ");
        fprintf(stderr,"char[3] variable keywordBeginning is not a string ");
        fprintf(stderr,"made of two characters, which were supposed to be ");
        fprintf(stderr,"the first two ones of the %d-th keyword ",counter);
        fprintf(stderr,"read.\n");
        return 0;
    }

    // Check that lengthMiddle stores keywordMiddle's size (between 2 and 11)
    if (lengthMiddle>11 || lengthMiddle<2)
    {
        PRINT_ERROR("In changeValuesOfParameters: the input lengthMiddle ");
        fprintf(stderr,"variable (=%d) should be an integer ",lengthMiddle);
        fprintf(stderr,"comprised between two and eleven, corresponding to ");
        fprintf(stderr,"the length of next part of the ");
        fprintf(stderr,"%d-th keyword read.\n",counter);
        return 0;
    }

    // Check keywordMiddle is a string of lengthMiddle char. (including '\0')
    if (!checkStringFromLength(keywordMiddle,lengthMiddle,lengthMiddle))
    {
        PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the input ");
        fprintf(stderr,"char[11] variable keywordMiddle is not a string made ");
        fprintf(stderr,"of %d characters, which were supposed ",lengthMiddle-1);
        fprintf(stderr,"to be the next ones read on the ");
        fprintf(stderr,"%d-th keyword.\n",counter);
        return 0;
    }

    // Check that lengthEnd stores keywordEnd's size (between 1 and 4)
    if (lengthEnd<1 || lengthEnd>4)
    {
        PRINT_ERROR("In changeValuesOfParameters: the input lengthEnd ");
        fprintf(stderr,"variable (=%d) should be an integer ",lengthEnd);
        fprintf(stderr,"comprised between one and four, corresponding to the ");
        fprintf(stderr,"length of last part of the %d-th keyword ",counter);
        fprintf(stderr,"read.\n");
        return 0;
    }

    // Check keywordEnd is a string of lengthEnd characters (including '\0')
    if (lengthEnd>1)
    {
        if (!checkStringFromLength(keywordEnd,lengthEnd,lengthEnd))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input char[4] variable keywordMEnd is not a ");
            fprintf(stderr,"string made of %d characters, which ",lengthEnd-1);
            fprintf(stderr,"were supposed to be the last ones read on the ");
            fprintf(stderr,"%d-th keyword.\n",counter);
            return 0;
        }
    }

    // Change the values of pParameters depending on the 81 different cases
    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (!strcmp(keywordBeginning,"op") && !strcmp(keywordMiddle,"t_mode"))
    {
        pParameters->opt_mode=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"ve") && !strcmp(keywordMiddle,"rbose"))
    {
        pParameters->verbose=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"c") &&
                                                       !strcmp(keywordEnd,"pu"))
    {
        pParameters->n_cpu=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"rh") && !strcmp(keywordMiddle,"o_opt"))
    {
        pParameters->rho_opt=readDouble;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_leng")
                                                    && !strcmp(keywordEnd,"th"))
    {
        if (readIntegerIn<pParameters->name_length)
        {
            PRINT_ERROR("In changeValuesOfParameters: the value ");
            fprintf(stderr,"(=%d) of the %d-th keyword ",readIntegerIn,counter);
            fprintf(stderr,"read is the one associated with the name_length ");
            fprintf(stderr,"variable. This prescribed value cannot be ");
            fprintf(stderr,"(strictly) lower than the existing one ");
            fprintf(stderr,"(=%d). Please modify ",pParameters->name_length);
            fprintf(stderr,"the preprocessor constant ");
            fprintf(stderr,"(NAME_LENGTH=%d) accordingly in ",NAME_LENGTH);
            fprintf(stderr,"loadParameters.h file or change the value of ");
            fprintf(stderr,"name_length keyword for a sufficiently larger ");
            fprintf(stderr,"one in the *.input file.\n");
            return 0;
        }
        else if (readIntegerIn>pParameters->name_length)
        {
            size=readIntegerIn;

           // Reallocate memory for the name_input keyword: the realloc function
           // returns a pointer to allocated memory, otherwise NULL
            stringCopy=(char*)realloc(pParameters->name_input,
                                                             size*sizeof(char));
            if (stringCopy==NULL)
            {
                PRINT_ERROR("In changeValuesOfParameters: could not ");
                fprintf(stderr,"reallocate memory for the (char*) ");
                fprintf(stderr,"pParameters->name_input variable.\n");
                return 0;
            }
            pParameters->name_input=stringCopy;

            // Reallocate memory for the name_result keyword if not NULL
            if (pParameters->name_result!=NULL)
            {
                stringCopy=(char*)realloc(pParameters->name_result,
                                                             size*sizeof(char));
                if (stringCopy==NULL)
                {
                    PRINT_ERROR("In changeValuesOfParameters: could not ");
                    fprintf(stderr,"reallocate memory for the (char*) ");
                    fprintf(stderr,"pParameters->name_result variable.\n");
                    return 0;
                }
                pParameters->name_result=stringCopy;
            }

            // Reallocate memory for the name_chem keyword if not NULL
            if (pParameters->name_chem!=NULL)
            {
                stringCopy=(char*)realloc(pParameters->name_chem,
                                                             size*sizeof(char));
                if (stringCopy==NULL)
                {
                    PRINT_ERROR("In changeValuesOfParameters: could not ");
                    fprintf(stderr,"reallocate memory for the (char*) ");
                    fprintf(stderr,"pParameters->name_chem variable.\n");
                    return 0;
                }
                pParameters->name_chem=stringCopy;
            }

            // Reallocate memory for the name_mesh keyword if not NULL
            if (pParameters->name_mesh!=NULL)
            {
                stringCopy=(char*)realloc(pParameters->name_mesh,
                                                             size*sizeof(char));
                if (stringCopy==NULL)
                {
                    PRINT_ERROR("In changeValuesOfParameters: could not ");
                    fprintf(stderr,"reallocate memory for the (char*) ");
                    fprintf(stderr,"pParameters->name_mesh variable.\n");
                    return 0;
                }
                pParameters->name_mesh=stringCopy;
            }

            // Reallocate memory for the name_elas keyword if not NULL
            if (pParameters->name_elas!=NULL)
            {
                stringCopy=(char*)realloc(pParameters->name_elas,
                                                             size*sizeof(char));
                if (stringCopy==NULL)
                {
                    PRINT_ERROR("In changeValuesOfParameters: could not ");
                    fprintf(stderr,"reallocate memory for the (char*) ");
                    fprintf(stderr,"pParameters->name_elas variable.\n");
                    return 0;
                }
                pParameters->name_elas=stringCopy;
            }

            // We do not forget here to update the name_length variable
            pParameters->name_length=readIntegerIn;
        }
    }
    else if (!strcmp(keywordBeginning,"en") && !strcmp(keywordMiddle,"d_data"))
    {
        // Nothing to do for the end_data keyword
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_resu")
                                                    && !strcmp(keywordEnd,"lt"))
    {
        // Check if this is the first time pParameters->name_result is found
        if (pParameters->name_result!=NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: the (char*) variable ");
            fprintf(stderr,"name_result ");
            fprintf(stderr,"(=%p) of the ",(void*)pParameters->name_result);
            fprintf(stderr,"structure pointed by pParameters has not been ");
            fprintf(stderr,"correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>8)
        if (!checkStringFromLength(readStringIn,9,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as name_result, is not a string ");
            fprintf(stderr,"of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 7 in ",pParameters->name_length);
            fprintf(stderr,"order to store something more than the *.result ");
            fprintf(stderr,"extension).\n");
            return 0;
        }

        // Allocate memory for name_result: calloc function returns a pointer to
        // the allocated memory, otherwise NULL; strncpy function returns a
        // pointer to the string (not used here)
        size=pParameters->name_length;
        pParameters->name_result=(char*)calloc(size,sizeof(char));
        if (pParameters->name_result==NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: could not allocate ");
            fprintf(stderr,"memory for the (char*) pParameters->name_result ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        strncpy(pParameters->name_result,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_chem"))
    {
        // Check if this is the first time pParameters->name_chem is found
        if (pParameters->name_chem!=NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: the name_chem (char*) ");
            fprintf(stderr,"variable (=%p) of ",(void*)pParameters->name_chem);
            fprintf(stderr,"the structure pointed by pParameters has not ");
            fprintf(stderr,"been correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>7)
        if (!checkStringFromLength(readStringIn,7,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as name_chem, is not a string ");
            fprintf(stderr,"of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
            fprintf(stderr,"order to store something more than the *.chem ");
            fprintf(stderr,"or *.wfn extension).\n");
            return 0;
        }

        // Allocate memory for name_chem
        size=pParameters->name_length;
        pParameters->name_chem=(char*)calloc(size,sizeof(char));
        if (pParameters->name_chem==NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: could not allocate ");
            fprintf(stderr,"memory for the char* pParameters->name_chem ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        strncpy(pParameters->name_chem,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_mesh"))
    {
        // Check if this is the first time pParameters->name_mesh is found
        if (pParameters->name_mesh!=NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: the name_mesh (char*) ");
            fprintf(stderr,"variable (=%p) of ",(void*)pParameters->name_mesh);
            fprintf(stderr,"the structure pointed by pParameters has not ");
            fprintf(stderr,"been correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>7)
        if (!checkStringFromLength(readStringIn,7,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as name_mesh, is not a string ");
            fprintf(stderr,"of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
            fprintf(stderr,"order to store something more than the *.mesh ");
            fprintf(stderr,"or *.cube extension).\n");
            return 0;
        }

        // Allocate memory for name_mesh
        size=pParameters->name_length;
        pParameters->name_mesh=(char*)calloc(size,sizeof(char));
        if (pParameters->name_mesh==NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: could not allocate ");
            fprintf(stderr,"memory for the (char*) pParameters->name_mesh ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        strncpy(pParameters->name_mesh,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_elas"))
    {
        // Check if this is the first time pParameters->name_elas is found
        if (pParameters->name_elas!=NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: the name_elas (char*) ");
            fprintf(stderr,"variable (=%p) of ",(void*)pParameters->name_elas);
            fprintf(stderr,"the structure pointed by pParameters has not ");
            fprintf(stderr,"been correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>7)
        if (!checkStringFromLength(readStringIn,7,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as name_elas, is not a string ");
            fprintf(stderr,"of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
            fprintf(stderr,"order to store something more than the *.elas ");
            fprintf(stderr,"extension).\n");
            return 0;
        }

        // Allocate memory for name_elas
        size=pParameters->name_length;
        pParameters->name_elas=(char*)calloc(size,sizeof(char));
        if (pParameters->name_elas==NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: could not allocate ");
            fprintf(stderr,"memory for the (char*) pParameters->name_elas ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        strncpy(pParameters->name_elas,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"nu") &&
                                            !strcmp(keywordMiddle,"_electrons"))
    {
        pParameters->nu_electrons=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"bo") && !strcmp(keywordMiddle,"hr_unit"))
    {
        pParameters->bohr_unit=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"se") &&
                                              !strcmp(keywordMiddle,"lect_orb"))
    {
        pParameters->select_orb=readDouble;
    }
    else if (!strcmp(keywordBeginning,"or") && !strcmp(keywordMiddle,"b_ort")
                                                    && !strcmp(keywordEnd,"ho"))
    {
        pParameters->orb_ortho=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"ne") &&
                                            !strcmp(keywordMiddle,"_electrons"))
    {
        pParameters->ne_electrons=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"mu") && !strcmp(keywordMiddle,"lti_det"))
    {
        pParameters->multi_det=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"or") && !strcmp(keywordMiddle,"b_rhf"))
    {
        pParameters->orb_rhf=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"se") &&
                                              !strcmp(keywordMiddle,"lect_box"))
    {
        pParameters->select_box=readDouble;
    }
    else if (!strcmp(keywordBeginning,"x_") && !strcmp(keywordMiddle,"min"))
    {
        pParameters->x_min=readDouble;
    }
    else if (!strcmp(keywordBeginning,"x_") && !strcmp(keywordMiddle,"max"))
    {
        pParameters->x_max=readDouble;
    }
    else if (!strcmp(keywordBeginning,"y_") && !strcmp(keywordMiddle,"min"))
    {
        pParameters->y_min=readDouble;
    }
    else if (!strcmp(keywordBeginning,"y_") && !strcmp(keywordMiddle,"max"))
    {
        pParameters->y_max=readDouble;
    }
    else if (!strcmp(keywordBeginning,"z_") && !strcmp(keywordMiddle,"min"))
    {
        pParameters->z_min=readDouble;
    }
    else if (!strcmp(keywordBeginning,"z_") && !strcmp(keywordMiddle,"max"))
    {
        pParameters->z_max=readDouble;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"x"))
    {
        pParameters->n_x=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"y"))
    {
        pParameters->n_y=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"z"))
    {
        pParameters->n_z=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_x"))
    {
        pParameters->delta_x=readDouble;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_y"))
    {
        pParameters->delta_y=readDouble;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_z"))
    {
        pParameters->delta_z=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_i") &&
                                                       !strcmp(keywordEnd,"ni"))
    {
        pParameters->ls_ini=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_t") &&
                                                      !strcmp(keywordEnd,"ype"))
    {
        pParameters->ls_type=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_x"))
    {
        pParameters->ls_x=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_y"))
    {
        pParameters->ls_y=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_z"))
    {
        pParameters->ls_z=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r") &&
                                                        !strcmp(keywordEnd,"x"))
    {
        pParameters->ls_rx=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r") &&
                                                        !strcmp(keywordEnd,"y"))
    {
        pParameters->ls_ry=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r") &&
                                                        !strcmp(keywordEnd,"z"))
    {
        pParameters->ls_rz=readDouble;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_er") &&
                                                        !strcmp(keywordEnd,"r"))
    {
        pParameters->met_err=readDouble;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_mi") &&
                                                        !strcmp(keywordEnd,"n"))
    {
        pParameters->met_min=readDouble;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_ma") &&
                                                        !strcmp(keywordEnd,"x"))
    {
        pParameters->met_max=readDouble;
    }
    else if (!strcmp(keywordBeginning,"tr") &&
                                            !strcmp(keywordMiddle,"ick_matrix"))
    {
        pParameters->trick_matrix=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"ap") &&
                                             !strcmp(keywordMiddle,"prox_mode"))
    {
        pParameters->approx_mode=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_ini"))
    {
        pParameters->iter_ini=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_max"))
    {
        pParameters->iter_max=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d0p"))
    {
        pParameters->iter_told0p=readDouble;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d1p"))
    {
        pParameters->iter_told1p=readDouble;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d2p"))
    {
        pParameters->iter_told2p=readDouble;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_type"))
    {
        pParameters->save_type=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_mesh"))
    {
        pParameters->save_mesh=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_data"))
    {
        pParameters->save_data=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_prin")
                                                     && !strcmp(keywordEnd,"t"))
    {
        pParameters->save_print=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_wher")
                                                     && !strcmp(keywordEnd,"e"))
    {
        pParameters->save_where=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_lengt")
                                                     && !strcmp(keywordEnd,"h"))
    {
        if (readIntegerIn<pParameters->path_length)
        {
            PRINT_ERROR("In changeValuesOfParameters: the value ");
            fprintf(stderr,"(=%d) of the %d-th keyword ",readIntegerIn,counter);
            fprintf(stderr,"read is the one associated with the path_length ");
            fprintf(stderr,"variable. This prescribed value cannot be ");
            fprintf(stderr,"(strictly) lower than the existing one ");
            fprintf(stderr,"(=%d). Please modify ",pParameters->path_length);
            fprintf(stderr,"the preprocessor constant ");
            fprintf(stderr,"(PATH_LENGTH=%d) accordingly in ",PATH_LENGTH);
            fprintf(stderr,"loadParameters.h file or change the value of ");
            fprintf(stderr,"path_length keyword for a sufficiently larger ");
            fprintf(stderr,"one in the *.input file.\n");
            return 0;
        }
        else if (readIntegerIn>pParameters->path_length)
        {
            size=readIntegerIn;

           // Reallocate memory for the path_medit keyword
            stringCopy=(char*)realloc(pParameters->path_medit,
                                                             size*sizeof(char));
            if (stringCopy==NULL)
            {
                PRINT_ERROR("In changeValuesOfParameters: could not ");
                fprintf(stderr,"reallocate memory for the (char*) ");
                fprintf(stderr,"pParameters->path_medit variable.\n");
                return 0;
            }
            pParameters->path_medit=stringCopy;

           // Reallocate memory for the path_mmg3d keyword
            stringCopy=(char*)realloc(pParameters->path_mmg3d,
                                                             size*sizeof(char));
            if (stringCopy==NULL)
            {
                PRINT_ERROR("In changeValuesOfParameters: could not ");
                fprintf(stderr,"reallocate memory for the (char*) ");
                fprintf(stderr,"pParameters->path_mmg3d variable.\n");
                return 0;
            }
            pParameters->path_mmg3d=stringCopy;

           // Reallocate memory for the path_mshdist keyword
            stringCopy=(char*)realloc(pParameters->path_mshdist,
                                                             size*sizeof(char));
            if (stringCopy==NULL)
            {
                PRINT_ERROR("In changeValuesOfParameters: could not ");
                fprintf(stderr,"reallocate memory for the (char*) ");
                fprintf(stderr,"pParameters->path_mshdist variable.\n");
                return 0;
            }
            pParameters->path_mshdist=stringCopy;

           // Reallocate memory for the path_elastic keyword
            stringCopy=(char*)realloc(pParameters->path_elastic,
                                                             size*sizeof(char));
            if (stringCopy==NULL)
            {
                PRINT_ERROR("In changeValuesOfParameters: could not ");
                fprintf(stderr,"reallocate memory for the (char*) ");
                fprintf(stderr,"pParameters->path_elastic variable.\n");
                return 0;
            }
            pParameters->path_elastic=stringCopy;

           // Reallocate memory for the path_advect keyword
            stringCopy=(char*)realloc(pParameters->path_advect,
                                                             size*sizeof(char));
            if (stringCopy==NULL)
            {
                PRINT_ERROR("In changeValuesOfParameters: could not ");
                fprintf(stderr,"reallocate memory for the (char*) ");
                fprintf(stderr,"pParameters->path_advect variable.\n");
                return 0;
            }
            pParameters->path_advect=stringCopy;

            // We do not forget here to update the path_length variable
            pParameters->path_length=readIntegerIn;
        }
    }
    else if (!strcmp(keywordBeginning,"pa") &&
                                              !strcmp(keywordMiddle,"th_medit"))
    {
        // Check readStringIn has a size less than pParameters->path_length (>1)
        if (!checkStringFromLength(readStringIn,2,pParameters->path_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_medit, is not a ");
            fprintf(stderr,"string with the correct length.\n");
            return 0;
        }
        size=pParameters->path_length;
        strncpy(pParameters->path_medit,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"pa") &&
                                              !strcmp(keywordMiddle,"th_mmg3d"))
    {
        // Check readStringIn has a size less than pParameters->path_length (>1)
        if (!checkStringFromLength(readStringIn,2,pParameters->path_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_mmg3d, is not a ");
            fprintf(stderr,"string with the correct length.\n");
            return 0;
        }
        size=pParameters->path_length;
        strncpy(pParameters->path_mmg3d,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_mshdi")
                                                    && !strcmp(keywordEnd,"st"))
    {
        // Check readStringIn has a size less than pParameters->path_length (>1)
        if (!checkStringFromLength(readStringIn,2,pParameters->path_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_mshdist, is not a ");
            fprintf(stderr,"string with the correct length.\n");
            return 0;
        }
        size=pParameters->path_length;
        strncpy(pParameters->path_mshdist,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_elast")
                                                    && !strcmp(keywordEnd,"ic"))
    {
        // Check readStringIn has a size less than pParameters->path_length (>1)
        if (!checkStringFromLength(readStringIn,2,pParameters->path_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_elastic, is not a ");
            fprintf(stderr,"string with the correct length.\n");
            return 0;
        }
        size=pParameters->path_length;
        strncpy(pParameters->path_elastic,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_advec")
                                                     && !strcmp(keywordEnd,"t"))
    {
        // Check readStringIn has a size less than pParameters->path_length (>1)
        if (!checkStringFromLength(readStringIn,2,pParameters->path_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input (char*) variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_advect, is not a ");
            fprintf(stderr,"string with the correct length.\n");
            return 0;
        }
        size=pParameters->path_length;
        strncpy(pParameters->path_advect,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_is") &&
                                                        !strcmp(keywordEnd,"o"))
    {
        pParameters->hmin_iso=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_is") &&
                                                        !strcmp(keywordEnd,"o"))
    {
        pParameters->hmax_iso=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_me") &&
                                                        !strcmp(keywordEnd,"t"))
    {
        pParameters->hmin_met=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_me") &&
                                                        !strcmp(keywordEnd,"t"))
    {
        pParameters->hmax_met=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_ls"))
    {
        pParameters->hmin_ls=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_ls"))
    {
        pParameters->hmax_ls=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ode_l") &&
                                                       !strcmp(keywordEnd,"ag"))
    {
        pParameters->hmode_lag=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_la") &&
                                                        !strcmp(keywordEnd,"g"))
    {
        pParameters->hmin_lag=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_la") &&
                                                        !strcmp(keywordEnd,"g"))
    {
        pParameters->hmax_lag=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_is")
                                                     && !strcmp(keywordEnd,"o"))
    {
        pParameters->hausd_iso=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_me")
                                                     && !strcmp(keywordEnd,"t"))
    {
        pParameters->hausd_met=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_ls"))
    {
        pParameters->hausd_ls=readDouble;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_la")
                                                     && !strcmp(keywordEnd,"g"))
    {
        pParameters->hausd_lag=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_is")
                                                     && !strcmp(keywordEnd,"o"))
    {
        pParameters->hgrad_iso=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_me")
                                                     && !strcmp(keywordEnd,"t"))
    {
        pParameters->hgrad_met=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_ls"))
    {
        pParameters->hgrad_ls=readDouble;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_la")
                                                     && !strcmp(keywordEnd,"g"))
    {
        pParameters->hgrad_lag=readDouble;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"mory"))
    {
        pParameters->memory=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"i") &&
                                                      !strcmp(keywordEnd,"ter"))
    {
        pParameters->n_iter=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"re") && !strcmp(keywordMiddle,"sidual"))
    {
        pParameters->residual=readDouble;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_t"))
    {
        pParameters->delta_t=readDouble;
    }
    else if (!strcmp(keywordBeginning,"no") && !strcmp(keywordMiddle,"_cfl"))
    {
        pParameters->no_cfl=readIntegerIn;
    }
    else
    {
        PRINT_ERROR("In changeValuesOfParameters: the string ");
        fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
        if (lengthEnd>1)
        {
            fprintf(stderr,"%s",keywordEnd);
        }
        fprintf(stderr,") associated with the %d-th keyword read ",counter);
        fprintf(stderr,"does not correspond to any known keyword of the ");
        fprintf(stderr,"structure Parameters.\n");
        return 0;
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The function readInputFileAndGetParameters reads the *.input file whose name
// has already been stored in the name_input variable of the structure pointed
// by pParameters, and it adjusts the structure Parameters accordingly. It has
// the Parameters* variable (defined in main.h) as input argument and it
// returns zero if an error occurs, otherwise one is returned in case of success
////////////////////////////////////////////////////////////////////////////////
int readInputFileAndGetParameters(Parameters* pParameters)
{
    char keywordBeginning[3]={'\0'}, keywordMiddle[11]={'\0'};
    char keywordEnd[4]={'\0'}, *readStringOut=NULL, *readStringIn=NULL;
    size_t lengthString=0;
    int lengthMiddle=0, lengthEnd=0, readIntegerIn=0, readIntegerOut=0;
    int readChar=0, keywordType=0, repetition[81]={0}, i=0;
    int counter=0, boolean=0;
    double readDouble=.0;
    FILE *infoFile=NULL;

    // Check if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In readInputFileAndGetParameters: the input variable ");
        fprintf(stderr,"pParameters=%p does not point to ",(void*)pParameters);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check the pParameters->name_length and pParameters->name_input variables
    if (!checkInputFileName(pParameters->name_input,pParameters->name_length))
    {
        PRINT_ERROR("In readInputFileAndGetParameters: checkInputFileName ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

    // Check if the *.input file have been previously created
    boolean=initialFileExists(pParameters->name_input,pParameters->name_length);
    if (abs(boolean)!=1)
    {
        PRINT_ERROR("In readInputFileAndGetParameters: initialFileExists ");
        fprintf(stderr,"function returned zero instead of (+/-) one, while ");
        fprintf(stderr,"attempting to check if the pParameters->name_input ");
        fprintf(stderr,"variable stores the name of an existing file.\n");
        return 0;
    }
    else if (boolean==-1)
    {
        PRINT_ERROR("In readInputFileAndGetParameters: the file ");
        fprintf(stderr,"%s could not be found at the ",pParameters->name_input);
        fprintf(stderr,"given location.\n");
        return 0;
    }

    // Check if the path_length variable
    if (pParameters->path_length<2)
    {
        PRINT_ERROR("In readInputFileAndGetParameters: the input pParameters ");
        fprintf(stderr,"structure does not have its path_length ");
        fprintf(stderr,"(=%d) variable strictly  ",pParameters->path_length);
        fprintf(stderr,"greater than one.\n");
        return 0;
    }

    // Initialize the dynamic allocation of memory for the readStringIn variable
    // calloc returns a pointer to the allocated memory, otherwise NULL
    lengthString=DEF_MAX(pParameters->name_length,pParameters->path_length);
    readStringIn=(char*)calloc(lengthString,sizeof(char));
    if (readStringIn==NULL)
    {
        PRINT_ERROR("In readInputFileAndGetParameters: could not allocate ");
        fprintf(stderr,"memory for the local (char*) variable readStringIn.\n");
        return 0;
    }

    // Check if the *.input file is opened (it must have been created before)
    // fopen function returns a FILE pointer on success, otherwise NULL
    fprintf(stdout,"\nOpening %s file. ",pParameters->name_input);
    infoFile=fopen(pParameters->name_input,"r");
    if (infoFile==NULL)
    {
        PRINT_ERROR("In readInputFileAndGetParameters: we were not able to ");
        fprintf(stderr,"open properly the %s file.\n",pParameters->name_input);

        // free function does not return value (void output)
        free(readStringIn);
        readStringIn=NULL;
        return 0;
    }
    fprintf(stdout,"Reading parameters. ");

    readIntegerOut=fscanf(infoFile," ");
    if (readIntegerOut)
    {
        PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function (EOF=%d) while attempting to read the ",EOF);
        fprintf(stderr,"first white space characters (in case there was ");
        fprintf(stderr,"some) of the %s file.\n",pParameters->name_input);
        closeTheFile(&infoFile,1);
        free(readStringIn);
        readStringIn=NULL;
        return 0;
    }

    counter=0;
    while (counter<81)
    {
        counter++;

        // Get the first 2 characters to distinguish among different keywords
        // fgets function returns NULL if it fails, otherwise the same pointer
        readStringOut=fgets(keywordBeginning,3,infoFile);
        if (readStringOut==NULL)
        {
            PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
            fprintf(stderr,"=%p of the standard fgets ",(void*)readStringOut);
            fprintf(stderr,"c-function in the attempt of reading the first ");
            fprintf(stderr,"two letters of the %d-th keyword.\n",counter);
            closeTheFile(&infoFile,1);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }

        // Treat the case where 'e' in end_data has already been read as double
        // strcmp returns 0 if the two strings are ==, otherwise <0 (resp. >0)
        // if the 1st string argument is shorter (resp. longer) than the 2nd one
        if (!strcmp(keywordBeginning,"nd"))
        {
            // fseek function returns zero on success otherwise a non-zero value
            if (fseek(infoFile,-3,SEEK_CUR))
            {
                PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
                fprintf(stderr,"of the standard fseek c-function in the ");
                fprintf(stderr,"attempt of reading the beginning of what ");
                fprintf(stderr,"may be 'end_data' for the %d-th ",counter);
                fprintf(stderr,"keyword (we only have read ");
                fprintf(stderr,"*%s*).\n",keywordBeginning);
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }

            // fgetc returns the unsigned char read cast to an int or EOF
            boolean=(fgetc(infoFile)=='e' && fgetc(infoFile)=='n');
            boolean=(boolean && fgetc(infoFile)=='d' && fgetc(infoFile)=='_');
            boolean=(boolean && fgetc(infoFile)=='d' && fgetc(infoFile)=='a');
            boolean=(boolean && fgetc(infoFile)=='t' && fgetc(infoFile)=='a');
            if (!boolean)
            {
                PRINT_ERROR("In readInputFileAndGetParameters: the first two ");
                fprintf(stderr,"letters read (=%s) for the ",keywordBeginning);
                fprintf(stderr,"%d-th keyword do not correspond to ",counter);
                fprintf(stderr,"the beginning of any known keyword of the ");
                fprintf(stderr,"structure Parameters (or wrong return of the ");
                fprintf(stderr,"standard fgetc c-function in the attempt of ");
                fprintf(stderr,"reading the 'end_data' keyword).\n");
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
            if (fseek(infoFile,-6,SEEK_CUR))
            {
                PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
                fprintf(stderr,"of the standard fseek c-function in the ");
                fprintf(stderr,"attempt of reading 'end_data' for the ");
                fprintf(stderr,"%d-th keyword.\n",counter);
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
            strncpy(keywordBeginning,"en",3);
        }

        // Skip the commented lines (those beginning with ##)
        while (!strcmp(keywordBeginning,"##"))
        {
            do
            {
                readChar=fgetc(infoFile);
            } while (readChar!='\n' && readChar!=EOF);

            if (readChar==EOF)
            {
                PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
                fprintf(stderr,"of the standard fgetc c-function or ");
                fprintf(stderr,"end-of-file (EOF=%d) reached without the ",EOF);
                fprintf(stderr,"end_data keyword written and read in the ");
                fprintf(stderr,"%s file.\n",pParameters->name_input);
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }

            readIntegerOut=fscanf(infoFile," ");
            if (readIntegerOut)
            {
                PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
                fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
                fprintf(stderr,"c-function (EOF=%d) while attempting ",EOF);
                fprintf(stderr,"to read the white space characters (in case ");
                fprintf(stderr,"there was some) after the string ");
                fprintf(stderr,"(=%s) associated with the ",readStringIn);
                fprintf(stderr,"%d-th keyword ",counter);
                fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
                if (lengthEnd>1)
                {
                    fprintf(stderr,"%s",keywordEnd);
                }
                fprintf(stderr,").\n");
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }

            readStringOut=fgets(keywordBeginning,3,infoFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
                fprintf(stderr,"=%p of the standard ",(void*)readStringOut);
                fprintf(stderr,"fgets c-function in the attempt of reading ");
                fprintf(stderr,"the first two letters of the %d-th ",counter);
                fprintf(stderr,"keyword.\n");
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
        }

        // Get length of the next keyword part to read (stored in lengthMiddle)
        boolean=getLengthAfterKeywordBeginning(keywordBeginning,counter);
        if (!boolean)
        {
            PRINT_ERROR("In readInputFileAndGetParameters: ");
            fprintf(stderr,"getLengthAfterKeywordBeginning function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here.\n");
            closeTheFile(&infoFile,1);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        lengthMiddle=boolean;

        // Reading the middle part of the keyword
        readStringOut=fgets(keywordMiddle,lengthMiddle,infoFile);
        if (readStringOut==NULL)
        {
            PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
            fprintf(stderr,"=%p of the standard fgets ",(void*)readStringOut);
            fprintf(stderr,"c-function in the attempt of reading the next ");
            fprintf(stderr,"part %s* of the %d-th ",keywordBeginning,counter);
            fprintf(stderr,"keyword.\n");
            closeTheFile(&infoFile,1);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }

        // Type of value to read after (1=int, 2=double, 3=string, -1=end_data)
        boolean=getTypeAfterKeyword(keywordMiddle,lengthMiddle,counter);
        if (!boolean)
        {
            PRINT_ERROR("In readInputFileAndGetParameters: ");
            fprintf(stderr,"getTypeAfterKeyword function returned zero, ");
            fprintf(stderr,"which is not the expected value here.\n");
            closeTheFile(&infoFile,1);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        keywordType=boolean;

        // Get the string length (including '\0') to end the keyword's reading
        boolean=getLengthAfterKeywordMiddle(keywordMiddle,lengthMiddle,counter);
        if (!boolean)
        {
            PRINT_ERROR("In readInputFileAndGetParameters: ");
            fprintf(stderr,"getLengthAfterkeywordMiddle function returned ");
            fprintf(stderr,"zero, which is not the expected value here.\n");
            closeTheFile(&infoFile,1);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        lengthEnd=boolean;

        // Read the next character if necessary
        if (lengthEnd>1)
        {
            readStringOut=fgets(keywordEnd,lengthEnd,infoFile);
            for (i=lengthEnd; i<4; i++)
            {
                keywordEnd[i]='\0';
            }
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readInputFileAndGetParameters: wrong return ");
                fprintf(stderr,"=%p of the standard ",(void*)readStringOut);
                fprintf(stderr,"fgets c-function in the attempt of reading ");
                fprintf(stderr,"the final part ");
                fprintf(stderr,"%s%s* of the ",keywordBeginning,keywordMiddle);
                fprintf(stderr,"%d-th keyword.\n",counter);
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
        }

        // Set up the detection of an already read keyword
        if (!detectRepetition(repetition,keywordBeginning,keywordMiddle,
                                     keywordEnd,lengthMiddle,lengthEnd,counter))
        {
            PRINT_ERROR("In readInputFileAndGetParameters: detectRepetition ");
            fprintf(stderr,"function returned zero instead of one.\n");
            closeTheFile(&infoFile,1);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        for (i=0; i<81; i++)
        {
            if (repetition[i]>1)
            {
                PRINT_ERROR("In readInputFileAndGetParameters: the ");
                fprintf(stderr,"%d-th keyword (=%s",counter,keywordBeginning);
                fprintf(stderr,"%s",keywordMiddle);
                if (lengthEnd>1)
                {
                    fprintf(stderr,"%s",keywordEnd);
                }
                fprintf(stderr,") has already been read before in the upper ");
                fprintf(stderr,"part of the %s file. ",pParameters->name_input);
                fprintf(stderr,"Please avoid repetitions by modifying the ");
                fprintf(stderr,"file accordingly.\n");
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
        }

        // Read the value appearing after the keyword (depending on its type)
        switch (keywordType)
        {
            case -1:
                counter=81;
                break;

            case 1:
                readIntegerOut=fscanf(infoFile," %d ",&readIntegerIn);
                if (readIntegerOut!=1)
                {
                    PRINT_ERROR("In readInputFileAndGetParameters: wrong ");
                    fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                    fprintf(stderr,"standard fscanf c-function in the ");
                    fprintf(stderr,"attempt of reading the value associated ");
                    fprintf(stderr,"with the %d-th keyword ",counter);
                    fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
                    if (lengthEnd>1)
                    {
                        fprintf(stderr,"%s",keywordEnd);
                    }
                    fprintf(stderr,", an integer value is expected here).\n");
                    closeTheFile(&infoFile,1);
                    free(readStringIn);
                    readStringIn=NULL;
                    return 0;
                }
                break;

            case 2:
                readIntegerOut=fscanf(infoFile," %lf ",&readDouble);
                if (readIntegerOut!=1)
                {
                    PRINT_ERROR("In readInputFileAndGetParameters: wrong ");
                    fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                    fprintf(stderr,"standard fscanf c-function in the ");
                    fprintf(stderr,"attempt of reading the value associated ");
                    fprintf(stderr,"with the %d-th keyword ",counter);
                    fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
                    if (lengthEnd>1)
                    {
                        fprintf(stderr,"%s",keywordEnd);
                    }
                    fprintf(stderr,", a double value is expected here).\n");
                    closeTheFile(&infoFile,1);
                    free(readStringIn);
                    readStringIn=NULL;
                    return 0;
                }
                break;

            case 3:
                // Check if lengthString is still the max of *_length variables
                boolean=DEF_MAX(pParameters->name_length,
                                                      pParameters->path_length);
                if ((int)lengthString<boolean)
                {
                   // realloc function returns a pointer to the allocated
                   // memory, otherwise NULL
                   lengthString=boolean;
                   readStringOut=(char*)realloc(readStringIn,
                                                     lengthString*sizeof(char));
                   if (readStringOut==NULL)
                    {
                        PRINT_ERROR("In readInputFileAndGetParameters: could ");
                        fprintf(stderr,"not reallocate memory for the ");
                        fprintf(stderr,"local (char*) readStringIn ");
                        fprintf(stderr,"variable.\n");
                        closeTheFile(&infoFile,1);
                        free(readStringIn);
                        readStringIn=NULL;
                        return 0;
                    }
                    readStringIn=readStringOut;
                }

                // Start reading the name of the path, character by character
                readIntegerOut=fscanf(infoFile," ");
                if (readIntegerOut)
                {
                    PRINT_ERROR("In readInputFileAndGetParameters: wrong ");
                    fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                    fprintf(stderr,"standard fscanf c-function (EOF=%d) ",EOF);
                    fprintf(stderr,"while attempting to read the white space ");
                    fprintf(stderr,"characters (in case there was some) ");
                    fprintf(stderr,"after the %d-th keyword ",counter);
                    fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
                    if (lengthEnd>1)
                    {
                        fprintf(stderr,"%s",keywordEnd);
                    }
                    fprintf(stderr,").\n");
                    closeTheFile(&infoFile,1);
                    free(readStringIn);
                    readStringIn=NULL;
                    return 0;
                }

                i=0;
                do {
                    readChar=fgetc(infoFile);
                    if (readChar==EOF)
                    {
                        PRINT_ERROR("In readInputFileAndGetParameters: ");
                        fprintf(stderr,"end-of-file (EOF=%d) reached ",EOF);
                        fprintf(stderr,"without the end_data keyword ");
                        fprintf(stderr,"encountered or wrong return ");
                        fprintf(stderr,"(=%d) of the standard fgetc ",readChar);
                        fprintf(stderr,"c-function in the attempt of reading ");
                        fprintf(stderr,"the %d-th letter of the string ",i+1);
                        fprintf(stderr,"associated with the %d-th ",counter);
                        fprintf(stderr,"keyword (=%s",keywordBeginning);
                        fprintf(stderr,"%s",keywordMiddle);
                        if (lengthEnd>1)
                        {
                            fprintf(stderr,"%s",keywordEnd);
                        }
                        fprintf(stderr,").\n");
                        closeTheFile(&infoFile,1);
                        free(readStringIn);
                        readStringIn=NULL;
                        return 0;
                    }
                    else
                    {
                        readStringIn[i]=(char)readChar;
                        i++;
                    }
                } while (readChar!=' ' && readChar!='\n' &&
                                                           i<(int)lengthString);

                if (readChar!=' ' && readChar!='\n' && i==(int)lengthString)
                {
                   PRINT_ERROR("In readInputFileAndGetParameters: it seems ");
                   fprintf(stderr,"that the name pointed by one of the ");
                   fprintf(stderr,"name_* or path_* variables associated ");
                   fprintf(stderr,"with the %d-th keyword has more ",counter);
                   fprintf(stderr,"than %d letters, which is the ",i-1);
                   fprintf(stderr,"maximum size allowed here.\n");
                   closeTheFile(&infoFile,1);
                   free(readStringIn);
                   readStringIn=NULL;
                   return 0;
                }
                readStringIn[i-1]='\0';
                while (i<(int)lengthString)
                {
                    readStringIn[i]='\0';
                    i++;
                }

                readIntegerOut=fscanf(infoFile," ");
                if (readIntegerOut)
                {
                    PRINT_ERROR("In readInputFileAndGetParameters: wrong ");
                    fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                    fprintf(stderr,"standard fscanf c-function (EOF=%d) ",EOF);
                    fprintf(stderr,"while attempting to read the white space ");
                    fprintf(stderr,"characters (in case there was some) ");
                    fprintf(stderr,"after the string (=%s) ",readStringIn);
                    fprintf(stderr,"associated with the %d-th ",counter);
                    fprintf(stderr,"keyword ");
                    fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
                    if (lengthEnd>1)
                    {
                        fprintf(stderr,"%s",keywordEnd);
                    }
                    fprintf(stderr,").\n");
                    closeTheFile(&infoFile,1);
                    free(readStringIn);
                    readStringIn=NULL;
                    return 0;
                }
                break;

            default:
                PRINT_ERROR("In readInputFileAndGetParameters: the local ");
                fprintf(stderr,"variable keywordType (=%d) ",keywordType);
                fprintf(stderr,"cannot only store the -1, 1, 2 or 3 values.\n");
                closeTheFile(&infoFile,1);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
                break;
        }

        // Saving the value in the pParameters structure
        if (!changeValuesOfParameters(pParameters,keywordBeginning,
                                      keywordMiddle,keywordEnd,readStringIn,
                                      lengthMiddle,lengthEnd,counter,
                                                      readIntegerIn,readDouble))
        {
            PRINT_ERROR("In readInputFileAndGetParameters: ");
            fprintf(stderr,"changeValuesOfParameters function returned zero ");
            fprintf(stderr,"instead of one.\n");
            closeTheFile(&infoFile,1);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
    } // This is the end of the while loop for keyword

    // Free the memory allocated for readStringIn
    free(readStringIn);
    readStringIn=NULL;

    // Closing the *.input file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(infoFile))
    {
        PRINT_ERROR("In readInputFileAndGetParameters: the ");
        fprintf(stderr,"%s file has not been closed ",pParameters->name_input);
        fprintf(stderr,"properly.\n");
        infoFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    infoFile=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The function writingDefaultElasticFile writes a default *.elas file in the
// case no file has been specified in the *.input file (using the name_elas
// keyword) i.e. if pParameters->name_elas is set to NULL. It uses the Lame
// coefficients given in the corresponding LAME_* preprocessor constants
// (defined in loadParameters.h). It has the Parameters* (defined in main.h)
// variable as input arguments and it returns one if the default *.elas file
// has been successfully written (and the pParameters->name_elas variable
// successfully updated) otherwise zero is returned if an error is encountered
////////////////////////////////////////////////////////////////////////////////
int writingDefaultElasticFile(Parameters* pParameters)
{
    size_t lengthName=0;
    FILE *elasticFile=NULL;

    // Testing if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In writingDefaultElasticFile: the input variable ");
        fprintf(stderr,"pParameters=%p does not point to ",(void*)pParameters);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check the pParameters->name_input variable
    if (!checkInputFileName(pParameters->name_input,pParameters->name_length))
    {
        PRINT_ERROR("In writingDefaultElasticFile: checkInputFileName ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

    // Check that we are in the situation where a default *.elas file is needed
    if (pParameters->name_elas!=NULL)
    {
        PRINT_ERROR("In writingDefaultElasticFile: we cannot generate a ");
        fprintf(stderr,"default *.elas file because one has already been ");
        fprintf(stderr,"specified in the %s file.\n",pParameters->name_input);
        return 0;
    }

    // calloc returns a pointer to the allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    pParameters->name_elas=(char*)calloc(lengthName,sizeof(char));
    if (pParameters->name_elas==NULL)
    {
        PRINT_ERROR("In writingDefaultElasticFile: could not allocate memory ");
        fprintf(stderr,"for the (char*) pParameters->name_elas variable.\n");
        return 0;
    }

    // strncpy function returns a pointer to the string (not used here)
    // strlen function returns the length of the string (not including '\0')
    strncpy(pParameters->name_elas,pParameters->name_input,lengthName);
    lengthName=strlen(pParameters->name_input);
    pParameters->name_elas[lengthName-6]='.';
    pParameters->name_elas[lengthName-5]='e';
    pParameters->name_elas[lengthName-4]='l';
    pParameters->name_elas[lengthName-3]='a';
    pParameters->name_elas[lengthName-2]='s';
    pParameters->name_elas[lengthName-1]='\0';

    // Check if the *.elas file is opened (warning: overwrite if it exists)
    // fopen function returns a FILE pointer on success, otherwise NULL
    if (pParameters->verbose>0)
    {
        fprintf(stdout,"\nOpening %s file. ",pParameters->name_elas);
    }
    elasticFile=fopen(pParameters->name_elas,"w+");
    if (elasticFile==NULL)
    {
        PRINT_ERROR("In writingDefaultElasticFile: we were not able to open ");
        fprintf(stderr,"and write into the %s file.\n",pParameters->name_elas);
        return 0;
    }
    if (pParameters->verbose>0)
    {
        fprintf(stdout,"Writing elastic parameters. ");
    }

    fprintf(elasticFile,"Dirichlet\n1\n10 Triangles f \n\nLame\n2\n");
    fprintf(elasticFile,"2 %lf %lf \n",LAME_EXT1,LAME_EXT2);
    fprintf(elasticFile,"3 %lf %lf \n",LAME_INT1,LAME_INT2);

    // Closing the *.elas file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(elasticFile))
    {
        PRINT_ERROR("In writingDefaultElasticFile: the ");
        fprintf(stderr,"%s file has not been closed ",pParameters->name_elas);
        fprintf(stderr,"properly.\n");
        elasticFile=NULL;
        return 0;
    }
    if (pParameters->verbose>0)
    {
        fprintf(stdout,"Closing file.\n");
    }
    elasticFile=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The function checkValuesOfAllParameters tests if the values of the structure
// pointed by pParameters that have been loaded are valid. It has the
// Parameters* variable (defined in main.h) as input argument and it returns one
// on success, otherwise zero is returned for an invalid value or an error
////////////////////////////////////////////////////////////////////////////////
int checkValuesOfAllParameters(Parameters* pParameters)
{
    size_t lengthName=0;
    char* fileLocation=NULL;
    int boolean=0, i=0, iMax=0;
    double dx=0., dy=0., dz=0.;

    // Testing if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the input variable ");
        fprintf(stderr,"pParameters=%p does not point to ",(void*)pParameters);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check the pParameters->name_length variable
    boolean=(pParameters->name_length>=DEF_MAX(8,NAME_LENGTH));
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the variable name_length ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->name_length);
        fprintf(stderr,"pointed by pParameters must be an integer ");
        fprintf(stderr,"(strictly) greater than seven (to store at least ");
        fprintf(stderr,"something more than the *.input extension) and which ");
        fprintf(stderr,"is at least equal or greater than its initial ");
        fprintf(stderr,"default value (=%d).\n",NAME_LENGTH);
        return 0;
    }

    // Check the pParameters->name_input variable
    if (!checkInputFileName(pParameters->name_input,pParameters->name_length))
    {
        PRINT_ERROR("In checkValuesOfAllParameters: checkInputFileName ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }
    if ((pParameters->name_input[0]=='.' || pParameters->name_input[1]=='~') &&
                                                pParameters->name_input[1]=='/')
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the name_input variable ");
        fprintf(stderr,"of the structure pointed by pParamters cannot begin ");
        fprintf(stderr,"with the './' prefix or the '~/' one.\n");
        return 0;
    }

    // Check pParameters->verbose
    boolean=(pParameters->verbose==0 || pParameters->verbose==1);
    boolean=(boolean || pParameters->verbose==2);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the verbose variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->verbose);
        fprintf(stderr,"by pParameters can only be set to 0, 1 or 2.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'verbose' ");
        fprintf(stderr,"keyword in the %s file.\n",pParameters->name_input);
        return 0;
    }
    if (pParameters->verbose>1)
    {
        fprintf(stdout,"Checking validity of all values in the structure ");
        fprintf(stdout,"Parameters.\n");
    }

    // Check pParameters->opt_mode
    boolean=(pParameters->opt_mode>-3 && pParameters->opt_mode<5);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the opt_mode variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->opt_mode);
        fprintf(stderr,"by pParameters can only be set to -2, -1, 0, 1, 2, ");
        fprintf(stderr,"3, or 4.\nPlease modify the value accordingly after ");
        fprintf(stderr,"the 'opt_mode' keyword in %s ",pParameters->name_input);
        fprintf(stderr,"file.\n");
        return 0;
    }

    // Check pParameters->n_cpu
#ifdef _OPENMP
    boolean=(pParameters->n_cpu>0);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the n_cpu variable ");
        fprintf(stderr,"(=%d) of the structure pointed by ",pParameters->n_cpu);
        fprintf(stderr,"pParameters must be a positive integer.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'n_cpu' ");
        fprintf(stderr,"keyword in the %s file.\n",pParameters->name_input);
        return 0;
    }

    boolean=(pParameters->n_cpu<=omp_get_max_threads());
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the n_cpu variable ");
        fprintf(stderr,"(=%d) of the structure pointed by ",pParameters->n_cpu);
        fprintf(stderr,"pParameters cannot be greater than the total ");
        fprintf(stderr,"number of threads (=%d) ",omp_get_max_threads());
        fprintf(stderr,"currently available on your computer.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'n_cpu' ");
        fprintf(stderr,"keyword in the %s file.\n",pParameters->name_input);
        return 0;
    }
#else
    boolean=(pParameters->n_cpu==N_CPU);
    if (!boolean)
    {
        if (pParameters->verbose>0)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters: the ");
            fprintf(stdout,"n_cpu variable (=%d) of the ",pParameters->n_cpu);
            fprintf(stdout,"structure pointed by pParameters has been ");
            fprintf(stdout,"changed from its default value (=%d) in ",N_CPU);
            fprintf(stdout,"the %s file, althought ",pParameters->name_input);
            fprintf(stdout,"the current version of the MPD program has not ");
            fprintf(stdout,"been compiled with the OpenMP library. Hence, no ");
            fprintf(stdout,"parallel calculus can be performed with the ");
            fprintf(stdout,"current version of the MPD program.\nThe ");
            fprintf(stdout,"pParameters->n_cpu variable is thus restored to ");
            fprintf(stdout,"its default value.\n\n");
        }
        pParameters->n_cpu=N_CPU;
    }
#endif

    // Check pParameters->rho_opt
    if (pParameters->opt_mode>0)
    {
        boolean=(pParameters->rho_opt>0.);
        if (!boolean && pParameters->verbose>1)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: the initial scaling factor ");
            fprintf(stdout,"pParameters->rho_opt (=%lf) ",pParameters->rho_opt);
            fprintf(stdout,"associated with the shape gradient does not have ");
            fprintf(stdout,"a positive (double) value.\n\n");
        }
    }
    else
    {
        boolean=(pParameters->rho_opt==RHO_OPT);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: the rho_opt variable ");
                fprintf(stdout,"(=%lf) of the structure ",pParameters->rho_opt);
                fprintf(stdout,"pointed by pParameters has been changed from ");
                fprintf(stdout,"its default value (=%lf) in the ",RHO_OPT);
                fprintf(stdout,"%s file, althought ",pParameters->name_input);
                fprintf(stdout,"the current the current optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such a parameter.\nThe pParameters->rho_opt ");
                fprintf(stdout,"variable is thus restored to its default ");
                fprintf(stdout,"value.\n\n");
            }
            pParameters->rho_opt=RHO_OPT;
        }
    }

    // Check pParameters->name_result
    if (pParameters->name_result!=NULL)
    {
        // Check for '~/' at the beginning of pParameters->name_result
        boolean=checkForTildeAndReplaceByHomePath(&pParameters->name_result,
                                                      pParameters->name_length);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: ");
            fprintf(stderr,"checkForTildeAndReplaceByHomePath function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, while attempting to replace the first '~' ");
            fprintf(stderr,"character by the full home directory path name ");
            fprintf(stderr,"in the pParameters->name_result variable.\n");
            fprintf(stderr,"Please check in the %s ",pParameters->name_input);
            fprintf(stderr,"file if a valid file name has been specified ");
            fprintf(stderr,"after the 'name_result' keyword (a non-empty ");
            fprintf(stderr,"string of length strictly less than ");
            fprintf(stderr,"%d), or extend properly ",pParameters->name_length);
            fprintf(stderr,"the allowed length for name_* file names thanks ");
            fprintf(stderr,"to the 'name_length' keyword.\nWe recommend to ");
            fprintf(stderr,"simply remove this keyword line in order to ");
            fprintf(stderr,"generate a default *.result file.\n");
            return 0;
        }
        else if (boolean>pParameters->name_length)
        {
            lengthName=boolean;
            fileLocation=(char*)realloc(pParameters->name_input,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->name_input variable.\n");
                return 0;
            }
            pParameters->name_input=fileLocation;

            if (pParameters->name_chem==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
                fprintf(stderr,"pParameters->name_chem");
                fprintf(stderr,"=%p does not ",(void*)pParameters->name_chem);
                fprintf(stderr,"point to a valid address.\nPlease, specify ");
                fprintf(stderr,"in the %s file a ",pParameters->name_input);
                fprintf(stderr,"valid name after the 'name_chem' keyword ");
                fprintf(stderr,"since a (*.chem/ *.wfn) chemical file is ");
                fprintf(stderr,"mandatory to load the chemical data ");
                fprintf(stderr,"properly.\n");
                return 0;
            }
            else
            {
                fileLocation=(char*)realloc(pParameters->name_chem,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_chem variable.\n");
                    return 0;
                }
                pParameters->name_chem=fileLocation;
            }

            if (pParameters->name_mesh!=NULL)
            {
                fileLocation=(char*)realloc(pParameters->name_mesh,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_mesh variable.\n");
                    return 0;
                }
                pParameters->name_mesh=fileLocation;
            }

            if (pParameters->name_elas!=NULL)
            {
                fileLocation=(char*)realloc(pParameters->name_elas,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_elas variable.\n");
                    return 0;
                }
                pParameters->name_elas=fileLocation;
            }

            // Update with the new length
            pParameters->name_length=boolean;
        }

        // Check for './' at the beginning of pParameters->name_result
        if (pParameters->name_result[0]=='.' &&
                                               pParameters->name_result[1]=='/')
        {
            iMax=pParameters->name_length;
            for (i=2; i<iMax; i++)
            {
                pParameters->name_result[i-2]=pParameters->name_result[i];
            }
            pParameters->name_result[iMax-2]='\0';
            pParameters->name_result[iMax-1]='\0';
        }

        // Check pParameters->name_result is a string of length greater than 7
        lengthName=strlen(pParameters->name_result);
        if (lengthName<8)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the name_result");
            fprintf(stderr,"variable (=%s) of the ",pParameters->name_result);
            fprintf(stderr,"structure pointed by pParameters should be a ");
            fprintf(stderr,"string of length (strictly) greater than 7 ");
            fprintf(stderr,"in order to store at least something more than ");
            fprintf(stderr,"the *.result extension.\nPlease specify in the ");
            fprintf(stderr,"%s file a correct name ",pParameters->name_input);
            fprintf(stderr,"after the 'name_result' keyword, or we recommend ");
            fprintf(stderr,"to simply remove this keyword line in order to ");
            fprintf(stderr,"generate a default *.result file.\n");
            return 0;
        }

        // Check if the *.result name ends with the ".result" extension
        boolean=(pParameters->name_result[lengthName-7]=='.');
        boolean=(boolean && pParameters->name_result[lengthName-6]=='r');
        boolean=(boolean && pParameters->name_result[lengthName-5]=='e');
        boolean=(boolean && pParameters->name_result[lengthName-4]=='s');
        boolean=(boolean && pParameters->name_result[lengthName-3]=='u');
        boolean=(boolean && pParameters->name_result[lengthName-2]=='l');
        boolean=(boolean && pParameters->name_result[lengthName-1]=='t');
        boolean=(boolean && pParameters->name_result[lengthName]=='\0');
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the name_result ");
            fprintf(stderr,"variable of the structure pointed by pParameters ");
            fprintf(stderr,"points to the %s file ",pParameters->name_result);
            fprintf(stderr,"name, which does not end with with the '.result' ");
            fprintf(stderr,"extension.\nPlease specify in the ");
            fprintf(stderr,"%s file a valid name ",pParameters->name_input);
            fprintf(stderr,"after the 'name_result' keyword, or we recommend ");
            fprintf(stderr,"to simply remove this keyword line in order to ");
            fprintf(stderr,"generate a default *.result file name.\n");
            return 0;
        }
    }
    else
    {
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: no *.result file name has been ");
            fprintf(stdout,"loaded in the structure pointed by pParameters. ");
            fprintf(stdout,"A default file name will thus be generated ");
            fprintf(stdout,"thanks to the %s file ",pParameters->name_input);
            fprintf(stdout,"name (where the '.input' extension will be ");
            fprintf(stdout,"replaced by the '.result' one).\n\n");
        }

        if (pParameters->name_length==8)
        {
            // In this (rare) case, we must add 1 char. for non-nul name_* var.
            lengthName=pParameters->name_length+1;

            // realloc returns a pointer to the allocated memory, otherwise NULL
            fileLocation=(char*)realloc(pParameters->name_input,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->name_input variable.\n");
                return 0;
            }
            pParameters->name_input=fileLocation;

            if (pParameters->name_chem!=NULL)
            {
                fileLocation=(char*)realloc(pParameters->name_chem,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_chem variable.\n");
                    return 0;
                }
                pParameters->name_chem=fileLocation;
            }
            else
            {
                PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
                fprintf(stderr,"pParameters->name_chem");
                fprintf(stderr,"=%p does not ",(void*)pParameters->name_chem);
                fprintf(stderr,"point to a valid address.\nPlease, specify ");
                fprintf(stderr,"in the %s file a ",pParameters->name_input);
                fprintf(stderr,"valid name after the 'name_chem' keyword ");
                fprintf(stderr,"since a (*.chem/ *.wfn) chemical file is ");
                fprintf(stderr,"mandatory in order to load the chemical data ");
                fprintf(stderr,"properly.\n");
                return 0;
            }

            if (pParameters->name_mesh!=NULL)
            {
                fileLocation=(char*)realloc(pParameters->name_mesh,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_mesh variable.\n");
                    return 0;
                }
                pParameters->name_mesh=fileLocation;
            }

            if (pParameters->name_elas!=NULL)
            {
                fileLocation=(char*)realloc(pParameters->name_elas,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_elas variable.\n");
                    return 0;
                }
                pParameters->name_elas=fileLocation;
            }

            // Do not forget to update the pParameters->name_length variable
            pParameters->name_length=lengthName;
        }

        // calloc returns a pointer to the allocated memory, otherwise NULL
        pParameters->name_result=(char*)calloc(pParameters->name_length,
                                                                  sizeof(char));
        if (pParameters->name_result==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: could not allocate ");
            fprintf(stderr,"memory for the pParameters->name_result ");
            fprintf(stderr,"variable.\n");
            return 0;
        }

        // strncpy function returns a pointer to the string (not counting '\0')
        strncpy(pParameters->name_result,pParameters->name_input,
                                                      pParameters->name_length);
        lengthName=strlen(pParameters->name_result);
        pParameters->name_result[lengthName-6]='.';
        pParameters->name_result[lengthName-5]='r';
        pParameters->name_result[lengthName-4]='e';
        pParameters->name_result[lengthName-3]='s';
        pParameters->name_result[lengthName-2]='u';
        pParameters->name_result[lengthName-1]='l';
        pParameters->name_result[lengthName]='t';
        pParameters->name_result[lengthName+1]='\0';
    }

    // Check pParameters->name_chem and if the associated string begins with '~'
    boolean=checkForTildeAndReplaceByHomePath(&pParameters->name_chem,
                                                      pParameters->name_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->name_chem variable.\nPlease check in ");
        fprintf(stderr,"the %s file if a valid file ",pParameters->name_input);
        fprintf(stderr,"name has been specified after the 'name_chem' ");
        fprintf(stderr,"keyword (a string of length strictly less than ");
        fprintf(stderr,"%d), or extend properly the ",pParameters->name_length);
        fprintf(stderr,"allowed length for name_* file names thanks to the ");
        fprintf(stderr,"'name_length' keyword.\nWe recall that such keyword ");
        fprintf(stderr,"line is mandatory in order to load the chemical data ");
        fprintf(stderr,"properly (via a *.chem or *.wfn file).\n");
        return 0;
    }
    else if (boolean>pParameters->name_length)
    {
        lengthName=boolean;
        fileLocation=(char*)realloc(pParameters->name_input,
                                                       lengthName*sizeof(char));
        if (fileLocation==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->name_input ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->name_input=fileLocation;

        fileLocation=(char*)realloc(pParameters->name_result,
                                                       lengthName*sizeof(char));
        if (fileLocation==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->name_result ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        pParameters->name_result=fileLocation;

        if (pParameters->name_mesh!=NULL)
        {
            fileLocation=(char*)realloc(pParameters->name_mesh,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->name_mesh variable.\n");
                return 0;
            }
            pParameters->name_mesh=fileLocation;
        }

        if (pParameters->name_elas!=NULL)
        {
            fileLocation=(char*)realloc(pParameters->name_elas,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->name_elas variable.\n");
                return 0;
            }
            pParameters->name_elas=fileLocation;
        }

        // Update with the new length
        pParameters->name_length=boolean;
    }

    // Check if pParameters->name_chem begins with './'
    if (pParameters->name_chem[0]=='.' && pParameters->name_chem[1]=='/')
    {
        iMax=pParameters->name_length;
        for (i=2; i<iMax; i++)
        {
            pParameters->name_chem[i-2]=pParameters->name_chem[i];
        }
        pParameters->name_chem[iMax-2]='\0';
        pParameters->name_chem[iMax-1]='\0';
    }

    // Check if pParameters->name_chem is a string of length greater than 5
    lengthName=strlen(pParameters->name_chem);
    if (lengthName<6)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the name_chem variable ");
        fprintf(stderr,"(=%s) of the structure ",pParameters->name_chem);
        fprintf(stderr,"pointed by pParameters should be a string of length ");
        fprintf(stderr,"(strictly) greater than 5 in order to store at least ");
        fprintf(stderr,"something more than the *.chem or *.wfn extension.\n");
        fprintf(stderr,"Please specify in the %s ",pParameters->name_input);
        fprintf(stderr,"file a correct name after the 'name_chem' keyword ");
        fprintf(stderr,"since a (*.chem/ *.wfn) chemical file is mandatory ");
        fprintf(stderr,"for loading the chemical data properly.\n");
        return 0;
    }

    // Check pParameters->ls_type
    boolean=(pParameters->ls_type==0 || pParameters->ls_type==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the ls_type variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->ls_type);
        fprintf(stderr,"by pParameters can only be set to 0 or 1.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'ls_type' ");
        fprintf(stderr,"keyword in the %s file.\n",pParameters->name_input);
        return 0;
    }

    // Check pParameters->name_mesh
    if (pParameters->name_mesh!=NULL)
    {
        // Check for '~/' at the beginning of pParameters->name_mesh
        boolean=checkForTildeAndReplaceByHomePath(&pParameters->name_mesh,
                                                      pParameters->name_length);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: ");
            fprintf(stderr,"checkForTildeAndReplaceByHomePath function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, while attempting to replace the first '~' ");
            fprintf(stderr,"character by the full home directory path name ");
            fprintf(stderr,"in the pParameters->name_mesh variable.\nPlease ");
            fprintf(stderr,"check in the %s file if ",pParameters->name_input);
            fprintf(stderr,"a valid mesh file name has been specified after ");
            fprintf(stderr,"the 'name_mesh' keyword (a non-empty string of ");
            fprintf(stderr,"length strictly less than ");
            fprintf(stderr,"%d), or extend properly ",pParameters->name_length);
            fprintf(stderr,"the allowed length for name_* file names thanks ");
            fprintf(stderr,"to the 'name_length' keyword, or simply remove ");
            fprintf(stderr,"this keyword line in order to generate a default ");
            fprintf(stderr,"mesh file.\n");
            return 0;
        }
        else if (boolean>pParameters->name_length)
        {
            lengthName=boolean;
            fileLocation=(char*)realloc(pParameters->name_input,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->name_input variable.\n");
                return 0;
            }
            pParameters->name_input=fileLocation;

            fileLocation=(char*)realloc(pParameters->name_result,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->name_result variable.\n");
                return 0;
            }
            pParameters->name_result=fileLocation;

            fileLocation=(char*)realloc(pParameters->name_chem,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->name_chem variable.\n");
                return 0;
            }
            pParameters->name_chem=fileLocation;

            if (pParameters->name_elas!=NULL)
            {
                fileLocation=(char*)realloc(pParameters->name_elas,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_elas variable.\n");
                    return 0;
                }
                pParameters->name_elas=fileLocation;
            }

            // Update with the new length
            pParameters->name_length=boolean;
        }

        // Check for './' at the beginning of pParameters->name_mesh
        if (pParameters->name_mesh[0]=='.' && pParameters->name_mesh[1]=='/')
        {
            iMax=pParameters->name_length;
            for (i=2; i<iMax; i++)
            {
                pParameters->name_mesh[i-2]=pParameters->name_mesh[i];
            }
            pParameters->name_mesh[iMax-2]='\0';
            pParameters->name_mesh[iMax-1]='\0';
        }

        // Check pParameters->name_mesh is a string of length greater than 5
        lengthName=strlen(pParameters->name_mesh);
        if (lengthName<6)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the name_mesh ");
            fprintf(stderr,"variable (=%s) of the ",pParameters->name_mesh);
            fprintf(stderr,"structure pointed by pParameters should be a ");
            fprintf(stderr,"string of length (strictly) greater than 5 ");
            fprintf(stderr,"in order to store at least something more than ");
            fprintf(stderr,"the *.mesh or *.cube extension.\nPlease specify ");
            fprintf(stderr,"in the %s file a correct ",pParameters->name_input);
            fprintf(stderr,"name after the 'name_mesh' keyword, or simply ");
            fprintf(stderr,"remove this keyword line in order to generate a ");
            fprintf(stderr,"default mesh file.\n");
            return 0;
        }
    }
    else
    {
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: no mesh file name has been loaded in ");
            fprintf(stdout,"the structure pointed by pParameters. A default ");
            fprintf(stdout,"file name will thus be generated thanks to the ");
            fprintf(stdout,"%s file name (where the ",pParameters->name_input);
            fprintf(stdout,"'.input' extension will be replaced by the ");
            fprintf(stdout,"'.mesh' one).\n\n");
        }
    }

    // Check pParameters->name_elas
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        if (pParameters->name_elas!=NULL)
        {
            boolean=initialFileExists(pParameters->name_elas,
                                                      pParameters->name_length);
            if (abs(boolean)!=1)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: ");
                fprintf(stderr,"initialFileExists function returned zero ");
                fprintf(stderr,"instead of (+/-) one, while attempting to ");
                fprintf(stderr,"check if the pParameters->name_elas variable ");
                fprintf(stderr,"stores the name of an existing file.\nPlease ");
                fprintf(stderr,"specify in the %s ",pParameters->name_input);
                fprintf(stderr,"file a valid name after the 'name_elas' ");
                fprintf(stderr,"keyword (a string of length strictly less ");
                fprintf(stderr,"than %d), or extend ",pParameters->name_length);
                fprintf(stderr,"if necessary the allowed length for name_* ");
                fprintf(stderr,"file names thanks to the 'name_length' ");
                fprintf(stderr,"keyword.\nWe recommend to simply remove ");
                fprintf(stderr,"this keyword line in order to generate a ");
                fprintf(stderr,"default *.elas file.\n");
                return 0;
            }
            else if (boolean==-1)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: the file ");
                fprintf(stderr,"%s could not be found ",pParameters->name_elas);
                fprintf(stderr,"at the given location.\nPlease specify in ");
                fprintf(stderr,"the %s file an ",pParameters->name_input);
                fprintf(stderr,"existing filen after the 'name_elas' ");
                fprintf(stderr,"keyword.\nWe recommend to simply remove this ");
                fprintf(stderr,"keyword line in order to generate a default ");
                fprintf(stderr,"*.elas file.\n");
                return 0;
            }

            // Check for './' at the beginning of pParameters->name_elas
            if (pParameters->name_elas[0]=='.' &&
                                                 pParameters->name_elas[1]=='/')
            {
                iMax=pParameters->name_length;
                for (i=2; i<iMax; i++)
                {
                    pParameters->name_elas[i-2]=pParameters->name_elas[i];
                }
                pParameters->name_elas[iMax-2]='\0';
                pParameters->name_elas[iMax-1]='\0';
            }

            // Check for '~/' at the beginning of pParameters->name_elas
            boolean=checkForTildeAndReplaceByHomePath(&pParameters->name_elas,
                                                      pParameters->name_length);
            if (!boolean)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: ");
                fprintf(stderr,"checkForTildeAndReplaceByHomePath function ");
                fprintf(stderr,"returned zero, which is not the expected ");
                fprintf(stderr,"value here, while attempting to replace the ");
                fprintf(stderr,"first '~' character by the full home ");
                fprintf(stderr,"directory path name in the ");
                fprintf(stderr,"pParameters->name_elas variable ");
                fprintf(stderr,"(=%s).\n",pParameters->name_elas);
                return 0;
            }
            else if (boolean>pParameters->name_length)
            {
                lengthName=boolean;
                fileLocation=(char*)realloc(pParameters->name_input,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_input variable.\n");
                    return 0;
                }
                pParameters->name_input=fileLocation;

                fileLocation=(char*)realloc(pParameters->name_result,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_result variable.\n");
                    return 0;
                }
                pParameters->name_result=fileLocation;

                fileLocation=(char*)realloc(pParameters->name_chem,
                                                       lengthName*sizeof(char));
                if (fileLocation==NULL)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                    fprintf(stderr,"reallocate memory for the ");
                    fprintf(stderr,"pParameters->name_chem variable.\n");
                    return 0;
                }
                pParameters->name_chem=fileLocation;

                if (pParameters->name_mesh!=NULL)
                {
                    fileLocation=(char*)realloc(pParameters->name_mesh,
                                                       lengthName*sizeof(char));
                    if (fileLocation==NULL)
                    {
                        PRINT_ERROR("In checkValuesOfAllParameters: could ");
                        fprintf(stderr,"not reallocate memory for the ");
                        fprintf(stderr,"pParameters->name_mesh variable.\n");
                        return 0;
                    }
                    pParameters->name_mesh=fileLocation;
                }

                // Update with the new length
                pParameters->name_length=boolean;
            }

            // Check that the *.elas file name has length greater than six
            lengthName=strlen(pParameters->name_elas);
            if (lengthName<6)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: the (char*) ");
                fprintf(stderr,"name_elas variable of the structure pointed ");
                fprintf(stderr,"by pParameters points to the ");
                fprintf(stderr,"%s file name, which ",pParameters->name_elas);
                fprintf(stderr,"should at least contain six characters ");
                fprintf(stderr,"instead of %d (in order to ",(int)lengthName-1);
                fprintf(stderr,"end with something more than the *.elas ");
                fprintf(stderr,"extension).\nPlease specify in the ");
                fprintf(stderr,"%s file a valid name ",pParameters->name_input);
                fprintf(stderr,"after the 'name_elas' keyword.\nWe recommend ");
                fprintf(stderr,"to simply remove this keyword line in order ");
                fprintf(stderr,"to generate a default*.elas file.\n");
                return 0;
            }

            // Check if the *.elas file name ends with the ".elas" extension
            if (pParameters->name_elas[lengthName-5]!='.' ||
                                    pParameters->name_elas[lengthName-4]!='e' ||
                                    pParameters->name_elas[lengthName-3]!='l' ||
                                    pParameters->name_elas[lengthName-2]!='a' ||
                                    pParameters->name_elas[lengthName-1]!='s' ||
                                       pParameters->name_elas[lengthName]!='\0')
            {
                PRINT_ERROR("In checkValuesOfAllParameters: the (char*) ");
                fprintf(stderr,"name_elas variable of the structure pointed ");
                fprintf(stderr,"by pParameters points to the ");
                fprintf(stderr,"%s file name, which ",pParameters->name_elas);
                fprintf(stderr,"does not end with with the '.elas' ");
                fprintf(stderr,"extension.\nPlease specify in the ");
                fprintf(stderr,"%s file a valid name ",pParameters->name_input);
                fprintf(stderr,"after the 'name_elas' keyword.\nWe recommend ");
                fprintf(stderr,"to simply remove this keyword line in order ");
                fprintf(stderr,"to generate a default*.elas file.\n");
                return 0;
            }

            // Allocate memory for the fileLocation variable
            lengthName=pParameters->name_length;
            fileLocation=(char*)calloc(lengthName,sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"allocate memory for the local (char*) ");
                fprintf(stderr,"fileLocation variable.\n");
                return 0;
            }

            // Set the default *.elas name
            strncpy(fileLocation,pParameters->name_input,lengthName);
            lengthName=strlen(fileLocation);
            fileLocation[lengthName-6]='.';
            fileLocation[lengthName-5]='e';
            fileLocation[lengthName-4]='l';
            fileLocation[lengthName-3]='a';
            fileLocation[lengthName-2]='s';
            fileLocation[lengthName-1]='\0';

            // strcmp returns 0 if the 2 strings are ==, otherwise <0 (resp. >0)
            // if the 1st string arg. is shorter (resp. longer) than the 2nd one
            if (strcmp(pParameters->name_elas,fileLocation))
            {
                boolean=initialFileExists(fileLocation,
                                                      pParameters->name_length);
                if (abs(boolean)!=1)
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: ");
                    fprintf(stderr,"initialFileExists function ");
                    fprintf(stderr,"returned zero instead of (+/-) one, ");
                    fprintf(stderr,"while attempting to check if the ");
                    fprintf(stderr,"%s file already exists.\n",fileLocation);

                    // free function does not return any value (void output)
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
                else if (boolean==1)
                {
                    // remove returns 0 on success, otherwise -1
                    if (remove(fileLocation))
                    {
                        PRINT_ERROR("In checkValuesOfAllParameters: wrong ");
                        fprintf(stderr,"return (=-1) of the standard remove ");
                        fprintf(stderr,"c-function in the attempt of ");
                        fprintf(stderr,"removing the ");
                        fprintf(stderr,"%s file.\n",fileLocation);
                        free(fileLocation);
                        fileLocation=NULL;
                        return 0;
                    }
                }

                if (!copyFileLocation(pParameters->name_elas,
                                         pParameters->name_length,
                                             pParameters->verbose,fileLocation))
                {
                    PRINT_ERROR("In checkValuesOfAllParameters: ");
                    fprintf(stderr,"copyFileLocation function returned zero ");
                    fprintf(stderr,"instead of one.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }

                // Update the name of the *.elas file
                strncpy(pParameters->name_elas,fileLocation,
                                                      pParameters->name_length);
            }

            // Free the memory allocated for fileLocation
            free(fileLocation);
            fileLocation=NULL;
        }
        else
        {
            if (pParameters->verbose>1)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: no *.elas file name has been ");
                fprintf(stdout,"loaded in the structure pointed by ");
                fprintf(stdout,"pParameters. A default file name will thus ");
                fprintf(stdout,"be generated thanks to the ");
                fprintf(stdout,"%s file name (where ",pParameters->name_input);
                fprintf(stdout,"the '.input' extension will be replaced by ");
                fprintf(stdout,"the '.elas' one), and default values will be ");
                fprintf(stdout,"inserted in this *.elas file.\n\n");
            }

            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nGenerating file for the elastic software ");
                fprintf(stdout,"with the default Lamé coefficients.");
            }

            if (!writingDefaultElasticFile(pParameters))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: ");
                fprintf(stderr,"writingDefaultElasticFile function returned ");
                fprintf(stderr,"zero instead of one.\n");
                return 0;
            }
        }
    }
    else
    {
        if (pParameters->name_elas!=NULL)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: an *.elas file name has been ");
                fprintf(stdout,"loaded in the structure pointed by ");
                fprintf(stdout,"pParameters although the current ");
                fprintf(stdout,"optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such a file, which is thus ignored from now ");
                fprintf(stdout,"on.\n\n");
            }
            free(pParameters->name_elas);
            pParameters->name_elas=NULL;
        }
    }

    // Check pParameters->nu_electrons
    boolean=(pParameters->nu_electrons>0);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the nu_electrons ");
        fprintf(stderr,"variable (=%d) of the ",pParameters->nu_electrons);
        fprintf(stderr,"structure pointed by pParameters must be a positive ");
        fprintf(stderr,"integer.\nPlease specify a valid value after the ");
        fprintf(stderr,"'nu_electrons' keyword in %s ",pParameters->name_input);
        fprintf(stderr,"file since a positive number of electrons to look ");
        fprintf(stderr,"for is mandatory for proceeding further.\n");
        return 0;
    }

    // Check pParameters->bohr_unit
    boolean=(pParameters->bohr_unit==0 || pParameters->bohr_unit==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
        fprintf(stderr,"pParameters->bohr_unit (=%d) ",pParameters->bohr_unit);
        fprintf(stderr,"can only be set to 0 or 1.\nPlease modify the value ");
        fprintf(stderr,"accordingly after the 'bohr_unit' keyword in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_input);
        return 0;
    }

    // Check pParameters->select_orb
    boolean=(pParameters->select_orb>=0. && pParameters->select_orb<.01);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: expecting\n0.0 <= (");
        fprintf(stderr,"pParameters->select_orb=%lf) ",pParameters->select_orb);
        fprintf(stderr,"< 0.01\nPlease modify the value accordingly after ");
        fprintf(stderr,"the 'select_orb' keyword in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_input);
        return 0;
    }

    // Check pParameters->orb_ortho
    boolean=(pParameters->orb_ortho==0 || pParameters->orb_ortho==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
        fprintf(stderr,"pParameters->orb_ortho (=%d) ",pParameters->orb_ortho);
        fprintf(stderr,"can only be set to 0 or 1.\nPlease modify the value ");
        fprintf(stderr,"accordingly after the 'orb_ortho' keyword in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_input);
        return 0;
    }

    // Check pParameters->ne_electrons
    boolean=(!pParameters->ne_electrons ||
                          pParameters->ne_electrons>=pParameters->nu_electrons);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the ne_electrons ");
        fprintf(stderr,"variable (=%d) of the ",pParameters->ne_electrons);
        fprintf(stderr,"structure pointed by pParameters ");
        if (pParameters->ne_electrons<0)
        {
            fprintf(stderr,"must be a non-negative integer.\n");
        }
        else
        {
             fprintf(stderr,"refers to the total number of electrons in the ");
             fprintf(stderr,"chemical system. Hence, it cannot be (strictly) ");
             fprintf(stderr,"lower than the pParameters->nu_electrons ");
             fprintf(stderr,"variable (=%d), ",pParameters->nu_electrons);
             fprintf(stderr,"representing the number of electrons we are ");
             fprintf(stderr,"looking for.\n");
        }
        fprintf(stderr,"Please modify the value accordingly after the ");
        fprintf(stderr,"'ne_electrons' keyword in %s ",pParameters->name_input);
        fprintf(stderr,"file.\nwe recommend to simply remove the ");
        fprintf(stderr,"corresponding keyword line to let the MPD program ");
        fprintf(stderr,"set this variable automatically during the reading ");
        fprintf(stderr,"of the (*.chem/ *.wfn) chemical file.\n");
        return 0;
    }

    // Check pParameters->multi_det
    boolean=(pParameters->multi_det==0 || pParameters->multi_det==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the multi_det variable ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->multi_det);
        fprintf(stderr,"pointed by pParameters can only be set to 0 or 1.\n");
        fprintf(stderr,"Please modify value accordingly after the ");
        fprintf(stderr,"'multi_det' keyword in %s ",pParameters->name_input);
        fprintf(stderr,"file.\nwe recommend to simply remove the c");
        fprintf(stderr,"corresponding keyword line to let the MPD program ");
        fprintf(stderr,"set this variable automatically during the reading ");
        fprintf(stderr,"of the (*.chem/ *.wfn) chemical file.\n");
        return 0;
    }

    // Check pParameters->orb_rhf
    boolean=(pParameters->orb_rhf==0 || pParameters->orb_rhf==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the orb_rh variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->orb_rhf);
        fprintf(stderr,"by pParameters can only be set to 0 or 1.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'orb_rhf' ");
        fprintf(stderr,"keyword in the %s file.\nWe ",pParameters->name_input);
        fprintf(stderr,"recommend to simply remove the corresponding keyword ");
        fprintf(stderr,"line to let the MPD program set this variable ");
        fprintf(stderr,"automatically during the reading of the ");
        fprintf(stderr,"(*.chem/ *.wfn) chemical file.\n");
        return 0;
    }

    // Check pParameters->select_box
    boolean=((pParameters->select_box>=0. && pParameters->select_box<=.1) ||
                   (pParameters->select_box>=.9 && pParameters->select_box<1.));
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: expecting\n0.0 <= (");
        fprintf(stderr,"pParameters->select_box=%lf) ",pParameters->select_box);
        fprintf(stderr,"<= 0.1\n\nor\n\n0.9 <= (");
        fprintf(stderr,"pParameters->select_box=%lf) ",pParameters->select_box);
        fprintf(stderr,"<  1.0\nPlease modify value accordingly after the ");
        fprintf(stderr,"'select_box' keyword in %s ",pParameters->name_input);
        fprintf(stderr,"file.\n");
        return 0;
    }

    // Check the variables related to the computational box
    boolean=(pParameters->x_min<pParameters->x_max);
    boolean=(boolean && pParameters->y_min<pParameters->y_max);
    boolean=(boolean && pParameters->z_min<pParameters->z_max);
    boolean=(boolean && pParameters->n_x>2);
    boolean=(boolean && pParameters->n_y>2);
    boolean=(boolean && pParameters->n_z>2);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: incorrect description of ");
        fprintf(stderr,"the cube associated with the computational box. ");
        fprintf(stderr,"Expecting\n");
        fprintf(stderr,"(pParameters->x_min=%lf) < ",pParameters->x_min);
        fprintf(stderr,"(pParameters->x_max=%lf)\n",pParameters->x_max);
        fprintf(stderr,"(pParameters->y_min=%lf) < ",pParameters->y_min);
        fprintf(stderr,"(pParameters->y_max=%lf)\n",pParameters->y_max);
        fprintf(stderr,"(pParameters->z_min=%lf) < ",pParameters->z_min);
        fprintf(stderr,"(pParameters->z_max=%lf)\n",pParameters->z_max);
        fprintf(stderr,"(pParameters->n_x=%d) > 2\n",pParameters->n_x);
        fprintf(stderr,"(pParameters->n_y=%d) > 2\n",pParameters->n_y);
        fprintf(stderr,"(pParameters->n_z=%d) > 2\n",pParameters->n_z);
        fprintf(stderr,"Please modify the values accordingly after the ");
        fprintf(stderr,"corresponding keywords in %s ",pParameters->name_input);
        fprintf(stderr,"file.\n");
        return 0;
    }

    dx=(pParameters->x_max-pParameters->x_min)/(double)(pParameters->n_x-1);
    dy=(pParameters->y_max-pParameters->y_min)/(double)(pParameters->n_y-1);
    dz=(pParameters->z_max-pParameters->z_min)/(double)(pParameters->n_z-1);

    boolean=(DEF_ABS(pParameters->delta_x-dx)<1.e-16);
    boolean=(boolean && DEF_ABS(pParameters->delta_y-dy)<1.e-16);
    boolean=(boolean && DEF_ABS(pParameters->delta_z-dz)<1.e-16);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: incorrect discretization ");
        fprintf(stderr,"of the cube associated with the computational box. ");
        fprintf(stderr,"Expecting\n");
        fprintf(stderr,"(pParameters->delta_x=%.18lf) ",pParameters->delta_x);
        fprintf(stderr,"== %.18lf\n",dx);
        fprintf(stderr,"(pParameters->delta_y=%.18lf) ",pParameters->delta_y);
        fprintf(stderr,"== %.18lf\n",dy);
        fprintf(stderr,"(pParameters->delta_z=%.18lf) ",pParameters->delta_z);
        fprintf(stderr,"== %.18lf\nPlease modify the values accordingly ",dz);
        fprintf(stderr,"after the corresponding keywords in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_input);
        return 0;
    }

    // Check pParameters->ls_ini
    boolean=(pParameters->ls_ini==0 || pParameters->ls_ini==1);
    boolean=(boolean || pParameters->ls_ini==2);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the ls_ini variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->ls_ini);
        fprintf(stderr,"by pParameters can only be set to 0, 1 or 2.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'ls_ini' ");
        fprintf(stderr,"keyword in the %s file.\nWe ",pParameters->name_input);
        fprintf(stderr,"recommend to simply remove the corresponding keyword ");
        fprintf(stderr,"line to let the MPD program set this variable ");
        fprintf(stderr,"automatically during the setting up of the mesh.\n");
        return 0;
    }

    // Check the variables related to the initial level-set function
    boolean=(pParameters->ls_x>=pParameters->x_min);
    boolean=(boolean && pParameters->ls_x<=pParameters->x_max);
    boolean=(boolean && pParameters->ls_y>=pParameters->y_min);
    boolean=(boolean && pParameters->ls_y<=pParameters->y_max);
    boolean=(boolean && pParameters->ls_z>=pParameters->z_min);
    boolean=(boolean && pParameters->ls_z<=pParameters->z_max);
    if (!boolean && pParameters->verbose>1)
    {
        fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
        fprintf(stdout,"function: the center (%lf,",pParameters->ls_x);
        fprintf(stdout,"%lf,%lf) of ",pParameters->ls_y,pParameters->ls_z);
        fprintf(stdout,"the initial domain is not located inside the ");
        fprintf(stdout,"computational domain ");
        fprintf(stdout,"[%lf,%lf]x",pParameters->x_min,pParameters->x_max);
        fprintf(stdout,"[%lf,%lf]x",pParameters->y_min,pParameters->y_max);
        fprintf(stdout,"[%lf,%lf].\n\n",pParameters->z_min,pParameters->z_max);
    }

    boolean=(pParameters->ls_rx>0. && pParameters->ls_ry>0.);
    boolean=(boolean && pParameters->ls_rz>0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the variables ls_rx ");
        fprintf(stderr,"(=%lf), ls_ry ",pParameters->ls_rx);
        fprintf(stderr,"(=%lf) and ls_rz ",pParameters->ls_ry);
        fprintf(stderr,"(=%lf) of the structure pointed ",pParameters->ls_rz);
        fprintf(stderr,"by pParameters must be positive.\nPlease modify ");
        fprintf(stderr,"the values accordingly after the 'ls_rx', 'ls_ry' ");
        fprintf(stderr,"and 'ls_rz' keywords in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_input);
        return 0;
    }

    if (pParameters->ls_type)
    {
        boolean=(pParameters->ls_rx==pParameters->ls_ry);
        boolean=(boolean || pParameters->ls_rx==pParameters->ls_rz);
        boolean=(boolean || pParameters->ls_ry==pParameters->ls_rz);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: in the case where ");
            fprintf(stderr,"the initial domain is a sphere/cigar ");
            fprintf(stderr,"(pParameters->ls_type=%d), ",pParameters->ls_type);
            fprintf(stderr,"at least two values must be equal among the ");
            fprintf(stderr,"variables ls_rx (=%lf), ls_ry ",pParameters->ls_rx);
            fprintf(stderr,"(=%lf) and ls_rz ",pParameters->ls_ry);
            fprintf(stderr,"(=%lf) of the structure ",pParameters->ls_rz);
            fprintf(stderr,"pointed by pParameters.\nPlease modify the ");
            fprintf(stderr,"values accordingly after the 'ls_rx', 'ls_ry' ");
            fprintf(stderr,"and 'ls_rz' keywords in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
        }
    }

    // Check the variables related to the metric computation
    if (pParameters->opt_mode>0)
    {
        boolean=(pParameters->met_err>0. && pParameters->met_min>0.);
        boolean=(boolean && pParameters->met_max>pParameters->met_min);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: expecting\n");
            fprintf(stderr,"(pParameters->met_err=%lf) ",pParameters->met_err);
            fprintf(stderr,"> 0.0\n");
            fprintf(stderr,"(pParameters->met_min=%lf) ",pParameters->met_min);
            fprintf(stderr,"> 0.0\n");
            fprintf(stderr,"(pParameters->met_max=%lf) ",pParameters->met_max);
            fprintf(stderr," > pParameters->met_min\nPlease modify the ");
            fprintf(stderr,"values accordingly after the corresponding ");
            fprintf(stderr,"keywords in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
        }
    }
    else
    {
        boolean=(pParameters->met_err==MET_ERR);
        boolean=(boolean && pParameters->met_min==MET_MIN);
        boolean=(boolean && pParameters->met_max==MET_MAX);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: at least one value among the ");
                fprintf(stdout,"variables met_err ");
                fprintf(stdout,"(=%lf), met_min ",pParameters->met_err);
                fprintf(stdout,"(=%lf), and met_max ",pParameters->met_min);
                fprintf(stdout,"(=%lf) of the structure ",pParameters->met_max);
                fprintf(stdout,"pointed by pParameters has been changed from ");
                fprintf(stdout,"its default value ");
                fprintf(stdout,"(MET_ERR=%lf, MET_MIN=%lf, ",MET_ERR,MET_MIN);
                fprintf(stdout,"MET_MAX=%lf) in the ",MET_MAX);
                fprintf(stdout,"%s file, althought ",pParameters->name_input);
                fprintf(stdout,"the current the current optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such parameters.\nThe pParameters->met_err, ");
                fprintf(stdout,"pParameters->met_min, and ");
                fprintf(stdout,"pParameters->met_max variables are thus ");
                fprintf(stdout,"restored to their default values.\n\n");
            }
            pParameters->met_err=MET_ERR;
            pParameters->met_min=MET_MIN;
            pParameters->met_max=MET_MAX;
        }
    }

    // Check pParameters->trick_matrix
    if (pParameters->opt_mode<=0)
    {
        boolean=(pParameters->trick_matrix==0 || pParameters->trick_matrix==1);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the trick_matrix ");
            fprintf(stderr,"variable (=%d) of the ",pParameters->trick_matrix);
            fprintf(stderr,"structure pointed by pParameters can only be set ");
            fprintf(stderr,"to 0 or 1.\nPlease modify the value accordingly ");
            fprintf(stderr,"after the 'trick_matrix' keyword in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
        }
    }
    else
    {
        boolean=(pParameters->trick_matrix==TRICK_MATRIX);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: the trick_matrix variable ");
                fprintf(stdout,"(=%d) of the ",pParameters->trick_matrix);
                fprintf(stdout,"structure pointed by pParameters has been ");
                fprintf(stdout,"changed from its default value ");
                fprintf(stdout,"(=%d) in the ",TRICK_MATRIX);
                fprintf(stdout,"%s file, althought ",pParameters->name_input);
                fprintf(stdout,"the current the current optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such a parameter.\nThe ");
                fprintf(stdout,"pParameters->trick_matrix variable is thus ");
                fprintf(stdout,"restored to its default value.\n\n");
            }
            pParameters->trick_matrix=TRICK_MATRIX;
        }
    }

    // Check pParameters->approx_mode
    if (pParameters->opt_mode<=0)
    {
        boolean=(pParameters->approx_mode==0 || pParameters->approx_mode==1);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the approx_mode ");
            fprintf(stderr,"variable (=%d) of the ",pParameters->approx_mode);
            fprintf(stderr,"structure pointed by pParameters can only be set ");
            fprintf(stderr,"to 0 or 1.\nPlease modify the value accordingly ");
            fprintf(stderr,"after the 'approx_mode' keyword in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
        }
    }
    else
    {
        boolean=(pParameters->approx_mode==APPROX_MODE);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: the approx_mode variable ");
                fprintf(stdout,"(=%d) of the ",pParameters->approx_mode);
                fprintf(stdout,"structure pointed by pParameters has been ");
                fprintf(stdout,"changed from its default value ");
                fprintf(stdout,"(=%d) in the ",APPROX_MODE);
                fprintf(stdout,"%s file, althought ",pParameters->name_input);
                fprintf(stdout,"the current the current optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such a parameter.\nThe ");
                fprintf(stdout,"pParameters->approx_mode variable is thus ");
                fprintf(stdout,"restored to its default value.\n\n");
            }
            pParameters->approx_mode=APPROX_MODE;
        }
    }

    // Check the variables related to the stop criteria
    boolean=(pParameters->iter_ini>=0);
    boolean=(boolean && pParameters->iter_max>=pParameters->iter_ini);
    boolean=(boolean && pParameters->iter_told0p>=0.);
    boolean=(boolean && pParameters->iter_told1p>=0.);
    boolean=(boolean && pParameters->iter_told2p>=0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: expecting\n");
        fprintf(stderr,"(pParameters->iter_ini=%d) >= 0",pParameters->iter_ini);
        fprintf(stderr,"\n(pParameters->iter_max=%d) ",pParameters->iter_max);
        fprintf(stderr,">= pParameters->iter_ini\n(pParameters->iter_told0p");
        fprintf(stderr,"=%lf) >= 0.0\n",pParameters->iter_told0p);
        fprintf(stderr,"(pParameters->iter_told1p");
        fprintf(stderr,"=%lf) >= 0.0\n",pParameters->iter_told1p);
        fprintf(stderr,"(pParameters->iter_told2p");
        fprintf(stderr,"=%lf) >= 0.0\nPlease modify ",pParameters->iter_told2p);
        fprintf(stderr,"the values accordingly after the corresponding ");
        fprintf(stderr,"keywords in the %s file.\n",pParameters->name_input);
        return 0;
    }

    // Check the variables related to the saving of data
    boolean=(pParameters->save_type==0 || pParameters->save_type==1);
    boolean=(boolean || pParameters->save_type==2);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the save_type variable ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->save_type);
        fprintf(stderr,"pointed by pParameters can only be set to 0, 1 or ");
        fprintf(stderr,"2.\nPlease modify the value accordingly after the ");
        fprintf(stderr,"'save_type' keyword in %s ",pParameters->name_input);
        fprintf(stderr,"file.\n");
        return 0;
    }

    boolean=(pParameters->save_mesh>=0 && pParameters->save_data>=0);
    boolean=(boolean && pParameters->save_print>=0);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the save_mesh ");
        fprintf(stderr,"(=%d), save_data ",pParameters->save_mesh);
        fprintf(stderr,"(=%d), and save_print ",pParameters->save_data);
        fprintf(stderr,"(=%d) variables of the ",pParameters->save_print);
        fprintf(stderr,"structure pointed by pParameters must be ");
        fprintf(stderr,"non-negative integers.\nPlease modify the values ");
        fprintf(stderr,"accordingly after the corresponding keywords in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_input);
        return 0;
    }

    boolean=(pParameters->save_where>0 && pParameters->save_where<8);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the save_where variable ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->save_where);
        fprintf(stderr,"pointed by pParameters can only be set to 1, 2, 3, ");
        fprintf(stderr,"4, 5, 6, or 7.\nPlease modify the value accordingly ");
        fprintf(stderr,"after the 'save_type' keyword in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_input);
        return 0;
    }

    // Check pParameters->path_length
    boolean=(pParameters->path_length>=DEF_MAX(2,PATH_LENGTH));
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the variable path_length ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->path_length);
        fprintf(stderr,"pointed by pParameters must be an integer ");
        fprintf(stderr,"(strictly) greater than one (to store at least ");
        fprintf(stderr,"something more than the terminating nul character) ");
        fprintf(stderr,"and which is at least equal or greater than its ");
        fprintf(stderr,"initial default value (=%d).\nPlease ",PATH_LENGTH);
        fprintf(stderr,"modify the value accordingly after the 'path_length' ");
        fprintf(stderr,"keyword in the %s file.\n",pParameters->name_input);
        return 0;
    }

    // Check pParameters->path_medit
    boolean=checkForTildeAndReplaceByHomePath(&pParameters->path_medit,
                                                      pParameters->path_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_medit variable.\nPlease check in ");
        fprintf(stderr,"the %s file that a valid ",pParameters->name_input);
        fprintf(stderr,"path name (a non-empty string of length strictly ");
        fprintf(stderr,"less than %d) has been ",pParameters->path_length);
        fprintf(stderr,"specified for the medit software after the ");
        fprintf(stderr,"'path_medit' keyword, or extend properly the allowed ");
        fprintf(stderr,"length for path_* names thanks to the 'path_length' ");
        fprintf(stderr,"keyword.\n");
        return 0;
    }
    else if (boolean>pParameters->path_length)
    {
        lengthName=boolean;
        if (pParameters->path_mmg3d==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mmg3d");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mmg3d);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mmg3d,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mmg3d variable.\n");
                return 0;
            }
            pParameters->path_mmg3d=fileLocation;
        }

        if (pParameters->path_mshdist==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mshdist");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mshdist);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mshdist,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mshdist variable.\n");
                return 0;
            }
            pParameters->path_mshdist=fileLocation;
        }

        if (pParameters->path_elastic==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_elastic");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_elastic);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_elastic,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_elastic variable.\n");
                return 0;
            }
            pParameters->path_elastic=fileLocation;
        }

        if (pParameters->path_advect==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_advect");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_advect);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_advect,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_advect variable.\n");
                return 0;
            }
            pParameters->path_advect=fileLocation;
        }

        // Update with the new length
        pParameters->path_length=boolean;
    }

    // Check pParameters->path_mmg3d
    boolean=checkForTildeAndReplaceByHomePath(&pParameters->path_mmg3d,
                                                      pParameters->path_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_mmg3d variable.\nPlease check in ");
        fprintf(stderr,"the %s file that a valid ",pParameters->name_input);
        fprintf(stderr,"path name (a non-empty string of length strictly ");
        fprintf(stderr,"less than %d) has been ",pParameters->path_length);
        fprintf(stderr,"specified for the mmg3d software after the ");
        fprintf(stderr,"'path_mmg3d' keyword, or extend properly the allowed ");
        fprintf(stderr,"length for path_* names thanks to the 'path_length' ");
        fprintf(stderr,"keyword.\n");
        return 0;
    }
    else if (boolean>pParameters->path_length)
    {
        lengthName=boolean;
        if (pParameters->path_medit==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_medit");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_medit);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_medit variable.\n");
                return 0;
            }
            pParameters->path_medit=fileLocation;
        }

        if (pParameters->path_mshdist==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mshdist");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mshdist);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mshdist,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mshdist variable.\n");
                return 0;
            }
            pParameters->path_mshdist=fileLocation;
        }

        if (pParameters->path_elastic==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_elastic");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_elastic);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_elastic,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_elastic variable.\n");
                return 0;
            }
            pParameters->path_elastic=fileLocation;
        }

        if (pParameters->path_advect==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_advect");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_advect);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_advect,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_advect variable.\n");
                return 0;
            }
            pParameters->path_advect=fileLocation;
        }

        // Update with the new length
        pParameters->path_length=boolean;
    }

    // Check pParameters->path_mshdist
    boolean=checkForTildeAndReplaceByHomePath(&pParameters->path_mshdist,
                                                      pParameters->path_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_mshdist variable.\nPlease check in ");
        fprintf(stderr,"the %s file that a valid ",pParameters->name_input);
        fprintf(stderr,"path name (a non-empty string of length strictly ");
        fprintf(stderr,"less than %d) has been ",pParameters->path_length);
        fprintf(stderr,"specified for the mshdist software after the ");
        fprintf(stderr,"'path_mshdist' keyword, or extend properly the ");
        fprintf(stderr,"allowed length for path_* names thanks to the ");
        fprintf(stderr,"'path_length' keyword.\n");
        return 0;
    }
    else if (boolean>pParameters->path_length)
    {
        lengthName=boolean;
        if (pParameters->path_medit==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_medit");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_medit);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_medit variable.\n");
                return 0;
            }
            pParameters->path_medit=fileLocation;
        }

        if (pParameters->path_mmg3d==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mmg3d");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mmg3d);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mmg3d,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mmg3d variable.\n");
                return 0;
            }
            pParameters->path_mmg3d=fileLocation;
        }

        if (pParameters->path_elastic==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_elastic");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_elastic);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_elastic,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_elastic variable.\n");
                return 0;
            }
            pParameters->path_elastic=fileLocation;
        }

        if (pParameters->path_advect==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_advect");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_advect);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_advect,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_advect variable.\n");
                return 0;
            }
            pParameters->path_advect=fileLocation;
        }

        // Update with the new length
        pParameters->path_length=boolean;
    }

    // Check pParameters->path_elastic
    boolean=checkForTildeAndReplaceByHomePath(&pParameters->path_elastic,
                                                      pParameters->path_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_elastic variable.\nPlease check in ");
        fprintf(stderr,"the %s file that a valid ",pParameters->name_input);
        fprintf(stderr,"path name (a non-empty string of length strictly ");
        fprintf(stderr,"less than %d) has been ",pParameters->path_length);
        fprintf(stderr,"specified for the elastic software after the ");
        fprintf(stderr,"'path_elastic' keyword, or extend properly the ");
        fprintf(stderr,"allowed length for path_* names thanks to the ");
        fprintf(stderr,"'path_length' keyword.\n");
        return 0;
    }
    else if (boolean>pParameters->path_length)
    {
        lengthName=boolean;
        if (pParameters->path_medit==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_medit");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_medit);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_medit variable.\n");
                return 0;
            }
            pParameters->path_medit=fileLocation;
        }

        if (pParameters->path_mmg3d==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mmg3d");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mmg3d);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mmg3d,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mmg3d variable.\n");
                return 0;
            }
            pParameters->path_mmg3d=fileLocation;
        }

        if (pParameters->path_mshdist==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mshdist");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mshdist);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mshdist,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mshdist variable.\n");
                return 0;
            }
            pParameters->path_mshdist=fileLocation;
        }

        if (pParameters->path_advect==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_advect");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_advect);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_advect,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_advect variable.\n");
                return 0;
            }
            pParameters->path_advect=fileLocation;
        }

        // Update with the new length
        pParameters->path_length=boolean;
    }

    // Check pParameters->path_advect
    boolean=checkForTildeAndReplaceByHomePath(&pParameters->path_advect,
                                                      pParameters->path_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: ");
        fprintf(stderr,"checkForTildeAndReplaceByHomePath function returned ");
        fprintf(stderr,"zero, which is not the expected value here, while ");
        fprintf(stderr,"attempting to replace the first '~' character by the ");
        fprintf(stderr,"full home directory path name in the ");
        fprintf(stderr,"pParameters->path_advect variable.\nPlease check in ");
        fprintf(stderr,"the %s file that a valid ",pParameters->name_input);
        fprintf(stderr,"path name (a non-empty string of length strictly ");
        fprintf(stderr,"less than %d) has been ",pParameters->path_length);
        fprintf(stderr,"specified for the advect software after the ");
        fprintf(stderr,"'path_advect' keyword, or extend properly the ");
        fprintf(stderr,"allowed length for path_* names thanks to the ");
        fprintf(stderr,"'path_length' keyword.\n");
        return 0;
    }
    else if (boolean>pParameters->path_length)
    {
        lengthName=boolean;
        if (pParameters->path_medit==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_medit");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_medit);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_medit,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_medit variable.\n");
                return 0;
            }
            pParameters->path_medit=fileLocation;
        }

        if (pParameters->path_mmg3d==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mmg3d");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mmg3d);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mmg3d,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mmg3d variable.\n");
                return 0;
            }
            pParameters->path_mmg3d=fileLocation;
        }

        if (pParameters->path_mshdist==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_mshdist");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_mshdist);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_mshdist,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_mshdist variable.\n");
                return 0;
            }
            pParameters->path_mshdist=fileLocation;
        }

        if (pParameters->path_elastic==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the variable ");
            fprintf(stderr,"pParameters->path_elastic");
            fprintf(stderr,"=%p does not ",(void*)pParameters->path_elastic);
            fprintf(stderr,"point to a valid address.\n");
            return 0;
        }
        else
        {
            fileLocation=(char*)realloc(pParameters->path_elastic,
                                                       lengthName*sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"reallocate memory for the ");
                fprintf(stderr,"pParameters->path_elastic variable.\n");
                return 0;
            }
            pParameters->path_elastic=fileLocation;
        }

        // Update with the new length
        pParameters->path_length=boolean;
    }

    // Check if the medit software works well (only if it is going to be used)
    if (pParameters->save_print)
    {
        // Remove meditOutput.txt if the file already exists
        boolean=initialFileExists("meditOutput.txt",16);
        if (abs(boolean)!=1)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: initialFileExists ");
            fprintf(stderr,"function returned zero instead of (+/-) one, ");
            fprintf(stderr,"while attempting to check if the meditOutput.txt ");
            fprintf(stderr,"file already exists.\n");
            return 0;
        }
        else if (boolean==1)
        {
            if (remove("meditOutput.txt"))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: wrong return ");
                fprintf(stderr,"(=-1) of the standard remove c-function in ");
                fprintf(stderr,"the attempt of removing the meditOutput.txt ");
                fprintf(stderr,"file.\n");
                return 0;
            }
        }

        // Allocate memory for the fileLocation variable
        lengthName=pParameters->path_length+50;
        fileLocation=(char*)calloc(lengthName,sizeof(char));
        if (fileLocation==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: could not allocate ");
            fprintf(stderr,"memory for the local (char*) fileLocation ");
            fprintf(stderr,"variable.\n");
            return 0;
        }

        // Test the path for the medit software
        strncpy(fileLocation,"command -v ",lengthName);
        strcat(fileLocation,pParameters->path_medit);
        strcat(fileLocation," >meditOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s",fileLocation);
        }

        // system returns is -1 error, otherwise the return status of command
        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for medit software (=%s) ",pParameters->path_medit);
            fprintf(stderr,"does not seem to work.\nPlease check that the ");
            fprintf(stderr,"path placed after the 'path_medit' keyword is ");
            fprintf(stderr,"valid in the %s file, or ",pParameters->name_input);
            fprintf(stderr,"if no such line exists, add one if the default ");
            fprintf(stderr,"PATH_MEDIT preprocessor constant ");
            fprintf(stderr,"(=%s) in loadParameters.h file is not ",PATH_MEDIT);
            fprintf(stderr,"correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        strncpy(fileLocation,pParameters->path_medit,lengthName);
        strcat(fileLocation," -h | grep usage: -i >>meditOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s\n",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for medit software (=%s) ",pParameters->path_medit);
            fprintf(stderr,"does not seem to work.\nPlease check that the ");
            fprintf(stderr,"path placed after the 'path_medit' keyword is ");
            fprintf(stderr,"valid in the %s file, or ",pParameters->name_input);
            fprintf(stderr,"if no such line exists, add one if the default ");
            fprintf(stderr,"PATH_MEDIT preprocessor constant ");
            fprintf(stderr,"(=%s) in loadParameters.h file is not ",PATH_MEDIT);
            fprintf(stderr,"correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (remove("meditOutput.txt"))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard remove c-function in the attempt ");
            fprintf(stderr,"of removing the meditOutput.txt file.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Free the memory allocation for fileLocation
        free(fileLocation);
        fileLocation=NULL;
    }
    else
    {
        if (pParameters->path_medit==NULL)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: the variable ");
            fprintf(stdout,"path_medit=%p ",(void*)pParameters->path_medit);
            fprintf(stdout,"of the structure pointed by pParameters does not ");
            fprintf(stdout,"point to a valid address, whereas it should be ");
            fprintf(stdout,"set to its default value (=%s).\n\n",PATH_MEDIT);
        }
        else
        {
            if (strcmp(pParameters->path_medit,PATH_MEDIT))
            {
                if (pParameters->verbose>0)
                {
                    fprintf(stdout,"\nWarning in checkValuesOfAllParameters: ");
                    fprintf(stdout,"a path name for the medit software has ");
                    fprintf(stdout,"been loaded in the structure pointed by ");
                    fprintf(stdout,"pParameters although the current ");
                    fprintf(stdout,"vizualization mode ");
                    fprintf(stdout,"(save_print=%d) ",pParameters->save_print);
                    fprintf(stdout,"does not require such a software.\nThe ");
                    fprintf(stdout,"pParameters->path_medit variable is thus ");
                    fprintf(stdout,"restored to its default value ");
                    fprintf(stdout,"(=%s).\n\n",PATH_MEDIT);
                }
                strncpy(pParameters->path_medit,PATH_MEDIT,
                                                      pParameters->path_length);
            }
        }
    }

    // Check if the mmg3d and pParameters->path_mshdist softwares work well
    if (pParameters->opt_mode>0)
    {
        // Remove mmg3dOutput.txt if the file already exists
        boolean=initialFileExists("mmg3dOutput.txt",16);
        if (abs(boolean)!=1)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: initialFileExists ");
            fprintf(stderr,"function returned zero instead of (+/-) one, ");
            fprintf(stderr,"while attempting to check if the mmg3dOutput.txt ");
            fprintf(stderr,"file already exists.\n");
            return 0;
        }
        else if (boolean==1)
        {
            if (remove("mmg3dOutput.txt"))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: wrong return ");
                fprintf(stderr,"(=-1) of the standard remove c-function in ");
                fprintf(stderr,"the attempt of removing the mmg3dOutput.txt ");
                fprintf(stderr,"file.\n");
                return 0;
            }
        }

        // Remove mshdistOutput.txt if the file already exists
        boolean=initialFileExists("mshdistOutput.txt",18);
        if (abs(boolean)!=1)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: initialFileExists ");
            fprintf(stderr,"function returned zero instead of (+/-) one, ");
            fprintf(stderr,"while attempting to check if the ");
            fprintf(stderr,"mshdistOutput.txt file already exists.\n");
            return 0;
        }
        else if (boolean==1)
        {
            if (remove("mshdistOutput.txt"))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: wrong return ");
                fprintf(stderr,"(=-1) of the standard remove c-function in ");
                fprintf(stderr,"the attempt of removing the ");
                fprintf(stderr,"mshdistOutput.txt file.\n");
                return 0;
            }
        }

        // Allocate memory for the fileLocation variable
        lengthName=pParameters->path_length+50;
        fileLocation=(char*)calloc(lengthName,sizeof(char));
        if (fileLocation==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: could not allocate ");
            fprintf(stderr,"memory for the local (char*) fileLocation ");
            fprintf(stderr,"variable.\n");
            return 0;
        }

        // Test the path for the mmg3d software
        strncpy(fileLocation,"command -v ",lengthName);
        strcat(fileLocation,pParameters->path_mmg3d);
        strcat(fileLocation," >mmg3dOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for mmg3d software (=%s) ",pParameters->path_mmg3d);
            fprintf(stderr,"does not seem to work.\nPlease check that the ");
            fprintf(stderr,"path placed after the 'path_mmg3d' keyword is ");
            fprintf(stderr,"valid in the %s file, or ",pParameters->name_input);
            fprintf(stderr,"if no such line exists, add one if the default ");
            fprintf(stderr,"PATH_MMG3D preprocessor constant ");
            fprintf(stderr,"(=%s) in loadParameters.h file is not ",PATH_MMG3D);
            fprintf(stderr,"correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        strncpy(fileLocation,pParameters->path_mmg3d,lengthName);
        strcat(fileLocation," -h | grep usage: -i >>mmg3dOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s\n",fileLocation);
        }
        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for mmg3d software (=%s) ",pParameters->path_mmg3d);
            fprintf(stderr,"does not seem to work.\nPlease check that the ");
            fprintf(stderr,"path placed after the 'path_mmg3d' keyword is ");
            fprintf(stderr,"valid in the %s file, or ",pParameters->name_input);
            fprintf(stderr,"if no such line exists, add one if the default ");
            fprintf(stderr,"PATH_MMG3D preprocessor constant ");
            fprintf(stderr,"(=%s) in loadParameters.h file is not ",PATH_MMG3D);
            fprintf(stderr,"correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Test the path for the mshdist software
        strncpy(fileLocation,"command -v ",lengthName);
        strcat(fileLocation,pParameters->path_mshdist);
        strcat(fileLocation," >mshdistOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for the mshdist software ");
            fprintf(stderr,"(=%s) does not seem to ",pParameters->path_mshdist);
            fprintf(stderr,"work.\nPlease check that the path placed after ");
            fprintf(stderr,"the 'path_mshdist' keyword is valid in the ");
            fprintf(stderr,"%s file, or if no such ",pParameters->name_input);
            fprintf(stderr,"line exists, add one if the default PATH_MSHDIST ");
            fprintf(stderr,"preprocessor constant (=%s) in ",PATH_MSHDIST);
            fprintf(stderr,"loadParameters.h file is not correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        strncpy(fileLocation,pParameters->path_mshdist,lengthName);
        strcat(fileLocation," -? | grep usage: -i >>mshdistOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s\n",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for the mshdist software ");
            fprintf(stderr,"(=%s) does not seem to ",pParameters->path_mshdist);
            fprintf(stderr,"work.\nPlease check that the path placed after ");
            fprintf(stderr,"the 'path_mshdist' keyword is valid in the ");
            fprintf(stderr,"%s file, or if no such ",pParameters->name_input);
            fprintf(stderr,"line exists, add one if the default PATH_MSHDIST ");
            fprintf(stderr,"preprocessor constant (=%s) in ",PATH_MSHDIST);
            fprintf(stderr,"loadParameters.h file is not correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (remove("mmg3dOutput.txt"))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard remove c-function in the attempt ");
            fprintf(stderr,"of removing the mmg3dOutput.txt file.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (remove("mshdistOutput.txt"))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard remove c-function in the attempt ");
            fprintf(stderr,"of removing the mshdistOutput.txt file.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Free the memory allocation for fileLocation
        free(fileLocation);
        fileLocation=NULL;
    }
    else
    {
        if (pParameters->path_mmg3d==NULL)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: the variable ");
            fprintf(stdout,"path_mmg3d=%p ",(void*)pParameters->path_mmg3d);
            fprintf(stdout,"of the structure pointed by pParameters does not ");
            fprintf(stdout,"point to a valid address, whereas it should be ");
            fprintf(stdout,"set to its default value (=%s).\n\n",PATH_MMG3D);
        }
        else
        {
            if (strcmp(pParameters->path_mmg3d,PATH_MMG3D))
            {
                if (pParameters->verbose>0)
                {
                    fprintf(stdout,"\nWarning in checkValuesOfAllParameters: ");
                    fprintf(stdout,"a path name for the mmg3d software has ");
                    fprintf(stdout,"been loaded in the structure pointed by ");
                    fprintf(stdout,"pParameters although the current ");
                    fprintf(stdout,"optimization mode ");
                    fprintf(stdout,"(=%d) does not ",pParameters->opt_mode);
                    fprintf(stdout,"require such a software.\nThe ");
                    fprintf(stdout,"pParameters->path_mmg3d variable is thus ");
                    fprintf(stdout,"restored to its default value ");
                    fprintf(stdout,"(=%s).\n\n",PATH_MMG3D);
                }
                strncpy(pParameters->path_mmg3d,PATH_MMG3D,
                                                      pParameters->path_length);
            }
        }

        if (pParameters->path_mshdist==NULL)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: the variable ");
            fprintf(stdout,"path_mshdist=%p ",(void*)pParameters->path_mshdist);
            fprintf(stdout,"of the structure pointed by pParameters does not ");
            fprintf(stdout,"point to a valid address, whereas it should be ");
            fprintf(stdout,"set to its default value (=%s).\n\n",PATH_MSHDIST);
        }
        else
        {
            if (strcmp(pParameters->path_mshdist,PATH_MSHDIST))
            {
                if (pParameters->verbose>0)
                {
                    fprintf(stdout,"\nWarning in checkValuesOfAllParameters: ");
                    fprintf(stdout,"a path name for the mshdist software has ");
                    fprintf(stdout,"been loaded in the structure pointed by ");
                    fprintf(stdout,"pParameters although the current ");
                    fprintf(stdout,"optimization mode ");
                    fprintf(stdout,"(=%d) does not ",pParameters->opt_mode);
                    fprintf(stdout,"require such a software.\nThe ");
                    fprintf(stdout,"pParameters->path_mshdist variable is ");
                    fprintf(stdout,"thus restored to its default value ");
                    fprintf(stdout,"(=%s).\n\n",PATH_MSHDIST);
                }
                strncpy(pParameters->path_mshdist,PATH_MSHDIST,
                                                      pParameters->path_length);
            }
        }
    }

    // Check if pParameters->path_elastic and pParameters->path_advect work well
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        // Remove elasticOutput.txt if the file already exists
        boolean=initialFileExists("elasticOutput.txt",18);
        if (abs(boolean)!=1)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: initialFileExists ");
            fprintf(stderr,"function returned zero instead of (+/-) one, ");
            fprintf(stderr,"while attempting to check if the ");
            fprintf(stderr,"elasticOutput.txt file already exists.\n");
            return 0;
        }
        else if (boolean==1)
        {
            if (remove("elasticOutput.txt"))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: wrong return ");
                fprintf(stderr,"(=-1) of the standard remove c-function in ");
                fprintf(stderr,"the attempt of removing the ");
                fprintf(stderr,"elasticOutput.txt file.\n");
                return 0;
            }
        }

        // Remove advectOutput.txt if the file already exists
        boolean=initialFileExists("advectOutput.txt",17);
        if (abs(boolean)!=1)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: initialFileExists ");
            fprintf(stderr,"function returned zero instead of (+/-) one, ");
            fprintf(stderr,"while attempting to check if the ");
            fprintf(stderr,"advectOutput.txt file already exists.\n");
            return 0;
        }
        else if (boolean==1)
        {
            if (remove("advectOutput.txt"))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: wrong return ");
                fprintf(stderr,"(=-1) of the standard remove c-function in ");
                fprintf(stderr,"the attempt of removing the ");
                fprintf(stderr,"advectOutput.txt file.\n");
                return 0;
            }
        }

        // Allocate memory for the fileLocation variable
        lengthName=pParameters->path_length+50;
        fileLocation=(char*)calloc(lengthName,sizeof(char));
        if (fileLocation==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: could not allocate ");
            fprintf(stderr,"memory for the local (char*) fileLocation ");
            fprintf(stderr,"variable.\n");
            return 0;
        }

        // Test the path for the elastic software
        strncpy(fileLocation,"command -v ",lengthName);
        strcat(fileLocation,pParameters->path_elastic);
        strcat(fileLocation," >elasticOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for the elastic software ");
            fprintf(stderr,"(=%s) does not seem to ",pParameters->path_elastic);
            fprintf(stderr,"work.\nPlease check that the path placed after ");
            fprintf(stderr,"the 'path_elastic' keyword is valid in the ");
            fprintf(stderr,"%s file, or if no such ",pParameters->name_input);
            fprintf(stderr,"line exists, add one if the default PATH_ELASTIC ");
            fprintf(stderr,"preprocessor constant (=%s) in ",PATH_ELASTIC);
            fprintf(stderr,"loadParameters.h file is not correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        strncpy(fileLocation,pParameters->path_elastic,lengthName);
        strcat(fileLocation," -h | grep usage: -i >>elasticOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s\n",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for the elastic software ");
            fprintf(stderr,"(=%s) does not seem to ",pParameters->path_elastic);
            fprintf(stderr,"work.\nPlease check that the path placed after ");
            fprintf(stderr,"the 'path_elastic' keyword is valid in the ");
            fprintf(stderr,"%s file, or if no such ",pParameters->name_input);
            fprintf(stderr,"line exists, add one if the default PATH_ELASTIC ");
            fprintf(stderr,"preprocessor constant (=%s) in ",PATH_ELASTIC);
            fprintf(stderr,"loadParameters.h file is not correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Test the path for the advect software
        strncpy(fileLocation,"command -v ",lengthName);
        strcat(fileLocation,pParameters->path_advect);
        strcat(fileLocation," >advectOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for the advect software ");
            fprintf(stderr,"(=%s) does not seem to ",pParameters->path_advect);
            fprintf(stderr,"work.\nPlease check that the path placed after ");
            fprintf(stderr,"the 'path_advect' keyword is valid in the ");
            fprintf(stderr,"%s file, or if no such ",pParameters->name_input);
            fprintf(stderr,"line exists, add one if the default PATH_ADVECT ");
            fprintf(stderr,"preprocessor constant (=%s) in ",PATH_ADVECT);
            fprintf(stderr,"loadParameters.h file is not correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        strncpy(fileLocation,pParameters->path_advect,lengthName);
        strcat(fileLocation," -h | grep usage: -i >>advectOutput.txt 2>&1");
        if (pParameters->verbose>1)
        {
            fprintf(stdout,"\n%s\n",fileLocation);
        }

        if (system(fileLocation))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard system c-function. The path name ");
            fprintf(stderr,"for the advect software ");
            fprintf(stderr,"(=%s) does not seem to ",pParameters->path_advect);
            fprintf(stderr,"work.\nPlease check that the path placed after ");
            fprintf(stderr,"the 'path_advect' keyword is valid in the ");
            fprintf(stderr,"%s file, or if no such ",pParameters->name_input);
            fprintf(stderr,"line exists, add one if the default PATH_ADVECT ");
            fprintf(stderr,"preprocessor constant (=%s) in ",PATH_ADVECT);
            fprintf(stderr,"loadParameters.h file is not correct.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (remove("elasticOutput.txt"))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard remove c-function in the attempt ");
            fprintf(stderr,"of removing the elasticOutput.txt file.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (remove("advectOutput.txt"))
        {
            PRINT_ERROR("In checkValuesOfAllParameters: wrong return (=-1) ");
            fprintf(stderr,"of the standard remove c-function in the attempt ");
            fprintf(stderr,"of removing the advectOutput.txt file.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Free the memory allocation for fileLocation
        free(fileLocation);
        fileLocation=NULL;
    }
    else
    {
        if (pParameters->path_elastic==NULL)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: the variable ");
            fprintf(stdout,"path_elastic=%p ",(void*)pParameters->path_elastic);
            fprintf(stdout,"of the structure pointed by pParameters does not ");
            fprintf(stdout,"point to a valid address, whereas it should be ");
            fprintf(stdout,"set to its default value (=%s).\n\n",PATH_ELASTIC);
        }
        else
        {
            if (strcmp(pParameters->path_elastic,PATH_ELASTIC))
            {
                if (pParameters->verbose>0)
                {
                    fprintf(stdout,"\nWarning in checkValuesOfAllParameters: ");
                    fprintf(stdout,"a path name for the elastic software has ");
                    fprintf(stdout,"been loaded in the structure pointed by ");
                    fprintf(stdout,"pParameters although the current ");
                    fprintf(stdout,"optimization mode ");
                    fprintf(stdout,"(=%d) does not ",pParameters->opt_mode);
                    fprintf(stdout,"require such a software.\nThe ");
                    fprintf(stdout,"pParameters->path_elastic variable is ");
                    fprintf(stdout,"thus restored to its default value ");
                    fprintf(stdout,"(=%s).\n\n",PATH_ELASTIC);
                }
                strncpy(pParameters->path_elastic,PATH_ELASTIC,
                                                      pParameters->path_length);
            }
        }

        if (pParameters->path_advect==NULL)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: the variable ");
            fprintf(stdout,"path_advect=%p ",(void*)pParameters->path_advect);
            fprintf(stdout,"of the structure pointed by pParameters does not ");
            fprintf(stdout,"point to a valid address, whereas it should be ");
            fprintf(stdout,"set to its default value (=%s).\n\n",PATH_ADVECT);
        }
        else
        {
            if (strcmp(pParameters->path_advect,PATH_ADVECT))
            {
                if (pParameters->verbose>0)
                {
                    fprintf(stdout,"\nWarning in checkValuesOfAllParameters: ");
                    fprintf(stdout,"a path name for the advect software has ");
                    fprintf(stdout,"been loaded in the structure pointed by ");
                    fprintf(stdout,"pParameters although the current ");
                    fprintf(stdout,"optimization mode ");
                    fprintf(stdout,"(=%d) does not ",pParameters->opt_mode);
                    fprintf(stdout,"require such a software.\nThe ");
                    fprintf(stdout,"pParameters->path_advect variable is ");
                    fprintf(stdout,"thus restored to its default value ");
                    fprintf(stdout,"(=%s).\n\n",PATH_ADVECT);
                }
                strncpy(pParameters->path_advect,PATH_ADVECT,
                                                      pParameters->path_length);
            }
        }
    }

    // Check the *_iso and *_met variables related to the mmg3d parameters
    if (pParameters->opt_mode>0)
    {
        boolean=(pParameters->hmin_iso>0.);
        boolean=(boolean && pParameters->hmax_iso>pParameters->hmin_iso);
        boolean=(boolean && pParameters->hausd_iso>0.);
        boolean=(boolean && pParameters->hgrad_iso>1.);
        boolean=(boolean && pParameters->hmin_met>0.);
        boolean=(boolean && pParameters->hmax_met>pParameters->hmin_met);
        boolean=(boolean && pParameters->hausd_met>0.);
        boolean=(boolean && pParameters->hgrad_met>1.);
        boolean=(boolean && pParameters->memory>=0);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: expecting\n(");
            fprintf(stderr,"pParameters->hmin_iso=%lf) ",pParameters->hmin_iso);
            fprintf(stderr,"> 0.0\n(pParameters->hmax_iso=");
            fprintf(stderr,"%lf) > ",pParameters->hmax_iso);
            fprintf(stderr,"pParameters->hmin_iso\n(pParameters->hausd_iso=");
            fprintf(stderr,"%lf) > 0.0\n",pParameters->hausd_iso);
            fprintf(stderr,"(pParameters->hgrad_iso=");
            fprintf(stderr,"%lf) > 1.0\n(",pParameters->hgrad_iso);
            fprintf(stderr,"pParameters->hmin_met=%lf) ",pParameters->hmin_met);
            fprintf(stderr,"> 0.0\n(pParameters->hmax_met=");
            fprintf(stderr,"%lf) > ",pParameters->hmax_met);
            fprintf(stderr,"pParameters->hmin_met\n(pParameters->hausd_met=");
            fprintf(stderr,"%lf) > 0.0\n",pParameters->hausd_met);
            fprintf(stderr,"(pParameters->hgrad_met=");
            fprintf(stderr,"%lf) > 1.0\n",pParameters->hgrad_met);
            fprintf(stderr,"(pParameters->memory=%d) ",pParameters->memory);
            fprintf(stderr,">= 0\nPlease modify the values accordingly after ");
            fprintf(stderr,"the corresponding keywords in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
         }
    }
    else
    {
        boolean=(pParameters->hmin_iso==HMIN_ISO);
        boolean=(boolean && pParameters->hmax_iso==HMAX_ISO);
        boolean=(boolean && pParameters->hausd_iso==HAUSD_ISO);
        boolean=(boolean && pParameters->hgrad_iso==HGRAD_ISO);
        boolean=(boolean && pParameters->hmin_met==HMIN_MET);
        boolean=(boolean && pParameters->hmax_met==HMAX_MET);
        boolean=(boolean && pParameters->hausd_met==HAUSD_MET);
        boolean=(boolean && pParameters->hgrad_met==HGRAD_MET);
        boolean=(boolean && pParameters->memory==MEMORY);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: at least one value among the ");
                fprintf(stdout,"variables hmin_iso ");
                fprintf(stdout,"(=%lf), hmax_iso ",pParameters->hmin_iso);
                fprintf(stdout,"(=%lf), hausd_iso ",pParameters->hmax_iso);
                fprintf(stdout,"(=%lf), hgrad_iso ",pParameters->hausd_iso);
                fprintf(stdout,"(=%lf), hmin_met ",pParameters->hgrad_iso);
                fprintf(stdout,"(=%lf), hmax_met ",pParameters->hmin_met);
                fprintf(stdout,"(=%lf), hausd_met ",pParameters->hmax_met);
                fprintf(stdout,"(=%lf), hgrad_met ",pParameters->hgrad_met);
                fprintf(stdout,"(=%lf), and memory ",pParameters->hgrad_met);
                fprintf(stdout,"(=%d) of the structure ",pParameters->memory);
                fprintf(stdout,"pointed by pParameters has been changed from ");
                fprintf(stdout,"its default value ");
                fprintf(stdout,"(HMIN_ISO=%lf, ",HMIN_ISO);
                fprintf(stdout,"HMAX_ISO=%lf, ",HMAX_ISO);
                fprintf(stdout,"HAUSD_ISO=%lf, ",HAUSD_ISO);
                fprintf(stdout,"HGRAD_ISO=%lf, ",HGRAD_ISO);
                fprintf(stdout,"(HMIN_MET=%lf, ",HMIN_MET);
                fprintf(stdout,"HMAX_MET=%lf, ",HMAX_MET);
                fprintf(stdout,"HAUSD_MET=%lf, ",HAUSD_MET);
                fprintf(stdout,"HGRAD_MET=%lf, and ",HGRAD_MET);
                fprintf(stdout,"MEMORY=%d) in the ",MEMORY);
                fprintf(stdout,"%s file, althought ",pParameters->name_input);
                fprintf(stdout,"the current the current optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such parameters, since they are only needed ");
                fprintf(stdout,"for the mmg3d software, which is not used ");
                fprintf(stdout,"here.\nThe pParameters->hmin_iso, ");
                fprintf(stdout,"pParameters->hmax_iso, ");
                fprintf(stdout,"pParameters->hausd_iso, ");
                fprintf(stdout,"pParameters->hgrad_iso, ");
                fprintf(stdout,"pParameters->hmin_met, ");
                fprintf(stdout,"pParameters->hmax_met, ");
                fprintf(stdout,"pParameters->hausd_met, ");
                fprintf(stdout,"pParameters->hgrad_met and ");
                fprintf(stderr,"pParameters->memory variables are thus ");
                fprintf(stdout,"restored to their default values.\n\n");
            }
            pParameters->hmin_iso=HMIN_ISO;
            pParameters->hmax_iso=HMAX_ISO;
            pParameters->hausd_iso=HAUSD_ISO;
            pParameters->hgrad_iso=HGRAD_ISO;

            pParameters->hmin_met=HMIN_MET;
            pParameters->hmax_met=HMAX_MET;
            pParameters->hausd_met=HAUSD_MET;
            pParameters->hgrad_met=HGRAD_MET;

            pParameters->memory=MEMORY;
        }
    }

    // Check the *_ls variables related to the mmg3d parameters
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2 ||
                                                       pParameters->opt_mode==4)
    {
        boolean=(pParameters->hmin_ls>0.);
        boolean=(boolean && pParameters->hmax_ls>pParameters->hmin_ls);
        boolean=(boolean && pParameters->hausd_ls>0.);
        boolean=(boolean && pParameters->hgrad_ls>1.);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: expecting\n");
            fprintf(stderr,"(pParameters->hmin_ls=%lf) ",pParameters->hmin_ls);
            fprintf(stderr,"> 0.0\n");
            fprintf(stderr,"(pParameters->hmax_ls=%lf)",pParameters->hmax_ls);
            fprintf(stderr," > pParameters->hmin_ls\n(");
            fprintf(stderr,"pParameters->hausd_ls=%lf) ",pParameters->hausd_ls);
            fprintf(stderr," > 0.0\n(pParameters->hgrad_ls=");
            fprintf(stderr,"%lf) > 1.0\nPlease modify ",pParameters->hgrad_ls);
            fprintf(stderr,"the values accordingly after the corresponding ");
            fprintf(stderr,"keywords in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
         }
    }
    else
    {
        boolean=(pParameters->hmin_ls==HMIN_LS);
        boolean=(boolean && pParameters->hmax_ls==HMAX_LS);
        boolean=(boolean && pParameters->hausd_ls==HAUSD_LS);
        boolean=(boolean && pParameters->hgrad_ls==HGRAD_LS);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: at least one value among the ");
                fprintf(stdout,"variables hmin_ls ");
                fprintf(stdout,"(=%lf), hmax_ls ",pParameters->hmin_ls);
                fprintf(stdout,"(=%lf), hausd_ls ",pParameters->hmax_ls);
                fprintf(stdout,"(=%lf), and hgrad_ls ",pParameters->hausd_ls);
                fprintf(stdout,"(=%lf) of the ",pParameters->hgrad_ls);
                fprintf(stdout,"structure pointed by pParameters has been ");
                fprintf(stdout,"changed from its default value ");
                fprintf(stdout,"(HMIN_LS=%lf, ",HMIN_LS);
                fprintf(stdout,"HMAX_LS=%lf, ",HMAX_LS);
                fprintf(stdout,"HAUSD_LS=%lf, and ",HAUSD_LS);
                fprintf(stdout,"HGRAD_LS=%lf) in the ",HGRAD_LS);
                fprintf(stdout,"%s file, althought ",pParameters->name_input);
                fprintf(stdout,"the current the current optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such parameters, since they are only needed ");
                fprintf(stdout,"for the level-set option ('-ls') of the ");
                fprintf(stdout,"mmg3d software, which is not used here.\nThe ");
                fprintf(stdout,"pParameters->hmin_ls, pParameters->hmax_ls, ");
                fprintf(stdout,"pParameters->hausd_ls, and ");
                fprintf(stdout,"pParameters->hgrad_ls variables are thus ");
                fprintf(stdout,"restored to their default values.\n\n");
            }
            pParameters->hmin_ls=HMIN_LS;
            pParameters->hmax_ls=HMAX_LS;
            pParameters->hausd_ls=HAUSD_LS;
            pParameters->hgrad_ls=HGRAD_LS;
        }
    }

    // Check the *_lag variables related to the mmg3d parameters
    if (pParameters->opt_mode==1 || pParameters->opt_mode==3)
    {
        boolean=(pParameters->hmode_lag==0 || pParameters->hmode_lag==1 ||
                                                     pParameters->hmode_lag==2);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the hmode_lag ");
            fprintf(stderr,"variable (=%d) of the ",pParameters->hmode_lag);
            fprintf(stderr,"structure pointed by pParameters can only be set ");
            fprintf(stderr,"to 0, 1, or 2.\nPlease modify the value ");
            fprintf(stderr,"accordingly after the 'hmode_lag' keyword in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
        }

        boolean=(pParameters->hmin_lag>0.);
        boolean=(boolean && pParameters->hmax_lag>pParameters->hmin_lag);
        boolean=(boolean && pParameters->hausd_lag>0.);
        boolean=(boolean && pParameters->hgrad_lag>1.);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: expecting\n(");
            fprintf(stderr,"pParameters->hmin_lag=%lf) ",pParameters->hmin_lag);
            fprintf(stderr,"> 0.0\n(pParameters->hmax_lag=");
            fprintf(stderr,"%lf) ",pParameters->hmax_lag);
            fprintf(stderr,"> pParameters->hmin_lag\n(");
            fprintf(stderr,"pParameters->hausd_lag=%lf",pParameters->hausd_lag);
            fprintf(stderr,") > 0.0\n(pParameters->hgrad_lag=");
            fprintf(stderr,"%lf) > 1.0\nPlease modify ",pParameters->hgrad_lag);
            fprintf(stderr,"the values accordingly after the corresponding ");
            fprintf(stderr,"keywords in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
         }
    }
    else
    {
        boolean=(pParameters->hmode_lag==HMODE_LAG);
        boolean=(boolean && pParameters->hmin_lag==HMIN_LAG);
        boolean=(boolean && pParameters->hmax_lag==HMAX_LAG);
        boolean=(boolean && pParameters->hausd_lag==HAUSD_LAG);
        boolean=(boolean && pParameters->hgrad_lag==HGRAD_LAG);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: at least one value among the ");
                fprintf(stdout,"variables hmode_lag ");
                fprintf(stdout,"(=%d), hmin_lag ",pParameters->hmode_lag);
                fprintf(stdout,"(=%lf), hmax_lag ",pParameters->hmin_lag);
                fprintf(stdout,"(=%lf), hausd_lag ",pParameters->hmax_lag);
                fprintf(stdout,"(=%lf), and hgrad_lag ",pParameters->hausd_lag);
                fprintf(stdout,"(=%lf) of the ",pParameters->hgrad_lag);
                fprintf(stdout,"structure pointed by pParameters has been ");
                fprintf(stdout,"changed from its default value ");
                fprintf(stdout,"(HMODE_LAG=%d, ",HMODE_LAG);
                fprintf(stdout,"(HMIN_LAG=%lf, ",HMIN_LAG);
                fprintf(stdout,"HMAX_LAG=%lf, ",HMAX_LAG);
                fprintf(stdout,"HAUSD_LAG=%lf, and ",HAUSD_LAG);
                fprintf(stdout,"HGRAD_LAG=%lf) in the ",HGRAD_LAG);
                fprintf(stdout,"%s file, althought ",pParameters->name_input);
                fprintf(stdout,"the current the current optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such parameters, since they are only needed ");
                fprintf(stdout,"for the lagrangian option ('-lag') of the ");
                fprintf(stdout,"mmg3d software, which is not used here.\nThe ");
                fprintf(stdout,"pParameters->hmode_lag, ");
                fprintf(stdout,"pParameters->hmin_lag, ");
                fprintf(stdout,"pParameters->hmax_lag, ");
                fprintf(stdout,"pParameters->hausd_lag, and ");
                fprintf(stdout,"pParameters->hgrad_lag variables are thus ");
                fprintf(stdout,"restored to their default values.\n\n");
            }
            pParameters->hmode_lag=HMODE_LAG;
            pParameters->hmin_lag=HMIN_LAG;
            pParameters->hmax_lag=HMAX_LAG;
            pParameters->hausd_lag=HAUSD_LAG;
            pParameters->hgrad_lag=HGRAD_LAG;
        }
    }

    // Check the variables related to the mshdist parameters
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2 ||
                                                       pParameters->opt_mode==4)
    {
        boolean=(pParameters->n_iter>=0);
        boolean=(boolean && pParameters->residual>=0.);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: expecting\n");
            fprintf(stderr,"(pParameters->n_iter=%d) >= 0",pParameters->n_iter);
            fprintf(stderr,"\n(pParameters->residual");
            fprintf(stderr,"=%lf) >= 0.0\nPlease ",pParameters->residual);
            fprintf(stderr,"modify the values accordingly after the ");
            fprintf(stderr,"corresponding keywords in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
         }
    }
    else
    {
        boolean=(pParameters->n_iter==N_ITER);
        boolean=(boolean && pParameters->residual==RESIDUAL);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: at least one of the variables ");
                fprintf(stdout,"n_iter (=%d) or residual ",pParameters->n_iter);
                fprintf(stdout,"(=%lf) of the ",pParameters->residual);
                fprintf(stdout,"structure pointed by pParameters has been ");
                fprintf(stdout,"changed from its default value ");
                fprintf(stdout,"(N_ITER=%d and RESIDUAL=%lf) ",N_ITER,RESIDUAL);
                fprintf(stdout,"in the %s file, ",pParameters->name_input);
                fprintf(stdout,"althought the current the current ");
                fprintf(stdout,"optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such parameters, since they are only needed ");
                fprintf(stdout,"for the mshdist software, which is not used ");
                fprintf(stdout,"here.\nThe pParameters->n_iter and ");
                fprintf(stdout,"pParameters->residual variables are thus ");
                fprintf(stdout,"restored to their default values.\n\n");
            }
            pParameters->n_iter=N_ITER;
            pParameters->residual=RESIDUAL;
        }
    }

    // Check the variables related to the advect parameters
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        boolean=(pParameters->delta_t>0.0);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the delta_t ");
            fprintf(stderr,"(=%lf) variable of the ",pParameters->delta_t);
            fprintf(stderr,"structure pointed by pParameters must be ");
            fprintf(stderr,"positive.\nPlease modify the value accordingly ");
            fprintf(stderr,"after the 'delta_t' keyword in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
        }

        boolean=(pParameters->no_cfl==0 || pParameters->no_cfl==1);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the no_cfl ");
            fprintf(stderr,"(=%d) variable of the ",pParameters->no_cfl);
            fprintf(stderr,"structure pointed by pParameters can only be set ");
            fprintf(stderr,"to 0 or 1.\nPlease modify the value accordingly ");
            fprintf(stderr,"after the 'no_cfl' keyword in the ");
            fprintf(stderr,"%s file.\n",pParameters->name_input);
            return 0;
        }
    }
    else
    {
        boolean=(pParameters->delta_t==DELTA_T);
        boolean=(boolean && pParameters->no_cfl==NO_CFL);
        if (!boolean)
        {
            if (pParameters->verbose>0)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: at least one of the variables ");
                fprintf(stdout,"delta_t (=%lf) or ",pParameters->delta_t);
                fprintf(stdout,"no_cfl (=%d) of the ",pParameters->no_cfl);
                fprintf(stdout,"structure pointed by pParameters has been ");
                fprintf(stdout,"changed from its default value ");
                fprintf(stdout,"(DELTA_T=%lf and NO_CFL=%d) ",DELTA_T,NO_CFL);
                fprintf(stdout,"in the %s file, ",pParameters->name_input);
                fprintf(stdout,"althought the current the current ");
                fprintf(stdout,"optimization mode ");
                fprintf(stdout,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stdout,"such parameters, since they are only needed ");
                fprintf(stdout,"for the advect software, which is not used ");
                fprintf(stdout,"here.\nThe pParameters->delta_t and ");
                fprintf(stdout,"pParameters->no_cfl variables are thus ");
                fprintf(stdout,"restored to their default values.\n\n");
            }
            pParameters->delta_t=DELTA_T;
            pParameters->no_cfl=NO_CFL;
        }
    }

    if (pParameters->verbose>1)
    {
        fprintf(stdout,"All variables in the structure Parameters are ok.\n");
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The function writingRestartFile, depending on the opt_mode variable, writes
// an *.restart file that contains the default and prescribed values used to
// perform the mpd algorithm before starting the optimization loop. It has the
// same name than the *.info file where the '.info' extension has been replaced
// by the '.restart' one. It has the Parameters* variable (defined in main.h) as
// input argument and it returns one on success, otherwise zero is returned for
// an invalid value or an error
////////////////////////////////////////////////////////////////////////////////
int writingRestartFile(Parameters* pParameters)
{
    size_t lengthName=0;
    char *fileName=NULL;
    FILE *restartFile=NULL;

    // Testing if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In writingRestartFile: the input variable ");
        fprintf(stderr,"pParameters=%p does not point to ",(void*)pParameters);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check pParameters->name_input
    if (!checkInputFileName(pParameters->name_input,pParameters->name_length))
    {
        PRINT_ERROR("In writingRestartFile: checkInputFileName function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Allocate memory for the *.restart file name
    // calloc returns a pointer to the allocated memory, otherwise NULL
    lengthName=pParameters->name_length+2;
    fileName=(char*)calloc(lengthName,sizeof(char));
    if (fileName==NULL)
    {
        PRINT_ERROR("In writingRestartFile: could not allocate memory for ");
        fprintf(stderr,"the local (char*) fileName variable, that was ");
        fprintf(stderr,"intended to temporarily store the name of the ");
        fprintf(stderr,"*.restart file.\n");
        return 0;
    }

    // strncpy function returns a pointer to the string (not used here)
    strncpy(fileName,pParameters->name_input,lengthName);
    lengthName=strlen(fileName);
    fileName[lengthName-6]='.';
    fileName[lengthName-5]='r';
    fileName[lengthName-4]='e';
    fileName[lengthName-3]='s';
    fileName[lengthName-2]='t';
    fileName[lengthName-1]='a';
    fileName[lengthName]='r';
    fileName[lengthName+1]='t';
    fileName[lengthName+2]='\0';
    lengthName=strlen(fileName);

    // Create and open the *.restart file (warning: overwrite file if it already
    // exists). fopen function returns a FILE pointer on success, otherwise NULL
    if (pParameters->verbose>0)
    {
        fprintf(stdout,"\nOpening %s file. ",fileName);
    }
    restartFile=fopen(fileName,"w+");
    if (restartFile==NULL)
    {
        PRINT_ERROR("In writingRestartFile: we were not able to write into ");
        fprintf(stderr,"the %s file.\n",fileName);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    if (pParameters->verbose>0)
    {
        fprintf(stdout,"Writing restart parameters. ");
    }

    // Write general parameters
    fprintf(restartFile,"\n## General parameters\n\n");
    fprintf(restartFile,"opt_mode %d \n",pParameters->opt_mode);
    fprintf(restartFile,"verbose %d \n",pParameters->verbose);
#ifdef _OPENMP
    fprintf(restartFile,"n_cpu %d \n",pParameters->n_cpu);
#endif
    if (pParameters->opt_mode>0)
    {
        fprintf(restartFile,"rho_opt %.8le \n",pParameters->rho_opt);
    }

    fprintf(restartFile,"\n## File name parameters\n\n");
    fprintf(restartFile,"name_length %d \n",pParameters->name_length);

    // Check and write the pParameters->name_result variable
    if (!checkStringFromLength(pParameters->name_result,9,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingRestartFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value ");
        fprintf(stderr,"here, after having checked that the (char*) ");
        fprintf(stderr,"name_result variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more ",pParameters->name_length);
        fprintf(stderr,"than 7 in order to store at least something more ");
        fprintf(stderr,"than the *.result extension).\n");
        fprintf(restartFile,"end_data\n");
        closeTheFile(&restartFile,pParameters->verbose);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(restartFile,"name_result %s \n",pParameters->name_result);

    // Check and write the pParameters->name_chem variable
    if (!checkStringFromLength(pParameters->name_chem,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingRestartFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the (char*) name_chem ");
        fprintf(stderr,"variable of the structure pointed by pParameters is ");
        fprintf(stderr,"not a string of length (strictly) less than ");
        fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
        fprintf(stderr,"order to store at least something more than the ");
        fprintf(stderr,"*.chem or *.wfn extension).\n");
        fprintf(restartFile,"end_data\n");
        closeTheFile(&restartFile,pParameters->verbose);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(restartFile,"name_chem %s \n",pParameters->name_chem);

    // Check and write the pParameters->name_mesh variable
    if (!checkStringFromLength(pParameters->name_mesh,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingRestartFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value ");
        fprintf(stderr,"here, after having checked that the (char*) ");
        fprintf(stderr,"name_mesh variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more ",pParameters->name_length);
        fprintf(stderr,"than 5 in order to store at least something more ");
        fprintf(stderr,"than the *.mesh or *.cube extension).\n");
        fprintf(restartFile,"end_data\n");
        closeTheFile(&restartFile,pParameters->verbose);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(restartFile,"name_mesh %s \n",pParameters->name_mesh);

    // Check and write the pParameters->name_elas variable if opt_mode=1/2
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        if (!checkStringFromLength(pParameters->name_elas,7,
                                                      pParameters->name_length))
        {
            PRINT_ERROR("In writingRestartFile: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked ");
            fprintf(stderr,"that the (char*) name_elas variable of the ");
            fprintf(stderr,"structure pointed by pParameters is not a ");
            fprintf(stderr,"string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 ",pParameters->name_length);
            fprintf(stderr,"in order to store at least something more ");
            fprintf(stderr,"than the *.elas extension).\n");
            fprintf(restartFile,"end_data\n");
            closeTheFile(&restartFile,pParameters->verbose);
            free(fileName);
            fileName=NULL;
            return 0;
        }
        fprintf(restartFile,"name_elas %s \n",pParameters->name_elas);
    }

    // Write chemical parameters
    fprintf(restartFile,"\n## Chemical parameters\n\n");
    fprintf(restartFile,"nu_electrons %d \n",pParameters->nu_electrons);
    fprintf(restartFile,"bohr_unit %d \n",pParameters->bohr_unit);
    fprintf(restartFile,"select_orb %.8le \n",pParameters->select_orb);
    fprintf(restartFile,"orb_ortho %d \n",pParameters->orb_ortho);
    fprintf(restartFile,"ne_electrons %d \n",pParameters->ne_electrons);
    fprintf(restartFile,"multi_det %d \n",pParameters->multi_det);
    fprintf(restartFile,"orb_rhf %d \n\n",pParameters->orb_rhf);

    // Write the variables related to the computational box
    fprintf(restartFile,"## Computational box parameters\n\n");
    fprintf(restartFile,"select_box %.8le \n",pParameters->select_box);
    fprintf(restartFile,"x_min %.8le \n",pParameters->x_min);
    fprintf(restartFile,"y_min %.8le \n",pParameters->y_min);
    fprintf(restartFile,"z_min %.8le \n\n",pParameters->z_min);

    fprintf(restartFile,"x_max %.8le \n",pParameters->x_max);
    fprintf(restartFile,"y_max %.8le \n",pParameters->y_max);
    fprintf(restartFile,"z_max %.8le \n\n",pParameters->z_max);

    fprintf(restartFile,"n_x %d \n",pParameters->n_x);
    fprintf(restartFile,"n_y %d \n",pParameters->n_y);
    fprintf(restartFile,"n_z %d \n\n",pParameters->n_z);

    fprintf(restartFile,"delta_x %.8le \n",pParameters->delta_x);
    fprintf(restartFile,"delta_y %.8le \n",pParameters->delta_y);
    fprintf(restartFile,"delta_z %.8le \n\n",pParameters->delta_z);

    fprintf(restartFile,"## Initial domain parameters\n\n");
    fprintf(restartFile,"ls_ini %d \n",pParameters->ls_ini);
    fprintf(restartFile,"ls_type %d \n",pParameters->ls_type);
    fprintf(restartFile,"ls_x %.8le \n",pParameters->ls_x);
    fprintf(restartFile,"ls_y %.8le \n",pParameters->ls_y);
    fprintf(restartFile,"ls_z %.8le \n\n",pParameters->ls_z);

    fprintf(restartFile,"ls_rx %.8le \n",pParameters->ls_rx);
    fprintf(restartFile,"ls_ry %.8le \n",pParameters->ls_ry);
    fprintf(restartFile,"ls_rz %.8le \n\n",pParameters->ls_rz);

    // Write variables related to the metric or matrix computation
    if (pParameters->opt_mode>0)
    {
        fprintf(restartFile,"## Metric computation parameters\n\n");
        fprintf(restartFile,"met_err %.8le \n",pParameters->met_err);
        fprintf(restartFile,"met_min %.8le \n",pParameters->met_min);
        fprintf(restartFile,"met_max %.8le \n\n",pParameters->met_max);
    }
    else
    {
        fprintf(restartFile,"## Matrix computation parameters\n\n");
        fprintf(restartFile,"trick_matrix %d \n",pParameters->trick_matrix);
        fprintf(restartFile,"approx_mode %d \n\n",pParameters->approx_mode);
    }

    // Write variables related to the stop criteria
    fprintf(restartFile,"## Stopping criteria parameters\n\n");
    fprintf(restartFile,"iter_ini %d \n",pParameters->iter_ini);
    fprintf(restartFile,"iter_max %d \n",pParameters->iter_max);
    fprintf(restartFile,"iter_told0p %.8le \n",pParameters->iter_told0p);
    fprintf(restartFile,"iter_told1p %.8le \n",pParameters->iter_told1p);
    fprintf(restartFile,"iter_told2p %.8le \n\n",pParameters->iter_told2p);

    // Write variables related to the saving of data
    fprintf(restartFile,"## Saving data parameters\n\n");
    fprintf(restartFile,"save_type %d \n",pParameters->save_type);
    fprintf(restartFile,"save_mesh %d \n",pParameters->save_mesh);
    fprintf(restartFile,"save_data %d \n",pParameters->save_data);
    fprintf(restartFile,"save_print %d \n",pParameters->save_print);
    fprintf(restartFile,"save_where %d \n\n",pParameters->save_where);

    fprintf(restartFile,"## Path name parameters\n\n");
    fprintf(restartFile,"path_length %d \n",pParameters->path_length);

    if (pParameters->save_print)
    {
        // Check and write the pParameters->path_medit variable
        if (!checkStringFromLength(pParameters->path_medit,2,
                                                      pParameters->path_length))
        {
            PRINT_ERROR("In writingRestartFile: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the (char*) path_medit variable of the structure ");
            fprintf(stderr,"pointed by pParameters is not a string with the ");
            fprintf(stderr,"correct length.\n");
            fprintf(restartFile,"end_data\n");
            closeTheFile(&restartFile,pParameters->verbose);
            free(fileName);
            fileName=NULL;
            return 0;
        }
        fprintf(restartFile,"path_medit %s \n",pParameters->path_medit);
    }

    if (pParameters->opt_mode>0)
    {
        // Check and write the pParameters->path_mmg3d variable
        if (!checkStringFromLength(pParameters->path_mmg3d,2,
                                                      pParameters->path_length))
        {
            PRINT_ERROR("In writingRestartFile: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the (char*) path_mmg3d variable of the structure ");
            fprintf(stderr,"pointed by pParameters is not a string with the ");
            fprintf(stderr,"correct length.\n");
            fprintf(restartFile,"end_data\n");
            closeTheFile(&restartFile,pParameters->verbose);
            free(fileName);
            fileName=NULL;
            return 0;
        }
        fprintf(restartFile,"path_mmg3d %s \n",pParameters->path_mmg3d);

        // Check and write the pParameters->path_mshdist variable
        if (!checkStringFromLength(pParameters->path_mshdist,2,
                                                      pParameters->path_length))
        {
            PRINT_ERROR("In writingRestartFile: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked ");
            fprintf(stderr,"that the (char*) path_mshdist variable of the ");
            fprintf(stderr,"structure pointed by pParameters is not a string ");
            fprintf(stderr,"with the correct length.\n");
            fprintf(restartFile,"end_data\n");
            closeTheFile(&restartFile,pParameters->verbose);
            free(fileName);
            fileName=NULL;
            return 0;
        }
        fprintf(restartFile,"path_mshdist %s \n",pParameters->path_mshdist);

        if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
        {
            // Check and write the pParameters->path_elastic variable
            if (!checkStringFromLength(pParameters->path_elastic,2,
                                                      pParameters->path_length))
            {
                PRINT_ERROR("In writingRestartFile: checkStringFromLength ");
                fprintf(stderr,"function returned zero, which is not the ");
                fprintf(stderr,"expected value here, after having checked ");
                fprintf(stderr,"that the (char*) path_elastic variable of ");
                fprintf(stderr,"the structure pointed by pParameters is not ");
                fprintf(stderr,"a string with the correct length.\n");
                fprintf(restartFile,"end_data\n");
                closeTheFile(&restartFile,pParameters->verbose);
                free(fileName);
                fileName=NULL;
                return 0;
            }
            fprintf(restartFile,"path_elastic %s \n",pParameters->path_elastic);

            // Check and write the pParameters->path_advect variable
            if (!checkStringFromLength(pParameters->path_advect,2,
                                                      pParameters->path_length))
            {
                PRINT_ERROR("In writingRestartFile: checkStringFromLength ");
                fprintf(stderr,"function returned zero, which is not the ");
                fprintf(stderr,"expected value here, after having checked ");
                fprintf(stderr,"that the (char*) path_advect variable of the ");
                fprintf(stderr,"the structure pointed by pParameters is not ");
                fprintf(stderr,"a string with the correct length.\n");
                fprintf(restartFile,"end_data\n");
                closeTheFile(&restartFile,pParameters->verbose);
                free(fileName);
                fileName=NULL;
                return 0;
            }
            fprintf(restartFile,"path_advect %s \n",pParameters->path_advect);
        }

        fprintf(restartFile,"\n## Mmg3d software parameters\n\n");
        fprintf(restartFile,"memory %d \n",pParameters->memory);

        // Write the *_iso and *_met variables related to the mmg3d parameters
        fprintf(restartFile,"hmin_iso %.8le \n",pParameters->hmin_iso);
        fprintf(restartFile,"hmax_iso %.8le \n",pParameters->hmax_iso);
        fprintf(restartFile,"hausd_iso %.8le \n",pParameters->hausd_iso);
        fprintf(restartFile,"hgrad_iso %.8le \n\n",pParameters->hgrad_iso);

        fprintf(restartFile,"hmin_met %.8le \n",pParameters->hmin_met);
        fprintf(restartFile,"hmax_met %.8le \n",pParameters->hmax_met);
        fprintf(restartFile,"hausd_met %.8le \n",pParameters->hausd_met);
        fprintf(restartFile,"hgrad_met %.8le \n\n",pParameters->hgrad_met);

        if (pParameters->opt_mode!=3)
        {
            // Write the *_ls variables related to the mmg3d parameters
            fprintf(restartFile,"hmin_ls %.8le \n",pParameters->hmin_ls);
            fprintf(restartFile,"hmax_ls %.8le \n",pParameters->hmax_ls);
            fprintf(restartFile,"hausd_ls %.8le \n",pParameters->hausd_ls);
            fprintf(restartFile,"hgrad_ls %.8le \n\n",pParameters->hgrad_ls);
        }

        if (pParameters->opt_mode==1 || pParameters->opt_mode==3)
        {
            // Write the *_lag variables related to the mmg3d parameters
            fprintf(restartFile,"hmode_lag %d \n",pParameters->hmode_lag);
            fprintf(restartFile,"hmin_lag %.8le \n",pParameters->hmin_lag);
            fprintf(restartFile,"hmax_lag %.8le \n",pParameters->hmax_lag);
            fprintf(restartFile,"hausd_lag %.8le \n",pParameters->hausd_lag);
            fprintf(restartFile,"hgrad_lag %.8le \n\n",pParameters->hgrad_lag);
        }

        if (pParameters->opt_mode!=3)
        {
            // Check the variables related to the mshdist parameters
            fprintf(restartFile,"## Mshdist software parameters\n\n");
            fprintf(restartFile,"n_iter %d \n",pParameters->n_iter);
            fprintf(restartFile,"residual %.8le \n",pParameters->residual);
        }

        if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
        {
            // Check the variables related to the advect parameters
            fprintf(restartFile,"\n## Advect software parameters\n\n");
            fprintf(restartFile,"delta_t %.8le \n",pParameters->delta_t);
            fprintf(restartFile,"no_cfl %d \n",pParameters->no_cfl);
        }
    }
    fprintf(restartFile,"\nend_data\n\nThe remaining part of this file ");
    fprintf(restartFile,"is considered as comments\n\n");

    // Closing the *.restart file: fclose returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(restartFile))
    {
        PRINT_ERROR("In writingRestartFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileName);
        restartFile=NULL;
        return 0;
    }
    if (pParameters->verbose)
    {
        fprintf(stdout,"Closing file.\n");
    }
    restartFile=NULL;

    // Free the memory allocated for fileName
    free(fileName);
    fileName=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The function loadParameters initializes all the variables of the structure
// pointed by pParameters thanks to the values given in an *.input file pointed
// by nameInputFile. The variables of the structure Parameters that are not
// specified in the *.input file are filled with some default values (given by
// the preprocessor constants defined in loadParameters.h file). However, the
// *.input file must contain at least (minimal configuration) the *.wfn/ *.chem
// (chemical) file, preceded by the 'name_chem' keyword, the (positive) number
// of electrons to look for, preceded by the 'nu_electrons' keyword, and
// optionally the *.mesh/ *.cube (mesh) file to start with, preceded by the
// 'name_mesh' keyword (if not specify a cube/cuboid or a sphere/cigar is built
// depending on the ls_type variable of the structure Parameters). Such file
// must also end with the 'end_data' keyword. The function loadParameters has
// the Parameters* variable (defined in main.h) and a char* variable as input
// arguments and it returns one on success, otherwise zero is returned on error
////////////////////////////////////////////////////////////////////////////////
int loadParameters(Parameters* pParameters, char* nameInputFile)
{
    // Initializing the structure pParameters to zero
    initializeParameterStructure(pParameters);

    // Set up the default parameters
    if (!setupDefaultParameters(pParameters,nameInputFile))
    {
        PRINT_ERROR("In loadParameters: setupDefaultParameters ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

   // Read and update the parameters contained in the *.input file
   if (!readInputFileAndGetParameters(pParameters))
   {
       PRINT_ERROR("In loadParameters: readInputFileAndGetParameters ");
       fprintf(stderr,"function returned zero instead of one.\n");
       return 0;
   }

   // Check values of all parameters
   if (!checkValuesOfAllParameters(pParameters))
   {
       PRINT_ERROR("In loadParameters: checkValuesOfAllParameters function ");
       fprintf(stderr,"returned zero instead of one.\n");
       return 0;
   }

   return 1;
}

