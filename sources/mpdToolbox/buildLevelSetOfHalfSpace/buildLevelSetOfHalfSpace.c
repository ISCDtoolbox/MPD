#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Allowed value 0 (x-plane) 1 (y-plane) 2 (z-plane)
#define XYZ_COORDINATE 0

// Computing the half-space (xyz>=LEVELSET_VALUE)
#define LEVELSET_VALUE 0.0

// Computing the other half-space (xyz<=LEVELSET_VALUE) if it is set to 1
#define COMPLEMENT_OR_NOT 0

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

#define PRINT_ERROR(stringToPrint)                                             \
do {                                                                           \
    fflush(stdout);                                                            \
    fprintf(stderr,"\nError encountered at line %d in %s ",__LINE__,__FILE__); \
    fprintf(stderr,"file on %s%s",endTimerAtError(),stringToPrint);            \
} while (0)

char* endTimerAtError(void);
void closeTheFile(FILE** pFileToClose);
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                             int maximumLength);
int getNumberOfVerticesInMeshFile(char nameFile[NAME_SIZE_MAX]);
int getCoordinateFromMeshFile(char nameFile[NAME_SIZE_MAX], int nVer,
                                                                    double* px);
int writingSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px);

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
int getNumberOfVerticesInMeshFile(char nameFile[NAME_SIZE_MAX])
{
    char *readStringOut=NULL, keyword[8]={'\0'};
    int readChar=0, readIntegerOut=0, nVer=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In getNumberOfVerticesInMeshFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the total number of mesh vertices. ");

    // Skip all characters until the 'V' of Vertices is found
    boolean=1;
    do
    {
        readChar=fgetc(meshFile);

        // Skip commented line if any
        if (readChar=='#')
        {
            while (readChar!='\n' && readChar!=EOF)
            {
                readChar=fgetc(meshFile);
            }
        }
        else if (readChar=='V')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In getNumberOfVerticesInMeshFile: wrong return ");
                fprintf(stderr,"of the fseek c-function while checking if ");
                fprintf(stderr,"the '%c' encountered ",(char)readChar);
                fprintf(stderr,"corresponds to the one of the Vertices ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'V' of Vertices (and not MeshVersionFormatted)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'V' of Vertices
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfVerticesInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'V' of the Vertices keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='V')
            {
                PRINT_ERROR("In getNumberOfVerticesInMeshFile: expecting ");
                fprintf(stderr,"'V' instead of %c while ",(char)readChar);
                fprintf(stderr,"attempting to read to the first ");
                fprintf(stderr,"character of the Vertices keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfVerticesInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any vertices found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of vertices
    readStringOut=fgets(keyword,8,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&nVer);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfVerticesInMeshFile: wrong ");
        fprintf(stderr,"return= of the fgets=%p ",(void*)readStringOut);
        fprintf(stderr,"or fscanf (=%d) c-function in the ",readIntegerOut);
        fprintf(stderr,"attempt of reading the number of vertices.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"ertices") || nVer<1)
    {
        PRINT_ERROR("In getNumberOfVerticesInMeshFile: expecting Vertices ");
        fprintf(stderr,"Nver instead of %c%s %d ",(char)readChar,keyword,nVer);
        fprintf(stderr,"(positive value).\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfVerticesInMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file (%d vertices found).\n",nVer);
    meshFile=NULL;

    return nVer;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int getCoordinateFromMeshFile(char nameFile[NAME_SIZE_MAX], int nVer,
                                                                     double* px)
{
    char *readStringOut=NULL, keyword[8]={'\0'};
    int readChar=0, readIntegerOut=0, readIntegerIn=0, i=0, boolean=0;
    double y=0., z=0.;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if px is set to NULL
    if (px==NULL)
    {
        PRINT_ERROR("In getCoordinateFromMeshFile: the input (double*) ");
        fprintf(stderr,"variable px=%p does not point to a valid ",(void*)px);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    // Check the number of vertices
    if (nVer<1)
    {
        PRINT_ERROR("In getCoordinateFromMeshFile: the input variable ");
        fprintf(stderr,"nVer (=%d) should be a positive integer.\n",nVer);
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In getCoordinateFromMeshFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the point coordinates. ");

    // Skip all characters until the 'V' of Vertices is found
    boolean=1;
    do
    {
        readChar=fgetc(meshFile);

        // Skip commented line if any
        if (readChar=='#')
        {
            while (readChar!='\n' && readChar!=EOF)
            {
                readChar=fgetc(meshFile);
            }
        }
        else if (readChar=='V')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In getCoordinateFromMeshFile: wrong return of ");
                fprintf(stderr,"the fseek c-function while checking if the ");
                fprintf(stderr,"'%c' encountered corresponds ",(char)readChar);
                fprintf(stderr,"to the one of the Vertices keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'V' of Vertices (and not MeshVersionFormatted)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'V' of Vertices
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getCoordinateFromMeshFile: wrong end-of-file ");
                fprintf(stderr,"return (=%d) of the fgetc ",readChar);
                fprintf(stderr,"c-function while attempting to read the 'V' ");
                fprintf(stderr,"of the Vertices keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='V')
            {
                PRINT_ERROR("In getCoordinateFromMeshFile: expecting 'V' ");
                fprintf(stderr,"instead of %c while ",(char)readChar);
                fprintf(stderr,"attempting to read to the first character of ");
                fprintf(stderr,"the Vertices keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getCoordinateFromMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any vertices found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of vertices
    readStringOut=fgets(keyword,8,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getCoordinateFromMeshFile: wrong ");
        fprintf(stderr,"return= of the fgets=%p ",(void*)readStringOut);
        fprintf(stderr,"or fscanf (=%d) c-function in the ",readIntegerOut);
        fprintf(stderr,"attempt of reading the number of vertices.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"ertices") || readIntegerIn!=nVer)
    {
        PRINT_ERROR("In getCoordinateFromMeshFile: expecting Vertices ");
        fprintf(stderr,"%d instead of %c%s ",nVer,(char)readChar,keyword);
        fprintf(stderr,"%d.\n",readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<nVer; i++)
    {

#if (!XYZ_COORDINATE)
        readIntegerOut=fscanf(meshFile,"%lf %lf %lf %d ",&px[i],&y,&z,
                                                                &readIntegerIn);
#elif (XYZ_COORDINATE==1)
        readIntegerOut=fscanf(meshFile,"%lf %lf %lf %d ",&y,&px[i],&z,
                                                                &readIntegerIn);
#else
        readIntegerOut=fscanf(meshFile,"%lf %lf %lf %d ",&y,&z,&px[i],
                                                                &readIntegerIn);
#endif

        if (readIntegerOut!=4)
        {
            PRINT_ERROR("In getCoordinateFromMeshFile: expecting 4 ");
            fprintf(stderr,"instead of %d for the return ",readIntegerOut);
            fprintf(stderr,"value of the fscanf c-function in the attempt of ");
            fprintf(stderr,"reading the coordinates of the %d-th ",i+1);
            fprintf(stderr,"vertex.\n");
            closeTheFile(&meshFile);
            return 0;
        }
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in getCoordinateFromMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    meshFile=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px)
{
    int i=0;
    FILE *solFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if px is set to NULL
    if (px==NULL)
    {
        PRINT_ERROR("In writingSolFile: the input (double*) variable ");
        fprintf(stderr,"px=%p does not point to a valid ",(void*)px);
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

    // Open the *.sol file (warning: overwrite the file if it already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    solFile=fopen(nameFile,"w+");
    if (solFile==NULL)
    {
        PRINT_ERROR("In writingSolFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing data. ");

    // Save the coordinates stored in px
    fprintf(solFile,"\nMeshVersionFormatted 2\n\nDimension 3\n\n");
    fprintf(solFile,"SolAtVertices\n%d\n1 1\n\n",nVer);
    for (i=0; i<nVer; i++)
    {
        fprintf(solFile,"%.8le \n",px[i]);
    }
    fprintf(solFile,"\nEnd\n\n");

    // Close the *.sol file
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
    char extension[2][13]={"input *.mesh","output *.sol"};
    int i=0, length=0, nVer=0, extensionLength[2]={7,6}, boolean=0;
    double *px=NULL;

    // Check the number of arguments
    if (argc!=3)
    {
        PRINT_ERROR("In main: the program only takes two arguments ");
        fprintf(stderr,"instead of %d.\n",argc-1);
        return EXIT_FAILURE;
    }

    // Check the length and extension of the input and output file names
    for (i=1; i<argc; i++)
    {
        length=checkStringFromLength(argv[i],extensionLength[i-1],
                                                                 NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s file name should be a string ",extension[i-1]);
            fprintf(stderr,"of at most %d characters (and ",NAME_SIZE_MAX-1);
            fprintf(stderr,"strictly more than %d ",extensionLength[i-1]);
            fprintf(stderr,"to contain at least something more than the ");
            fprintf(stderr,"%s extension).\n",extension[i-1]);
            return EXIT_FAILURE;
        }

        boolean=argv[i][length-5]==extension[i-1][8];
        boolean=(boolean && argv[i][length-4]==extension[i-1][9]);
        boolean=(boolean && argv[i][length-3]==extension[i-1][10]);
        boolean=(boolean && argv[i][length-2]==extension[i-1][11]);
        boolean=(boolean && argv[i][length-1]==extension[i-1][12]);
        if (i==1)
        {
            boolean=(boolean && argv[i][length-6]==extension[i-1][7]);
        }

        if (!boolean)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s extension does not correspond ",extension[i-1]);
            fprintf(stderr,"to the one of the %s file name.\n",argv[i]);
            return EXIT_FAILURE;
        }
    }

    // Count the total number of mesh vertices
    nVer=getNumberOfVerticesInMeshFile(argv[1]);
    if (nVer<1)
    {
        PRINT_ERROR("In main: the total number of mesh vertices ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nVer);
        return EXIT_FAILURE;
    }

    // Allocate memory for the point coordinates of the mesh vertices
    px=(double*)calloc(nVer,sizeof(double));
    if (px==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(double*) variable px=%p.\n",(void*)px);
        return EXIT_FAILURE;
    }

    // Store the point coordinate in px
    if (!getCoordinateFromMeshFile(argv[1],nVer,px))
    {
        PRINT_ERROR("In main: getCoordinateFromMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        free(px);
        px=NULL;
        return EXIT_FAILURE;
    }

    // Convert the coord. into the signed distance of the half-space
    for (i=0; i<nVer; i++)
    {
        px[i]-=LEVELSET_VALUE;
    }
#if (!COMPLEMENT_OR_NOT)
    for (i=0; i<nVer; i++)
    {
        px[i]*=-1.;
    }
#endif

#if (!COMPLEMENT_OR_NOT)
  #if (!XYZ_COORDINATE)
    fprintf(stdout,"\nBuilding the level-set function of the half-space ");
    fprintf(stdout,"(x>%.8lf) at mesh vertices.\n",LEVELSET_VALUE);
  #elif (XYZ_COORDINATE==1)
    fprintf(stdout,"\nBuilding the level-set function of the half-space ");
    fprintf(stdout,"(y>%.8lf) at mesh vertices.\n",LEVELSET_VALUE);
  #else
    fprintf(stdout,"\nBuilding the level-set function of the half-space ");
    fprintf(stdout,"(z>%.8lf) at mesh vertices.\n",LEVELSET_VALUE);
  #endif
#else
  #if (!XYZ_COORDINATE)
    fprintf(stdout,"\nBuilding the level-set function of the half-space ");
    fprintf(stdout,"(x<%.8lf) at mesh vertices.\n",LEVELSET_VALUE);
  #elif (XYZ_COORDINATE==1)
    fprintf(stdout,"\nBuilding the level-set function of the half-space ");
    fprintf(stdout,"(y<%.8lf) at mesh vertices.\n",LEVELSET_VALUE);
  #else
    fprintf(stdout,"\nBuilding the level-set function of the half-space ");
    fprintf(stdout,"(z<%.8lf) at mesh vertices.\n",LEVELSET_VALUE);
  #endif
#endif

    // Writing the *.sol file
    if (!writingSolFile(argv[2],nVer,px))
    {
        PRINT_ERROR("In main: writingsolFile function returned zero instead ");
        fprintf(stderr,"of one.\n");
        free(px);
        px=NULL;
        return EXIT_FAILURE;
    }

    // Free the memory dynamically allocated
    free(px);
    px=NULL;

    return EXIT_SUCCESS;
}

