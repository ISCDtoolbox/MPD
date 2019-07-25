#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
int readVerticesInObjFile(char nameFile[NAME_SIZE_MAX], int vMin, double* px,
                                                        double* py, double* pz);
int countNumberOfTrianglesInObjFile(char nameFile[NAME_SIZE_MAX]);
int readTrianglesInObjFile(char nameFile[NAME_SIZE_MAX], int tMin, int nVer,
                                                  int* ip1, int* ip2, int* ip3);
int mergingObjFiles(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
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
        fprintf(stderr,"return (=%d) of the fgetc c-function ",readChar);
        fprintf(stderr,"while attempting to read the coordinates of the ");
        fprintf(stderr,"first vertex.\n");
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
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the first coordinate ");
            fprintf(stderr,"of the %d-th vertex.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the second coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the second coordinate ");
            fprintf(stderr,"of the %d-th vertex.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the third coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the third coordinate ");
            fprintf(stderr,"of the %d-th vertex.\n",counter+1);
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
            fprintf(stderr,"c-function while attempting to read the ");
            fprintf(stderr,"coordinates of the %d-th vertex.\n",counter);
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
int readVerticesInObjFile(char nameFile[NAME_SIZE_MAX], int vMin, double* px,
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

    // Check the initial position in the list of vertices
    if (vMin<0)
    {
        PRINT_ERROR("In readVerticesInObjFile: the input variable ");
        fprintf(stderr,"vMin (=%d) should not be a negative integer.\n",vMin);
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
        fprintf(stderr,"return (=%d) of the fgetc c-function ",readChar);
        fprintf(stderr,"while attempting to read the coordinates of the ");
        fprintf(stderr,"first vertex.\n");
        closeTheFile(&objFile);
        return 0;
    }

    // Read and store each point coordinates
    counter=0;
    while (readChar=='v')
    {
        // Read the first coordinate
        readIntegerOut=fscanf(objFile," %lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the first coordinate ");
            fprintf(stderr,"of the %d-th vertex.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }
        px[vMin+counter]=readDouble;

        // Read the second coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the second coordinate ");
            fprintf(stderr,"of the %d-th vertex.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }
        py[vMin+counter]=readDouble;

        // Read the third coordinate
        readIntegerOut=fscanf(objFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the third coordinate ");
            fprintf(stderr,"of the %d-th vertex.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }
        pz[vMin+counter]=readDouble;

        // Increment counter by one
        counter++;

        // Read the next character
        readChar=fgetc(objFile);
        if (readChar==EOF)
        {
            PRINT_ERROR("In readVerticesInObjFile: wrong end-of-file return ");
            fprintf(stderr,"(=%d) of the fgetc c-function while ",readChar);
            fprintf(stderr,"attempting to read the coordinates of the ");
            fprintf(stderr,"%d-th vertex.\n",counter);
            closeTheFile(&objFile);
            return 0;
        }
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
        // Check the character read
        if (readChar==EOF)
        {
            PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fgetc c-function while ",readChar);
            fprintf(stderr,"attempting to read the vertex references of the ");
            fprintf(stderr,"%d-th triangle.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the first point reference
        readIntegerOut=fscanf(objFile," %d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the first vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the second point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the second vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the third point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In countNumberOfTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the third vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",counter+1);
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
int readTrianglesInObjFile(char nameFile[NAME_SIZE_MAX], int tMin, int nVer,
                                                   int* ip1, int* ip2, int* ip3)
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

    // Check the initial position in the list of vertices and triangles
    if (tMin<0 || nVer<0)
    {
        PRINT_ERROR("In readTrianglesInObjFile: the input variables ");
        fprintf(stderr,"tMin (=%d) or nVer (=%d) should not be ",tMin,nVer);
        fprintf(stderr,"negative integers.\n");
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
        // Check the character read
        if (readChar==EOF)
        {
            PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fgetc c-function while ",readChar);
            fprintf(stderr,"attempting to read the vertex references of the ");
            fprintf(stderr,"%d-th triangle.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }

        // Read the first point reference
        readIntegerOut=fscanf(objFile," %d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the first vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }
        ip1[tMin+counter]=readIntegerIn+nVer;

        // Read the second point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the second vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }
        ip2[tMin+counter]=readIntegerIn+nVer;

        // Read the third point reference
        readIntegerOut=fscanf(objFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInObjFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function ",readIntegerOut);
            fprintf(stderr,"while attempting to read the third vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",counter+1);
            closeTheFile(&objFile);
            return 0;
        }
        ip3[tMin+counter]=readIntegerIn+nVer;

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
int mergingObjFiles(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
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
        PRINT_ERROR("In mergingObjFiles: at least one of the input ");
        fprintf(stderr,"(double*) variable ip1=%p ",(void*)ip1);
        fprintf(stderr,"ip2=%p or ip3=%p does not ",(void*)ip2,(void*)ip3);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In mergingObjFiles: at least one of the input ");
        fprintf(stderr,"(double*) variable px=%p py=%p ",(void*)px,(void*)py);
        fprintf(stderr,"or pz=%p does not point to a valid ",(void*)pz);
        fprintf(stderr,"address.\n");
        return 0;
    }

    if (nVer<1 || nTri<1)
    {
        PRINT_ERROR("In mergingObjFiles: the input variables nVer ");
        fprintf(stderr,"(=%d) or nTri (=%d) should be positive ",nVer,nTri);
        fprintf(stderr,"integers.\n");
        return 0;
    }

    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"w+");
    if (objFile==NULL)
    {
        PRINT_ERROR("In mergingObjFiles: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing merged data. ");

    for (i=0; i<nVer; i++)
    {
        fprintf(objFile,"v %.8le %.8le %.8le \n",px[i],py[i],pz[i]); 
    }
    fprintf(objFile,"\n");
    for (i=0; i<nTri; i++)
    {
        if (ip1[i]<1 || ip1[i]>nVer)
        {
            PRINT_ERROR("In mergingObjFiles : the first vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th triangle ",ip1[i],i+1);
            fprintf(stderr,"should a positive integer not strictly greater ");
            fprintf(stderr,"than the total number of mesh vertices ");
            fprintf(stderr,"(=%d).\n",nVer);
            closeTheFile(&objFile);
            return 0;
        }
        if (ip2[i]<1 || ip2[i]>nVer)
        {
            PRINT_ERROR("In mergingObjFiles : the second vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th triangle ",ip2[i],i+1);
            fprintf(stderr,"should a positive integer not strictly greater ");
            fprintf(stderr,"than the total number of mesh vertices ");
            fprintf(stderr,"(=%d).\n",nVer);
            closeTheFile(&objFile);
            return 0;
        }
        if (ip3[i]<1 || ip3[i]>nVer)
        {
            PRINT_ERROR("In mergingObjFiles : the third vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th triangle ",ip3[i],i+1);
            fprintf(stderr,"should a positive integer not strictly greater ");
            fprintf(stderr,"than the total number of mesh vertices ");
            fprintf(stderr,"(=%d).\n",nVer);
            closeTheFile(&objFile);
            return 0;
        }
        fprintf(objFile,"f %d %d %d \n",ip1[i],ip2[i],ip3[i]); 
    }
    fprintf(objFile,"\n");

    if (fclose(objFile))
    {
        fprintf(stdout,"\nWarning in mergingObjFiles function: we ");
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
    size_t length=0;
    int i=0, *nVer=NULL, *nTri=NULL, *ip1=NULL, *ip2=NULL, *ip3=NULL, vMin=0;
    int tMin=0;
    double *px=NULL, *py=NULL, *pz=NULL;

    // Check the number of *.obj files
    if (argc<4)
    {
        PRINT_ERROR("In main: the program takes at least three arguments ");
        fprintf(stderr,"instead of %d.\n",argc-1);
        return EXIT_FAILURE;
    }

    // Check the length and extension of the input and output *.obj file names
    for (i=1; i<argc; i++)
    {
        length=checkStringFromLength(argv[i],6,NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            if (i==argc-1)
            {
                fprintf(stderr,"output *.obj file name should be a string of ");
            }
            else
            {
                fprintf(stderr,"input *.obj file names should be strings of ");
            }
            fprintf(stderr,"at most %d characters (and ",NAME_SIZE_MAX-1);
            fprintf(stderr,"strictly more than 4 to contain at least ");
            fprintf(stderr,"something more than the *.obj extension).\n");
            return EXIT_FAILURE;
        }

        if (argv[i][length-5]!='.' || argv[i][length-4]!='o' ||
                             argv[i][length-3]!='b' || argv[i][length-2]!='j' ||
                                                        argv[i][length-1]!='\0')
        {
            PRINT_ERROR("In main: the ");
            if (i==argc-1)
            {
                fprintf(stderr,"output file name %s does not end ",argv[i]);
            }
            else
            {
                fprintf(stderr,"input file name %s does not end ",argv[i]);
            }

            fprintf(stderr,"with the *.obj extension.\n");
            return EXIT_FAILURE;
        }
    }

    // Allocate memory for number of vertices and triangles
    length=argc-1;
    nVer=(int*)calloc(length,sizeof(int));
    nTri=(int*)calloc(length,sizeof(int));
    if (nVer==NULL || nTri==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(int*) variables nVer=%p and/or ",(void*)nVer);
        fprintf(stderr,"nTri=%p.\n",(void*)nTri);
        free(nVer);
        free(nTri);
        nVer=NULL;
        nTri=NULL;
        return 0;
    }

    // Count the number of triangles and vertices in all input *.obj files
    nVer[argc-2]=0;
    nTri[argc-2]=0;
    for (i=0; i<argc-2; i++)
    {
        nVer[i]=countNumberOfVerticesInObjFile(argv[i+1]);
        nTri[i]=countNumberOfTrianglesInObjFile(argv[i+1]);
        if (nVer[i]<1 || nTri[i]<1)
        {
            PRINT_ERROR("In main: countNumberOfVerticesInObjFile or ");
            fprintf(stderr,"countNumberOfTrianglesInObjFile function ");
            fprintf(stderr,"failed.\n");
            free(nVer);
            free(nTri);
            nVer=NULL;
            nTri=NULL;
            return EXIT_FAILURE;
        }
        nVer[argc-2]+=nVer[i];
        nTri[argc-2]+=nTri[i];
    }

    // Allocate memory for point coordinates and triangles vertex references
    length=nVer[argc-2];
    px=(double*)calloc(length,sizeof(double));
    py=(double*)calloc(length,sizeof(double));
    pz=(double*)calloc(length,sizeof(double));

    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(double*) variables px=%p, py=%p ",(void*)px,(void*)py);
        fprintf(stderr,"and/or pz=%p.\n",(void*)pz);
        free(nVer);
        free(nTri);
        free(px);
        free(py);
        free(pz);
        nVer=NULL;
        nTri=NULL;
        px=NULL;
        py=NULL;
        pz=NULL;
        return EXIT_FAILURE;
    }

    length=nTri[argc-2];
    ip1=(int*)calloc(length,sizeof(int));
    ip2=(int*)calloc(length,sizeof(int));
    ip3=(int*)calloc(length,sizeof(int));

    if (ip1==NULL || ip2==NULL || ip3==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(int*) variables ip1=%p ",(void*)ip1);
        fprintf(stderr,"ip2=%p and/or ip3=%p.\n",(void*)ip2,(void*)ip3);
        free(nVer);
        free(nTri);
        free(px);
        free(py);
        free(pz);
        free(ip1);
        free(ip2);
        free(ip3);
        nVer=NULL;
        nTri=NULL;
        px=NULL;
        py=NULL;
        pz=NULL;
        ip1=NULL;
        ip2=NULL;
        ip3=NULL;
        return EXIT_FAILURE;
    }

    // Read vertices and triangles
    vMin=0;
    tMin=0;
    for (i=0; i<argc-2; i++)
    {
        if (!readVerticesInObjFile(argv[i+1],vMin,px,py,pz))
        {
            PRINT_ERROR("In main: readVerticesInObjFile function returned ");
            fprintf(stderr,"zero instead of one.\n");
            free(nVer);
            free(nTri);
            free(px);
            free(py);
            free(pz);
            free(ip1);
            free(ip2);
            free(ip3);
            nVer=NULL;
            nTri=NULL;
            px=NULL;
            py=NULL;
            pz=NULL;
            ip1=NULL;
            ip2=NULL;
            ip3=NULL;
            return EXIT_FAILURE;
        }
        if (!readTrianglesInObjFile(argv[i+1],tMin,vMin,ip1,ip2,ip3))
        {
            PRINT_ERROR("In main: readTrianglesInObjFile function returned ");
            fprintf(stderr,"zero instead of one.\n");
            free(nVer);
            free(nTri);
            free(px);
            free(py);
            free(pz);
            free(ip1);
            free(ip2);
            free(ip3);
            nVer=NULL;
            nTri=NULL;
            px=NULL;
            py=NULL;
            pz=NULL;
            ip1=NULL;
            ip2=NULL;
            ip3=NULL;
            return EXIT_FAILURE;
        }
        vMin+=nVer[i];
        tMin+=nTri[i];
    }

    // Writing merged data in the output *.obj file
    fprintf(stdout,"\nMerging all the input *.obj files (%d ",nVer[argc-2]);
    fprintf(stdout,"vertices and %d triangles).\n",nTri[argc-2]);
    if (!mergingObjFiles(argv[argc-1],nVer[argc-2],px,py,pz,nTri[argc-2],ip1,
                                                                       ip2,ip3))
    {
        PRINT_ERROR("In main: mergingObjFiles function returned zero instead ");
        fprintf(stderr,"of one.\n");
        free(nVer);
        free(nTri);
        free(px);
        free(py);
        free(pz);
        free(ip1);
        free(ip2);
        free(ip3);
        nVer=NULL;
        nTri=NULL;
        px=NULL;
        py=NULL;
        pz=NULL;
        ip1=NULL;
        ip2=NULL;
        ip3=NULL;
        return EXIT_FAILURE;
    }

    // Free the dynamically allocated memory
    free(nVer);
    free(nTri);
    free(px);
    free(py);
    free(pz);
    free(ip1);
    free(ip2);
    free(ip3);
    nVer=NULL;
    nTri=NULL;
    px=NULL;
    py=NULL;
    pz=NULL;
    ip1=NULL;
    ip2=NULL;
    ip3=NULL;

    return EXIT_SUCCESS;
}

