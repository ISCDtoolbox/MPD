/**
* \file loadParameters.c
* \brief It contains all the functions used to load properly the parameters
*        from a *.info file (and use the default values if they are not
*        specified) in the MPD algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* The main function of this file is called \ref loadParameters and many other
* functions should be static but have been defined as non-static for performing
* unit-tests on them.
*/

#include "loadParameters.h"

/* ************************************************************************** */
// The function initializeParameterStructure sets to zero all the variables of
// the structure Parameters (and pointers to NULL). It has the Parameters*
// variable (defined in main.h) as input argument and it does not return value
/* ************************************************************************** */
void initializeParameterStructure(Parameters* pParameters)
{
    // 70 parameters
    if (pParameters!=NULL)
    {
        pParameters->opt_mode=0;
        pParameters->verbose=0;
        pParameters->n_cpu=0;

        pParameters->name_length=0;
        pParameters->name_info=NULL;
        pParameters->name_data=NULL;
        pParameters->name_chem=NULL;
        pParameters->name_mesh=NULL;
        pParameters->name_elas=NULL;

        pParameters->nu_electrons=0;
        pParameters->nu_spin=0;
        pParameters->orb_rhf=0;

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

        pParameters->ls_type=0;
        pParameters->ls_x=0.;
        pParameters->ls_y=0.;
        pParameters->ls_z=0.;
        pParameters->ls_r=0.;

        pParameters->met_err=0.;
        pParameters->met_min=0.;
        pParameters->met_max=0.;

        pParameters->trick_matrix=0;
        pParameters->approx_mode=0;

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

        pParameters->n_iter=0;
        pParameters->residual=0.;
        pParameters->delta_t=0.;
        pParameters->no_cfl=0;
    }

    return;
}

/* ************************************************************************** */
// The function freeParameterMemory frees the memory dynamically allocated with
// calloc/malloc/realloc for the structure Parameters (but other variables than
// pointers are not reset to zero). It has the Parameters* variable (defined in
// main.h) as input argument and it does not return any value
/* ************************************************************************** */
void freeParameterMemory(Parameters* pParameters)
{
    if (pParameters!=NULL)
    {
        // The free function does not return any value (void output)
        free(pParameters->name_info);
        pParameters->name_info=NULL;

        free(pParameters->name_data);
        pParameters->name_data=NULL;

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

/* ************************************************************************** */
// The function setupDefaultParameters fills all the variables of the structure
// pointed by pParameters to their default values (given by the preprocessor
// constants of loadParameters.h). The nameInfo variable, which should contain
// the name of the *.info (input) file, is stored in the name_info variable of
// the structure Parameters. The function has the Parameters* variable (defined
// in main.h) and the char* nameInfo variable as input arguments and it returns
// zero if an error occurred, otherwise one is returned in case of success
/* ************************************************************************** */
int setupDefaultParameters(Parameters* pParameters, char* nameInfo)
{
    // Check if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: the pParameters ");
        fprintf(stderr,"(input) variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pParameters);
        return 0;
    }

    // Check if the nameInfo is a string of length less than NAME_LENGTH
    if (!checkStringFromLength(nameInfo,7,NAME_LENGTH))
    {
        PRINT_ERROR("In setupDefaultParameters: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the input ");
        fprintf(stderr,"char* variable called nameInfo, which was supposed ");
        fprintf(stderr,"to store the name of the (input) *.info file, is not ");
        fprintf(stderr,"a string of length strictly less than %d ",NAME_LENGTH);
        fprintf(stderr,"(and more than 5 to contain at least something more ");
        fprintf(stderr,"than the *.info extension).\n");
        return 0;
    }

    // Setting up the default values to the variables
    pParameters->opt_mode=OPT_MODE;
    pParameters->verbose=VERBOSE;
    pParameters->n_cpu=N_CPU;
    pParameters->name_length=NAME_LENGTH;

    // calloc function returns a pointer to the allocated memory, otherwise NULL
    pParameters->name_info=(char*)calloc(NAME_LENGTH,sizeof(char));
    if (pParameters->name_info==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the char* pParameters->name_info variable.\n");
        return 0;
    }

    // strncpy function returns a pointer to the string (not used here)
    strncpy(pParameters->name_info,nameInfo,NAME_LENGTH);
    fprintf(stdout,"\nDefault values for parameters will be changed from ");
    fprintf(stdout,"%s file.",pParameters->name_info);

    pParameters->name_data=NULL;
    pParameters->name_chem=NULL;
    pParameters->name_mesh=NULL;
    pParameters->name_elas=NULL;

    pParameters->nu_electrons=0;
    pParameters->nu_spin=0;
    pParameters->orb_rhf=0;

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

    pParameters->ls_type=LS_TYPE;
    pParameters->ls_x=LS_X;
    pParameters->ls_y=LS_Y;
    pParameters->ls_z=LS_Z;
    pParameters->ls_r=LS_R;

    pParameters->met_err=MET_ERR;
    pParameters->met_min=MET_MIN;
    pParameters->met_max=MET_MAX;

    pParameters->trick_matrix=TRICK_MATRIX;
    pParameters->approx_mode=APPROX_MODE;

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
    pParameters->path_medit=(char*)calloc(PATH_LENGTH,sizeof(char));
    if (pParameters->path_medit==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the char* pParameters->path_medit variable.\n");
        return 0;
    }
    strncpy(pParameters->path_medit,PATH_MEDIT,PATH_LENGTH);

    pParameters->path_mmg3d=(char*)calloc(PATH_LENGTH,sizeof(char));
    if (pParameters->path_mmg3d==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the char* pParameters->path_mmg3d variable.\n");
        return 0;
    }
    strncpy(pParameters->path_mmg3d,PATH_MMG3D,PATH_LENGTH);

    pParameters->path_mshdist=(char*)calloc(PATH_LENGTH,sizeof(char));
    if (pParameters->path_mshdist==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the char* pParameters->path_mshdist variable.\n");
        return 0;
    }
    strncpy(pParameters->path_mshdist,PATH_MSHDIST,PATH_LENGTH);

    pParameters->path_elastic=(char*)calloc(PATH_LENGTH,sizeof(char));
    if (pParameters->path_elastic==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the char* pParameters->path_elastic variable.\n");
        return 0;
    }
    strncpy(pParameters->path_elastic,PATH_ELASTIC,PATH_LENGTH);

    pParameters->path_advect=(char*)calloc(PATH_LENGTH,sizeof(char));
    if (pParameters->path_advect==NULL)
    {
        PRINT_ERROR("In setupDefaultParameters: could not allocate memory ");
        fprintf(stderr,"for the char* pParameters->path_advect variable.\n");
        return 0;
    }
    strncpy(pParameters->path_advect,PATH_ADVECT,PATH_LENGTH);

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

    pParameters->n_iter=N_ITER;
    pParameters->residual=RESIDUAL;
    pParameters->delta_t=DELTA_T;
    pParameters->no_cfl=NO_CFL;

    return 1;
}

/* ************************************************************************** */
// The function getLengthAfterKeywordBeginning returns the number of characters
// to read at line counter (including the terminating nul one '\0') depending
// on the different possibilities for keywordBeginning. It has a char[3]
// keywordBeginning variable and an int counter variable as input arguments and
// it returns an integer between two and nine, or eleven, otherwise zero is
// returned if an error is encountered
/* ************************************************************************** */
int getLengthAfterKeywordBeginning(char keywordBeginning[3], int counter)
{
    int returnValue=0;

    // Check that counter is between 1 and 70 (total number of keywords)
    if (counter<1 || counter>70)
    {
        PRINT_ERROR("In getLengthAfterKeywordBeginning: the input variable ");
        fprintf(stderr,"counter, corresponding to the %d-th keyword ",counter);
        fprintf(stderr,"read, should be an integer between one and seventy ");
        fprintf(stderr,"(the total number of different possible keywords).\n");
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
    else if (!strcmp(keywordBeginning,"no"))
    {
       returnValue=5;
    }
    else if (!strcmp(keywordBeginning,"ve") || !strcmp(keywordBeginning,"nu") ||
             !strcmp(keywordBeginning,"or") || !strcmp(keywordBeginning,"de") ||
               !strcmp(keywordBeginning,"me") || !strcmp(keywordBeginning,"hm"))
    {
       returnValue=6;
    }
    else if (!strcmp(keywordBeginning,"op") || !strcmp(keywordBeginning,"en") ||
             !strcmp(keywordBeginning,"it") || !strcmp(keywordBeginning,"ha") ||
               !strcmp(keywordBeginning,"hg") || !strcmp(keywordBeginning,"re"))
    {
       returnValue=7;
    }
    else if (!strcmp(keywordBeginning,"na") || !strcmp(keywordBeginning,"sa"))
    {
       returnValue=8;
    }
    else if (!strcmp(keywordBeginning,"pa"))
    {
       returnValue=9;
    }
    else if (!strcmp(keywordBeginning,"ap"))
    {
       returnValue=10;
    }
    else if (!strcmp(keywordBeginning,"tr"))
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

/* ************************************************************************** */
// The function getTypeAfterKeyword, depending on the different cases for
// keywordMiddle (a string of length lengthMiddle including '\0') returns the
// type (-1= no type (end_data keyword), 1=integer, 2=double, 3=string) of
// variables to be read after by fscanf at line counter. It has the char[11]
// keywordMiddle variable, and two int variables (lengthMiddle and counter) as
// input arguments and returns -1, 1, 2, or 3, otherwise zero in case of error
/* ************************************************************************** */
int getTypeAfterKeyword(char keywordMiddle[11], int lengthMiddle, int counter)
{
    int returnValue=0, boolean1=0, boolean2=0, boolean3=0;

    // Check that counter is between 1 and 70 (total number of keywords)
    if (counter<1 || counter>70)
    {
        PRINT_ERROR("In getTypeAfterKeyword: the input variable counter, ");
        fprintf(stderr,"corresponding to the %d-th keyword read, ",counter);
        fprintf(stderr,"should be an integer between one and and seventy ");
        fprintf(stderr,"(the total number of different possible keywords).\n");
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
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_elec"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_spin"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"b_rhf"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"x"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"y"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"z"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_t"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ick_matrix"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"prox_mode"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"er_max"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_type"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_mesh"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_data"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_prin"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ve_wher"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"th_lengt"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ode_l"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"i"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_cfl"));

    boolean2=(!strcmp(keywordMiddle,"min"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"max"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lta_x"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lta_y"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"lta_z"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_x"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_y"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_z"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"_r"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_err"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_min"));
    boolean2=(boolean2 || !strcmp(keywordMiddle,"t_max"));
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


    boolean3=(!strcmp(keywordMiddle,"me_data"));
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

/* ************************************************************************** */
// The function getLengthAfterKeywordMiddle, depending on the different cases
// for keywordMiddle (a string of length lengthMiddle including '\0') evaluates
// the number of characters (including '\0') to be read after keywordMiddle in
// order to complete the reading of the (counter)-th keyword. It has a char[11]
// keywordMiddle, and two int variables (lengthMiddle and counter) as input
// arguments and it returns 1, 2, 3, or 4, otherwise zero in case of error
/* ************************************************************************** */
int getLengthAfterKeywordMiddle(char keywordMiddle[11], int lengthMiddle,
                                                                    int counter)
{
    int returnValue=0, boolean1=0, boolean2=0, boolean3=0, boolean4=0;

    // Check that counter is between 1 and 70 (total number of keywords)
    if (counter<1 || counter>70)
    {
        PRINT_ERROR("In getLengthAfterKeywordMiddle: the input variable ");
        fprintf(stderr,"counter, corresponding to the %d-th keyword ",counter);
        fprintf(stderr,"read, should be an integer between one and seventy ");
        fprintf(stderr,"(the total number of different possible keywords).\n");
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
    boolean1=(boolean1 || !strcmp(keywordMiddle,"d_data"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_data"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_chem"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_mesh"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"me_elas"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_spin"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"b_rhf"));
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
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_r"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"t_err"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"t_max"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"t_min"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"ick_matrix"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"prox_mode"));
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
    boolean1=(boolean1 || !strcmp(keywordMiddle,"sidual"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"lta_t"));
    boolean1=(boolean1 || !strcmp(keywordMiddle,"_cfl"));

    boolean2=(!strcmp(keywordMiddle,"ve_prin"));
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
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_mshdi"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"th_elast"));
    boolean3=(boolean3 || !strcmp(keywordMiddle,"ode_l"));

    boolean4=(!strcmp(keywordMiddle,"_t"));
    boolean4=(boolean4 || !strcmp(keywordMiddle,"er_tol"));
    boolean4=(boolean4 || !strcmp(keywordMiddle,"i"));

    if (!strcmp(keywordMiddle,"_elec"))
    {
        returnValue=6;
    }
    else if (boolean4)
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

/* ************************************************************************** */
// The function detectRepetition simply adds one in the array repetition at the
// location corresponding to the (counter)-th keyword in the *.info file read as
// the concatenation of the strings keywordBeginning+keywordMiddle(+keywordEnd)
// where keywordMiddle has size lengthMiddle and keywordEnd has size lengthEnd
// It has the int[70] repetition, three char[] variables (keywordBeginning[3],
// keywordMiddle[11], and keywordEnd[6]), and three int variables (lengthMiddle,
// lengthEnd, and counter) as input arguments and it returns one on success,
// otherwise zero is returned if an error occurred
/* ************************************************************************** */
int detectRepetition(int repetition[70], char keywordBeginning[3],
                     char keywordMiddle[11], char keywordEnd[6],
                                   int lengthMiddle, int lengthEnd, int counter)
{
    // Check that counter is between 1 and 70 (total number of keywords)
    if (counter<1 || counter>70)
    {
        PRINT_ERROR("In detectRepetition: the input variable counter ");
        fprintf(stderr,"corresponding to the %d-th keyword read, ",counter);
        fprintf(stderr,"should be an integer between one and and seventy ");
        fprintf(stderr,"(the total number of different possible keywords).\n");
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

    // Check that lengthEnd stores keywordEnd's size (between 1 and 6)
    if (lengthEnd<1 || lengthEnd>6)
    {
        PRINT_ERROR("In detectRepetition: the input lengthEnd variable ");
        fprintf(stderr,"(=%d) should be an integer comprised ",lengthEnd);
        fprintf(stderr,"between one and six, corresponding to the length ");
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
            fprintf(stderr,"char[6] variable keywordMEnd is not a string ");
            fprintf(stderr,"made of %d characters, which were ",lengthEnd-1);
            fprintf(stderr,"supposed to be the last ones read on the ");
            fprintf(stderr,"%d-th keyword.\n",counter);
            return 0;
        }
    }

    // Distinguishing the 70 different cases and excluding non-valid keywords
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
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_leng")
                                                    && !strcmp(keywordEnd,"th"))
    {
        repetition[3]++;
    }
    else if (!strcmp(keywordBeginning,"en") && !strcmp(keywordMiddle,"d_data"))
    {
        repetition[4]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_data"))
    {
        repetition[5]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_chem"))
    {
        repetition[6]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_mesh"))
    {
        repetition[7]++;
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_elas"))
    {
        repetition[8]++;
    }
    else if (!strcmp(keywordBeginning,"nu") && !strcmp(keywordMiddle,"_elec")
                                                 && !strcmp(keywordEnd,"trons"))
    {
        repetition[9]++;
    }
    else if (!strcmp(keywordBeginning,"nu") && !strcmp(keywordMiddle,"_spin"))
    {
        repetition[10]++;
    }
    else if (!strcmp(keywordBeginning,"or") && !strcmp(keywordMiddle,"b_rhf"))
    {
        repetition[11]++;
    }
    else if (!strcmp(keywordBeginning,"x_") && !strcmp(keywordMiddle,"min"))
    {
        repetition[12]++;
    }
    else if (!strcmp(keywordBeginning,"x_") && !strcmp(keywordMiddle,"max"))
    {
        repetition[13]++;
    }
    else if (!strcmp(keywordBeginning,"y_") && !strcmp(keywordMiddle,"min"))
    {
        repetition[14]++;
    }
    else if (!strcmp(keywordBeginning,"y_") && !strcmp(keywordMiddle,"max"))
    {
        repetition[15]++;
    }
    else if (!strcmp(keywordBeginning,"z_") && !strcmp(keywordMiddle,"min"))
    {
        repetition[16]++;
    }
    else if (!strcmp(keywordBeginning,"z_") && !strcmp(keywordMiddle,"max"))
    {
        repetition[17]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"x"))
    {
        repetition[18]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"y"))
    {
        repetition[19]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"z"))
    {
        repetition[20]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_x"))
    {
        repetition[21]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_y"))
    {
        repetition[22]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_z"))
    {
        repetition[23]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_t") &&
                                                      !strcmp(keywordEnd,"ype"))
    {
        repetition[24]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_x"))
    {
        repetition[25]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_y"))
    {
        repetition[26]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_z"))
    {
        repetition[27]++;
    }
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r"))
    {
        repetition[28]++;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_err"))
    {
        repetition[29]++;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_min"))
    {
        repetition[30]++;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_max"))
    {
        repetition[31]++;
    }
    else if (!strcmp(keywordBeginning,"tr") &&
                                            !strcmp(keywordMiddle,"ick_matrix"))
    {
        repetition[32]++;
    }
    else if (!strcmp(keywordBeginning,"ap") &&
                                             !strcmp(keywordMiddle,"prox_mode"))
    {
        repetition[33]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_max"))
    {
        repetition[34]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d0p"))
    {
        repetition[35]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d1p"))
    {
        repetition[36]++;
    }
    else if (!strcmp(keywordBeginning,"it") && !strcmp(keywordMiddle,"er_tol")
                                                   && !strcmp(keywordEnd,"d2p"))
    {
        repetition[37]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_type"))
    {
        repetition[38]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_mesh"))
    {
        repetition[39]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_data"))
    {
        repetition[40]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_prin")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[41]++;
    }
    else if (!strcmp(keywordBeginning,"sa") && !strcmp(keywordMiddle,"ve_wher")
                                                     && !strcmp(keywordEnd,"e"))
    {
        repetition[42]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_lengt")
                                                     && !strcmp(keywordEnd,"h"))
    {
        repetition[43]++;
    }
    else if (!strcmp(keywordBeginning,"pa") &&
                                              !strcmp(keywordMiddle,"th_medit"))
    {
        repetition[44]++;
    }
    else if (!strcmp(keywordBeginning,"pa") &&
                                              !strcmp(keywordMiddle,"th_mmg3d"))
    {
        repetition[45]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_mshdi")
                                                    && !strcmp(keywordEnd,"st"))
    {
        repetition[46]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_elast")
                                                    && !strcmp(keywordEnd,"ic"))
    {
        repetition[47]++;
    }
    else if (!strcmp(keywordBeginning,"pa") && !strcmp(keywordMiddle,"th_advec")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[48]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_is") &&
                                                        !strcmp(keywordEnd,"o"))
    {
        repetition[49]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_is") &&
                                                        !strcmp(keywordEnd,"o"))
    {
        repetition[50]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_me") &&
                                                        !strcmp(keywordEnd,"t"))
    {
        repetition[51]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_me") &&
                                                        !strcmp(keywordEnd,"t"))
    {
        repetition[52]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_ls"))
    {
        repetition[53]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_ls"))
    {
        repetition[54]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ode_l") &&
                                                       !strcmp(keywordEnd,"ag"))
    {
        repetition[55]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"in_la") &&
                                                        !strcmp(keywordEnd,"g"))
    {
        repetition[56]++;
    }
    else if (!strcmp(keywordBeginning,"hm") && !strcmp(keywordMiddle,"ax_la") &&
                                                        !strcmp(keywordEnd,"g"))
    {
        repetition[57]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_is")
                                                     && !strcmp(keywordEnd,"o"))
    {
        repetition[58]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_me")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[59]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_ls"))
    {
        repetition[60]++;
    }
    else if (!strcmp(keywordBeginning,"ha") && !strcmp(keywordMiddle,"usd_la")
                                                     && !strcmp(keywordEnd,"g"))
    {
        repetition[61]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_is")
                                                     && !strcmp(keywordEnd,"o"))
    {
        repetition[62]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_me")
                                                     && !strcmp(keywordEnd,"t"))
    {
        repetition[63]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_ls"))
    {
        repetition[64]++;
    }
    else if (!strcmp(keywordBeginning,"hg") && !strcmp(keywordMiddle,"rad_la")
                                                     && !strcmp(keywordEnd,"g"))
    {
        repetition[65]++;
    }
    else if (!strcmp(keywordBeginning,"n_") && !strcmp(keywordMiddle,"i") &&
                                                      !strcmp(keywordEnd,"ter"))
    {
        repetition[66]++;
    }
    else if (!strcmp(keywordBeginning,"re") && !strcmp(keywordMiddle,"sidual"))
    {
        repetition[67]++;
    }
    else if (!strcmp(keywordBeginning,"de") && !strcmp(keywordMiddle,"lta_t"))
    {
        repetition[68]++;
    }
    else if (!strcmp(keywordBeginning,"no") && !strcmp(keywordMiddle,"_cfl"))
    {
        repetition[69]++;
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

/* ************************************************************************** */
// The function changeValuesOfParameters saves the value stored either in
// readIntegerIn, readDouble, or readStringIn in the structure pointed by
// pParameters, depending on the type of the (counter)-th keyword in the *.info
// file, read as a concatenation of keywordBeginning+keywordMiddle(+keywordEnd)
// where keywordMiddle has size lengthMiddle, and keywordEnd has size lengthEnd.
// It has the Parameters* variable (defined in main.h), 3 char[] variables
// (keywordbeginning[3], keywordMiddle[11], and keywordEnd[4]), a char*
// readStringIn, four int variables (lengthMiddle, lengthEnd, counter, and
// readIntegerIn), and a double readDouble variable as input arguments, and it
// returns one on success, otherwise zero is returned if an error is encountered
/* ************************************************************************** */
int changeValuesOfParameters(Parameters* pParameters, char keywordBeginning[3],
                             char keywordMiddle[11], char keywordEnd[6],
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
        PRINT_ERROR("In changeValuesOfParameters: the input pParameters ");
        fprintf(stderr,"variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pParameters);
        return 0;
    }

    // Check that pParameters stores correct strings for name_info and path_*
    boolean=checkStringFromLength(pParameters->name_info,7,
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
        fprintf(stderr,"one of the char* variables name_info, path_medit, ");
        fprintf(stderr,"path_mmg3d, path_mshdist, path_elastic, or ");
        fprintf(stderr,"path_advect of the input structure pointed by ");
        fprintf(stderr,"pParameters is not a string with the correct size.\n");
        return 0;
    }

    // Check that counter is between 1 and 70 (total number of keywords)
    if (counter<1 || counter>70)
    {
        PRINT_ERROR("In changeValuesOfParameters: the input variable counter ");
        fprintf(stderr,"corresponding to the %d-th keyword read, ",counter);
        fprintf(stderr,"should be an integer between one and and seventy ");
        fprintf(stderr,"(the total number of different possible keywords).\n");
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

    // Check that lengthEnd stores keywordEnd's size (between 1 and 6)
    if (lengthEnd<1 || lengthEnd>6)
    {
        PRINT_ERROR("In changeValuesOfParameters: the input lengthEnd ");
        fprintf(stderr,"variable (=%d) should be an integer ",lengthEnd);
        fprintf(stderr,"comprised between one and six, corresponding to the ");
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
            fprintf(stderr,"the input char[6] variable keywordMEnd is not a ");
            fprintf(stderr,"string made of %d characters, which ",lengthEnd-1);
            fprintf(stderr,"were supposed to be the last ones read on the ");
            fprintf(stderr,"%d-th keyword.\n",counter);
            return 0;
        }
    }

    // Change the values of pParameters depending on the 70 different cases
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
            fprintf(stderr,"one in the input *.info file.\n");
            return 0;
        }
        else if (readIntegerIn>pParameters->name_length)
        {
            size=readIntegerIn;

           // Reallocate memory for the name_info keyword: the realloc function
           // returns a pointer to allocated memory, otherwise NULL
            stringCopy=(char*)realloc(pParameters->name_info,size*sizeof(char));
            if (stringCopy==NULL)
            {
                PRINT_ERROR("In changeValuesOfParameters: could not ");
                fprintf(stderr,"reallocate memory for the char* ");
                fprintf(stderr,"pParameters->name_info variable.\n");
                return 0;
            }
            pParameters->name_info=stringCopy;

            // Reallocate memory for the name_data keyword if not NULL
            if (pParameters->name_data!=NULL)
            {
                stringCopy=(char*)realloc(pParameters->name_data,
                                                             size*sizeof(char));
                if (stringCopy==NULL)
                {
                    PRINT_ERROR("In changeValuesOfParameters: could not ");
                    fprintf(stderr,"reallocate memory for the char* ");
                    fprintf(stderr,"pParameters->name_data variable.\n");
                    return 0;
                }
                pParameters->name_data=stringCopy;
            }

            // Reallocate memory for the name_chem keyword if not NULL
            if (pParameters->name_chem!=NULL)
            {
                stringCopy=(char*)realloc(pParameters->name_chem,
                                                             size*sizeof(char));
                if (stringCopy==NULL)
                {
                    PRINT_ERROR("In changeValuesOfParameters: could not ");
                    fprintf(stderr,"reallocate memory for the char* ");
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
                    fprintf(stderr,"reallocate memory for the char* ");
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
                    fprintf(stderr,"reallocate memory for the char* ");
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
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_data"))
    {
        // Check if this is the first time pParameters->name_data is found
        if (pParameters->name_data!=NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: the name_data char* ");
            fprintf(stderr,"variable (=%p) of ",(void*)pParameters->name_data);
            fprintf(stderr,"the structure pointed by pParameters has not ");
            fprintf(stderr,"been correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>6)
        if (!checkStringFromLength(readStringIn,7,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input char* variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as name_data, is not a string ");
            fprintf(stderr,"of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
            fprintf(stderr,"order to store something more than the *.data ");
            fprintf(stderr,"extension).\n");
            return 0;
        }

        // Allocate memory for name_data: calloc function returns a pointer to
        // the allocated memory, otherwise NULL; strncpy function returns a
        // pointer to the string (not used here)
        size=pParameters->name_length;
        pParameters->name_data=(char*)calloc(size,sizeof(char));
        if (pParameters->name_data==NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: could not allocate ");
            fprintf(stderr,"memory for the char* pParameters->name_data ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        strncpy(pParameters->name_data,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"na") && !strcmp(keywordMiddle,"me_chem"))
    {
        // Check if this is the first time pParameters->name_chem is found
        if (pParameters->name_chem!=NULL)
        {
            PRINT_ERROR("In changeValuesOfParameters: the name_chem char* ");
            fprintf(stderr,"variable (=%p) of ",(void*)pParameters->name_chem);
            fprintf(stderr,"the structure pointed by pParameters has not ");
            fprintf(stderr,"been correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>6)
        if (!checkStringFromLength(readStringIn,7,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input char* variable readStringIn, ");
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
            PRINT_ERROR("In changeValuesOfParameters: the name_mesh char* ");
            fprintf(stderr,"variable (=%p) of ",(void*)pParameters->name_mesh);
            fprintf(stderr,"the structure pointed by pParameters has not ");
            fprintf(stderr,"been correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>6)
        if (!checkStringFromLength(readStringIn,7,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input char* variable readStringIn, ");
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
            fprintf(stderr,"memory for the char* pParameters->name_mesh ");
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
            PRINT_ERROR("In changeValuesOfParameters: the name_elas char* ");
            fprintf(stderr,"variable (=%p) of ",(void*)pParameters->name_elas);
            fprintf(stderr,"the structure pointed by pParameters has not ");
            fprintf(stderr,"been correctly initialized to the NULL pointer.\n");
            return 0;
        }

        // Check readStringIn has a size less than pParameters->name_length (>6)
        if (!checkStringFromLength(readStringIn,7,pParameters->name_length))
        {
            PRINT_ERROR("In changeValuesOfParameters: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the input char* variable readStringIn, ");
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
            fprintf(stderr,"memory for the char* pParameters->name_elas ");
            fprintf(stderr,"variable.\n");
            return 0;
        }
        strncpy(pParameters->name_elas,readStringIn,size);
    }
    else if (!strcmp(keywordBeginning,"nu") && !strcmp(keywordMiddle,"_elec")
                                                 && !strcmp(keywordEnd,"trons"))
    {
        pParameters->nu_electrons=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"nu") && !strcmp(keywordMiddle,"_spin"))
    {
        pParameters->nu_spin=readIntegerIn;
    }
    else if (!strcmp(keywordBeginning,"or") && !strcmp(keywordMiddle,"b_rhf"))
    {
        pParameters->orb_rhf=readIntegerIn;
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
    else if (!strcmp(keywordBeginning,"ls") && !strcmp(keywordMiddle,"_r"))
    {
        pParameters->ls_r=readDouble;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_err"))
    {
        pParameters->met_err=readDouble;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_min"))
    {
        pParameters->met_min=readDouble;
    }
    else if (!strcmp(keywordBeginning,"me") && !strcmp(keywordMiddle,"t_max"))
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
            fprintf(stderr,"one in the input *.info file.\n");
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
                fprintf(stderr,"reallocate memory for the char* ");
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
                fprintf(stderr,"reallocate memory for the char* ");
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
                fprintf(stderr,"reallocate memory for the char* ");
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
                fprintf(stderr,"reallocate memory for the char* ");
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
                fprintf(stderr,"reallocate memory for the char* ");
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
            fprintf(stderr,"the input char* variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_medit, is not a string ");
            fprintf(stderr,"of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 1 in ",pParameters->path_length);
            fprintf(stderr,"order to store something more than the ");
            fprintf(stderr,"terminating nul character).\n");
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
            fprintf(stderr,"the input char* variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_mmg3d, is not a string ");
            fprintf(stderr,"of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 1 in ",pParameters->path_length);
            fprintf(stderr,"order to store something more than the ");
            fprintf(stderr,"terminating nul character).\n");
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
            fprintf(stderr,"the input char* variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_mshdist, is not a ");
            fprintf(stderr,"string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 1 in ",pParameters->path_length);
            fprintf(stderr,"order to store something more than the ");
            fprintf(stderr,"terminating nul character).\n");
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
            fprintf(stderr,"the input char* variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_elastic, is not a ");
            fprintf(stderr,"string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 1 in ",pParameters->path_length);
            fprintf(stderr,"order to store something more than the ");
            fprintf(stderr,"terminating nul character).\n");
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
            fprintf(stderr,"the input char* variable readStringIn, ");
            fprintf(stderr,"corresponding to the %d-th keyword ",counter);
            fprintf(stderr,"sucessfully read as path_advect, is not a ");
            fprintf(stderr,"string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 1 in ",pParameters->path_length);
            fprintf(stderr,"order to store something more than the ");
            fprintf(stderr,"terminating nul character).\n");
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

/* ************************************************************************** */
// The function readInfoFileAndGetParameters reads the *.info file whose name
// has already been stored in the name_info variable of the structure pointed
// by pParameters, and it adjusts the structure Parameters accordingly. It has
// the Parameters* variable (defined in main.h) as input argument and it
// returns zero if an error occurs, otherwise one is returned in case of success
/* ************************************************************************** */
int readInfoFileAndGetParameters(Parameters* pParameters)
{
    char keywordBeginning[3]={'\0'}, keywordMiddle[11]={'\0'};
    char keywordEnd[6]={'\0'}, *readStringOut=NULL, *readStringIn=NULL;
    char *fileName=NULL;
    size_t lengthString=0;
    int lengthMiddle=0, lengthEnd=0, readIntegerIn=0, readIntegerOut=0;
    int readChar=0, lengthName=0, keywordType=0, repetition[70]={0}, i=0;
    int counter=0, boolean=0;
    double readDouble=.0;
    FILE *infoFile=NULL;

    // Check if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: the input pParameters ");
        fprintf(stderr,"variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pParameters);
        return 0;
    }

    // Check if the name_length and path_length variables store correct values
    if (pParameters->name_length<7 || pParameters->path_length<2)
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: the input pParameters ");
        fprintf(stderr,"structure does not have its name_length ");
        fprintf(stderr,"(=%d) and path_length ",pParameters->name_length);
        fprintf(stderr," (=%d) variables strictly ",pParameters->path_length);
        fprintf(stderr,"greater than six and one, respectively.\n");
        return 0;
    }

    // Initialize the dynamic allocation of memory for the readStringIn variable
    // calloc returns a pointer to the allocated memory, otherwise NULL
    lengthString=DEF_MAX(pParameters->name_length,pParameters->path_length);
    readStringIn=(char*)calloc(lengthString,sizeof(char));
    if (readStringIn==NULL)
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: could not allocate ");
        fprintf(stderr,"memory for the char* (local) variable readStringIn.\n");
        return 0;
    }

    // Check if the *.info file have been previously created
    fileName=pParameters->name_info;
    if (initialFileExists(fileName,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: initialFileExists ");
        fprintf(stderr,"function did not return one which is the expected ");
        fprintf(stderr,"value here.\n");

        // free function does not return value (void output)
        free(readStringIn);
        readStringIn=NULL;
        return 0;
    }

    // Check that *.info file name has length greater than six (including '/0')
    // strlen function returns the length of the string. Here, it can be saved
    // as an int (and not size_t) because initialFileExists function has already
    // checked that fileName is a string of length less than
    // pParameters->name_length (which is an int)
    lengthName=strlen(fileName);
    if (lengthName<6)
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: ");
        fprintf(stderr,"%s file name should at least contain six ",fileName);
        fprintf(stderr,"characters instead of %d ",lengthName-1);
        fprintf(stderr,"(in order to end with something more than the *.info ");
        fprintf(stderr,"extension).\n");
        free(readStringIn);
        readStringIn=NULL;
        return 0;
    }

    // Check if the *.info file name ends with the ".info" extension
    boolean=(fileName[lengthName-5]=='.' && fileName[lengthName-4]=='i');
    boolean=(boolean && fileName[lengthName-3]=='n');
    boolean=(boolean && fileName[lengthName-2]=='f');
    boolean=(boolean && fileName[lengthName-1]=='o');
    boolean=(boolean && fileName[lengthName]=='\0');
    if (!boolean)
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: ");
        fprintf(stderr,"%s file name does not end with with the ",fileName);
        fprintf(stderr,"'.info' extension.\n");
        free(readStringIn);
        readStringIn=NULL;
        return 0;
    }

    // Check if the *.info file is opened (it must have been created before)
    // fopen function returns a FILE pointer on success, otherwise NULL
    fprintf(stdout,"\nOpening %s file. ",fileName);
    infoFile=fopen(fileName,"r");
    if (infoFile==NULL)
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: we were not able to ");
        fprintf(stderr,"read into the %s file.\n",fileName);
        free(readStringIn);
        readStringIn=NULL;
        return 0;
    }
    fprintf(stdout,"Reading parameters. ");

    fscanf(infoFile," ");
    counter=0;
    while (counter<70)
    {
        counter++;

        // Get the first 2 characters to distinguish among different keywords
        // fgets function returns NULL if it fails, otherwise the same pointer
        readStringOut=fgets(keywordBeginning,3,infoFile);
        if (readStringOut==NULL)
        {
            PRINT_ERROR("In readInfoFileAndGetParameters: wrong return ");
            fprintf(stderr,"%p of the fgets function ",(void*)readStringOut);
            fprintf(stderr,"in the attempt of reading the first two letters ");
            fprintf(stderr,"of the %d-th keyword.\n",counter);
            closeTheFile(&infoFile);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }

        // Treat the case where 'e' in end_data has already been read as double
        // fseek function returns zero on success otherwise a non-zero value
        if (!strcmp(keywordBeginning,"nd"))
        {
            if (fseek(infoFile,-3,SEEK_CUR))
            {
                PRINT_ERROR("In readInfoFileAndGetParameters: wrong return ");
                fprintf(stderr,"of the fseek function in the attempt of ");
                fprintf(stderr,"reading the beginning of what may be");
                fprintf(stderr,"'end_data' for the %d-th keyword ",counter);
                fprintf(stderr,"(we only have read *%s*).\n",keywordBeginning);
                closeTheFile(&infoFile);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
            boolean=(fgetc(infoFile)=='e' && fgetc(infoFile)=='n');
            boolean=(boolean && fgetc(infoFile)=='d' && fgetc(infoFile)=='_');
            boolean=(boolean && fgetc(infoFile)=='d' && fgetc(infoFile)=='a');
            boolean=(boolean && fgetc(infoFile)=='t' && fgetc(infoFile)=='a');
            if (!boolean)
            {
                PRINT_ERROR("In readInfoFileAndGetParameters: the first two ");
                fprintf(stderr,"letters read (=%s) for the ",keywordBeginning);
                fprintf(stderr,"%d-th keyword do not correspond to ",counter);
                fprintf(stderr,"the beginning of any known keyword of the ");
                fprintf(stderr,"structure Parameters (or wrong return of the ");
                fprintf(stderr,"fgetc function in the attempt of reading the ");
                fprintf(stderr,"'end_data' keyword).\n");
                closeTheFile(&infoFile);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
            if (fseek(infoFile,-6,SEEK_CUR))
            {
                PRINT_ERROR("In readInfoFileAndGetParameters: wrong return ");
                fprintf(stderr,"of the fseek function in the attempt of ");
                fprintf(stderr,"reading 'end_data' for the %d-th ",counter);
                fprintf(stderr,"keyword.\n");
                closeTheFile(&infoFile);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
            strncpy(keywordBeginning,"en",3);
        }

        // Get length of the next keyword part to read (stored in lengthMiddle)
        boolean=getLengthAfterKeywordBeginning(keywordBeginning,counter);
        if (!boolean)
        {
            PRINT_ERROR("In readInfoFileAndGetParameters: ");
            fprintf(stderr,"getLengthAfterKeywordBeginning function ");
            fprintf(stderr,"returned zero which is not the expected value ");
            fprintf(stderr,"here.\n");
            closeTheFile(&infoFile);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        lengthMiddle=boolean;

        // Reading the middle part of the keyword
        readStringOut=fgets(keywordMiddle,lengthMiddle,infoFile);
        if (readStringOut==NULL)
        {
            PRINT_ERROR("In readInfoFileAndGetParameters: wrong return ");
            fprintf(stderr,"%p of fgets function in the ",(void*)readStringOut);
            fprintf(stderr,"attempt of reading the next part ");
            fprintf(stderr,"%s* of the %d-th ",keywordBeginning,counter);
            fprintf(stderr,"keyword.\n");
            closeTheFile(&infoFile);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }

        // Type of value to read after (1=int, 2=double, 3=string, -1=end_data)
        boolean=getTypeAfterKeyword(keywordMiddle,lengthMiddle,counter);
        if (!boolean)
        {
            PRINT_ERROR("In readInfoFileAndGetParameters: ");
            fprintf(stderr,"getTypeAfterKeyword function returned zero which ");
            fprintf(stderr,"is not the expected value here.\n");
            closeTheFile(&infoFile);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        keywordType=boolean;

        // Get the string length (including '\0') to end the keyword's reading
        boolean=getLengthAfterKeywordMiddle(keywordMiddle,lengthMiddle,counter);
        if (!boolean)
        {
            PRINT_ERROR("In readInfoFileAndGetParameters: ");
            fprintf(stderr,"getLengthAfterkeywordMiddle function returned ");
            fprintf(stderr,"zero which is not the expected value here.\n");
            closeTheFile(&infoFile);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        lengthEnd=boolean;

        // Read the next character if necessary
        if (lengthEnd>1)
        {
            readStringOut=fgets(keywordEnd,lengthEnd,infoFile);
            for (i=lengthEnd; i<6; i++)
            {
                keywordEnd[i]='\0';
            }
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readInfoFileAndGetParameters: wrong return ");
                fprintf(stderr,"%p of the fgets ",(void*)readStringOut);
                fprintf(stderr,"function in the attempt of reading the final ");
                fprintf(stderr,"part %s%s* ",keywordBeginning,keywordMiddle);
                fprintf(stderr,"of the %d-th keyword.\n",counter);
                closeTheFile(&infoFile);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
        }

        // Set up the detection of an already read keyword
        if (!detectRepetition(repetition,keywordBeginning,keywordMiddle,
                                     keywordEnd,lengthMiddle,lengthEnd,counter))
        {
            PRINT_ERROR("In readInfoFileAndGetParameters: detectRepetition ");
            fprintf(stderr,"function returned zero instead of one.\n");
            closeTheFile(&infoFile);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
        for (i=0; i<70; i++)
        {
            if (repetition[i]>1)
            {
                PRINT_ERROR("In readInfoFileAndGetParameters: the ");
                fprintf(stderr,"%d-th keyword (=%s",counter,keywordBeginning);
                fprintf(stderr,"%s",keywordMiddle);
                if (lengthEnd>1)
                {
                    fprintf(stderr,"%s",keywordEnd);
                }
                fprintf(stderr,") has already been read before in the upper ");
                fprintf(stderr,"part of the %s file. Please avoid ",fileName);
                fprintf(stderr,"repetitions by modifying the file ");
                fprintf(stderr,"accordingly.\n");
                closeTheFile(&infoFile);
                free(readStringIn);
                readStringIn=NULL;
                return 0;
            }
        }

        // Read the value appearing after the keyword (depending on its type)
        switch (keywordType)
        {
            case -1:
                counter=70;
                break;

            case 1:
                readIntegerOut=fscanf(infoFile," %d ",&readIntegerIn);
                if (readIntegerOut!=1)
                {
                    PRINT_ERROR("In readInfoFileAndGetParameters: wrong ");
                    fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                    fprintf(stderr,"fscanf function in the attempt of ");
                    fprintf(stderr,"reading the value associated with the ");
                    fprintf(stderr,"%d-th keyword ",counter);
                    fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
                    if (lengthEnd>1)
                    {
                        fprintf(stderr,"%s",keywordEnd);
                    }
                    fprintf(stderr,", an integer value is expected here).\n");
                    closeTheFile(&infoFile);
                    free(readStringIn);
                    readStringIn=NULL;
                    return 0;
                }
                break;

            case 2:
                readIntegerOut=fscanf(infoFile," %lf ",&readDouble);
                if (readIntegerOut!=1)
                {
                    PRINT_ERROR("In readInfoFileAndGetParameters: wrong ");
                    fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                    fprintf(stderr,"fscanf function in the attempt of ");
                    fprintf(stderr,"reading the value associated with the ");
                    fprintf(stderr,"%d-th keyword ",counter);
                    fprintf(stderr,"(=%s%s",keywordBeginning,keywordMiddle);
                    if (lengthEnd>1)
                    {
                        fprintf(stderr,"%s",keywordEnd);
                    }
                    fprintf(stderr,", a double value is expected here).\n");
                    closeTheFile(&infoFile);
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
                        PRINT_ERROR("In readInfoFileAndGetParameters: could ");
                        fprintf(stderr,"not reallocate memory for the ");
                        fprintf(stderr,"(local) char* readStringIn ");
                        fprintf(stderr,"variable.\n");
                        closeTheFile(&infoFile);
                        free(readStringIn);
                        readStringIn=NULL;
                        return 0;
                    }
                    readStringIn=readStringOut;
                }

                // Start reading the name of the path, character by character
                fscanf(infoFile," ");
                i=0;
                do {
                    readChar=fgetc(infoFile);
                    if (readChar==EOF)
                    {
                        PRINT_ERROR("In readInfoFileAndGetParameters: ");
                        fprintf(stderr,"end-of-file (EOF=%d) reached ",EOF);
                        fprintf(stderr,"without the end_data keyword ");
                        fprintf(stderr,"encountered or wrong return ");
                        fprintf(stderr,"(=%d) of the fgetc function ",readChar);
                        fprintf(stderr,"in the attempt of reading the ");
                        fprintf(stderr,"%d-th letter of the string ",i+1);
                        fprintf(stderr,"associated with the %d-th ",counter);
                        fprintf(stderr,"keyword (=%s",keywordBeginning);
                        fprintf(stderr,"%s",keywordMiddle);
                        if (lengthEnd>1)
                        {
                            fprintf(stderr,"%s",keywordEnd);
                        }
                        fprintf(stderr,").\n");
                        closeTheFile(&infoFile);
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
                   PRINT_ERROR("In readInfoFileAndGetParameters: it seems ");
                   fprintf(stderr,"that the name pointed by one of the ");
                   fprintf(stderr,"name_* or path_* variables associated ");
                   fprintf(stderr,"with the %d-th keyword has more ",counter);
                   fprintf(stderr,"than %d letters, which is the ",i-1);
                   fprintf(stderr,"maximum size allowed here.\n");
                   closeTheFile(&infoFile);
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
                fscanf(infoFile," ");
                break;

            default:
                PRINT_ERROR("In readInfoFileAndGetParameters: the local ");
                fprintf(stderr,"variable keywordType (=%d) ",keywordType);
                fprintf(stderr,"cannot only store the -1, 1, 2 or 3 values.\n");
                closeTheFile(&infoFile);
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
            PRINT_ERROR("In readInfoFileAndGetParameters: ");
            fprintf(stderr,"changeValuesOfParameters function returned zero ");
            fprintf(stderr,"instead of one.\n");
            closeTheFile(&infoFile);
            free(readStringIn);
            readStringIn=NULL;
            return 0;
        }
    } // This is the end of the while loop for keyword

    // Free the memory allocated for readStringIn
    free(readStringIn);
    readStringIn=NULL;

    // Closing the *.info file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(infoFile))
    {
        PRINT_ERROR("In readInfoFileAndGetParameters: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileName);
        infoFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    infoFile=NULL;

    return 1;
}

/* ************************************************************************** */
// The function checkValuesOfAllParameters tests if the values of the structure
// pointed by pParameters that have been loaded are valid. It has the
// Parameters* variable (defined in main.h) as input argument and it returns one
// on success, otherwise zero is returned for an invalid value or an error
/* ************************************************************************** */
int checkValuesOfAllParameters(Parameters* pParameters)
{
    size_t lengthName=0;
    char* fileLocation=NULL;
    int boolean=0;
    double dx=0., dy=0., dz=0.;

    // Testing if the input variable pParameters is pointing to NULL
    boolean=(pParameters!=NULL);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the input pParameters ");
        fprintf(stderr,"variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pParameters);
        return 0;
    }

    // Check pParameters->name_length
    boolean=(pParameters->name_length>=DEF_MAX(7,NAME_LENGTH));
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the variable name_length ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->name_length);
        fprintf(stderr,"pointed by pParameters must be an integer ");
        fprintf(stderr,"(strictly) greater than six (to store at least ");
        fprintf(stderr,"something more than the *.info extension) and which ");
        fprintf(stderr,"is at least equal or greater than its initial ");
        fprintf(stderr,"default value (=%d).\n",NAME_LENGTH);
        return 0;
    }

    // Check pParameters->name_info
    boolean=checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the char* ");
        fprintf(stderr,"name_info variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.info extension).\n");
        return 0;
    }

    // Check if the *.info name ends with the ".info" extension
    // strlen function returns the length of the string (without the nul char)
    lengthName=strlen(pParameters->name_info);
    if (pParameters->name_info[lengthName-5]!='.' ||
                                    pParameters->name_info[lengthName-4]!='i' ||
                                    pParameters->name_info[lengthName-3]!='n' ||
                                    pParameters->name_info[lengthName-2]!='f' ||
                                    pParameters->name_info[lengthName-1]!='o' ||
                                       pParameters->name_info[lengthName]!='\0')
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the char* name_info ");
        fprintf(stderr,"variable of the structure pointed by pParameters ");
        fprintf(stderr," points to the %s file name, ",pParameters->name_info);
        fprintf(stderr,"which does not end with the '.info' extension.\n");
        return 0;
    }

    // Check pParameters->verbose
    boolean=(pParameters->verbose==0 || pParameters->verbose==1);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the verbose variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->verbose);
        fprintf(stderr,"by pParameters can only be set to 0 or 1.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'verbose' ");
        fprintf(stderr,"keyword in %s file.\n",pParameters->name_info);
        return 0;
    }
    if (pParameters->verbose)
    {
        fprintf(stdout,"Checking validity of all values in the ");
        fprintf(stdout,"structure Parameters.\n");
    }

    // Check pParameters->opt_mode
    boolean=(pParameters->opt_mode>-3 && pParameters->opt_mode<5);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the opt_mode variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->opt_mode);
        fprintf(stderr,"by pParameters can only be set to -2, -1, 0, 1, 2, ");
        fprintf(stderr,"3, or 4.\nPlease modify the value accordingly after ");
        fprintf(stderr,"the 'opt_mode' keyword in %s ",pParameters->name_info);
        fprintf(stderr,"file.\n");
        return 0;
    }

    // Check pParameters->n_cpu
    boolean=(pParameters->n_cpu>0);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the n_cpu variable ");
        fprintf(stderr,"(=%d) of the structure pointed by ",pParameters->n_cpu);
        fprintf(stderr,"pParameters must be a positive integer.\nPlease ");
        fprintf(stderr,"modify the value accordingly after the 'n_cpu' ");
        fprintf(stderr,"keyword in %s file.\n",pParameters->name_info);
        return 0;
    }

    // Check pParameters->name_data
    if (pParameters->name_data!=NULL)
    {
        boolean=checkStringFromLength(pParameters->name_data,7,
                                                      pParameters->name_length);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: ");
            fprintf(stderr,"checkStringFromLength function returned zero, ");
            fprintf(stderr,"which is not the expected value here, after ");
            fprintf(stderr,"having checked that the char* name_data variable ");
            fprintf(stderr,"of the structure pointed by pParameters is not ");
            fprintf(stderr,"a string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
            fprintf(stderr,"order to store at least something more than the ");
            fprintf(stderr," *.data extension).\nPlease specify in the ");
            fprintf(stderr,"%s file a valid name ",pParameters->name_info);
            fprintf(stderr,"after the 'name_data' keyword, or extend ");
            fprintf(stderr,"properly the allowed length for name_* file ");
            fprintf(stderr,"names thanks to the 'name_length' keyword, or ");
            fprintf(stderr,"simply remove this keyword line in order to ");
            fprintf(stderr,"generate a default *.data file.\n");
            return 0;
        }

        // Check if the *.data name ends with the ".data" extension
        lengthName=strlen(pParameters->name_data);
        if (pParameters->name_data[lengthName-5]!='.' ||
                                    pParameters->name_data[lengthName-4]!='d' ||
                                    pParameters->name_data[lengthName-3]!='a' ||
                                    pParameters->name_data[lengthName-2]!='t' ||
                                    pParameters->name_data[lengthName-1]!='a' ||
                                       pParameters->name_data[lengthName]!='\0')
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the char* name_data ");
            fprintf(stderr,"variable of the structure pointed by pParameters ");
            fprintf(stderr,"points to the %s file ",pParameters->name_data);
            fprintf(stderr,"name, which does not end with with the '.data' ");
            fprintf(stderr,"extension.\nPlease specify in the ");
            fprintf(stderr,"%s file a valid name ",pParameters->name_info);
            fprintf(stderr,"after the 'name_data' keyword, or simply ");
            fprintf(stderr,"remove this keyword line in order to generate a ");
            fprintf(stderr,"default *.data file name.\n");
            return 0;
        }
    }
    else
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: no *.data file name has been ");
            fprintf(stdout,"loaded in the structure pointed by pParameters. ");
            fprintf(stdout,"A default file name will thus be generated ");
            fprintf(stdout,"thanks to the %s file ",pParameters->name_info);
            fprintf(stdout,"name (where the '.info' extension will be ");
            fprintf(stdout,"replaced by the '.data' one).\n\n");
        }

        // calloc returns a pointer to the allocated memory, otherwise NULL
        pParameters->name_data=(char*)calloc(pParameters->name_length,
                                                                  sizeof(char));
        if (pParameters->name_data==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: could not allocate ");
            fprintf(stderr,"memory for the pParameters->name_data variable.\n");
            return 0;
        }

        // strncpy function returns a pointer to the string (not counting '\0')
        strncpy(pParameters->name_data,pParameters->name_info,
                                                      pParameters->name_length);
        boolean=strlen(pParameters->name_data);
        pParameters->name_data[boolean-5]='.';
        pParameters->name_data[boolean-4]='d';
        pParameters->name_data[boolean-3]='a';
        pParameters->name_data[boolean-2]='t';
        pParameters->name_data[boolean-1]='a';
        pParameters->name_data[boolean]='\0';
    }

    // Check pParameters->name_chem
    boolean=checkStringFromLength(pParameters->name_chem,7,
                                                      pParameters->name_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the char* ");
        fprintf(stderr,"name_chem variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.chem or *.wfn extension).\nPlease, specify in the ");
        fprintf(stderr,"%s file a valid name after ",pParameters->name_info);
        fprintf(stderr,"the 'name_chem' keyword since a (*.chem/ *.wfn) ");
        fprintf(stderr,"chemical file is mandatory in order to load the ");
        fprintf(stderr,"chemical data properly.\n");
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
        fprintf(stderr,"keyword in %s file.\n",pParameters->name_info);
        return 0;
    }

    // Check pParameters->name_mesh
    if (pParameters->name_mesh!=NULL)
    {
        boolean=checkStringFromLength(pParameters->name_mesh,7,
                                                      pParameters->name_length);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: ");
            fprintf(stderr,"checkStringFromLength function returned zero, ");
            fprintf(stderr,"which is not the expected value here, after ");
            fprintf(stderr,"having checked that the char* name_mesh variable ");
            fprintf(stderr,"of the structure pointed by pParameters is not ");
            fprintf(stderr,"a string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
            fprintf(stderr,"order to store at least something more than the ");
            fprintf(stderr," *.mesh or *.cube extension).\nPlease specify in ");
            fprintf(stderr,"the %s file a valid name ",pParameters->name_info);
            fprintf(stderr,"after the 'name_mesh' keyword, or extend ");
            fprintf(stderr,"properly the allowed length for name_* file ");
            fprintf(stderr,"names thanks to the 'name_length' keyword, or ");
            fprintf(stderr,"simply remove this keyword line in order to ");
            fprintf(stderr,"generate a default mesh file.\n");
            return 0;
        }
    }
    else
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
            fprintf(stdout,"function: no *.mesh file name has been loaded in ");
            fprintf(stdout,"the structure pointed by pParameters. A default ");
            fprintf(stdout,"file name will thus be generated thanks to the ");
            fprintf(stdout,"%s file name (where the ",pParameters->name_info);
            fprintf(stdout,"'.info' extension will be replaced by the ");
            fprintf(stdout,"'.mesh' one). The initial domain contained ");
            if (pParameters->ls_type)
            {
                fprintf(stdout,"into this mesh file will be a sphere.\n\n");
            }
            else
            {
                fprintf(stdout,"into this mesh file will be a cube.\n\n");
            }
        }
    }

    // Check pParameters->name_elas
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        if (pParameters->name_elas!=NULL)
        {
            boolean=initialFileExists(pParameters->name_elas,
                                                      pParameters->name_length);
            if (boolean!=1)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: ");
                fprintf(stderr,"initialFileExists function did not return ");
                fprintf(stderr,"zero, which is not the expected value here, ");
                fprintf(stderr,"after having checked that the char* ");
                fprintf(stderr,"name_elas variable of the structure pointed ");
                fprintf(stderr,"by pParameters is not pointing to the name ");
                fprintf(stderr,"of an existing *.elas file of length ");
                fprintf(stderr,"(strictly) less than ");
                fprintf(stderr,"%d (and more than 5 ",pParameters->name_length);
                fprintf(stderr,"in order to store at least something more ");
                fprintf(stderr,"than the *.elas extension).\nPlease specify ");
                fprintf(stderr,"in the %s file a ",pParameters->name_info);
                fprintf(stderr,"valid name after the 'name_elas' keyword, or ");
                fprintf(stderr,"extend if necessary the allowed length for ");
                fprintf(stderr,"name_* file names thanks to the ");
                fprintf(stderr,"'name_length' keyword, or simply remove this ");
                fprintf(stderr,"keyword line in order to generate a default ");
                fprintf(stderr,"*.elas file.\n");
                return 0;
            }

            // Check that *.elas file name has length greater than six
            lengthName=strlen(pParameters->name_elas);
            if (lengthName<6)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: the char* ");
                fprintf(stderr,"name_elas variable of the structure pointed ");
                fprintf(stderr,"by pParameters points to the ");
                fprintf(stderr,"%s file name, which ",pParameters->name_elas);
                fprintf(stderr,"should at least contain six characters ");
                fprintf(stderr,"instead of %ld (in order to end ",lengthName-1);
                fprintf(stderr,"with something more than the *.elas ");
                fprintf(stderr,"extension).\nPlease specify in the ");
                fprintf(stderr,"%s file a valid name ",pParameters->name_info);
                fprintf(stderr,"after the 'name_elas' keyword, or simply ");
                fprintf(stderr,"remove this keyword line in order to ");
                fprintf(stderr,"generate a default*.elas file.\n");
                return 0;
            }

            // Check if the *.elas name ends with the ".elas" extension
            if (pParameters->name_elas[lengthName-5]!='.' ||
                                    pParameters->name_elas[lengthName-4]!='e' ||
                                    pParameters->name_elas[lengthName-3]!='l' ||
                                    pParameters->name_elas[lengthName-2]!='a' ||
                                    pParameters->name_elas[lengthName-1]!='s' ||
                                       pParameters->name_elas[lengthName]!='\0')
            {
                PRINT_ERROR("In checkValuesOfAllParameters: the char* ");
                fprintf(stderr,"name_elas variable of the structure pointed ");
                fprintf(stderr,"by pParameters points to the ");
                fprintf(stderr,"%s file name, which ",pParameters->name_elas);
                fprintf(stderr,"does not end with with the '.elas' ");
                fprintf(stderr,"extension.\nPlease specify in the ");
                fprintf(stderr,"%s file a valid name ",pParameters->name_info);
                fprintf(stderr,"after the 'name_elas' keyword, or simply ");
                fprintf(stderr,"remove this keyword line in order to ");
                fprintf(stderr,"generate a default *.elas file name.\n");
                return 0;
            }

            // Allocate memory for the fileLocation variable
            lengthName=pParameters->name_length;
            fileLocation=(char*)calloc(lengthName,sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In checkValuesOfAllParameters: could not ");
                fprintf(stderr,"allocate memory for the local char* ");
                fprintf(stderr,"fileLocation variable.\n");
                return 0;
            }

            // Set the default *.elas name
            strncpy(fileLocation,pParameters->name_info,lengthName);
            lengthName=strlen(fileLocation);
            fileLocation[lengthName-5]='.';
            fileLocation[lengthName-4]='e';
            fileLocation[lengthName-3]='l';
            fileLocation[lengthName-2]='a';
            fileLocation[lengthName-1]='s';
            fileLocation[lengthName]='\0';

            // strcmp returns 0 if the 2 strings are ==, otherwise <0 (resp. >0)
            // if the 1st string arg. is shorter (resp. longer) than the 2nd one
            if (strcmp(pParameters->name_elas,fileLocation))
            {
                if (initialFileExists(fileLocation,pParameters->name_length)==1)
                {
                    // remove returns 0 on success, otherwise -1
                    if (remove(fileLocation))
                    {
                        PRINT_ERROR("In checkValuesOfAllParameters: wrong ");
                        fprintf(stderr,"return (=-1) of the standard remove ");
                        fprintf(stderr,"c-function in the attempt of ");
                        fprintf(stderr,"removing the ");
                        fprintf(stderr,"%s file.\n",fileLocation);

                        // free function does not return any value (void output)
                        free(fileLocation);
                        fileLocation=NULL;
                        return 0;
                    }
                }

                if (!copyFileLocation(pParameters->name_elas,
                                         pParameters->name_length,fileLocation))
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
            if (pParameters->verbose)
            {
                fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
                fprintf(stdout,"function: no *.elas file name has been ");
                fprintf(stdout,"loaded in the structure pointed by ");
                fprintf(stdout,"pParameters. A default file name will thus ");
                fprintf(stdout,"be generated thanks to the ");
                fprintf(stdout,"%s file name (where ",pParameters->name_info);
                fprintf(stdout,"the '.info' extension will be replaced by ");
                fprintf(stdout,"the '.elas' one), and default values will be ");
                fprintf(stdout,"inserted in this *.elas file.\n\n");
            }

            fprintf(stdout,"\nGenerating file for the elastic software with ");
            fprintf(stdout,"default Lamé coefficients.");
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
            PRINT_ERROR("In checkValuesOfAllParameters: an *.elas file name ");
            fprintf(stderr,"has been loaded in the structure pointed by ");
            fprintf(stderr,"pParameters although the current optimization ");
            fprintf(stderr,"mode (=%d) does not ",pParameters->opt_mode);
            fprintf(stderr,"require such a file.\nPlease delete the line ");
            fprintf(stderr,"containing the 'name_elas' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
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
        fprintf(stderr,"'nu_electrons' keyword in %s ",pParameters->name_info);
        fprintf(stderr,"file since a positive number of electrons to look ");
        fprintf(stderr,"for is mandatory for proceeding further.\n");
        return 0;
    }

    // Check pParameters->nu_spin
    boolean=(abs(pParameters->nu_spin)<=pParameters->nu_electrons);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the nu_spin variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->nu_spin);
        fprintf(stderr,"by pParameters must not be (strictly) lower ");
        fprintf(stderr,"than -%d or greater than ",pParameters->nu_electrons);
        fprintf(stderr,"%d.\nPlease modify the ",pParameters->nu_electrons);
        fprintf(stderr,"value accordingly after the 'nu_spin' keyword in ");
        fprintf(stderr,"%s file.\n",pParameters->name_info);
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
        fprintf(stderr,"keyword in %s file.\n",pParameters->name_info);
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
        fprintf(stderr,"corresponding keywords in %s ",pParameters->name_info);
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
        fprintf(stderr,"after the corresponding keywords in ");
        fprintf(stderr,"%s file.\n",pParameters->name_info);
        return 0;
    }

    // Check the variables related to the initial level-set function
    boolean=(pParameters->ls_x>=pParameters->x_min);
    boolean=(boolean && pParameters->ls_x<=pParameters->x_max);
    boolean=(boolean && pParameters->ls_y>=pParameters->y_min);
    boolean=(boolean && pParameters->ls_y<=pParameters->y_max);
    boolean=(boolean && pParameters->ls_z>=pParameters->z_min);
    boolean=(boolean && pParameters->ls_z<=pParameters->z_max);
    if (!boolean && pParameters->verbose)
    {
        fprintf(stdout,"\nWarning in checkValuesOfAllParameters ");
        fprintf(stdout,"function: the center (%lf,",pParameters->ls_x);
        fprintf(stdout,"%lf,%lf) of ",pParameters->ls_y,pParameters->ls_z);
        if (pParameters->ls_type)
        {
            fprintf(stdout,"the initial sphere is not located inside the ");
        }
        else
        {
            fprintf(stdout,"the initial cube is not located inside the ");
        }
        fprintf(stdout,"computational domain ");
        fprintf(stdout,"[%lf,%lf]x",pParameters->x_min,pParameters->x_max);
        fprintf(stdout,"[%lf,%lf]x",pParameters->y_min,pParameters->y_max);
        fprintf(stdout,"[%lf,%lf].\n\n",pParameters->z_min,pParameters->z_max);
    }

    boolean=(pParameters->ls_r>0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the ls_r variable ");
        fprintf(stderr,"(=%lf) of the structure pointed by ",pParameters->ls_r);
        fprintf(stderr,"pParameters, and corresponding to the ");
        if (pParameters->ls_type)
        {
             fprintf(stderr,"radius of the initial sphere, ");
        }
        else
        {
             fprintf(stderr,"length of the initial cube, ");
        }
        fprintf(stderr,"must be positive.\nPlease modify the value ");
        fprintf(stderr,"accordingly after the 'ls_r' keyword in ");
        fprintf(stderr,"%s file.\n",pParameters->name_info);
        return 0;
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
            fprintf(stderr,"keywords in %s file.\n",pParameters->name_info);
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
            PRINT_ERROR("In checkValuesOfAllParameters: at least one value ");
            fprintf(stderr,"among the met_err, met_min, and met_max ");
            fprintf(stderr,"variables of the structure pointed by ");
            fprintf(stderr,"pParameters is not set to its default value ");
            fprintf(stderr,"although the current optimization mode ");
            fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
            fprintf(stderr,"them.\nPlease delete the line(s) containing the ");
            fprintf(stderr,"corresponding keyword(s) in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
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
            fprintf(stderr,"after the 'trick_matrix' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
        }
    }
    else
    {
        boolean=(pParameters->trick_matrix==TRICK_MATRIX);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the trick_matrix ");
            fprintf(stderr,"variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not set to its default value ");
            fprintf(stderr,"(=%d) although the current ",TRICK_MATRIX);
            fprintf(stderr,"optimization mode (=%d) ",pParameters->opt_mode);
            fprintf(stderr,"does not require it.\nPlease delete the line ");
            fprintf(stderr,"containing the 'trick_matrix' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
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
            fprintf(stderr,"after the 'approx_mode' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
        }
    }
    else
    {
        boolean=(pParameters->approx_mode==APPROX_MODE);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the approx_mode ");
            fprintf(stderr,"variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not set to its default value ");
            fprintf(stderr,"(=%d) although the current ",APPROX_MODE);
            fprintf(stderr,"optimization mode (=%d) ",pParameters->opt_mode);
            fprintf(stderr,"does not require it.\nPlease delete the line ");
            fprintf(stderr,"containing the 'approx_mode' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
        }
    }

    // Check the variables related to the stop criteria
    boolean=(pParameters->iter_max>=0 && pParameters->iter_told0p>=0.);
    boolean=(boolean && pParameters->iter_told1p>=0.);
    boolean=(boolean && pParameters->iter_told2p>=0.);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: expecting\n");
        fprintf(stderr,"(pParameters->iter_max=%d) ",pParameters->iter_max);
        fprintf(stderr,">= 0\n(pParameters->iter_told0p");
        fprintf(stderr,"=%lf) >= 0.0\n",pParameters->iter_told0p);
        fprintf(stderr,"(pParameters->iter_told1p");
        fprintf(stderr,"=%lf) >= 0.0\n",pParameters->iter_told1p);
        fprintf(stderr,"(pParameters->iter_told2p");
        fprintf(stderr,"=%lf) >= 0.0\nPlease modify ",pParameters->iter_told2p);
        fprintf(stderr,"the values accordingly after the corresponding ");
        fprintf(stderr,"keywords in %s file.\n",pParameters->name_info);
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
        fprintf(stderr,"'save_type' keyword in %s ",pParameters->name_info);
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
        fprintf(stderr,"accordingly after the corresponding keywords in ");
        fprintf(stderr,"%s file.\n",pParameters->name_info);
        return 0;
    }

    boolean=(pParameters->save_where>0 && pParameters->save_where<8);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: the save_where variable ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->save_where);
        fprintf(stderr,"pointed by pParameters can only be set to 1, 2, 3, ");
        fprintf(stderr,"4, 5, 6, or 7.\nPlease modify the value accordingly ");
        fprintf(stderr,"after the 'save_type' keyword in ");
        fprintf(stderr,"%s file.\n",pParameters->name_info);
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
        fprintf(stderr,"initial default value (=%d).\n",PATH_LENGTH);
        return 0;
    }

    // Check pParameters->path_medit
    boolean=checkStringFromLength(pParameters->path_medit,2,
                                                      pParameters->path_length);
    if (!boolean)
    {
        PRINT_ERROR("In checkValuesOfAllParameters: ");
        fprintf(stderr,"checkStringFromLength function returned zero, which ");
        fprintf(stderr,"is not the expected value here, after having checked ");
        fprintf(stderr,"that the char* path_medit variable of the structure ");
        fprintf(stderr,"pointed by pParameters is not a string of length ");
        fprintf(stderr,"(strictly) less than %d ",pParameters->path_length);
        fprintf(stderr,"(and more than 1 in order to store at least ");
        fprintf(stderr,"something more than the terminating nul character).\n");
        fprintf(stderr,"Please specify in the %s file ",pParameters->name_info);
        fprintf(stderr,"a valid path name needed for the medit software ");
        fprintf(stderr,"after the 'path_medit' keyword or extend properly ");
        fprintf(stderr,"the allowed length for path_* names thanks to the ");
        fprintf(stderr,"'path_length' keyword.\n");
        return 0;
    }

    // Check pParameters->path_mmg3d
    if (pParameters->opt_mode>0)
    {
        boolean=checkStringFromLength(pParameters->path_mmg3d,2,
                                                      pParameters->path_length);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: ");
            fprintf(stderr,"checkStringFromLength function returned zero, ");
            fprintf(stderr,"which is not the expected value here, after ");
            fprintf(stderr,"having checked that the char* path_mmg3d ");
            fprintf(stderr,"variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not a string of length (strictly) ");
            fprintf(stderr,"less than %d (and more ",pParameters->path_length);
            fprintf(stderr,"(than 1 in order to store at least something ");
            fprintf(stderr,"more than the terminating nul character).\nPlease");
            fprintf(stderr,"specify in the %s file ",pParameters->name_info);
            fprintf(stderr,"a valid path name needed for the mmg3d software ");
            fprintf(stderr,"after the 'path_mmg3d' keyword or extend ");
            fprintf(stderr,"properly the allowed length for path_* names ");
            fprintf(stderr,"thanks to the 'path_length' keyword.\n");
            return 0;
       }
    }
    else
    {
        if (pParameters->path_mmg3d==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the path_mmg3d ");
            fprintf(stderr,"variable of the structure pointed by pParameters ");
            fprintf(stderr,"is pointing to the ");
            fprintf(stderr,"%p address, ",(void*)pParameters->path_mmg3d);
            fprintf(stderr,"whereas it should be set to its default value ");
            fprintf(stderr,"(=%s).\n",PATH_MMG3D);
            return 0;
        }
        else
        {
            if (strcmp(pParameters->path_mmg3d,PATH_MMG3D))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: a path name ");
                fprintf(stderr,"for the mmg3d software has been loaded in ");
                fprintf(stderr,"the structure pointed by pParameters ");
                fprintf(stderr,"although the current optimization mode ");
                fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stderr,"such a software.\nPlease delete the line ");
                fprintf(stderr,"containing the 'path_mmg3d' keyword in ");
                fprintf(stderr,"%s file.\n",pParameters->name_info);
                return 0;
            }
        }
    }

    // Check pParameters->path_mshdist
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2 ||
                                                       pParameters->opt_mode==4)
    {
        boolean=checkStringFromLength(pParameters->path_mshdist,2,
                                                      pParameters->path_length);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: ");
            fprintf(stderr,"checkStringFromLength function returned zero, ");
            fprintf(stderr,"which is not the expected value here, after ");
            fprintf(stderr,"having checked that the char* path_mshdist ");
            fprintf(stderr,"variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not a string of length (strictly) ");
            fprintf(stderr,"less than %d (and more ",pParameters->path_length);
            fprintf(stderr,"(than 1 in order to store at least something ");
            fprintf(stderr,"more than the terminating nul character).\nPlease");
            fprintf(stderr,"specify in the %s file ",pParameters->name_info);
            fprintf(stderr,"a valid path name needed for the mshdist ");
            fprintf(stderr,"software after the 'path_mshdist' keyword or");
            fprintf(stderr,"extend properly the allowed length for path_* ");
            fprintf(stderr,"names thanks to the 'path_length' keyword.\n");
            return 0;
       }
    }
    else
    {
        if (pParameters->path_mshdist==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the path_mshdist ");
            fprintf(stderr,"variable of the structure pointed by pParameters ");
            fprintf(stderr,"is pointing to the ");
            fprintf(stderr,"%p address, ",(void*)pParameters->path_mshdist);
            fprintf(stderr,"whereas it should be set to its default value ");
            fprintf(stderr,"(=%s).\n",PATH_MSHDIST);
            return 0;
        }
        else
        {
            if (strcmp(pParameters->path_mshdist,PATH_MSHDIST))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: a path name ");
                fprintf(stderr,"for the mshdist software has been loaded in ");
                fprintf(stderr,"the structure pointed by pParameters ");
                fprintf(stderr,"although the current optimization mode ");
                fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stderr,"such a software.\nPlease delete the line ");
                fprintf(stderr,"containing the 'path_mshdist' keyword in ");
                fprintf(stderr,"%s file.\n",pParameters->name_info);
                return 0;
            }
        }
    }

    // Check pParameters->path_elastic and pParameters->path_advect
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        boolean=checkStringFromLength(pParameters->path_elastic,2,
                                                      pParameters->path_length);
        boolean=(boolean && checkStringFromLength(pParameters->path_advect,2,
                                                     pParameters->path_length));
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: ");
            fprintf(stderr,"checkStringFromLength function returned zero, ");
            fprintf(stderr,"which is not the expected value here, after ");
            fprintf(stderr,"having successively checked that one of the ");
            fprintf(stderr,"path_elastic or path_advect (char*) variables of ");
            fprintf(stderr,"the structure pointed by pParameters is not a ");
            fprintf(stderr,"string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 1 in ",pParameters->path_length);
            fprintf(stderr,"order to store at least something more than the ");
            fprintf(stderr,"terminating nul character).\nPlease specify in ");
            fprintf(stderr,"the %s file some valid ",pParameters->name_info);
            fprintf(stderr,"path names needed for the elastic and advect ");
            fprintf(stderr,"softwares, respectively after the 'path_elastic' ");
            fprintf(stderr,"and 'path_advect' keywords or extend properly ");
            fprintf(stderr,"the allowed length for path_* names thanks to ");
            fprintf(stderr,"the 'path_length' keyword.\n");
            return 0;
       }
    }
    else
    {
        if (pParameters->path_elastic==NULL || pParameters->path_advect==NULL)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: one of the ");
            fprintf(stderr,"path_elastic ");
            fprintf(stderr,"(=%p) or ",(void*)pParameters->path_elastic);
            fprintf(stderr,"path_advect ");
            fprintf(stderr,"(=%p) variables ",(void*)pParameters->path_advect);
            fprintf(stderr,"of the structure pointed by pParameters is ");
            fprintf(stderr,"pointing to NULL, whereas they should be both ");
            fprintf(stderr,"set to their default values (%s and ",PATH_ELASTIC);
            fprintf(stderr,"%s).\n",PATH_ADVECT);
            return 0;
        }
        else
        {
            if (strcmp(pParameters->path_elastic,PATH_ELASTIC) ||
                                   strcmp(pParameters->path_advect,PATH_ADVECT))
            {
                PRINT_ERROR("In checkValuesOfAllParameters: at least one of ");
                fprintf(stderr,"the path names for the elastic and advect ");
                fprintf(stderr,"softwares have been loaded in the structure ");
                fprintf(stderr,"pointed by pParameters although the current ");
                fprintf(stderr,"optimization mode ");
                fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
                fprintf(stderr,"such softwares.\nPlease delete the line(s) ");
                fprintf(stderr,"containing the corresponding keyword(s) in ");
                fprintf(stderr,"%s file.\n",pParameters->name_info);
                return 0;
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
            fprintf(stderr,"%lf) > 1.0\nPlease modify ",pParameters->hgrad_met);
            fprintf(stderr,"the values accordingly after the corresponding ");
            fprintf(stderr,"keywords in %s file.\n",pParameters->name_info);
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
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: at least one of the ");
            fprintf(stderr,"*_iso or *_met variables of the structure ");
            fprintf(stderr,"pointed by pParameters (and needed for the mmg3d ");
            fprintf(stderr,"software) is not set to its default value ");
            fprintf(stderr,"although the current optimization mode ");
            fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
            fprintf(stderr,"such parameters.\nPlease delete the line(s) ");
            fprintf(stderr,"containing the corresponding keyword(s) in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
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
            fprintf(stderr,"keywords in %s file.\n",pParameters->name_info);
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
            PRINT_ERROR("In checkValuesOfAllParameters: at least one of the ");
            fprintf(stderr,"*_ls variables of the structure pointed by ");
            fprintf(stderr,"pParameters (and needed for the mmg3d software) ");
            fprintf(stderr,"is not set to its default value although the ");
            fprintf(stderr,"current optimization mode ");
            fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
            fprintf(stderr,"such parameters.\nPlease delete the line(s) ");
            fprintf(stderr,"containing the corresponding keyword(s) in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
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
            fprintf(stderr,"accordingly after the 'hmode_lag' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
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
            fprintf(stderr,"keywords in %s file.\n",pParameters->name_info);
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
            PRINT_ERROR("In checkValuesOfAllParameters: at least one of the ");
            fprintf(stderr,"*_lag variables of the structure pointed by ");
            fprintf(stderr,"pParameters (and needed for the mmg3d software) ");
            fprintf(stderr,"is not set to its default value although the ");
            fprintf(stderr,"current optimization mode ");
            fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
            fprintf(stderr,"such parameters.\nPlease delete the line(s) ");
            fprintf(stderr,"containing the corresponding keyword(s) in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
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
            fprintf(stderr,"corresponding keywords in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
         }
    }
    else
    {
        boolean=(pParameters->n_iter==N_ITER);
        boolean=(boolean && pParameters->residual==RESIDUAL);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: at least one of the ");
            fprintf(stderr,"n_iter or residual variable of the structure ");
            fprintf(stderr,"pointed by pParameters (and needed for the ");
            fprintf(stderr,"mshdist software) is not set to its default ");
            fprintf(stderr,"value although the current optimization mode ");
            fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
            fprintf(stderr,"such parameters.\nPlease delete the line(s) ");
            fprintf(stderr,"containing the corresponding keyword(s) in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
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
            fprintf(stderr,"after the 'delta_t' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
        }

        boolean=(pParameters->no_cfl==0 || pParameters->no_cfl==1);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: the no_cfl ");
            fprintf(stderr,"(=%d) variable of the ",pParameters->no_cfl);
            fprintf(stderr,"structure pointed by pParameters can only be set ");
            fprintf(stderr,"to 0 or 1.\nPlease modify the value accordingly ");
            fprintf(stderr,"after the 'no_cfl' keyword in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
        }
    }
    else
    {
        boolean=(pParameters->delta_t==DELTA_T);
        boolean=(boolean && pParameters->no_cfl==NO_CFL);
        if (!boolean)
        {
            PRINT_ERROR("In checkValuesOfAllParameters: at least one of the ");
            fprintf(stderr,"delta_t or no_cfl variable of the structure ");
            fprintf(stderr,"pointed by pParameters (and needed for the ");
            fprintf(stderr,"advect software) is not set to its default value ");
            fprintf(stderr,"although the current optimization mode ");
            fprintf(stderr,"(=%d) does not require ",pParameters->opt_mode);
            fprintf(stderr,"such parameters.\nPlease delete the line(s) ");
            fprintf(stderr,"containing the corresponding keyword(s) in ");
            fprintf(stderr,"%s file.\n",pParameters->name_info);
            return 0;
        }
    }

    if (pParameters->verbose)
    {
        fprintf(stdout,"All variables in the structure Parameters are ok.\n");
    }

    return 1;
}

/* ************************************************************************** */
// The function writingDefaultElasticFile writes a default *.elas file in the
// case where pParameters->opt_mode=1 or 2 and if no file has been specified in
// the *.info file (using the name_elas keyword) i.e. if pParameters->name_elas
// is set to NULL. It uses the Lame coefficients given in the corresponding
// LAME_* preporcessor constants (defined in loadParameters.h). It has the
// Parameters* (defined in main.h) variable as input arguments and it returns
// one if the default *.elas file has been successfully written (and the
// pParameters->name_elas variable successfully updated) otherwise zero is
// returned if an error is encountered
/* ************************************************************************** */
int writingDefaultElasticFile(Parameters* pParameters)
{
    size_t lengthName=0;
    FILE *elasticFile=NULL;

    // Testing if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In writingDefaultElasticFile: the input pParameters ");
        fprintf(stderr,"variable is pointing to the %p ",(void*)pParameters);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check pParameters->name_info
    if (!checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingDefaultElasticFile: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the char* ");
        fprintf(stderr,"name_info variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.info extension).\n");
        return 0;
    }

    // Check that we are in the situation where a default *.elas file is needed
    if (pParameters->name_elas!=NULL)
    {
        PRINT_ERROR("In writingDefaultElasticFile: we cannot generate a ");
        fprintf(stderr,"default *.elas file because one has already been ");
        fprintf(stderr,"specified in the %s file.\n",pParameters->name_info);
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

    // strlen function returns the length of the string (not including '\0')
    lengthName=strlen(pParameters->name_info);
    if (pParameters->name_info[lengthName-5]!='.' ||
                                    pParameters->name_info[lengthName-4]!='i' ||
                                    pParameters->name_info[lengthName-3]!='n' ||
                                    pParameters->name_info[lengthName-2]!='f' ||
                                    pParameters->name_info[lengthName-1]!='o' ||
                                       pParameters->name_info[lengthName]!='\0')
    {
        PRINT_ERROR("In writingDefaultElasticFile: the name_info variable ");
        fprintf(stderr,"(=%s) of the structure ",pParameters->name_info);
        fprintf(stderr,"pointed by pParameters does not end with the '.info' ");
        fprintf(stderr,"extension.\n");
        return 0;
    }

    // strncpy function returns a pointer to the string (not used here)
    strncpy(pParameters->name_elas,pParameters->name_info,lengthName);
    pParameters->name_elas[lengthName-5]='.';
    pParameters->name_elas[lengthName-4]='e';
    pParameters->name_elas[lengthName-3]='l';
    pParameters->name_elas[lengthName-2]='a';
    pParameters->name_elas[lengthName-1]='s';
    pParameters->name_elas[lengthName]='\0';

    // Check if the *.elas file is opened (warning: overwrite if it exists)
    // fopen function returns a FILE pointer on success, otherwise NULL
    fprintf(stdout,"\nOpening %s file. ",pParameters->name_elas);
    elasticFile=fopen(pParameters->name_elas,"w+");
    if (elasticFile==NULL)
    {
        PRINT_ERROR("In writingDefaultElasticFile: we were not able to open ");
        fprintf(stderr,"and write into the %s file.\n",pParameters->name_elas);
        return 0;
    }
    fprintf(stdout,"Writing elastic parameters. ");

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
    fprintf(stdout,"Closing file.\n");
    elasticFile=NULL;

    return 1;
}

/* ************************************************************************** */
// The function writingInputFile, depending on the opt_mode variable, writes an
// *.input file that contains the default and prescribed values used to perform
// the mpd algorithm before starting the optimization loop. It has the same
// name than the *.info file where the '.info' extension has been replaced by
// the '.input' one. It has the Parameters* variable (defined in main.h) as
// input argument and it returns one on success, otherwise zero is returned for
// an invalid value or an error
/* ************************************************************************** */
int writingInputFile(Parameters* pParameters)
{
    size_t lengthName=0;
    char *fileName=NULL;
    FILE *inputFile=NULL;

    // Testing if the input variable pParameters is pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In writingInputFile: the input pParameters variable is ");
        fprintf(stderr,"pointing to the %p address.\n",(void*)pParameters);
        return 0;
    }

    // Check pParameters->name_info name
    if (!checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingInputFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the char* name_info ");
        fprintf(stderr,"variable of the structure pointed by pParameters is ");
        fprintf(stderr,"not a string of length (strictly) less than ");
        fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
        fprintf(stderr,"order to store at least something more than the ");
        fprintf(stderr,"*.info extension).\n");
        return 0;
    }

    // Check if the *.info file name ends with the ".info" extension
    // strlen function returns the length of the string.
    lengthName=strlen(pParameters->name_info);
    if (pParameters->name_info[lengthName-5]!='.' ||
                                    pParameters->name_info[lengthName-4]!='i' ||
                                    pParameters->name_info[lengthName-3]!='n' ||
                                    pParameters->name_info[lengthName-2]!='f' ||
                                    pParameters->name_info[lengthName-1]!='o' ||
                                       pParameters->name_info[lengthName]!='\0')
    {
        PRINT_ERROR("In writingInputFile: ");
        fprintf(stderr,"%s file does not end with the ",pParameters->name_info);
        fprintf(stderr,"'.info' extension.\n");
        return 0;
    }

    // Allocate memory for the *.input file
    // calloc returns a pointer to the allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileName=(char*)calloc(lengthName,sizeof(char));
    if (fileName==NULL)
    {
        PRINT_ERROR("In writingInputFile: could not allocate memory for the ");
        fprintf(stderr,"char* (local) fileName variable, that was suppose to ");
        fprintf(stderr,"temporarily store the name of the *.input file.\n");
        return 0;
    }

    // strncpy function returns a pointer to the string (not used here)
    strncpy(fileName,pParameters->name_info,lengthName);
    lengthName=strlen(fileName);
    fileName[lengthName-5]='.';
    fileName[lengthName-4]='i';
    fileName[lengthName-3]='n';
    fileName[lengthName-2]='p';
    fileName[lengthName-1]='u';
    fileName[lengthName]='t';
    fileName[lengthName+1]='\0';
    lengthName++;

    // Create and open the *.input file (warning: overwrite file if it already
    // exists). fopen function returns a FILE pointer on success, otherwise NULL
    fprintf(stdout,"\nOpening %s file. ",fileName);
    inputFile=fopen(fileName,"w+");
    if (inputFile==NULL)
    {
        PRINT_ERROR("In writingInputFile: we were not able to write into the ");
        fprintf(stderr,"%s file.\n",fileName);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(stdout,"Writing input parameters. ");

    // Write general parameters
    fprintf(inputFile,"opt_mode %d \n",pParameters->opt_mode);
    fprintf(inputFile,"verbose %d \n",pParameters->verbose);
    fprintf(inputFile,"n_cpu %d \n",pParameters->n_cpu);
    fprintf(inputFile,"name_length %d \n\n",pParameters->name_length);

    // Check and write the pParameters->name_data variable
    if (!checkStringFromLength(pParameters->name_data,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingInputFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value ");
        fprintf(stderr,"here, after having checked that the char* ");
        fprintf(stderr,"name_data variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more ",pParameters->name_length);
        fprintf(stderr,"than 5 in order to store at least something more ");
        fprintf(stderr,"than the *.data extension).\n");
        fprintf(inputFile,"end_data\n");
        closeTheFile(&inputFile);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(inputFile,"name_data %s \n",pParameters->name_data);

    // Check and write the pParameters->name_chem variable
    if (!checkStringFromLength(pParameters->name_chem,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingInputFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the char* name_chem ");
        fprintf(stderr,"variable of the structure pointed by pParameters is ");
        fprintf(stderr,"not a string of length (strictly) less than ");
        fprintf(stderr,"%d (and more than 5 in ",pParameters->name_length);
        fprintf(stderr,"order to store at least something more than the ");
        fprintf(stderr,"*.chem or *.wfn extension).\n");
        fprintf(inputFile,"end_data\n");
        closeTheFile(&inputFile);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(inputFile,"name_chem %s \n",pParameters->name_chem);

    // Check and write the pParameters->name_mesh variable
    if (!checkStringFromLength(pParameters->name_mesh,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In writingInputFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value ");
        fprintf(stderr,"here, after having checked that the char* ");
        fprintf(stderr,"name_mesh variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more ",pParameters->name_length);
        fprintf(stderr,"than 5 in order to store at least something more ");
        fprintf(stderr,"than the *.mesh or *.cube extension).\n");
        fprintf(inputFile,"end_data\n");
        closeTheFile(&inputFile);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(inputFile,"name_mesh %s \n",pParameters->name_mesh);

    // Check and write the pParameters->name_elas variable if opt_mode=1/2
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        if (!checkStringFromLength(pParameters->name_elas,7,
                                                      pParameters->name_length))
        {
            PRINT_ERROR("In writingInputFile: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked ");
            fprintf(stderr,"that the char* name_elas variable of the ");
            fprintf(stderr,"structure pointed by pParameters is not a ");
            fprintf(stderr,"string of length (strictly) less than ");
            fprintf(stderr,"%d (and more than 5 ",pParameters->name_length);
            fprintf(stderr,"in order to store at least something more ");
            fprintf(stderr,"than the *.elas extension).\n");
            fprintf(inputFile,"end_data\n");
            closeTheFile(&inputFile);
            free(fileName);
            fileName=NULL;
            return 0;
        }
        fprintf(inputFile,"name_elas %s \n",pParameters->name_elas);
    }

    // Write chemical parameters
    fprintf(inputFile,"\nnu_electrons %d \n",pParameters->nu_electrons);
    fprintf(inputFile,"nu_spin %d \n",pParameters->nu_spin);
    fprintf(inputFile,"orb_rhf %d \n\n",pParameters->orb_rhf);

    // Write the variables related to the computational box
    fprintf(inputFile,"x_min %.8le \n",pParameters->x_min);
    fprintf(inputFile,"y_min %.8le \n",pParameters->y_min);
    fprintf(inputFile,"z_min %.8le \n\n",pParameters->z_min);

    fprintf(inputFile,"x_max %.8le \n",pParameters->x_max);
    fprintf(inputFile,"y_max %.8le \n",pParameters->y_max);
    fprintf(inputFile,"z_max %.8le \n\n",pParameters->z_max);

    fprintf(inputFile,"n_x %d \n",pParameters->n_x);
    fprintf(inputFile,"n_y %d \n",pParameters->n_y);
    fprintf(inputFile,"n_z %d \n\n",pParameters->n_z);

    fprintf(inputFile,"delta_x %.8le \n",pParameters->delta_x);
    fprintf(inputFile,"delta_y %.8le \n",pParameters->delta_y);
    fprintf(inputFile,"delta_z %.8le \n\n",pParameters->delta_z);

    fprintf(inputFile,"ls_type %d \n",pParameters->ls_type);
    fprintf(inputFile,"ls_x %.8le \n",pParameters->ls_x);
    fprintf(inputFile,"ls_y %.8le \n",pParameters->ls_y);
    fprintf(inputFile,"ls_z %.8le \n",pParameters->ls_z);
    fprintf(inputFile,"ls_r %.8le \n\n",pParameters->ls_r);

    // Write variables related to the metric or matrix computation
    if (pParameters->opt_mode>0)
    {
        fprintf(inputFile,"met_err %.8le \n",pParameters->met_err);
        fprintf(inputFile,"met_min %.8le \n",pParameters->met_min);
        fprintf(inputFile,"met_max %.8le \n\n",pParameters->met_max);
    }
    else
    {
        fprintf(inputFile,"trick_matrix %d \n",pParameters->trick_matrix);
        fprintf(inputFile,"approx_mode %d \n\n",pParameters->approx_mode);
    }

    // Write variables related to the stop criteria
    fprintf(inputFile,"iter_max %d \n",pParameters->iter_max);
    fprintf(inputFile,"iter_told0p %.8le \n",pParameters->iter_told0p);
    fprintf(inputFile,"iter_told1p %.8le \n",pParameters->iter_told1p);
    fprintf(inputFile,"iter_told2p %.8le \n\n",pParameters->iter_told2p);

    // Write variables related to the saving of data
    fprintf(inputFile,"save_type %d \n",pParameters->save_type);
    fprintf(inputFile,"save_mesh %d \n",pParameters->save_mesh);
    fprintf(inputFile,"save_data %d \n",pParameters->save_data);
    fprintf(inputFile,"save_print %d \n",pParameters->save_print);
    fprintf(inputFile,"save_where %d \n\n",pParameters->save_where);
    fprintf(inputFile,"path_length %d \n",pParameters->path_length);

    // Check and write the pParameters->path_medit variable
    if (!checkStringFromLength(pParameters->path_medit,2,
                                                      pParameters->path_length))
    {
        PRINT_ERROR("In writingInputFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the char* path_medit ");
        fprintf(stderr,"variable of the structure pointed by pParameters is ");
        fprintf(stderr,"not a string of length (strictly) less than ");
        fprintf(stderr,"%d (and more than 1 in ",pParameters->path_length);
        fprintf(stderr,"order to store at least something more than the ");
        fprintf(stderr,"terminating nul character).\n");
        fprintf(inputFile,"end_data\n");
        closeTheFile(&inputFile);
        free(fileName);
        fileName=NULL;
        return 0;
    }
    fprintf(inputFile,"path_medit %s \n",pParameters->path_medit);

    if (pParameters->opt_mode>0)
    {
        // Check and write the pParameters->path_mmg3d variable
        if (!checkStringFromLength(pParameters->path_mmg3d,2,
                                                      pParameters->path_length))
        {
            PRINT_ERROR("In writingInputFile: checkStringFromLength function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, after having checked that the char* ");
            fprintf(stderr,"path_mmg3d variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not a string of length (strictly) ");
            fprintf(stderr,"less than %d (and more ",pParameters->path_length);
            fprintf(stderr,"than 1 in order to store at least something more ");
            fprintf(stderr,"than the terminating nul character).\n");
            fprintf(inputFile,"end_data\n");
            closeTheFile(&inputFile);
            free(fileName);
            fileName=NULL;
            return 0;
        }
        fprintf(inputFile,"path_mmg3d %s \n",pParameters->path_mmg3d);

        if (pParameters->opt_mode!=3)
        {
            // Check and write the pParameters->path_mshdist variable
            if (!checkStringFromLength(pParameters->path_mshdist,2,
                                                      pParameters->path_length))
            {
                PRINT_ERROR("In writingInputFile: checkStringFromLength ");
                fprintf(stderr,"function returned zero, which is not the ");
                fprintf(stderr,"expected value here, after having checked ");
                fprintf(stderr,"that the char* path_mshdist variable of the ");
                fprintf(stderr,"structure pointed by pParameters is not a ");
                fprintf(stderr,"string of length (strictly) less than ");
                fprintf(stderr,"%d (and more than 1 ",pParameters->path_length);
                fprintf(stderr,"in order to store at least something more ");
                fprintf(stderr,"than the terminating nul character).\n");
                fprintf(inputFile,"end_data\n");
                closeTheFile(&inputFile);
                free(fileName);
                fileName=NULL;
                return 0;
            }
            fprintf(inputFile,"path_mshdist %s \n",pParameters->path_mshdist);

            if (pParameters->opt_mode!=4)
            {
                // Check and write the pParameters->path_elastic variable
                if (!checkStringFromLength(pParameters->path_elastic,2,
                                                      pParameters->path_length))
                {
                    PRINT_ERROR("In writingInputFile: checkStringFromLength ");
                    fprintf(stderr,"function returned zero, which is not the ");
                    fprintf(stderr,"expected value here, after having ");
                    fprintf(stderr,"checked that the char* path_elastic ");
                    fprintf(stderr,"variable of the structure pointed by ");
                    fprintf(stderr,"pParameters is not a string of length ");
                    fprintf(stderr,"(strictly) less than ");
                    fprintf(stderr,"%d ",pParameters->path_length);
                    fprintf(stderr,"(and more than 1 in order to store at ");
                    fprintf(stderr,"least something more than the ");
                    fprintf(stderr,"terminating nul character).\n");
                    fprintf(inputFile,"end_data\n");
                    closeTheFile(&inputFile);
                    free(fileName);
                    fileName=NULL;
                    return 0;
                }
                fprintf(inputFile,"path_elastic %s \n",
                                                     pParameters->path_elastic);

                // Check and write the pParameters->path_advect variable
                if (!checkStringFromLength(pParameters->path_advect,2,
                                                      pParameters->path_length))
                {
                    PRINT_ERROR("In writingInputFile: checkStringFromLength ");
                    fprintf(stderr,"function returned zero, which is not the ");
                    fprintf(stderr,"expected value here, after having ");
                    fprintf(stderr,"checked that the char* path_advect ");
                    fprintf(stderr,"variable of the structure pointed by ");
                    fprintf(stderr,"pParameters is not a string of length ");
                    fprintf(stderr,"(strictly) less than ");
                    fprintf(stderr,"%d ",pParameters->path_length);
                    fprintf(stderr,"(and more than 1 in order to store at ");
                    fprintf(stderr,"least something more than the ");
                    fprintf(stderr,"terminating nul character).\n");
                    fprintf(inputFile,"end_data\n");
                    closeTheFile(&inputFile);
                    free(fileName);
                    fileName=NULL;
                    return 0;
                }
                fprintf(inputFile,"path_advect %s \n",pParameters->path_advect);
            }
        }

        // Write the *_iso and *_met variables related to the mmg3d parameters
        fprintf(inputFile,"\nhmin_iso %.8le \n",pParameters->hmin_iso);
        fprintf(inputFile,"hmax_iso %.8le \n",pParameters->hmax_iso);
        fprintf(inputFile,"hausd_iso %.8le \n",pParameters->hausd_iso);
        fprintf(inputFile,"hgrad_iso %.8le \n\n",pParameters->hgrad_iso);

        fprintf(inputFile,"hmin_met %.8le \n",pParameters->hmin_met);
        fprintf(inputFile,"hmax_met %.8le \n",pParameters->hmax_met);
        fprintf(inputFile,"hausd_met %.8le \n",pParameters->hausd_met);
        fprintf(inputFile,"hgrad_met %.8le \n\n",pParameters->hgrad_met);

        if (pParameters->opt_mode!=3)
        {
            // Write the *_ls variables related to the mmg3d parameters
            fprintf(inputFile,"hmin_ls %.8le \n",pParameters->hmin_ls);
            fprintf(inputFile,"hmax_ls %.8le \n",pParameters->hmax_ls);
            fprintf(inputFile,"hausd_ls %.8le \n",pParameters->hausd_ls);
            fprintf(inputFile,"hgrad_ls %.8le \n\n",pParameters->hgrad_ls);
        }

        if (pParameters->opt_mode==1 || pParameters->opt_mode==3)
        {
            // Write the *_lag variables related to the mmg3d parameters
            fprintf(inputFile,"hmode_lag %d \n",pParameters->hmode_lag);
            fprintf(inputFile,"hmin_lag %.8le \n",pParameters->hmin_lag);
            fprintf(inputFile,"hmax_lag %.8le \n",pParameters->hmax_lag);
            fprintf(inputFile,"hausd_lag %.8le \n",pParameters->hausd_lag);
            fprintf(inputFile,"hgrad_lag %.8le \n\n",pParameters->hgrad_lag);
        }

        if (pParameters->opt_mode!=3)
        {
            // Check the variables related to the mshdist parameters
            fprintf(inputFile,"n_iter %d \n",pParameters->n_iter);
            fprintf(inputFile,"residual %.8le \n",pParameters->residual);
        }

        if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
        {
            // Check the variables related to the advect parameters
            fprintf(inputFile,"delta_t %.8le \n",pParameters->delta_t);
            fprintf(inputFile,"no_cfl %d \n",pParameters->no_cfl);
        }
    }
    fprintf(inputFile,"\nend_data\n\nThe remaining part of this file ");
    fprintf(inputFile,"is considered as comments\n\n");

    // Closing the *.input file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(inputFile))
    {
        PRINT_ERROR("In writingInputFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileName);
        inputFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    inputFile=NULL;

    // Free the memory allocated for fileName
    free(fileName);
    fileName=NULL;

    return 1;
}

/* ************************************************************************** */
// The function loadParameters initializes all the variables of the structure
// pointed by pParameters thanks to the values given in an *.info (input) file
// pointed by nameInfo. The variables of the structure Parameters that are not
// specified in the *.info file are filled with some default values (given by
// the preprocessor constants defined in loadParameters.h file). However, the
// *.info file must contain at least (minimal configuration) the *.wfn/ *.chem
// (chemical) file, preceded by 'name_chem' keyword, the (positive) number of
// electrons to look for, preceded by 'nu_electrons' keyword, and optionally the
// *.mesh/ *.cube (mesh) file to start with, preceded by 'name_mesh' keyword (if
// not specify a cube or a sphere is built depending on the ls_type variable of
// the structure Parameters). The function loadParameters has the Parameters*
// variable (defined in main.h) and a char* variable as input arguments and it
// returns one on success, otherwise zero is returned if an error is encountered
/* ************************************************************************** */
int loadParameters(Parameters* pParameters, char* nameInfo)
{
    // Initializing the structure pParameters to zero
    initializeParameterStructure(pParameters);

    // Set up the default parameters
    if (!setupDefaultParameters(pParameters,nameInfo))
    {
        PRINT_ERROR("In loadParameters: setupDefaultParameters ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

   // Read and update parameters contained in the *.info file
   if (!readInfoFileAndGetParameters(pParameters))
   {
       PRINT_ERROR("In loadParameters: readInfoFileAndGetParameters function ");
       fprintf(stderr,"returned zero instead of one.\n");
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

