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
int getNumberOfVerticesInMeshFile(char nameFile[NAME_SIZE_MAX]);
int readVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                                                        double* py, double* pz);
int getNumberOfTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX]);
int readTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX], int nTri, int* ip1,
                                                            int* ip2, int* ip3);
int writingObjFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
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
        PRINT_ERROR("In getNumberOfVerticesInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of vertices.\n");
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
    meshFile=NULL;
    fprintf(stdout,"Closing file (%d vertices found).\n",nVer);

    return nVer;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
                                                         double* py, double* pz)
{
    char *readStringOut=NULL, keyword[8]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    double readDouble=0.;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In readVerticesInMeshFile: at least one of the input ");
        fprintf(stderr,"(double*) variables px=%p py=%p ",(void*)px,(void*)py);
        fprintf(stderr,"and/or pz=%p does not point to a valid ",(void*)pz);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of vertices
    if (nVer<1)
    {
        PRINT_ERROR("In readVerticesInMeshFile: the input variable nVer ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nVer);
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In readVerticesInMeshFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading vertices. ");

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
                PRINT_ERROR("In readVerticesInMeshFile: wrong return of the ");
                fprintf(stderr,"fseek c-function while checking if the ");
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
                PRINT_ERROR("In readVerticesInMeshFile: wrong end-of-file ");
                fprintf(stderr,"return (=%d) of the fgetc ",readChar);
                fprintf(stderr,"c-function while attempting to read the 'V' ");
                fprintf(stderr,"of the Vertices keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='V')
            {
                PRINT_ERROR("In readVerticesInMeshFile: expecting 'V' ");
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
        PRINT_ERROR("In readVerticesInMeshFile: wrong return ");
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
        PRINT_ERROR("In readVerticesInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of vertices.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"ertices") || readIntegerIn!=nVer)
    {
        PRINT_ERROR("In readVerticesInMeshFile: expecting Vertices ");
        fprintf(stderr,"%d instead of ",nVer);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<nVer; i++)
    {
        // Reading the first coordinate
        readIntegerOut=fscanf(meshFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the first coordinate of ");
            fprintf(stderr,"the %d-th vertex.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        px[i]=readDouble;

        // Reading the second coordinate
        readIntegerOut=fscanf(meshFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the second coordinate of ");
            fprintf(stderr,"the %d-th vertex.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        py[i]=readDouble;

        // Reading the third coordinate
        readIntegerOut=fscanf(meshFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the third coordinate of ");
            fprintf(stderr,"the %d-th vertex.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pz[i]=readDouble;

        // Reading the vertex label
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readVerticesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the label associated with ");
            fprintf(stderr,"the %d-th vertex.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in readVerticesInMeshFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int getNumberOfTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX])
{
    char *readStringOut=NULL, keyword[9]={'\0'};
    int readChar=0, readIntegerOut=0, nTri=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In getNumberOfTrianglesInMeshFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the total number of mesh triangles. ");

    // Skip all characters until the 'T' of Triangles is found
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
        else if (readChar=='T')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In getNumberOfTrianglesInMeshFile: wrong return ");
                fprintf(stderr,"of the fseek c-function while checking if ");
                fprintf(stderr,"the '%c' encountered ",(char)readChar);
                fprintf(stderr,"corresponds to the one of the Triangles ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'T' of Triangles (and not RequiredTriangles)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'T' of Triangles
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfTrianglesInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'T' of the Triangles keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='T')
            {
                PRINT_ERROR("In getNumberOfTrianglesInMeshFile: expecting ");
                fprintf(stderr,"'T' instead of %c while ",(char)readChar);
                fprintf(stderr,"attempting to read to the first ");
                fprintf(stderr,"character of the Triangles keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            if (!boolean)
            {
                // Reading the letter after 'T' to distinguish
                // Triangles from Tetrahedra, Tangent, Time, ...
                readChar=fgetc(meshFile);
                if (readChar==EOF)
                {
                    PRINT_ERROR("In getNumberOfTrianglesInMeshFile: wrong ");
                    fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                    fprintf(stderr,"fgetc c-function while attempting to ");
                    fprintf(stderr,"read the letter after 'T' during the ");
                    fprintf(stderr,"search of the Triangles keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                else if (readChar!='r')
                {
                    boolean=1;
                }

                if (fseek(meshFile,-1,SEEK_CUR))
                {
                    PRINT_ERROR("In getNumberOfTrianglesInMeshFile: wrong ");
                    fprintf(stderr,"return of the fseek c-function while ");
                    fprintf(stderr,"checking if the '%c' ",(char)readChar);
                    fprintf(stderr,"encountered corresponds to the one of ");
                    fprintf(stderr,"the Triangles keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfTrianglesInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any triangles found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of triangles
    readStringOut=fgets(keyword,9,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&nTri);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfTrianglesInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of triangles.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"riangles") || nTri<1)
    {
        PRINT_ERROR("In getNumberOfTrianglesInMeshFile: expecting Triangles ");
        fprintf(stderr,"Ntri instead of %c%s %d ",(char)readChar,keyword,nTri);
        fprintf(stderr,"(positive value).\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfTrianglesInMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file (%d triangles found).\n",nTri);

    return nTri;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX], int nTri, int* ip1,
                                                             int* ip2, int* ip3)
{
    char *readStringOut=NULL, keyword[9]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    if (ip1==NULL || ip2==NULL || ip3==NULL)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: at least one of the input ");
        fprintf(stderr,"(int*) variables ip1=%p ip2=%p ",(void*)ip1,(void*)ip2);
        fprintf(stderr,"and/or ip3=%p does not point to a valid ",(void*)ip3);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of triangles
    if (nTri<1)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: the input variable nTri ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nTri);
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("InreadTrianglesInMeshFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading triangles. ");

    // Skip all characters until the 'T' of Triangles is found
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
        else if (readChar=='T')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In readTrianglesInMeshFile: wrong return of the ");
                fprintf(stderr,"fseek c-function while checking if the ");
                fprintf(stderr,"'%c' encountered corresponds ",(char)readChar);
                fprintf(stderr,"to the one of the Triangles keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'T' of Triangles (and not RequiredTriangles)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'T' of Triangles
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In readTrianglesInMeshFile: wrong end-of-file ");
                fprintf(stderr,"return (=%d) of the fgetc ",readChar);
                fprintf(stderr,"c-function while attempting to read the 'T' ");
                fprintf(stderr,"of the Triangles keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='T')
            {
                PRINT_ERROR("In readTrianglesInMeshFile: expecting 'T' ");
                fprintf(stderr,"instead of %c while ",(char)readChar);
                fprintf(stderr,"attempting to read to the first character of ");
                fprintf(stderr,"the Triangles keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            if (!boolean)
            {
                // Reading the letter after 'T' to distinguish
                // Triangles from Tetrahedra, Tangent, Time, ...
                readChar=fgetc(meshFile);
                if (readChar==EOF)
                {
                    PRINT_ERROR("In readTrianglesInMeshFile: wrong ");
                    fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                    fprintf(stderr,"fgetc c-function while attempting to ");
                    fprintf(stderr,"read the letter after 'T' during the ");
                    fprintf(stderr,"search of the Triangles keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                else if (readChar!='r')
                {
                    boolean=1;
                }

                if (fseek(meshFile,-1,SEEK_CUR))
                {
                    PRINT_ERROR("In readTrianglesInMeshFile: wrong return ");
                    fprintf(stderr,"of the fseek c-function while checking ");
                    fprintf(stderr,"if the '%c' encountered ",(char)readChar);
                    fprintf(stderr,"corresponds to the one of the Triangles ");
                    fprintf(stderr,"keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any triangles found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of triangles
    readStringOut=fgets(keyword,9,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of triangles.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"riangles") || readIntegerIn!=nTri)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: expecting Triangles ");
        fprintf(stderr,"%d instead of ",nTri);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<nTri; i++)
    {
        // Reading the first vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the first vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        ip1[i]=readIntegerIn;

        // Reading the second vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the second vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        ip2[i]=readIntegerIn;

        // Reading the third vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the third vertex ");
            fprintf(stderr,"reference of the %d-th triangle.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        ip3[i]=readIntegerIn;

        // Reading the vertex label
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the label associated with ");
            fprintf(stderr,"the %d-th vertex.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in readTrianglesInMeshFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingObjFile(char nameFile[NAME_SIZE_MAX], int nVer, double* px,
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
        PRINT_ERROR("In writingObjFile: at least one of the input (int*) ");
        fprintf(stderr,"variables ip1=%p ip2=%p and/or ",(void*)ip1,(void*)ip2);
        fprintf(stderr,"ip3=%p does not point to a valid ",(void*)ip3);
        fprintf(stderr,"address.\n");
        return 0;
    }

    if (px==NULL || py==NULL || pz==NULL)
    {
        PRINT_ERROR("In writingObjFile: at least one of the input (double*) ");
        fprintf(stderr,"variables px=%p py=%p and/or ",(void*)px,(void*)py);
        fprintf(stderr,"pz=%p does not point to a valid ",(void*)pz);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of vertices and triangles
    if (nVer<1 || nTri<1)
    {
        PRINT_ERROR("In writingObjFile: the input variables nVer ");
        fprintf(stderr,"(=%d) and nTri (=%d) should be positive ",nVer,nTri);
        fprintf(stderr,"integers.\n");
        return 0;
    }

    // Open the *.obj file (warning: overwrite the file if it already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"w+");
    if (objFile==NULL)
    {
        PRINT_ERROR("In writingObjFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing data. ");

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
        fprintf(stdout,"\nWarning in writingObjFile function: we could not ");
        fprintf(stdout,"close the %s file properly.\n",nameFile);
        objFile=NULL;
        return 0;
    }
    objFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    char extension[2][13]={"input *.mesh","output *.obj"};
    int i=0, nVer=0, nTri=0, length=0, extensionLength[2]={7,6}, boolean=0;
    int *ip1=NULL, *ip2=NULL, *ip3=NULL;
    double *px=NULL, *py=NULL, *pz=NULL;

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

    // Get the total number of vertices and triangles in the *.mesh input file
    nVer=getNumberOfVerticesInMeshFile(argv[1]);
    nTri=getNumberOfTrianglesInMeshFile(argv[1]);
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
    if (!readVerticesInMeshFile(argv[1],nVer,px,py,pz))
    {
        PRINT_ERROR("In main: readVerticesInMeshFile function returned zero ");
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
    if (!readTrianglesInMeshFile(argv[1],nTri,ip1,ip2,ip3))
    {
        PRINT_ERROR("In main: readTrianglesInMeshFile function returned zero ");
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

    fprintf(stdout,"\nExtracting the vertices and triangles from the ");
    fprintf(stdout,"three-dimensional mesh.\n");

    // Writing the *.obj output file
    if (!writingObjFile(argv[2],nVer,px,py,pz,nTri,ip1,ip2,ip3))
    {
        PRINT_ERROR("In main: writingObjFile function returned zero instead ");
        fprintf(stderr,"of one.\n");
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

