#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Convert Bohr into Angstrom (1.e-10m) if set to 0 at compilation
// and do the converse (Angstrom->Bohr) if it is set to 1
#define BOHR_OR_NOT 0

// Bohr radius in Angstrom (1.e-10m)
#define BOHR_RADIUS 0.5291772109217

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
int countNumberOfVerticesInObjFile(char nameFile[NAME_SIZE_MAX]);
int readVerticesInObjFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                                                        double* py, double* pz);
int countNumberOfTrianglesInObjFile(char nameFile[NAME_SIZE_MAX]);
int readTrianglesInObjFile(char nameFile[NAME_SIZE_MAX], int nTri, int* ip1,
                                                            int* ip2, int* ip3);
int writingResizedObjFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                          double* py, double* pz, int nTri, int* ip1, int* ip2,
                                                                      int* ip3);

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
int countNumberOfVerticesInObjFile(char nameFile[NAME_SIZE_MAX])
{
    int readChar=0, counter=0, readIntegerOut=0;
    double readDouble=0.;
    FILE *objFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Open the *.obj file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"r");
    if (objFile==NULL)
    {
        PRINT_ERROR("In countNumberOfVerticesInObjFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Counting the total number of vertices. ");

    // Potentially read the first white space characters
    readIntegerOut=fscanf(objFile," ");
    if (readIntegerOut)
    {
        PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong return ");
        fprintf(stderr,"(=%d) of the fscanf function ",readIntegerOut);
        fprintf(stderr,"(EOF=%d) while attempting to read the ",EOF);
        fprintf(stderr,"first white space characters (in case there was ");
        fprintf(stderr,"some).\n");
        closeTheFile(&objFile);
        return 0;
    }

    // Read the first character
    readChar=fgetc(objFile);
    if (readChar==EOF)
    {
        PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong end-of-file ");
        fprintf(stderr,"return (=%d) of the fgetc c-function.\n",readChar);
        closeTheFile(&objFile);
        return 0;
    }

    // Count the number of vertices
    counter=0;
    while (readChar=='v')
    {
        // Read the first coordinate
        readIntegerOut=fscanf(objFile," %lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the second coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the third coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }

        // Increment counter by one
        counter++;

        // Read the next character
        readChar=fgetc(objFile);
        if (readChar==EOF)
        {
            PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong ");
            fprintf(stderr,"end-of-file return (=%d) of the fgetc ",readChar);
            fprintf(stderr,"c-function.\n");
            closeTheFile(&objFile);
            return 0;
        }
    }

    // Close the *.obj file
    if (fclose(objFile))
    {
        fprintf(stdout,"\nWarning in countNumberOfVerticesInObjFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        objFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file (%d vertices found).\n",counter);
    objFile=NULL;

    return counter;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readVerticesInObjFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                                                         double* py, double* pz)
{
    int readChar=0, counter=0, readIntegerOut=0;
    double readDouble=0.;
    FILE *objFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In readVerticesInObjFile: at least one of the input ");
        fprintf(stderr,"(double*) variable px=%p py=%p ",(void*)px,(void*)py);
        fprintf(stderr,"or pz=%p does not point to a valid ",(void*)pz);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the total number of vertices
    if (nVer<1)
    {
        PRINT_ERROR("In readVerticesInObjFile: the input variable nVer ");
        fprintf(stderr,"(=%d) should be a positive integer (since it ",nVer);
        fprintf(stderr,"refers to the total number of vertices to read).\n");
        return 0;
    }

    // Open the *.obj file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"r");
    if (objFile==NULL)
    {
        PRINT_ERROR("In readVerticesInObjFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading vertices. ");

    // Potentially read the first white space characters
    readIntegerOut=fscanf(objFile," ");
    if (readIntegerOut)
    {
        PRINT_ERROR("In readVerticesInObjFile: wrong return ");
        fprintf(stderr,"(=%d) of the fscanf function ",readIntegerOut);
        fprintf(stderr,"(EOF=%d) while attempting to read the ",EOF);
        fprintf(stderr,"first white space characters (in case there was ");
        fprintf(stderr,"some).\n");
        closeTheFile(&objFile);
        return 0;
    }

    // Read the first character
    readChar=fgetc(objFile);
    if (readChar==EOF)
    {
        PRINT_ERROR("In readVerticesInObjFile: wrong end-of-file return ");
        fprintf(stderr,"(=%d) of the fgetc c-function.\n",readChar);
        closeTheFile(&objFile);
        return 0;
    }

    // Read and store each point coordinates
    counter=0;
    while (readChar=='v' && counter<nVer)
    {
        // Read the first coordinate
        readIntegerOut=fscanf(objFile," %lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }
        px[counter]=readDouble;

        // Read the second coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }
        py[counter]=readDouble;

        // Read the third coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }
        pz[counter]=readDouble;

        // Increment counter by one
        counter++;

        // Read the next character
        readChar=fgetc(objFile);
        if (readChar==EOF)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong end-of-file return ");
            fprintf(stderr,"(=%d) of the fgetc c-function.\n",readChar);
            closeTheFile(&objFile);
            return 0;
        }
    }

    // Check that the number of vertices read is correct
    if (readChar=='v' && counter==nVer)
    {
        PRINT_ERROR("In readVerticesInObjFile: it seems that there are more ");
        fprintf(stderr,"vertices than the expected number (=%d).\n",counter);
        closeTheFile(&objFile);
        return 0;
    }

    // Close the *.obj file
    if (fclose(objFile))
    {
        fprintf(stdout,"\nWarning in readVerticesInObjFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        objFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    objFile=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int countNumberOfTrianglesInObjFile(char nameFile[NAME_SIZE_MAX])
{
    int readChar=0, counter=0, readIntegerOut=0, readIntegerIn=0;
    FILE *objFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Open the *.obj file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"r");
    if (objFile==NULL)
    {
        PRINT_ERROR("In countNumberOfTrianglesInObjFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Counting the total number of triangles. ");

    // Skip the vertex part
    readChar=fgetc(objFile);
    while (readChar!='f' && readChar!=EOF)
    {
        readChar=fgetc(objFile);
    }

    if (readChar==EOF)
    {
        PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any triangle found.\n");
        closeTheFile(&objFile);
        return 0;
    }

    // Count the number of triangles
    counter=0;
    do
    {
        // Read the first point reference
        readIntegerOut=fscanf(objFile," %d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the second point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the third point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }

        // Increment counter by one and read the next character
        counter++;
        readChar=fgetc(objFile);

    } while (readChar=='f');

    // Close the *.obj file
    if (fclose(objFile))
    {
        fprintf(stdout,"\nWarning in countNumberOfTrianglesInObjFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        objFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file (%d triangles found).\n",counter);
    objFile=NULL;

    return counter;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readTrianglesInObjFile(char nameFile[NAME_SIZE_MAX], int nTri, int* ip1,
                                                             int* ip2, int* ip3)
{
    int readChar=0, counter=0, readIntegerOut=0, readIntegerIn=0;
    FILE *objFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    if (ip1==NULL || ip2==NULL || ip3==NULL)
    {
        PRINT_ERROR("In readTrianglesInObjFile: at least one of the input ");
        fprintf(stderr,"(int*) variable ip1=%p ip2=%p ",(void*)ip1,(void*)ip2);
        fprintf(stderr,"or ip3=%p does not point to a valid ",(void*)ip3);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the total number of triangles
    if (nTri<1)
    {
        PRINT_ERROR("In readTrianglesInObjFile: the input variable nTri ");
        fprintf(stderr,"(=%d) should be a positive integer (since it ",nTri);
        fprintf(stderr,"refers to the total number of triangles to read).\n");
        return 0;
    }

    // Open the *.obj file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"r");
    if (objFile==NULL)
    {
        PRINT_ERROR("In readTrianglesInObjFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading triangles. ");

    // Skip the vertices part
    readChar=fgetc(objFile);
    while (readChar!='f' && readChar!=EOF)
    {
        readChar=fgetc(objFile);
    }

    if (readChar==EOF)
    {
        PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any triangle found.\n");
        closeTheFile(&objFile);
        return 0;
    }

    // Read and store each triangle references
    counter=0;
    do
    {
        // Read the first point reference
        readIntegerOut=fscanf(objFile," %d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }
        ip1[counter]=readIntegerIn;

        // Read the second point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }
        ip2[counter]=readIntegerIn;

        // Read the third point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function.\n",readIntegerOut);
            closeTheFile(&objFile);
            return 0;
        }
        ip3[counter]=readIntegerIn;

        // Increment counter by one and read the next character
        counter++;
        readChar=fgetc(objFile);

    } while (readChar=='f');

    // Close the *.obj file
    if (fclose(objFile))
    {
        fprintf(stdout,"\nWarning in readTrianglesInObjFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        objFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    objFile=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingResizedObjFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                          double* py, double* pz, int nTri, int* ip1, int* ip2,
                                                                       int* ip3)
{
    int i=0;
    FILE *objFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    if (ip1==NULL || ip2==NULL || ip3==NULL)
    {
        PRINT_ERROR("In writingResizedObjFile: at least one of the input ");
        fprintf(stderr,"(int*) variable ip1=%p ip2=%p ",(void*)ip1,(void*)ip2);
        fprintf(stderr,"or ip3=%p does not point to a valid ",(void*)ip3);
        fprintf(stderr,"address.\n");
        return 0;
    }

    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In writingResizedObjFile: at least one of the input ");
        fprintf(stderr,"(double*) variable px=%p py=%p ",(void*)px,(void*)py);
        fprintf(stderr,"or pz=%p does not point to a valid ",(void*)pz);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of vertices and triangles
    if (nVer<1 || nTri<1)
    {
        PRINT_ERROR("In writingResizedObjFile: the input variables nVer ");
        fprintf(stderr,"(=%d) and nTri (=%d) should be positive ",nVer,nTri);
        fprintf(stderr,"integers.\n");
        return 0;
    }

    // Open the *.obj file (warning: overwrite the file if it already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"w+");
    if (objFile==NULL)
    {
        PRINT_ERROR("In writingResizedObjFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing resized data. ");

    // Writing the point coordinates
    fprintf(objFile,"\n");
    for (i=0; i<nVer; i++)
    {
        fprintf(objFile,"v %.8le %.8le %.8le \n",px[i],py[i],pz[i]);
    }
    fprintf(objFile,"\n");

    // Writing triangle references
    for (i=0; i<nTri; i++)
    {
        fprintf(objFile,"f %d %d %d \n",ip1[i],ip2[i],ip3[i]);
    }
    fprintf(objFile,"\n");

    // Close the *.obj file
    if (fclose(objFile))
    {
        fprintf(stdout,"\nWarning in writingResizedObjFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        objFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    objFile=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    char extension[2][7]={"input","output"};
    int i=0, length=0, nVer=0, nTri=0, *ip1=NULL, *ip2=NULL, *ip3=NULL;
    double *px=NULL, *py=NULL, *pz=NULL;

    // Check the number of arguments
    if (argc!=3)
    {
        PRINT_ERROR("In main: the program only takes two *.obj arguments ");
        fprintf(stderr,"instead of %d.\n",argc-1);
        return EXIT_FAILURE;
    }

    // Check the length and extension of the input and output file names
    for (i=1; i<argc; i++)
    {
        length=checkStringFromLength(argv[i],6,NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s file name should be a string ",extension[i-1]);
            fprintf(stderr,"of at most %d characters (and ",NAME_SIZE_MAX-1);
            fprintf(stderr,"strictly more than 4 to contain at least ");
            fprintf(stderr,"something more than the *.obj extension).\n");
            return EXIT_FAILURE;
        }

        if (argv[i][length-5]!='.' || argv[i][length-4]!='o' ||
                             argv[i][length-3]!='b' || argv[i][length-2]!='j' ||
                                                        argv[i][length-1]!='\0')
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s file name %s does not ",extension[i-1],argv[i]);
            fprintf(stderr,"end with the *.obj extension).\n");
            return EXIT_FAILURE;
        }
    }

    // Count the total number of vertices and triangles
    nVer=countNumberOfVerticesInObjFile(argv[1]);
    nTri=countNumberOfTrianglesInObjFile(argv[1]);
    if (nVer<1 || nTri<1)
    {
        PRINT_ERROR("In main: the total number of vertices ");
        fprintf(stderr,"(=%d) and triangles (=%d) should be ",nVer,nTri);
        fprintf(stderr,"positive integers.\n");
        return 0;
    }

    // Allocate memory for point coordinates and triangle references
    px=(double*)calloc(nVer,sizeof(double));
    py=(double*)calloc(nVer,sizeof(double));
    pz=(double*)calloc(nVer,sizeof(double));
    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(double*) variables px=%p py=%p ",(void*)px,(void*)py);
        fprintf(stderr,"and/or pz=%p.\n",(void*)pz);
        return EXIT_FAILURE;
    }

    ip1=(int*)calloc(nTri,sizeof(int));
    ip2=(int*)calloc(nTri,sizeof(int));
    ip3=(int*)calloc(nTri,sizeof(int));
    if (ip1==NULL || ip2==NULL || ip3==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(int*) variables ip1=%p ",(void*)ip1);
        fprintf(stderr,"ip2=%p and/or ip3=%p.\n",(void*)ip2,(void*)ip3);
        free(px);
        free(py);
        free(pz);
        px=NULL;
        py=NULL;
        pz=NULL;
        return EXIT_FAILURE;
    }

    // Read the point coordinates
    if (!readVerticesInObjFile(argv[1],nVer,px,py,pz))
    {
        PRINT_ERROR("In main: readVerticesInObjFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(px);
        free(py);
        free(pz);
        free(ip1);
        free(ip2);
        free(ip3);
        px=NULL;
        py=NULL;
        pz=NULL;
        ip1=NULL;
        ip2=NULL;
        ip3=NULL;
        return EXIT_FAILURE;
    }

    // Read the triangle references
    if (!readTrianglesInObjFile(argv[1],nTri,ip1,ip2,ip3))
    {
        PRINT_ERROR("In main: readTrianglesInObjFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(px);
        free(py);
        free(pz);
        free(ip1);
        free(ip2);
        free(ip3);
        px=NULL;
        py=NULL;
        pz=NULL;
        ip1=NULL;
        ip2=NULL;
        ip3=NULL;
        return EXIT_FAILURE;
    }

    // Scaling the point coordinates
    for (i=0; i<nVer; i++)
    {
#if (!BOHR_OR_NOT)
        px[i]*=BOHR_RADIUS;
        py[i]*=BOHR_RADIUS;
        pz[i]*=BOHR_RADIUS;
#else
        px[i]/=BOHR_RADIUS;
        py[i]/=BOHR_RADIUS;
        pz[i]/=BOHR_RADIUS;
#endif
    }

#if (!BOHR_OR_NOT)
    fprintf(stdout,"\nConverting the coordinates of the mesh vertices from ");
    fprintf(stdout,"Bohrs into Angstroms (1.0e-10m).\n");
#else
    fprintf(stdout,"\nConverting the coordinates of the mesh vertices from ");
    fprintf(stdout,"Angstroms (1.0e-10m) into Bohrs.\n");
#endif

    // Writing the resized *.obj file
    if (!writingResizedObjFile(argv[2],nVer,px,py,pz,nTri,ip1,ip2,ip3))
    {
        PRINT_ERROR("In main: writingResizedObjFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(px);
        free(py);
        free(pz);
        free(ip1);
        free(ip2);
        free(ip3);
        px=NULL;
        py=NULL;
        pz=NULL;
        ip1=NULL;
        ip2=NULL;
        ip3=NULL;
        return EXIT_FAILURE;
    }

    // Free the memory dynamically allocated
    free(px);
    free(py);
    free(pz);
    free(ip1);
    free(ip2);
    free(ip3);
    px=NULL;
    py=NULL;
    pz=NULL;
    ip1=NULL;
    ip2=NULL;
    ip3=NULL;

    return EXIT_SUCCESS;
}

