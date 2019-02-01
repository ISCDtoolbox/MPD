/**
* \file loadChemistry.c
* \brief It contains all the functions used to initially load the chemical
*        informations from a *.wfn/ *.chem file in the MPD algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* The main function of this file is called \ref loadChemistry and many other
* functions should be static but have been define as non-static for performing
* unit-tests on them.
*/

#include "loadChemistry.h"

/* ************************************************************************** */
// The function initializeChemicalStructure sets to zero all the variables of
// the ChemicalSystem structure (and pointers to NULL). It has the
// ChemicalSystem* variable (defined in main.h) as input argument and does not
// return any value (void output)
/* ************************************************************************** */
void initializeChemicalStructure(ChemicalSystem* pChemicalSystem)
{
    if (pChemicalSystem!=NULL)
    {
        pChemicalSystem->nnucl=0;
        pChemicalSystem->pnucl=NULL;

        pChemicalSystem->ngauss=0;

        pChemicalSystem->nmorb=0;
        pChemicalSystem->pmorb=NULL;
    }

    return;
}

/* ************************************************************************** */
// The function freeChemicalMemory frees the memory dynamically allocated with
// calloc/malloc/realloc for the ChemicalSystem structure (but other variables
// than pointers are not reset to zero). It has the ChemicalSystem* variable
// (defined in main.h) as input argument and it does not return any value
/* ************************************************************************** */
void freeChemicalMemory(ChemicalSystem* pChemicalSystem)
{
    int i=0, iMax=0;

    if (pChemicalSystem!=NULL)
    {
        free(pChemicalSystem->pnucl); // free function does not any return value
        pChemicalSystem->pnucl=NULL;

        if (pChemicalSystem->pmorb!=NULL)
        {
            iMax=pChemicalSystem->nmorb;
            if (iMax<1)
            {
                fprintf(stdout,"\nWarning in freeChemicalMemory function: ");
                fprintf(stdout,"%d molecular orbitals are saved in ",iMax);
                fprintf(stdout,"pChemicalSystem (input) variable. Some ");
                fprintf(stdout,"allocated pointers may not be freed up ");
                fprintf(stdout,"correctly.\n");
            }
            for (i=0; i<iMax; i++)
            {
                free(pChemicalSystem->pmorb[i].coeff);
                pChemicalSystem->pmorb[i].coeff=NULL;

                free(pChemicalSystem->pmorb[i].exp);
                pChemicalSystem->pmorb[i].exp=NULL;

                free(pChemicalSystem->pmorb[i].nucl);
                pChemicalSystem->pmorb[i].nucl=NULL;

                free(pChemicalSystem->pmorb[i].type);
                pChemicalSystem->pmorb[i].type=NULL;
             }
        }

        free(pChemicalSystem->pmorb);
        pChemicalSystem->pmorb=NULL;
    }

    return;
}

/* ************************************************************************** */
// The function getChemicalFormat determines if the file located at fileLocation
// exists and if its name ends by the *.chem extension or the *.wfn one, with a
// length (strictly) lower than the nameLength variable, and more than five (to
// be able to store at least something more than the *.chem or *.wfn extension).
// It has the char* fileLocation and int nameLength variables as input
// arguments, and it return zero if an error occurs, otherwise 1 (resp. -1)
// is returned for the *.chem (resp. *.wfn) format.
/* ************************************************************************** */
int getChemicalFormat(char* fileLocation, int nameLength)
{
    size_t length=0;
    int returnValue=0;

    // Check that nameLength is (strictly) greater than six
    if (nameLength<7)
    {
        PRINT_ERROR("In getChemicalFormat: the input nameLength ");
        fprintf(stderr,"(=%d) variable should be an integer ",nameLength);
        fprintf(stderr,"(strictly) greater than six.\n");
        return 0;
    }

    // Check if the file exists at fileLocation
    if (initialFileExists(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In getChemicalFormat: the initialFileExists function ");
        fprintf(stderr,"did not returned one, which was the expected value ");
        fprintf(stderr,"here.\n");
        return 0;
    }

    // Check that its size can at least contain the *.chem or *.wfn extension
    // strlen returns the length of the string, but not including the char '\0'
    length=strlen(fileLocation);
    if (length<6)
    {
        PRINT_ERROR("In getChemicalFormat: the input fileLocation variable ");
        fprintf(stderr,"(=%s) should have a length that at ",fileLocation);
        fprintf(stderr,"least equals six in order to end with something more ");
        fprintf(stderr,"than the '.chem' or '.wfn' extension.\n");
        return 0;
    }
    // Remark here: if the file is entitled 'A.wfn' the mpdProgram will fail

    // Distinguish the *.chem from *.wfn format
    if (fileLocation[length-4]=='.' && fileLocation[length-3]=='w' &&
                   fileLocation[length-2]=='f' && fileLocation[length-1]=='n' &&
                                                     fileLocation[length]=='\0')
    {
        returnValue=-1;
    }
    else if (fileLocation[length-5]=='.' && fileLocation[length-4]=='c' &&
                   fileLocation[length-3]=='h' && fileLocation[length-2]=='e' &&
                      fileLocation[length-1]=='m' && fileLocation[length]=='\0')
    {
        returnValue=1;
    }
    else
    {
        PRINT_ERROR("In getChemicalFormat: the input fileLocation variable ");
        fprintf(stderr,"(=%s) does not end with the '.chem' ",fileLocation);
        fprintf(stderr,"or '.wfn' extension.\n");
        returnValue=0;
    }

    return returnValue;
}

/* ************************************************************************** */
// The function readChemFileandAllocateChemicalSystem reads the file at
// fileLocation (file must exist with length (strictly) lower than nameLength),
// checks *.chem syntax, allocates memory, and fills the values in the variables
// of the structure pointed by pChemicalSystem. It has the char* fileLocation
// and ChemicalSystem* variable (defined in main.h) as input arguments and it
// returns zero if an error is encountered, one (respectively minus one) if the
// chemical data are successfully loaded and correspond to an restricted (resp.
// unrestricted) Hartree-Fock chemical structure
/* ************************************************************************** */
int readChemFileandAllocateChemicalSystem(char* fileLocation, int nameLength,
                                                ChemicalSystem* pChemicalSystem)
{
    size_t length=0;
    char readStringIn[33]={'\0'}, *readStringOut=NULL;
    int readIntegerIn=0, readIntegerOut=0, i=0, iMax=0, j=0, jMax=0;
    int returnValue=0;
    double readDouble=0.;
    MolecularOrbital *pMolecularOrbital=NULL, *pMolecularOrbital2=NULL;
    FILE *chemicalFile=NULL;

    // Check if the input pChemicalSystem variable is pointing to NULL
    if (pChemicalSystem==NULL)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: the input ");
        fprintf(stderr,"pChemicalSystem variable is pointing to the ");
        fprintf(stderr,"%p adress.\n",(void*)pChemicalSystem);
        return 0;
    }

    // Initialize the structure to zero for variables and NULL for pointers
    initializeChemicalStructure(pChemicalSystem);

    // Check that fileLocation points to a valid existing file
    if (getChemicalFormat(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: ");
        fprintf(stderr,"getChemicalFormat function did not returned one, ");
        fprintf(stderr,"which was the expected value here, after having ");
        fprintf(stderr,"checked that the char* input variable fileLocation ");
        fprintf(stderr,"does not point to the name of an existing and valid ");
        fprintf(stderr,"*.chem file.\n");
        return 0;
    }

    // Check if the *.chem file is well opened: fopen function returns a FILE
    // pointer on success, otherwise NULL is returned
    fprintf(stdout,"\nOpening %s file. ",fileLocation);
    chemicalFile=fopen(fileLocation,"r");
    if (chemicalFile==NULL)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: could not ");
        fprintf(stderr,"read the content of %s file.\n",fileLocation);
        return 0;
    }
    fprintf(stdout,"Reading chemical data. ");

    // fscanf function returns the number of input items successfully matched
    // and assigned, which can be fewer than provided for. Important remark:
    // white space characters, such as blanks and newline characters, causes
    // fscanf (scanf and sscanf) to read, but not to store, all consecutive
    // white space characters in the input up to the next character that is not
    // white space. One white space character in format-string matches any
    // combination of white space characters in the input
    fscanf(chemicalFile," ");

    // Read number of molecular orbitals and store it in pChemicalSystem->nmorb
    // fgets returns NULL if it fails, otherwise the same (char*) input address
    readStringOut=fgets(readStringIn,18,chemicalFile);
    readIntegerOut=fscanf(chemicalFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"of the fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the number of molecular orbitals.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (strcmp(readStringIn,"MolecularOrbitals") || readIntegerIn<1)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: expecting ");
        fprintf(stderr,"MolecularOrbitals Nmorb instead of %s ",readStringIn);
        fprintf(stderr,"%d (positive value).\n",readIntegerIn);
        closeTheFile(&chemicalFile);
        return 0;
    }
    pChemicalSystem->nmorb=readIntegerIn;

    // Allocate memory for each MolecularOrbital and check if it worked
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=readIntegerIn;
    pChemicalSystem->pmorb=(MolecularOrbital*)calloc(length,
                                                      sizeof(MolecularOrbital));
    if (pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: could not ");
        fprintf(stderr,"allocate memory for the (MolecularOrbital*) ");
        fprintf(stderr,"pChemicalSystem->pmorb variable.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    // Read number of primitives and store it in pChemicalSystem->ngauss
    readStringOut=fgets(readStringIn,11,chemicalFile);
    readIntegerOut=fscanf(chemicalFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"of the fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the number of primitives.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    if (strcmp(readStringIn,"Primitives") || readIntegerIn<1)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: expecting ");
        fprintf(stderr,"Primitives Ngauss instead of %s ",readStringIn);
        fprintf(stderr,"%d (positive value).\n",readIntegerIn);
        closeTheFile(&chemicalFile);
        return 0;
    }
    pChemicalSystem->ngauss=readIntegerIn;

    // Dynamically allocate the memory in each MolecularOrbital structure
    iMax=pChemicalSystem->nmorb;
    for (i=0; i<iMax; i++)
    {
        // Initializing to zero the (i+1)-th MolecularOrbital structure
        pMolecularOrbital=&pChemicalSystem->pmorb[i];
        pMolecularOrbital->spin=0;
        pMolecularOrbital->coeff=NULL;
        pMolecularOrbital->exp=NULL;
        pMolecularOrbital->nucl=NULL;
        pMolecularOrbital->type=NULL;

        // Dynamically allocate memory for primitives and check if it worked
        length=readIntegerIn;
        pMolecularOrbital->coeff=(double*)calloc(length,sizeof(double));
        pMolecularOrbital->exp=(double*)calloc(length,sizeof(double));
        pMolecularOrbital->nucl=(int*)calloc(length,sizeof(int));
        pMolecularOrbital->type=(int*)calloc(length,sizeof(int));
        if (pMolecularOrbital->coeff==NULL || pMolecularOrbital->exp==NULL ||
                 pMolecularOrbital->nucl==NULL || pMolecularOrbital->type==NULL)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: could not ");
            fprintf(stderr,"allocate memory for %d primitives ",readIntegerIn);
            fprintf(stderr,"in the %d-th MolecularOrbital structure.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }
    }

    // Read the number of nuclei and store it in pChemicalSystem->nnucl
    readStringOut=fgets(readStringIn,7,chemicalFile);
    readIntegerOut=fscanf(chemicalFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"of the fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the number of nuclei.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    if (strcmp(readStringIn,"Nuclei") || readIntegerIn<1)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: expecting ");
        fprintf(stderr,"Nuclei Nnucl instead of %s ",readStringIn);
        fprintf(stderr,"%d (positive value).\n",readIntegerIn);
        closeTheFile(&chemicalFile);
        return 0;
    }
    pChemicalSystem->nnucl=readIntegerIn;

    // Dynamically allocate memory for nuclei and check if it worked
    length=readIntegerIn;
    pChemicalSystem->pnucl=(Nucleus*)calloc(length,sizeof(Nucleus));
    if (pChemicalSystem->pnucl==NULL)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: could not ");
        fprintf(stderr,"allocate memory for the (Nucleus*) ");
        fprintf(stderr,"pChemicalSystem->pnucl variable.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    // Saving the Nucleus data in pMesh->pnucl and check if it worked
    iMax=readIntegerIn;
    for (i=0; i<iMax; i++)
    {
        j=0;
        j+=fscanf(chemicalFile,"%lf ",&readDouble);
        pChemicalSystem->pnucl[i].x=readDouble;

        j+=fscanf(chemicalFile,"%lf ",&readDouble);
        pChemicalSystem->pnucl[i].y=readDouble;

        j+=fscanf(chemicalFile,"%lf ",&readDouble);
        pChemicalSystem->pnucl[i].z=readDouble;

        if (j!=3)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: we were ");
            fprintf(stderr,"not able to properly save the three-coordinate ");
            fprintf(stderr,"positions of the %d-th Nucleus (only %d ",i+1,j);
            fprintf(stderr,"values properly saved instead of three).\n");
            closeTheFile(&chemicalFile);
            return 0;
        }

        readIntegerOut=fscanf(chemicalFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"(=%d) return of the fscanf ",readIntegerOut);
            fprintf(stderr,"function while attempting to read the charge of ");
            fprintf(stderr,"the %d-th Nucleus structure.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>100)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: the ");
            fprintf(stderr,"charge (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"Nucleus must be a positive integer that is not ");
            fprintf(stderr,"(strictly) greater than one hundred.\n");
            closeTheFile(&chemicalFile);
            return 0;
        }
        pChemicalSystem->pnucl[i].charge=readIntegerIn;
    }

    // Read the line containing the keywords Coefficient Exponent Center Type
    readStringOut=fgets(readStringIn,33,chemicalFile);
    if (readStringOut==NULL)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"%p of the fgets function ",(void*)readStringOut);
        fprintf(stderr,"while attempting to read the keywords Coefficient, ");
        fprintf(stderr,"Exponent, Center, and Type.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    if (strcmp(readStringIn,"Coefficient Exponent Center Type"))
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: expecting ");
        fprintf(stderr,"'Coefficient Exponent Center Type' instead of ");
        fprintf(stderr,"'%s' at this line.\n",readStringIn);
        closeTheFile(&chemicalFile);
        return 0;
    }

    // For each MocularOrbital, read and save successively the spin followed
    // by the enumeration of the gaussian coefficients, exponents, center
    // references, and associated types
    fscanf(chemicalFile," ");
    iMax=pChemicalSystem->nmorb;
    for (i=0; i<iMax; i++)
    {
        readIntegerOut=fscanf(chemicalFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"(=%d) return of the fscanf ",readIntegerOut);
            fprintf(stderr,"function while attempting to read the ");
            fprintf(stderr,"description of the %d-th MolecularOrbital ",i+1);
            fprintf(stderr,"structure.\n");
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (readIntegerIn!=i+1)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: expecting ");
            fprintf(stderr,"to read the %d-th MolecularOrbital structure ",i+1);
            fprintf(stderr,"instead of the %d-th one.\n",readIntegerIn);
            closeTheFile(&chemicalFile);
            return 0;
        }

        // Read and check the spin of the (i+1)-th MolecularOrbital structure
        readStringOut=fgets(readStringIn,5,chemicalFile);
        readIntegerOut=fscanf(chemicalFile," %d ",&readIntegerIn);
        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the fgets %p or ",(void*)readStringOut);
            fprintf(stderr,"fscanf (=%d) function while ",readIntegerOut);
            fprintf(stderr,"attempting to read the spin of the %d-th ",i+1);
            fprintf(stderr,"MolecularOrbital structure.\n");
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (strcmp(readStringIn,"Spin") || abs(readIntegerIn)!=1)
        {
            PRINT_ERROR("In readChemFileandAllocateChemicalSystem: expecting ");
            fprintf(stderr,"Spin (+/-)1 instead of %s ",readStringIn);
            fprintf(stderr,"%d (unitary value).\n",readIntegerIn);
            closeTheFile(&chemicalFile);
            return 0;
        }
        pChemicalSystem->pmorb[i].spin=readIntegerIn;

        // Read the next informations of the (i+1)-th MolecularOrbital structure
        jMax=pChemicalSystem->ngauss;
        pMolecularOrbital=&pChemicalSystem->pmorb[i];
        for (j=0; j<jMax; j++)
        {
            // Read coefficient
            readIntegerOut=fscanf(chemicalFile,"%lf ",&readDouble);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In readChemFileandAllocateChemicalSystem: ");
                fprintf(stderr,"wrong return (=%d) of the ",readIntegerOut);
                fprintf(stderr,"fscanf function while attempting to read the ");
                fprintf(stderr,"%d-th coefficient of the %d-th ",j+1,i+1);
                fprintf(stderr,"MolecularOrbital structure.\n");
                closeTheFile(&chemicalFile);
                return 0;
            }
            pMolecularOrbital->coeff[j]=readDouble;

            // Read and check exponent
            readIntegerOut=fscanf(chemicalFile,"%lf ",&readDouble);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In readChemFileandAllocateChemicalSystem: ");
                fprintf(stderr,"wrong return (=%d) of the ",readIntegerOut);
                fprintf(stderr,"fscanf function while attempting to read the ");
                fprintf(stderr,"%d-th Gaussian exponent of the %d-th ",j+1,i+1);
                fprintf(stderr,"MolecularOrbital structure.\n");
                closeTheFile(&chemicalFile);
                return 0;
            }

            if (readDouble<=0.)
            {
                PRINT_ERROR("In readChemFileandAllocateChemicalSystem: the ");
                fprintf(stderr,"%d-th Gaussian exponent of the %d-th ",j+1,i+1);
                fprintf(stderr,"MolecularOrbital structure should be a ");
                fprintf(stderr,"positive double value (instead of ");
                fprintf(stderr,"%lf).\n",readDouble);
                closeTheFile(&chemicalFile);
                return 0;
            }
            pMolecularOrbital->exp[j]=readDouble;

            // Read and check center assignments
            readIntegerOut=fscanf(chemicalFile,"%d ",&readIntegerIn);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In readChemFileandAllocateChemicalSystem: ");
                fprintf(stderr,"wrong return (=%d) of the ",readIntegerOut);
                fprintf(stderr,"fscanf function while attempting to read the ");
                fprintf(stderr,"%d-th center assignment of the %d-th ",j+1,i+1);
                fprintf(stderr,"MolecularOrbital structure.\n");
                closeTheFile(&chemicalFile);
                return 0;
            }

            if (readIntegerIn<1 || readIntegerIn>pChemicalSystem->nnucl)
            {
                PRINT_ERROR("In readChemFileandAllocateChemicalSystem: the ");
                fprintf(stderr,"%d-th center assignment of the %d-th ",j+1,i+1);
                fprintf(stderr,"MolecularOrbital structure should be a ");
                fprintf(stderr,"positive integer less than the total number ");
                fprintf(stderr,"of nuclei %d ",pChemicalSystem->nnucl);
                fprintf(stderr,"(instead of %d).\n",readIntegerIn);
                closeTheFile(&chemicalFile);
                return 0;
            }
            pMolecularOrbital->nucl[j]=readIntegerIn;

            // Read and check type assignments
            readIntegerOut=fscanf(chemicalFile,"%d ",&readIntegerIn);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In readChemFileandAllocateChemicalSystem: ");
                fprintf(stderr,"wrong return (=%d) of the ",readIntegerOut);
                fprintf(stderr,"fscanf function while attempting to read the ");
                fprintf(stderr,"%d-th type assignment of the %d-th ",j+1,i+1);
                fprintf(stderr,"MolecularOrbital structure.\n");
                closeTheFile(&chemicalFile);
                return 0;
            }

            if (readIntegerIn<1 || readIntegerIn>20)
            {
                PRINT_ERROR("In readChemFileandAllocateChemicalSystem: the ");
                fprintf(stderr,"%d-th type assignment of the %d-th ",j+1,i+1);
                fprintf(stderr,"MolecularOrbital structure should be a ");
                fprintf(stderr,"positive integer less than 20 (instead of ");
                fprintf(stderr,"%d).\n",readIntegerIn);
                closeTheFile(&chemicalFile);
                return 0;
            }
            pMolecularOrbital->type[j]=readIntegerIn;
        }
    }

    // Check that the "End" keyword is ending the file (everything after that
    // is considered as a comment)
    readStringOut=fgets(readStringIn,4,chemicalFile);
    if (readStringOut==NULL)
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"%p of the fgets function ",(void*)readStringOut);
        fprintf(stderr,"while attempting to read the End keyword.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    if (strcmp(readStringIn,"End"))
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: expecting ");
        fprintf(stderr,"the 'End' keyword instead of '%s' at ",readStringIn);
        fprintf(stderr,"this line.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    // Closing the *.chem file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(chemicalFile))
    {
        PRINT_ERROR("In readChemFileandAllocateChemicalSystem: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        chemicalFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    chemicalFile=NULL;

    // Check if the loaded chemical data are restricted Hartree-Fock ones or not
    returnValue=1;
    if (pChemicalSystem->nmorb%2)
    {
        returnValue=-1;
    }
    else
    {
        j=0;
        iMax=pChemicalSystem->nmorb;
        for (i=0; i<iMax; i++)
        {
            if (pChemicalSystem->pmorb[i].spin==1)
            {
                j++;
            }
        }

        iMax=pChemicalSystem->nmorb/2;
        if (j!=iMax)
        {
            returnValue=-1;
        }
        else
        {
            jMax=pChemicalSystem->ngauss;
            for (i=0; i<iMax; i++)
            {
                pMolecularOrbital=&pChemicalSystem->pmorb[i];
                pMolecularOrbital2=&pChemicalSystem->pmorb[iMax+i];
                for (j=0; j<jMax; j++)
                {
                    if (pMolecularOrbital->coeff[j]!=
                                                   pMolecularOrbital2->coeff[j])
                    {
                        returnValue=-1;
                        break;
                    }
                    else if (pMolecularOrbital->exp[j]!=
                                                     pMolecularOrbital2->exp[j])
                    {
                        returnValue=-1;
                        break;
                    }
                    else if (pMolecularOrbital->nucl[j]!=
                                                    pMolecularOrbital2->nucl[j])
                    {
                        returnValue=-1;
                        break;
                    }
                    else if (pMolecularOrbital->type[j]!=
                                                    pMolecularOrbital2->type[j])
                    {
                        returnValue=-1;
                        break;
                    }
                }
                if (returnValue==-1)
                {
                    break;
                }
            }
        }
    }

    return returnValue;
}

/* ************************************************************************** */
// The function readAndConvertWfnFile reads a *.temp file (which is intended
// to be the copy of a *.wfn file) at fileLocation (such a file must have been
// previously created), except the first line, and converts all charToRemove
// encountered into charToPutInstead. It has a char* variable (fileLocation)
// and two char variables (charToRemove, charToPutInstead) as input arguments
// and it returns zero if an error occurred, otherwise one in case of success
/* ************************************************************************** */
int readAndConvertWfnFile(char* fileLocation, char charToRemove,
                                          char charToPutInstead, int nameLength)
{
    int readChar=0;
    FILE *wfnFile=NULL;

    // Check if the file have been previously created
    if (initialFileExists(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In readAndConvertWfnFile: initialFileExists function ");
        fprintf(stderr,"did not return one, which was the expected value ");
        fprintf(stderr,"here, after having checked that the (char*) ");
        fprintf(stderr,"fileLocation variable is not storing the name of an ");
        fprintf(stderr,"existing and valid wfn-format file.\n");
        return 0;
    }

    // Check if the file is well opened (file must have been previously created)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    fprintf(stdout,"Opening %s file. ",fileLocation);
    wfnFile=fopen(fileLocation,"r+");
    if (wfnFile==NULL)
    {
        PRINT_ERROR("In readAndConvertWfnFile: could not open and write into ");
        fprintf(stderr,"the %s file.\n",fileLocation);
        return 0;
    }
    fprintf(stdout,"Converting data (any '%c' ",charToRemove);
    fprintf(stdout,"into '%c'). ",charToPutInstead);

    // Skip the first line: fgetc function returns the character read as an
    // unsigned char cast to an int or EOF on the end of file or error (because
    // EOF can't be stored as a char)
    do {
        readChar=fgetc(wfnFile);
    } while (readChar!='\n' && readChar!=EOF);
    if (readChar==EOF)
    {
        PRINT_ERROR("In readAndConvertWfnFile: problem encountered with the ");
        fprintf(stderr,"fgetc function or end-of-file reached without any ");
        fprintf(stderr,"data written in the %s file.\n",fileLocation);
        closeTheFile(&wfnFile);
        return 0;
    }

    // Remark: the 'D' symbol now only appears in *.wfn at fileLocation either
    // for scientific notation (Fortran 1.0D+01) or for the END DATA keyword so
    // it can now be easily converted into the C syntax (1.0e+01)
    do {
        readChar=fgetc(wfnFile);
        if (readChar==(int)charToRemove)
        {
            // fseek returns zero on success, otherwise a non-zero value
            if (fseek(wfnFile,-1,SEEK_CUR))
            {
                PRINT_ERROR("In readAndConvertWfnFile: wrong return of the ");
                fprintf(stderr,"fseek function in the %s file.\n",fileLocation);
                closeTheFile(&wfnFile);
                return 0;
            }

            // fputc returns the same character on success, otherwise EOF value
            if (fputc(charToPutInstead,wfnFile)==EOF)
            {
                PRINT_ERROR("In readAndConvertWfnFile: wrong return of the ");
                fprintf(stderr,"fputc function in the %s file.\n",fileLocation);
                closeTheFile(&wfnFile);
                return 0;
            }
        }
    } while (readChar!=EOF);

    // Testing if the file is well closed: close returns zero if the input
    // FILE* variable is successfully closed, otherwise EOF is returned
    if (fclose(wfnFile))
    {
        PRINT_ERROR("In readAndConvertWfnFile: we were not able to close the ");
        fprintf(stderr,"%s file properly.\n",fileLocation);
        wfnFile=NULL;
        return 0;
    }
    wfnFile=NULL;
    fprintf(stdout,"Closing file.");

    return 1;
}

/* ************************************************************************** */
// The function readWfnFileAndAllocateChemicalSystem reads the file at
// fileLocation (file must exist with length (strictly) lower than nameLength),
// checks *.wfn syntax, allocates memory, and fills the values in the variables
// of the structure pointed by pChemicalSystem. It has the char* fileLocation
// and ChemicalSystem* variable (defined in main.h) as input arguments and it
// returns zero if an error is encountered, one (respectively minus one) if the
// chemical data are successfully loaded and correspond to an restricted (resp.
// unrestricted) Hartree-Fock chemical structure
/* ************************************************************************** */
int readWfnFileAndAllocateChemicalSystem(char* fileLocation, int nameLength,
                                                ChemicalSystem* pChemicalSystem)
{
    size_t length=0;
    char readStringIn[18]={'\0'}, *readStringOut=NULL;
    int readIntegerIn=0, readIntegerOut=0, readChar=0, i=0, iMax=0, j=0, jMax=0;
    int returnValue=0, deltaSpin=0;
    double readDouble=0.;
    MolecularOrbital *pMolecularOrbital=NULL, *pMolecularOrbitalI=NULL;
    FILE *wfnFile=NULL;

    // Check if the input pChemicalSystem variable is pointing to NULL
    if (pChemicalSystem==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: the input ");
        fprintf(stderr,"pChemicalSystem variable is pointing to the ");
        fprintf(stderr,"%p adress.\n",(void*)pChemicalSystem);
        return 0;
    }

    // Initialize the structure to zero for variables and NULL for pointers
    initializeChemicalStructure(pChemicalSystem);

   // Check that nameLength is (strictly) greater than six
    if (nameLength<7)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: the input ");
        fprintf(stderr,"nameLength (=%d) variable should be an ",nameLength);
        fprintf(stderr,"integer (strictly) greater than six.\n");
        return 0;
    }

    // Check that fileLocation points to a valid existing file
    if (initialFileExists(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: the ");
        fprintf(stderr,"initialFileExists function did not returned one, ");
        fprintf(stderr,"which was the expected value here.\n");
        return 0;
    }

    // Check that its size can at least contain the *.chem or *.wfn extension
    // strlen returns the length of the string, but not including the char '\0'
    length=strlen(fileLocation);
    if (length<6)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: the input ");
        fprintf(stderr,"fileLocation variable (=%s) should have ",fileLocation);
        fprintf(stderr,"a length that at least equals six in order to end ");
        fprintf(stderr,"with something more than the '.temp' extension.\n");
        return 0;
    }

    // Check that fileLocation ends with the '.temp' extension
    if (fileLocation[length-5]!='.' || fileLocation[length-4]!='t' ||
                   fileLocation[length-3]!='e' || fileLocation[length-2]!='m' ||
                      fileLocation[length-1]!='p' || fileLocation[length]!='\0')
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: the input ");
        fprintf(stderr,"fileLocation variable (=%s) does not ",fileLocation);
        fprintf(stderr,"end with the '.temp' extension.\n");
        return 0;
    }

    // Check if the *.temp file is well opened: fopen function returns a FILE
    // pointer on success, otherwise NULL is returned
    fprintf(stdout,"\nOpening %s file. ",fileLocation);
    wfnFile=fopen(fileLocation,"r");
    if (wfnFile==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: could not ");
        fprintf(stderr,"read the content of %s file.\n",fileLocation);
        return 0;
    }
    fprintf(stdout,"Reading and saving chemical data. ");

    // Skip the first line of the (copy *.temp of the) *.wfn file: fgetc returns
    // the character read as an unsigned char cast to an int or EOF on the end
    // of file or error (because EOF can't be stored as a char)
    do {
        readChar=fgetc(wfnFile);
    } while (readChar!='\n' && readChar!=EOF);
    if (readChar==EOF)
    {
        PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: problem ");
        fprintf(stderr,"encountered with the (standard) fgetc c-function or ");
        fprintf(stderr,"end-of-file reached without any data read in ");
        fprintf(stderr,"%s file.\n",fileLocation);
        closeTheFile(&wfnFile);
        return 0;
    }

    // Read the expected word GAUSSIAN in the second line
    // fgets returns NULL if it fails, otherwise the same (char*) input address
    readStringOut=fgets(readStringIn,9,wfnFile);
    if (readStringOut==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"%p of the (standard) fgets ",(void*)readStringOut);
        fprintf(stderr,"c-function while attempting to read the GAUSSIAN ");
        fprintf(stderr,"keyword.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (strcmp(readStringIn,"GAUSSIAN"))
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: expecting ");
        fprintf(stderr,"the keyword GAUSSIAN instead of %s.\n",readStringIn);
        closeTheFile(&wfnFile);
        return 0;
    }

    // Read number of molecular orbitals and store it in pChemicalSystem->nmorb
    // fscanf function returns the number of input items successfully matched
    // and assigned, which can be fewer than provided for. Important remark:
    // white space characters, such as blanks and newline characters, causes
    // fscanf (scanf and sscanf) to read, but not to store, all consecutive
    // white space characters in the input up to the next character that is not
    // white space. One white space character in format-string matches any
    // combination of white space characters in the input
    readIntegerOut=fscanf(wfnFile," %d ",&readIntegerIn);
    readStringOut=fgets(readStringIn,13,wfnFile);

    if (readIntegerOut!=1 || readStringOut==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"of the (standard) fscanf (=%d) or ",readIntegerOut);
        fprintf(stderr,"fgets %p c-function while ",(void*)readStringOut);
        fprintf(stderr,"attempting to read the number of molecular ");
        fprintf(stderr,"orbitals.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    if (readIntegerIn<1 || strcmp(readStringIn,"MOL ORBITALS"))
    {
        PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
        fprintf(stderr,"Nmorb MOL ORBITALS instead of %d ",readIntegerIn);
        fprintf(stderr,"%s (positive value).\n",readStringIn);
        closeTheFile(&wfnFile);
        return 0;
    }
    pChemicalSystem->nmorb=readIntegerIn;

    // Allocate memory for each MolecularOrbital and check if it worked
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=readIntegerIn;
    pChemicalSystem->pmorb=(MolecularOrbital*)calloc(length,
                                                      sizeof(MolecularOrbital));
    if (pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: could not ");
        fprintf(stderr,"allocate memory for the (MolecularOrbital*) ");
        fprintf(stderr,"pChemicalSystem->pmorb variable.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    // Read the number of primitives and store it in pChemicalSystem->ngauss
    readIntegerOut=fscanf(wfnFile," %d ",&readIntegerIn);
    readStringOut=fgets(readStringIn,11,wfnFile);

    if (readIntegerOut!=1 || readStringOut==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"of the (standard) fscanf (=%d) or ",readIntegerOut);
        fprintf(stderr,"fgets %p c-function while ",(void*)readStringOut);
        fprintf(stderr,"attempting to read the number of primitives.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    if (readIntegerIn<1 || strcmp(readStringIn,"PRIMITIVES"))
    {
        PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
        fprintf(stderr,"Ngauss PRIMITIVES instead of %d ",readIntegerIn);
        fprintf(stderr,"%s (positive value).\n",readStringIn);
        closeTheFile(&wfnFile);
        return 0;
    }
    pChemicalSystem->ngauss=readIntegerIn;

    // Dynamically allocate the memory in each MolecularOrbital structure
    iMax=pChemicalSystem->nmorb;
    for (i=0; i<iMax; i++)
    {
        // Initializing to zero the (i+1)-th MolecularOrbital structure
        pMolecularOrbital=&pChemicalSystem->pmorb[i];
        pMolecularOrbital->spin=0;
        pMolecularOrbital->coeff=NULL;
        pMolecularOrbital->exp=NULL;
        pMolecularOrbital->nucl=NULL;
        pMolecularOrbital->type=NULL;

        // Dynamically allocate memory for primitives and check if it worked
        length=readIntegerIn;
        pMolecularOrbital->coeff=(double*)calloc(length,sizeof(double));
        pMolecularOrbital->exp=(double*)calloc(length,sizeof(double));
        pMolecularOrbital->nucl=(int*)calloc(length,sizeof(int));
        pMolecularOrbital->type=(int*)calloc(length,sizeof(int));
        if (pMolecularOrbital->coeff==NULL || pMolecularOrbital->exp==NULL ||
                 pMolecularOrbital->nucl==NULL || pMolecularOrbital->type==NULL)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: could not ");
            fprintf(stderr,"allocate memory for %d primitives ",readIntegerIn);
            fprintf(stderr,"in the %d-th MolecularOrbital structure.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }
    }

    // Read the number of nuclei and store it in pChemicalSystem->nnucl
    readIntegerOut=fscanf(wfnFile," %d ",&readIntegerIn);
    readStringOut=fgets(readStringIn,7,wfnFile);

    if (readIntegerOut!=1 || readStringOut==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"of the (standard) fscanf (=%d) or ",readIntegerOut);
        fprintf(stderr,"fgets %p c-function while ",(void*)readStringOut);
        fprintf(stderr,"attempting to read the number of nuclei.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    if (readIntegerIn<1 || strcmp(readStringIn,"NUCLEI"))
    {
        PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
        fprintf(stderr,"Nnucl NUCLEI instead of %d ",readIntegerIn);
        fprintf(stderr,"%s (positive value).\n",readStringIn);
        closeTheFile(&wfnFile);
        return 0;
    }
    pChemicalSystem->nnucl=readIntegerIn;

    // Dynamically allocate memory for nuclei and check if it worked
    length=readIntegerIn;
    pChemicalSystem->pnucl=(Nucleus*)calloc(length,sizeof(Nucleus));
    if (pChemicalSystem->pnucl==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: could not ");
        fprintf(stderr,"allocate memory for the (Nucleus*) ");
        fprintf(stderr,"pChemicalSystem->pnucl variable.\n");
        closeTheFile(&wfnFile);
        return 0;
    }


    // Saving the Nucleus data in pMesh->pnucl and check if it worked
    fscanf(wfnFile," ");
    iMax=readIntegerIn;
    for (i=0; i<iMax; i++)
    {
        // Read the name of the molecule and check its number
        readStringOut=fgets(readStringIn,3,wfnFile);
        readIntegerOut=fscanf(wfnFile," %d (",&readIntegerIn);

        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fgets ");
            fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the symbol of the %d-th Nucleus.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (readIntegerIn!=i+1)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"%s %d instead of ",readStringIn,i+1);
            fprintf(stderr,"%s %d while scanning ",readStringIn,readIntegerIn);
            fprintf(stderr,"the %d-th Nucleus.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        // Read the centre reference and store its number in readIntegerIn
        readStringOut=fgets(readStringIn,7,wfnFile);
        readIntegerOut=fscanf(wfnFile," %d) ",&readIntegerIn);

        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fgets ");
            fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the center reference of the ");
            fprintf(stderr,"%d-th Nucleus.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (strcmp(readStringIn,"CENTRE") || readIntegerIn<1 ||
                                                             readIntegerIn>iMax)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"%s %d instead of ",readStringIn,i+1);
            fprintf(stderr,"%s %d (positive value ",readStringIn,readIntegerIn);
            fprintf(stderr,"strictly less than %d), while scanning the ",iMax);
            fprintf(stderr,"number of the center related to the ");
            fprintf(stderr,"%d-th Nucleus.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (readIntegerIn!=i+1)
        {
            fprintf(stdout,"\nWarning in ");
            fprintf(stdout,"readWfnFileAndAllocateChemicalSystem function: ");
            fprintf(stdout,"the Nucleus data are not defined in increasing ");
            fprintf(stdout,"order (the %d-th Nucleus data are numbered ",i+1);
            fprintf(stdout,"as the %d-th one).\n",readIntegerIn);
        }

        j=0;
        j+=fscanf(wfnFile,"%lf ",&readDouble);
        pChemicalSystem->pnucl[readIntegerIn-1].x=readDouble;

        j+=fscanf(wfnFile,"%lf ",&readDouble);
        pChemicalSystem->pnucl[readIntegerIn-1].y=readDouble;

        j+=fscanf(wfnFile,"%lf ",&readDouble);
        pChemicalSystem->pnucl[readIntegerIn-1].z=readDouble;

        if (j!=3)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: we were ");
            fprintf(stderr,"not able to properly save the three-coordinate ");
            fprintf(stderr,"positions of the %d-th Nucleus ",readIntegerIn);
            fprintf(stderr,"(only %d values properly saved instead of 3).\n",j);
            closeTheFile(&wfnFile);
            return 0;
        }

        // Read the charge number of the current nucleus
        readStringOut=fgets(readStringIn,7,wfnFile);
        readIntegerOut=fscanf(wfnFile," = %lf ",&readDouble);

        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fgets ");
            fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the charge number associated with the ");
            fprintf(stderr,"%d-th Nucleus.\n",readIntegerIn);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (strcmp(readStringIn,"CHARGE") || readDouble>100. || readDouble<1.)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: the ");
            fprintf(stderr,"charge number (=%lf) associated with ",readDouble);
            fprintf(stderr,"the %d-th Nucleus must be a ",readIntegerIn);
            fprintf(stderr,"positive value that is not (strictly) greater ");
            fprintf(stderr,"one hundred and should represent an integer.\n");
            closeTheFile(&wfnFile);
            return 0;
        }
        pChemicalSystem->pnucl[readIntegerIn-1].charge=(int)readDouble;
    }

    // Save the centre assignments in pChemicalSystem->pmorb[0].nucl and copy
    // them in all pChemicalSystem->pmorb[i].nucl i=1..pChemicalSystem->nmorb-1
    pMolecularOrbital=&pChemicalSystem->pmorb[0];
    iMax=pChemicalSystem->nnucl;
    jMax=pChemicalSystem->ngauss;
    for (j=0; j<jMax; j++)
    {
        readChar=fgetc(wfnFile);
        if (readChar==EOF)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: problem ");
            fprintf(stderr,"encountered with the (standard) fgetc c-function ");
            fprintf(stderr,"or end-of-file reached without all the data ");
            fprintf(stderr,"saved for the %d-th Centre Assignment.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }
        else if (readChar=='C')
        {
            readStringOut=fgets(readStringIn,18,wfnFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
                fprintf(stderr,"return of the (standard) fgets ");
                fprintf(stderr,"%p c-function while ",(void*)readStringOut);
                fprintf(stderr,"attempting to read the %d-th Center ",j+1);
                fprintf(stderr,"Assignment.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
            if (strcmp(readStringIn,"ENTRE ASSIGNMENTS"))
            {
                PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: ");
                fprintf(stderr,"expecting CENTRE ASSIGNMENTS instead of ");
                fprintf(stderr,"C%s while reading the %d-th ",readStringIn,j+1);
                fprintf(stderr,"one.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
            fscanf(wfnFile," ");
        }
        else
        {
            // fseek returns zero on success, otherwise a non-zero value
            if (fseek(wfnFile,-1,SEEK_CUR))
            {
                PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: wrong ");
                fprintf(stderr,"return of the (standard) fseek c-function ");
                fprintf(stderr,"while reading the %d-th Centre ",j+1);
                fprintf(stderr,"Assignment.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
        }

        readIntegerOut=fscanf(wfnFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fscanf ");
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the %d-th Center Assignment.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>iMax)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"a positive integer that is not (strictly) ");
            fprintf(stderr,"greater than the total number of nuclei %d ",iMax);
            fprintf(stderr,"instead of %d while reading the ",readIntegerIn);
            fprintf(stderr,"%d-th Centre Assignment.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }
        pMolecularOrbital->nucl[j]=readIntegerIn;
    }

    iMax=pChemicalSystem->nmorb;
    if (iMax>1)
    {
        for (i=1; i<iMax; i++)
        {
            pMolecularOrbitalI=&pChemicalSystem->pmorb[i];
            for (j=0; j<jMax; j++)
            {
                pMolecularOrbitalI->nucl[j]=pMolecularOrbital->nucl[j];
            }
        }
    }

    // Save the type assignments in pChemicalSystem->pmorb[0].type and copy them
    // in all pChemicalSystem->pmorb[i].type i=1..pChemicalSystem->nmorb-1
    pMolecularOrbital=&pChemicalSystem->pmorb[0];
    jMax=pChemicalSystem->ngauss;
    for (j=0; j<jMax; j++)
    {
        readChar=fgetc(wfnFile);
        if (readChar==EOF)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: problem ");
            fprintf(stderr,"encountered with the (standard) fgetc c-function ");
            fprintf(stderr,"or end-of-file reached without all the data ");
            fprintf(stderr,"saved for the %d-th Type Assignment.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }
        else if (readChar=='T')
        {
            readStringOut=fgets(readStringIn,16,wfnFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
                fprintf(stderr,"return of the (standard) fgets ");
                fprintf(stderr,"%p c-function while ",(void*)readStringOut);
                fprintf(stderr,"attempting to read the %d-th Type ",j+1);
                fprintf(stderr,"Assignment.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
            if (strcmp(readStringIn,"YPE ASSIGNMENTS"))
            {
                PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: ");
                fprintf(stderr,"expecting TYPE ASSIGNMENTS instead of ");
                fprintf(stderr,"T%s while reading the %d-th ",readStringIn,j+1);
                fprintf(stderr,"one.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
            fscanf(wfnFile," ");
        }
        else
        {
            // fseek returns zero on success, otherwise a non-zero value
            if (fseek(wfnFile,-1,SEEK_CUR))
            {
                PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: wrong ");
                fprintf(stderr,"return of the (standard) fseek c-function ");
                fprintf(stderr,"while reading the %d-th Type ",j+1);
                fprintf(stderr,"Assignment.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
        }

        readIntegerOut=fscanf(wfnFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fscanf ");
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the %d-th Type Assignment.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>20)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"a positive integer that is not (strictly) ");
            fprintf(stderr,"greater than twenty instead of %d ",readIntegerIn);
            fprintf(stderr,"while reading the %d-th Centre Assignment.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }
        pMolecularOrbital->type[j]=readIntegerIn;
    }

    iMax=pChemicalSystem->nmorb;
    if (iMax>1)
    {
        for (i=1; i<iMax; i++)
        {
            pMolecularOrbitalI=&pChemicalSystem->pmorb[i];
            for (j=0; j<jMax; j++)
            {
                pMolecularOrbitalI->type[j]=pMolecularOrbital->type[j];
            }
        }
    }

    // Save the exponents in pChemicalSystem->pmorb[0].exp and copy them in all
    // pChemicalSystem->pmorb[i].exp i=1..pChemicalSystem->nmorb-1
    pMolecularOrbital=&pChemicalSystem->pmorb[0];
    jMax=pChemicalSystem->ngauss;
    for (j=0; j<jMax; j++)
    {
        readChar=fgetc(wfnFile);
        if (readChar==EOF)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: problem ");
            fprintf(stderr,"encountered with the (standard) fgetc c-function ");
            fprintf(stderr,"or end-of-file reached without all the data ");
            fprintf(stderr,"saved for the %d-th Gaussian Exponent.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }
        else if (readChar=='E')
        {
            readStringOut=fgets(readStringIn,9,wfnFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
                fprintf(stderr,"return of the (standard) fgets ");
                fprintf(stderr,"%p c-function while ",(void*)readStringOut);
                fprintf(stderr,"attempting to read the %d-th Gaussian",j+1);
                fprintf(stderr,"Exponent.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
            if (strcmp(readStringIn,"XPONENTS"))
            {
                PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: ");
                fprintf(stderr,"expecting EXPONENTS instead of ");
                fprintf(stderr,"E%s while reading the %d-th ",readStringIn,j+1);
                fprintf(stderr,"Gaussian one.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
            fscanf(wfnFile," ");
        }
        else
        {
            // fseek returns zero on success, otherwise a non-zero value
            if (fseek(wfnFile,-1,SEEK_CUR))
            {
                PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: wrong ");
                fprintf(stderr,"return of the (standard) fseek c-function ");
                fprintf(stderr,"while reading the %d-th Gaussian ",j+1);
                fprintf(stderr,"Exponent.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
        }

        readIntegerOut=fscanf(wfnFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fscanf ");
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the %d-th Gaussian Exponent.\n",j+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (readDouble<=.0)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"a positive (double) value instead of ");
            fprintf(stderr,"%lf while reading the %d-th ",readDouble,j+1);
            fprintf(stderr,"Gaussian exponent.\n");
            closeTheFile(&wfnFile);
            return 0;
        }
        pMolecularOrbital->exp[j]=readDouble;
    }

    iMax=pChemicalSystem->nmorb;
    if (iMax>1)
    {
        for (i=1; i<iMax; i++)
        {
            pMolecularOrbitalI=&pChemicalSystem->pmorb[i];
            for (j=0; j<jMax; j++)
            {
                pMolecularOrbitalI->exp[j]=pMolecularOrbital->exp[j];
            }
        }
    }

    // Save in pChemicalSystem->pmorb[i].coeff i=0..pChemicalSystem->nmorb-1
    // the different primitives coefficients related to the different orbitals
    deltaSpin=0;
    iMax=pChemicalSystem->nmorb;
    jMax=pChemicalSystem->ngauss;
    for (i=0; i<iMax; i++)
    {
        // Read the first line associated with the i-th orbital
        readStringOut=fgets(readStringIn,3,wfnFile);
        readIntegerOut=fscanf(wfnFile," %d ",&readIntegerIn);

        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fgets ");
            fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the primitive coefficients associated ");
            fprintf(stderr,"with the %d-th MolecularOrbital structure.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (strcmp(readStringIn,"MO"))
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"the MO keyword instead of the %s ",readStringIn);
            fprintf(stderr,"one, while attempting to read the primitive ");
            fprintf(stderr,"coefficients of the %d-th MolecularOrbital ",i+1);
            fprintf(stderr,"structure.\n");
            closeTheFile(&wfnFile);
            return 0;
        }

        // Update (only one time allowed) deltaSpin if a gap in MO if observed
        // It happens for UHF wave functions to indicate spin in MO enumeration
        if (readIntegerIn!=i+1 && !deltaSpin)
        {
            deltaSpin=readIntegerIn-i-1;
        }

        // Check the readIntegerIn(-deltaSpin) variable
        if (readIntegerIn-deltaSpin!=i+1)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"MO %d instead of %d, ",i+1+deltaSpin,readIntegerIn);
            fprintf(stderr,"while attempting to read the primitive ");
            fprintf(stderr,"coefficients of the %d-th MolecularOrbital ",i+1);
            fprintf(stderr,"structure.\n");
            closeTheFile(&wfnFile);
            return 0;
        }

        readStringOut=fgets(readStringIn,3,wfnFile);
        readIntegerOut=fscanf(wfnFile," %lf ",&readDouble);

        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fgets ");
            fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the primitive coefficients associated ");
            fprintf(stderr,"with the %d-th MolecularOrbital structure.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (strcmp(readStringIn,"MO") || readDouble!=0.)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"MO 0.0 instead of %s %lf ",readStringIn,readDouble);
            fprintf(stderr,"while attempting to read the primitive ");
            fprintf(stderr,"coefficients of the %d-th MolecularOrbital ",i+1);
            fprintf(stderr,"structure.\n");
            closeTheFile(&wfnFile);
            return 0;
        }

        readStringOut=fgets(readStringIn,7,wfnFile);
        readIntegerOut=fscanf(wfnFile," = %lf ",&readDouble);

        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fgets ");
            fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the occupation number associated with ");
            fprintf(stderr,"the %d-th MolecularOrbital structure.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (strcmp(readStringIn,"OCC NO") || (readDouble!=1. && readDouble!=2.))
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"OCC NO = 1.0000000 or OCC NO = 2.0000000 instead ");
            fprintf(stderr,"of %s %lf while ",readStringIn,readDouble);
            fprintf(stderr,"attempting to read the occupation number ");
            fprintf(stderr,"associated with the %d-th ",readIntegerIn);
            fprintf(stderr,"MolecularOrbital structure.\n");
            closeTheFile(&wfnFile);
            return 0;
        }

        // Temporarily save the occupation number of the (readInteger)-th
        // molecular orbital in ChemicalSystem->pmorb[readIntegerIn-1].spin in
        // order to potentially duplicate the molecular orbital (2 spins) later
        pMolecularOrbitalI=&pChemicalSystem->pmorb[i];
        if (readDouble==2.)
        {
            pMolecularOrbitalI->spin=2;
        }
        else
        {
            if (deltaSpin)
            {
                pMolecularOrbitalI->spin=-1;
            }
            else
            {
                pMolecularOrbitalI->spin=1;
            }
        }

        readStringOut=fgets(readStringIn,12,wfnFile);
        readIntegerOut=fscanf(wfnFile," = %lf ",&readDouble);

        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
            fprintf(stderr,"return of the (standard) fgets ");
            fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-function while attempting ",readIntegerOut);
            fprintf(stderr,"to read the energy associated with the %d-th ",i+1);
            fprintf(stderr,"MolecularOrbital structure.\n");
            closeTheFile(&wfnFile);
            return 0;
        }

        // Remarks: in the *.wfn format the molecular orbitals are written in
        // order of increasing MO energy (i.e. most negative to least negative)
        // Molecular orbitals should be written in the order: doubly occupied,
        // singly occupied Alpha, singly occupied Beta
        if (strcmp(readStringIn,"ORB. ENERGY") || readDouble>0.)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
            fprintf(stderr,"ORB. ENERGY = -(double) instead of ");
            fprintf(stderr,"%s %lf (non-positive ",readStringIn,readDouble);
            fprintf(stderr,"value) while attempting to read the energy ");
            fprintf(stderr,"associated with the %d-th ",readIntegerIn);
            fprintf(stderr,"MolecularOrbital structure.\n");
            closeTheFile(&wfnFile);
            return 0;
        }

        // Save the primitive coefficients in pMolecularOrbitalI->coeff[j-1]
        // j=1...pChemicalSystem->ngauss
        for (j=0; j<jMax; j++)
        {
            readIntegerOut=fscanf(wfnFile,"%lf ",&readDouble);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong ");
                fprintf(stderr,"return of the (standard) fscanf ");
                fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                fprintf(stderr,"attempting to read the %d-th primitive ",j+1);
                fprintf(stderr,"coefficient of the %d-th ",i+1);
                fprintf(stderr,"MolecularOrbital structure.\n");
                closeTheFile(&wfnFile);
                return 0;
            }
            pMolecularOrbitalI->coeff[j]=readDouble;
        }
    }

    // Check that the END DATA keyword follows (transformed into ENe eATA here
    // by the readAndConvertWfnFile function so as to read scientific notation)
    readStringOut=fgets(readStringIn,9,wfnFile);
    if (readStringOut==NULL)
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: wrong return ");
        fprintf(stderr,"of the (standard) fgets %p ",(void*)readStringOut);
        fprintf(stderr,"c-function while attempting to read the END DATA ");
        fprintf(stderr,"keyword.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    if (strcmp(readStringIn,"ENe eATA"))
    {
        PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: expecting ");
        fprintf(stderr,"ENe eATA instead of %s while attempting ",readStringIn);
        fprintf(stderr,"to read the END DATA keyword.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    // Closing the (copy *.temp of the) *.wfn file: fclose returns zero if the
    // input FILE* variable is successfully closed, otherwise EOF (end-of-file)
    if (fclose(wfnFile))
    {
        PRINT_ERROR("In readWfnFileandAllocateChemicalSystem: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        wfnFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    wfnFile=NULL;

    // Check the number of molecular orbitals that have a spin number equal to
    // two, and duplicate them in the pChemicalSystem structure
    returnValue=1;
    readIntegerIn=0;
    iMax=pChemicalSystem->nmorb;
    for (i=0; i<iMax; i++)
    {
        if (pChemicalSystem->pmorb[i].spin==2)
        {
            readIntegerIn++;
        }
        else
        {
            returnValue=-1;
        }
    }

    // Allocating memory for extra molecular orbitals and checks if it worked
    // realloc returns a pointer to the newly allocated memory, otherwise NULL
    if (readIntegerIn>0)
    {
        length=iMax+readIntegerIn;
        pMolecularOrbital=(MolecularOrbital*)realloc(pChemicalSystem->pmorb,
                                               length*sizeof(MolecularOrbital));
        if (pMolecularOrbital==NULL)
        {
            PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: could not ");
            fprintf(stderr,"reallocate memory for %d ",readIntegerIn+iMax);
            fprintf(stderr,"MolecularOrbital structures.\n");
            return 0;
        }
        pChemicalSystem->pmorb=pMolecularOrbital;

        // Update the new number of molecular orbitals
        pChemicalSystem->nmorb=iMax+readIntegerIn;
        readIntegerIn=iMax-1;

        // Duplicate the molecular orbitals with an occupation number of two
        iMax=pChemicalSystem->nmorb;
        jMax=pChemicalSystem->ngauss;
        for (i=0; i<iMax; i++)
        {
            pMolecularOrbitalI=&pChemicalSystem->pmorb[i];
            if (pMolecularOrbitalI->spin==2)
            {
                pMolecularOrbitalI->spin=1;
                readIntegerIn++;

                // Initializing MolecularOrbital structure
                pMolecularOrbital=&pChemicalSystem->pmorb[readIntegerIn];
                pMolecularOrbital->spin=0;
                pMolecularOrbital->coeff=NULL;
                pMolecularOrbital->exp=NULL;
                pMolecularOrbital->nucl=NULL;
                pMolecularOrbital->type=NULL;

                // Change the spin of the duplicated MolecularOrbital structure
                pMolecularOrbital->spin=-1;

                // Allocating memory for primitives
                pMolecularOrbital->coeff=(double*)calloc(jMax,sizeof(double));
                pMolecularOrbital->exp=(double*)calloc(jMax,sizeof(double));
                pMolecularOrbital->nucl=(int*)calloc(jMax,sizeof(int));
                pMolecularOrbital->type=(int*)calloc(jMax,sizeof(int));

                // Check if the allocation worked
                if (pMolecularOrbital->coeff==NULL ||
                                                 pMolecularOrbital->exp==NULL ||
                                                pMolecularOrbital->nucl==NULL ||
                                                  pMolecularOrbital->type==NULL)
                {
                    PRINT_ERROR("In readWfnFileAndAllocateChemicalSystem: ");
                    fprintf(stderr,"could not reallocate memory for %d ",jMax);
                    fprintf(stderr,"primitives in the %d-th ",i+1);
                    fprintf(stderr,"MolecularOrbital structure.\n");
                    return 0;
                }

                for (j=0; j<jMax; j++)
                {
                    pMolecularOrbital->coeff[j]=pMolecularOrbitalI->coeff[j];
                    pMolecularOrbital->exp[j]=pMolecularOrbitalI->exp[j];
                    pMolecularOrbital->nucl[j]=pMolecularOrbitalI->nucl[j];
                    pMolecularOrbital->type[j]=pMolecularOrbitalI->type[j];
                }
            }
        }
    }

    return returnValue;
}

/* ************************************************************************** */
// The function writingChemicalFile writes the chemical data stored in the
// structure pointed by pChemicalSystem, according the the *.chem format, into
// a file located at fileLocation (warning: reset and overwrite the file if it
// always exists), which must be a path name of (positive) length (strictly)
// less than nameLength. It has the char* fileLocation, the int nameLength, and
// the ChemicalSystem* variable (defined in main.h) as input arguments and it
// returns zero if an error occurred, otherwise one in case of success
/* ************************************************************************** */
int writingChemicalFile(char* fileLocation, int nameLength,
                                                ChemicalSystem* pChemicalSystem)
{
    int i=0, j=0, numberOfMolecularOrbitals=0, numberOfNuclei=0;
    int numberOfPrimitives=0;
    MolecularOrbital *pMolecularOrbital=NULL;
    FILE *chemicalFile=NULL;

    // Check if the input pChemicalSystem variable is pointing to NULL
    if (pChemicalSystem==NULL)
    {
        PRINT_ERROR("In writingChemicalFile: the input pChemicalSystem ");
        fprintf(stderr,"variable is pointing to the ");
        fprintf(stderr,"%p adress.\n",(void*)pChemicalSystem);
        return 0;
    }

    // Check fileLocation is a string of length not greater than nameLength (>6)
    if (!checkStringFromLength(fileLocation,7,nameLength))
    {
        PRINT_ERROR("In writingChemicalFile: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the char* fileLocation ");
        fprintf(stderr,"variable is not a string of length (strictly) less ");
        fprintf(stderr,"than %d (and more than 5 in order to ",nameLength);
        fprintf(stderr,"store at least something more than the *.chem ");
        fprintf(stderr,"extension).\n");
        return 0;
    }

    numberOfMolecularOrbitals=pChemicalSystem->nmorb;
    if (numberOfMolecularOrbitals<1)
    {
        PRINT_ERROR("In writingChemicalFile: we cannot have ");
        fprintf(stderr,"%d molecular orbitals ",numberOfMolecularOrbitals);
        fprintf(stderr,"saved in the structure pointed by pChemicalSystem.\n");
        return 0;
    }

    numberOfPrimitives=pChemicalSystem->ngauss;
    if (numberOfPrimitives<1)
    {
        PRINT_ERROR("In writingChemicalFile: we cannot have ");
        fprintf(stderr,"%d primitives saved in each ",numberOfPrimitives);
        fprintf(stderr,"MolecularOrbital structure of pChemicalSystem.\n");
        return 0;
    }

    numberOfNuclei=pChemicalSystem->nnucl;
    if (numberOfNuclei<1)
    {
        PRINT_ERROR("In writingChemicalFile: we cannot have ");
        fprintf(stderr,"%d nuclei saved in the structure ",numberOfNuclei);
        fprintf(stderr,"pointed by pChemicalSystem.\n");
        return 0;
    }

    // Check if the file is well opened (warning: overwrite file if it exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    fprintf(stdout,"Opening %s file. ",fileLocation);
    chemicalFile=fopen(fileLocation,"w+");
    if (chemicalFile==NULL)
    {
        PRINT_ERROR("In writingChemicalFile: could not open and write into ");
        fprintf(stderr,"the %s file.\n",fileLocation);
        return 0;
    }
    fprintf(stdout,"Writing chemical data. ");

    // Writing number of molecular orbitals, primitives, and nuclei
    fprintf(chemicalFile,"MolecularOrbitals %d\n",numberOfMolecularOrbitals);
    fprintf(chemicalFile,"\nPrimitives %d\n",numberOfPrimitives);
    fprintf(chemicalFile,"\nNuclei\n%d\n",numberOfNuclei);

    // Write the Nucleus data
    for (i=0; i<numberOfNuclei; i++)
    {
        fprintf(chemicalFile,"%.8le ",pChemicalSystem->pnucl[i].x);
        fprintf(chemicalFile,"%.8le ",pChemicalSystem->pnucl[i].y);
        fprintf(chemicalFile,"%.8le ",pChemicalSystem->pnucl[i].z);
        fprintf(chemicalFile,"%d \n",pChemicalSystem->pnucl[i].charge);
    }

    // Write the content of each MolecularOrbital structure
    fprintf(chemicalFile,"\nCoefficient Exponent Center Type");
    for (i=0; i<numberOfMolecularOrbitals; i++)
    {
        fprintf(chemicalFile,"\n%d",i+1);
        pMolecularOrbital=&pChemicalSystem->pmorb[i];

        // Check the spin value
        if (abs(pMolecularOrbital->spin)!=1)
        {
            PRINT_ERROR("In writingChemicalFile: the spin variable of the ");
            fprintf(stderr,"%d-th MolecularOrbital structure should be ",i+1);
            fprintf(stderr,"(+/-)1 instead of %d.\n",pMolecularOrbital->spin);
            fprintf(chemicalFile,"\nEnd");
            closeTheFile(&chemicalFile);
            return 0;
        }
        fprintf(chemicalFile,"\nSpin %d\n",pMolecularOrbital->spin);

        // Check if no error occurs for the pointers in pMolecularOrbital
        if (pMolecularOrbital->coeff==NULL || pMolecularOrbital->exp==NULL ||
                 pMolecularOrbital->nucl==NULL || pMolecularOrbital->type==NULL)
        {
            PRINT_ERROR("In writingChemicalFile: one of the following ");
            fprintf(stderr,"address is not valid in the %d-th ",i+1);
            fprintf(stderr,"MolecularOrbital structure:\n");
            fprintf(stderr,"pChemicalSystem->pmorb[%d].coeff=",i);
            fprintf(stderr,"%p\n",(void*)pMolecularOrbital->coeff);
            fprintf(stderr,"pChemicalSystem->pmorb[%d].exp=",i);
            fprintf(stderr,"%p\n",(void*)pMolecularOrbital->exp);
            fprintf(stderr,"pChemicalSystem->pmorb[%d].nucl=",i);
            fprintf(stderr,"%p\n",(void*)pMolecularOrbital->nucl);
            fprintf(stderr,"pChemicalSystem->pmorb[%d].type=",i);
            fprintf(stderr,"%p\n",(void*)pMolecularOrbital->type);
            fprintf(chemicalFile,"\nEnd");
            closeTheFile(&chemicalFile);
            return 0;
        }

        for (j=0; j<numberOfPrimitives; j++)
        {
            // Write the coefficient value
            fprintf(chemicalFile,"%.8le ",pMolecularOrbital->coeff[j]);

            // Check and write the exponent value
            if (pMolecularOrbital->exp[j]<=0.)
            {
                PRINT_ERROR("In writingChemicalFile: the ");
                fprintf(stderr,"%d-th primitive exponent of the ",j+1);
                fprintf(stderr,"%d-th MolecularOrbital structure should ",i+1);
                fprintf(stderr,"be positive instead of having the ");
                fprintf(stderr,"%lf value.\n",pMolecularOrbital->exp[j]);
                fprintf(chemicalFile,"\nEnd");
                closeTheFile(&chemicalFile);
                return 0;
            }
            fprintf(chemicalFile,"%.8le ",pMolecularOrbital->exp[j]);

            // Check and write the center assignment value
            if (pMolecularOrbital->nucl[j]<1 ||
                                      pMolecularOrbital->nucl[j]>numberOfNuclei)
            {
                PRINT_ERROR("In writingChemicalFile: the ");
                fprintf(stderr,"%d-th primitive center assignment of the ",j+1);
                fprintf(stderr,"%d-th MolecularOrbital structure should ",i+1);
                fprintf(stderr,"be a positive integer, not (strictly) ");
                fprintf(stderr,"greater than the total number of nuclei ");
                fprintf(stderr,"%d, instead of having the ",numberOfNuclei);
                fprintf(stderr,"%d value.\n",pMolecularOrbital->nucl[j]);
                fprintf(chemicalFile,"\nEnd");
                closeTheFile(&chemicalFile);
                return 0;
            }
            fprintf(chemicalFile,"%d ",pMolecularOrbital->nucl[j]);

            // Check and write the type assignment value
            if (pMolecularOrbital->type[j]<1 || pMolecularOrbital->type[j]>20)
            {
                PRINT_ERROR("In writingChemicalFile: the ");
                fprintf(stderr,"%d-th primitive type assignment of the ",j+1);
                fprintf(stderr,"%d-th MolecularOrbital structure should ",i+1);
                fprintf(stderr,"be a positive integer, not (strictly) ");
                fprintf(stderr,"greater than twenty instead of having the ");
                fprintf(stderr,"%d value.\n",pMolecularOrbital->type[j]);
                fprintf(chemicalFile,"\nEnd");
                closeTheFile(&chemicalFile);
                return 0;
            }
            fprintf(chemicalFile,"%d \n",pMolecularOrbital->type[j]);
        }
    }
    fprintf(chemicalFile,"\nEnd");

    // Closing the *.chem file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(chemicalFile))
    {
        PRINT_ERROR("In writingChemicalFile: the");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        chemicalFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.");
    chemicalFile=NULL;

    return 1;
}

/* ************************************************************************** */
// The function loadChemistry extracts the chemical data from a *.chem or *.wfn
// file (warning: reset and overwrite chemicalOut.chem file if it already
// exists) at the location pointed by the name_chem variable of the structure
// pointed by pParameters. It has the Parameters* and ChemicalSystem* variables
// (both defined in main.h) as input arguments and it returns zero if an error
// occurred, otherwise one is returned in case of sucess
/* ************************************************************************** */
int loadChemistry(Parameters* pParameters, ChemicalSystem *pChemicalSystem)
{
    size_t length=0;
    char *fileLocation=NULL;
    int orbRhf=0;

    // Testing if pParameters or pChemicalSystem is pointing to NULL
    if (pParameters==NULL || pChemicalSystem==NULL)
    {
        PRINT_ERROR("In loadChemistry: at least one of the input pParameters ");
        fprintf(stderr,"%p or pChemicalSystem ",(void*)pParameters);
        fprintf(stderr,"%p variable does not point to ",(void*)pChemicalSystem);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check pParameters->name_info variable
    if (!checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length))
    {
        PRINT_ERROR("In loadChemistry: checkStringFromLength function ");
        fprintf(stderr,"returned zero, which is not the expected value here, ");
        fprintf(stderr,"after having checked that the char* name_info ");
        fprintf(stderr,"variable of the structure pointed by pParameters is ");
        fprintf(stderr,"not a string of length (strictly) less than ");
        fprintf(stderr,"%d (and strcitly more than ",pParameters->name_length);
        fprintf(stderr,"5 in order to store at least something more than the ");
        fprintf(stderr,"*.info extension).\n");
        return 0;
    }

    // Check if the *.info file name ends with the ".info" extension
    // strlen returns the length of the string, but not including the char '\0'
    length=strlen(pParameters->name_info);
    if (pParameters->name_info[length-5]!='.' ||
                                        pParameters->name_info[length-4]!='i' ||
                                        pParameters->name_info[length-3]!='n' ||
                                        pParameters->name_info[length-2]!='f' ||
                                        pParameters->name_info[length-1]!='o' ||
                                           pParameters->name_info[length]!='\0')
    {
        PRINT_ERROR("In loadChemistry: ");
        fprintf(stderr,"%s file name does not end ",pParameters->name_info);
        fprintf(stderr,"with with the '.info' extension.\n");
        return 0;
    }

    // calloc returns a pointer to the allocated memory, otherwise NULL
    length=pParameters->name_length;
    fileLocation=(char*)calloc(length,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In loadChemistry: could not allocate memory for the ");
        fprintf(stderr,"local (char*) variable fileLocation.\n");
        return 0;
    }

    // Reading the chemical file at fileLocation if it exists and returns the
    // type of format (1 is *.chem, -1 is *.wfn, 0 refers to an error)
    switch (getChemicalFormat(pParameters->name_chem,pParameters->name_length))
    {
        case 1:
            fprintf(stdout,"\nChemistry will be loaded from ");
            fprintf(stdout,"%s file.",pParameters->name_chem);

            // Reading the *.chem file
            orbRhf=readChemFileandAllocateChemicalSystem(pParameters->name_chem,
                                                       pParameters->name_length,
                                                               pChemicalSystem);

            // Check the pParameters->orb_rhf variable
            if (abs(orbRhf)!=1)
            {
                PRINT_ERROR("In loadChemistry: ");
                fprintf(stderr,"readChemFileandAllocateChemicalSystem ");
                fprintf(stderr,"function returned %d instead of ",orbRhf);
                fprintf(stderr,"(+/-)1.\n");
                free(fileLocation);   // free function does not any return value
                fileLocation=NULL;
                return 0;
            }
            if (orbRhf==1)
            {
                if (!pParameters->orb_rhf)
                {
                    pParameters->orb_rhf=1;
                }
            }
            else
            {
                if (pParameters->orb_rhf)
                {
                    PRINT_ERROR("In loadChemistry: apparently, the orb_rhf ");
                    fprintf(stderr,"(=%d) variable of ",pParameters->orb_rhf);
                    fprintf(stderr,"the structure pointed by pParameters has ");
                    fprintf(stderr,"been set to one in the ");
                    fprintf(stderr,"%s file (i.e. ",pParameters->name_info);
                    fprintf(stderr,"restricted Hartree-fock was suggested), ");
                    fprintf(stderr,"whereas in the %s ",pParameters->name_chem);
                    fprintf(stderr,"file, the chemical data correspond to ");
                    fprintf(stderr,"an unrestricted Hartree-Fock structure.\n");
                    fprintf(stderr,"Pleace check the files and modify the ");
                    fprintf(stderr,"value accordingly after the 'orb_rhf' ");
                    fprintf(stderr,"keyword in the %s ",pParameters->name_info);
                    fprintf(stderr,"file, or or simply delete the ");
                    fprintf(stderr,"corresponding line in order to proceed ");
                    fprintf(stderr,"further.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            // Set the default *.chem name in fileLocation in case of file copy
            // strncpy function returns a pointer to the string (not used here)
            strncpy(fileLocation,pParameters->name_info,length);
            length=strlen(fileLocation);
            fileLocation[length-5]='.';
            fileLocation[length-4]='c';
            fileLocation[length-3]='h';
            fileLocation[length-2]='e';
            fileLocation[length-1]='m';
            fileLocation[length]='\0';

            // Copy the *.chem file into a default *.chem if names are different
            // strcmp returns 0 if the 2 strings are =, otherwise <0 (resp. >0)
            // if the 1st string arg is shorter (resp. longer) than the 2nd one
            if (strcmp(pParameters->name_chem,fileLocation))
            {
                if (initialFileExists(fileLocation,pParameters->name_length)==1)
                {
                    // remove returns 0 on success, otherwise -1 (errno is set)
                    if (remove(fileLocation))
                    {
                        PRINT_ERROR("In loadChemistry: wrong return (=-1) of ");
                        fprintf(stderr,"the standard remove c-function in ");
                        fprintf(stderr,"the attempt of removing the ");
                        fprintf(stderr,"%s file.\n",fileLocation);
                        free(fileLocation);
                        fileLocation=NULL;
                        return 0;
                    }
                }
                if (!copyFileLocation(pParameters->name_chem,
                                         pParameters->name_length,fileLocation))
                {
                    PRINT_ERROR("In loadChemistry: copyFileLocation function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
                strncpy(pParameters->name_chem,fileLocation,
                                                      pParameters->name_length);
            }
            break;

        case -1:
            fprintf(stdout,"\nChemistry will be loaded from ");
            fprintf(stdout,"%s file.",pParameters->name_chem);

            // Copy the *.wfn file into a (previously removed if exists) *.temp
            strncpy(fileLocation,pParameters->name_info,length);
            length=strlen(fileLocation);
            fileLocation[length-5]='.';
            fileLocation[length-4]='t';
            fileLocation[length-3]='e';
            fileLocation[length-2]='m';
            fileLocation[length-1]='p';
            fileLocation[length]='\0';

            if (initialFileExists(fileLocation,pParameters->name_length)==1)
            {
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In loadChemistry: wrong return (=-1) of the ");
                    fprintf(stderr,"standard remove c-function in the ");
                    fprintf(stderr,"attempt of removing the ");
                    fprintf(stderr,"%s file.\n",fileLocation);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            if (!copyFileLocation(pParameters->name_chem,
                                         pParameters->name_length,fileLocation))
            {
                PRINT_ERROR("In loadChemistry: copyFileLocation function ");
                fprintf(stderr,"returned zero instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            // Update the *.temp name in the name_chem variable of pParameters
            strncpy(pParameters->name_chem,fileLocation,
                                                      pParameters->name_length);

            // Converting the Fortran scientific notation (1.0D+01) into the C
            // format (1.0e+01) in the *.temp file (the copy of the *.wfn file)
            if (!readAndConvertWfnFile(fileLocation,'D','e',
                                                      pParameters->name_length))
            {
                PRINT_ERROR("In loadChemistry: readAndConvertWfnFile ");
                fprintf(stderr,"function returned zero instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            // Reading the (copy *.temp of the) *.wfn file
            orbRhf=readWfnFileAndAllocateChemicalSystem(fileLocation,
                                                       pParameters->name_length,
                                                               pChemicalSystem);
            // Check the pParameters->orb_rhf variable
            if (abs(orbRhf)!=1)
            {
                PRINT_ERROR("In loadChemistry: ");
                fprintf(stderr,"readWfnFileandAllocateChemicalSystem ");
                fprintf(stderr,"function returned %d instead of ",orbRhf);
                fprintf(stderr,"(+/-)1.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }
            if (orbRhf==1)
            {
                if (!pParameters->orb_rhf)
                {
                    pParameters->orb_rhf=1;
                }
            }
            else
            {
                if (pParameters->orb_rhf)
                {
                    PRINT_ERROR("In loadChemistry: apparently, the orb_rhf ");
                    fprintf(stderr,"(=%d) variable of ",pParameters->orb_rhf);
                    fprintf(stderr,"the structure pointed by pParameters has ");
                    fprintf(stderr,"been set to one in the ");
                    fprintf(stderr,"%s file (i.e. ",pParameters->name_info);
                    fprintf(stderr,"restricted Hartree-fock was suggested), ");
                    fprintf(stderr,"whereas in the %s ",pParameters->name_chem);
                    fprintf(stderr,"file, the chemical data correspond to ");
                    fprintf(stderr,"an unrestricted Hartree-Fock structure.\n");
                    fprintf(stderr,"Pleace check the files and modify the ");
                    fprintf(stderr,"value accordingly after the 'orb_rhf' ");
                    fprintf(stderr,"keyword in the %s ",pParameters->name_info);
                    fprintf(stderr,"file, or or simply delete the ");
                    fprintf(stderr,"corresponding line in order to proceed ");
                    fprintf(stderr,"further.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            // Overwrite the *.temp file to save the *.wfn into a *.chem format
            if (!writingChemicalFile(fileLocation,
                                      pParameters->name_length,pChemicalSystem))
            {
                PRINT_ERROR("In loadChemistry: writingChemicalFile function ");
                fprintf(stderr,"returned zero instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            // Change the *.temp extension into an *.chem one (warning: the
            // corresponding *.chem file will be next deleted if it exists)
            length=strlen(fileLocation);
            fileLocation[length-5]='.';
            fileLocation[length-4]='c';
            fileLocation[length-3]='h';
            fileLocation[length-2]='e';
            fileLocation[length-1]='m';
            fileLocation[length]='\0';

            if (initialFileExists(fileLocation,pParameters->name_length)==1)
            {
                // remove returns zero on success, otherwise -1 and errno is set
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In loadChemistry: wrong return (=-1) of the ");
                    fprintf(stderr,"standard remove c-function in the ");
                    fprintf(stderr,"attempt of removing the ");
                    fprintf(stderr,"%s file.\n",fileLocation);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            // Rename the *.temp file into an *.chem one
            if (!renameFileLocation(pParameters->name_chem,
                                         pParameters->name_length,fileLocation))
            {
                PRINT_ERROR("In loadChemistry: rename FileLocation function ");
                fprintf(stderr,"returned zero instead of one.\n");
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }

            // Copy the *.chem name in the name_chem variable of pParameters
            strncpy(pParameters->name_chem,fileLocation,
                                                      pParameters->name_length);
            break;

        default:
            PRINT_ERROR("In loadChemistry: getChemicalFormat function did ");
            fprintf(stderr,"not return (+/-) one, which was the expected ");
            fprintf(stderr,"value here.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
            break;
    }

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    fprintf(stdout,"\nChemical system (");
    if (pParameters->orb_rhf)
    {
        fprintf(stdout,"restricted Hartree-Fock) successfully loaded:\n");
    }
    else
    {
        fprintf(stdout,"unrestricted Hartree-Fock) successfully loaded:\n");
    }
    fprintf(stdout,"%d Molecular Orbitals, ",pChemicalSystem->nmorb);
    fprintf(stdout,"%d Primitives, ",pChemicalSystem->ngauss);
    fprintf(stdout,"and %d Nuclei.\n",pChemicalSystem->nnucl);

    // Checking the pParameters->nu_electrons variable
    if (pParameters->nu_electrons<1 ||
                               pParameters->nu_electrons>pChemicalSystem->nmorb)
    {
        PRINT_ERROR("In loadChemistry: the nu_electrons variable ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->nu_electrons);
        fprintf(stderr,"pointed by pParameters must be a positive integer ");
        fprintf(stderr,"that cannot be (strictly) greater than the total ");
        fprintf(stderr,"number of electrons (=%d).\n",pChemicalSystem->nmorb);
        return 0;
    }

    return 1;
}

