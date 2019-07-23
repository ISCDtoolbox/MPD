#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

// Boolean to include the computation of normals or not
#define INCLUDE_NORMALS 0

// Label of the internal tetrahedra belonging to the domain
#define LABEL_INTERNAL_TETRA 3

// Label of the boundary triangles belonging to the domain boundary
#define LABEL_BOUNDARY 10

#define PRINT_ERROR(stringToPrint)                                             \
do {                                                                           \
        fflush(stdout);                                                        \
        fprintf(stderr,"\nError encountered at line %d in ",__LINE__);         \
        fprintf(stderr,"%s file on %s",__FILE__,endTimerAtError());            \
        fprintf(stderr,"%s",stringToPrint);                                    \
} while (0)

typedef struct {
    double x;
    double y;
    double z;
    int label;
    double value;
} Point;

#if (INCLUDE_NORMALS)
  typedef struct {
    int p;
    double x;
    double y;
    double z;
  } Vector;
#endif

typedef struct {
    int p1;
    int p2;
    int p3;
    int label;
} Triangle;

typedef struct {
    int p1;
    int p2;
    int p3;
    int p4;
    int label;
} Tetrahedron;

typedef struct {
    int nver;
    Point* pver;
    int ntri;
    Triangle* ptri;
    int ntet;
    Tetrahedron* ptet;
#if (INCLUDE_NORMALS)
    int nnorm;
    Vector* pnorm;
#endif
} Mesh;

char* endTimerAtError(void);
void closeTheFile(FILE** pFileToClose);
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                             int maximumLength);
void initializeMeshStructure(Mesh* pMesh);
void freeMeshMemory(Mesh* pMesh);
int getNumberOfVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int readVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int getNumberOfTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int readTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int getNumberOfTetrahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int readTetrahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
#if (INCLUDE_NORMALS)
int getNumberOfNormalsInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int readNormalCoordinatesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int readNormalAtVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int extractingNewMesh(Mesh* pMesh, int* pNver, int* pNtet, int* pTet,
                                int* pNtri, int* pTri, int* pNnorm, int* pNorm);
int writingNewMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh, int nVer,
                       int nTet, int* pTet, int nTri, int* pTri, int nNorm,
                                                                    int* pNorm);
#else
int extractingNewMesh(Mesh* pMesh, int* pNver, int* pNtet, int* pTet,
                                                         int* pNtri, int* pTri);
int writingNewMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh, int nVer,
                                      int nTet, int* pTet, int nTri, int* pTri);
#endif

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
void initializeMeshStructure(Mesh* pMesh)
{
    if (pMesh!=NULL)
    {
        pMesh->nver=0;
        pMesh->pver=NULL;

        pMesh->ntri=0;
        pMesh->ptri=NULL;

        pMesh->ntet=0;
        pMesh->ptet=NULL;

#if (INCLUDE_NORMALS)
        pMesh->nnorm=0;
        pMesh->pnorm=NULL;
#endif
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void freeMeshMemory(Mesh* pMesh)
{
    if (pMesh!=NULL)
    {
        free(pMesh->pver);
        pMesh->pver=NULL;

        free(pMesh->ptri);
        pMesh->ptri=NULL;

        free(pMesh->ptet);
        pMesh->ptet=NULL;

#if (INCLUDE_NORMALS)
        free(pMesh->pnorm);
        pMesh->pnorm=NULL;
#endif
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int getNumberOfVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[8]={'\0'};
    int readChar=0, readIntegerOut=0, nVer=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In getNumberOfVerticesInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid address.\n");
        return 0;
    }

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
    pMesh->nver=nVer;

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
    fprintf(stdout,"Closing file (%d vertices found).\n",pMesh->nver);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[8]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    double readDouble=0.;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readVerticesInMeshFile: the input (Mesh*) variable ");
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of vertices and its associated pointer
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In readVerticesInMeshFile: the pMesh->nver variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nver);
        return 0;
    }

    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In readVerticesInMeshFile: the (Point*) variable ");
        fprintf(stderr,"pMesh->pver=%p does not point to ",(void*)pMesh->pver);
        fprintf(stderr,"a valid address.\n");
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

    if (strcmp(keyword,"ertices") || readIntegerIn!=pMesh->nver)
    {
        PRINT_ERROR("In readVerticesInMeshFile: expecting Vertices ");
        fprintf(stderr,"%d instead of ",pMesh->nver);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<pMesh->nver; i++)
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
        pMesh->pver[i].x=readDouble;

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
        pMesh->pver[i].y=readDouble;

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
        pMesh->pver[i].z=readDouble;

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
        pMesh->pver[i].label=readIntegerIn;
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
int getNumberOfTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[9]={'\0'};
    int readChar=0, readIntegerOut=0, nTri=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In getNumberOfTrianglesInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid address.\n");
        return 0;
    }

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
    pMesh->ntri=nTri;

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
    fprintf(stdout,"Closing file (%d triangles found).\n",pMesh->ntri);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readTrianglesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[9]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: the input (Mesh*) variable ");
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of triangles, vertices, and its associated pointer
    if (pMesh->ntri<1 || pMesh->nver<1)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: the variables pMesh->ntri ");
        fprintf(stderr,"(=%d) and pMesh->nver (=%d) ",pMesh->ntri,pMesh->nver);
        fprintf(stderr,"should be positive integers.\n");
        return 0;
    }

    if (pMesh->ptri==NULL)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: the (Triangle*) variable ");
        fprintf(stderr,"pMesh->ptri=%p does not point to ",(void*)pMesh->ptri);
        fprintf(stderr,"a valid address.\n");
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

    if (strcmp(keyword,"riangles") || readIntegerIn!=pMesh->ntri)
    {
        PRINT_ERROR("In readTrianglesInMeshFile: expecting Triangles ");
        fprintf(stderr,"%d instead of ",pMesh->ntri);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<pMesh->ntri; i++)
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

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: the first vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"triangle should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptri[i].p1=readIntegerIn;

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

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: the second vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"triangle should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptri[i].p2=readIntegerIn;

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

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: the third vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"triangle should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptri[i].p3=readIntegerIn;

        // Reading the vertex label
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTrianglesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the label associated with ");
            fprintf(stderr,"the %d-th triangle.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptri[i].label=readIntegerIn;
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
int getNumberOfTetrahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[10]={'\0'};
    int readChar=0, readIntegerOut=0, nTet=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the total number of mesh tetrahedra. ");

    // Skip all characters until the 'T' of Tetrahedra is found
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
                PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: wrong ");
                fprintf(stderr,"return of the fseek c-function while ");
                fprintf(stderr,"checking if the '%c' ",(char)readChar);
                fprintf(stderr,"encountered corresponds to the one of ");
                fprintf(stderr,"the Tetrahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'T' of Tetrahedra (and not RequiredTetrahedra)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'T' of Tetrahedra
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'T' of the Tetrahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='T')
            {
                PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: expecting ");
                fprintf(stderr,"'T' instead of %c while ",(char)readChar);
                fprintf(stderr,"attempting to read to the first ");
                fprintf(stderr,"character of the Tetrahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            if (!boolean)
            {
                // Reading the letter after 'T' to distinguish
                // Tetrahedra from Triangles, Tangent, Time, ...
                readChar=fgetc(meshFile);
                if (readChar==EOF)
                {
                    PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: wrong ");
                    fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                    fprintf(stderr,"fgetc c-function while attempting to ");
                    fprintf(stderr,"read the letter after 'T' during the ");
                    fprintf(stderr,"search of the Tetrahedra keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                else if (readChar!='e')
                {
                    boolean=1;
                }

                if (fseek(meshFile,-1,SEEK_CUR))
                {
                    PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: wrong ");
                    fprintf(stderr,"return of the fseek c-function while ");
                    fprintf(stderr,"checking if the '%c' ",(char)readChar);
                    fprintf(stderr,"encountered corresponds to the one of ");
                    fprintf(stderr,"the Tetrahedra keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any tetrahedra found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of tetrahedra
    readStringOut=fgets(keyword,10,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&nTet);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of tetrahedra.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"etrahedra") || nTet<1)
    {
        PRINT_ERROR("In getNumberOfTetrahedraInMeshFile: expecting ");
        fprintf(stderr,"Tetrahedra Ntet instead of ");
        fprintf(stderr,"%c%s %d (positive ",(char)readChar,keyword,nTet);
        fprintf(stderr,"value).\n");
        closeTheFile(&meshFile);
        return 0;
    }
    pMesh->ntet=nTet;

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfTetrahedraInMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file (%d tetrahedra found).\n",pMesh->ntet);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readTetrahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[10]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readTetrahedraInMeshFile: the input (Mesh*) variable ");
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of tetrathedra and its associated pointer
    if (pMesh->ntet<1 || pMesh->nver<1)
    {
        PRINT_ERROR("In readTetrathedraInMeshFile: the variables pMesh->ntet ");
        fprintf(stderr,"(=%d) and pMesh->nver (=%d) ",pMesh->ntet,pMesh->nver);
        fprintf(stderr,"should be a positive integer.\n");
        return 0;
    }

    if (pMesh->ptet==NULL)
    {
        PRINT_ERROR("In readTetrathedraInMeshFile: the (Tetrahedron*) ");
        fprintf(stderr,"variable pMesh->ptet=%p does not ",(void*)pMesh->ptet);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("InreadTetrathedraInMeshFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading tetrathedra. ");

    // Skip all characters until the 'T' of Tetrathedra is found
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
                PRINT_ERROR("In readTetrathedraInMeshFile: wrong return of ");
                fprintf(stderr,"the fseek c-function while checking if the ");
                fprintf(stderr,"'%c' encountered corresponds ",(char)readChar);
                fprintf(stderr,"to the one of the Tetrathedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'T' of Tetrathedra(and not RequiredTetrathedra)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'T' of Tetrathedra
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In readTetrathedraInMeshFile: wrong end-of-file ");
                fprintf(stderr,"return (=%d) of the fgetc ",readChar);
                fprintf(stderr,"c-function while attempting to read the 'T' ");
                fprintf(stderr,"of the Tetrathedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='T')
            {
                PRINT_ERROR("In readTetrathedraInMeshFile: expecting 'T' ");
                fprintf(stderr,"instead of %c while ",(char)readChar);
                fprintf(stderr,"attempting to read to the first character of ");
                fprintf(stderr,"the Tetrathedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            if (!boolean)
            {
                // Reading the letter after 'T' to distinguish
                // Tetrathedra from Triangles, Tangent, Time, ...
                readChar=fgetc(meshFile);
                if (readChar==EOF)
                {
                    PRINT_ERROR("In readTetrathedraInMeshFile: wrong ");
                    fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                    fprintf(stderr,"fgetc c-function while attempting to ");
                    fprintf(stderr,"read the letter after 'T' during the ");
                    fprintf(stderr,"search of the Tetrathedra keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                else if (readChar!='e')
                {
                    boolean=1;
                }

                if (fseek(meshFile,-1,SEEK_CUR))
                {
                    PRINT_ERROR("In readTetrathedraInMeshFile: wrong return ");
                    fprintf(stderr,"of the fseek c-function while checking ");
                    fprintf(stderr,"if the '%c' encountered ",(char)readChar);
                    fprintf(stderr,"corresponds to the one of the ");
                    fprintf(stderr,"Tetrathedra keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In readTetrathedraInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any tetrathedra found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of tetrathedra
    readStringOut=fgets(keyword,10,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readTetrathedraInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of tetrathedra.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"etrahedra") || readIntegerIn!=pMesh->ntet)
    {
        PRINT_ERROR("In readTetrathedraInMeshFile: expecting Tetrathedra ");
        fprintf(stderr,"%d instead of ",pMesh->ntet);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<pMesh->ntet; i++)
    {
        // Reading the first vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the first vertex ");
            fprintf(stderr,"reference of the %d-th tetrahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: the first vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"tetrahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptet[i].p1=readIntegerIn;

        // Reading the second vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the second vertex ");
            fprintf(stderr,"reference of the %d-th tetrahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: the second vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"tetrahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptet[i].p2=readIntegerIn;

        // Reading the third vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the third vertex ");
            fprintf(stderr,"reference of the %d-th tetrahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: the third vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"tetrahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptet[i].p3=readIntegerIn;

        // Reading the fourth vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the fourth vertex ");
            fprintf(stderr,"reference of the %d-th tetrahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: the fourth vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"tetrahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptet[i].p4=readIntegerIn;

        // Reading the vertex label
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readTetrathedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the label associated with ");
            fprintf(stderr,"the %d-th tetrahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->ptet[i].label=readIntegerIn;
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in readTetrathedraInMeshFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

#if (INCLUDE_NORMALS)
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int getNumberOfNormalsInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[7]={'\0'};
    int readChar=0, readIntegerOut=0, nNorm=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In getNumberOfNormalsInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In getNumberOfNormalsInMeshFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the total number of normal vectors. ");

    // Skip all characters until the 'N' of Normals is found
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
        else if (readChar=='N')
        {
            // Read the middle part of the keyword (N)ormal
            readStringOut=fgets(keyword,6,meshFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In getNumberOfNormalsInMeshFile: wrong return ");
                fprintf(stderr,"of the standard ");
                fprintf(stderr,"fgets=%p c-function in ",(void*)readStringOut);
                fprintf(stderr,"the attempt of reading the number of normal ");
                fprintf(stderr,"vectors.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            if (strcmp(keyword,"ormal"))
            {
                PRINT_ERROR("In getNumberOfNormalsInMeshFile: expecting ");
                fprintf(stderr,"Normal instead of ");
                fprintf(stderr,"%c%s.\n",(char)readChar,keyword);
                closeTheFile(&meshFile);
                return 0;
            }

            // Distinguish the Normals from NormalAtVertices keyword
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfNormalsInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the letter after Normal during the ");
                fprintf(stderr,"search of the Normals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar=='s')
            {
                if (fseek(meshFile,-6,SEEK_CUR))
                {
                    PRINT_ERROR("In getNumberOfNormalsInMeshFile: wrong ");
                    fprintf(stderr,"return of the fseek c-function while ");
                    fprintf(stderr,"checking if the '%c' ",(char)readChar);
                    fprintf(stderr,"encountered corresponds to the one of ");
                    fprintf(stderr,"the Normals keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                readChar='N';
                boolean=0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfNormalsInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any normal vector found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of normal vectors
    readStringOut=fgets(keyword,7,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&nNorm);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfNormalsInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of normal vectors.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"ormals") || nNorm<1)
    {
        PRINT_ERROR("In getNumberOfNormalsInMeshFile: expecting Normals ");
        fprintf(stderr,"Nnorm instead of %c%s ",(char)readChar,keyword);
        fprintf(stderr,"%d (positive value).\n",nNorm);
        closeTheFile(&meshFile);
        return 0;
    }
    pMesh->nnorm=nNorm;

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfNormalsInMeshFile function: ");
        fprintf(stdout,"we could not close the %s file properly.\n",nameFile);
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file (%d normal vectors found).\n",pMesh->nnorm);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readNormalCoordinatesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[7]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    double readDouble=0.;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readNormalCoordinatesInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the number of normal vectors and its associated pointer
    if (pMesh->nnorm<1)
    {
        PRINT_ERROR("In readNormalCoordinatesInMeshFile: the pMesh->nnorm ");
        fprintf(stderr,"variable (=%d) should be a positive ",pMesh->nnorm);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    if (pMesh->pnorm==NULL)
    {
        PRINT_ERROR("In readNormalCoordinatesInMeshFile: the (Vector*) ");
        fprintf(stderr,"variable pMesh->pnorm=%p does ",(void*)pMesh->pnorm);
        fprintf(stderr,"not point to a valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In readNormalCoordinatesInMeshFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading coordinates of normal vectors. ");

    // Skip all characters until the 'N' of Normals is found
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
        else if (readChar=='N')
        {
            // Read the middle part of the keyword (N)ormal
            readStringOut=fgets(keyword,6,meshFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong ");
                fprintf(stderr,"return of the standard ");
                fprintf(stderr,"fgets=%p c-function in ",(void*)readStringOut);
                fprintf(stderr,"the attempt of reading the coordinates of ");
                fprintf(stderr,"normal vectors.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            if (strcmp(keyword,"ormal"))
            {
                PRINT_ERROR("In readNormalCoordinatesInMeshFile: expecting ");
                fprintf(stderr,"Normal instead of ");
                fprintf(stderr,"%c%s.\n",(char)readChar,keyword);
                closeTheFile(&meshFile);
                return 0;
            }

            // Distinguish the Normals from NormalAtVertices keyword
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the letter after Normal during the ");
                fprintf(stderr,"search of the Normals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar=='s')
            {
                if (fseek(meshFile,-6,SEEK_CUR))
                {
                    PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong ");
                    fprintf(stderr,"return of the fseek c-function while ");
                    fprintf(stderr,"checking if the '%c' ",(char)readChar);
                    fprintf(stderr,"encountered corresponds to the one of ");
                    fprintf(stderr,"the Normals keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                readChar='N';
                boolean=0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any normal vector found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of normal vectors
    readStringOut=fgets(keyword,7,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of normal vectors.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"ormals") || readIntegerIn!=pMesh->nnorm)
    {
        PRINT_ERROR("In readNormalCoordinatesInMeshFile: expecting Normals ");
        fprintf(stderr,"%d instead of ",pMesh->nnorm);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<pMesh->nnorm; i++)
    {
        // Reading the first coordinate
        readIntegerOut=fscanf(meshFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the first coordinate of ");
            fprintf(stderr,"the %d-th normal vector.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pnorm[i].x=readDouble;

        // Reading the second coordinate
        readIntegerOut=fscanf(meshFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the second coordinate of ");
            fprintf(stderr,"the %d-th normal vector.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pnorm[i].y=readDouble;

        // Reading the third coordinate
        readIntegerOut=fscanf(meshFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readNormalCoordinatesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the third coordinate of ");
            fprintf(stderr,"the %d-th normal vector.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pnorm[i].z=readDouble;
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in readNormalCoordinatesInMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stderr,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readNormalAtVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[16]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0, iNorm=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readNormalAtVerticesInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the number of normal vectors and its associated pointer
    if (pMesh->nnorm<1 || pMesh->nver<1)
    {
        PRINT_ERROR("In readNormalAtVerticesInMeshFile: the variables ");
        fprintf(stderr,"pMesh->nnorm (=%d) and pMesh->nver ",pMesh->nnorm);
        fprintf(stderr,"(=%d) should be positive integers.\n",pMesh->nver);
        return 0;
    }

    if (pMesh->pnorm==NULL)
    {
        PRINT_ERROR("In readNormalAtVerticesInMeshFile: the (Vector*) ");
        fprintf(stderr,"variable pMesh->pnorm=%p does ",(void*)pMesh->pnorm);
        fprintf(stderr,"not point to a valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In readNormalAtVerticesInMeshFile: we could not open ");
        fprintf(stderr,"the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading vertices associated with normal vectors. ");

    // Skip all characters until the 'N' of NormalAtVertices is found
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
        else if (readChar=='N')
        {
            // Read the middle part of the keyword (N)ormal(AtVertices)
            readStringOut=fgets(keyword,6,meshFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readNormalAtVerticesInMeshFile: wrong return ");
                fprintf(stderr,"of the standard ");
                fprintf(stderr,"fgets=%p c-function in ",(void*)readStringOut);
                fprintf(stderr,"the attempt of reading the vertices ");
                fprintf(stderr,"associated with the normal vectors.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            if (strcmp(keyword,"ormal"))
            {
                PRINT_ERROR("In readNormalAtVerticesInMeshFile: expecting ");
                fprintf(stderr,"Normal instead of ");
                fprintf(stderr,"%c%s.\n",(char)readChar,keyword);
                closeTheFile(&meshFile);
                return 0;
            }

            // Distinguish the NormalAtVertices from Normals keyword
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In readNormalAtVerticesInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the letter after Normal during the ");
                fprintf(stderr,"search of the Normals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar=='A')
            {
                // Distinguish NormalAtVertices from NormalAtTetrahedra
                readStringOut=fgets(keyword,3,meshFile);
                if (readStringOut==NULL)
                {
                    PRINT_ERROR("In readNormalAtVerticesInMeshFile: wrong ");
                    fprintf(stderr,"return of the standard ");
                    fprintf(stderr,"fgets=%p ",(void*)readStringOut);
                    fprintf(stderr,"c-function in the attempt of reading the ");
                    fprintf(stderr,"NormalAtVertices keyword.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (!strcmp(keyword,"tV"))
                {
                    if (fseek(meshFile,-8,SEEK_CUR))
                    {
                        PRINT_ERROR("In readNormalAtVerticesInMeshFile: ");
                        fprintf(stderr,"wrong return of the fseek c-function ");
                        fprintf(stderr,"while reading the NormalAtVertices ");
                        fprintf(stderr,"keyword.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    readChar='N';
                    boolean=0;
                }
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In readNormalAtVerticesInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any normal vector found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of normal vectors
    readStringOut=fgets(keyword,16,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readNormalAtVerticesInMeshFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of normal vectors.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"ormalAtVertices") || readIntegerIn!=pMesh->nnorm)
    {
        PRINT_ERROR("In readNormalAtVerticesInMeshFile: expecting ");
        fprintf(stderr,"NormalAtVertices %d instead of ",pMesh->nnorm);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    boolean=0;
    for (i=0; i<pMesh->nnorm; i++)
    {
        // Reading the vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readNormalAtVerticesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the vertex reference of ");
            fprintf(stderr,"the %d-th normal vector.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readNormalAtVerticesInMeshFile: the vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"normal vector should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }

        // Reading the vector reference
        readIntegerOut=fscanf(meshFile,"%d ",&iNorm);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readNormalAtVerticesInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the vector reference of ");
            fprintf(stderr,"the %d-th normal vector.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (iNorm<1 || iNorm>pMesh->nnorm)
        {
            PRINT_ERROR("In readNormalAtVerticesInMeshFile: the vector ");
            fprintf(stderr,"reference (=%d) of the %d-th normal ",iNorm,i+1);
            fprintf(stderr,"vector should be a positive integer not strictly ");
            fprintf(stderr,"greater than the total number of normal ");
            fprintf(stderr,"vectors (=%d).\n",pMesh->nnorm);
            closeTheFile(&meshFile);
            return 0;
        }

        if (iNorm-1!=i && !boolean)
        {
            fprintf(stdout,"\nWarning in the readNormalAtVerticesInMeshFile ");
            fprintf(stdout,"function: the normal vectors are not numbered in ");
            fprintf(stdout,"increasing order during their mesh description ");
            fprintf(stdout,"(the %d-th normal vector read is referred ",i+1);
            fprintf(stdout,"to as %d).\n",iNorm);
            boolean=1;
        }
        pMesh->pnorm[iNorm-1].p=readIntegerIn;
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in readNormalAtVerticesInMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stderr,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}
#endif

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#if (INCLUDE_NORMALS)
int extractingNewMesh(Mesh* pMesh, int* pNver, int* pNtet, int* pTet,
                                 int* pNtri, int* pTri, int* pNnorm, int* pNorm)
#else
int extractingNewMesh(Mesh* pMesh, int* pNver, int* pNtet, int* pTet,
                                                          int* pNtri, int* pTri)
#endif
{
    int i=0, iMax=0, j=0, k=0, kMax=0, nVer=0, nTri=0, nTet=0, boolean=0;
#if (INCLUDE_NORMALS)
    int nNorm=0;
#endif

    // Check if the input pointers are set to NULL
    boolean=(pMesh==NULL || pNver==NULL || pNtet==NULL || pTet==NULL ||
                                                     pNtri==NULL || pTri==NULL);
#if (INCLUDE_NORMALS)
    boolean=(boolean || pNnorm==NULL || pNorm==NULL);
#endif
    if (boolean)
    {
        PRINT_ERROR("In extractingNewMesh: at least one of the input ");
        fprintf(stderr,"variables pMesh=%p, ",(void*)pMesh);
        fprintf(stderr,"pNver=%p, ",(void*)pNver);
        fprintf(stderr,"pNtet=%p, ",(void*)pNtet);
        fprintf(stderr,"pTet=%p, ",(void*)pTet);
        fprintf(stderr,"pNtri=%p, ",(void*)pNtri);
        fprintf(stderr,"pTri=%p",(void*)pTri);
#if (INCLUDE_NORMALS)
        fprintf(stderr,", pNnorm=%p, or pNorm=%p",(void*)pNnorm,(void*)pNorm);
#endif
        fprintf(stderr," does not point to a valid adress.\n");
        return 0;
    }

    // Check the number of mesh elements and their associated pointers
    boolean=(pMesh->nver<1 || pMesh->ntri<1 || pMesh->ntet<1);
#if (INCLUDE_NORMALS)
    boolean=(boolean || pMesh->nnorm<1);
#endif
    if (boolean)
    {
        PRINT_ERROR("In extractingNewMesh: at least one of the variables ");
        fprintf(stderr,"pMesh->nver (=%d), ",pMesh->nver);
        fprintf(stderr,"pMesh->ntri (=%d), ",pMesh->ntri);
        fprintf(stderr,"pMesh->ntet (=%d) ",pMesh->ntet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or pMesh->nnorm (=%d) ",pMesh->nnorm);
#endif
        fprintf(stderr,"is not a positive integer.\n");
        return 0;
    }

    boolean=(pMesh->pver==NULL || pMesh->ptri==NULL || pMesh->ptet==NULL);
#if (INCLUDE_NORMALS)
    boolean=(boolean || pMesh->pnorm==NULL);
#endif
    if (boolean)
    {
        PRINT_ERROR("In extractingNewMesh: at least one of the variables ");
        fprintf(stderr,"pMesh->pver=%p, ",(void*)pMesh->pver);
        fprintf(stderr,"pMesh->ptri=%p, ",(void*)pMesh->ptri);
        fprintf(stderr,"pMesh->ptet=%p ",(void*)pMesh->ptet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or pMesh->pnorm=%p ",(void*)pMesh->pnorm);
#endif
        fprintf(stderr,"does not point to a valid adress.\n");
        return 0;
    }

    // Set the new tetrahedron adjacency
    nVer=0;
    nTet=0;
    iMax=pMesh->ntet;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptet[i].label==LABEL_INTERNAL_TETRA)
        {
            nTet++;

            // Add first vertex if not already contained in pTet
            pMesh->pver[pMesh->ptet[i].p1-1].label=1010;
            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p1==pMesh->ptet[j].p1)
                    {
                        pTet[4*i]=pTet[4*j];
                        break;
                    }
                    else if (pMesh->ptet[i].p1==pMesh->ptet[j].p2)
                    {
                        pTet[4*i]=pTet[4*j+1];
                        break;
                    }
                    else if (pMesh->ptet[i].p1==pMesh->ptet[j].p3)
                    {
                        pTet[4*i]=pTet[4*j+2];
                        break;
                    }
                    else if (pMesh->ptet[i].p1==pMesh->ptet[j].p4)
                    {
                        pTet[4*i]=pTet[4*j+3];
                        break;
                    }
                }
            }
            if (j==i)
            {
                pTet[4*i]=nVer;
                nVer++;
            }

            // Add second vertex if not already contained in pTet
            pMesh->pver[pMesh->ptet[i].p2-1].label=1010;
            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p2==pMesh->ptet[j].p1)
                    {
                        pTet[4*i+1]=pTet[4*j];
                        break;
                    }
                    else if (pMesh->ptet[i].p2==pMesh->ptet[j].p2)
                    {
                        pTet[4*i+1]=pTet[4*j+1];
                        break;
                    }
                    else if (pMesh->ptet[i].p2==pMesh->ptet[j].p3)
                    {
                        pTet[4*i+1]=pTet[4*j+2];
                        break;
                    }
                    else if (pMesh->ptet[i].p2==pMesh->ptet[j].p4)
                    {
                        pTet[4*i+1]=pTet[4*j+3];
                        break;
                    }
                }
            }
            if (j==i)
            {
                pTet[4*i+1]=nVer;
                nVer++;
            }

            // Add third vertex if not already contained in pTet
            pMesh->pver[pMesh->ptet[i].p3-1].label=1010;
            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p3==pMesh->ptet[j].p1)
                    {
                        pTet[4*i+2]=pTet[4*j];
                        break;
                    }
                    else if (pMesh->ptet[i].p3==pMesh->ptet[j].p2)
                    {
                        pTet[4*i+2]=pTet[4*j+1];
                        break;
                    }
                    else if (pMesh->ptet[i].p3==pMesh->ptet[j].p3)
                    {
                        pTet[4*i+2]=pTet[4*j+2];
                        break;
                    }
                    else if (pMesh->ptet[i].p3==pMesh->ptet[j].p4)
                    {
                        pTet[4*i+2]=pTet[4*j+3];
                        break;
                    }
                }
            }
            if (j==i)
            {
                pTet[4*i+2]=nVer;
                nVer++;
            }

            // Add fourth vertex if not already contained in pTet
            pMesh->pver[pMesh->ptet[i].p4-1].label=1010;
            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p4==pMesh->ptet[j].p1)
                    {
                        pTet[4*i+3]=pTet[4*j];
                        break;
                    }
                    else if (pMesh->ptet[i].p4==pMesh->ptet[j].p2)
                    {
                        pTet[4*i+3]=pTet[4*j+1];
                        break;
                    }
                    else if (pMesh->ptet[i].p4==pMesh->ptet[j].p3)
                    {
                        pTet[4*i+3]=pTet[4*j+2];
                        break;
                    }
                    else if (pMesh->ptet[i].p4==pMesh->ptet[j].p4)
                    {
                        pTet[4*i+3]=pTet[4*j+3];
                        break;
                    }
                }
            }
            if (j==i)
            {
                pTet[4*i+3]=nVer;
                nVer++;
            }
        }
    }

    // Set the new triangle adjacency
    nTri=0;
    iMax=pMesh->ntri;
    kMax=pMesh->ntet;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptri[i].label==LABEL_BOUNDARY)
        {
            nTri++;

            // Add first vertex if not already contained in pTri
            pMesh->pver[pMesh->ptri[i].p1-1].label=1011;
            for (j=0; j<i; j++)
            {
                if (pMesh->ptri[j].label==LABEL_BOUNDARY)
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
                for (k=0; k<kMax; k++)
                {
                    if (pMesh->ptet[k].label==LABEL_INTERNAL_TETRA)
                    {
                        if (pMesh->ptri[i].p1==pMesh->ptet[k].p1)
                        {
                            pTri[3*i]=pTet[4*k];
                            break;
                        }
                        else if (pMesh->ptri[i].p1==pMesh->ptet[k].p2)
                        {
                            pTri[3*i]=pTet[4*k+1];
                            break;
                        }
                        else if (pMesh->ptri[i].p1==pMesh->ptet[k].p3)
                        {
                            pTri[3*i]=pTet[4*k+2];
                            break;
                        }
                        else if (pMesh->ptri[i].p1==pMesh->ptet[k].p4)
                        {
                            pTri[3*i]=pTet[4*k+3];
                            break;
                        }
                    }
                }
            }

            // Add second vertex if not already contained in pTri
            pMesh->pver[pMesh->ptri[i].p2-1].label=1011;
            for (j=0; j<i; j++)
            {
                if (pMesh->ptri[j].label==LABEL_BOUNDARY)
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
                for (k=0; k<kMax; k++)
                {
                    if (pMesh->ptet[k].label==LABEL_INTERNAL_TETRA)
                    {
                        if (pMesh->ptri[i].p2==pMesh->ptet[k].p1)
                        {
                            pTri[3*i+1]=pTet[4*k];
                            break;
                        }
                        else if (pMesh->ptri[i].p2==pMesh->ptet[k].p2)
                        {
                            pTri[3*i+1]=pTet[4*k+1];
                            break;
                        }
                        else if (pMesh->ptri[i].p2==pMesh->ptet[k].p3)
                        {
                            pTri[3*i+1]=pTet[4*k+2];
                            break;
                        }
                        else if (pMesh->ptri[i].p2==pMesh->ptet[k].p4)
                        {
                            pTri[3*i+1]=pTet[4*k+3];
                            break;
                        }
                    }
                }
            }

            // Add third vertex if not already contained in pTri
            pMesh->pver[pMesh->ptri[i].p3-1].label=1011;
            for (j=0; j<i; j++)
            {
                if (pMesh->ptri[j].label==LABEL_BOUNDARY)
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
                for (k=0; k<kMax; k++)
                {
                    if (pMesh->ptet[k].label==LABEL_INTERNAL_TETRA)
                    {
                        if (pMesh->ptri[i].p3==pMesh->ptet[k].p1)
                        {
                            pTri[3*i+2]=pTet[4*k];
                            break;
                        }
                        else if (pMesh->ptri[i].p3==pMesh->ptet[k].p2)
                        {
                            pTri[3*i+2]=pTet[4*k+1];
                            break;
                        }
                        else if (pMesh->ptri[i].p3==pMesh->ptet[k].p3)
                        {
                            pTri[3*i+2]=pTet[4*k+2];
                            break;
                        }
                        else if (pMesh->ptri[i].p3==pMesh->ptet[k].p4)
                        {
                            pTri[3*i+2]=pTet[4*k+3];
                            break;
                        }
                    }
                }
            }
        }
    }

#if (INCLUDE_NORMALS)
    // Set the new normal adjacency
    nNorm=0;
    iMax=pMesh->nnorm;
    kMax=pMesh->ntet;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->pver[pMesh->pnorm[i].p-1].label==1011)
        {
            nNorm++;

            for (k=0; k<kMax; k++)
            {
                if (pMesh->ptet[k].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->pnorm[i].p==pMesh->ptet[k].p1)
                    {
                        pNorm[i]=pTet[4*k];
                        break;
                    }
                    else if (pMesh->pnorm[i].p==pMesh->ptet[k].p2)
                    {
                        pNorm[i]=pTet[4*k+1];
                        break;
                    }
                    else if (pMesh->pnorm[i].p==pMesh->ptet[k].p3)
                    {
                        pNorm[i]=pTet[4*k+2];
                        break;
                    }
                    else if (pMesh->pnorm[i].p==pMesh->ptet[k].p4)
                    {
                        pNorm[i]=pTet[4*k+3];
                        break;
                    }
                }
            }
        }
    }
#endif

    // Set the new number of mesh elements
    *pNver=nVer;
    *pNtet=nTet;
    *pNtri=nTri;
#if (INCLUDE_NORMALS)
    *pNnorm=nNorm;
#endif

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#if (INCLUDE_NORMALS)
int writingNewMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh, int nVer,
                       int nTet, int* pTet, int nTri, int* pTri, int nNorm,
                                                                     int* pNorm)
#else
int writingNewMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh, int nVer,
                                       int nTet, int* pTet, int nTri, int* pTri)
#endif
{
    int i=0, iMax=0, j=0, boolean=0;
#if (INCLUDE_NORMALS)
    int jNorm=0;
#endif
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    boolean=(pMesh==NULL || pTet==NULL || pTri==NULL);
#if (INCLUDE_NORMALS)
    boolean=(boolean || pNorm==NULL);
#endif
    if (boolean)
    {
        PRINT_ERROR("In writingNewMeshFile: at least one of the input ");
        fprintf(stderr,"variables pMesh=%p, ",(void*)pMesh);
        fprintf(stderr,"pTri=%p, pTet=%p ",(void*)pTri,(void*)pTet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or pNorm=%p ",(void*)pNorm);
#endif
        fprintf(stderr,"does not point to a valid adress.\n");
        return 0;
    }

    // Check the number of new mesh elements
    boolean=(nVer<1 || nTet<1 || nTri<1);
#if (INCLUDE_NORMALS)
    boolean=(boolean || nNorm<1);
#endif
    if (boolean)
    {
        PRINT_ERROR("In writingNewMeshFile: at least one of the input ");
        fprintf(stderr,"variables nVer (=%d), nTri (=%d), ",nVer,nTri);
        fprintf(stderr,"nTet (=%d) ",nTet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or nNorm (=%d) ",nNorm);
#endif
        fprintf(stderr,"is not a positive integer.\n");
        return 0;
    }

    // Check the number of mesh elements and their associated pointers
    boolean=(pMesh->nver<1 || pMesh->ntri<1 || pMesh->ntet<1);
#if (INCLUDE_NORMALS)
    boolean=(boolean || pMesh->nnorm<1);
#endif
    if (boolean)
    {
        PRINT_ERROR("In writingNewMeshFile: at least one of the variables ");
        fprintf(stderr,"pMesh->nver (=%d), ",pMesh->nver);
        fprintf(stderr,"pMesh->ntri (=%d), ",pMesh->ntri);
        fprintf(stderr,"pMesh->ntet (=%d) ",pMesh->ntet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or pMesh->nnorm (=%d) ",pMesh->nnorm);
#endif
        fprintf(stderr,"is not a positive integer.\n");
        return 0;
    }

    boolean=(pMesh->pver==NULL || pMesh->ptri==NULL || pMesh->ptet==NULL);
#if (INCLUDE_NORMALS)
    boolean=(boolean || pMesh->pnorm==NULL);
#endif
    if (boolean)
    {
        PRINT_ERROR("In writingNewMeshFile: at least one of the variables ");
        fprintf(stderr,"pMesh->pver=%p, ",(void*)pMesh->pver);
        fprintf(stderr,"pMesh->ptri=%p, ",(void*)pMesh->ptri);
        fprintf(stderr,"pMesh->ptet=%p ",(void*)pMesh->ptet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or pMesh->pnorm=%p ",(void*)pMesh->pnorm);
#endif
        fprintf(stderr,"does not point to a valid adress.\n");
        return 0;
    }

    // Opening *.mesh file (warning: reset and overwrite file if already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"w+");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In writingNewMeshFile: we were not able to open and ");
        fprintf(stderr,"write into the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing new mesh. ");

    fprintf(meshFile,"MeshVersionFormatted 2\n\nDimension 3\n");
    fprintf(meshFile,"\nVertices\n%d\n",nVer);

    iMax=pMesh->ntet;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptet[i].label==LABEL_INTERNAL_TETRA)
        {
            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p1==pMesh->ptet[j].p1 ||
                                         pMesh->ptet[i].p1==pMesh->ptet[j].p2 ||
                                         pMesh->ptet[i].p1==pMesh->ptet[j].p3 ||
                                           pMesh->ptet[i].p1==pMesh->ptet[j].p4)
                    {
                        break;
                    }
                }
            }
            if (j==i)
            {
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p1-1].x);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p1-1].y);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p1-1].z);
                if (pMesh->pver[pMesh->ptet[i].p1-1].label==1011)
                {
                    fprintf(meshFile,"1 \n");
                }
                else
                {
                    fprintf(meshFile,"0 \n");
                }
            }

            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p2==pMesh->ptet[j].p1 ||
                                         pMesh->ptet[i].p2==pMesh->ptet[j].p2 ||
                                         pMesh->ptet[i].p2==pMesh->ptet[j].p3 ||
                                           pMesh->ptet[i].p2==pMesh->ptet[j].p4)
                    {
                        break;
                    }
                }
            }
            if (j==i)
            {
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p2-1].x);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p2-1].y);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p2-1].z);
                if (pMesh->pver[pMesh->ptet[i].p2-1].label==1011)
                {
                    fprintf(meshFile,"1 \n");
                }
                else
                {
                    fprintf(meshFile,"0 \n");
                }
            }

            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p3==pMesh->ptet[j].p1 ||
                                         pMesh->ptet[i].p3==pMesh->ptet[j].p2 ||
                                         pMesh->ptet[i].p3==pMesh->ptet[j].p3 ||
                                           pMesh->ptet[i].p3==pMesh->ptet[j].p4)
                    {
                        break;
                    }
                }
            }
            if (j==i)
            {
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p3-1].x);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p3-1].y);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p3-1].z);
                if (pMesh->pver[pMesh->ptet[i].p3-1].label==1011)
                {
                    fprintf(meshFile,"1 \n");
                }
                else
                {
                    fprintf(meshFile,"0 \n");
                }
            }

            for (j=0; j<i; j++)
            {
                if (pMesh->ptet[j].label==LABEL_INTERNAL_TETRA)
                {
                    if (pMesh->ptet[i].p4==pMesh->ptet[j].p1 ||
                                         pMesh->ptet[i].p4==pMesh->ptet[j].p2 ||
                                         pMesh->ptet[i].p4==pMesh->ptet[j].p3 ||
                                           pMesh->ptet[i].p4==pMesh->ptet[j].p4)
                    {
                        break;
                    }
                }
            }
            if (j==i)
            {
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p4-1].x);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p4-1].y);
                fprintf(meshFile,"%.8le ",pMesh->pver[pMesh->ptet[i].p4-1].z);
                if (pMesh->pver[pMesh->ptet[i].p4-1].label==1011)
                {
                    fprintf(meshFile,"1 \n");
                }
                else
                {
                    fprintf(meshFile,"0 \n");
                }
            }
        }
    }

    fprintf(meshFile,"\nTriangles\n%d\n",nTri);
    iMax=pMesh->ntri;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptri[i].label==LABEL_BOUNDARY)
        {
            fprintf(meshFile,"%d ",pTri[3*i]+1);
            fprintf(meshFile,"%d ",pTri[3*i+1]+1);
            fprintf(meshFile,"%d 1 \n",pTri[3*i+2]+1);
        }
    }


    fprintf(meshFile,"\nTetrahedra\n%d\n",nTet);
    iMax=pMesh->ntet;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptet[i].label==LABEL_INTERNAL_TETRA)
        {
            fprintf(meshFile,"%d ",pTet[4*i]+1);
            fprintf(meshFile,"%d ",pTet[4*i+1]+1);
            fprintf(meshFile,"%d ",pTet[4*i+2]+1);
            fprintf(meshFile,"%d 0 \n",pTet[4*i+3]+1);
        }
    }

#if (INCLUDE_NORMALS)
    fprintf(meshFile,"\nNormals\n%d\n",nNorm);
    iMax=pMesh->nnorm;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->pver[pMesh->pnorm[i].p-1].label==1011)
        {
            fprintf(meshFile,"%.8le ",pMesh->pnorm[i].x);
            fprintf(meshFile,"%.8le ",pMesh->pnorm[i].y);
            fprintf(meshFile,"%.8le \n",pMesh->pnorm[i].y);
        }
    }

    fprintf(meshFile,"\nNormalAtVertices\n%d\n",nNorm);
    jNorm=0;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->pver[pMesh->pnorm[i].p-1].label==1011)
        {
            fprintf(meshFile,"%d %d \n",pNorm[i]+1,jNorm+1);
            jNorm++;
        }
    }
#endif

    fprintf(meshFile,"\nEnd\n\n");

    // Closing the output *.mesh file
    if (fclose(meshFile))
    {
        PRINT_ERROR("In writingNewMeshFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",nameFile);
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int i=0, length=0, boolean=0, nVer=0, nTet=0, nTri=0;
    int *pTet=NULL, *pTri=NULL;
#if (INCLUDE_NORMALS)
    int nNorm=0, *pNorm=NULL;
#endif
    Mesh mesh;

    // Initialize Mesh structure to zero (and NULL for pointers)
    initializeMeshStructure(&mesh);

    // Check the number of arguments
    if (argc!=3)
    {
        PRINT_ERROR("In main: the program only takes two arguments instead ");
        fprintf(stderr,"of %d.\n",argc-1);
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Check the length and extension of the input and output *.mesh file names
    for (i=1; i<argc; i++)
    {
        length=checkStringFromLength(argv[i],7,NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            if (i==1)
            {
                fprintf(stderr,"input *.mesh file name should be a string ");
            }
            else
            {
                fprintf(stderr,"output *.mesh file name should be a string ");
            }
            fprintf(stderr,"of at most %d characters (and ",NAME_SIZE_MAX-1);
            fprintf(stderr,"strictly more than 7 to contain at least ");
            fprintf(stderr,"something more than the *.mesh extension).\n");
            freeMeshMemory(&mesh);
            return EXIT_FAILURE;
        }

        boolean=(argv[i][length-6]=='.' && argv[i][length-5]=='m');
        boolean=(boolean && argv[i][length-4]=='e' && argv[i][length-3]=='s');
        boolean=(boolean && argv[i][length-2]=='h' && argv[i][length-1]=='\0');
        if (!boolean)
        {
            PRINT_ERROR("In main: the ");
            if (i==1)
            {
                fprintf(stderr,"input *.mesh extension does not correspond ");
            }
            else
            {
                fprintf(stderr,"output *.mesh extension does not correspond ");
            }
            fprintf(stderr,"to the one of the %s file name.\n",argv[i]);
            freeMeshMemory(&mesh);
            return EXIT_FAILURE;
        }
    }

    // Get the total number of vertices in the *.mesh input file
    if (!getNumberOfVerticesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: getNumberOfVerticesInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return 0;
    }

    // Get the total number of triangles in the *.mesh input file
    if (!getNumberOfTrianglesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: getNumberOfTrianglesInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return 0;
    }

    // Get the total number of tetrahedra in the *.mesh input file
    if (!getNumberOfTetrahedraInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: getNumberOfTetrahedraInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return 0;
    }

#if (INCLUDE_NORMALS)
    // Get the total number of normal vectors in the *.mesh input file
    if (!getNumberOfNormalsInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: getNumberOfNormalsInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return 0;
    }
#endif

    // Allocate memory for the mesh elements
    mesh.pver=(Point*)calloc(mesh.nver,sizeof(Point));
    mesh.ptri=(Triangle*)calloc(mesh.ntri,sizeof(Triangle));
    mesh.ptet=(Tetrahedron*)calloc(mesh.ntet,sizeof(Tetrahedron));
    boolean=(mesh.pver==NULL || mesh.ptri==NULL || mesh.ptet==NULL);
#if (INCLUDE_NORMALS)
    mesh.pnorm=(Vector*)calloc(mesh.nnorm,sizeof(Vector));
    boolean=(boolean || mesh.pnorm==NULL);
#endif
    if (boolean)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"variables mesh.pver=%p ",(void*)mesh.pver);
        fprintf(stderr,"mesh.ptri=%p, ",(void*)mesh.ptri);
        fprintf(stderr,"mesh.ptet=%p ",(void*)mesh.ptet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or mesh.pnorm=%p ",(void*)mesh.pnorm);
#endif
        fprintf(stderr,"in Mesh structure.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Read the point coordinates
    if (!readVerticesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readVerticesInMeshFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Read the triangles references
    if (!readTrianglesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readTrianglesInMeshFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Read the tetrahedra references
    if (!readTetrahedraInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readTetrahedraInMeshFile function returned ");
        fprintf(stderr,"zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

#if (INCLUDE_NORMALS)
    // Read the coordinates of the normal vectors
    if (!readNormalCoordinatesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readNormalCoordinatesInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return 0;
    }

    // Read normal vectors at mesh vertices
    if (!readNormalAtVerticesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readNormalAtVerticesInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return 0;
    }
#endif

    // Allocate memory for the mesh adjacency
    pTri=(int*)calloc(3*mesh.ntri,sizeof(int));
    pTet=(int*)calloc(4*mesh.ntet,sizeof(int));
    boolean=(pTri==NULL || pTet==NULL);
#if (INCLUDE_NORMALS)
    pNorm=(int*)calloc(mesh.nnorm,sizeof(int));
    boolean=(boolean || pNorm==NULL);
#endif
    if (boolean)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"variables pTri=%p, pTet=%p ",(void*)pTri,(void*)pTet);
#if (INCLUDE_NORMALS)
        fprintf(stderr,"or pNorm=%p ",(void*)pNorm);
#endif
        fprintf(stderr,"for building the mesh adjacency.\n");
        freeMeshMemory(&mesh);
        free(pTri);
        pTri=NULL;
        free(pTet);
        pTet=NULL;
#if (INCLUDE_NORMALS)
        free(pNorm);
        pNorm=NULL;
#endif
        return EXIT_FAILURE;
    }


    // Extract the new internal mesh
    fprintf(stdout,"\nExtracting the new mesh (tetrahedra labelled ");
    fprintf(stdout,"%d and triangles labelled ",LABEL_INTERNAL_TETRA);
    fprintf(stdout,"%d).\n",LABEL_BOUNDARY);
#if (INCLUDE_NORMALS)
    if(!extractingNewMesh(&mesh,&nVer,&nTet,pTet,&nTri,pTri,&nNorm,pNorm))
    {
        PRINT_ERROR("In main: extractingNewMesh function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        free(pTri);
        free(pTet);
        free(pNorm);
        pTri=NULL;
        pTet=NULL;
        pNorm=NULL;
        return EXIT_FAILURE;
    }
#else
    if(!extractingNewMesh(&mesh,&nVer,&nTet,pTet,&nTri,pTri))
    {
        PRINT_ERROR("In main: extractingNewMesh function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        free(pTri);
        free(pTet);
        pTri=NULL;
        pTet=NULL;
        return EXIT_FAILURE;
    }
#endif

    // writing the output *.mesh file
#if (INCLUDE_NORMALS)
    if (!writingNewMeshFile(argv[2],&mesh,nVer,nTet,pTet,nTri,pTri,nNorm,pNorm))
    {
        PRINT_ERROR("In main: writingNewMeshFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        free(pTri);
        free(pTet);
        free(pNorm);
        pTri=NULL;
        pTet=NULL;
        pNorm=NULL;
        return EXIT_FAILURE;
    }
#else
    if (!writingNewMeshFile(argv[2],&mesh,nVer,nTet,pTet,nTri,pTri))
    {
        PRINT_ERROR("In main: writingNewMeshFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        free(pTri);
        free(pTet);
        pTri=NULL;
        pTet=NULL;
        return EXIT_FAILURE;
    }
#endif

    // Free the allocated memory
    freeMeshMemory(&mesh);
    free(pTri);
    free(pTet);
    pTri=NULL;
    pTet=NULL;
#if (INCLUDE_NORMALS)
    free(pNorm);
    pNorm=NULL;
#endif

    return EXIT_SUCCESS;
}

