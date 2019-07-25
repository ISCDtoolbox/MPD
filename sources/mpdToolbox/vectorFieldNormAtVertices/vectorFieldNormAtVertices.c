#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <math.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

#define PRINT_ERROR(stringToPrint)                                             \
do {                                                                           \
        fflush(stdout);                                                        \
        fprintf(stderr,"\nError encountered at line %d in ",__LINE__);         \
        fprintf(stderr,"%s file on %s",__FILE__,endTimerAtError());            \
        fprintf(stderr,"%s",stringToPrint);                                    \
} while (0)

char* endTimerAtError(void);
void closeTheFile(FILE** pFileToClose);
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                             int maximumLength);
int getNumberOfVerticesInSolFile(char nameFile[NAME_SIZE_MAX]);
int readVectorialSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                                                        double* py, double* pz);
int writingSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* pNorm);

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
char* endTimerAtError(void)
{
    time_t localFinalTimer=0;

    time(&localFinalTimer);

    return ctime(&localFinalTimer);
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void closeTheFile(FILE** pFileToClose)
{
    if (pFileToClose!=NULL)
    {
        if (*pFileToClose!=NULL)
        {
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

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                              int maximumLength)
{
    int i=0, j=0;

    // Check if stringToCheck is pointing to NULL
    if (stringToCheck==NULL)
    {
        PRINT_ERROR("In checkStringFromLength: the input (char*) variable ");
        fprintf(stderr,"stringToCheck=%p does not point ",(void*)stringToCheck);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check if the bounds are correct
    if (minimumLength<1 || maximumLength<minimumLength)
    {
        PRINT_ERROR("In checkStringFromLength: the input variable ");
        fprintf(stderr,"minimumLength (=%d) should be a ",minimumLength);
        fprintf(stderr,"positive integer less or equal to the other input ");
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
        fprintf(stderr,"stringToCheck=%p is supposed ",(void*)stringToCheck);
        fprintf(stderr,"to point to a valid string. It seems that ");
        fprintf(stderr,"stringToCheck is not a string (no terminating NUL ");
        fprintf(stderr,"character found among the characters ");
        for (j=0; j<maximumLength; j++)
        {
            fprintf(stderr,"%c",stringToCheck[j]);
        }
        fprintf(stderr,") or has a length greater or equal to maximumLength ");
        fprintf(stderr,"(=%d).\n",maximumLength);
        return 0;
    }
    else if (i<minimumLength-1)
    {
        PRINT_ERROR("In checkStringFromLength: the input (char*) variable ");
        fprintf(stderr,"stringToCheck is a string (=%s) of ",stringToCheck);
        fprintf(stderr,"length %d, which is strictly less than the minimal ",i);
        fprintf(stderr,"size (=%d) allowed here.\n",minimumLength-1);
        return 0;
    }

    // Add the terminating NUL character '\0' to the length size
    i++;

    return i;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int getNumberOfVerticesInSolFile(char nameFile[NAME_SIZE_MAX])
{
    char *readStringOut=NULL, keyword[13]={'\0'};
    int readChar=0, readIntegerOut=0, nVer=0, boolean=0;
    FILE *solFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Open the input *.sol file: it must already exist in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    solFile=fopen(nameFile,"r");
    if (solFile==NULL)
    {
        PRINT_ERROR("In getNumberOfVerticesInSolFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the total number of solutions. ");

    // Skip all characters until the 'S' of SolAtVertices is found
    boolean=1;
    do
    {
        readChar=fgetc(solFile);

        // Skip commented line if any
        if (readChar=='#')
        {
            while (readChar!='\n' && readChar!=EOF)
            {
                readChar=fgetc(solFile);
            }
        }
        else if (readChar=='S')
        {
            if (fseek(solFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In getNumberOfVerticesInSolFile: wrong return ");
                fprintf(stderr,"of the fseek c-function while checking if ");
                fprintf(stderr,"the '%c' encountered ",(char)readChar);
                fprintf(stderr,"corresponds to the one of the SolAtVertices ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&solFile);
                return 0;
            }

            // Check if it is 'S' of SolAtVertices (and not ISoLAtVertices)
            readChar=fgetc(solFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'S' of SolAtVertices
            readChar=fgetc(solFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfVerticesInSolFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'S' of the SolAtVertices keyword.\n");
                closeTheFile(&solFile);
                return 0;
            }
            else if (readChar!='S')
            {
                PRINT_ERROR("In getNumberOfVerticesInSolFile: expecting ");
                fprintf(stderr,"'S' instead of %c while ",(char)readChar);
                fprintf(stderr,"attempting to read to the first ");
                fprintf(stderr,"character of the SolAtVertices keyword.\n");
                closeTheFile(&solFile);
                return 0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfVerticesInSolFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any solutions at vertices found.\n");
        closeTheFile(&solFile);
        return 0;
    }

    // Read the number of solutions at vertices
    readStringOut=fgets(keyword,13,solFile);
    readIntegerOut=fscanf(solFile," %d ",&nVer);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfVerticesInSolFile: wrong ");
        fprintf(stderr,"return= of the fgets=%p ",(void*)readStringOut);
        fprintf(stderr,"or fscanf (=%d) c-function in the ",readIntegerOut);
        fprintf(stderr,"attempt of reading the number of solutions at ");
        fprintf(stderr,"vertices.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (strcmp(keyword,"olAtVertices") || nVer<1)
    {
        PRINT_ERROR("In getNumberOfVerticesInSolFile: expecting ");
        fprintf(stderr,"SolAtVertices Nver instead of ");
        fprintf(stderr,"%c%s %d (positive ",(char)readChar,keyword,nVer);
        fprintf(stderr,"value).\n");
        closeTheFile(&solFile);
        return 0;
    }

    // Close the input *.sol file
    if (fclose(solFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfVerticesInSolFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        solFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file (%d solutions found).\n",nVer);
    solFile=NULL;

    return nVer;
}


////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readVectorialSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                                                         double* py, double* pz)
{
    char *readStringOut=NULL, keyword[21]={'\0'};
    int i=0, readIntegerOut=0, readIntegerIn=0;
    double readDouble=0.;
    FILE *solFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In readVectorialSolFile: at least one of the input ");
        fprintf(stderr,"(double*) variables px=%p py=%p ",(void*)px,(void*)py);
        fprintf(stderr,"and/or pz=%p does not point to a valid ",(void*)pz);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    // Check the number of solutions at vertices
    if (nVer<1)
    {
        PRINT_ERROR("In readVectorialSolFile: the input variable nVer ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nVer);
        return 0;
    }

    // Open the input *.sol file: it must already exist in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    solFile=fopen(nameFile,"r");
    if (solFile==NULL)
    {
        PRINT_ERROR("In readVectorialSolFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading mesh solutions. ");

    // Potentially read the first white space characters
    readIntegerOut=fscanf(solFile," ");
    if (readIntegerOut)
    {
        PRINT_ERROR("In readVectorialSolFile: wrong return ");
        fprintf(stderr,"(=%d) of the fscanf function ",readIntegerOut);
        fprintf(stderr,"while attempting to read the first white space ");
        fprintf(stderr,"(in case there was some).\n");
        closeTheFile(&solFile);
        return 0;
    }

    // Check the real format (1 simple precision/ 2 double precision)
    readStringOut=fgets(keyword,21,solFile);
    readIntegerOut=fscanf(solFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readVectorialSolFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the MeshVersionFormatted keyword.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (strcmp(keyword,"MeshVersionFormatted") || readIntegerIn!=2)
    {
        PRINT_ERROR("In readVectorialSolFile: expecting MeshVersionFormatted ");
        fprintf(stderr,"2 instead of %s %d.\n",keyword,readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the dimension (two, three)
    readStringOut=fgets(keyword,10,solFile);
    readIntegerOut=fscanf(solFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readVectorialSolFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the Dimension keyword.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (strcmp(keyword,"Dimension") || readIntegerIn!=3)
    {
        PRINT_ERROR("In readVectorialSolFile: expecting Dimension 3 instead ");
        fprintf(stderr,"of %s %d.\n",keyword,readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the number of mesh vertices in the *.sol file
    readStringOut=fgets(keyword,14,solFile);
    readIntegerOut=fscanf(solFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readVectorialSolFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the SolAtVertices keyword.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (strcmp(keyword,"SolAtVertices") || readIntegerIn!=nVer)
    {
        PRINT_ERROR("In readVectorialSolFile: expecting SolAtVertices ");
        fprintf(stderr,"%d instead of %s ",nVer,keyword);
        fprintf(stderr,"%d.\n",readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the number of solutions
    readIntegerOut=fscanf(solFile,"%d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readVectorialSolFile: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the number of ");
        fprintf(stderr,"solutions.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (readIntegerIn!=1)
    {
        PRINT_ERROR("In readVectorialSolFile: expecting one solution ");
        fprintf(stderr,"(instead of %d ones).\n",readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the vectorial type of solutions
    readIntegerOut=fscanf(solFile,"%d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readVectorialSolFile: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the type of ");
        fprintf(stderr,"solutions.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (readIntegerIn!=2)
    {
        PRINT_ERROR("In readVectorialSolFile: expecting two (vectorial type) ");
        fprintf(stderr,"(instead of %d).\n",readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Read vectorial values at the mesh vertices from the *.sol file
    for (i=0; i<nVer; i++)
    {
        // Read the first value
        readIntegerOut=fscanf(solFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVectorialSolFile: wrong return ");
            fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
            fprintf(stderr,"c-function in the attempt of reading the first ");
            fprintf(stderr,"value associated with the %d-th vertex.\n",i+1);
            closeTheFile(&solFile);
            return 0;
        }
        px[i]=readDouble;

        // Read the second value
        readIntegerOut=fscanf(solFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVectorialSolFile: wrong return ");
            fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
            fprintf(stderr,"c-function in the attempt of reading the second ");
            fprintf(stderr,"value associated with the %d-th vertex.\n",i+1);
            closeTheFile(&solFile);
            return 0;
        }
        py[i]=readDouble;


        // Read the third value
        readIntegerOut=fscanf(solFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVectorialSolFile: wrong return ");
            fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
            fprintf(stderr,"c-function in the attempt of reading the third ");
            fprintf(stderr,"value associated with the %d-th vertex.\n",i+1);
            closeTheFile(&solFile);
            return 0;
        }
        pz[i]=readDouble;
    }

    // Close the input *.sol file
    if (fclose(solFile))
    {
        fprintf(stdout,"\nWarning in readVectorialSolFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        solFile=NULL;
        return 0;
    }
    solFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* pNorm)
{
    int i=0;
    FILE *solFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if pNorm is set to NULL
    if (pNorm==NULL)
    {
        PRINT_ERROR("In writingSolFile: the input (double*) variable ");
        fprintf(stderr,"pNorm=%p does not point to a valid ",(void*)pNorm);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of vertices
    if (nVer<1)
    {
        PRINT_ERROR("In writingSolFile: the input variable nVer ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nVer);
        return 0;
    }

    // Open output *.sol file (warning: overwrite the file if it already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    solFile=fopen(nameFile,"w+");
    if (solFile==NULL)
    {
        PRINT_ERROR("In writingSolFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing data. ");

    // Save the norm of the vector field with the help of pNorm
    fprintf(solFile,"MeshVersionFormatted 2\n\nDimension 3\n\n");
    fprintf(solFile,"SolAtVertices\n%d\n1 1\n\n",nVer);
    for (i=0; i<nVer; i++)
    {
        fprintf(solFile,"%.8le \n",pNorm[i]);
    }
    fprintf(solFile,"\nEnd\n\n");

    // Close the output *.sol file
    if (fclose(solFile))
    {
        fprintf(stdout,"\nWarning in writingSolFile function: we could not ");
        fprintf(stdout,"close the %s file properly.\n",nameFile);
        solFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    solFile=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    char extension[2][7]={"input","output"};
    int length=0, boolean=0, i=0, nVer=0;
    double *px=NULL, *py=NULL, *pz=NULL, *pNorm=NULL;

    // Check the number of arguments
    if (argc!=3)
    {
        PRINT_ERROR("In main: the program only takes two arguments instead ");
        fprintf(stderr,"of %d.\n",argc-1);
        return EXIT_FAILURE;
    }

    // Check the length and extension of the input and output *.sol file names
    for (i=1; i<argc; i++)
    {
        length=checkStringFromLength(argv[i],6,NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s *.sol file name should be a ",extension[i-1]);
            fprintf(stderr,"string of at most %d characters ",NAME_SIZE_MAX-1);
            fprintf(stderr,"(and strictly more than 4 to contain at least ");
            fprintf(stderr,"something more than the *.sol extension).\n");
            return EXIT_FAILURE;
        }

        boolean=(argv[i][length-5]=='.' && argv[i][length-4]=='s');
        boolean=(boolean && argv[i][length-3]=='o' && argv[i][length-2]=='l');
        boolean=(boolean && argv[i][length-1]=='\0');
        if (!boolean)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"*.sol extension does not correspond to the one ");
            fprintf(stderr,"of the %s file name.\n",argv[i]);
            return EXIT_FAILURE;
        }
    }

    // Count the total number of solutions at the mesh vertices
    nVer=getNumberOfVerticesInSolFile(argv[1]);
    if (nVer<1)
    {
        PRINT_ERROR("In main: the total number of solutions at mesh vertices ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nVer);
        return EXIT_FAILURE;
    }

    // Allocate memory for the solutions
    px=(double*)calloc(nVer,sizeof(double));
    py=(double*)calloc(nVer,sizeof(double));
    pz=(double*)calloc(nVer,sizeof(double));
    pNorm=(double*)calloc(nVer,sizeof(double));
    if (px==NULL || py==NULL || pz==NULL || pNorm==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(double*) variable px=%p, py=%p, ",(void*)px,(void*)py);
        fprintf(stderr,"pz=%p and/or pNorm=%p.\n",(void*)pz,(void*)pNorm);
        free(px);
        free(py);
        free(pz);
        free(pNorm);
        px=NULL;
        py=NULL;
        pz=NULL;
        pNorm=NULL;
        return EXIT_FAILURE;
    }

    // Store the vector field coordinates
    if (!readVectorialSolFile(argv[1],nVer,px,py,pz))
    {
        PRINT_ERROR("In main: readVectorialSolFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(px);
        free(py);
        free(pz);
        free(pNorm);
        px=NULL;
        py=NULL;
        pz=NULL;
        pNorm=NULL;
        return EXIT_FAILURE;
    }

    // Compute the norm of the vector field
    fprintf(stdout,"\nComputing the norm of the vector field at the mesh ");
    fprintf(stdout,"vertices.\n");
    for (i=0; i<nVer; i++)
    {
        pNorm[i]=sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]);
    }

    // Save the norm of the vector field in the output *.sol file
    if (!writingSolFile(argv[2],nVer,pNorm))
    {
        PRINT_ERROR("In main: writingSolFile function returned zero instead ");
        fprintf(stderr,"of one.\n");
        free(px);
        free(py);
        free(pz);
        free(pNorm);
        px=NULL;
        py=NULL;
        pz=NULL;
        pNorm=NULL;
        return EXIT_FAILURE;
    }

    // Free the memory dynamically allocated
    free(px);
    free(py);
    free(pz);
    free(pNorm);
    px=NULL;
    py=NULL;
    pz=NULL;
    pNorm=NULL;

    return EXIT_SUCCESS;
}

