/**
* \file optimization.c
* \brief It contains all the functions used to compute the optimization loop
*        of the MPD algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* The main function of this file is called \ref optimization and many other
* functions should be static but have been defines as non-static for performing
* unit-tests on them.
*/

#include "loadParameters.h"
#include "loadMesh.h"
#include "adaptMesh.h"
#include "optimization.h"

/* ************************************************************************** */
// The function initializeDataStructure sets to zero all the variables of the
// Data structure (and pointers to NULL). It has the Data* variable (defined in
// main.h) as input argument and it does not return any value (void output)
/* ************************************************************************** */
void initializeDataStructure(Data* pData)
{
    if (pData!=NULL)
    {
        pData->ndata=0;
        pData->niter=0;

        pData->pnu=NULL;
        pData->pop=NULL;

        pData->d0p=NULL;
        pData->d1p=NULL;
        pData->d2p=NULL;

        pData->tim=NULL;
        pData->ctim=NULL;

        pData->nprob=0;
        pData->pprob=NULL;

        pData->nmat=0;
        pData->pmat=NULL;
    }

    return;
}

/* ************************************************************************** */
// The function freeDataMemory frees the memory dynamically allocated with
// calloc/malloc/realloc for the Data structure (but other variables than
// pointers are not reset to zero). It has the Data* variable (defined in
// main.h) as input argument and it does not return any value (void output)
/* ************************************************************************** */
void freeDataMemory(Data* pData)
{
    int i=0, iMax=0;

    if (pData!=NULL)
    {
        iMax=pData->ndata;
        if (iMax<1 && (pData->pprob!=NULL || pData->pmat!=NULL))
        {
             fprintf(stdout,"\nWarning in freeDataMemory function: ");
             fprintf(stdout,"%d iterations are saved in pData (input) ",iMax);
             fprintf(stdout,"variable. Some allocated pointers may not be ");
             fprintf(stdout,"freed up correctly.\n");
        }

        free(pData->pnu); // free function does not return any value
        pData->pnu=NULL;

        free(pData->pop);
        pData->pop=NULL;

        free(pData->d0p);
        pData->d0p=NULL;

        free(pData->d1p);
        pData->d1p=NULL;

        free(pData->d2p);
        pData->d2p=NULL;

        free(pData->tim);
        pData->tim=NULL;

        free(pData->ctim);
        pData->ctim=NULL;

        if (pData->pprob!=NULL)
        {
            for (i=0; i<iMax; i++)
            {
                free(pData->pprob[i].pk);
                pData->pprob[i].pk=NULL;

                free(pData->pprob[i].pkl);
                pData->pprob[i].pkl=NULL;
            }
        }
        free(pData->pprob);
        pData->pprob=NULL;

        if (pData->pmat!=NULL)
        {
            for (i=0; i<iMax; i++)
            {
                free(pData->pmat[i].coef);
                pData->pmat[i].coef=NULL;

                free(pData->pmat[i].diag);
                pData->pmat[i].diag=NULL;

                free(pData->pmat[i].vect);
                pData->pmat[i].vect=NULL;
            }
        }
        free(pData->pmat);
        pData->pmat=NULL;
    }

    return;
}

/* ************************************************************************** */
// The function allocateMemoryForData dynamically allocates the memory for
// the Data structure according to pChemicalSystem and pParameters. It has the
// Parameters*, Data*, and ChemicalSystem* variables (both defined in main.h)
// as input arguments and it returns one one succes otherwise zero for an error
/* ************************************************************************** */
int allocateMemoryForData(Parameters* pParameters, Data* pData,
                                                ChemicalSystem* pChemicalSystem)
{
    size_t length=0;
    int i=0, iMax=0;
    Probabilities* pProbabilities=NULL;
    OverlapMatrix* pOverlapMatrix=NULL;

    // Check the input variables
    if (pParameters==NULL || pData==NULL || pChemicalSystem==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: at least one of the input ");
        fprintf(stderr,"variables pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pData=%p, or ",(void*)pData);
        fprintf(stderr,"pChemicalSystem=%p does not ",(void*)pChemicalSystem);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    if (pChemicalSystem->nmorb<1)
    {
        PRINT_ERROR("allocateMemoryForData: the total number of electrons ");
        fprintf(stderr,"(=%d) saved in the ",pChemicalSystem->nmorb);
        fprintf(stderr,"structure pointed by pChemicalSystem should be a ");
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    if (pParameters->iter_max<0)
    {
        PRINT_ERROR("In allocateMemoryForData: the maximal number of ");
        fprintf(stderr,"iterations saved in the structure pointed by ");
        fprintf(stderr,"pParameters should be a positive integer instead of ");
        fprintf(stderr,"%d.\n",pParameters->iter_max);
        return 0;
    }
    pData->ndata=pParameters->iter_max+1;
    pData->niter=0;

    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=pData->ndata;
    pData->pnu=(double*)calloc(length,sizeof(double));
    if (pData->pnu==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->pnu variable.\n");
        return 0;
    }

    pData->pop=(double*)calloc(length,sizeof(double));
    if (pData->pop==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->pop variable.\n");
        return 0;
    }

    pData->d0p=(double*)calloc(length,sizeof(double));
    if (pData->d0p==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->d0p variable.\n");
        return 0;
    }

    pData->d1p=(double*)calloc(length,sizeof(double));
    if (pData->d1p==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->d1p variable.\n");
        return 0;
    }

    pData->d2p=(double*)calloc(length,sizeof(double));
    if (pData->d2p==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->d2p variable.\n");
        return 0;
    }

    pData->tim=(double*)calloc(length,sizeof(double));
    if (pData->tim==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->tim variable.\n");
        return 0;
    }

    pData->ctim=(double*)calloc(length,sizeof(double));
    if (pData->ctim==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->ctim variable.\n");
        return 0;
    }

    pData->pprob=(Probabilities*)calloc(length,sizeof(Probabilities));
    if (pData->pprob==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->pprob variable.\n");
        return 0;
    }

    pData->pmat=(OverlapMatrix*)calloc(length,sizeof(OverlapMatrix));
    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In allocateMemoryForData: could not allocate memory for ");
        fprintf(stderr,"the pData->pmat variable.\n");
        return 0;
    }

    pData->nprob=pChemicalSystem->nmorb+1;
    length=pData->nprob;
    iMax=pData->ndata;
    for (i=0; i<iMax; i++)
    {
        pProbabilities=&pData->pprob[i];
        pProbabilities->pk=(double*)calloc(length,sizeof(double));
        if (pProbabilities->pk==NULL)
        {
            PRINT_ERROR("In allocateMemoryForData: could not allocate memory ");
            fprintf(stderr,"for the pk variable of the %d-th ",i+1);
            fprintf(stderr,"Probabilities structure.\n");
            return 0;
        }

        pProbabilities->pkl=(double*)calloc(length*length,sizeof(double));
        if (pProbabilities->pkl==NULL)
        {
            PRINT_ERROR("In allocateMemoryForData: could not allocate memory ");
            fprintf(stderr,"for the pkl variable of the %d-th ",i+1);
            fprintf(stderr,"Probabilities structure.\n");
            return 0;
        }
    }

    pData->nmat=pChemicalSystem->nmorb;
    length=pData->nmat;
    for (i=0; i<iMax; i++)
    {
        pOverlapMatrix=&pData->pmat[i];
        pOverlapMatrix->coef=(double*)calloc(length*length,sizeof(double));
        if (pOverlapMatrix->coef==NULL)
        {
            PRINT_ERROR("In allocateMemoryForData: could not allocate memory ");
            fprintf(stderr,"for the coef variable of the %d-th ",i+1);
            fprintf(stderr,"OverlapMatrix structure.\n");
            return 0;
        }

        pOverlapMatrix->diag=(double*)calloc(length,sizeof(double));
        if (pOverlapMatrix->diag==NULL)
        {
            PRINT_ERROR("In allocateMemoryForData: could not allocate memory ");
            fprintf(stderr,"for the diag variable of the %d-th ",i+1);
            fprintf(stderr,"OverlapMatrix structure.\n");
            return 0;
        }

        pOverlapMatrix->vect=(double*)calloc(length*length,sizeof(double));
        if (pOverlapMatrix->vect==NULL)
        {
            PRINT_ERROR("In allocateMemoryForData: could not allocate memory ");
            fprintf(stderr,"for the vect variable of the %d-th ",i+1);
            fprintf(stderr,"OverlapMatrix structure.\n");
            return 0;
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function addLengthForFileName checks that all the file names stored in
// the structure pointed by pParameters are string of correct length (strictly
// less than the pParameters->name_length variable and more than five) and that
// they ends with the correct extension (*.info, *.data, *.chem, *.mesh, and
// if needed *.elas). Then, it looks at the length of these names and if it
// remains less than pParameters->name_length when an additionalLength is added
// nothing is done. Otherwise, it dynamically reallocate memory so that each
// file names can store name of size lower than pParameters->name_length +
// additionalLength. Finally, it updates the pParameters->name_length variable.
// It has the Parameters* (defined in main.h) and the int additionalLength
// variables as input arguments and it returns one one succes otherwise zero
/* ************************************************************************** */
int addLengthForFileName(Parameters* pParameters, int additionalLength)
{
    size_t lengthName=0;
    char *pName=NULL;
    int boolean=0;

    // Check that the input pParameters variable is not pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In addLengthForFileName: the input pParameters ");
        fprintf(stderr,"variable is pointing to the %p ",(void*)pParameters);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check that the additional length variable is positive
    if (additionalLength<1)
    {
        PRINT_ERROR("In addLengthForFileName: the input additionalLength ");
        fprintf(stderr,"variable (=%d) should be a positive ",additionalLength);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    // Check the pParameters->name_info variable
    if (!checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In addLengthForFileName: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the char* ");
        fprintf(stderr,"name_info variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.info extension).\n");
        return 0;
    }

    // strlen returns the length of the string without the terminating '\0' char
    lengthName=strlen(pParameters->name_info);
    if (pParameters->name_info[lengthName-5]!='.' ||
                                    pParameters->name_info[lengthName-4]!='i' ||
                                    pParameters->name_info[lengthName-3]!='n' ||
                                    pParameters->name_info[lengthName-2]!='f' ||
                                    pParameters->name_info[lengthName-1]!='o' ||
                                       pParameters->name_info[lengthName]!='\0')
    {
        PRINT_ERROR("In addLengthForFileName: the name_info variable ");
        fprintf(stderr,"(=%s) of the structure ",pParameters->name_info);
        fprintf(stderr,"pointed by pParameters does not end with the '.info' ");
        fprintf(stderr,"extension.\n");
        return 0;
    }

    // Check the pParameters->name_data variable
    if (!checkStringFromLength(pParameters->name_data,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In addLengthForFileName: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the char* ");
        fprintf(stderr,"name_data variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.data extension).\n");
        return 0;
    }

    lengthName=strlen(pParameters->name_data);
    if (pParameters->name_data[lengthName-5]!='.' ||
                                    pParameters->name_data[lengthName-4]!='d' ||
                                    pParameters->name_data[lengthName-3]!='a' ||
                                    pParameters->name_data[lengthName-2]!='t' ||
                                    pParameters->name_data[lengthName-1]!='a' ||
                                       pParameters->name_data[lengthName]!='\0')
    {
        PRINT_ERROR("In addLengthForFileName: the name_data variable ");
        fprintf(stderr,"(=%s) of the structure ",pParameters->name_data);
        fprintf(stderr,"pointed by pParameters does not end with the '.data' ");
        fprintf(stderr,"extension.\n");
        return 0;
    }

    // Check the pParameters->name_chem variable
    if (!checkStringFromLength(pParameters->name_chem,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In addLengthForFileName: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the char* ");
        fprintf(stderr,"name_chem variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.chem extension).\n");
        return 0;
    }

    lengthName=strlen(pParameters->name_chem);
    if (pParameters->name_chem[lengthName-5]!='.' ||
                                    pParameters->name_chem[lengthName-4]!='c' ||
                                    pParameters->name_chem[lengthName-3]!='h' ||
                                    pParameters->name_chem[lengthName-2]!='e' ||
                                    pParameters->name_chem[lengthName-1]!='m' ||
                                       pParameters->name_chem[lengthName]!='\0')
    {
        PRINT_ERROR("In addLengthForFileName: the name_chem variable ");
        fprintf(stderr,"(=%s) of the structure ",pParameters->name_chem);
        fprintf(stderr,"pointed by pParameters does not end with the '.chem' ");
        fprintf(stderr,"extension.\n");
        return 0;
    }

    // Check the pParameters->name_mesh variable
    if (!checkStringFromLength(pParameters->name_mesh,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In addLengthForFileName: checkStringFromLength ");
        fprintf(stderr,"function returned zero, which is not the expected ");
        fprintf(stderr,"value here, after having checked that the char* ");
        fprintf(stderr,"name_mesh variable of the structure pointed by ");
        fprintf(stderr,"pParameters is not a string of length (strictly) ");
        fprintf(stderr,"less than %d (and more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.mesh extension).\n");
        return 0;
    }

    lengthName=strlen(pParameters->name_mesh);
    if (pParameters->name_mesh[lengthName-5]!='.' ||
                                    pParameters->name_mesh[lengthName-4]!='m' ||
                                    pParameters->name_mesh[lengthName-3]!='e' ||
                                    pParameters->name_mesh[lengthName-2]!='s' ||
                                    pParameters->name_mesh[lengthName-1]!='h' ||
                                       pParameters->name_mesh[lengthName]!='\0')
    {
        PRINT_ERROR("In addLengthForFileName: the name_mesh variable ");
        fprintf(stderr,"(=%s) of the structure ",pParameters->name_mesh);
        fprintf(stderr,"pointed by pParameters does not end with the '.mesh' ");
        fprintf(stderr,"extension.\n");
        return 0;
    }

    // Check the pParameters->name_elas variable
    if (pParameters->opt_mode==1 || pParameters->opt_mode==2)
    {
        if (!checkStringFromLength(pParameters->name_elas,7,
                                                      pParameters->name_length))
        {
            PRINT_ERROR("In addLengthForFileName: checkStringFromLength ");
            fprintf(stderr,"function returned zero, which is not the ");
            fprintf(stderr,"expected value here, after having checked that ");
            fprintf(stderr,"the char* name_elas variable of the structure ");
            fprintf(stderr,"pointed by pParameters is not a string of length ");
            fprintf(stderr,"(strictly) less than %d ",pParameters->name_length);
            fprintf(stderr,"(and more than 5 in order to store at least ");
            fprintf(stderr,"something more than the *.elas extension).\n");
            return 0;
        }

        lengthName=strlen(pParameters->name_elas);
        if (pParameters->name_elas[lengthName-5]!='.' ||
                                    pParameters->name_elas[lengthName-4]!='e' ||
                                    pParameters->name_elas[lengthName-3]!='l' ||
                                    pParameters->name_elas[lengthName-2]!='a' ||
                                    pParameters->name_elas[lengthName-1]!='s' ||
                                       pParameters->name_elas[lengthName]!='\0')
        {
            PRINT_ERROR("In addLengthForFileName: the name_elas variable ");
            fprintf(stderr,"(=%s) of the structure ",pParameters->name_elas);
            fprintf(stderr,"pointed by pParameters does not end with the ");
            fprintf(stderr,"'.elas' extension.\n");
            return 0;
        }
    }
    else
    {
        // Case where no *.elas file is needed and should not be prescribed
        if (pParameters->name_elas!=NULL)
        {
            PRINT_ERROR("In addLengthForFileName: the opt_mode ");
            fprintf(stderr,"(=%d) variable of the ",pParameters->opt_mode);
            fprintf(stderr,"structure pointed by pParameters does not ");
            fprintf(stderr,"correspond to the optimization mode where an ");
            fprintf(stderr,"*.elas file is needed but somehow the name_elas ");
            fprintf(stderr,"variable of the structure pointed by pParameters ");
            fprintf(stderr,"is pointing to the ");
            fprintf(stderr,"%p address, which ",(void*)pParameters->name_elas);
            fprintf(stderr,"is not allowed here.\n");
            return 0;
        }
    }

    // Reallocate memory for the name of the saved meshes (*.00010.mesh)
    boolean=0;
    lengthName=strlen(pParameters->name_info);
    boolean+=(pParameters->name_length<=(int)lengthName+additionalLength);

    lengthName=strlen(pParameters->name_data);
    boolean+=(pParameters->name_length<=(int)lengthName+additionalLength);

    lengthName=strlen(pParameters->name_chem);
    boolean+=(pParameters->name_length<=(int)lengthName+additionalLength);

    lengthName=strlen(pParameters->name_mesh);
    boolean+=(pParameters->name_length<=(int)lengthName+additionalLength);

    if (pParameters->name_elas!=NULL)
    {
        lengthName=strlen(pParameters->name_elas);
        boolean+=(pParameters->name_length<=(int)lengthName+additionalLength);
    }

    if (boolean)
    {
        // realloc returns a pointer to allocated memory, otherwise NULL
        lengthName=pParameters->name_length+additionalLength;
        pName=(char*)realloc(pParameters->name_info,lengthName*sizeof(char));
        if (pName==NULL)
        {
            PRINT_ERROR("In addLengthForFileName: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->name_info variable.\n");
            return 0;
        }
        pParameters->name_info=pName;

        pName=(char*)realloc(pParameters->name_data,lengthName*sizeof(char));
        if (pName==NULL)
        {
            PRINT_ERROR("In addLengthForFileName: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->name_data variable.\n");
            return 0;
        }
        pParameters->name_data=pName;

        pName=(char*)realloc(pParameters->name_chem,lengthName*sizeof(char));
        if (pName==NULL)
        {
            PRINT_ERROR("In addLengthForFileName: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->name_chem variable.\n");
            return 0;
        }
        pParameters->name_chem=pName;

        pName=(char*)realloc(pParameters->name_mesh,lengthName*sizeof(char));
        if (pName==NULL)
        {
            PRINT_ERROR("In addLengthForFileName: could not reallocate ");
            fprintf(stderr,"memory for the pParameters->name_mesh variable.\n");
            return 0;
        }
        pParameters->name_mesh=pName;

        if (pParameters->name_elas!=NULL)
        {
            pName=(char*)realloc(pParameters->name_elas,
                                                       lengthName*sizeof(char));
            if (pName==NULL)
            {
                PRINT_ERROR("In addLengthForFileName: could not reallocate ");
                fprintf(stderr,"memory for the pParameters->name_elas ");
                fprintf(stderr,"variable.\n");
                return 0;
            }
            pParameters->name_elas=pName;
        }

        // Do not forget to update the pParameters->name_length variable
        pParameters->name_length=lengthName;
    }

    return 1;
}

/* ************************************************************************** */
// The function evaluatingPrimitiveAtVertices evaluates at the point (px,py,pz)
// the function associated to a Gaussian-type primitive of pMolecularOrbital
// described in pChemicalSystem. It has the ChemicalSystem*, MolecularOrbital*
// variables (both defined in main.h), the three point coordinates and the int
// characterizing the Gaussian-type primitive as input arguments. It returns
// the value of the associated function at the given point.
/* ************************************************************************** */
double evaluatingPrimitiveAtVertices(double px, double py, double pz,
                                     ChemicalSystem* pChemicalSystem,
                                     MolecularOrbital* pMolecularOrbital,
                                                                  int primitive)
{
    // First, if coefficient is zero, return zero to avoid waist of time
    double coefficient=pMolecularOrbital->coeff[primitive];
    if (coefficient==0.)
    {
        return 0.;
    }

    double exponent=pMolecularOrbital->exp[primitive];
    int type=pMolecularOrbital->type[primitive];

    // Warning: we stored Nuclei reference as in *.chem file so do not forget
    // to remove one from these integers when calling pChemicalSystem->pnucl
    int iNucleus=pMolecularOrbital->nucl[primitive];
    Nucleus* pCenter=&pChemicalSystem->pnucl[iNucleus-1];

    double dx=px-(pCenter->x);
    double dy=py-(pCenter->y);
    double dz=pz-(pCenter->z);

    double distanceSquared=dx*dx+dy*dy+dz*dz;
    double function=coefficient*exp(-exponent*distanceSquared);

    // Evaluate function according of the type of the Gaussian-type primitive
    // function (ORB_* are defined in main.h)
    switch (type)
    {
        // These preprocessor constants are defined in main.h (hence common to
        // all *.h files) because it is also used in optimization.c functions
        case ORB_1S:
            break;

        case ORB_2PX:
            function*=dx;
            break;

        case ORB_2PY:
            function*=dy;
            break;

        case ORB_2PZ:
            function*=dz;
            break;

        case ORB_3DXX:
            function*=dx*dx;
            break;

        case ORB_3DYY:
            function*=dy*dy;
            break;

        case ORB_3DZZ:
            function*=dz*dz;
            break;

        case ORB_3DXY:
            function*=dx*dy;
            break;

        case ORB_3DXZ:
            function*=dx*dz;
            break;

        case ORB_3DYZ:
            function*=dy*dz;
            break;

        case ORB_4FXXX:
            function*=dx*dx*dx;
            break;

        case ORB_4FYYY:
            function*=dy*dy*dy;
            break;

        case ORB_4FZZZ:
            function*=dz*dz*dz;
            break;

        case ORB_4FXXY:
            function*=dx*dx*dy;
            break;

        case ORB_4FXXZ:
            function*=dx*dx*dz;
            break;

        case ORB_4FYYZ:
            function*=dy*dy*dz;
            break;

        case ORB_4FXYY:
            function*=dx*dy*dy;
            break;

        case ORB_4FXZZ:
            function*=dx*dz*dz;
            break;

        case ORB_4FYZZ:
            function*=dy*dz*dz;
            break;

        case ORB_4FXYZ:
            function*=dx*dy*dz;
            break;

        default:
            fprintf(stdout,"\nWarning in evaluatingPrimitiveAtVertices ");
            fprintf(stdout,"function: the local type (=%d) variable ",type);
            fprintf(stdout,"should be a (positive) integer not (strictly) ");
            fprintf(stdout,"greater than twenty here. We have treated it as ");
            fprintf(stdout,"if it was equal to one (1s-type orbital).\n");
            break;
    }

    return function;
}

/* ************************************************************************** */
// The function evaluateOrbitalsIJ computes the value of Orb(i)*Orb(j) at the
// point (px,py,pz), where the orbitals' description are stored in the
// structures pointed by pMolecularOrbitalI and pMolecularOrbitalJ of
// pChemicalSystem. It has the ChemicalSystem*, the two MolecularOrbital*
// variables (both defined in main.h), and the three point coordinates as input
// arguments, and it returns the value of the function (Orb_i*Orb_j)(px,py,pz)
/* ************************************************************************** */
double evaluateOrbitalsIJ(double px, double py, double pz,
                          ChemicalSystem* pChemicalSystem,
                          MolecularOrbital* pMolecularOrbitalI,
                                           MolecularOrbital* pMolecularOrbitalJ)
{
    int k=0;
    int kMax=pChemicalSystem->ngauss;

    double functionI=0.;
    double functionJ=0.;

    // Case where Orbitals i and j have different spin (to avoid waist of time)
    if (pMolecularOrbitalI->spin!=pMolecularOrbitalJ->spin)
    {
        return 0.;
    }

    for (k=0; k<kMax; k++)
    {
        // Evaluating function for Orbital i and j
        functionI+=evaluatingPrimitiveAtVertices(px,py,pz,pChemicalSystem,
                                                          pMolecularOrbitalI,k);
        functionJ+=evaluatingPrimitiveAtVertices(px,py,pz,pChemicalSystem,
                                                          pMolecularOrbitalJ,k);
    }

    return functionI*functionJ;
}

/* ************************************************************************** */
// The function evaluateOrbitalsII computes the value of Orb(i)*Orb(i) at the
// point (px,py,pz), where the orbital description is stored in the structure
// pointed by pMolecularOrbitalI of pChemicalSystem. It has the ChemicalSystem*,
// the MolecularOrbital* variables (both defined in main.h), and the three point
// coordinates as input arguments, and it returns the value of the function
// (Orb_i)(px,py,pz)*(Orb_i)(px,py,pz)
/* ************************************************************************** */
double evaluateOrbitalsII(double px, double py, double pz,
                          ChemicalSystem* pChemicalSystem,
                                           MolecularOrbital* pMolecularOrbitalI)
{
    int k=0;
    int kMax=pChemicalSystem->ngauss;
    double function=0.;

    for (k=0; k<kMax; k++)
    {
        // Evaluating function for Orbital i
        function+=evaluatingPrimitiveAtVertices(px,py,pz,pChemicalSystem,
                                                          pMolecularOrbitalI,k);
    }

    return function*function;
}

/* ************************************************************************** */
// The function computeVolumeTetrahedron evaluates the volume (in fact 6 times
// the volume) of a tetrehedron defined by four points. It has the four Point*
// (defined in main.h) variables as input arguments, and it returns the volume
// of the tetrahedron frame (12,13,14) in terms of determinant (vector product)
/* ************************************************************************** */
double computeVolumeTetrahedron(Point* pp1, Point* pp2, Point* pp3, Point* pp4)
{
    double a11=pp2->x;
    double a12=pp3->x;
    double a13=pp4->x;

    double a21=pp2->y;
    double a22=pp3->y;
    double a23=pp4->y;

    double a31=pp2->z;
    double a32=pp3->z;
    double a33=pp4->z;

    double determinant=pp1->x;
    a11-=determinant;
    a12-=determinant;
    a13-=determinant;

    determinant=pp1->y;
    a21-=determinant;
    a22-=determinant;
    a23-=determinant;

    determinant=pp1->z;
    a31-=determinant;
    a32-=determinant;
    a33-=determinant;

    determinant=a11*(a22*a33-a32*a23)-a21*(a12*a33-a32*a13)+
                                                          a31*(a12*a23-a22*a13);

    // Remark: in fact we have computed 6*volumeOfTetrahedron but it is
    // already included in the sum of the weight coefficients
    // (6*CST_A+4*CST_B+4*CST_C=1/6) for 3D numerical integration formula

    return DEF_ABS(determinant);
}

/* ************************************************************************** */
// The function computeOverlapMatrix calculates the overlap matrix S(Omega)_ij
// defined by int_Omega Orb(i)*Orb(j), where the molecular orbitals' is stored
// in the structure pointed by pChemicalSystem. The domain of integration Omega
// corresponds to the tetrahedra of pMesh that are not labelled by labelToAvoid
// and the coefficients of the overlap matrix are stored in the coef array
// variable of the OverlapMatrix structure located at
// pData->pmat[iterationInTheLoop]. It has the Parameters*, Mesh*, Data*,
// ChemicalSystem* variables (both defined in main.h), and the two integers
// labelToAvoid and iterationInTheLoop as input arguments. It returns one if
// the coefficients of the overlap matrix have been successfully computed,
// otherwise zero is returned if an error is encountered during the process
/* ************************************************************************** */
int computeOverlapMatrix(Parameters* pParameters, Mesh* pMesh, Data* pData,
                         ChemicalSystem* pChemicalSystem, int labelToAvoid,
                                                         int iterationInTheLoop)
{
    int i=0, j=0, k=0, ip1=0, ip2=0, ip3=0, ip4=0, nTet=0, nMorb=0, nGauss=0;
    double integral=0., integralK=0., volumeTetrahedron=0., value=0., px=0.;
    double py=0., pz=0.;
    MolecularOrbital *pMolecularOrbitalI=NULL, *pMolecularOrbitalJ=NULL;
    Point  *pp1=NULL, *pp2=NULL, *pp3=NULL, *pp4=NULL;
    OverlapMatrix *pOverlapMatrix=NULL;

    // Check input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                                                          pChemicalSystem==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrix: at least one of the input ");
        fprintf(stderr,"variables pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, pData=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"or pChemicalSystem=%p ",(void*)pChemicalSystem);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check input integers
    if (labelToAvoid!=-1 && labelToAvoid!=2 && labelToAvoid!=3)
    {
        PRINT_ERROR("computeOverlapMatrix: the input labelToAvoid variable ");
        fprintf(stderr,"(=%d) can only be set to minus one, ",labelToAvoid);
        fprintf(stderr,"two or three.\n");
        return 0;
    }

    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("computeOverlapMatrix: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"non-negative integer not (strictly) greater than ");
        fprintf(stderr,"the maximal number of allowed iterations ");
        fprintf(stderr,"(=%d) in the optimization ",pParameters->iter_max);
        fprintf(stderr,"loop.\n");
        return 0;
    }

    // Check that the mesh is made of tetrahedra
    if (pParameters->opt_mode<=0  || pMesh->ptet==NULL)
    {
        PRINT_ERROR("computeOverlapMatrix: the function can only be used ");
        fprintf(stderr,"when the optimization mode ");
        fprintf(stderr,"(=%d) is positive i.e. the ",pParameters->opt_mode);
        fprintf(stderr," structure pointed by pMesh is made of tetrahedra ");
        fprintf(stderr,"(pMesh->ptet=%p).\n",(void*)pMesh->ptet);
        return 0;
    }

    if (pParameters->opt_mode!=1 || !iterationInTheLoop)
    {
        fprintf(stdout,"\nEvaluating the overlap matrix S(%d) ",labelToAvoid);
        fprintf(stdout,"on mesh.\n");
    }
    else
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\nEvaluating the overlap matrix ");
            fprintf(stdout,"S(%d) on mesh.\n",labelToAvoid);
        }
        else
        {
            fprintf(stdout,"65 %% done.\n");
        }
        
    }

    // Set the pointer for the overlap matrix
    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrix: the pmat variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pmat);
        return 0;
    }
    pOverlapMatrix=&pData->pmat[iterationInTheLoop];

    // Check the coef pointer variable of pOverlapMatrix
    if (pOverlapMatrix->coef==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrix: the coef variable of the ");
        fprintf(stderr,"%d-th OverlapMatrix structure ",iterationInTheLoop+1);
        fprintf(stderr,"is pointing to the %p ",(void*)pOverlapMatrix->coef);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check mesh and chemical parameters
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In computeOverlapMatrix: the total number of vertices ");
        fprintf(stderr,"(=%d) in the mesh should be a positive ",pMesh->nver);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrix: the pver variable of the ");
        fprintf(stderr,"structure pointed by pMesh is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pMesh->pver);
        return 0;
    }

    nTet=pMesh->ntet;
    if (nTet<1)
    {
        PRINT_ERROR("In computeOverlapMatrix: the total number of tetrahedra ");
        fprintf(stderr,"(=%d) in the mesh should be a positive ",nTet);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    nMorb=pChemicalSystem->nmorb;
    if (nMorb<1)
    {
        PRINT_ERROR("In computeOverlapMatrix: the total number of molecular ");
        fprintf(stderr,"orbitals (=%d) should be a positive integer.\n",nMorb);
        return 0;
    }

    if (pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrix: the pmorb variable of the ");
        fprintf(stderr,"structure pointed by pChemicalSystem is pointing to ");
        fprintf(stderr,"the %p address.\n",(void*)pChemicalSystem->pmorb);
        return 0;
    }

    nGauss=pChemicalSystem->ngauss;
    if (nGauss<1)
    {
        PRINT_ERROR("In computeOverlapMatrix: the total number of ");
        fprintf(stderr,"Gaussian-type primitives (=%d) should be a ",nGauss);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    if (pData->nmat!=nMorb)
    {
        PRINT_ERROR("In computeOverlapMatrix: the size of the overlap matrix ");
        fprintf(stderr,"should be (%d)x(%d) instead of ",nMorb,nMorb);
        fprintf(stderr,"(%d)x(%d).\n",pData->nmat,pData->nmat);
        return 0;
    }

    if (pParameters->orb_rhf)
    {
        if (nMorb%2!=0)
        {
            PRINT_ERROR("In computeOverlapMatrix: since we are dealing with ");
            fprintf(stderr,"a restricted Hartree-Fock system ");
            fprintf(stderr,"(pParameters->orb_rhf=%d) ",pParameters->orb_rhf);
            fprintf(stderr,"the total number of molecular orbitals ");
            fprintf(stderr," (=%d) should be an even number.\n",nMorb);
            return 0;
        }
    }

    // Initialization of the coefficients of the overlap matrix to zero
    for (i=0; i<nMorb; i++)
    {
        for (j=0; j<nMorb; j++)
        {
            pOverlapMatrix->coef[i*nMorb+j]=0.;
        }
    }

    // Evaluation of the symmetric overlap matrix S
    for (i=0; i<nMorb; i++)
    {
        pMolecularOrbitalI=&pChemicalSystem->pmorb[i];

         // Avoid calculation for restricted Hartree-Fock spin repetitions
        if (pParameters->orb_rhf)
        {
            if (i>=nMorb/2)
            {
                continue;
            }
        }

        for (j=0; j<i; j++)
        {
            pMolecularOrbitalJ=&pChemicalSystem->pmorb[j];

            // Case where Orbitals i and j have different spin (and take into
            // account the case of restricted Hartree-Fock spin repetitions)
            if (pParameters->orb_rhf)
            {
                k=(pMolecularOrbitalI->spin==1 && pMolecularOrbitalJ->spin==1);
            }
            else
            {
                k=(pMolecularOrbitalI->spin==pMolecularOrbitalJ->spin);
            }
            if (!k)
            {
                continue;
            }

            integralK=0.;
#pragma omp parallel for reduction(+:integralK) private(integral,volumeTetrahedron,value,ip1,ip2,ip3,ip4,pp1,pp2,pp3,pp4,px,py,pz)
            for (k=0; k<nTet; k++)
            {
                // Skip the label labelToAvoid for tetrahedra
                if (pMesh->ptet[k].label==labelToAvoid)
                {
                    continue;
                }

                // Get the adresses of the vertices of the k-th tetrahedron
                ip1=pMesh->ptet[k].p1;
                ip2=pMesh->ptet[k].p2;
                ip3=pMesh->ptet[k].p3;
                ip4=pMesh->ptet[k].p4;

                // Warning here: we have stored point reference as given
                // in the *.mesh file so do not forget to remove one from these
                // integers when calling the pMesh->pver variables
                pp1=&pMesh->pver[ip1-1];
                pp2=&pMesh->pver[ip2-1];
                pp3=&pMesh->pver[ip3-1];
                pp4=&pMesh->pver[ip4-1];

                // 1st to 6th point estimation
                px=CST_a*((pp1->x)+(pp2->x))+CST_aa*((pp3->x)+(pp4->x));
                py=CST_a*((pp1->y)+(pp2->y))+CST_aa*((pp3->y)+(pp4->y));
                pz=CST_a*((pp1->z)+(pp2->z))+CST_aa*((pp3->z)+(pp4->z));
                value=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                         pMolecularOrbitalI,pMolecularOrbitalJ);

                px=CST_a*((pp1->x)+(pp3->x))+CST_aa*((pp2->x)+(pp4->x));
                py=CST_a*((pp1->y)+(pp3->y))+CST_aa*((pp2->y)+(pp4->y));
                pz=CST_a*((pp1->z)+(pp3->z))+CST_aa*((pp2->z)+(pp4->z));
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_a*((pp1->x)+(pp4->x))+CST_aa*((pp2->x)+(pp3->x));
                py=CST_a*((pp1->y)+(pp4->y))+CST_aa*((pp2->y)+(pp3->y));
                pz=CST_a*((pp1->z)+(pp4->z))+CST_aa*((pp2->z)+(pp3->z));
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_a*((pp2->x)+(pp3->x))+CST_aa*((pp1->x)+(pp4->x));
                py=CST_a*((pp2->y)+(pp3->y))+CST_aa*((pp1->y)+(pp4->y));
                pz=CST_a*((pp2->z)+(pp3->z))+CST_aa*((pp1->z)+(pp4->z));
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_a*((pp2->x)+(pp4->x))+CST_aa*((pp1->x)+(pp3->x));
                py=CST_a*((pp2->y)+(pp4->y))+CST_aa*((pp1->y)+(pp3->y));
                pz=CST_a*((pp2->z)+(pp4->z))+CST_aa*((pp1->z)+(pp3->z));
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_a*((pp3->x)+(pp4->x))+CST_aa*((pp1->x)+(pp2->x));
                py=CST_a*((pp3->y)+(pp4->y))+CST_aa*((pp1->y)+(pp2->y));
                pz=CST_a*((pp3->z)+(pp4->z))+CST_aa*((pp1->z)+(pp2->z));
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                value*=CST_A;
                integral=value;

                // 7th to 10th point estimation
                px=CST_b*((pp1->x)+(pp2->x)+(pp3->x))+CST_bb*(pp4->x);
                py=CST_b*((pp1->y)+(pp2->y)+(pp3->y))+CST_bb*(pp4->y);
                pz=CST_b*((pp1->z)+(pp2->z)+(pp3->z))+CST_bb*(pp4->z);
                value=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                         pMolecularOrbitalI,pMolecularOrbitalJ);

                px=CST_b*((pp1->x)+(pp2->x)+(pp4->x))+CST_bb*(pp3->x);
                py=CST_b*((pp1->y)+(pp2->y)+(pp4->y))+CST_bb*(pp3->y);
                pz=CST_b*((pp1->z)+(pp2->z)+(pp4->z))+CST_bb*(pp3->z);
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_b*((pp1->x)+(pp3->x)+(pp4->x))+CST_bb*(pp2->x);
                py=CST_b*((pp1->y)+(pp3->y)+(pp4->y))+CST_bb*(pp2->y);
                pz=CST_b*((pp1->z)+(pp3->z)+(pp4->z))+CST_bb*(pp2->z);
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_b*((pp2->x)+(pp3->x)+(pp4->x))+CST_bb*(pp1->x);
                py=CST_b*((pp2->y)+(pp3->y)+(pp4->y))+CST_bb*(pp1->y);
                pz=CST_b*((pp2->z)+(pp3->z)+(pp4->z))+CST_bb*(pp1->z);
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                value*=CST_B;
                integral+=value;

                // 10th to 14th point estimation
                px=CST_c*((pp1->x)+(pp2->x)+(pp3->x))+CST_cc*(pp4->x);
                py=CST_c*((pp1->y)+(pp2->y)+(pp3->y))+CST_cc*(pp4->y);
                pz=CST_c*((pp1->z)+(pp2->z)+(pp3->z))+CST_cc*(pp4->z);
                value=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                         pMolecularOrbitalI,pMolecularOrbitalJ);

                px=CST_c*((pp1->x)+(pp2->x)+(pp4->x))+CST_cc*(pp3->x);
                py=CST_c*((pp1->y)+(pp2->y)+(pp4->y))+CST_cc*(pp3->y);
                pz=CST_c*((pp1->z)+(pp2->z)+(pp4->z))+CST_cc*(pp3->z);
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_c*((pp1->x)+(pp3->x)+(pp4->x))+CST_cc*(pp2->x);
                py=CST_c*((pp1->y)+(pp3->y)+(pp4->y))+CST_cc*(pp2->y);
                pz=CST_c*((pp1->z)+(pp3->z)+(pp4->z))+CST_cc*(pp2->z);
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                px=CST_c*((pp2->x)+(pp3->x)+(pp4->x))+CST_cc*(pp1->x);
                py=CST_c*((pp2->y)+(pp3->y)+(pp4->y))+CST_cc*(pp1->y);
                pz=CST_c*((pp2->z)+(pp3->z)+(pp4->z))+CST_cc*(pp1->z);
                value+=evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                          pMolecularOrbitalI,
                                                            pMolecularOrbitalJ);

                value*=CST_C;
                integral+=value;

                // Multiply by (six times) the volume of the tetrahedron
                volumeTetrahedron=computeVolumeTetrahedron(pp1,pp2,pp3,pp4);
                integral*=volumeTetrahedron;
                integralK=integralK+integral;
            }

            // End of the parallelization for the loop for
            pOverlapMatrix->coef[i*nMorb+j]=integralK;
            if (pParameters->opt_mode!=1 || pParameters->verbose ||
                                                            !iterationInTheLoop)
            {
                if (pParameters->orb_rhf || pParameters->verbose)
                {
                    fprintf(stdout,"%d %% done.\n",
                               (int)(100.*(i*.5*nMorb+j+1.)/(.25*nMorb*nMorb)));
                }
                else
                {
                    fprintf(stdout,"%d %% done.\n",
                                      (int)(100.*(i*nMorb+j+1.)/(nMorb*nMorb)));
                }
            }
        }

        // Treat the particular case i=j
        integralK=0.;
#pragma omp parallel for reduction(+:integralK) private(integral,volumeTetrahedron,value,ip1,ip2,ip3,ip4,pp1,pp2,pp3,pp4,px,py,pz)
        for (k=0; k<nTet; k++)
        {
            // Skip the label labelToAvoid for tetrahedra
            if (pMesh->ptet[k].label==labelToAvoid)
            {
                continue;
            }

            // Get the adresses of the vertices of the k-th tetrahedron
            ip1=pMesh->ptet[k].p1;
            ip2=pMesh->ptet[k].p2;
            ip3=pMesh->ptet[k].p3;
            ip4=pMesh->ptet[k].p4;

            // Warning here: we have stored point reference as given in the
            // *.mesh file so do not forget to remove one from these integers
            // when calling the pMesh->pver variable
            pp1=&pMesh->pver[ip1-1];
            pp2=&pMesh->pver[ip2-1];
            pp3=&pMesh->pver[ip3-1];
            pp4=&pMesh->pver[ip4-1];

            // 1st to 6th point estimation
            px=CST_a*((pp1->x)+(pp2->x))+CST_aa*((pp3->x)+(pp4->x));
            py=CST_a*((pp1->y)+(pp2->y))+CST_aa*((pp3->y)+(pp4->y));
            pz=CST_a*((pp1->z)+(pp2->z))+CST_aa*((pp3->z)+(pp4->z));
            value=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_a*((pp1->x)+(pp3->x))+CST_aa*((pp2->x)+(pp4->x));
            py=CST_a*((pp1->y)+(pp3->y))+CST_aa*((pp2->y)+(pp4->y));
            pz=CST_a*((pp1->z)+(pp3->z))+CST_aa*((pp2->z)+(pp4->z));
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_a*((pp1->x)+(pp4->x))+CST_aa*((pp2->x)+(pp3->x));
            py=CST_a*((pp1->y)+(pp4->y))+CST_aa*((pp2->y)+(pp3->y));
            pz=CST_a*((pp1->z)+(pp4->z))+CST_aa*((pp2->z)+(pp3->z));
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_a*((pp2->x)+(pp3->x))+CST_aa*((pp1->x)+(pp4->x));
            py=CST_a*((pp2->y)+(pp3->y))+CST_aa*((pp1->y)+(pp4->y));
            pz=CST_a*((pp2->z)+(pp3->z))+CST_aa*((pp1->z)+(pp4->z));
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_a*((pp2->x)+(pp4->x))+CST_aa*((pp1->x)+(pp3->x));
            py=CST_a*((pp2->y)+(pp4->y))+CST_aa*((pp1->y)+(pp3->y));
            pz=CST_a*((pp2->z)+(pp4->z))+CST_aa*((pp1->z)+(pp3->z));
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_a*((pp3->x)+(pp4->x))+CST_aa*((pp1->x)+(pp2->x));
            py=CST_a*((pp3->y)+(pp4->y))+CST_aa*((pp1->y)+(pp2->y));
            pz=CST_a*((pp3->z)+(pp4->z))+CST_aa*((pp1->z)+(pp2->z));
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            value*=CST_A;
            integral=value;

            // 7th to 10th point estimation
            px=CST_b*((pp1->x)+(pp2->x)+(pp3->x))+CST_bb*(pp4->x);
            py=CST_b*((pp1->y)+(pp2->y)+(pp3->y))+CST_bb*(pp4->y);
            pz=CST_b*((pp1->z)+(pp2->z)+(pp3->z))+CST_bb*(pp4->z);
            value=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_b*((pp1->x)+(pp2->x)+(pp4->x))+CST_bb*(pp3->x);
            py=CST_b*((pp1->y)+(pp2->y)+(pp4->y))+CST_bb*(pp3->y);
            pz=CST_b*((pp1->z)+(pp2->z)+(pp4->z))+CST_bb*(pp3->z);
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_b*((pp1->x)+(pp3->x)+(pp4->x))+CST_bb*(pp2->x);
            py=CST_b*((pp1->y)+(pp3->y)+(pp4->y))+CST_bb*(pp2->y);
            pz=CST_b*((pp1->z)+(pp3->z)+(pp4->z))+CST_bb*(pp2->z);
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_b*((pp2->x)+(pp3->x)+(pp4->x))+CST_bb*(pp1->x);
            py=CST_b*((pp2->y)+(pp3->y)+(pp4->y))+CST_bb*(pp1->y);
            pz=CST_b*((pp2->z)+(pp3->z)+(pp4->z))+CST_bb*(pp1->z);
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            value*=CST_B;
            integral+=value;

            // 10th to 14th point estimation
            px=CST_c*((pp1->x)+(pp2->x)+(pp3->x))+CST_cc*(pp4->x);
            py=CST_c*((pp1->y)+(pp2->y)+(pp3->y))+CST_cc*(pp4->y);
            pz=CST_c*((pp1->z)+(pp2->z)+(pp3->z))+CST_cc*(pp4->z);
            value=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_c*((pp1->x)+(pp2->x)+(pp4->x))+CST_cc*(pp3->x);
            py=CST_c*((pp1->y)+(pp2->y)+(pp4->y))+CST_cc*(pp3->y);
            pz=CST_c*((pp1->z)+(pp2->z)+(pp4->z))+CST_cc*(pp3->z);
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_c*((pp1->x)+(pp3->x)+(pp4->x))+CST_cc*(pp2->x);
            py=CST_c*((pp1->y)+(pp3->y)+(pp4->y))+CST_cc*(pp2->y);
            pz=CST_c*((pp1->z)+(pp3->z)+(pp4->z))+CST_cc*(pp2->z);
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            px=CST_c*((pp2->x)+(pp3->x)+(pp4->x))+CST_cc*(pp1->x);
            py=CST_c*((pp2->y)+(pp3->y)+(pp4->y))+CST_cc*(pp1->y);
            pz=CST_c*((pp2->z)+(pp3->z)+(pp4->z))+CST_cc*(pp1->z);
            value+=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

            value*=CST_C;
            integral+=value;

            // Multiply by (six times) the volume of the tetrahedron
            volumeTetrahedron=computeVolumeTetrahedron(pp1,pp2,pp3,pp4);
            integral*=volumeTetrahedron;
            integralK=integralK+integral;
        }

        // End of the parallelization for the loop for
        pOverlapMatrix->coef[i*nMorb+i]=integralK;
        if (pParameters->opt_mode!=1 || pParameters->verbose || 
                                                            !iterationInTheLoop)
        {
            if (pParameters->orb_rhf)
            {
                fprintf(stdout,"%d %% done.\n",
                               (int)(100.*(i*.5*nMorb+i+1.)/(.25*nMorb*nMorb)));
            }
            else
            {
                fprintf(stdout,"%d %% done.\n",
                                      (int)(100.*(i*nMorb+i+1.)/(nMorb*nMorb)));
            }
        }
    }

    // Normally, we do not need to complete the matrix which is symmetric
    // since LAPACK_dsyev function only need a triangular part. Warning:
    // since LAPACK is written in Fortran (interfaced with c by the lapacke
    // library) the matrices are stored colomn_wise by default. Here, we
    // complete the matrix in the case it might be used for the saving of data
    for (i=0; i<nMorb; i++)
    {
        for (j=i+1; j<nMorb; j++)
        {
            pOverlapMatrix->coef[i*nMorb+j]=pOverlapMatrix->coef[j*nMorb+i];
        }
    }

    // Complete the matrix in the case of a restricted Hartree-Fock system
    if (pParameters->orb_rhf)
    {
        k=nMorb/2;
        for (i=0; i<k; i++)
        {
            for (j=0; j<k; j++)
            {
                pOverlapMatrix->coef[(i+k)*nMorb+(j+k)]=
                                                pOverlapMatrix->coef[i*nMorb+j];
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function convertingType converts the integer type as given in the
// standard nomenclature of the *.wfn format (see the descriptions of the ORB_*
// preprocessor cosntants in main.h for further details) into another integer
// defined by x+10y+100z, where x, y, and z refers to the exponents in the
// x-sublayer, y-subayer, and z-sublayer. It has the int type variable as input
// arguments, which is expected to be comprised between one and twenty (i.e.
// equal to one of the ORB_* preprocessor constants). It returns the converted
// type and if the type variable is not between one and twenty, it returns zero
/* ************************************************************************** */
int convertingType(int type)
{
    int returnValue=0;

    switch (type)
    {
        default:
            fprintf(stdout,"\nWarning in convertingType function: the input ");
            fprintf(stdout,"type variable (=%d) should be a positive ",type);
            fprintf(stdout,"integer not (strictly) greater than twenty here. ");
            fprintf(stdout,"We have treated it as if it was equal to one ");
            fprintf(stdout,"(1s-type orbital).\n");
        case ORB_1S:
            returnValue=0;
            break;

        case ORB_2PX:
            returnValue=1;
            break;

        case ORB_2PY:
            returnValue=10;
            break;

        case ORB_2PZ:
            returnValue=100;
            break;

        case ORB_3DXX:
            returnValue=2;
            break;

        case ORB_3DYY:
            returnValue=20;
            break;

        case ORB_3DZZ:
            returnValue=200;
            break;

        case ORB_3DXY:
            returnValue=11;
            break;

        case ORB_3DXZ:
            returnValue=101;
            break;

        case ORB_3DYZ:
            returnValue=110;
            break;

        case ORB_4FXXX:
            returnValue=3;
            break;

        case ORB_4FYYY:
            returnValue=30;
            break;

        case ORB_4FZZZ:
            returnValue=300;
            break;

        case ORB_4FXXY:
            returnValue=12;
            break;

        case ORB_4FXXZ:
            returnValue=102;
            break;

        case ORB_4FXYY:
            returnValue=21;
            break;

        case ORB_4FYYZ:
            returnValue=120;
            break;

        case ORB_4FXZZ:
            returnValue=201;
            break;

        case ORB_4FYZZ:
            returnValue=210;
            break;

        case ORB_4FXYZ:
            returnValue=111;
            break;
    }

    return returnValue;
}

/* ************************************************************************** */
// The function evaluateGaussianIntegral computes the exact analytical
// expression of the one-dimensional Gaussian integral from t0 to t1 of
// t^exp0*exp(-t*t) with respect to the t-variable, and according to the
// seven different cases for the exponent variable (exp0=0...6). It uses the erf
// and exp function in order to compute the value of the integral. It has the
// int exp0 and the two double t0 and t1 variables as input arguments. It
// returns the value of the integral and if the exp0 variable is not comprised
// between zero and six, it returns the integral as if exp0=0, which corresponds
// to the case of the erf function (multiply by sqrt(pi) i.e. up to a constant)
/* ************************************************************************** */
double evaluateGaussianIntegral(int exp0, double t0, double t1)
{
    double integral=0.;

    switch (exp0)
    {
        default:
            fprintf(stdout,"Warning in evaluateGaussianIntegral function: ");
            fprintf(stdout,"the input exp0 variable (=%d) should only ",exp0);
            fprintf(stdout,"be set to zero, one, two, three, four, five, or ");
            fprintf(stdout,"six. We have treated it as zero (erf function).\n");
        case 0:
            integral=sqrt(M_PI)*(erf(t1)-erf(t0));
            break;

        case 1:
            integral=exp(-t0*t0)-exp(-t1*t1);
            break;

        case 2:
            integral=.5*sqrt(M_PI)*(erf(t1)-erf(t0));
            integral+=t0*exp(-t0*t0)-t1*exp(-t1*t1);
            break;

        case 3:
            integral=(1.+t0*t0)*exp(-t0*t0)-(1.+t1*t1)*exp(-t1*t1);
            break;

        case 4:
            integral=.75*sqrt(M_PI)*(erf(t1)-erf(t0));
            integral+=t0*(1.5+t0*t0)*exp(-t0*t0)-t1*(1.5+t1*t1)*exp(-t1*t1);
            break;

        case 5:
            integral=(2.+t0*t0*(2.+t0*t0))*exp(-t0*t0)
                                             -(2.+t1*t1*(2.+t1*t1))*exp(-t1*t1);
            break;

        case 6:
            integral=1.875*sqrt(M_PI)*(erf(t1)-erf(t0));
            integral+=t0*(3.75+t0*t0*(2.5+t0*t0))*exp(-t0*t0)
                                       -t1*(3.75+t1*t1*(2.5+t1*t1))*exp(-t1*t1);
            break;
    }

    return integral;
}

/* ************************************************************************** */
// The function evaluateOneIntegralIJ computes the exact analytical expression
// of the one-dimensional integral from t0 to t1 of
// 2*(t-cI)^expI*(t+cJ)^expJ*exp(-t^2) with respect to the t-variable, and
// according to the different sixteen cases for the exponent variables
// (expI=0...3, expJ=0..3). It expands the polynomial and then compute
// iteratively each mononomial integral thanks to the evaluateGaussianIntegral
// function. It has the two int (expI, expJ) and the four double variables
// (cI, cJ, t0, t1) as input arguments. It returns the value of the integral
// and the exponents variables are treated as zero if they are not comprised
// between zero and three
/* ************************************************************************** */
double evaluateOneIntegralIJ(int expI, int expJ, double cI, double cJ,
                                                           double t0, double t1)
{
    double integral=0.;

    switch (expI)
    {
        default:
            fprintf(stdout,"Warning in evaluateOneIntegralIJ function: ");
            fprintf(stdout,"the input expI variable (=%d) should only ",expI);
            fprintf(stdout,"be set to zero, one, two, or three. We have ");
            fprintf(stdout,"treated it as zero (1s-type orbital).\n");
        case 0:
            switch (expJ)
            {
                default:
                    fprintf(stdout,"Warning in evaluateOneIntegralIJ ");
                    fprintf(stdout,"function: the input expJ variable ");
                    fprintf(stdout,"(=%d) should only be set to zero, ",expJ);
                    fprintf(stdout,"one, two, or three. We have treated it ");
                    fprintf(stdout,"as zero (1s-type orbital).\n");
                case 0:
                    integral=evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 1:
                    integral=evaluateGaussianIntegral(1,t0,t1);
                    integral+=cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 2:
                    integral=evaluateGaussianIntegral(2,t0,t1);
                    integral+=2.*cJ*evaluateGaussianIntegral(1,t0,t1);
                    integral+=cJ*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 3:
                    integral=evaluateGaussianIntegral(3,t0,t1);
                    integral+=3.*cJ*evaluateGaussianIntegral(2,t0,t1);
                    integral+=3.*cJ*cJ*evaluateGaussianIntegral(1,t0,t1);
                    integral+=cJ*cJ*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;
            }
            break;

        case 1:
            switch (expJ)
            {
                default:
                    fprintf(stdout,"Warning in evaluateOneIntegralIJ ");
                    fprintf(stdout,"function: the input expJ variable ");
                    fprintf(stdout,"(=%d) should only be set to zero, ",expJ);
                    fprintf(stdout,"one, two, or three. We have treated it ");
                    fprintf(stdout,"as zero (1s-type orbital).\n");
                case 0:
                    integral=evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 1:
                    integral=evaluateGaussianIntegral(2,t0,t1);
                    integral+=(cJ-cI)*evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 2:
                    integral=evaluateGaussianIntegral(3,t0,t1);
                    integral+=(2.*cJ-cI)*evaluateGaussianIntegral(2,t0,t1);
                    integral+=cJ*(cJ-2.*cI)*evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*cJ*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 3:
                    integral=evaluateGaussianIntegral(4,t0,t1);
                    integral+=(3.*cJ-cI)*evaluateGaussianIntegral(3,t0,t1);
                    integral+=3.*cJ*(cJ-cI)*evaluateGaussianIntegral(2,t0,t1);
                    integral+=cJ*cJ*(cJ-3.*cI)
                                             *evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*cJ*cJ*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;
            }
            break;

        case 2:
            switch (expJ)
            {
                default:
                    fprintf(stdout,"Warning in evaluateOneIntegralIJ ");
                    fprintf(stdout,"function: the input expJ variable ");
                    fprintf(stdout,"(=%d) should only be set to zero, ",expJ);
                    fprintf(stdout,"one, two, or three. We have treated it ");
                    fprintf(stdout,"as zero (1s-type orbital).\n");
                case 0:
                    integral=evaluateGaussianIntegral(2,t0,t1);
                    integral-=2.*cI*evaluateGaussianIntegral(1,t0,t1);
                    integral+=cI*cI*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 1:
                    integral=evaluateGaussianIntegral(3,t0,t1);
                    integral+=(cJ-2.*cI)*evaluateGaussianIntegral(2,t0,t1);
                    integral+=cI*(cI-2.*cJ)*evaluateGaussianIntegral(1,t0,t1);
                    integral+=cI*cI*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 2:
                    integral=evaluateGaussianIntegral(4,t0,t1);
                    integral+=2.*(cJ-cI)*evaluateGaussianIntegral(3,t0,t1);
                    integral+=(cJ*cJ-4.*cI*cJ+cI*cI)
                                             *evaluateGaussianIntegral(2,t0,t1);
                    integral+=2.*cI*cJ*(cI-cJ)
                                             *evaluateGaussianIntegral(1,t0,t1);
                    integral+=cI*cI*cJ*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 3:
                    integral=evaluateGaussianIntegral(5,t0,t1);
                    integral+=(3.*cJ-2.*cI)*evaluateGaussianIntegral(4,t0,t1);
                    integral+=(cI*cI-6.*cI*cJ+3.*cJ*cJ)
                                             *evaluateGaussianIntegral(3,t0,t1);
                    integral+=cJ*(cJ*cJ-6.*cI*cJ+3.*cI*cI)
                                             *evaluateGaussianIntegral(2,t0,t1);
                    integral+=cI*cJ*cJ*(3.*cI-2.*cJ)
                                             *evaluateGaussianIntegral(1,t0,t1);
                    integral+=cI*cI*cJ*cJ*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;
            }
            break;

        case 3:
            switch (expJ)
            {
                default:
                    fprintf(stdout,"Warning in evaluateOneIntegralIJ ");
                    fprintf(stdout,"function: the input expJ variable ");
                    fprintf(stdout,"(=%d) should only be set to zero, ",expJ);
                    fprintf(stdout,"one, two, or three. We have treated it ");
                    fprintf(stdout,"as zero (1s-type orbital).\n");
                case 0:
                    integral=evaluateGaussianIntegral(3,t0,t1);
                    integral-=3.*cI*evaluateGaussianIntegral(2,t0,t1);
                    integral+=3.*cI*cI*evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*cI*cI*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 1:
                    integral=evaluateGaussianIntegral(4,t0,t1);
                    integral+=(cJ-3.*cI)*evaluateGaussianIntegral(3,t0,t1);
                    integral+=3.*cI*(cI-cJ)*evaluateGaussianIntegral(2,t0,t1);
                    integral+=cI*cI*(3.*cJ-cI)
                                             *evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*cI*cI*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 2:
                    integral=evaluateGaussianIntegral(5,t0,t1);
                    integral+=(2.*cJ-3.*cI)*evaluateGaussianIntegral(4,t0,t1);
                    integral+=(cJ*cJ-6.*cI*cJ+3.*cI*cI)
                                             *evaluateGaussianIntegral(3,t0,t1);
                    integral-=cI*(cI*cI-6.*cI*cJ+3.*cJ*cJ)
                                             *evaluateGaussianIntegral(2,t0,t1);
                    integral+=cI*cI*cJ*(3.*cJ-2.*cI)
                                             *evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*cI*cI*cJ*cJ*evaluateGaussianIntegral(0,t0,t1);
                    break;

                case 3:
                    integral=evaluateGaussianIntegral(6,t0,t1);
                    integral+=3.*(cJ-cI)*evaluateGaussianIntegral(5,t0,t1);
                    integral+=3.*(cJ*cJ-3.*cI*cJ+cI*cI)
                                             *evaluateGaussianIntegral(4,t0,t1);
                    integral+=(cJ-cI)*(cI*cI-8.*cI*cJ+cJ*cJ)
                                             *evaluateGaussianIntegral(3,t0,t1);
                    integral-=3.*cI*cJ*(cJ*cJ-3.*cI*cJ+cI*cI)
                                             *evaluateGaussianIntegral(2,t0,t1);
                    integral+=3.*cI*cI*cJ*cJ*(cJ-cI)
                                             *evaluateGaussianIntegral(1,t0,t1);
                    integral-=cI*cI*cI*cJ*cJ*cJ
                                             *evaluateGaussianIntegral(0,t0,t1);
                    break;
            }
            break;
    }

    return integral;
}

/* ************************************************************************** */
// The function evaluateTripleIntegralIJ evaluates on the hexahedron stored at
// the kHexahedron-th position in the array pointed by pMesh->phex the exact
// analytical expression of the three-dimensional integral of prim(i)*prim(j),
// where prim(i), prim(j) refers to the Gaussian-type primitives referred to
// as primitiveI and primitiveJ in the MolecularOrbital structures pointed by
// pMolecularOrbitalI and pMolecularOrbitalJ in pChemicalSystem. To get the
// exact expression, we use the fact that the domain of integration is a cube
// so the separation of variables can be applied on the integrand (product of
// two Gaussian-type primitives) followed by a change of variables
// tx=sqrt(expI+expJ)*[x-(expI*nuclIx+expJ*nuclJx)/(expI+expJ)] (where similar
// expressions hold for the y and z coordinates). It has the Parameters*, the
// Mesh*, the ChemicalSystem*, the two MolecularOrbital* variables (both defined
// in main.h) and the three integers primitiveI, primitiveJ, and kHexahedron as
// input arguments. It returns the value of the integral over the hexahedron
/* ************************************************************************** */
double evaluateTripleIntegralIJ(Parameters* pParameters, Mesh *pMesh,
                                ChemicalSystem *pChemicalSystem,
                                MolecularOrbital *pMolecularOrbitalI,
                                MolecularOrbital *pMolecularOrbitalJ,
                                int primitiveI, int primitiveJ, int kHexahedron)
{
    // First, if a coefficient is zero, return zero to avoid a waist of time
    double coefficientI=pMolecularOrbitalI->coeff[primitiveI];
    if (coefficientI==0.)
    {
        return 0.;
    }

    double coefficientJ=pMolecularOrbitalJ->coeff[primitiveJ];
    if (coefficientJ==0.)
    {
        return 0.;
    }

    double exponentI=pMolecularOrbitalI->exp[primitiveI];
    double exponentJ=pMolecularOrbitalJ->exp[primitiveJ];
    double exponentIJ=exponentI+exponentJ;

    int typeI=pMolecularOrbitalI->type[primitiveI];
    int layerI=convertingType(typeI);
    int sublayerIz=layerI/100;
    int sublayerIy=layerI%100;
    int sublayerIx=sublayerIy%10;
    sublayerIy/=10;

    int typeJ=pMolecularOrbitalJ->type[primitiveJ];
    int layerJ=convertingType(typeJ);
    int sublayerJz=layerJ/100;
    int sublayerJy=layerJ%100;
    int sublayerJx=sublayerJy%10;
    sublayerJy/=10;

    int layerIJ=sublayerIx+sublayerIy+sublayerIz+sublayerJx+
                                                          sublayerJy+sublayerJz;

    // Warning: we stored Nuclei reference as in *.chem file so do not forget
    // to remove one from these integers when calling pChemicalSystem->pnucl
    int iNucleus=pMolecularOrbitalI->nucl[primitiveI];
    Nucleus* pCenterI=&pChemicalSystem->pnucl[iNucleus-1];

    iNucleus=pMolecularOrbitalJ->nucl[primitiveJ];
    Nucleus* pCenterJ=&pChemicalSystem->pnucl[iNucleus-1];

    // Warning: we stored Point reference as in *.mesh file so do not forget
    // to remove one from these integers when calling pMesh->pver
    iNucleus=pMesh->phex[kHexahedron].p1;
    Point *pPoint=&pMesh->pver[iNucleus-1];

    double dxI=(pPoint->x)-(pCenterI->x);
    double dyI=(pPoint->y)-(pCenterI->y);
    double dzI=(pPoint->z)-(pCenterI->z);

    double dxJ=(pPoint->x)-(pCenterJ->x);
    double dyJ=(pPoint->y)-(pCenterJ->y);
    double dzJ=(pPoint->z)-(pCenterJ->z);

    double dx=(pCenterI->x)-(pCenterJ->x);
    double dy=(pCenterI->y)-(pCenterJ->y);
    double dz=(pCenterI->z)-(pCenterJ->z);

    double distanceSquared=dx*dx+dy*dy+dz*dz;
    double exponent=exponentI*exponentJ/exponentIJ;
    double integral=.125*coefficientI*coefficientJ*
                  exp(-exponent*distanceSquared)/pow(exponentIJ,1.5+.5*layerIJ);

    exponentIJ=sqrt(exponentIJ);
    double valueMoinsX=(exponentI*dxI+exponentJ*dxJ)/exponentIJ;
    double valueMoinsY=(exponentI*dyI+exponentJ*dyJ)/exponentIJ;
    double valueMoinsZ=(exponentI*dzI+exponentJ*dzJ)/exponentIJ;

    dxI+=pParameters->delta_x;
    dyI+=pParameters->delta_y;
    dzI+=pParameters->delta_z;

    dxJ+=pParameters->delta_x;
    dyJ+=pParameters->delta_y;
    dzJ+=pParameters->delta_z;

    double valuePlusX=(exponentI*dxI+exponentJ*dxJ)/exponentIJ;
    double valuePlusY=(exponentI*dyI+exponentJ*dyJ)/exponentIJ;
    double valuePlusZ=(exponentI*dzI+exponentJ*dzJ)/exponentIJ;

    double cIx=exponentJ*dx/exponentIJ;
    double cIy=exponentJ*dy/exponentIJ;
    double cIz=exponentJ*dz/exponentIJ;

    double cJx=exponentI*dx/exponentIJ;
    double cJy=exponentI*dy/exponentIJ;
    double cJz=exponentI*dz/exponentIJ;

    // Evaluating the three-dimensional integral by separation of variables
    integral*=evaluateOneIntegralIJ(sublayerIx,sublayerJx,cIx,cJx,valueMoinsX,
                                                                    valuePlusX);
    integral*=evaluateOneIntegralIJ(sublayerIy,sublayerJy,cIy,cJy,valueMoinsY,
                                                                    valuePlusY);
    integral*=evaluateOneIntegralIJ(sublayerIz,sublayerJz,cIz,cJz,valueMoinsZ,
                                                                    valuePlusZ);
    return integral;
}

/* ************************************************************************** */
// The function computeOverlapMatrixOnGrid calculates the overlap matrix
// S(Omega)_ij=int_Omega Orb(i)*Orb(j), where the molecular orbitals' is stored
// in the structure pointed by pChemicalSystem. The domain of integration Omega
// corresponds to the hexahedra of pMesh that are NOT labelled
// labelToConsiderOrAvoid if the iterationInTheLoop is set to zero or if the
// pParameters->trick_matrix is set to zero, otherwise Omega corresponds to the
// hexahedra of pMesh labelled labelToConsiderOrAvoid. The coefficients of the
// overlap matrix are stored in the coef array variable of the OverlapMatrix
// structure located at pData->pmat[iterationInTheLoop]. It has the
// Parameters*, Mesh*, Data*, ChemicalSystem* variables (both defined in
// main.h), and the two integers labelToConsiderOrAvoid and iterationInTheLoop
// as input arguments. It returns one if the coefficients of the overlap matrix
// have been successfully computed, otherwise zero is returned in case of error
/* ************************************************************************** */
int computeOverlapMatrixOnGrid(Parameters* pParameters, Mesh* pMesh,
                               Data* pData, ChemicalSystem* pChemicalSystem,
                               int labelToConsiderOrAvoid,
                                                         int iterationInTheLoop)
{
    int i=0, j=0, k=0, lI=0, lJ=0, nHex=0, nMorb=0, nGauss=0;
    double integral=0., integralK=0.;
    MolecularOrbital *pMolecularOrbitalI=NULL, *pMolecularOrbitalJ=NULL;
    OverlapMatrix *pOverlapMatrix=NULL, *pOverlapMatrixOld=NULL;

    // Check input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                                                          pChemicalSystem==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: at least one of the ");
        fprintf(stderr,"input variables pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, pData=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"or pChemicalSystem=%p ",(void*)pChemicalSystem);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check input integers
    if (labelToConsiderOrAvoid!=-1 && abs(labelToConsiderOrAvoid)!=2 &&
                                                 abs(labelToConsiderOrAvoid)!=3)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the input ");
        fprintf(stderr,"labelToConsiderOrAvoid variable ");
        fprintf(stderr,"(=%d) can only be set to ",labelToConsiderOrAvoid);
        fprintf(stderr,"minus one, (+/-)two or (+/-)three.\n");
        return 0;
    }

    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the input ");
        fprintf(stderr,"iterationInTheLoop variable (=%d) ",iterationInTheLoop);
        fprintf(stderr,"should be a non-negative integer not (strictly) ");
        fprintf(stderr,"greater than the maximal number of allowed ");
        fprintf(stderr,"iterations (=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Check that the mesh is made of hexahedra
    if (pParameters->opt_mode>0  || pMesh->phex==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the function can only be ");
        fprintf(stderr,"used when the optimization mode ");
        fprintf(stderr,"(=%d) is not positive i.e. the ",pParameters->opt_mode);
        fprintf(stderr," structure pointed by pMesh is made of hexahedra ");
        fprintf(stderr,"(pMesh->phex=%p).\n",(void*)pMesh->phex);
        return 0;
    }

    fprintf(stdout,"\nEvaluating the overlap matrix ");
    fprintf(stdout,"S(%d) on mesh.\n",labelToConsiderOrAvoid);

    // Set the pointer for the overlap matrix
    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the pmat variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pmat);
        return 0;
    }
    pOverlapMatrix=&pData->pmat[iterationInTheLoop];

    // Check the coef pointer variable of pOverlapMatrix
    if (pOverlapMatrix->coef==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the coef variable of the ");
        fprintf(stderr,"%d-th OverlapMatrix structure ",iterationInTheLoop+1);
        fprintf(stderr,"is pointing to the %p ",(void*)pOverlapMatrix->coef);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check mesh and chemical parameters
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the total number of ");
        fprintf(stderr,"vertices (=%d) in the mesh should be a ",pMesh->nver);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the pver variable of the ");
        fprintf(stderr,"structure pointed by pMesh is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pMesh->pver);
        return 0;
    }

    nHex=pMesh->nhex;
    if (nHex<1)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the total number of ");
        fprintf(stderr,"hexahedra (=%d) in the mesh should be a ",nHex);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    nMorb=pChemicalSystem->nmorb;
    if (nMorb<1)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the total number of ");
        fprintf(stderr,"molecular orbitals (=%d) should be a positive ",nMorb);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    if (pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the pmorb variable of ");
        fprintf(stderr,"the structure pointed by pChemicalSystem is pointing ");
        fprintf(stderr,"to the %p address.\n",(void*)pChemicalSystem->pmorb);
        return 0;
    }

    nGauss=pChemicalSystem->ngauss;
    if (nGauss<1)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the total number of ");
        fprintf(stderr,"Gaussian-type primitives (=%d) should be a ",nGauss);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    if (pData->nmat!=nMorb)
    {
        PRINT_ERROR("In computeOverlapMatrixOnGrid: the size of the ");
        fprintf(stderr,"overlap matrix should be (%d)x(%d) ",nMorb,nMorb);
        fprintf(stderr,"instead of (%d)x(%d).\n",pData->nmat,pData->nmat);
        return 0;
    }

    if (pParameters->orb_rhf)
    {
        if (nMorb%2!=0)
        {
            PRINT_ERROR("In computeOverlapMatrixOnGrid: since we are dealing ");
            fprintf(stderr,"with a restricted Hartree-Fock system ");
            fprintf(stderr,"(pParameters->orb_rhf=%d) ",pParameters->orb_rhf);
            fprintf(stderr,"the total number of molecular orbitals ");
            fprintf(stderr," (=%d) should be an even number.\n",nMorb);
            return 0;
        }
    }

    // Initialize the coefficients of the overlap matrix to zero if necessary
    if (!pParameters->trick_matrix || !iterationInTheLoop)
    {
        for (i=0; i<nMorb; i++)
        {
            for (j=0; j<nMorb; j++)
            {
                pOverlapMatrix->coef[i*nMorb+j]=0.;
            }
        }
    }

    // Evaluation of the symmetric overlap matrix S
    for (i=0; i<nMorb; i++)
    {
        pMolecularOrbitalI=&pChemicalSystem->pmorb[i];

         // Avoid calculation for restricted Hartree-Fock spin repetitions
        if (pParameters->orb_rhf)
        {
            if (i>=nMorb/2)
            {
                continue;
            }
        }

        for (j=0; j<=i; j++)
        {
            pMolecularOrbitalJ=&pChemicalSystem->pmorb[j];

            // In the case of where pParameters->trick_matrix is active and that
            // we are not at the initial step in the optimization loop
            // iterationInTheLoop>0), integrals on hexahedra labelled -2 or -3
            // are added to the previous overlap matrix coefficients; otherwise,
            // the overlap matrix is computed from zero by avoiding hexahedra
            // labebelled -1 (whole computational box), 2 or 3
            if (labelToConsiderOrAvoid==-2 || labelToConsiderOrAvoid==-3)
            {
                if (!pParameters->trick_matrix || !iterationInTheLoop)
                {
                    PRINT_ERROR("In computeOverlapMatrixOnGrid: it is not ");
                    fprintf(stderr,"possible to compute the overlap matrix ");
                    fprintf(stderr,"S(%d) ",labelToConsiderOrAvoid);
                    if (!iterationInTheLoop)
                    {
                        fprintf(stderr,"at the %d-th ",iterationInTheLoop);
                        fprintf(stderr,"iteration.\n");
                    }
                    else
                    {
                        fprintf(stderr,"when the trick_matrix variable ");
                        fprintf(stderr,"(=%d) ",pParameters->trick_matrix);
                        fprintf(stderr,"is not active.\n");
                    }
                    return 0;
                }

                // Initialize the value to the initial one and only one time
                // Warning: here we assume in shapeDerivative function we
                // compute S(-2) followed by S(-3) (and not the contrary)
                if (labelToConsiderOrAvoid==-2)
                {
                    pOverlapMatrixOld=&pData->pmat[iterationInTheLoop-1];
                    pOverlapMatrix->coef[i*nMorb+j]=
                                             pOverlapMatrixOld->coef[i*nMorb+j];
                }
            }

            // Case where Orbitals i and j have different spin (and take into
            // account the case of restricted Hartree-Fock spin repetitions)
            if (pParameters->orb_rhf)
            {
                k=(pMolecularOrbitalI->spin==1 && pMolecularOrbitalJ->spin==1);
            }
            else
            {
                k=(pMolecularOrbitalI->spin==pMolecularOrbitalJ->spin);
            }
            if (!k)
            {
                continue;
            }

            integralK=0.;
#pragma omp parallel for reduction(+:integralK) private(integral,lI,lJ)
            for (k=0; k<nHex; k++)
            {
                integral=0.;

                // Skip or consider the labelToConsiderOrAvoid for hexahedra
                if (labelToConsiderOrAvoid<-1)
                {
                    if (pMesh->phex[k].label!=labelToConsiderOrAvoid)
                    {
                        continue;
                    }
                }
                else
                {
                    if (pMesh->phex[k].label==labelToConsiderOrAvoid)
                    {
                        continue;
                    }
                }

                // Compute the three-dimensional integral analytically
                for (lI=0; lI<nGauss; lI++)
                {
                    for (lJ=0; lJ<nGauss; lJ++)
                    {
                        integral+=evaluateTripleIntegralIJ(pParameters,pMesh,
                                                           pChemicalSystem,
                                                           pMolecularOrbitalI,
                                                           pMolecularOrbitalJ,
                                                                       lI,lJ,k);
                    }
                }
                integralK=integralK+integral;
            }

            // End of the parallelization for the loop for. Update the overlap
            // matrix coefficients depending on the labelToConsiderOrAvoid
            if (labelToConsiderOrAvoid==-2)
            {
                pOverlapMatrix->coef[i*nMorb+j]-=integralK;
            }
            else
            {
                pOverlapMatrix->coef[i*nMorb+j]+=integralK;
            }

            if (pParameters->orb_rhf)
            {
                fprintf(stdout,"%d %% done.\n",
                               (int)(100.*(i*.5*nMorb+j+1.)/(.25*nMorb*nMorb)));
            }
            else
            {
                fprintf(stdout,"%d %% done.\n",
                                      (int)(100.*(i*nMorb+j+1.)/(nMorb*nMorb)));
            }
        }
    }

    // Normally, we do not need to complete the matrix which is symmetric
    // since LAPACK_dsyev function only need a triangular part. Warning:
    // since LAPACK is written in Fortran (interfaced with c by the lapacke
    // library) the matrices are stored colomn_wise by default. Here, we
    // complete the matrix in the case it might be used for the saving of data
    for (i=0; i<nMorb; i++)
    {
        for (j=i+1; j<nMorb; j++)
        {
            pOverlapMatrix->coef[i*nMorb+j]=pOverlapMatrix->coef[j*nMorb+i];
        }
    }

    // Complete the matrix in the case of a restricted Hartree-Fock system
    if (pParameters->orb_rhf)
    {
        k=nMorb/2;
        for (i=0; i<k; i++)
        {
            for (j=0; j<k; j++)
            {
                pOverlapMatrix->coef[(i+k)*nMorb+(j+k)]=
                                                pOverlapMatrix->coef[i*nMorb+j];
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function diagonalizeOverlapMatrix diagonalize the (square symmetric)
// matrix whose coefficients are stored in pData->pmat[iterationInTheLoop]
// and if verbose is not set to zero, its prints the eigenvalues and
// eigenvectors in the standard output stream. It has the Parameters*, the Data*
// variable (both defined in main.h), and the int iterationInTheLoop as input
// arguments and it returns one on success, otherwise zero for an error
/* ************************************************************************** */
int diagonalizeOverlapMatrix(Parameters* pParameters, Data* pData,
                                                         int iterationInTheLoop)
{
    int i=0, j=0, nMorb=0;
    OverlapMatrix *pOverlapMatrix=NULL;

    // Since Lapack is written in Fortran (column-wise storage), better to use
    // LAPACK_COL_MAJOR instead of LAPACK_ROW_MAJOR to avoid transposition step
    int matrixLayout=LAPACK_COL_MAJOR;

    // If jobz='N', it only computes the eigenvalues and if jobz='V', it
    // computes also the (orthonormal) eigenvectors on exit. Check that each
    // line (i.e. column for LAPACK) is a eigenvector if LAPACK_COL_MAJOR is set
    char jobz='V';

    // if uplo='U' (respectively uplo='L'), it considers the upper (resp. lower)
    // part of the matrix as input for the symmetric matrix to diagonalize
    char uplo='U';

    // n is used to interpret the array with matrixLayout
    lapack_int n=0;

    // lda is the leading dimension of the array (for non-square matrices)
    lapack_int lda=0;

    // info is the function output (=0 for successfull exit, =-i<0 if i-th
    // input variable is illegal, =i>0 if an error occurs during the
    // diagonalization process)
    lapack_int info=0;

    // Check that the input pointers are not pointing to NULL
    if (pData==NULL || pParameters==NULL)
    {
        PRINT_ERROR("In diagonalizeOverlapMatrix: at least one of the input ");
        fprintf(stderr,"variable pParameters=%p ",(void*)pParameters);
        fprintf(stderr,"or pData=%p does not have a valid ",(void*)pData);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the iterationInTheLoop variable
    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In diagonalizeOverlapMatrix: the input ");
        fprintf(stderr,"iterationInTheLoop variable (=%d) ",iterationInTheLoop);
        fprintf(stderr,"should be a (non-negative) integer not (strictly) ");
        fprintf(stderr,"greater than the maximal number of iterations ");
        fprintf(stderr,"(=%d) allowed in the ",pParameters->iter_max);
        fprintf(stderr,"optimization loop.\n");
        return 0;
    }

    // Check the size of the overlap matrix is positive
    nMorb=pData->nmat;
    if (nMorb<1)
    {
        PRINT_ERROR("In diagonalizeOverlapMatrix: the size of the overlap ");
        fprintf(stderr,"matrix (=%d) should be a positive ",nMorb);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    if (pParameters->orb_rhf)
    {
        if (nMorb%2!=0)
        {
            PRINT_ERROR("In diagonalizeOverlapMatrix: since we are dealing ");
            fprintf(stderr,"with a restricted Hartree-Fock system ");
            fprintf(stderr,"(pParameters->orb_rhf=%d) ",pParameters->orb_rhf);
            fprintf(stderr,"the total number of molecular orbitals ");
            fprintf(stderr," (=%d) should be an even number.\n",nMorb);
            return 0;
        }
        n=nMorb/2;
        lda=nMorb/2;
    }
    else
    {
        n=nMorb;
        lda=nMorb;
    }

    // Set the pointer for the overlap matrix
    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In diagonalizeOverlapMatrix: the pmat variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pmat);
        return 0;
    }
    pOverlapMatrix=&pData->pmat[iterationInTheLoop];

    // Check the pointers variable of pOverlapMatrix
    if (pOverlapMatrix->coef==NULL || pOverlapMatrix->diag==NULL ||
                                                     pOverlapMatrix->vect==NULL)
    {
        PRINT_ERROR("In diagonalizeOverlapMatrix: at least one of the ");
        fprintf(stderr,"variables of the %d-th ",iterationInTheLoop+1);
        fprintf(stderr,"OverlapMatrix structure ");
        fprintf(stderr,"(coef=%p, ",(void*)pOverlapMatrix->coef);
        fprintf(stderr,"diag=%p, ",(void*)pOverlapMatrix->diag);
        fprintf(stderr,"vect=%p) does not point ",(void*)pOverlapMatrix->vect);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    if (pParameters->verbose)
    {
        if ((pParameters->opt_mode!=-2 && pParameters->opt_mode!=1) ||
                                                            !iterationInTheLoop)
        {
            fprintf(stdout,"\nEvaluating the eigenvalues and eigenvectors ");
            fprintf(stdout,"of the overlap matrix:\n");
            for (i=0; i<nMorb; i++)
            {
                for (j=0; j<nMorb; j++)
                {
                    fprintf(stdout,"%le ",pOverlapMatrix->coef[i*nMorb+j]);
                }
                fprintf(stdout,"\n");
            }
        }
    }

    // Initialize to zero the coefficients of the eigenvalues and eigenvectors
    for (i=0; i<nMorb; i++)
    {
        for (j=0; j<nMorb; j++)
        {
            pOverlapMatrix->vect[i*nMorb+j]=0.;
        }
        pOverlapMatrix->diag[i]=0.;
    }

    // Fill lower part of the eigenvectors (i.e. the upper part for column-wise
    // storage as in Fortran) with the coefficients of the overlap matrix
    for (i=0; i<n; i++)
    {
        for (j=0; j<=i; j++)
        {
             pOverlapMatrix->vect[i*n+j]=pOverlapMatrix->coef[i*nMorb+j];
        }
    }

    // Diagonalization using dsyev of lapacke library (lapack interface for C)
    info=LAPACKE_dsyev(matrixLayout,jobz,uplo,n,pOverlapMatrix->vect,lda,
                                                          pOverlapMatrix->diag);
    if (info)
    {
        PRINT_ERROR("In diagonalizeOverlapMatrix: dsyev function ");
        fprintf(stderr,"in Lapacke library failed and returned %d ",info);
        fprintf(stderr,"instead of zero.\n");
        return 0;
    }

    // Duplicate eigenvalues and eigenvectors for restricted Hartree-Fock system
    if (pParameters->orb_rhf)
    {
        for (i=0; i<n; i++)
        {
            for (j=0; j<n; j++)
            {
                pOverlapMatrix->vect[(i+n)*nMorb+(j+n)]=
                                                    pOverlapMatrix->vect[i*n+j];
            }
            pOverlapMatrix->diag[i+n]=pOverlapMatrix->diag[i];
        }

        for (i=0; i<n; i++)
        {
            for (j=0; j<nMorb; j++)
            {
                if (j<n)
                {
                    pOverlapMatrix->vect[i*nMorb+j]=
                                        pOverlapMatrix->vect[(i+n)*nMorb+(j+n)];
                }
                else
                {
                    pOverlapMatrix->vect[i*nMorb+j]=0.;
                }
            }
        }
    }

    if (pParameters->verbose)
    {
        if ((pParameters->opt_mode!=-2 && pParameters->opt_mode!=1) ||
                                                            !iterationInTheLoop)
        {
            // Print the eigenvalues and eigenvectors
            fprintf(stdout,"\nEigenvalues: \n");
            for (i=0; i<nMorb; i++)
            {
                fprintf(stdout,"%le \n",pOverlapMatrix->diag[i]);
            }
            fprintf(stdout,"\nEigenvectors (one per line): \n");
            for (i=0; i<nMorb; i++)
            {
                for (j=0; j<nMorb; j++)
                {
                    fprintf(stdout,"%le ",pOverlapMatrix->vect[i*nMorb+j]);
                }
                fprintf(stdout,"\n");
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function computeProbability calculates the probability to find exactly
// a certain number nu of electrons in the domain Omega and in the complement
// the total number of electrons minus nu by using the eigenvalues of the
// associated overlap matrix (Cancès et al. formula). All these probabilities
// are also saved in the structure pointed by pData->pprob[iterationInTheLoop]
// and the total population of electrons inside the domain is also computed and
// saved in pData->pop[iterationInTheLoop], while the probability to find
// the pParameters->nu_electrons in the domain is saved in
// pData->pnu[iterationInTheLoop]. It has the Parameters*, the Data* variables
// (both defined in main.h), and the integer iterationInTheLoop as input
// arguments. It returns one if the probabilities have been successfully
// computed and saved in pData, otherwise zero is returned in case of error
/* ************************************************************************** */
int computeProbability(Parameters* pParameters, Data* pData,
                                                         int iterationInTheLoop)
{
    int i=0, j=0, nProb=0, nu=0, nMorb=0;
    double value=0., alpha=0., beta=0.;
    Probabilities* pProbabilities=NULL;
    OverlapMatrix* pOverlapMatrix=NULL;

    // Check that the input variables are not pointing to NULL
    if (pData==NULL || pParameters==NULL)
    {
        PRINT_ERROR("In computeProbability: at least one of the input ");
        fprintf(stderr,"variable pParameters=%p ",(void*)pParameters);
        fprintf(stderr,"or pData=%p does not have a valid ",(void*)pData);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the iterationInTheLoop variable
    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In computeProbability: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"(non-negative) integer not (strictly) greater than ");
        fprintf(stderr,"the maximal number of iterations ");
        fprintf(stderr,"(=%d) allowed in the ",pParameters->iter_max);
        fprintf(stderr,"optimization loop.\n");
        return 0;
    }

    // Check pointers for the saving of the probability and total population
    if (pData->pnu==NULL || pData->pop==NULL)
    {
        PRINT_ERROR("In computeProbability: at least one of the ");
        fprintf(stderr,"pnu=%p or pop=%p ",(void*)pData->pnu,(void*)pData->pop);
        fprintf(stderr,"variables of the structure pointed by pData does not ");
        fprintf(stderr,"have a valid adress.\n");
        return 0;
    }

    // Check the total number of electrons
    nMorb=pData->nmat;
    if (nMorb<1)
    {
        PRINT_ERROR("In computeProbability: the total number of electrons ");
        fprintf(stderr,"(=%d) in the chemical system should be a ",nMorb);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    // Check the number nu of electrons we look for
    nu=pParameters->nu_electrons;
    if (nu<1 || nu>nMorb)
    {
        PRINT_ERROR("In computeProbability: the number ");
        fprintf(stderr,"(=%d) of electrons we are looking for should be ",nu);
        fprintf(stderr,"a (positive) integer not (strictly) greater than ");
        fprintf(stderr,"the total number of electrons (=%d) in the ",nMorb);
        fprintf(stderr,"chemical system.\n");
        return 0;
    }

    // Check the size of the Probabilities structures
    nProb=pData->nprob;
    if (nProb!=1+nMorb)
    {
        PRINT_ERROR("In computeProbability: the size of the array storing ");
        fprintf(stderr,"the different probabilities should be set to ");
        fprintf(stderr,"%d instead of %d.\n",nMorb+1,nProb);
        return 0;
    }

    // Set the pointer for the Probabilities structure
    if (pData->pprob==NULL)
    {
        PRINT_ERROR("In computeProbability: the pprob variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pprob);
        return 0;
    }
    pProbabilities=&pData->pprob[iterationInTheLoop];

    // Check the pointers variable of pProbabilities
    if (pProbabilities->pk==NULL || pProbabilities->pkl==NULL)
    {
        PRINT_ERROR("In computeProbability: at least one of the pointer ");
        fprintf(stderr,"variables of the %d-th ",iterationInTheLoop+1);
        fprintf(stderr,"Probabilities structure ");
        fprintf(stderr,"(pk=%p, ",(void*)pProbabilities->pk);
        fprintf(stderr,"pkl=%p) does not point ",(void*)pProbabilities->pkl);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Set the pointer for the OverlapMatrix structure
    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In computeProbability: the pmat variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pmat);
        return 0;
    }
    pOverlapMatrix=&pData->pmat[iterationInTheLoop];

    // Check the diag pointer variable of pOverlapMatrix
    if (pOverlapMatrix->diag==NULL)
    {
        PRINT_ERROR("In computeProbability: the diag variable of the ");
        fprintf(stderr,"%d-th OverlapMatrix structure ",iterationInTheLoop+1);
        fprintf(stderr,"is pointing to the %p ",(void*)pOverlapMatrix->diag);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Note that for the moment we are not able to compute a spin probabilities
    if (pParameters->verbose)
    {
        if ((pParameters->opt_mode!=-2 && pParameters->opt_mode!=1) ||
                                                            !iterationInTheLoop)
        {
            fprintf(stdout,"\nComputing the probability to find exactly ");
            fprintf(stdout,"%d electron",nu);
            if (nu>1)
            {
                fprintf(stdout,"s");
            }
            fprintf(stdout," inside the domain Omega, and ");
            fprintf(stdout,"%d electron",nMorb-nu);
            if (nMorb-nu>1)
            {
                fprintf(stdout,"s");
            }
            fprintf(stdout," outside.\n");
        }
    }

    // Computing the recursive formula (5)-(7) of Cancès, Keriven, Lodier, Savin
    // (2004, p.374) to compute the probability in O(nMorb^2) operations
    pProbabilities->pkl[0]=1.;
    for (i=1; i<nProb; i++)
    {
        beta=pOverlapMatrix->diag[i-1];
        alpha=1.-beta;
        value=alpha*pProbabilities->pkl[(i-1)*nProb];
        pProbabilities->pkl[i*nProb]=value;
        for (j=1; j<i; j++)
        {
            value=beta*pProbabilities->pkl[(i-1)*nProb+(j-1)]+
                                       alpha*pProbabilities->pkl[(i-1)*nProb+j];
            pProbabilities->pkl[i*nProb+j]=value;
        }
        value=beta*pProbabilities->pkl[(i-1)*nProb+(i-1)];
        pProbabilities->pkl[i*nProb+i]=value;
    }

    // Save the probabilities and the one asked by the user: Prob_nu(Omega)
    for (i=0; i<nProb; i++)
    {
        pProbabilities->pk[i]=pProbabilities->pkl[nMorb*nProb+i];
    }
    pData->pnu[iterationInTheLoop]=pProbabilities->pk[nu];

    // Print the probabilities and save
    if ((pParameters->opt_mode!=-2 && pParameters->opt_mode!=1) ||
                                                            !iterationInTheLoop)
    {
        if (pParameters->verbose)    
        {
            for (i=0; i<nProb; i++)
            {
                value=pProbabilities->pk[i];
                if (value<0.|| value>1.)
                {
                    fprintf(stdout,"\nWarning in computeProbability ");
                    fprintf(stdout,"function: probability(%d electrons in ",i);
                    fprintf(stdout,"Omega) = %.8lf which is not ",value);
                    fprintf(stdout,"comprised between zero and one.\n");
                }
                else
                {
                    if (i==nu)
                    {
                        fprintf(stdout,"\n");
                    }
                    fprintf(stdout,"Probability(%d electrons in Omega) = ",i);
                    fprintf(stdout,"%.8lf \n",value);
                    if (i==nu)
                    {
                        fprintf(stdout,"\n");
                    }
                }
            }
        }
        else
        {
            fprintf(stdout,"\nProbability(nu=%d electrons in Omega) = ",nu);
            fprintf(stdout,"%.8lf\n",pData->pnu[iterationInTheLoop]);
        }
    }
    else if (pParameters->opt_mode==1)
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\nProbability(nu=%d electrons in Omega) = ",nu);
            fprintf(stdout,"%.8lf\n",pData->pnu[iterationInTheLoop]);
        }
        else
        {
            fprintf(stdout,"100 %% done.\n");
        }
    }

    // Save and print total population of electrons in the domain Omega
    // using the formula Population = sum_nu nu*Prob_nu(Omega)
    value=0.;
    for (i=1; i<nProb; i++)
    {
        value+=i*pProbabilities->pk[i];
    }
    pData->pop[iterationInTheLoop]=value;
    if ((pParameters->opt_mode!=-2 && pParameters->opt_mode!=1) ||
                                                            !iterationInTheLoop)
    {
        fprintf(stdout,"\nTotal population in Omega is equal to ");
        fprintf(stdout,"%.8lf\n",value);
    }
    else if (pParameters->opt_mode==1 && pParameters->verbose)
    {
        fprintf(stdout,"\nTotal population in Omega is equal to ");
        fprintf(stdout,"%.8lf\n",value);
    }

    return 1;
}

/* ************************************************************************** */
// The function AddOrRemoveHexahedronToMesh modify the label associated with one
// of the two hexahedra of that are having the j-th quadrilateral in common.
// These two hexahedra are quickly identified thanks to the Adjacency structures
// built and stored in pMesh. If the addOrRemove variable is set to 1, it adds
// the hexahedron that do not belong to the domain by changing the label from 2
// to 3 (we recall that we use the same convention than the mmg3d software:
// internal elements are labelled three otherwise two). If set to -1, it removes
// the hexahedron that belongs to the domain by changing its label from 3 to 2.
// Finally, if set to -3, the external hexahedron is added but it still remains
// to check if the fact of adding it will increase the probability. For this
// purpose, instead of changing its label from 2 to 3, we change it from 2 to -3
// the sign allowing us to locate fastly this element later in the algorithm.
// Similarly, if addOrRemove is set to -2, the internal hexahedron is pendingly
// removed i.e. its label is changed from 3 to -2. It has the Mesh* variable
// (defined in main.h) and the two integers (j, addOrRemove) as input arguments
// It returns the position of the hexahedron in the array pointed by pMesh->phex
// on success, otherwise zero is returned if addOrRemove is not set to (+/-)one
// minus two/three and a warning is displayed in the standard output stream
/* ************************************************************************** */
int AddOrRemoveHexahedronToMesh(Mesh* pMesh, int j, int addOrRemove)
{
    int iHexahedron=0;

    switch (addOrRemove)
    {
        // Warning: the hexahedron reference is saved as in the *.mesh file so
        // do not forget to remove one from these integer when calling them
        case 1:
            iHexahedron=pMesh->padj[j].hexout;
            pMesh->phex[iHexahedron-1].label=3;
            break;

        case -1:
            iHexahedron=pMesh->padj[j].hexin;
            pMesh->phex[iHexahedron-1].label=2;
            break;

        case -3:
            iHexahedron=pMesh->padj[j].hexout;
            pMesh->phex[iHexahedron-1].label=-3;
            break;

        case -2:
            iHexahedron=pMesh->padj[j].hexin;
            pMesh->phex[iHexahedron-1].label=-2;
            break;

        default:
            fprintf(stdout,"\nWarning in AddOrRemoveHexahedronToMesh  ");
            fprintf(stdout,"function:the input addOrRemove variable ");
            fprintf(stdout,"(=%d) can only be set to (+/-)one, ",addOrRemove);
            fprintf(stdout,"minus two or minus three.\n");
            iHexahedron=0;
            break;
    }

    return iHexahedron;
}

/* ************************************************************************** */
// The function evaluateShapeGradientAtThePoint calculates the shape gradient
// of the probability (to find exactly nu electrons) at the point (px,py,pz)
// thanks to the overlap matrix of pChemicalSystem, and more precisely thanks to
// the corresponding eigenvalues and eigenvectors stored in the structure
// pointed by pOverlapMatrix. The calculations are performed recursively, using
// the pkl (nProb)x(nProb)-array variable of the structure pointed by
// pProbabilities. It has the ChemicalSystem*, OverlapMatrix*, Probabilities*
// (both defined in main.h), the three point coordinates (px,py,pz), and the
// two integers (nProb,nu) as input arguments. It returns the computed value of
// the shape gradient.
/* ************************************************************************** */
double evaluateShapeGradientAtThePoint(double px, double py, double pz,
                                       int nProb, Probabilities* pProbabilities,
                                       int nu, OverlapMatrix* pOverlapMatrix,
                                                ChemicalSystem* pChemicalSystem)
{
    int i=0, j=0, l=0, p=0, q=0, nMorb=0;
    double function=0., value=0., shapeGradientAtThePoint=0., alpha=0., beta=0.;
    MolecularOrbital *pMolecularOrbitalI=NULL, *pMolecularOrbitalJ=NULL;

    nMorb=pChemicalSystem->nmorb;
    for (i=0; i<nMorb; i++)
    {
        pMolecularOrbitalI=&pChemicalSystem->pmorb[i];

        for (j=0; j<i; j++)
        {
            pMolecularOrbitalJ=&pChemicalSystem->pmorb[j];

            // Skip if the spin of the two orbitals are different
            if (pMolecularOrbitalI->spin!=pMolecularOrbitalJ->spin)
            {
                continue;
            }

            // Evaluate the product Orb(i)*Orb(j) at the point. Here we multiply
            // result by 2 (case j<i but also i<j by symmetry of the evaluation)
            function=2.*evaluateOrbitalsIJ(px,py,pz,pChemicalSystem,
                                         pMolecularOrbitalI,pMolecularOrbitalJ);

            // Computing the recursive formula of the article of Cancès,
            // Keriven, Lodier and Savin (2004, above (15) p.376) allowing to
            // evaluate the shape gradient in O(nMorb^2) operations
            value=0.;
            for (l=0; l<nMorb; l++)
            {
                pProbabilities->pkl[0]=1.;
                for (p=1; p<nProb; p++)
                {
                    if (l==p-1)
                    {
                        beta=1.;
                        alpha=-1.;
                    }
                    else
                    {
                        beta=pOverlapMatrix->diag[p-1];
                        alpha=1.-beta;
                    }

                    pProbabilities->pkl[p*nProb]=
                                         alpha*pProbabilities->pkl[(p-1)*nProb];

                    for (q=1; q<p; q++)
                    {
                       pProbabilities->pkl[p*nProb+q]=
                                     beta*pProbabilities->pkl[(p-1)*nProb+(q-1)]
                                      +alpha*pProbabilities->pkl[(p-1)*nProb+q];
                    }

                    pProbabilities->pkl[p*nProb+p]=
                                    beta*pProbabilities->pkl[(p-1)*nProb+(p-1)];
                }

                // For those who want to check: C language stores matrix
                // row-wise whereas Fortran uses coloumn-wise storage. Since we
                // have used the dsyev function of the lapacke library
                // (c-interface for LAPACK) for obtaining the eigenvectors.
                // Here is the reasoning: we have given the lower part of the
                // symmetric overlap matrix to diagonalize, and the resulting
                // eigenvectors computed by dsyev are the column of the
                // associated orthogonal matrix, but they were saved line-wise
                // because dsyev thought the storage would be column-wise which
                // is not the case in C. To sum up, the i-th coordinate of the
                // l-th eigenpOverlapMatrix->vect[l*nProb+i]
                value+=pProbabilities->pkl[nMorb*nProb+nu]
                                                *pOverlapMatrix->vect[l*nMorb+i]
                                               *pOverlapMatrix->vect[l*nMorb+j];
            }

            shapeGradientAtThePoint+=value*function;
        }

        // We now treat the case i=j
        function=evaluateOrbitalsII(px,py,pz,pChemicalSystem,
                                                            pMolecularOrbitalI);

        // Computing the recursive formula of the article of Cancès, Keriven,
        // Lodier and Savin (2004, above (15) p.376) allowing to evaluate the
        // shape gradient in O(nMorb^2) operations
        value=0.;
        for (l=0; l<nMorb; l++)
        {
            pProbabilities->pkl[0]=1.;
            for (p=1; p<nProb; p++)
            {
                if (l==p-1)
                {
                    beta=1.;
                    alpha=-1.;
                }
                else
                {
                    beta=pOverlapMatrix->diag[p-1];
                    alpha=1.-beta;
                }

                pProbabilities->pkl[p*nProb]=
                                         alpha*pProbabilities->pkl[(p-1)*nProb];

                for (q=1; q<p; q++)
                {
                    pProbabilities->pkl[p*nProb+q]=
                                     beta*pProbabilities->pkl[(p-1)*nProb+(q-1)]
                                      +alpha*pProbabilities->pkl[(p-1)*nProb+q];
                }

                 pProbabilities->pkl[p*nProb+p]=
                                    beta*pProbabilities->pkl[(p-1)*nProb+(p-1)];
            }

            // For those who want to check: C language stores matrix row-wise
            // whereas Fortran uses coloumn-wise storage. Since we have used the
            // dsyev function of the lapacke library (c-interface for LAPACK)
            // for obtaining the eigenvectors. Here is the reasoning: we have
            // given the lower part of the symmetric overlap matrix to
            // diagonalize, and the resulting eigenvectors computed by dsyev
            // are the column of the associated orthogonal matrix, but they were
            // saved line-wise because dsyev thought the storage would be
            // column-wise which is not the case in C. To sum up, the i-th
            // coordinate of the l-th eigenvector associated with the l-th
            // eigenvalue is stored in pOverlapMatrix->vect[l*nProb+i]
            value+=pProbabilities->pkl[nMorb*nProb+nu]
                                                *pOverlapMatrix->vect[l*nMorb+i]
                                               *pOverlapMatrix->vect[l*nMorb+j];
        }

        shapeGradientAtThePoint+=value*function;
    }

    return shapeGradientAtThePoint;
}

/* ************************************************************************** */
// The function computeShapeGradient calculates the shape gradient of the
// probability at the vertices of pMesh if pParameters->opt_mode is positive,
// otherwise at the center of the quadrilateral representing the boundary of the
// internal domain. In the latter case, it also directly modifies the mesh
// depending on the sign of the shape gradient, which is computed thanks to the
// eigenvalues and eigenvectors of the overlap matrix associated with
// pChemicalSystem and stored in pData->pmat[iterationInTheLoop]. It has the
// Parameters*, the Mesh*, the Data*, the ChemicalSystem* variables (both
// defined in main.h) and the integer iterationInTheLoop as input argument.
// It returns one if the shape gradient has been successfully computed (and the
// domain succesfully modified in the case of hexahedral meshes) otherwise zero
// is returned if an error is encountered
/* ************************************************************************** */
int computeShapeGradient(Parameters* pParameters, Mesh* pMesh, Data* pData,
                         ChemicalSystem* pChemicalSystem,
                                                         int iterationInTheLoop)
{
    int i1=0, i3=0, j1=0, j3=0, k=0, l=0, p=0, p1=0, p3=0, nProb=0, nMorb=0;
    int nu=0, nVer=0, nTri=0, nAdj=0;
    double value=0., px=0., py=0., pz=0.;
    Probabilities* pProbabilities=NULL;
    OverlapMatrix* pOverlapMatrix=NULL;
    Point *pVertex=NULL;

    // Check input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                                                          pChemicalSystem==NULL)
    {
        PRINT_ERROR("In computeShapeGradient: at least one of the input ");
        fprintf(stderr,"variables pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, pData=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"or pChemicalSystem=%p ",(void*)pChemicalSystem);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check the input iterationInTheLoop variable
    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("computeShapeGradient: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"non-negative integer not (strictly) greater than the ");
        fprintf(stderr,"maximal number of allowed iterations ");
        fprintf(stderr,"(=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Check the number of mesh vertices
    nVer=pMesh->nver;
    if (nVer<1)
    {
        PRINT_ERROR("In computeShapeGradient: the total number of vertices ");
        fprintf(stderr,"(=%d) in the mesh should be a positive ",nVer);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    // Check that the array storing the mesh vertices is not pointing to NULL
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In computeShapeGradient: the ");
        fprintf(stderr,"pver=%p variable of the structure ",(void*)pMesh->pver);
        fprintf(stderr,"pointed by pMesh does not have a valid address.\n");
        return 0;
    }

    if (pParameters->opt_mode<=0)
    {
        // Check the number of quadrilaterals for the internal domain
        nAdj=pMesh->nadj;
        if (nAdj<1 || pMesh->nqua<1)
        {
            PRINT_ERROR("In computeShapeGradient: the number of ");
            fprintf(stderr,"quadrilaterals (=%d) delimiting the ",nAdj);
            fprintf(stderr,"interior of the domain should be a positive ");
            fprintf(stderr,"integer as well as the total number of ");
            fprintf(stderr,"quadrilaterals (=%d) in the mesh.\n",pMesh->nqua);
            return 0;
        }

        // Check that the adjacency structure has been set up
        if (pMesh->padj==NULL || pMesh->pqua==NULL)
        {
            PRINT_ERROR("In computeShapeGradient: the Adjacency structures ");
            fprintf(stderr,"have not been set up in the hexahedral mesh ");
            fprintf(stderr,"(pMesh->padj=%p) or no ",(void*)pMesh->padj);
            fprintf(stderr,"quadrilaterals has been found ");
            fprintf(stderr,"(pMesh->pqua=%p).\n",(void*)pMesh->pqua);
            return 0;
        }
    }
    else
    {
        // Check the number of triangles
        nTri=pMesh->ntri;
        if (nTri<1)
        {
            PRINT_ERROR("In computeShapeGradient: the total number of ");
            fprintf(stderr,"triangles (=%d) in the mesh should be a ",nTri);
            fprintf(stderr,"positive integer.\n");
            return 0;
        }

        // Check that the pMesh->ptri variable is not pointing to NULL
        if (pMesh->ptri==NULL)
        {
            PRINT_ERROR("In computeShapeGradient: the boundary structures ");
            fprintf(stderr,"have not been set up in the tetrahedral mesh: ");
            fprintf(stderr,"pMesh->ptri=%p.\n",(void*)pMesh->ptri);
            return 0;
        }
    }

    // Check the total number of electrons and its associated pointer
    nMorb=pChemicalSystem->nmorb;
    if (nMorb<1)
    {
        PRINT_ERROR("In computeShapeGradient: the total number of electrons ");
        fprintf(stderr,"(=%d) in the chemical system should be a ",nMorb);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    if (pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In computeShapeGradient: the pmorb variable of the ");
        fprintf(stderr,"structure pointed by pChemicalSystem is pointing to ");
        fprintf(stderr,"the %p address.\n",(void*)pChemicalSystem->pmorb);
        return 0;
    }

    // Check the number nu of electrons we look for
    nu=pParameters->nu_electrons;
    if (nu<1 || nu>nMorb)
    {
        PRINT_ERROR("In computeShapeGradient: the number ");
        fprintf(stderr,"(=%d) of electrons we are looking for should be ",nu);
        fprintf(stderr,"a (positive) integer not (strictly) greater than ");
        fprintf(stderr,"the total number of electrons (=%d) in the ",nMorb);
        fprintf(stderr,"chemical system.\n");
        return 0;
    }

    // Check the size of the Probabilities structures
    nProb=pData->nprob;
    if (nProb!=1+nMorb)
    {
        PRINT_ERROR("In computeShapeGradient: the size of the array storing ");
        fprintf(stderr,"the different probabilities should be set to ");
        fprintf(stderr,"%d instead of %d.\n",nMorb+1,nProb);
        return 0;
    }

    // Set the pointer for the Probabilities structure
    if (pData->pprob==NULL)
    {
        PRINT_ERROR("In computeShapeGradient: the pprob variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pprob);
        return 0;
    }
    pProbabilities=&pData->pprob[iterationInTheLoop];

    // Check the pkl variable of pProbabilities
    if (pProbabilities->pkl==NULL)
    {
        PRINT_ERROR("In computeShapeGradient: the pkl variable of the ");
        fprintf(stderr,"%d-th Probabilities structure ",iterationInTheLoop+1);
        fprintf(stderr,"is pointing to the %p ",(void*)pProbabilities->pkl);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the size of the OverlapMatrix structures
    if (pData->nmat!=nMorb)
    {
        PRINT_ERROR("In computeShapeGradient: the common size defining the ");
        fprintf(stderr,"arrays of the different OverlapMatrix structures ");
        fprintf(stderr,"should be set to %d instead of ",nMorb);
        fprintf(stderr,"%d.\n",pData->nmat);
        return 0;
    }

    // Set the pointer for the overlap matrix
    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In computeShapeGradient: the pmat variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pmat);
        return 0;
    }
    pOverlapMatrix=&pData->pmat[iterationInTheLoop];

    // Check the diag and vect pointer variables of pOverlapMatrix
    if (pOverlapMatrix->diag==NULL || pOverlapMatrix->vect==NULL)
    {
        PRINT_ERROR("In computeShapeGradient: at least one of the diag or ");
        fprintf(stderr,"vect variables of the %d-th ",iterationInTheLoop+1);
        fprintf(stderr,"OverlapMatrix structure ");
        fprintf(stderr,"(diag=%p, ",(void*)pOverlapMatrix->diag);
        fprintf(stderr,"vect=%p) does not point ",(void*)pOverlapMatrix->vect);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Distinguish the level-set mode for tetrahedral and hexahedral meshes
    if (pParameters->opt_mode!=-2)
    {
        fprintf(stdout,"\nComputing the shape gradient of the probability.\n");
    }

    if (pParameters->opt_mode>0)
    {
        // Check that boundary points of the internal domain have positive label
        for (k=0; k<nTri; k++)
        {
            // Skip the triangles that are not part of the internal boundary
            if (pMesh->ptri[k].label!=10)
            {
                continue;
            }

            p=pMesh->ptri[k].p1;
            if (pMesh->pver[p-1].label<=0)
            {
                pMesh->pver[p-1].label=10;
            }

            p=pMesh->ptri[k].p2;
            if (pMesh->pver[p-1].label<=0)
            {
                pMesh->pver[p-1].label=10;
            }

            p=pMesh->ptri[k].p3;
            if (pMesh->pver[p-1].label<=0)
            {
                pMesh->pver[p-1].label=10;
            }
        }

        if (pParameters->opt_mode==2 || pParameters->opt_mode==3)
        {
            for (k=0; k<nTri; k++)
            {
                // Skip the triangles that are not part of the internal boundary
                if (pMesh->ptri[k].label!=10)
                {
                    continue;
                }

                // Storing the shape gradient at the vertex if not already done
                p=pMesh->ptri[k].p1;
                pVertex=&pMesh->pver[p-1];
                if (pVertex->label>0)
                {
                    px=pVertex->x;
                    py=pVertex->y;
                    pz=pVertex->z;
                    pVertex->label*=-1;
                    pVertex->value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                }

                p=pMesh->ptri[k].p2;
                pVertex=&pMesh->pver[p-1];
                if (pVertex->label>0)
                {
                    px=pVertex->x;
                    py=pVertex->y;
                    pz=pVertex->z;
                    pVertex->label*=-1;
                    pVertex->value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                }

                p=pMesh->ptri[k].p3;
                pVertex=&pMesh->pver[p-1];
                if (pVertex->label>0)
                {
                    px=pVertex->x;
                    py=pVertex->y;
                    pz=pVertex->z;
                    pVertex->label*=-1;
                    pVertex->value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                }
            }

            // Reset the label to their positive value
            for (k=0; k<nTri; k++)
            {
                // Skip the triangles that are not part of the internal boundary
                if (pMesh->ptri[k].label!=10)
                {
                    continue;
                }

                p=pMesh->ptri[k].p1;
                if (pMesh->pver[p-1].label<0)
                {
                    pMesh->pver[p-1].label*=-1;
                }

                p=pMesh->ptri[k].p2;
                if (pMesh->pver[p-1].label<0)
                {
                    pMesh->pver[p-1].label*=-1;
                }

                p=pMesh->ptri[k].p3;
                if (pMesh->pver[p-1].label<0)
                {
                    pMesh->pver[p-1].label*=-1;
                }
            }
        }
        else
        {
            for (k=0; k<nVer; k++)
            {
                // Save the three vertex coordinates
                pVertex=&pMesh->pver[k];
                px=pVertex->x;
                py=pVertex->y;
                pz=pVertex->z;

                // Storing the shape gradient at the vertices
                pVertex->value=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                           pProbabilities,nu,
                                                           pOverlapMatrix,
                                                               pChemicalSystem);

                // If pParameters->opt_mode==4 (Omega_new={dP_nu(Omega_old)>0})
                // we invert temporarily the sign to get a level-set function.
                if (pParameters->opt_mode==4)
                {
                    pVertex->value*=-1.;
                }
            }
        }
    }
    else
    {
        if (pParameters->opt_mode!=-2)
        {
            for (k=0; k<nAdj; k++)
            {
                // Load the center point coordinates in the k-th quadrilateral
                l=pMesh->padj[k].quad;

                p1=pMesh->pqua[l-1].p1;
                px=pMesh->pver[p1-1].x;
                py=pMesh->pver[p1-1].y;
                pz=pMesh->pver[p1-1].z;

                p=pMesh->pqua[l-1].p2;
                px+=pMesh->pver[p-1].x;
                py+=pMesh->pver[p-1].y;
                pz+=pMesh->pver[p-1].z;

                p3=pMesh->pqua[l-1].p3;
                px+=pMesh->pver[p3-1].x;
                py+=pMesh->pver[p3-1].y;
                pz+=pMesh->pver[p3-1].z;

                p=pMesh->pqua[l-1].p4;
                px+=pMesh->pver[p-1].x;
                py+=pMesh->pver[p-1].y;
                pz+=pMesh->pver[p-1].z;

                px*=.25;
                py*=.25;
                pz*=.25;

                // Computing the shape gradient of the probability at this point
                value=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                      pProbabilities,nu,
                                                      pOverlapMatrix,
                                                               pChemicalSystem);

                l=(p1-1)/pParameters->n_z;
                i1=l/pParameters->n_y;
                j1=l%pParameters->n_y;

                l=(p3-1)/pParameters->n_z;
                i3=l/pParameters->n_y;
                j3=l%pParameters->n_y;

                // Change labels of the hexahedra sharing the k-th quadrilateral
                if (pParameters->trick_matrix)
                {
                    if (value>0.)
                    {
                        if (i1==i3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexout-1].p1)
                            {
                                px+=pParameters->delta_x;
                            }
                            else
                            {
                                px-=pParameters->delta_x;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value>=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-3))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else if (j1==j3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexout-1].p1)
                            {
                                py+=pParameters->delta_y;
                            }
                            else
                            {
                                py-=pParameters->delta_y;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value>=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-3))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexout-1].p1)
                            {
                                pz+=pParameters->delta_z;
                            }
                            else
                            {
                                pz-=pParameters->delta_z;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value>=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-3))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                    }
                    else if (value<0.)
                    {
                        if (i1==i3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexin-1].p1)
                            {
                                px+=pParameters->delta_x;
                            }
                            else
                            {
                                px-=pParameters->delta_x;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value<=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-2))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else if (j1==j3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexin-1].p1)
                            {
                                py+=pParameters->delta_y;
                            }
                            else
                            {
                                py-=pParameters->delta_y;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value<=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-2))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexin-1].p1)
                            {
                                pz+=pParameters->delta_z;
                            }
                            else
                            {
                                pz-=pParameters->delta_z;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value<=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-2))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (value>0.)
                    {
                        if (i1==i3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexout-1].p1)
                            {
                                px+=pParameters->delta_x;
                            }
                            else
                            {
                                px-=pParameters->delta_x;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value>=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,1))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else if (j1==j3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexout-1].p1)
                            {
                                py+=pParameters->delta_y;
                            }
                            else
                            {
                                py-=pParameters->delta_y;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value>=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,1))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexout-1].p1)
                            {
                                pz+=pParameters->delta_z;
                            }
                            else
                            {
                                pz-=pParameters->delta_z;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value>=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,1))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                    }
                    else if (value<0.)
                    {
                        if (i1==i3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexin-1].p1)
                            {
                                px+=pParameters->delta_x;
                            }
                            else
                            {
                                px-=pParameters->delta_x;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value<=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-1))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else if (j1==j3)
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexin-1].p1)
                            {
                                py+=pParameters->delta_y;
                            }
                            else
                            {
                                py-=pParameters->delta_y;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value<=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-1))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if (p1==pMesh->phex[pMesh->padj[k].hexin-1].p1)
                            {
                                pz+=pParameters->delta_z;
                            }
                            else
                            {
                                pz-=pParameters->delta_z;
                            }
                            value=evaluateShapeGradientAtThePoint(px,py,pz,
                                                               nProb,
                                                               pProbabilities,
                                                               nu,
                                                               pOverlapMatrix,
                                                               pChemicalSystem);
                            if (value<=0.)
                            {
                                if (!AddOrRemoveHexahedronToMesh(pMesh,k,-1))
                                {
                                    PRINT_ERROR("In computeShapeGradient: Add");
                                    fprintf(stderr,"OrRemoveHexahedronToMesh ");
                                    fprintf(stderr,"function returned zero, ");
                                    fprintf(stderr,"which is not the ");
                                    fprintf(stderr,"expected value here.\n");
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function computeAreaTriangle evaluates the area of a triangle defined by
// three points. It has the three Point* (defined in main.h) variables as input
// arguments, and it returns the area of the triangle (123) in terms of a
// vector product: 0.5*norm(12^13)
/* ************************************************************************** */
double computeAreaTriangle(Point* pp1, Point* pp2, Point* pp3)
{
    double a1=pp2->x;
    double a2=pp2->y;
    double a3=pp2->z;

    double b1=pp3->x;
    double b2=pp3->y;
    double b3=pp3->z;

    double dx=pp1->x;
    double dy=pp1->y;
    double dz=pp1->z;

    a1-=dx;
    a2-=dy;
    a3-=dz;

    b1-=dx;
    b2-=dy;
    b3-=dz;

    dx=a2*b3-a3*b2;
    dy=b1*a3-b3*a1;
    dz=a1*b2-a2*b1;

    return .5*sqrt(dx*dx+dy*dy+dz*dz);
}

/* ************************************************************************** */
// The function computeShapeResidual calculates the two-dimensional integral of
// the square of the shape gradient of the probability over the boundary of the
// internal domain, which corresponds to the shape derivative of the probability
// taken in the direction of the shape gradient and represents the residual of
// the first-order optimality conditions for the shape optimization problem.
// It has the Parameters*, the Mesh*, the Data*, the ChemicalSystem* variables
// (both defined in main.h) and the integer iterationInTheLoop as input
// argument. It returns one if the quantity has been successfully computed and
// stored in the pData->d1p[iterationInTheLoop] variable, otherwise zero (error)
/* ************************************************************************** */
int computeShapeResidual(Parameters* pParameters, Mesh* pMesh, Data* pData,
                         ChemicalSystem* pChemicalSystem,
                                                         int iterationInTheLoop)
{
    int k=0, l=0, ip1=0, ip2=0, ip3=0, nProb=0, nMorb=0, nu=0, nVer=0, nTri=0;
    int nAdj=0;
    double area=0., integral=0., function=0., value=0., px=0., py=0., pz=0.;
    Probabilities* pProbabilities=NULL;
    OverlapMatrix* pOverlapMatrix=NULL;
    Point *pp1=NULL, *pp2=NULL, *pp3=NULL;

    // Check input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                                                          pChemicalSystem==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: at least one of the input ");
        fprintf(stderr,"variables pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, pData=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"or pChemicalSystem=%p ",(void*)pChemicalSystem);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check the input iterationInTheLoop variable
    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("computeShapeResidual: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"non-negative integer not (strictly) greater than the ");
        fprintf(stderr,"maximal number of allowed iterations ");
        fprintf(stderr,"(=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Check the number of mesh vertices
    nVer=pMesh->nver;
    if (nVer<1)
    {
        PRINT_ERROR("In computeShapeResidual: the total number of vertices ");
        fprintf(stderr,"(=%d) in the mesh should be a positive ",nVer);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    // Check that the array storing the mesh vertices is not pointing to NULL
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: the ");
        fprintf(stderr,"pver=%p variable of the structure ",(void*)pMesh->pver);
        fprintf(stderr,"pointed by pMesh does not have a valid address.\n");
        return 0;
    }

    if (pParameters->opt_mode<=0)
    {
        // Check the number of quadrilaterals for the internal domain
        nAdj=pMesh->nadj;
        if (nAdj<1 || pMesh->nqua<1)
        {
            PRINT_ERROR("In computeShapeResidual: the number of ");
            fprintf(stderr,"quadrilaterals (=%d) delimiting the ",nAdj);
            fprintf(stderr,"interior of the domain should be a positive ");
            fprintf(stderr,"integer as well as the total number of ");
            fprintf(stderr,"quadrilaterals (=%d) in the mesh.\n",pMesh->nqua);
            return 0;
        }

        // Check that the adjacency structure has been set up
        if (pMesh->padj==NULL || pMesh->pqua==NULL)
        {
            PRINT_ERROR("In computeShapeResidual: the Adjacency structures ");
            fprintf(stderr,"have not been set up in the hexahedral mesh ");
            fprintf(stderr,"(pMesh->padj=%p) or no ",(void*)pMesh->padj);
            fprintf(stderr,"quadrilaterals has been found ");
            fprintf(stderr,"(pMesh->pqua=%p).\n",(void*)pMesh->pqua);
            return 0;
        }
    }
    else
    {
        // Check the number of triangles
        nTri=pMesh->ntri;
        if (nTri<1)
        {
            PRINT_ERROR("In computeShapeResidual: the total number of ");
            fprintf(stderr,"triangles (=%d) in the mesh should be a ",nTri);
            fprintf(stderr,"positive integer.\n");
            return 0;
        }

        // Check that the pMesh->ptri variable is not pointing to NULL
        if (pMesh->ptri==NULL)
        {
            PRINT_ERROR("In computeShapeResidual: the boundary structures ");
            fprintf(stderr,"have not been set up in the tetrahedral mesh: ");
            fprintf(stderr,"pMesh->ptri=%p.\n",(void*)pMesh->ptri);
            return 0;
        }
    }

    // Check the total number of electrons
    nMorb=pChemicalSystem->nmorb;
    if (nMorb<1)
    {
        PRINT_ERROR("In computeShapeResidual: the total number of electrons ");
        fprintf(stderr,"(=%d) in the chemical system should be a ",nMorb);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    if (pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: the pmorb variable of the ");
        fprintf(stderr,"structure pointed by pChemicalSystem is pointing to ");
        fprintf(stderr,"the %p address.\n",(void*)pChemicalSystem->pmorb);
        return 0;
    }

    // Check the number nu of electrons we look for
    nu=pParameters->nu_electrons;
    if (nu<1 || nu>nMorb)
    {
        PRINT_ERROR("In computeShapeResidual: the number ");
        fprintf(stderr,"(=%d) of electrons we are looking for should be ",nu);
        fprintf(stderr,"a (positive) integer not (strictly) greater than ");
        fprintf(stderr,"the total number of electrons (=%d) in the ",nMorb);
        fprintf(stderr,"chemical system.\n");
        return 0;
    }

    // Check the size of the Probabilities structures
    nProb=pData->nprob;
    if (nProb!=1+nMorb)
    {
        PRINT_ERROR("In computeShapeResidual: the size of the array storing ");
        fprintf(stderr,"the different probabilities should be set to ");
        fprintf(stderr,"%d instead of %d.\n",nMorb+1,nProb);
        return 0;
    }

    // Check the d1p variable
    if (pData->d1p==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: the d1p variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->d1p);
        return 0;
    }

    // Set the pointer for the Probabilities structure
    if (pData->pprob==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: the pprob variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pprob);
        return 0;
    }
    pProbabilities=&pData->pprob[iterationInTheLoop];

    // Check the pkl variable of pProbabilities
    if (pProbabilities->pkl==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: the pkl variable of the ");
        fprintf(stderr,"%d-th Probabilities structure ",iterationInTheLoop+1);
        fprintf(stderr,"is pointing to the %p ",(void*)pProbabilities->pkl);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the size of the OverlapMatrix structures
    if (pData->nmat!=nMorb)
    {
        PRINT_ERROR("In computeShapeResidual: the common size defining the ");
        fprintf(stderr,"arrays of the different OverlapMatrix structures ");
        fprintf(stderr,"should be set to %d instead of ",nMorb);
        fprintf(stderr,"%d.\n",pData->nmat);
        return 0;
    }

    // Set the pointer for the overlap matrix
    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: the pmat variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pmat);
        return 0;
    }
    pOverlapMatrix=&pData->pmat[iterationInTheLoop];

    // Check the diag and vect pointer variables of pOverlapMatrix
    if (pOverlapMatrix->diag==NULL || pOverlapMatrix->vect==NULL)
    {
        PRINT_ERROR("In computeShapeResidual: at least one of the diag or ");
        fprintf(stderr,"vect variables of the %d-th ",iterationInTheLoop+1);
        fprintf(stderr,"OverlapMatrix structure ");
        fprintf(stderr,"(diag=%p, ",(void*)pOverlapMatrix->diag);
        fprintf(stderr,"vect=%p) does not point ",(void*)pOverlapMatrix->vect);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    if (pParameters->opt_mode>0)
    {
        // Compute the shape derivative of the probability by using Cowper's
        // formula (1973) on triangles for int_partialOmega (dp/dOmega)^2
        integral=0.;
        for (k=0; k<nTri; k++)
        {
            if (pMesh->ptri[k].label!=10)
            {
                continue;
            }

            ip1=pMesh->ptri[k].p1;
            ip2=pMesh->ptri[k].p2;
            ip3=pMesh->ptri[k].p3;

            pp1=&pMesh->pver[ip1-1];
            pp2=&pMesh->pver[ip2-1];
            pp3=&pMesh->pver[ip3-1];

            // First point evaluation
            px=CST_1*(pp1->x+pp2->x+pp3->x);
            py=CST_1*(pp1->y+pp2->y+pp3->y);
            pz=CST_1*(pp1->z+pp2->z+pp3->z);
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value=CST_ONE*function*function;

            // Second to fourth point evaluation
            px=CST_2*(pp1->x+pp2->x)+CST_22*pp3->x;
            py=CST_2*(pp1->y+pp2->y)+CST_22*pp3->y;
            pz=CST_2*(pp1->z+pp2->z)+CST_22*pp3->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            px=CST_2*(pp1->x+pp3->x)+CST_22*pp2->x;
            py=CST_2*(pp1->y+pp3->y)+CST_22*pp2->y;
            pz=CST_2*(pp1->z+pp3->z)+CST_22*pp2->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            px=CST_2*(pp2->x+pp3->x)+CST_22*pp1->x;
            py=CST_2*(pp2->y+pp3->y)+CST_22*pp1->y;
            pz=CST_2*(pp2->z+pp3->z)+CST_22*pp1->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            // Fifth to seventh point evaluation
            px=CST_3*(pp1->x+pp2->x)+CST_33*pp3->x;
            py=CST_3*(pp1->y+pp2->y)+CST_33*pp3->y;
            pz=CST_3*(pp1->z+pp2->z)+CST_33*pp3->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            px=CST_3*(pp1->x+pp3->x)+CST_33*pp2->x;
            py=CST_3*(pp1->y+pp3->y)+CST_33*pp2->y;
            pz=CST_3*(pp1->z+pp3->z)+CST_33*pp2->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            px=CST_3*(pp2->x+pp3->x)+CST_33*pp1->x;
            py=CST_3*(pp2->y+pp3->y)+CST_33*pp1->y;
            pz=CST_3*(pp2->z+pp3->z)+CST_33*pp1->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            // Evaluate the area of the triangle
            area=computeAreaTriangle(pp1,pp2,pp3);
            integral+=value*area;
        }

        // Save the value in the pData structure
        pData->d1p[iterationInTheLoop]=integral;
    }
    else
    {
        // Compute the shape derivative of the probability by using Cowper's
        // formula (1973) on triangles for int_partialOmega (dp/dOmega)^2
        integral=0.;
        for (k=0; k<nAdj; k++)
        {
            l=pMesh->padj[k].quad;

            // First half of the square
            ip1=pMesh->pqua[l-1].p1;
            ip2=pMesh->pqua[l-1].p2;
            ip3=pMesh->pqua[l-1].p4;

            pp1=&pMesh->pver[ip1-1];
            pp2=&pMesh->pver[ip2-1];
            pp3=&pMesh->pver[ip3-1];

            // First point evaluation
            px=CST_1*(pp1->x+pp2->x+pp3->x);
            py=CST_1*(pp1->y+pp2->y+pp3->y);
            pz=CST_1*(pp1->z+pp2->z+pp3->z);
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value=CST_ONE*function*function;

            // Second to fourth point evaluation
            px=CST_2*(pp1->x+pp2->x)+CST_22*pp3->x;
            py=CST_2*(pp1->y+pp2->y)+CST_22*pp3->y;
            pz=CST_2*(pp1->z+pp2->z)+CST_22*pp3->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            px=CST_2*(pp1->x+pp3->x)+CST_22*pp2->x;
            py=CST_2*(pp1->y+pp3->y)+CST_22*pp2->y;
            pz=CST_2*(pp1->z+pp3->z)+CST_22*pp2->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            px=CST_2*(pp2->x+pp3->x)+CST_22*pp1->x;
            py=CST_2*(pp2->y+pp3->y)+CST_22*pp1->y;
            pz=CST_2*(pp2->z+pp3->z)+CST_22*pp1->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            // Fifth to seventh point evaluation
            px=CST_3*(pp1->x+pp2->x)+CST_33*pp3->x;
            py=CST_3*(pp1->y+pp2->y)+CST_33*pp3->y;
            pz=CST_3*(pp1->z+pp2->z)+CST_33*pp3->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            px=CST_3*(pp1->x+pp3->x)+CST_33*pp2->x;
            py=CST_3*(pp1->y+pp3->y)+CST_33*pp2->y;
            pz=CST_3*(pp1->z+pp3->z)+CST_33*pp2->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            px=CST_3*(pp2->x+pp3->x)+CST_33*pp1->x;
            py=CST_3*(pp2->y+pp3->y)+CST_33*pp1->y;
            pz=CST_3*(pp2->z+pp3->z)+CST_33*pp1->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            // Evaluate the area of the triangle
            area=computeAreaTriangle(pp1,pp2,pp3);
            integral+=value*area;

            // Second half of the square
            ip1=pMesh->pqua[l-1].p2;
            ip2=pMesh->pqua[l-1].p3;
            ip3=pMesh->pqua[l-1].p4;

            pp1=&pMesh->pver[ip1-1];
            pp2=&pMesh->pver[ip2-1];
            pp3=&pMesh->pver[ip3-1];

            // First point evaluation
            px=CST_1*(pp1->x+pp2->x+pp3->x);
            py=CST_1*(pp1->y+pp2->y+pp3->y);
            pz=CST_1*(pp1->z+pp2->z+pp3->z);
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value=CST_ONE*function*function;

            // Second to fourth point evaluation
            px=CST_2*(pp1->x+pp2->x)+CST_22*pp3->x;
            py=CST_2*(pp1->y+pp2->y)+CST_22*pp3->y;
            pz=CST_2*(pp1->z+pp2->z)+CST_22*pp3->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            px=CST_2*(pp1->x+pp3->x)+CST_22*pp2->x;
            py=CST_2*(pp1->y+pp3->y)+CST_22*pp2->y;
            pz=CST_2*(pp1->z+pp3->z)+CST_22*pp2->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            px=CST_2*(pp2->x+pp3->x)+CST_22*pp1->x;
            py=CST_2*(pp2->y+pp3->y)+CST_22*pp1->y;
            pz=CST_2*(pp2->z+pp3->z)+CST_22*pp1->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_TWO*function*function;

            // Fifth to seventh point evaluation
            px=CST_3*(pp1->x+pp2->x)+CST_33*pp3->x;
            py=CST_3*(pp1->y+pp2->y)+CST_33*pp3->y;
            pz=CST_3*(pp1->z+pp2->z)+CST_33*pp3->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            px=CST_3*(pp1->x+pp3->x)+CST_33*pp2->x;
            py=CST_3*(pp1->y+pp3->y)+CST_33*pp2->y;
            pz=CST_3*(pp1->z+pp3->z)+CST_33*pp2->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            px=CST_3*(pp2->x+pp3->x)+CST_33*pp1->x;
            py=CST_3*(pp2->y+pp3->y)+CST_33*pp1->y;
            pz=CST_3*(pp2->z+pp3->z)+CST_33*pp1->z;
            function=evaluateShapeGradientAtThePoint(px,py,pz,nProb,
                                                     pProbabilities,nu,
                                                     pOverlapMatrix,
                                                               pChemicalSystem);
            value+=CST_THREE*function*function;

            // Evaluate the area of the triangle
            area=computeAreaTriangle(pp1,pp2,pp3);
            integral+=value*area;
        }

        // Save the value in the pData structure
        pData->d1p[iterationInTheLoop]=integral;
    }

    return 1;
}

/* ************************************************************************** */
// The function shapeDerivative computes the overlapMatrix, the probability,
// its shape gradient, and associated residual at iterationInTheLoop. In the
// case of hexahedral meshes with level-set perturbations, it also modify the
// mesh according to the sign of the shape gradient. It has the Parameters*,
// Mesh*, Data*, ChemicalSystem*, (defined in main.h), and the integer
// iterationInTheLoop as input arguments. It returns one on success, otherwise
// zero is returned if an error is encountered during the process
/* ************************************************************************** */
int shapeDerivative(Parameters* pParameters, Mesh* pMesh, Data* pData,
                        ChemicalSystem* pChemicalSystem, int iterationInTheLoop)
{
    int i=0, iMax=0, sizeMemory=0;

    // Check that the pParameters variable is not pointing to NULL
    if (pParameters==NULL)
    {
        PRINT_ERROR("In shapeDerivative: the input pParameters variable is ");
        fprintf(stderr,"pointing to the %p address.\n",(void*)pParameters);
        return 0;
    }

    // Distinguigh the case of tetrahedral meshes from the hexahedral one
    // Warning: if pParameters->opt_mode==4 we invert in computeShapeGradient
    // function the sign of the shape gradient to transform it into a level-set
    // function. More precisely, we look for the positive part of the shape
    // gradient (of the current domain) as the new domain in this optimization
    // mode. Considering the shape gradient as a level-set function, we thus
    // need to invert its sign so that the (new) domain can be identified as
    // its negative part by the external mshdist and mmg3d software, recovering
    // the standard interpretation of the domain via a level-set function
    if (pParameters->opt_mode>0)
    {

        if (pParameters->verbose)
        {
            fprintf(stdout,"\nWe recall that here -1 (respectively 2, 3) ");
            fprintf(stdout,"avoids all tetrahedra labelled minus one (resp. ");
            fprintf(stdout,"two, three) i.e. it computes the overlap matrix ");
            fprintf(stdout,"on the whole computational domain (resp. on the ");
            fprintf(stdout,"the MPD domain Omega, on the complement of ");
            fprintf(stdout,"Omega).\n");
        }

        // Computing overlap matrix. Warning: bounadary triangles are labbelled
        // 10 but not necessarily the related vertices (ok now and checked in
        // writeShapeSolFile function)
        if (!computeOverlapMatrix(pParameters,pMesh,pData,pChemicalSystem,2,
                                                            iterationInTheLoop))
        {
            PRINT_ERROR("In shapeDerivative: computeOverlapMatrix function ");
            fprintf(stderr,"returned zero instead of one.\n");
            return 0;
        }
    }
    else
    {
        if (!iterationInTheLoop)
        {
            if (pParameters->verbose)
            {

                fprintf(stdout,"\nWe recall that here -1 (respectively 2, 3) ");
                fprintf(stdout,"avoids all hexahedra labelled minus one ");
                fprintf(stdout,"(resp. two, three) i.e. it computes the ");
                fprintf(stdout,"overlap matrix on the whole computational ");
                fprintf(stdout,"domain (resp. on the the MPD domain Omega, ");
                fprintf(stdout,"on the complement of Omega).\n");
            }

            // Computing overlap matrix normally
            if (!computeOverlapMatrixOnGrid(pParameters,pMesh,pData,
                                          pChemicalSystem,2,iterationInTheLoop))
            {
                PRINT_ERROR("In shapeDerivative: computeOverlapMatrix ");
                fprintf(stderr,"function returned zero instead of one.\n");
                return 0;
            }
        }
        else
        {
            if (pParameters->trick_matrix)
            {
                if (pParameters->verbose)
                {
                    fprintf(stdout,"\nWe recall that here -3 (respectively ");
                    fprintf(stdout,"-2) considers all the hexahedra inside ");
                    fprintf(stderr,"the MPD domain Omega plus (resp. minus) ");
                    fprintf(stderr,"those next to it with positive (resp. ");
                    fprintf(stderr,"negative) shape gradient at the ");
                    fprintf(stderr,"connecting quadrilateral center point.\n");
                }

                // Otherwise add to the previous overlap matrix integrals on
                // hexa labelled -3 and remove integrals on those labelled -2
                // Warning: here first set -2 then -3 in labelToConsiderOrAvoid
                // input arguments of computeOverlapMatrixOnGrid, (order counts)
                if (!computeOverlapMatrixOnGrid(pParameters,pMesh,pData,
                                         pChemicalSystem,-2,iterationInTheLoop))
                {
                    PRINT_ERROR("In shapeDerivative: computeOverlapMatrix ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }
                if (!computeOverlapMatrixOnGrid(pParameters,pMesh,pData,
                                         pChemicalSystem,-3,iterationInTheLoop))
                {
                    PRINT_ERROR("In shapeDerivative: computeOverlapMatrix ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }

                // Check the number of hexahedra and its corresponding pointer
                iMax=pMesh->nhex;
                if (iMax<1)
                {
                    PRINT_ERROR("In hapeDerivative: expecting a positive ");
                    fprintf(stderr,"number of hexahedra instead of ");
                    fprintf(stderr,"%d in the pMesh structure.\n",iMax);
                    return 0;
                }
                if (pMesh->phex==NULL)
                {
                    PRINT_ERROR("In shapeDerivative: the phex variable of ");
                    fprintf(stderr,"the pMesh structure is pointing to the ");
                    fprintf(stderr,"%p address.\n",(void*)pMesh->phex);
                    return 0;
                }

                // Act the fact that the new domain can now be considered
                for (i=0; i<iMax; i++)
                {
                    pMesh->phex[i].label=abs(pMesh->phex[i].label);
                }
            }
            else
            {
                if (pParameters->verbose)
                {

                    fprintf(stdout,"\nWe recall that here -1 (respectively ");
                    fprintf(stdout,"2, 3) avoids all hexahedra labelled ");
                    fprintf(stdout,"minus one (resp. two, three) i.e. it ");
                    fprintf(stdout,"computes the overlap matrix on the whole ");
                    fprintf(stdout,"computational domain (resp. on the the ");
                    fprintf(stdout,"MPD domain Omega, on the complement of ");
                    fprintf(stdout,"Omega).\n");
                }

                // Computing overlap matrix normally
                if (!computeOverlapMatrixOnGrid(pParameters,pMesh,pData,
                                          pChemicalSystem,2,iterationInTheLoop))
                {
                    PRINT_ERROR("In shapeDerivative: computeOverlapMatrix ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }
            }

            // Free the memory allocated for Adjacency table
            sizeMemory=sizeof(int)+(pMesh->nadj)*sizeof(Adjacency);
            fprintf(stdout,"\nCleaning allocated memory ");
            fprintf(stdout,"(%d,",sizeMemory/1000000);
            fprintf(stdout,"%d Mo) for the ",sizeMemory%1000000);
            fprintf(stdout,"Adjacency structures. ");
            free(pMesh->padj);
            pMesh->padj=NULL;
            pMesh->nadj=0;

            // Rebuild the Adjacency structures and boundary quandrilaterals
            fprintf(stdout,"\nBuild the new Adjacency structures to get the ");
            fprintf(stdout,"updated domain.\n");
            if (!getLevelSetQuadrilaterals(pParameters,pMesh))
            {
                PRINT_ERROR("In shapeDerivative: getLevelSetQuadrilaterals ");
                fprintf(stderr,"function returned zero instead of one.\n");
                return 0;
            }
        }
    }

    // Diagonalize the overlap matrix. Warning here: we use lapacke.h,
    // and lapacke package must have been previously installed
    if (!diagonalizeOverlapMatrix(pParameters,pData,iterationInTheLoop))
    {
        PRINT_ERROR("In shapeDerivative: diagonalizeOverlapMatrix function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Compute the probabilities and total population inside the domain
    if (!computeProbability(pParameters,pData,iterationInTheLoop))
    {
        PRINT_ERROR("In shapeDerivative: computeProbability function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Compute the shape gradient of the probability
    if (!computeShapeGradient(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
    {
        PRINT_ERROR("In shapeDerivative: computeShapeGradient function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    if (!computeShapeResidual(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
    {
        PRINT_ERROR("In shapeDerivative: computeShapeResidual function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    return 1;
}

/* ************************************************************************** */
// The function writingShapeSolFile writes into a (vectorial) *.sol file the
// values associated with the product of what is stored in pMesh->pver[i].value
// by the associated normal vector coordinates, where i stands the vertices
// that belongs to the boundary of the MPD domain; otherwise the values written
// are set to zero. The name of the *.sol file is the one stored in
// pParameters->name_mesh but where the *.mesh extension has been replaced by
// the *.sol one (warning: it reset and overwrites the file if it already
// exists). It has the Parameters* and Mesh* variables (both defined in the
// main.h) as input arguments and it returns zero if an error occurred,
// otherwise one on success
/* ************************************************************************** */
int writingShapeSolFile(Parameters* pParameters, Mesh* pMesh)
{
    size_t lengthName=0;
    char* fileLocation=NULL;
    int i=0, iMax=0, j=0, jMin=0, jMax=0;
    double nx=0., ny=0., nz=0., value=0.;
    FILE *shapeFile=NULL;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In writingShapeSolFile: one of the input variables ");
        fprintf(stderr,"pParameters=%p or ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p is not a valid address.\n",(void*)pMesh);
        return 0;
    }

    // Check we are in dealing with the tetrahedral mesh
    if (pParameters->opt_mode<=0)
    {
        PRINT_ERROR("In writingShapeSolFile: the function can only be used ");
        fprintf(stderr,"if the pParameters->opt_mode variable ");
        fprintf(stderr,"(=%d) is a positive integer.\n",pParameters->opt_mode);
        return 0;
    }

    // Check the number of vertices
    iMax=pMesh->nver;
    if (iMax<1)
    {
        PRINT_ERROR("In writingShapeSolFile: the total number of vertices ");
        fprintf(stderr,"(=%d) in the tetrahedral mesh should be a ",iMax);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    // Check the number of normal vectors in pMesh
    iMax=pMesh->nnorm;
    if (iMax<1)
    {
        PRINT_ERROR("In writingShapeSolFile: the total number of normal ");
        fprintf(stderr," vectors (=%d) in the tetrahedral mesh should ",iMax);
        fprintf(stderr,"be a positive integer.\n");
        return 0;
    }

    // Check the mesh pointers used in this function
    if (pMesh->pver==NULL || pMesh->pnorm==NULL)
    {
        PRINT_ERROR("In writingShapeSolFile: one of the variables ");
        fprintf(stderr,"pMesh->pver=%p or ",(void*)pMesh->pver);
        fprintf(stderr,"pMesh->pnorm=%p does not point ",(void*)pMesh->pnorm);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check that normal vector were stored in increasing order otherwise the
    // code for this function must be modified (rebuilt a normal adjacency)
    iMax=pMesh->nnorm;
    for (i=1; i<iMax; i++)
    {
        if (pMesh->pnorm[i-1].p>=pMesh->pnorm[i].p)
        {
            PRINT_ERROR("In writingShapeSolFile: the ");
            fprintf(stderr,"%d-th and ",pMesh->pnorm[i-1].p);
            fprintf(stderr,"%d-th normal vectors were not ",pMesh->pnorm[i].p);
            fprintf(stderr,"stored in increasing order.\nPlease list the ");
            fprintf(stderr,"normal vectors of the *.mesh file in increasing ");
            fprintf(stderr,"order so as to proceed further.\n");
            return 0;
        }
    }

    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In writingShapeSolFile: getMeshFormat function did not ");
        fprintf(stderr,"return one, which was the expected value here.\n");
        return 0;
    }

    // Allocate memory for fileLocation and checked if it worked
    // calloc function returns a pointer to allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In writingShapeSolFile: could not allocate memory for ");
        fprintf(stderr,"the (local) char* fileLocation variable.\n");
        return 0;
    }

    // strncpy returns a pointer to the string (not used here)
    // strlen function returns the length of the string not including the '\0'
    strncpy(fileLocation,pParameters->name_mesh,lengthName);
    lengthName=strlen(fileLocation);
    fileLocation[lengthName-5]='.';
    fileLocation[lengthName-4]='s';
    fileLocation[lengthName-3]='o';
    fileLocation[lengthName-2]='l';
    fileLocation[lengthName-1]='\0';

    // Opening *.sol file (warning: reset and overwrite file if already exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"\nOpening %s file. ",fileLocation);
    }
    shapeFile=fopen(fileLocation,"w+");
    if (shapeFile==NULL)
    {
        PRINT_ERROR("In writingShapeSolFile: we were not able to open and ");
        fprintf(stderr,"write into the %s file",fileLocation);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Start writing vectorial data. ");
    }

    // Writing according to the *.sol format: MeshVersionFormated (1=single,
    // 2=double precision), Dimension (2 or 3), SolAtVertices numberOfSolution
    // typeOfSolution (1=scalar, 2=vector, 3=symmetric tensor)
    fprintf(shapeFile,"MeshVersionFormatted 2\n\nDimension 3\n\n");
    fprintf(shapeFile,"SolAtVertices\n%d\n1 2 \n\n",pMesh->nver);

    jMin=0;
    iMax=pMesh->nnorm;
    for (i=0; i<iMax; i++)
    {
        jMax=(pMesh->pnorm[i].p)-1;

        for (j=jMin; j<jMax; j++)
        {
            fprintf(shapeFile,"%.8le %.8le %.8le \n",0.,0.,0.);

            // Label zero the point with no normal except those on ridges
            // (12,14,15,16,23,25,26,34,35,36,45,46) and corners
            // (125,126,145,146,235,236,345,346)
            switch (pMesh->pver[j].label)
            {
                case 12:
                case 14:
                case 15:
                case 16:
                case 23:
                case 25:
                case 26:
                case 34:
                case 35:
                case 36:
                case 45:
                case 46:
                case 125:
                case 126:
                case 145:
                case 146:
                case 235:
                case 236:
                case 345:
                case 346:
                    break;

                default:
                    pMesh->pver[j].label=0;
                    break;
            }
        }

        // Write the value times the normal vector
        value=pMesh->pver[jMax].value;
        nx=value*(pMesh->pnorm[i].x);
        ny=value*(pMesh->pnorm[i].y);
        nz=value*(pMesh->pnorm[i].z);
        fprintf(shapeFile,"%.8le %.8le %.8le \n",nx,ny,nz);

        if (pMesh->pver[jMax].label<1 || pMesh->pver[jMax].label>6)
        {
            // Label 10 the point with normal vector except those on the
            // computational box faces (1,2,3,4,5,6)
            if (pMesh->pver[jMax].label!=10)
            {
                PRINT_ERROR("In writingShapeSolFile: the label of the ");
                fprintf(stderr,"%d-th vertex of the mesh should be ",jMax+1);
                fprintf(stderr,"equal to 10 instead of ");
                fprintf(stderr,"%d.\n",pMesh->pver[jMax].label);
                closeTheFile(&shapeFile);
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }
        }

        jMin=jMax+1;
    }

    if (jMin<pMesh->nver)
    {
        jMax=pMesh->nver;
        for (j=jMin; j<jMax; j++)
        {
            fprintf(shapeFile,"%.8le %.8le %.8le \n",0.,0.,0.);

            // Label zero the point with no normal except those on ridges
            // (12,14,15,16,23,25,26,34,35,36,45,46) and corners
            // (125,126,145,146,235,236,345,346)
            switch (pMesh->pver[j].label)
            {
                case 12:
                case 14:
                case 15:
                case 16:
                case 23:
                case 25:
                case 26:
                case 34:
                case 35:
                case 36:
                case 45:
                case 46:
                case 125:
                case 126:
                case 145:
                case 146:
                case 235:
                case 236:
                case 345:
                case 346:
                    break;

                default:
                    pMesh->pver[j].label=0;
                    break;
            }
        }
    }
    fprintf(shapeFile,"\nEnd");

    // Closing the *.sol file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(shapeFile))
    {
        PRINT_ERROR("In writingShapeSolFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        shapeFile=NULL;
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    shapeFile=NULL;
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Closing file.\n");
    }

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

/* ************************************************************************** */
// The function writingObjFile writes into a *.obj file the 3 coordinates of the
// vertices of an internal domain contained in pMesh, and also the boundary
// triangles associated to it (labelled 10) with the right vertex connectivity.
// The name of the *.obj file is the one stored in pParameters->name_mesh but
// where the *.mesh extension has been replaced by the *.obj one (warning: it
// reset and overwrites the file if it already exists). It has the Parameters*
// and Mesh* variables (both defined in the main.h) as input arguments and it
// returns zero if an error occurred, otherwise one on success
/* ************************************************************************** */
int writingObjFile(Parameters* pParameters, Mesh* pMesh)
{
    size_t lengthName=0;
    char *fileLocation=NULL;
    int i=0, j=0, iMax=0, nTri=0, *pTri=NULL;
    FILE *objFile=NULL;

    // Check the input pointers
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In writingObjFile: at least one of the input ");
        fprintf(stderr,"variables (pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p) ",(void*)pMesh);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check that we are dealing with a tetrahedral mesh
    if (pParameters->opt_mode<=0)
    {
        PRINT_ERROR("In writingObjFile: the function can only be used ");
        fprintf(stderr,"if the pParameters->opt_mode variable ");
        fprintf(stderr,"(=%d) is a positive integer.\n",pParameters->opt_mode);
        return 0;
    }

    // Check the number of vertices
    iMax=pMesh->nver;
    if (iMax<1)
    {
        PRINT_ERROR("In writingObjFile: the total number of vertices ");
        fprintf(stderr,"(=%d) in the tetrahedral mesh should be a ",iMax);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    // Check the number of triangles
    iMax=pMesh->ntri;
    if (iMax<1)
    {
        PRINT_ERROR("In writingObjFile: the total number of boundary ");
        fprintf(stderr," triangles (=%d) in the tetrahedral mesh should ",iMax);
        fprintf(stderr,"be a positive integer.\n");
        return 0;
    }

    // Check the corresponding pointers
    if (pMesh->pver==NULL || pMesh->ptri==NULL)
    {
        PRINT_ERROR("In writingObjFile: one of the variables ");
        fprintf(stderr,"pMesh->pver=%p or ",(void*)pMesh->pver);
        fprintf(stderr,"pMesh->ptri=%p does not point ",(void*)pMesh->ptri);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check that an internal domain is encoded in the mesh
    nTri=0;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptri[i].label==10)
        {
            nTri++;
        }
    }
    if (nTri<1)
    {
        PRINT_ERROR("In writingObjFile: the tetrahedral mesh does not seem ");
        fprintf(stderr,"to contain any internal domain. The number of ");
        fprintf(stderr,"boundary triangles (=%d) labelled ten in the ",nTri);
        fprintf(stderr,"current mesh should be a positive integer.\n"); 
        return 0;
    }

    // Check the pParameters->name_mesh variable
    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In writingObjFile: getMeshFormat function did not ");
        fprintf(stderr,"return one, which was the expected value here.\n");
        return 0;
    }

    // Allocate memory for fileLocation and checked if it worked
    // calloc function returns a pointer to allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In writingObjFile: could not allocate memory for ");
        fprintf(stderr,"the (local) char* fileLocation variable.\n");
        return 0;
    }

    // strncpy returns a pointer to the string (not used here)
    // strlen function returns the length of the string not including the '\0'
    strncpy(fileLocation,pParameters->name_mesh,lengthName);
    lengthName=strlen(fileLocation);
    fileLocation[lengthName-5]='.';
    fileLocation[lengthName-4]='o';
    fileLocation[lengthName-3]='b';
    fileLocation[lengthName-2]='j';
    fileLocation[lengthName-1]='\0';

    // Opening *.obj file (warning: reset and overwrite file if already exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"\nOpening %s file. ",fileLocation);
    }
    objFile=fopen(fileLocation,"w+");
    if (objFile==NULL)
    {
        PRINT_ERROR("In writingObjFile: we were not able to open and ");
        fprintf(stderr,"write into the %s file",fileLocation);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Start writing object data. ");
    }

    // Allocate memory for the adjacency table of triangles
    pTri=(int*)calloc(3*pMesh->ntri,sizeof(int));
    if (pTri==NULL)
    {
        PRINT_ERROR("In writingObjFile: could not allocate memory for the ");
        fprintf(stderr,"local (int*) pTri variable.\n");
        closeTheFile(&objFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    
    // Write the vertices by setting a triangle adjacency
    iMax=pMesh->ntri;
    nTri=0;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptri[i].label==10)
        {
            // Check for repetition of p1 before saving the point
            for (j=0; j<i; j++)
            {
                if (pMesh->ptri[j].label==10)
                {
                    if (pMesh->ptri[i].p1==pMesh->ptri[j].p1)
                    {
                        pTri[3*i]=pTri[3*j];
                        break;
                    }
                    else if (pMesh->ptri[i].p1==pMesh->ptri[j].p2)
                    {
                        pTri[3*i]=pTri[3*j+1];
                        break;
                    }
                    else if (pMesh->ptri[i].p1==pMesh->ptri[j].p3)
                    {
                        pTri[3*i]=pTri[3*j+2];
                        break;
                    }
                }
            }
            if (j==i)
            {
                pTri[3*i]=nTri;
                nTri++;
                fprintf(objFile,"v %.8le ",pMesh->pver[pMesh->ptri[i].p1-1].x);
                fprintf(objFile,"%.8le ",pMesh->pver[pMesh->ptri[i].p1-1].y);
                fprintf(objFile,"%.8le \n",pMesh->pver[pMesh->ptri[i].p1-1].z);
            }

            // Check for repetition of p2 before saving the point
            for (j=0; j<i; j++)
            {
                if (pMesh->ptri[j].label==10)
                {
                    if (pMesh->ptri[i].p2==pMesh->ptri[j].p1)
                    {
                        pTri[3*i+1]=pTri[3*j];
                        break;
                    }
                    else if (pMesh->ptri[i].p2==pMesh->ptri[j].p2)
                    {
                        pTri[3*i+1]=pTri[3*j+1];
                        break;
                    }
                    else if (pMesh->ptri[i].p2==pMesh->ptri[j].p3)
                    {
                        pTri[3*i+1]=pTri[3*j+2];
                        break;
                    }
                }
            }
            if (j==i)
            {
                pTri[3*i+1]=nTri;
                nTri++;
                fprintf(objFile,"v %.8le ",pMesh->pver[pMesh->ptri[i].p2-1].x);
                fprintf(objFile,"%.8le ",pMesh->pver[pMesh->ptri[i].p2-1].y);
                fprintf(objFile,"%.8le \n",pMesh->pver[pMesh->ptri[i].p2-1].z);
            }

            // Check for repetition of p3 before saving the point
            for (j=0; j<i; j++)
            {
                if (pMesh->ptri[j].label==10)
                {
                    if (pMesh->ptri[i].p3==pMesh->ptri[j].p1)
                    {
                        pTri[3*i+2]=pTri[3*j];
                        break;
                    }
                    else if (pMesh->ptri[i].p3==pMesh->ptri[j].p2)
                    {
                        pTri[3*i+2]=pTri[3*j+1];
                        break;
                    }
                    else if (pMesh->ptri[i].p3==pMesh->ptri[j].p3)
                    {
                        pTri[3*i+2]=pTri[3*j+2];
                        break;
                    }
                }
            }
            if (j==i)
            {
                pTri[3*i+2]=nTri;
                nTri++;
                fprintf(objFile,"v %.8le ",pMesh->pver[pMesh->ptri[i].p3-1].x);
                fprintf(objFile,"%.8le ",pMesh->pver[pMesh->ptri[i].p3-1].y);
                fprintf(objFile,"%.8le \n",pMesh->pver[pMesh->ptri[i].p3-1].z);
            }
        }
    }
    fprintf(objFile,"\n");

    // Saving triangles with the right adjacency
    iMax=pMesh->ntri;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptri[i].label==10)
        {
            fprintf(objFile,"f %d %d ",pTri[3*i]+1,pTri[3*i+1]+1);
            fprintf(objFile,"%d \n",pTri[3*i+2]+1);
        }
    }
    fprintf(objFile,"\n");

    // Free the memory allocated for pTri
    free(pTri);
    pTri=NULL;

    // Closing the *.obj file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(objFile))
    {
        PRINT_ERROR("In writingObjFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        objFile=NULL;
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    objFile=NULL;
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Closing file.\n");
    }

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

/* ************************************************************************** */
// The function saveOrRemoveMeshInTheLoop is used in the optimization loop
// to either save the mesh in the *.mesh and/or *.cube/*.obj format according to
// the pParameters->save_type and iterationInTheLoop variables, or remove the
// *.(iterationInTheLoop).mesh file if it already exists, depending if the
// booleanForSave input variable is set to one or zero. In the latter case, the
// file must already exists as well as its associated *.mesh one whose path
// name is stored in the Parameters->name_mesh variable. We recall that the
// *.cube format requires the charge and center coordinates of the nuclei
// associated with the chemical system. The function has the Parameters*,
// Mesh*, ChemicalSystem* (both defined in main.h) and the integer variable
// iterationInTheLoop as input arguments. It returns one one success, otherwise
// zero is returned uf an error is encountered during the process
/* ************************************************************************** */
int saveOrRemoveMeshInTheLoop(Parameters* pParameters, Mesh* pMesh,
                              ChemicalSystem* pChemicalSystem,
                                     int booleanForSave, int iterationInTheLoop)
{
    size_t lengthName=0;
    char *fileLocation=NULL;

    // Check the input pointers
    if (pParameters==NULL || pMesh==NULL || pChemicalSystem==NULL)
    {
        PRINT_ERROR("In saveOrRemoveMeshInTheLoop: at least one of the input ");
        fprintf(stderr,"variables (pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, ",(void*)pMesh);
        fprintf(stderr,"pChemicalSystem=%p) ",(void*)pChemicalSystem);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    if (booleanForSave)
    {
        // Saving the mesh discretization in a *.mesh file
        if (!writingMeshFile(pParameters,pMesh))
        {
            PRINT_ERROR("In saveOrRemoveMeshInTheLoop: writingMeshFile ");
            fprintf(stderr,"function returned zero instead of one.\n");
            return 0;
        }
    }
    else
    {
        // Check pParameters->name_mesh variable
        if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
        {
            PRINT_ERROR("In saveOrRemoveMeshInTheLoop: getMeshFormat ");
            fprintf(stderr,"function did not returned one, which was the ");
            fprintf(stderr,"expected value here.\n");
            return 0;
        }
    }

    // strlen returns the length of the string without the terminating '\0'
    lengthName=strlen(pParameters->name_mesh);
    if ((int)lengthName>pParameters->name_length-11)
    {
        PRINT_ERROR("In saveOrRemoveMeshInTheLoop: the maximal size ");
        fprintf(stderr,"(=%d) allocated for the ",pParameters->name_length-11);
        fprintf(stderr,"names has not been increased from ten.\n");
        return 0;
    }

    // calloc function returns a pointer to the allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In saveOrRemoveMeshInTheLoop: could not allocate memory ");
        fprintf(stderr,"for the local char* fileLocation variable.\n");
        return 0;
    }

    // sprintf returns the total number of characters written is returned
    // not including the char '\0', otherwise a negative number on failure
    lengthName=strlen(pParameters->name_mesh);
    pParameters->name_mesh[lengthName-5]='\0';
    if (sprintf(fileLocation,"%s.%d.mesh",pParameters->name_mesh,
                                                          iterationInTheLoop)<0)
    {
        PRINT_ERROR("In saveOrRemoveMeshInTheLoop: the standard sprintf ");
        fprintf(stderr,"c-function returned an unexpected negative ");
        fprintf(stderr,"value, while attempting to save the mesh at the ");
        fprintf(stderr,"%d-th iteration of the ",iterationInTheLoop);
        fprintf(stderr,"optimization loop.\n");
        free(fileLocation);    // free function does not return any value
        fileLocation=NULL;
        return 0;
    }
    pParameters->name_mesh[lengthName-5]='.';

    if (initialFileExists(fileLocation,pParameters->name_length)==1)
    {
        // remove returns 0 on success, otherwise -1
        if (remove(fileLocation))
        {
            PRINT_ERROR("In saveOrRemoveMeshInTheLoop: wrong return (=-1) of ");
            fprintf(stderr,"the standard remove c-function in the ");
            fprintf(stderr,"attempt of removing the %s ",fileLocation);
            fprintf(stderr,"file.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }
    }

    if (booleanForSave)
    {
        if (!copyFileLocation(pParameters->name_mesh,pParameters->name_length,
                                                                  fileLocation))
        {
            PRINT_ERROR("In saveOrRemoveMeshInTheLoop: copyFileLocation ");
            fprintf(stderr,"function returned zero instead of one.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (pParameters->opt_mode<=0 && pParameters->save_type!=1)
        {
            if (!writingCubeFile(pParameters,pChemicalSystem,pMesh))
            {
                PRINT_ERROR("In saveOrRemoveMeshInTheLoop: writingCubeFile ");
                fprintf(stderr,"function returned zero instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            if (!pParameters->save_type)
            {
                fprintf(stdout,"rm %s",fileLocation);
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In saveOrRemoveMeshInTheLoop: wrong return ");
                    fprintf(stderr,"(=-1) of the standard remove c-function ");
                    fprintf(stderr,"in the attempt of removing the ");
                    fprintf(stderr,"%s file.\n",fileLocation);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            lengthName=strlen(fileLocation);
            fileLocation[lengthName-5]='.';
            fileLocation[lengthName-4]='c';
            fileLocation[lengthName-3]='u';
            fileLocation[lengthName-2]='b';
            fileLocation[lengthName-1]='e';
            fileLocation[lengthName]='\0';

            lengthName=strlen(pParameters->name_mesh);
            pParameters->name_mesh[lengthName-5]='.';
            pParameters->name_mesh[lengthName-4]='c';
            pParameters->name_mesh[lengthName-3]='u';
            pParameters->name_mesh[lengthName-2]='b';
            pParameters->name_mesh[lengthName-1]='e';
            pParameters->name_mesh[lengthName]='\0';

            if (initialFileExists(fileLocation,pParameters->name_length)==1)
            {
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In saveOrRemoveMeshInTheLoop: wrong return ");
                    fprintf(stderr,"(=-1) of the standard remove c-function ");
                    fprintf(stderr,"in the attempt of removing the ");
                    fprintf(stderr,"%s file.\n",fileLocation);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            if (!renameFileLocation(pParameters->name_mesh,
                                         pParameters->name_length,fileLocation))
            {
                PRINT_ERROR("In saveOrRemoveMeshInTheLoop: ");
                fprintf(stderr,"renameFileLocation function returned zero ");
                fprintf(stderr,"instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            lengthName=strlen(pParameters->name_mesh);
            pParameters->name_mesh[lengthName-5]='.';
            pParameters->name_mesh[lengthName-4]='m';
            pParameters->name_mesh[lengthName-3]='e';
            pParameters->name_mesh[lengthName-2]='s';
            pParameters->name_mesh[lengthName-1]='h';
            pParameters->name_mesh[lengthName]='\0';
        }
        else if (pParameters->opt_mode>0 && pParameters->save_type!=1)
        {
            if (!writingObjFile(pParameters,pMesh))
            {
                PRINT_ERROR("In saveOrRemoveMeshInTheLoop: writingObjFile ");
                fprintf(stderr,"function returned zero instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            if (!pParameters->save_type)
            {
                fprintf(stdout,"rm %s",fileLocation);
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In saveOrRemoveMeshInTheLoop: wrong return ");
                    fprintf(stderr,"(=-1) of the standard remove c-function ");
                    fprintf(stderr,"in the attempt of removing the ");
                    fprintf(stderr,"%s file.\n",fileLocation);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            lengthName=strlen(fileLocation);
            fileLocation[lengthName-5]='.';
            fileLocation[lengthName-4]='o';
            fileLocation[lengthName-3]='b';
            fileLocation[lengthName-2]='j';
            fileLocation[lengthName-1]='\0';

            lengthName=strlen(pParameters->name_mesh);
            pParameters->name_mesh[lengthName-5]='.';
            pParameters->name_mesh[lengthName-4]='o';
            pParameters->name_mesh[lengthName-3]='b';
            pParameters->name_mesh[lengthName-2]='j';
            pParameters->name_mesh[lengthName-1]='\0';

            if (initialFileExists(fileLocation,pParameters->name_length)==1)
            {
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In saveOrRemoveMeshInTheLoop: wrong return ");
                    fprintf(stderr,"(=-1) of the standard remove c-function ");
                    fprintf(stderr,"in the attempt of removing the ");
                    fprintf(stderr,"%s file.\n",fileLocation);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            if (!renameFileLocation(pParameters->name_mesh,
                                         pParameters->name_length,fileLocation))
            {
                PRINT_ERROR("In saveOrRemoveMeshInTheLoop: ");
                fprintf(stderr,"renameFileLocation function returned zero ");
                fprintf(stderr,"instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            lengthName=strlen(pParameters->name_mesh);
            pParameters->name_mesh[lengthName-4]='.';
            pParameters->name_mesh[lengthName-3]='m';
            pParameters->name_mesh[lengthName-2]='e';
            pParameters->name_mesh[lengthName-1]='s';
            pParameters->name_mesh[lengthName]='h';
            pParameters->name_mesh[lengthName+1]='\0';
        }
    }

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

/* ************************************************************************** */
// The function saveDataInTheLoop finishes to evaluate the non-already-computed
// data at the (iterationInTheLoop)-th iteration of the optimization loop, saves
// the mesh in a *.(iterationInTheLoop).mesh/ *.(iterationInTheLoop).cube file
// thanks to the saveOrRemoveMeshInTheLoop function (since saveDataInTheLoop is
// performed at the end of the optimization loop, this mesh will correspond to
// the one of the next iteration, thus getting a copy in case of crash), and
// deletes the *.(iterationInTheLoop-1).mesh/*.(iterationInTheLoop-1).cube
// previous mesh file except if the pParameters->save_mesh variable is positive
// and a multiple of (iterationInTheLoop-1). Similarly, if
// pParameters->save_data is positive and a multiple of iterationInTheLoop,
// it saves all the calculated values stored in the structure pointed by pData,
// appending them in the *.data file whose name is stored in the
// pParameters->name_data variable. It has the Parameters*, Mesh*, Data*,
// ChemicalSystem* structures (both defined in main.h), the integer
// iterationInTheLoop as input arguments, and also the three time_t* variables
// (pGlobalInitialTimer, pStartLocalTimer, pEndLocalTimer) that are used to
// compute the execution time of one iteration and the cumulated time. The
// function returns one on success, otherwise zero is returned in case of error
/* ************************************************************************** */
int saveDataInTheLoop(Parameters* pParameters, Mesh* pMesh, Data* pData,
                      ChemicalSystem* pChemicalSystem, int iterationInTheLoop,
                      time_t* pGlobalInitialTimer, time_t* pStartLocalTimer,
                                                         time_t* pEndLocalTimer)
{
    size_t lengthName=0;
    FILE *dataFile=NULL;

    // Check the input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                           pChemicalSystem==NULL || pGlobalInitialTimer==NULL ||
                                 pStartLocalTimer==NULL || pEndLocalTimer==NULL)
    {
        PRINT_ERROR("In saveDataInTheLoop: one of the input pointers ");
        fprintf(stderr,"pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pData=%p, pMesh=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"pChemicalSystem=%p, ",(void*)pChemicalSystem);
        fprintf(stderr,"pGlobalInitialTimer=%p, ",(void*)pGlobalInitialTimer);
        fprintf(stderr,"pStartLocalTimer=%p or ",(void*)pStartLocalTimer);
        fprintf(stderr,"pEndLocalTimer=%p, ",(void*)pEndLocalTimer);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check the iterationInTheLoop variable
    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In saveDataInTheLoop: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"non-negative integer not (strictly) greater than ");
        fprintf(stderr,"the maximal number (=%d) ",pParameters->iter_max);
        fprintf(stderr,"of iterations allowed in the optimizationn loop.\n");
        return 0;
    }

    // Check the pointers variables of the structure pointed by pData
    if (pData->pnu==NULL || pData->pop==NULL || pData->d0p==NULL ||
                     pData->d1p==NULL || pData->d2p==NULL || pData->tim==NULL ||
                                                              pData->ctim==NULL)
    {
        PRINT_ERROR("In saveDataInTheLoop: at least one of the input pointers");
        fprintf(stderr,"pData->pnu=%p, ",(void*)pData->pnu);
        fprintf(stderr,"pData->pop=%p, ",(void*)pData->pop);
        fprintf(stderr,"pData->d0p=%p, ",(void*)pData->d0p);
        fprintf(stderr,"pData->d1p=%p, ",(void*)pData->d1p);
        fprintf(stderr,"pData->d2p=%p, ",(void*)pData->d2p);
        fprintf(stderr,"pData->tim=%p, ",(void*)pData->tim);
        fprintf(stderr,"pData->ctim=%p, ",(void*)pData->ctim);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Update the d0p and d1p variables of the structure pointed by pData
    if (!iterationInTheLoop)
    {
        pData->d0p[iterationInTheLoop]=pData->pnu[iterationInTheLoop];
        if (pParameters->opt_mode>0)
        {
            pData->d2p[iterationInTheLoop]=pData->d1p[iterationInTheLoop];
        }
        else
        {
            pData->d2p[iterationInTheLoop]=pData->d0p[iterationInTheLoop];
        }
    }
    else
    {
        pData->d0p[iterationInTheLoop]=DEF_ABS(pData->pnu[iterationInTheLoop]
                                             -pData->pnu[iterationInTheLoop-1]);
        if (pParameters->opt_mode>0)
        {
            pData->d2p[iterationInTheLoop]=
                                          DEF_ABS(pData->d1p[iterationInTheLoop]
                                             -pData->d1p[iterationInTheLoop-1]);
        }
        else
        {
            pData->d2p[iterationInTheLoop]=
                                          DEF_ABS(pData->d0p[iterationInTheLoop]
                                             -pData->d0p[iterationInTheLoop-1]);
        }
    }

    // Save the mesh in case of crash. Important remark: since saveDataInTheLoop
    // will be placed at the end of the optimization loop, the mesh saved
    // correspond in fact to the (iterationInTheLoop+1) one in the loop (an
    // *.0.mesh file is automatically saved at initialization). Hence, if the
    // program crash, a mesh will be there, and if it do not crash, it will be
    // deleted at the next iteration by this function (see below) except of
    // course if the saving options stored in pParameters->save_print is
    // positive and a multiple of the iterationInTheLoop variable
    if (!saveOrRemoveMeshInTheLoop(pParameters,pMesh,pChemicalSystem,1,
                                                            iterationInTheLoop))
    {
        PRINT_ERROR("In saveDataInTheLoop: saveOrRemoveMeshInTheLoop ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

    // Delete the mesh of the previous iteration if not specified in pParameters
    if (iterationInTheLoop>1)
    {
        if (pParameters->save_mesh<=0)
        {
            if (!saveOrRemoveMeshInTheLoop(pParameters,pMesh,pChemicalSystem,0,
                                                          iterationInTheLoop-1))
            {
                PRINT_ERROR("In saveDataInTheLoop: ");
                fprintf(stderr,"saveOrRemoverMeshInTheLoop function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }
        }
        else
        {
            if ((iterationInTheLoop-1)%pParameters->save_mesh)
            {
                if (!saveOrRemoveMeshInTheLoop(pParameters,pMesh,
                                               pChemicalSystem,0,
                                                          iterationInTheLoop-1))
                {
                    PRINT_ERROR("In saveDataInTheLoop: ");
                    fprintf(stderr,"saveOrRemoverMeshInTheLoop function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }
    }

    // Check the pParameters->name_data variable
    if (!checkStringFromLength(pParameters->name_data,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In saveDataInTheLoop: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the charù name_data ");
        fprintf(stderr,"variable of the structure pointed by pParameters is ");
        fprintf(stderr,"not a string of length (strictly) less than ");
        fprintf(stderr,"%d (and more than 5 ",pParameters->name_length);
        fprintf(stderr,"in order to store at least something more ");
        fprintf(stderr,"than the *.data extension).\n");
        return 0;
    }

    // Check the total number of electrons
    if (pChemicalSystem->nmorb<1)
    {
        PRINT_ERROR("In saveDataInTheLoop: the total number of electrons ");
        fprintf(stderr,"(=%d) in the chemical system ",pChemicalSystem->nmorb);
        fprintf(stderr,"should be a positive integer.\n");
        return 0;
    }
    // Check the number of electrons
    if (pParameters->nu_electrons<1 ||
                               pParameters->nu_electrons>pChemicalSystem->nmorb)
    {
        PRINT_ERROR("In saveDataInTheLoop: the number ");
        fprintf(stderr,"nu=%d of electrons we are ",pParameters->nu_electrons);
        fprintf(stderr,"looking for should be a positive integer not ");
        fprintf(stderr,"(strictly) greater than the total number of ");
        fprintf(stderr,"electrons (=%d) in the ",pChemicalSystem->nmorb);
        fprintf(stderr,"chemical system.\n");
        return 0;
    }

    // Check if the *.data name ends with the ".data" extension
    // strlen returns the length of the string, but not including the char '\0'
    lengthName=strlen(pParameters->name_data);
    if (pParameters->name_data[lengthName-5]!='.' ||
                                    pParameters->name_data[lengthName-4]!='d' ||
                                    pParameters->name_data[lengthName-3]!='a' ||
                                    pParameters->name_data[lengthName-2]!='t' ||
                                    pParameters->name_data[lengthName-1]!='a' ||
                                       pParameters->name_data[lengthName]!='\0')
    {
        PRINT_ERROR("In saveDataInTheLoop: ");
        fprintf(stderr,"%s file name does not end ",pParameters->name_data);
        fprintf(stderr,"with the '.data' extension.\n");
        return 0;
    }

    // Update the tim and ctim variables of the structure pointed by pData
    time(pEndLocalTimer);
    pData->tim[iterationInTheLoop]=difftime(*pEndLocalTimer,*pStartLocalTimer);
    pData->ctim[iterationInTheLoop]=
                                 difftime(*pEndLocalTimer,*pGlobalInitialTimer);

    if (pParameters->save_data>0)
    {
        if (iterationInTheLoop%pParameters->save_data==0)
        {
            // Opening *.data file (append file if already exists otherwise
            // created) fopen returns a FILE pointer on success, otherwise NULL
            fprintf(stdout,"\nOpening %s file. ",pParameters->name_data);
            dataFile=fopen(pParameters->name_data,"a");
            if (dataFile==NULL)
            {
                PRINT_ERROR("In saveDataInTheLoop: could not open and append ");
                fprintf(stderr,"in %s file.\n",pParameters->name_data);
                return 0;
            }
            fprintf(stdout,"Appending data. ");

            if (!iterationInTheLoop)
            {
                fprintf(dataFile,"\n\n%sMaximizing the ",endTimerAtError());
                fprintf(dataFile,"probability to find ");
                fprintf(dataFile,"%d electrons ",pParameters->nu_electrons);
                fprintf(dataFile,"inside the domain and ");
                fprintf(dataFile,"%d",
                              pChemicalSystem->nmorb-pParameters->nu_electrons);
                fprintf(dataFile," outside.\n\nIter. Prob. Pop. Dprob. Res. ");
                if (pParameters->opt_mode>0)
                {
                     fprintf(dataFile,"Dres. ");
                }
                else
                {
                    fprintf(dataFile,"D2prob. ");
                }
                fprintf(dataFile,"Time (in min.) Cumulated time (in min.)\n\n");
            }

            fprintf(dataFile,"%d ",iterationInTheLoop);
            fprintf(dataFile,"%.8lf ",pData->pnu[iterationInTheLoop]);
            fprintf(dataFile,"%.8lf ",pData->pop[iterationInTheLoop]);
            fprintf(dataFile,"%.8le ",pData->d0p[iterationInTheLoop]);
            fprintf(dataFile,"%.8le ",pData->d1p[iterationInTheLoop]);
            fprintf(dataFile,"%.8le ",pData->d2p[iterationInTheLoop]);
            fprintf(dataFile,"%.2lf ",pData->tim[iterationInTheLoop]/60.);
            fprintf(dataFile,"%.2lf \n",pData->ctim[iterationInTheLoop]/60.);

            // Closing the *.data file: fclose returns zero if the input FILE*
            // variable is successfully closed, otherwise EOF (end-of-file)
            if (fclose(dataFile))
            {
                PRINT_ERROR("In saveDataInTheLoop: the ");
                fprintf(stderr,"%s file has not been ",pParameters->name_data);
                fprintf(stderr,"closed properly.\n");
                dataFile=NULL;
                return 0;
            }
            dataFile=NULL;
            fprintf(stdout,"Closing file.\n");
        }
    }
    else
    {
        if (!iterationInTheLoop)
        {
            fprintf(stdout,"\nOpening %s file. ",pParameters->name_data);
            dataFile=fopen(pParameters->name_data,"a");
            if (dataFile==NULL)
            {
                PRINT_ERROR("In saveDataInTheLoop: could not open and append ");
                fprintf(stderr,"in %s file.\n",pParameters->name_data);
                return 0;
            }
            fprintf(stdout,"Appending data. ");

            fprintf(dataFile,"\n\n%sMaximizing the ",endTimerAtError());
            fprintf(dataFile,"probability to find ");
            fprintf(dataFile,"%d electrons ",pParameters->nu_electrons);
            fprintf(dataFile,"inside the domain and ");
            fprintf(dataFile,"%d",
                              pChemicalSystem->nmorb-pParameters->nu_electrons);
            fprintf(dataFile," outside.\n\nIter. Prob. Pop. Dprob. Res. ");
            if (pParameters->opt_mode>0)
            {
                 fprintf(dataFile,"Dres. ");
            }
            else
            {
                fprintf(dataFile,"D2prob. ");
            }
            fprintf(dataFile,"Time (in sec.) Cumulated time (in sec.)\n\n");

            fprintf(dataFile,"%d ",iterationInTheLoop);
            fprintf(dataFile,"%.8lf ",pData->pnu[iterationInTheLoop]);
            fprintf(dataFile,"%.8lf ",pData->pop[iterationInTheLoop]);
            fprintf(dataFile,"%.8le ",pData->d0p[iterationInTheLoop]);
            fprintf(dataFile,"%.8le ",pData->d1p[iterationInTheLoop]);
            fprintf(dataFile,"%.8le ",pData->d2p[iterationInTheLoop]);
            fprintf(dataFile,"%d ",(int)pData->tim[iterationInTheLoop]);
            fprintf(dataFile,"%d \n",(int)pData->ctim[iterationInTheLoop]);

            if (fclose(dataFile))
            {
                PRINT_ERROR("In saveDataInTheLoop: the ");
                fprintf(stderr,"%s file has not been ",pParameters->name_data);
                fprintf(stderr,"closed properly.\n");
                dataFile=NULL;
                return 0;
            }
            dataFile=NULL;
            fprintf(stdout,"Closing file.\n");
        }
    }

    // At the end of the optimization loop, we keep track of iterationInTheLoop
    // thanks to the pData->niter
    pData->niter++;

    return 1;
}


/* ************************************************************************** */
// The function setupInitialData successively allocates memory for the structure
// pointed by pData i.e. for the saving of data, enlarge of 10 letters the
// maximal length allowed for path names, reallocates the memory accordingly
// in the structure pointed by pParameters, write an *.input file recapitulating
// all the parameters used (default ones and those prescribed by the user in
// the *.info file, the name being similar but where the '.info' extension is
// replaced by the '.input' one), set up the number of threads/cpu for the
// parallel calculations (pParameters->n_cpu variable), compute all the
// quantities (probability, ...), save them in the *.data file given by
// the pParameters->name_data variable, and save also the mesh in a *.0.mesh
// file. It allows the user to always keep track of all the parameters with the
// *.info file, always get the initial mesh saved before the optimization loop, 
// and terminates the setting up of the initialization process of the MPD
// algorithm. It has the Parameters*, Mesh*, Data*, ChemicalSystem* structures
// (both defined in main.h) as input arguments, and also the three time_t*
// variables (pGlobalInitialTimer, pStartLocalTimer, pEndLocalTimer) that are
// used to compute the execution time of the initialization and the one needed
// for both loading the data, and initialize them. The function returns one on
// success, otherwise zero is returned if an error is encountered in the process
/* ************************************************************************** */
int setupInitialData(Parameters* pParameters, Mesh* pMesh, Data* pData,
                     ChemicalSystem* pChemicalSystem,
                     time_t* pGlobalInitialTimer, time_t* pStartLocalTimer,
                                                         time_t* pEndLocalTimer)
{
    // Check the input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                           pChemicalSystem==NULL || pGlobalInitialTimer==NULL ||
                                 pStartLocalTimer==NULL || pEndLocalTimer==NULL)
    {
        PRINT_ERROR("In setupInitialData: one of the input pointers ");
        fprintf(stderr,"pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pData=%p, pMesh=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"pChemicalSystem=%p, ",(void*)pChemicalSystem);
        fprintf(stderr,"pGlobalInitialTimer=%p, ",(void*)pGlobalInitialTimer);
        fprintf(stderr,"pStartLocalTimer=%p or ",(void*)pStartLocalTimer);
        fprintf(stderr,"pEndLocalTimer=%p, ",(void*)pEndLocalTimer);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Allocate memory for the saving of data
    if (!allocateMemoryForData(pParameters,pData,pChemicalSystem))
    {
        PRINT_ERROR("In setupInitialData: allocateMemoryForData function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Check we can add 10 letters to the file names for the saving of meshes
    if (!addLengthForFileName(pParameters,10))
    {
        PRINT_ERROR("In setupInitialData: addLengthForFileName function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Write the *.input file before starting the optimization loop
    if (!writingRestartFile(pParameters))
    {
        PRINT_ERROR("In setupInitialData: writingRestartFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Set the number of threads for the parallelization
    if (pParameters->n_cpu<1)
    {
        PRINT_ERROR("In setupInitialData: the number of threads ");
        fprintf(stderr,"(=%d) for the parallelization ",pParameters->n_cpu);
        fprintf(stderr,"should be a positive integer.\n");
        return 0;
    }
    omp_set_num_threads(pParameters->n_cpu);

    // Compute the overlap matrix, probability, shape gradient, and residual
    // for the initial domain
    if (!shapeDerivative(pParameters,pMesh,pData,pChemicalSystem,0))
    {
        PRINT_ERROR("In setupInitialData: shapeDerivative function returned ");
        fprintf(stderr,"zero instead of one.\n");
        return 0;
    }

    // Save initial data and mesh
    if (!saveDataInTheLoop(pParameters,pMesh,pData,pChemicalSystem,0,
                           pGlobalInitialTimer,pStartLocalTimer,pEndLocalTimer))
    {
        PRINT_ERROR("In setupInitialData: saveDataInTheLoop function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    return 1;
}

/* ************************************************************************** */
// The function exhaustiveSearchAlgorithm computes the pParameters->opt_mode=-2
// of the optimization function. It tries to add or remove the hexahedra that
// are touching the boundary of the domain. Only those increasing the
// probability are retained. It has the Parameters*, Mesh*, Data*,
// ChemicalSystem* (both defined in main.h) and the int iterationInTheLoop
// variables as input arguments. It returns one on success otherwise zero
/* ************************************************************************** */
int exhaustiveSearchAlgorithm(Parameters* pParameters, Mesh* pMesh,
                               Data* pData, ChemicalSystem* pChemicalSystem,
                                                         int iterationInTheLoop)
{
    int i=0, j=0, k=0, kHexahedron=0, l=0, lI=0, lJ=0, nHex=0, nAdj=0, nMorb=0;
    int nGauss=0, sizeMemory=0, lPrint=0;
    double integral=0., *pCoefficient=NULL, *pProbabilityOld=NULL;
    double *pProbability=NULL, probability=0.;
    MolecularOrbital *pMolecularOrbitalI=NULL, *pMolecularOrbitalJ=NULL;
    OverlapMatrix *pOverlapMatrix=NULL, *pOverlapMatrixOld=NULL;

    // Check input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                                                          pChemicalSystem==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: at least one of the ");
        fprintf(stderr,"input variables pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, pData=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"or pChemicalSystem=%p ",(void*)pChemicalSystem);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check the iterationInTheLoopVariable variable
    if (iterationInTheLoop<1 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the input ");
        fprintf(stderr,"iterationInTheLoop variable (=%d) ",iterationInTheLoop);
        fprintf(stderr,"should be a positive integer not (strictly) ");
        fprintf(stderr,"greater than the maximal number of allowed ");
        fprintf(stderr,"iterations (=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Check that we are dealing with the right optimization mode
    if (pParameters->opt_mode!=-2)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the function can only be ");
        fprintf(stderr,"used when the optimization mode ");
        fprintf(stderr,"(=%d) is set to minus two.\n",pParameters->opt_mode);
        return 0;
    }

    // Check the number of hexahedra and its associated pointer
    nHex=pMesh->nhex;
    if (nHex<1)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the total number of ");
        fprintf(stderr,"hexahedra (=%d) in the structure pointed ",nHex);
        fprintf(stderr,"by pMesh should be a positive integer.\n");
        return 0;
    }

    if (pMesh->phex==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the structure pointed by ");
        fprintf(stderr,"pMesh does not store any hexahedra ");
        fprintf(stderr,"(pMesh->phex=%p).\n",(void*)pMesh->phex);
        return 0;
    }

    // Check the number of boundary quadrilaterals and its associated pointer
    nAdj=pMesh->nadj;
    if (nAdj<1)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the total number of ");
        fprintf(stderr,"Adjacency structures (=%d) in the structure ",nAdj);
        fprintf(stderr,"pointed by pMesh should be a positive integer.\n");
        return 0;
    }

    if (pMesh->padj==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the structure pointed by ");
        fprintf(stderr,"pMesh does not store any Adjacency structures ");
        fprintf(stderr,"(pMesh->padj=%p).\n",(void*)pMesh->padj);
        return 0;
    }

    // Check the chemical parameters
    nMorb=pChemicalSystem->nmorb;
    if (nMorb<1)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the total number of ");
        fprintf(stderr,"molecular orbitals (=%d) should be a positive ",nMorb);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    if (pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the pmorb variable of ");
        fprintf(stderr,"the structure pointed by pChemicalSystem is pointing ");
        fprintf(stderr,"to the %p address.\n",(void*)pChemicalSystem->pmorb);
        return 0;
    }

    nGauss=pChemicalSystem->ngauss;
    if (nGauss<1)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the total number of ");
        fprintf(stderr,"Gaussian-type primitives (=%d) should be a ",nGauss);
        fprintf(stderr,"positive integer.\n");
        return 0;
    }

    if (pParameters->orb_rhf)
    {
        if (nMorb%2!=0)
        {
            PRINT_ERROR("In exhaustiveSearchAlgorithm: since we are dealing ");
            fprintf(stderr,"with a restricted Hartree-Fock system ");
            fprintf(stderr,"(pParameters->orb_rhf=%d) ",pParameters->orb_rhf);
            fprintf(stderr,"the total number of molecular orbitals ");
            fprintf(stderr," (=%d) should be an even number.\n",nMorb);
            return 0;
        }
    }

    // Check the size of the overlap matrix and its associated pointer
    if (pData->nmat!=nMorb)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the size of the ");
        fprintf(stderr,"overlap matrix should be (%d)x(%d) ",nMorb,nMorb);
        fprintf(stderr,"instead of (%d)x(%d).\n",pData->nmat,pData->nmat);
        return 0;
    }

    if (pData->pmat==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the pmat variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pmat);
        return 0;
    }
    pOverlapMatrix=&pData->pmat[iterationInTheLoop];
    pOverlapMatrixOld=&pData->pmat[iterationInTheLoop-1];

    // Check the coef pointer variable of pOverlapMatrix and pOverlapMatrixOld
    if (pOverlapMatrix->coef==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the coef variable of the ");
        fprintf(stderr,"%d-th OverlapMatrix structure ",iterationInTheLoop+1);
        fprintf(stderr,"is pointing to the %p ",(void*)pOverlapMatrix->coef);
        fprintf(stderr,"address.\n");
        return 0;
    }

    if (pOverlapMatrixOld->coef==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the coef variable of the ");
        fprintf(stderr,"%d-th OverlapMatrix structure ",iterationInTheLoop);
        fprintf(stderr,"is pointing to the %p ",(void*)pOverlapMatrixOld->coef);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the population pointer
    if (pData->pop==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the pop variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pop);
        return 0;
    }

    // Allocate memory for pCoefficient
    // calloc returns a pointer to the allocated memory, otherwise NULL
    pCoefficient=(double*)calloc(nMorb*nMorb,sizeof(double));
    if (pCoefficient==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: could not allocate memory ");
        fprintf(stderr,"for the local double* pCoefficient variable.\n");
        return 0;
    }

    // Initializing pCoefficient
    for (i=0; i<nMorb; i++)
    {
        for (j=0; j<nMorb; j++)
        {
            pCoefficient[i*nMorb+j]=pOverlapMatrixOld->coef[i*nMorb+j];
            pOverlapMatrix->coef[i*nMorb+j]=0.;
        }
    }

    // Set the probability pointers
    if (pData->pnu==NULL)
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: the pnu variable of the ");
        fprintf(stderr,"structure pointed by pData is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pData->pnu);
        free(pCoefficient);
        pCoefficient=NULL;
        return 0;
    }
    pProbabilityOld=&pData->pnu[iterationInTheLoop-1];
    pProbability=&pData->pnu[iterationInTheLoop];
    probability=*pProbabilityOld;


    fprintf(stdout,"\nModifying exhaustively the internal domain cube by ");
    fprintf(stdout,"cube.\n");
    if (!pParameters->verbose)
    {
        lPrint=1+nAdj/6;
    }

    for (k=0; k<nAdj; k++)
    {
        if (!pParameters->verbose)
        {
            if ((k+1)%lPrint==0)
            {
                fprintf(stdout,"%d %% done.\n",
                                               (int)(100.*(k+1.)/(double)nAdj));
            }
        }

        // Try to add kHexahedron to mesh (if not already treated)
        kHexahedron=pMesh->padj[k].hexout-1;

        if (pMesh->phex[kHexahedron].label!=-3)
        {
            // Evaluating S(Omega+kHexahedron)
            for (i=0; i<nMorb; i++)
            {
                pMolecularOrbitalI=&pChemicalSystem->pmorb[i];

                // Avoid calculation for restricted Hartree-Fock system
                if (pParameters->orb_rhf)
                {
                    if (i>=nMorb/2)
                    {
                        continue;
                    }
                }

                for (j=0; j<=i; j++)
                {
                    pMolecularOrbitalJ=&pChemicalSystem->pmorb[j];

                    // Compute the three-dimensional integral analytically
                    integral=0.;
                    for (lI=0; lI<nGauss; lI++)
                    {
                        for (lJ=0; lJ<nGauss; lJ++)
                        {
                            integral+=evaluateTripleIntegralIJ(pParameters,
                                                             pMesh,
                                                             pChemicalSystem,
                                                             pMolecularOrbitalI,
                                                             pMolecularOrbitalJ,
                                                             lI,lJ,kHexahedron);
                        }
                    }
                    pOverlapMatrix->coef[i*nMorb+j]=pCoefficient[i*nMorb+j]
                                                                      +integral;
                }
            }

            // Complete using the symmetry of the matrix
            for (i=0; i<nMorb; i++)
            {
                for (j=i+1; j<nMorb; j++)
                {
                    pOverlapMatrix->coef[i*nMorb+j]=
                                                pOverlapMatrix->coef[j*nMorb+i];
                }
            }

            // Complete matrix in the case of a restricted Hartree-Fock system
            if (pParameters->orb_rhf)
            {
                l=nMorb/2;
                for (i=0; i<l; i++)
                {
                    for (j=0; j<l; j++)
                    {
                        pOverlapMatrix->coef[(i+l)*nMorb+(j+l)]=
                                                pOverlapMatrix->coef[i*nMorb+j];
                    }
                }
            }

            // Diagonalize the overlap matrix. Warning here: we use lapacke.h,
            // and lapacke package must have been previously installed
            if (!diagonalizeOverlapMatrix(pParameters,pData,iterationInTheLoop))
            {
                PRINT_ERROR("In exhaustiveSearchAlgorithm: ");
                fprintf(stderr,"diagonalizeOverlapMatrix function returned ");
                fprintf(stderr,"zero instead of one.\n");
                free(pCoefficient);
                pCoefficient=NULL;
                return 0;
            }

            // Compute the probabilities and total population inside the domain
            if (!computeProbability(pParameters,pData,iterationInTheLoop))
            {
                PRINT_ERROR("In exhaustiveSearchAlgorithm: ");
                fprintf(stderr,"computeProbability function returned zero ");
                fprintf(stderr,"instead of one.\n");
                free(pCoefficient);
                pCoefficient=NULL;
                return 0;
            }

            if (*pProbability>probability)
            {
                if (pParameters->verbose)
                {
                    fprintf(stdout,"Adding the %d-th ",kHexahedron+1);
                    fprintf(stdout,"hexahedron to the domain: ");
                    fprintf(stdout,"P(Omega+Hexa)=%.8lf\n",*pProbability);
                }
                pMesh->phex[kHexahedron].label=-3;

                // Update pCoefficient and probability
                probability=*pProbability;
                for (i=0; i<nMorb; i++)
                {
                    for (j=0; j<nMorb; j++)
                    {
                        pCoefficient[i*nMorb+j]=pOverlapMatrix->coef[i*nMorb+j];
                    }
                }
            }
        }

        // Try to remove kHexahedron to mesh (if not already treated)
        kHexahedron=pMesh->padj[k].hexin-1;

        if (pMesh->phex[kHexahedron].label!=-2)
        {
            // Evaluating S(Omega-kHexahedron)
            for (i=0; i<nMorb; i++)
            {
                pMolecularOrbitalI=&pChemicalSystem->pmorb[i];

                // Avoid calculation for restricted Hartree-Fock system
                if (pParameters->orb_rhf)
                {
                    if (i>=nMorb/2)
                    {
                        continue;
                    }
                }

                for (j=0; j<=i; j++)
                {
                    pMolecularOrbitalJ=&pChemicalSystem->pmorb[j];

                    // Compute the 3-dimensional integral analytically
                    integral=0.;
                    for (lI=0; lI<nGauss; lI++)
                    {
                        for (lJ=0; lJ<nGauss; lJ++)
                        {
                            integral+=evaluateTripleIntegralIJ(pParameters,
                                                             pMesh,
                                                             pChemicalSystem,
                                                             pMolecularOrbitalI,
                                                             pMolecularOrbitalJ,
                                                             lI,lJ,kHexahedron);
                        }
                    }
                    pOverlapMatrix->coef[i*nMorb+j]=
                                               pCoefficient[i*nMorb+j]-integral;
                }
            }

            // Complete using the symmetry of the matrix
            for (i=0; i<nMorb; i++)
            {
                for (j=i+1; j<nMorb; j++)
                {
                    pOverlapMatrix->coef[i*nMorb+j]=
                                                pOverlapMatrix->coef[j*nMorb+i];
                }
            }

            // Complete matrix for restricted Hartree-Fock system
            if (pParameters->orb_rhf)
            {
                l=nMorb/2;
                for (i=0; i<l; i++)
                {
                    for (j=0; j<l; j++)
                    {
                        pOverlapMatrix->coef[(i+l)*nMorb+(j+l)]=
                                                pOverlapMatrix->coef[i*nMorb+j];
                    }
                }
            }

            // Diagonalize overlap matrix. Warning: we use lapacke.h,
            // and lapacke package must have been previously installed
            if (!diagonalizeOverlapMatrix(pParameters,pData,iterationInTheLoop))
            {
                PRINT_ERROR("In exhaustiveSearchAlgorithm: ");
                fprintf(stderr,"diagonalizeOverlapMatrix function ");
                fprintf(stderr,"returned zero instead of one.\n");
                free(pCoefficient);
                pCoefficient=NULL;
                return 0;
            }

            // Compute probabilities and total population in the domain 
            if (!computeProbability(pParameters,pData,iterationInTheLoop))
            {
                PRINT_ERROR("In exhaustiveSearchAlgorithm: ");
                fprintf(stderr,"computeProbability function ");
                fprintf(stderr,"returned zero instead of one.\n");
                free(pCoefficient);
                pCoefficient=NULL;
                return 0;
            }

            if (*pProbability>probability)
            {
                if (pParameters->verbose)
                {
                    fprintf(stdout,"Removing the %d-th ",kHexahedron+1);
                    fprintf(stdout,"hexahedron of the domain: ");
                    fprintf(stdout,"P(Omega-Hexa)=%.8lf\n",*pProbability);
                }
                pMesh->phex[kHexahedron].label=-2;

                // Update pCoefficient and probability
                probability=*pProbability;
                for (i=0; i<nMorb; i++)
                {
                    for (j=0; j<nMorb; j++)
                    {
                        pCoefficient[i*nMorb+j]=pOverlapMatrix->coef[i*nMorb+j];
                    }
                }
            }
        }
    }

    // Update the final overlap matrix coefficients
    for (i=0; i<nMorb; i++)
    {
        for (j=0; j<nMorb; j++)
        {
           pOverlapMatrix->coef[i*nMorb+j]=pCoefficient[i*nMorb+j];
        }
    }

    if (!pParameters->verbose)
    {
        fprintf(stdout,"100 %% done.\n");
    }

    // Diagonalize overlap matrix. Warning: we use lapacke.h,
    // and lapacke package must have been previously installed
    pParameters->opt_mode=-1;
    if (!diagonalizeOverlapMatrix(pParameters,pData,iterationInTheLoop))
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: diagonalizeOverlapMatrix ");
        fprintf(stderr,"function returned zero instead of one.\n");
        free(pCoefficient);
        pCoefficient=NULL;
        return 0;
    }

    // Compute probabilities and total population in the domain 
    if (!computeProbability(pParameters,pData,iterationInTheLoop))
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: computeProbability ");
        fprintf(stderr,"function returned zero instead of one.\n");
        free(pCoefficient);
        pCoefficient=NULL;
        return 0;
    }
    pParameters->opt_mode=-2;

    // Free the memory allocated for pCoefficient
    free(pCoefficient);
    pCoefficient=NULL;

    // Act the fact that the new domain can now be considered
    for (i=0; i<nHex; i++)
    {
        pMesh->phex[i].label=abs(pMesh->phex[i].label);
    }

    // Free the memory allocated for Adjacency table
    sizeMemory=sizeof(int)+(pMesh->nadj)*sizeof(Adjacency);
    fprintf(stdout,"\nCleaning allocated memory ");
    fprintf(stdout,"(%d,",sizeMemory/1000000);
    fprintf(stdout,"%d Mo) for the ",sizeMemory%1000000);
    fprintf(stdout,"Adjacency structures. ");
    free(pMesh->padj);
    pMesh->padj=NULL;
    pMesh->nadj=0;

    // Rebuild the Adjacency structures and boundary quandrilaterals
    fprintf(stdout,"\nBuild the new Adjacency structures to get the ");
    fprintf(stdout,"updated domain.\n");
    if (!getLevelSetQuadrilaterals(pParameters,pMesh))
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: getLevelSetQuadrilaterals ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

    if (!computeShapeResidual(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
    {
        PRINT_ERROR("In exhaustiveSearchAlgorithm: computeShapeResidual ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

    return 1;
}

/* ************************************************************************** */
// The function computeLevelSet computes the signed distance function i.e. the
// level-set function with unitary gradient norm of the internal domain of pMesh
// and saves the values in an *.chi.sol file associated with an *.mesh one. It
// has the Parameters* and Mesh* variables (both defined in main.h) as input
// arguments and it returns zero if an error occurred, otherwise one is returned
// on success
/* ************************************************************************** */
int computeLevelSet(Parameters* pParameters, Mesh* pMesh,
                                                         int iterationInTheLoop)
{
    size_t lengthName=0;
    char* fileLocation=NULL;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In computeLevelSet: at least one of the input variable ");
        fprintf(stderr,"pParameters=%p or",(void*)pParameters);
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the pParameters->opt_mode variable
    if (pParameters->opt_mode!=1 && pParameters->opt_mode!=2)
    {
        PRINT_ERROR("In computeLevelSet: the pParameters->opt_mode ");
        fprintf(stderr,"variable (=%d) can only be set ",pParameters->opt_mode);
        fprintf(stderr,"to one or two here.\n");
        return 0;
    }

    // Check the iterationInTheLoopVariable variable
    if (iterationInTheLoop<1 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In computeLevelSet: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"positive integer not (strictly) greater than the ");
        fprintf(stderr,"maximal number of allowed iterations ");
        fprintf(stderr,"(=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Check the name_mesh and name_length variables
    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In computeLevelSet: getMeshFormat function did not ");
        fprintf(stderr,"return one, which was the expected value here.\n");
        return 0;
    }

    // Store the name of the *.chi.mesh file in fileLocation
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In computeLevelSet: could not allocate memory for the ");
        fprintf(stderr,"local char* fileLocation variable.\n");
        return 0;
    }
    strncpy(fileLocation,pParameters->name_mesh,lengthName);
    lengthName=strlen(fileLocation);
    fileLocation[lengthName-5]='.';
    fileLocation[lengthName-4]='c';
    fileLocation[lengthName-3]='h';
    fileLocation[lengthName-2]='i';
    fileLocation[lengthName-1]='.';
    fileLocation[lengthName]='m';
    fileLocation[lengthName+1]='e';
    fileLocation[lengthName+2]='s';
    fileLocation[lengthName+3]='h';
    fileLocation[lengthName+4]='\0';

    // Remove the *.chi.mesh file if it already exists.
    switch (initialFileExists(fileLocation,pParameters->name_length))
    {
        case -1:
            break;

        case 1:
            // remove returns 0 on success, otherwise -1
            if (remove(fileLocation))
            {
                PRINT_ERROR("In computeLevelSet: wrong return (=-1) of the ");
                fprintf(stderr,"standard remove c-function in the attempt of ");
                fprintf(stderr,"removing the %s file.\n",fileLocation);
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }
            break;

        default:
            PRINT_ERROR("In computeLevelSet: initialFileExists function ");
            fprintf(stderr,"returned zero instead of (+/-)one.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
            break;
    }

    // Temporary rename the *.mesh by *.chi.mesh
    if (!renameFileLocation(pParameters->name_mesh,pParameters->name_length,
                                                                  fileLocation))
    {
        PRINT_ERROR("In computeLevelSet: renameFileLocation function ");
        fprintf(stderr,"returned zero instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    strncpy(pParameters->name_mesh,fileLocation,pParameters->name_length);

    // Generate the level-set function from mesh with mshdist
    if (!renormalizeWithMshdistSoftware(pParameters,"dom"))
    {
        PRINT_ERROR("In computeLevelSet: renormalizeWithMshdistSoftware ");
        fprintf(stderr,"function returned zero instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }

    // Vizualize the shape gradient after renormalization
    if (pParameters->opt_mode==2)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==2)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In computeLevelSet: ");
                    fprintf(stderr,"plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }
        }
    }

    // Rename the *.mesh file as it was
    lengthName=strlen(fileLocation);
    fileLocation[lengthName-9]='.';
    fileLocation[lengthName-8]='m';
    fileLocation[lengthName-7]='e';
    fileLocation[lengthName-6]='s';
    fileLocation[lengthName-5]='h';
    fileLocation[lengthName-4]='\0';
    fileLocation[lengthName-3]='\0';
    fileLocation[lengthName-2]='\0';
    fileLocation[lengthName-1]='\0';
    fileLocation[lengthName]='\0';
    if (!renameFileLocation(pParameters->name_mesh,pParameters->name_length,
                                                                  fileLocation))
    {
        PRINT_ERROR("In computeLevelSet: renameFileLocation function ");
        fprintf(stderr,"returned zero instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    strncpy(pParameters->name_mesh,fileLocation,pParameters->name_length);

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

/* ************************************************************************** */
// The function saveTheShapeGradient contains successive functions that are
// used to vizualize the norm of the shape gradient, save it in the vectorial
// *.sol file, and vizualize it with the medit software. It has the
// Parameters*, Mesh*, (both defined in main.h), and the int
// interationInTheLoop variables as input arguments. It returns one on success
// otherwise zero if an error is encoutnered during the process
/* ************************************************************************** */
int saveTheShapeGradient(Parameters* pParameters, Mesh* pMesh,
                                                         int iterationInTheLoop)
{
    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In saveTheShapeGradient: at least one of the input ");
        fprintf(stderr,"variable pParameters=%p or",(void*)pParameters);
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the iterationInTheLoopVariable variable
    if (iterationInTheLoop<1 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In saveTheShapeGradient: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"positive integer not (strictly) greater than the ");
        fprintf(stderr,"maximal number of allowed iterations ");
        fprintf(stderr,"(=%d).\n",pParameters->iter_max);
        return 0;
    }

    if (pParameters->opt_mode==3)
    {
            fprintf(stdout,"\nSTEP 1: DEFORM THE MESH ACCORDING TO THE ");
            fprintf(stdout,"COMPUTED SHAPE GRADIENT MOTION.\n");
    }
    else if (pParameters->opt_mode==2)
    {
            fprintf(stdout,"\nSTEP 1: EXTEND THE COMPUTED SHAPE GRADIENT ");
            fprintf(stdout,"OUTSIDE THE BOUNDARY OF THE DOMAIN.\n");
    }

    // Vizualize the norm of the shape gradient at the boundary vertices
    if (pParameters->opt_mode!=1)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==3)
            {
                if (!writingSolFile(pParameters,pMesh))
                {
                    PRINT_ERROR("In saveTheShapeGradient: writingSolFile ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }

                if (!plotMeshWithMeditSoftware(pParameters))
                {
                     PRINT_ERROR("In saveTheShapeGradient: ");
                     fprintf(stderr,"plotMeshWithMeditSoftware function ");
                     fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }
    }

    // Save the vectorial shape gradient at the shape vertices
    if (!writingShapeSolFile(pParameters,pMesh))
    {
        PRINT_ERROR("In saveTheShapeGradient: writingShapeSolFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Vizualize the vectorial shape gradient at the shape vertices
    if (pParameters->opt_mode!=1)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==4)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In saveTheShapeGradient: ");
                    fprintf(stderr,"plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function computeEulerianMode contains successive functions that are
// used to specifically compute the Eulerian mode in the optimization function
// It has the Parameters*, Mesh*, (both defined in main.h), and the int
// interationInTheLoop variables as input arguments. It returns one on success
// otherwise zero if an error is encoutnered during the process
/* ************************************************************************** */
int computeEulerianMode(Parameters* pParameters, Mesh* pMesh,
                                                         int iterationInTheLoop)
{
    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In computeEulerianMode: at least one of the input ");
        fprintf(stderr,"variable pParameters=%p or",(void*)pParameters);
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the pParameters->opt_mode variable
    if (pParameters->opt_mode!=1 && pParameters->opt_mode!=2)
    {
        PRINT_ERROR("In computeEulerianMode: the pParameters->opt_mode ");
        fprintf(stderr,"variable (=%d) can only be set ",pParameters->opt_mode);
        fprintf(stderr,"to one or two here.\n");
        return 0;
    }

    // Check the iterationInTheLoopVariable variable
    if (iterationInTheLoop<1 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In computeEulerianMode: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"positive integer not (strictly) greater than the ");
        fprintf(stderr,"maximal number of allowed iterations ");
        fprintf(stderr,"(=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Extend the shape gradient outside the domain thanks to the elasticity
    if (!extendShapeGradientWithElasticSoftware(pParameters))
    {
        PRINT_ERROR("In computeEulerianMode: ");
        fprintf(stderr,"extendLevelSetWithElasticSoftware function returned ");
        fprintf(stderr,"zero instead of one.\n");
        return 0;
    }

    // Vizualize the extension of the vectorial shape gradient
    if (pParameters->opt_mode==2)
    {
       if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==5)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In computeEulerianMode: ");
                    fprintf(stderr,"plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }

        fprintf(stdout,"\nSTEP 2: GET THE LEVEL-SET FUNCTION OF ");
        fprintf(stdout,"THE DOMAIN WITH A UNITARY GRADIENT NORM.\n");
    }

    // Generate the level-set function from mesh with mshdist
    if (!computeLevelSet(pParameters,pMesh,iterationInTheLoop))
    {
        PRINT_ERROR("In computeEulerianMode: computeLevelSet function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    if (pParameters->opt_mode==2)
    {
        fprintf(stdout,"\nSTEP 3: ADVECT THE LEVEL-SET FUNCTION ");
        fprintf(stdout,"ACCORDING TO THE EXTENDED SHAPE GRADIENT.\n");
    }
         
    // Advect the level-set function according to the parameters
    if (!advectLevelSetWithAdvectSoftware(pParameters))
    {
        PRINT_ERROR("In computeEulerianMode:  ");
        fprintf(stderr,"advectLevelSetWithAdvectSoftware function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Vizualize the advected level-set function on the mesh
    if (pParameters->opt_mode==2)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==6)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In computeEulerianMode: ");
                    fprintf(stderr,"plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function computeLagrangianMode contains successive functions that are
// used to specifically compute the Lagrangian mode in the optimization function
// It has the Parameters*, Mesh*, (both defined in main.h), and the int
// interationInTheLoop variables as input arguments. It returns one on success
// otherwise zero if an error is encoutnered during the process
/* ************************************************************************** */
int computeLagrangianMode(Parameters* pParameters, Mesh* pMesh,
                                                         int iterationInTheLoop)
{
    int sizeMemory=0, nIter=0;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In computeLagrangianMode: at least one of the input ");
        fprintf(stderr,"variable pParameters=%p or",(void*)pParameters);
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the pParameters->opt_mode variable
    if (pParameters->opt_mode!=1 && pParameters->opt_mode!=3)
    {
        PRINT_ERROR("In computeLagrangianMode: the pParameters->opt_mode ");
        fprintf(stderr,"variable (=%d) can only be set ",pParameters->opt_mode);
        fprintf(stderr,"to one or three here.\n");
        return 0;
    }

    // Check the iterationInTheLoopVariable variable
    if (iterationInTheLoop<1 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In computeLagrangianMode: the input iterationInTheLoop ");
        fprintf(stderr,"variable (=%d) should be a ",iterationInTheLoop);
        fprintf(stderr,"positive integer not (strictly) greater than the ");
        fprintf(stderr,"maximal number of allowed iterations ");
        fprintf(stderr,"(=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Free the memory allocated for the mesh
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        sizeMemory=sizeof(Mesh)+(pMesh->nver)*sizeof(Point);
        sizeMemory+=(pMesh->nnorm+pMesh->ntan)*sizeof(Vector);
        sizeMemory+=(pMesh->nedg)*sizeof(Edge);
        sizeMemory+=(pMesh->ntri)*sizeof(Triangle);
        sizeMemory+=(pMesh->ntet)*sizeof(Tetrahedron);
        fprintf(stdout,"\nCleaning the allocated memory ");
        fprintf(stdout,"(%d,%d Mo) for ",sizeMemory/1000000,sizeMemory%1000000);
        fprintf(stdout,"a mesh adaptation according to the shape gradient ");
        fprintf(stdout,"with a Lagrangian approach.\n");
    }
    freeMeshMemory(pMesh);

    // Warning: mmg3d software must have been previously installed
    if (!adaptMeshWithMmg3dSoftware(pParameters,"lag"))
    {
        PRINT_ERROR("In computeLagrangianMode: adaptMeshWithMmg3dSoftware ");
        fprintf(stderr,"function returned zero instead of one.\n");;
        return 0;
    }

    // Read the intermediate *.mesh file
    if (!readMeshFileAndAllocateMesh(pParameters,pMesh))
    {
        PRINT_ERROR("In computeLagrangianMode: readMeshFileAndAllocateMesh ");
        fprintf(stderr,"function returned zero instead of one.\n");
        return 0;
    }

    // Update the parameters related to the computational box
    if (!updateDiscretizationParameters(pParameters,pMesh))
    {
        PRINT_ERROR("In computeLagrangianMode: ");
        fprintf(stderr,"updateDiscretizationParameters function returned ");
        fprintf(stderr,"zero instead of one.\n");
        return 0;
    }

    // Vizualize the intermediate mesh
    if (pParameters->opt_mode==3)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==7)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In computeLagrangianMode: ");
                    fprintf(stderr,"plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }
    }

    if (pParameters->opt_mode==3)
    {
        fprintf(stdout,"\nSTEP 2: ADAPT THE MESH TO BOTH THE NEW DOMAIN ");
        fprintf(stdout,"GEOMETRY AND SYSTEM CHEMISTRY.\n");
    }

    // Generate a coarse level-set function of the intermediate mesh
    nIter=pParameters->n_iter;
    pParameters->n_iter=10;
    if (!renormalizeWithMshdistSoftware(pParameters,"dom"))
    {
        PRINT_ERROR("In computeLagrangianMode: ");
        fprintf(stderr,"renormalizeWithMshdistSoftware function returned ");
        fprintf(stderr,"zero instead of one.\n");
        return 0;
    }
    pParameters->n_iter=nIter;

    // Vizualize the coarse level-set function of the intermediate mesh
    if (pParameters->opt_mode==3)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==2)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In computeLagrangianMode: ");
                    fprintf(stderr,"plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function computeProbabilityAndReloadPreviousMesh compute the probability
// of a given mesh then reload the previous mesh in the structure pointed by
// pMesh. It has the Parameters*, Mesh*, Data*, ChemicalSystem* (both defined in
// main.h), and the int interationInTheLoop variables as input arguments. It
// returns the value of the probability on success otherwise -10000 for an error
/* ************************************************************************** */
double computeProbabilityAndReloadPreviousMesh(Parameters* pParameters, 
                                               Mesh* pMesh, Data* pData,
                                               ChemicalSystem* pChemicalSystem,
                                                         int iterationInTheLoop)
{
    size_t lengthName=0;
    char *fileLocation=NULL;
    int sizeMemory=0;
    double returnValue=0.;

    // Computing overlap matrix. Warning: bounadary triangles are labbelled 10
    // but not necessarily the related vertices (ok now and checked in
    // writeShapeSolFile function)
    if (!computeOverlapMatrix(pParameters,pMesh,pData,pChemicalSystem,2,
                                                            iterationInTheLoop))
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
        fprintf(stderr,"computeOverlapMatrix function returned zero instead ");
        fprintf(stderr,"of one.\n");
        return -10000.;
    }

    // Diagonalize the overlap matrix. Warning: we use lapacke.h
    // and lapacke package must have been previously installed
    if (!diagonalizeOverlapMatrix(pParameters,pData,iterationInTheLoop))
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
        fprintf(stderr,"diagonalizeOverlapMatrix function returned zero ");
        fprintf(stderr,"instead of one.\n");
        return -10000.;
    }

    // Compute the probabilities and total population in the domain
    if (!computeProbability(pParameters,pData,iterationInTheLoop))
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
        fprintf(stderr,"computeProbability function returned zero instead ");
        fprintf(stderr,"of one.\n");
        return -10000.;
    }
    returnValue=pData->pnu[iterationInTheLoop];

    // Store the name of the *.mesh file in fileLocation
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: could not ");
        fprintf(stderr,"allocate memory for the local char* fileLocation ");
        fprintf(stderr,"variable.\n");
        return -10000.;
    }

    // Store the *.[iterationInTheLoop-1].mesh name in fileLocation
    strncpy(fileLocation,pParameters->name_mesh,lengthName);
    lengthName=strlen(fileLocation);
    fileLocation[lengthName-5]='\0';

    // sprintf returns the total number of characters written
    // not including the '\0', otherwise a negative number on failure
    if (sprintf(pParameters->name_mesh,"%s.%d.mesh",fileLocation,
                                                        iterationInTheLoop-1)<0)
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: the ");
        fprintf(stderr,"standard sprintf c-function returned an unexpected ");
        fprintf(stderr,"negative value.\n");
        free(fileLocation);
        fileLocation=NULL;
        return -10000.;
    }
    fileLocation[lengthName-5]='.';

    // Check if *.[iterationInTheLoop-1].mesh file exists.
    if (initialFileExists(pParameters->name_mesh,pParameters->name_length)!=1)
    {
            PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
            fprintf(stderr,"initialFileExists function did not return one, ");
            fprintf(stderr,"which was the expected value here.\n");
            free(fileLocation);
            fileLocation=NULL;
            return -10000.;
    }

    // Check if *.mesh file exists and remove it
    if (initialFileExists(fileLocation,pParameters->name_length)!=1)
    {
            PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
            fprintf(stderr,"initialFileExists function did not return one, ");
            fprintf(stderr,"which was the expected value here.\n");
            free(fileLocation);
            fileLocation=NULL;
            return -10000.;
    }

    if (remove(fileLocation))
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: wrong ");
        fprintf(stderr,"return (=-1) of the standard remove c-function in ");
        fprintf(stderr,"the attempt of removing the %s file.\n",fileLocation);
        free(fileLocation);
        fileLocation=NULL;
        return -10000.;
    }

    // Copy the *.[iterationInTheLoop-1].mesh into *.mesh
    if (!copyFileLocation(pParameters->name_mesh,pParameters->name_length,
                                                                  fileLocation))
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
        fprintf(stderr,"copyFileLocation function returned zero instead ");
        fprintf(stderr,"of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return -10000.;
    }
    strncpy(pParameters->name_mesh,fileLocation,pParameters->name_length);

    // Free the memory allocated for the mesh
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        sizeMemory=sizeof(Mesh)+(pMesh->nver)*sizeof(Point);
        sizeMemory+=(pMesh->nnorm+pMesh->ntan)*sizeof(Vector);
        sizeMemory+=(pMesh->nedg)*sizeof(Edge);
        sizeMemory+=(pMesh->ntri)*sizeof(Triangle);
        sizeMemory+=(pMesh->ntet)*sizeof(Tetrahedron);
        fprintf(stdout,"\nCleaning the allocated memory ");
        fprintf(stdout,"(%d,%d Mo) for ",sizeMemory/1000000,sizeMemory%1000000);
        fprintf(stdout,"reloading the %d-th mesh.\n",iterationInTheLoop-1);
    }
    freeMeshMemory(pMesh);

    // Read the new *.mesh file
    if (!readMeshFileAndAllocateMesh(pParameters,pMesh))
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
        fprintf(stderr,"readMeshFileAndAllocateMesh function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return -10000.;
    }

    // Update the parameters related to the computational box
    if (!updateDiscretizationParameters(pParameters,pMesh))
    {
        PRINT_ERROR("In computeProbabilityAndReloadPreviousMesh: ");
        fprintf(stderr,"updateDiscretizationParameters function returned ");
        fprintf(stderr,"zero instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return -10000.;
    }

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return returnValue;
}

/* ************************************************************************** */
// The function optimization modifies the shape of the MPD domain according to
// the shape derivative in order to increase to probability. This is the
// function used iteratively in the optimization loop of the main function.
// It has the Parameters*, Mesh*, Data*, ChemicalSystem* (both defined in
// main.h), the int interationInTheLoop, and the three time_t* variables as
// input arguments. It returns one on success otherwise zero for an error
/* ************************************************************************** */
int optimization(Parameters* pParameters, Mesh* pMesh, Data* pData,
                 ChemicalSystem* pChemicalSystem, int iterationInTheLoop,
                 time_t* pGlobalInitialTimer, time_t* pStartLocalTimer,
                                                         time_t* pEndLocalTimer)
{
    int i=0, counter=0, n=0, nMax=0, boolean=0;
    double tMin=0, tMax=0, t0=0., t1=0., *pShapeGradient=NULL, pMax=0., pMin=0.;
    double p0=0., p1=0., h=0., deltaT=0., hMin=0., hMax=0.;

    // Check the input pointers
    if (pParameters==NULL || pMesh==NULL || pData==NULL ||
                           pChemicalSystem==NULL || pGlobalInitialTimer==NULL ||
                                 pStartLocalTimer==NULL || pEndLocalTimer==NULL)
    {
        PRINT_ERROR("In optimization: one of the input pointers ");
        fprintf(stderr,"pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pData=%p, pMesh=%p, ",(void*)pMesh,(void*)pData);
        fprintf(stderr,"pChemicalSystem=%p, ",(void*)pChemicalSystem);
        fprintf(stderr,"pGlobalInitialTimer=%p, ",(void*)pGlobalInitialTimer);
        fprintf(stderr,"pStartLocalTimer=%p or ",(void*)pStartLocalTimer);
        fprintf(stderr,"pEndLocalTimer=%p, ",(void*)pEndLocalTimer);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    // Check the iteration in the loop variable
    if (iterationInTheLoop<1 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In optimization: the input iterationInTheLoop variable ");
        fprintf(stderr,"(=%d) should be a positive ",iterationInTheLoop);
        fprintf(stderr,"integer not (strictly) greater than the maximal ");
        fprintf(stderr,"number (=%d) of iterations ",pParameters->iter_max);
        fprintf(stderr,"allowed in the optimization loop.\n");
        return 0;
    }

    // Check the stop criteria
    boolean=0;
    if (pParameters->iter_told0p>0.)
    {
        if (DEF_ABS(pData->d0p[iterationInTheLoop-1])<pParameters->iter_told0p)
        {
            boolean++;
        }
    }
    else
    {
        return -1;
    }

    if (pParameters->iter_told1p>0.)
    {
        if (DEF_ABS(pData->d1p[iterationInTheLoop-1])<pParameters->iter_told1p)
        {
            boolean++;
        }
    }
    else
    {
        return -1;
    }

    if (pParameters->iter_told2p>0.)
    {
        if (DEF_ABS(pData->d2p[iterationInTheLoop-1])<pParameters->iter_told2p)
        {
            if (pParameters->opt_mode<=0)
            {
                return -1;
            }
            else
            {
                boolean++;
            }
        }
    }
    else
    {
        return -1;
    }

    if (boolean==3)
    {
        return -1;
    }

    switch (pParameters->opt_mode)
    {
// Case where Omega_new={dP/dOmega_old>0}
        case 4:

            fprintf(stdout,"\nSTEP 1: MAKE THE SHAPE GRADIENT A LEVEL-SET ");
            fprintf(stdout,"FUNCTION WITH UNITARY GRADIENT.\n");

            // Save (the opposite of) the shape gradient at all vertices
            if (!writingSolFile(pParameters,pMesh))
            {
                PRINT_ERROR("In optimization: writingSolFile function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Vizualize the shape gradient before renormalization
            if (pParameters->save_print>0)
            {
                if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==3)
                {
                    if (!plotMeshWithMeditSoftware(pParameters))
                    {
                        PRINT_ERROR("In optimization: ");
                        fprintf(stderr,"plotMeshWithMeditSoftware function ");
                        fprintf(stderr,"returned zero instead of one.\n");
                        return 0;
                    }
                }
            }

            // Renormalize the shape gradient into a level-set function
            if (pParameters->verbose)
            {
                fprintf(stdout,"\nRenormalization of the shape gradient.");
            }

            if (!renormalizeWithMshdistSoftware(pParameters,"sol"))
            {
                PRINT_ERROR("In optimization: renormalizeWithMshdistSoftware ");
                fprintf(stderr,"function returned zero instead of one.\n");
                return 0;
            }

            // Vizualize the shape gradient after renormalization
            if (pParameters->save_print>0)
            {
                if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==2)
                {
                    if (!plotMeshWithMeditSoftware(pParameters))
                    {
                        PRINT_ERROR("In optimization: ");
                        fprintf(stderr,"plotMeshWithMeditSoftware function ");
                        fprintf(stderr,"returned zero instead of one.\n");
                        return 0;
                    }
                }
            }

            // Adapt the mesh to both molecular orbitals and level set function
            if (!performLevelSetAdaptation(pParameters,pMesh,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                fprintf(stderr,"function returned zero instead of one.\n");
                return 0;
            }

            fprintf(stdout,"\nSTEP 4: COMPUTE THE PROBABILITY, POPULATION ");
            fprintf(stdout,"AND SHAPE GRADIENT ON NEW DOMAIN.\n");

            // Compute the overlap matrix, probability, shape gradient, and
            // residual for the new domain
            if (!shapeDerivative(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: shapeDerivative function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Save data and delete mesh if necessary
            if (!saveDataInTheLoop(pParameters,pMesh,pData,pChemicalSystem,
                                   iterationInTheLoop,pGlobalInitialTimer,
                                               pStartLocalTimer,pEndLocalTimer))
            {
                PRINT_ERROR("In optimization: saveDataInTheLoop function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }
            break;

// Case where Omega_new=Lagangian_advection{dP/dOmega_old*Normal}
        case 3:

            // Save the shape gradient
            if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: saveTheShapeGradient function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Advect the mesh thanks to Lagrangian mode of mmg3d software
            if (!computeLagrangianMode(pParameters,pMesh,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: computeLagrangianMode function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Re-Adapt the mesh to both molecular orbitals and domain geometry
            if (!performLevelSetAdaptation(pParameters,pMesh,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                fprintf(stderr,"function returned zero instead of one.\n");
                return 0;
            }

            fprintf(stdout,"\nSTEP 3: COMPUTE PROBABILITY, POPULATION ");
            fprintf(stdout,"AND SHAPE GRADIENT ON THE NEW DOMAIN.\n");

            // Compute the overlap matrix, probability, shape gradient, and
            // residual for the new domain
            if (!shapeDerivative(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: shapeDerivative function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Save data and delete mesh if necessary
            if (!saveDataInTheLoop(pParameters,pMesh,pData,pChemicalSystem,
                                   iterationInTheLoop,pGlobalInitialTimer,
                                               pStartLocalTimer,pEndLocalTimer))
            {
                PRINT_ERROR("In optimization: saveDataInTheLoop function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            break;

// Case where Omega_new=EulerianLevelSet_advection{dP/dOmega_old*Normal}
        case 2:

            // Hacking with a temporary adaptative mode
            if (pParameters->nu_spin)
            {
                pParameters->delta_t=deltaT;
                if (pData->d0p[iterationInTheLoop-1]<
                                         1.e-6*pData->d1p[iterationInTheLoop-1])
                {
                    pParameters->delta_t=1.e-6;
                }
                else
                {
                    pParameters->delta_t=pData->d0p[iterationInTheLoop-1]/
                                               pData->d1p[iterationInTheLoop-1];
                }
                fprintf(stdout,"\nAdaptative step: %lf.\n",
                                                          pParameters->delta_t);
            }

            // Save the shape gradient
            if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: saveTheShapeGradient function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Advect the mesh thanks to Eulerian mode (level-set approach)
            if (!computeEulerianMode(pParameters,pMesh,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: computeEulerianMode function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Adapt the mesh to both molecular orbitals and new domain geometry
            if (!performLevelSetAdaptation(pParameters,pMesh,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                fprintf(stderr,"function returned zero instead of one.\n");
                return 0;
            } 

            fprintf(stdout,"\nSTEP 6: COMPUTE PROBABILITY, POPULATION ");
            fprintf(stdout,"AND SHAPE GRADIENT ON THE NEW DOMAIN.\n");

            // Compute the overlap matrix, probability, shape gradient, and
            // residual for the new domain
            if (!shapeDerivative(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: shapeDerivative function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Save data and delete mesh if necessary
            if (!saveDataInTheLoop(pParameters,pMesh,pData,pChemicalSystem,
                                   iterationInTheLoop,pGlobalInitialTimer,
                                               pStartLocalTimer,pEndLocalTimer))
            {
                PRINT_ERROR("In optimization: saveDataInTheLoop function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Hacking with a temporary adaptative mode
            if (pParameters->nu_spin)
            {
                pParameters->delta_t=deltaT;
            }

            break;

// Golden search algorithm with Eulerian/Lagrangian perturbations
// now Armijo-Goldstein
        case 1:

            // Save the initial shape gradient computed at the mesh vertices
            pShapeGradient=(double*)calloc(pMesh->nver,sizeof(double));
            if (pShapeGradient==NULL)
            {
                PRINT_ERROR("In optimization: could not allocate memory for ");
                fprintf(stderr,"the local double* pShapeGradient variable.\n");
                return 0;
            }
            for (i=0; i<pMesh->nver; i++)
            {
                pShapeGradient[i]=pMesh->pver[i].value;
            }

            // Initialize extremal value of tMin: we known that for tMax
            // Armijo's rule is not satisfied (thus Goldstein yes) with
            // parameters 0.25 (Armijo) and 0.75 (Goldstein)
            p0=pData->pnu[iterationInTheLoop-1];
            h=pData->d1p[iterationInTheLoop-1];
            hMin=.5*(pParameters->hmin_ls+pParameters->hmin_lag);
            hMax=.5*(pParameters->hmax_ls+pParameters->hmax_lag);

            t1=-2.;
            tMin=DEF_ABS(p0)/h;
            tMax=4.*DEF_ABS(1.-p0)/h;
            if (tMin>=tMax)
            {
                tMin=tMax/10.;
            }

            nMax=5;
            counter=-1;
            do
            {
                counter++;

                // Perform an initial perturbation with intensity tMin
                // (tMin too big Eulerian perturbations, else Lagrangian's ones)
                fprintf(stdout,"\nSEARCHING THE STARTING INTERVAL FOR ");
                fprintf(stdout,"OPTIMAL STEP.\nCOMPUTING p(%lf).\n",tMin);
                for (i=0; i<pMesh->nver; i++)
                {
                    pMesh->pver[i].value=tMin*pShapeGradient[i];
                }

                // Save the shape gradient
                if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: saveTheShapeGradient ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                if (tMin*sqrt(h)<hMin*hMin)
                {
                    // Advect mesh thanks to Lagrangian mode of mmg3d software
                    fprintf(stdout,"\nLagrangian mode.\n");
                    if (!computeLagrangianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: computeLagrangianMode ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }
                }
                else
                {
                    // Advect mesh thanks to Eulerian mode (level-set approach)
                    fprintf(stdout,"\nEulerian mode (level-set).\n");
                    if (!computeEulerianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: computeEulerianMode ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }
                }

                // Adapt mesh to both molecular orbitals and new domain geometry
                if (!performLevelSetAdaptation(pParameters,pMesh,
                                            pChemicalSystem,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Compute pMin and reload the previous mesh
                pMin=computeProbabilityAndReloadPreviousMesh(pParameters,pMesh,
                                                            pData,
                                                            pChemicalSystem,
                                                            iterationInTheLoop);
                if (pMin==-10000.)
                {
                    PRINT_ERROR("In optimization: ");
                    fprintf(stderr,"computeProbabilityAndReloadPreviousMesh ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Look if pMin satisfied Armijo's rule or not
                if (pMin>=p0+.25*tMin*h)
                {
                    // Then look if Goldstein rule is satisfied or not
                    if (pMin<=p0+.75*tMin*h)
                    {
                        t1=tMin;
                    }
                    else
                    {
                        // Try to estimate better hMax to avoid big advections 
                        t1=10.*tMin;
                        if (t1>tMin && t1<tMax)
                        {
                            if (t1>.5*(tMax+tMin))
                            {
                                t1=.5*(tMax+tMin);
                            }
                        }
                        else
                        {
                            t1=.5*(tMax+tMin);
                        }
                    }
                }
                else
                {
                    tMax=tMin;
                    tMin=tMax/100.;
                }

            } while (t1==-2. && counter<nMax);

            // Case where numerical errors does not yield to Armijo's condition
            if (t1==-2. && counter==nMax)
            {
                t1=tMin;
            }

            // Now Armijo's rule is satisfied for tMin and not for tMax
            if (t1!=tMin)
            {
                fprintf(stdout,"\nINITIAL INTERVAL FOUND: ");
                fprintf(stdout,"[%lf, %lf].\n",tMin,tMax);
                fprintf(stdout,"STARTING THE ARMIJO-GOLDSTEIN LINE SEARCH.\n");
                do
                {
                    for (i=0; i<pMesh->nver; i++)
                    {
                        pMesh->pver[i].value=t1*pShapeGradient[i];
                    }

                    // Save the shape gradient
                    if (!saveTheShapeGradient(pParameters,pMesh,
                                                            iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: saveTheShapeGradient ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }

                    if (t1*sqrt(h)<hMin*hMin)
                    {
                        // Advect mesh thanks to Lagrangian mode of mmg3d
                        fprintf(stdout,"\nLagrangian mode.\n");
                        if (!computeLagrangianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"computeLagrangianMode function ");
                            fprintf(stderr,"returned zero instead of one.\n");
                            free(pShapeGradient);
                            pShapeGradient=NULL;
                            return 0;
                        }
                    }
                    else
                    {
                        // Advect mesh thanks to Eulerian mode (level-set)
                        fprintf(stdout,"\nEulerian mode (level-set).\n");
                        if (!computeEulerianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"computeEulerianMode function ");
                            fprintf(stderr,"returned zero instead of one.\n");
                            free(pShapeGradient);
                            pShapeGradient=NULL;
                            return 0;
                        }
                    }

                    // Adapt mesh to molecular orbitals and new domain geometry
                    if (!performLevelSetAdaptation(pParameters,pMesh,
                                            pChemicalSystem,iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: ");
                        fprintf(stderr,"performLevelSetAdaptation function ");
                        fprintf(stderr,"returned zero instead of one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }

                    // Compute p1 and reload the previous mesh
                    p1=computeProbabilityAndReloadPreviousMesh(pParameters,
                                                            pMesh,
                                                            pData,
                                                            pChemicalSystem,
                                                            iterationInTheLoop);
                    if (p1==-10000.)
                    {
                        PRINT_ERROR("In optimization: ");
                        fprintf(stderr,
                                    "computeProbabilityAndReloadPreviousMesh ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }

                    // Look if pMin satisfied Armijo's rule or not
                    if (p1>=p0+.25*t1*h)
                    {
                        // Then look if Goldstein rule is satisfied or not
                        if (p1<=p0+.75*t1*h)
                        {
                            tMin=t1;
                            tMax=t1;
                        }
                        else
                        {
                            tMin=t1;
                            if (p1<p0+pParameters->iter_told0p)
                            {
                                // Case where num. errors never yield Golstein
                                tMax=tMin;
                            }
                            else
                            {
                                t1=.5*(tMax+tMin);
                                fprintf(stdout,"\nRESTRICTING LINE SEARCH ");
                                fprintf(stdout,"TO THE INTERVAL ");
                                fprintf(stdout,"[%lf, %lf].\n",tMin,tMax);
                                fprintf(stdout,"p(%lf)=%lf AND ",tMin,p1);
                                fprintf(stdout,"COMPUTING p(%lf).\n",t1);

                            }
                        }
                    }
                    else
                    {
                        tMax=t1;
                        t1=.5*(tMax+tMin);                     
                        fprintf(stdout,"\nRESTRICTING LINE SEARCH ");
                        fprintf(stdout,"TO THE INTERVAL ");
                        fprintf(stdout,"[%lf, %lf].\n",tMin,tMax);
                        fprintf(stdout,"p(%lf)=%lf AND ",tMax,p1);
                        fprintf(stdout,"COMPUTING p(%lf).\n",t1);
                    }
                } while (tMin!=tMax);
            }

            // The optimal step has been found, update t0 (used after comments)
            t0=t1;

/*            tMax=1.;
            tMin=0.;
            pMin=pData->pnu[iterationInTheLoop-1];

            // First perform an initial Eulerian perturbation of intensity one
            fprintf(stdout,"\nSEARCHING THE STARTING INTERVAL FOR THE ");
            fprintf(stdout,"OPTIMAL STEP.\nCOMPUTING p(%lf).\n",tMax);

            // Save the shape gradient
            if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: saveTheShapeGradient function ");
                fprintf(stderr,"returned zero instead of one.\n");
                free(pShapeGradient);
                pShapeGradient=NULL;
                return 0;
            }

            // Advect the mesh thanks to Eulerian mode (level-set approach)
            if (!computeEulerianMode(pParameters,pMesh,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: computeEulerianMode function ");
                fprintf(stderr,"returned zero instead of one.\n");
                free(pShapeGradient);
                pShapeGradient=NULL;
                return 0;
            }

            // Adapt the mesh to both molecular orbitals and new domain geometry
            if (!performLevelSetAdaptation(pParameters,pMesh,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                fprintf(stderr,"function returned zero instead of one.\n");
                free(pShapeGradient);
                pShapeGradient=NULL;
                return 0;
            }

            // Compute pMax and reload the previous mesh
            pMax=computeProbabilityAndReloadPreviousMesh(pParameters,pMesh,
                                                         pData,pChemicalSystem,
                                                            iterationInTheLoop);
            if (pMax==-10000.)
            {
                PRINT_ERROR("In optimization: ");
                fprintf(stderr,"computeProbabilityAndReloadPreviousMesh ");
                fprintf(stderr,"function returned zero instead of one.\n");
                free(pShapeGradient);
                pShapeGradient=NULL;
                return 0;
            }

            // Trying to enlarge the searching interval
            t0=tMax;
            p0=pMax;

            tMax=tMin;
            pMax=pMin;

            counter=0;
            while (pMax<p0 && counter<5)
            {
                fprintf(stdout,"\nTRYING TO ENLARGE THE SEARCHING INTERVAL ");
                fprintf(stdout,"[%lf, %lf].\n",tMax,t0);
                fprintf(stdout,"p(%lf)=%lf p(%lf)=%lf. ",tMax,pMax,t0,p0);

                tMin=tMax;
                pMin=pMax;

                tMax=t0;
                pMax=p0;

                counter++;
                t0=exp(counter);
                fprintf(stdout,"COMPUTING p(%lf).\n",t0);

                for (i=0; i<pMesh->nver; i++)
                {
                    pMesh->pver[i].value=t0*pShapeGradient[i];
                }

                // Save the shape gradient
                if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: saveTheShapeGradient ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Advect the mesh thanks to Eulerian mode (level-set approach)
                if (!computeEulerianMode(pParameters,pMesh,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: computeEulerianMode ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Adapt mesh to both molecular orbitals and new domain geometry
                if (!performLevelSetAdaptation(pParameters,pMesh,
                                               pChemicalSystem,
                                                            iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Compute p0 and reload the previous mesh
                p0=computeProbabilityAndReloadPreviousMesh(pParameters,pMesh,
                                                           pData,
                                                           pChemicalSystem,
                                                            iterationInTheLoop);
                if (p0==-10000.)
                {
                    PRINT_ERROR("In optimization: ");
                    fprintf(stderr,"computeProbabilityAndReloadPreviousMesh ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }
            }

            if (pMax>=p0)
            {

                tMax=t0;
                pMax=p0;
    
                // Starting golden-line search on the initial interval
                fprintf(stdout,"\nINITIAL INTERVAL FOUND: ");
                fprintf(stdout,"[%lf, %lf].\n",tMin,tMax);
                fprintf(stdout,"STARTING THE GOLDEN-SECTION LINE SEARCH.\n");

                h=tMax-tMin;
                if (tMax==1.)
                {
                    nMax=2;
                    //nMax=(int)(floor(log(1.e-1/h)/log(INV_PHI)));
                }
                else
                {
                    nMax=0;
                }

                // Evaluation of p0 (t0<1. Lagr. otherwise Eulerian approach)
                t0=tMin+INV_PHI2*h;
                for (i=0; i<pMesh->nver; i++)
                {
                    pMesh->pver[i].value=t0*pShapeGradient[i];
                }

                // Save the shape gradient
                if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: saveTheShapeGradient");
                    fprintf(stderr," function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                if (t0<1.)
                {
                    // Advect mesh thanks to Lagrangian mode of mmg3d software
                    if (!computeLagrangianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: computeLagrangianMode ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }
                }
                else
                {
                    // Advect mesh thanks to Eulerian mode (level-set approach)
                    if (!computeEulerianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: computeEulerianMode ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }
                }

                // Adapt mesh to both molecular orbitals and new domain geometry
                if (!performLevelSetAdaptation(pParameters,pMesh,
                                            pChemicalSystem,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Compute p0 and reload the previous mesh
                p0=computeProbabilityAndReloadPreviousMesh(pParameters,pMesh,
                                                           pData,
                                                           pChemicalSystem,
                                                            iterationInTheLoop);
                if (p0==-10000.)
                {
                    PRINT_ERROR("In optimization: ");
                    fprintf(stderr,"computeProbabilityAndReloadPreviousMesh ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Evaluation of p1 (t1<1. Lagr. otherwise Eulerian approach)
                t1=tMin+INV_PHI*h;
                fprintf(stdout,"\nFIRST EVALUATION ENDED: p(%lf)=%lf; ",t0,p0);
                fprintf(stdout,"COMPUTING p(%lf).\n",t1);
                for (i=0; i<pMesh->nver; i++)
                {
                    pMesh->pver[i].value=t1*pShapeGradient[i];
                }

                // Save the shape gradient
                if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: saveTheShapeGradient ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                if (t1<1.)
                {
                    // Advect mesh thanks to Lagrangian mode of mmg3d software
                    if (!computeLagrangianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: computeLagrangianMode ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }
                }
                else
                {
                    // Advect mesh thanks to Eulerian mode (level-set approach)
                    if (!computeEulerianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                    {
                        PRINT_ERROR("In optimization: computeEulerianMode ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(pShapeGradient);
                        pShapeGradient=NULL;
                        return 0;
                    }
                }

                // Adapt mesh to both molecular orbitals and new domain geometry
                if (!performLevelSetAdaptation(pParameters,pMesh,
                                               pChemicalSystem,
                                                            iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                // Compute p1 and reload the previous mesh
                p1=computeProbabilityAndReloadPreviousMesh(pParameters,pMesh,
                                                           pData,
                                                           pChemicalSystem,
                                                            iterationInTheLoop);
                if (p1==-10000.)
                {
                    PRINT_ERROR("In optimization: ");
                    fprintf(stderr,"computeProbabilityAndReloadPreviousMesh ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }

                for (n=0; n<nMax; n++)
                {
                    if (p0<p1)
                    {
                        tMin=t0;
                        pMin=p0;

                        t0=t1;
                        p0=p1;

                        h*=INV_PHI;
                        t1=tMin+INV_PHI*h;

                        fprintf(stdout,"\nTRYING TO RESTRICT THE LINE SEARCH ");
                        fprintf(stdout,"ON THE INTERVAL ");
                        fprintf(stdout,"[%lf, %lf].\n",tMin,tMax);
                        fprintf(stdout,"p(%lf)=%lf ",tMin,pMin);
                        fprintf(stdout,"p(%lf)=%lf ",t0,p0);
                        fprintf(stdout,"p(%lf)=%lf.\nCOMPUTING ",tMax,pMax);
                        fprintf(stdout,"p(%lf).\n",t1);

                        for (i=0; i<pMesh->nver; i++)
                        {
                            pMesh->pver[i].value=t1*pShapeGradient[i];
                        }

                       // Save the shape gradient
                       if (!saveTheShapeGradient(pParameters,pMesh,
                                                            iterationInTheLoop))
                       {
                           PRINT_ERROR("In optimization: ");
                           fprintf(stderr,"saveTheShapeGradient function ");
                           fprintf(stderr,"returned zero instead of one.\n");
                           free(pShapeGradient);
                           pShapeGradient=NULL;
                           return 0;
                       }

                       if (t1<1.)
                       {
                            // Advect mesh thanks to Lagrangian mode of mmg3d
                            if (!computeLagrangianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                            {
                                PRINT_ERROR("In optimization: ");
                                fprintf(stderr,"computeLagrangianMode ");
                                fprintf(stderr,"function returned zero ");
                                fprintf(stderr,"instead of one.\n");
                                free(pShapeGradient);
                                pShapeGradient=NULL;
                                return 0;
                            }
                        }
                        else
                        {
                            // Advect mesh thanks to Eul. mode (level-set mode)
                            if (!computeEulerianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                            {
                                PRINT_ERROR("In optimization: ");
                                fprintf(stderr,"computeEulerianMode function ");
                                fprintf(stderr,"returned zero instead of ");
                                fprintf(stderr,"one.\n");
                                free(pShapeGradient);
                                pShapeGradient=NULL;
                                return 0;
                            }
                        }

                        // Adapt mesh to both molecular orbitals and new domain
                        if (!performLevelSetAdaptation(pParameters,pMesh,
                                                   pChemicalSystem,
                                                            iterationInTheLoop))
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"performLevelSetAdaptation ");
                            fprintf(stderr,"function returned zero instead ");
                            fprintf(stderr,"of one.\n");
                            free(pShapeGradient);
                            pShapeGradient=NULL;
                            return 0;
                        }

                        // Compute p1 and reload the previous mesh
                        p1=computeProbabilityAndReloadPreviousMesh(pParameters,
                                                                   pMesh,pData,
                                                                pChemicalSystem,
                                                            iterationInTheLoop);
                        if (p1==-10000.)
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"computeProbability");
                            fprintf(stderr,"AndReloadPreviousMesh function ");
                            fprintf(stderr,"returned zero instead of one.\n");
                            free(pShapeGradient);
                            pShapeGradient=NULL;
                            return 0;
                        }
                    }
                    else
                    {
                        tMax=t1;
                        pMax=p1;

                        t1=t0;
                        p1=p0;

                        h*=INV_PHI;
                        t0=tMin+INV_PHI2*h;

                        fprintf(stdout,"\nTRYING TO RESTRICT THE LINE SEARCH ");
                        fprintf(stdout,"ON THE INTERVAL ");
                        fprintf(stdout,"[%lf, %lf].\n",tMin,tMax);
                        fprintf(stdout,"p(%lf)=%lf ",tMin,pMin);
                        fprintf(stdout,"p(%lf)=%lf ",t1,p1);
                        fprintf(stdout,"p(%lf)=%lf.\nCOMPUTING ",tMax,pMax);
                        fprintf(stdout,"p(%lf).\n",t0);

                        for (i=0; i<pMesh->nver; i++)
                        {
                            pMesh->pver[i].value=t0*pShapeGradient[i];
                        }

                        // Save the shape gradient
                        if (!saveTheShapeGradient(pParameters,pMesh,
                                                            iterationInTheLoop))
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"saveTheShapeGradient function ");
                            fprintf(stderr,"returned zero instead of one.\n");
                            free(pShapeGradient);
                            pShapeGradient=NULL;
                            return 0;
                        }

                        if (t0<1.)
                        {
                            // Advect mesh thanks to Lagrangian mode of mmg3d
                            if (!computeLagrangianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                            {
                                PRINT_ERROR("In optimization: ");
                                fprintf(stderr,"computeLagrangianMode ");
                                fprintf(stderr,"function returned zero ");
                                fprintf(stderr,"instead of one.\n");
                                free(pShapeGradient);
                                pShapeGradient=NULL;
                                return 0;
                            }
                        }
                        else
                        {
                            // Advect mesh thanks to Eul. mode (level-set mode)
                            if (!computeEulerianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                            {
                                PRINT_ERROR("In optimization: ");
                                fprintf(stderr,"computeEulerianMode function ");
                                fprintf(stderr,"returned zero instead of ");
                                fprintf(stderr,"one.\n");
                                free(pShapeGradient);
                                pShapeGradient=NULL;
                                return 0;
                            }
                        }

                        // Adapt mesh to both molecular orbitals and new domain
                        if (!performLevelSetAdaptation(pParameters,pMesh,
                                                       pChemicalSystem,
                                                            iterationInTheLoop))
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"performLevelSetAdaptation ");
                            fprintf(stderr,"function returned zero instead ");
                            fprintf(stderr,"of one.\n");
                            free(pShapeGradient);
                            pShapeGradient=NULL;
                            return 0;
                        }

                        // Compute p0 and reload the previous mesh
                        p0=computeProbabilityAndReloadPreviousMesh(pParameters,
                                                                   pMesh,pData,
                                                                pChemicalSystem,
                                                            iterationInTheLoop);
                        if (p0==-10000.)
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"computeProbabilityAnd");
                            fprintf(stderr,"ReloadPreviousMesh function ");
                            fprintf(stderr,"returned zero instead of one.\n");
                            free(pShapeGradient);
                            pShapeGradient=NULL;
                            return 0;
                        }
                    }
                }
                if (p0<p1)
                {
                    t0=t1;
                }
            }
*/

            // Finally compute the optimal line search
            fprintf(stdout,"\nOPTIMAL STEP FOUND: %lf.\nCOMPUTING THE ",t0);
            fprintf(stdout,"CORRESPONDING ADVECTED DOMAIN.\n");

            for (i=0; i<pMesh->nver; i++)
            {
                pMesh->pver[i].value=t0*pShapeGradient[i];
            }

            // Free the memory allocated for pShapeGradient
            free(pShapeGradient);
            pShapeGradient=NULL; 

            if (t0*sqrt(h)<hMin*hMin)
            {
                pParameters->opt_mode=3;
            }
            else
            {
                pParameters->opt_mode=2;
            }

            // Save the shape gradient
            if (!saveTheShapeGradient(pParameters,pMesh,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: saveTheShapeGradient ");
                fprintf(stderr,"function returned zero instead of ");
                fprintf(stderr,"one.\n");
                return 0;
            }

            if (t0*sqrt(h)<hMin*hMin)
            {
                // Advect mesh thanks to Lagrangian mode of mmg3d software
                if (!computeLagrangianMode(pParameters,pMesh,
                                                            iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: ");
                    fprintf(stderr,"computeLagrangianMode function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }
            }
            else
            {
                // Advect mesh thanks to Eulerian mode (level-set approach)
                pParameters->opt_mode=2;
                if (!computeEulerianMode(pParameters,pMesh,iterationInTheLoop))
                {
                    PRINT_ERROR("In optimization: ");
                    fprintf(stderr,"computeEulerianMode function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    free(pShapeGradient);
                    pShapeGradient=NULL;
                    return 0;
                }
            }

            // Adapt mesh to both molecular orbitals and new domain
            if (!performLevelSetAdaptation(pParameters,pMesh,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: performLevelSetAdaptation ");
                fprintf(stderr,"function returned zero instead of one.\n");
                return 0;
            }

            if (t0*sqrt(h)<hMin*hMin)
            {
                fprintf(stdout,"\nSTEP 3: COMPUTE PROBABILITY, POPULATION ");
                fprintf(stdout,"AND SHAPE GRADIENT ON THE NEW DOMAIN.\n");
            }
            else
            {
                fprintf(stdout,"\nSTEP 6: COMPUTE PROBABILITY, POPULATION ");
                fprintf(stdout,"AND SHAPE GRADIENT ON THE NEW DOMAIN.\n");
            }

            if ((iterationInTheLoop+1)%3==0)
            {
                pParameters->opt_mode=4;
            }

            // Compute the overlap matrix, probability, shape gradient, and
            // residual for the new domain
            if (!shapeDerivative(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: shapeDerivative function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Save data and delete mesh if necessary
            if (!saveDataInTheLoop(pParameters,pMesh,pData,pChemicalSystem,
                                   iterationInTheLoop,pGlobalInitialTimer,
                                               pStartLocalTimer,pEndLocalTimer))
            {
                PRINT_ERROR("In optimization: saveDataInTheLoop function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            pParameters->opt_mode=1;
            break;

// Omega_new=Omega_Old+Hexa(dP/dOmega_old>0)-Hexa(dP/dOmega_old<0)
        case -1:

            // Vizualize the mesh of the previous iteration
            if (iterationInTheLoop>1)
            {
                if (pParameters->save_print>0)
                {
                    if ((iterationInTheLoop-1)%pParameters->save_print==0)
                    {
                        if (!plotMeshWithMeditSoftware(pParameters))
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"plotMeshWithMeditSoftware ");
                            fprintf(stderr,"function returned zero instead ");
                            fprintf(stderr,"of one.\n");
                            return 0;
                        }
                    }
                }
            }

            // Compute the overlap matrix, probability, shape gradient, and
            // residual for the new domain
            if (!shapeDerivative(pParameters,pMesh,pData,pChemicalSystem,
                                                            iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: shapeDerivative function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Save data and delete mesh if necessary
            if (!saveDataInTheLoop(pParameters,pMesh,pData,pChemicalSystem,
                                   iterationInTheLoop,pGlobalInitialTimer,
                                               pStartLocalTimer,pEndLocalTimer))
            {
                PRINT_ERROR("In optimization: saveDataInTheLoop function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }
            break;

// Omega_new=Omega_Old+Hexa[P(old+Hexa)>P(old)]-Hexa[P(old-Hexa)>P(old)]
        case -2:

            // Vizualize the mesh of the previous iteration
            if (iterationInTheLoop>1)
            {
                if (pParameters->save_print>0)
                {
                    if ((iterationInTheLoop-1)%pParameters->save_print==0)
                    {
                        if (!plotMeshWithMeditSoftware(pParameters))
                        {
                            PRINT_ERROR("In optimization: ");
                            fprintf(stderr,"plotMeshWithMeditSoftware ");
                            fprintf(stderr,"function returned zero instead ");
                            fprintf(stderr,"of one.\n");
                            return 0;
                        }
                    }
                }
            }

            // Compute the overlap matrix, probability, shape gradient, and
            // residual for the new domain after removing or adding hexahedron
            // close to the boundary one by one according to the probability
            if (!exhaustiveSearchAlgorithm(pParameters,pMesh,pData,
                                            pChemicalSystem,iterationInTheLoop))
            {
                PRINT_ERROR("In optimization: shapeDerivative function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Save data and delete mesh if necessary
            if (!saveDataInTheLoop(pParameters,pMesh,pData,pChemicalSystem,
                                   iterationInTheLoop,pGlobalInitialTimer,
                                               pStartLocalTimer,pEndLocalTimer))
            {
                PRINT_ERROR("In optimization: saveDataInTheLoop function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }
            break;

            default:
                PRINT_ERROR("In optimization: the opt_mode variable ");
                fprintf(stderr,"(=%d) can only take ",pParameters->opt_mode);
                fprintf(stderr,"the value 4, 3, 2, -1, or -2 inside the");
                fprintf(stderr,"optimization function.\n ");
                return 0;
                break;
    }

    return 1;
}

