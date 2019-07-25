#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

// Label of the internal tetrahedra belonging to the domain
#define LABEL_INTERNAL_TETRA 3

// Label of the boundary triangles belonging to the domain boundary
#define LABEL_BOUNDARY 10

// Convert Bohr into Angstrom (1.e-10m) if set to 0 at compilation
// and do the converse (Angstrom->Bohr) if it is set to 1
#define BOHR_OR_NOT 1

// Bohr radius in Angstrom (1.e-10m)
#define BOHR_RADIUS 0.5291772109217

#define DEF_ABS(a) (((a) > 0.0) ? (a) : (-(a)))

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
double computeVolumeTetrahedron(Point* pp1, Point* pp2, Point* pp3, Point* pp4);
double computeVolumeOfDomain(Mesh* pMesh);
int readLsSolFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int writingDomainInObjFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);

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

    // Check the number of triangles and its associated pointer
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
        fprintf(stderr,"should be positive integers.\n");
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

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
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

    // Remark: we compute 6*volumeOfTetrahedron
    determinant=a11*(a22*a33-a32*a23)-a21*(a12*a33-a32*a13)+
                                                          a31*(a12*a23-a22*a13);

    return DEF_ABS(determinant);
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
double computeVolumeOfDomain(Mesh* pMesh)
{
    int i=0, iMax=0;
    double volume=0.;

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In computeVolumeOfDomain: the input (Mesh*) variable ");
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    // Check the number of vtetrahedra and its associated pointer
    if (pMesh->ntet<1)
    {
        PRINT_ERROR("In computeVolumeOfDomain: the pMesh->ntet variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntet);
        return 0;
    }

    if (pMesh->ptet==NULL)
    {
        PRINT_ERROR("In computeVolumeOfDomain: the (Tetrahedron*) variable ");
        fprintf(stderr,"pMesh->ptet=%p does not point to ",(void*)pMesh->ptet);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    volume=0.;
    iMax=pMesh->ntet;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptet[i].label==LABEL_INTERNAL_TETRA)
        {
            volume+=computeVolumeTetrahedron(&pMesh->pver[pMesh->ptet[i].p1-1],
                                             &pMesh->pver[pMesh->ptet[i].p2-1],
                                             &pMesh->pver[pMesh->ptet[i].p3-1],
                                             &pMesh->pver[pMesh->ptet[i].p4-1]);
        }
    }
    volume/=6.;

#if (BOHR_OR_NOT)
    volume*=BOHR_RADIUS;
    volume*=BOHR_RADIUS;
    volume*=BOHR_RADIUS;
#endif
    fprintf(stdout,"\nVolume of the internal domain = %.8lf\n",volume);

    return volume;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readLsSolFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    size_t length=0;
    char *readStringOut=NULL, keyword[21]={'\0'};
    char nameSolFile[NAME_SIZE_MAX]={'\0'};
    int i=0, readIntegerOut=0, readIntegerIn=0;
    double readDouble=0.;
    FILE *solFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readLsSolFile: the input (Mesh*) variable ");
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    // Check the number of vertices and its associated pointer
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In readLsSolFile: the pMesh->nver variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nver);
        return 0;
    }

    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In readLsSolFile: the (Point*) variable ");
        fprintf(stderr,"pMesh->pver=%p does not point to ",(void*)pMesh->pver);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Change the *.mesh extension of nameFile into the *.sol one of nameSolFile
    strncpy(nameSolFile,nameFile,NAME_SIZE_MAX);
    length=strlen(nameSolFile);
    if (length<6)
    {
        PRINT_ERROR("In readLsSolFile: the length of the input ");
        fprintf(stderr,"(char[%d]) variable namefile ",NAME_SIZE_MAX);
        fprintf(stderr,"(=%s) should be strictly more than 5 ",nameFile);
        fprintf(stderr,"(instead of %d) in order to contain ",(int)length);
        fprintf(stderr,"at least something more than the *.mesh extension.\n");
        return 0;
    }
    nameSolFile[length-4]='s';
    nameSolFile[length-3]='o';
    nameSolFile[length-2]='l';
    nameSolFile[length-1]='\0';

    // Open the *.sol file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameSolFile);
    solFile=fopen(nameSolFile,"r");
    if (solFile==NULL)
    {
        PRINT_ERROR("In readLsSolFile: we could not open the ");
        fprintf(stderr,"%s file properly (maybe it does not  ",nameSolFile);
        fprintf(stderr,"exist at the given location).\n");
        return 0;
    }
    fprintf(stdout,"Reading mesh solutions. ");

    // Potentially read the first white space characters
    readIntegerOut=fscanf(solFile," ");
    if (readIntegerOut)
    {
        PRINT_ERROR("In readLsSolFile: wrong return ");
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
        PRINT_ERROR("In readLsSolFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the MeshVersionFormatted keyword.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (strcmp(keyword,"MeshVersionFormatted") || readIntegerIn!=2)
    {
        PRINT_ERROR("In readLsSolFile: expecting MeshVersionFormatted 2 ");
        fprintf(stderr,"instead of %s %d.\n",keyword,readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the dimension (two, three)
    readStringOut=fgets(keyword,10,solFile);
    readIntegerOut=fscanf(solFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readLsSolFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the Dimension keyword.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (strcmp(keyword,"Dimension") || readIntegerIn!=3)
    {
        PRINT_ERROR("In readLsSolFile: expecting Dimension 3 instead of ");
        fprintf(stderr,"%s %d.\n",keyword,readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the number of mesh vertices in the *.sol file
    readStringOut=fgets(keyword,14,solFile);
    readIntegerOut=fscanf(solFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readLsSolFile: wrong return of the ");
        fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the SolAtVertices keyword.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (strcmp(keyword,"SolAtVertices") || readIntegerIn!=pMesh->nver)
    {
        PRINT_ERROR("In readLsSolFile: expecting SolAtVertices ");
        fprintf(stderr,"%d instead of %s ",pMesh->nver,keyword);
        fprintf(stderr,"%d.\n",readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the number of solutions
    readIntegerOut=fscanf(solFile,"%d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readLsSolFile: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the number of ");
        fprintf(stderr,"solutions.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (readIntegerIn!=1)
    {
        PRINT_ERROR("In readLsSolFile: expecting one solution ");
        fprintf(stderr,"(instead of %d ones).\n",readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Check the scalar type of solutions
    readIntegerOut=fscanf(solFile,"%d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readLsSolFile: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the type of ");
        fprintf(stderr,"solutions.\n");
        closeTheFile(&solFile);
        return 0;
    }

    if (readIntegerIn!=1)
    {
        PRINT_ERROR("In readLsSolFile: expecting one (scalar type) ");
        fprintf(stderr,"(instead of %d).\n",readIntegerIn);
        closeTheFile(&solFile);
        return 0;
    }

    // Read values at the mesh vertices from the *.sol file
    for (i=0; i<pMesh->nver; i++)
    {
        readIntegerOut=fscanf(solFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readLsSolFile: wrong return ");
            fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
            fprintf(stderr,"c-function in the attempt of reading the ");
            fprintf(stderr,"value associated with the %d-th vertex.\n",i+1);
            closeTheFile(&solFile);
            return 0;
        }
        pMesh->pver[i].value=readDouble;
    }

    // Close the *.sol file
    if (fclose(solFile))
    {
        fprintf(stdout,"\nWarning in readLsSolFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameSolFile);
        solFile=NULL;
        return 0;
    }
    solFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingDomainInObjFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    int i=0, j=0, iMax=0, nVer=0, nTri=0, *pTri=NULL;
    FILE *objFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In writingDomainInObjFile: the input (Mesh*) variable ");
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    // Check the number of vertices and its associated pointer
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In writingDomainInObjFile: the pMesh->nver variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nver);
        return 0;
    }

    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In writingDomainInObjFile: the (Point*) variable ");
        fprintf(stderr,"pMesh->pver=%p does not point to ",(void*)pMesh->pver);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check the number of triangles and its associated pointer
    if (pMesh->ntri<1)
    {
        PRINT_ERROR("In writingDomainInObjFile: the pMesh->ntri variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntri);
        return 0;
    }

    if (pMesh->ptri==NULL)
    {
        PRINT_ERROR("In writingDomainInObjFile: the (Triangle*) variable ");
        fprintf(stderr,"pMesh->ptri=%p does not point to ",(void*)pMesh->ptri);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Open the *.obj file (warning: overwrite the file if it already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    objFile=fopen(nameFile,"w+");
    if (objFile==NULL)
    {
        PRINT_ERROR("In writingDomainInObjFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing data. ");

    // Allocate memory for the adjacency table of triangles
    pTri=(int*)calloc(3*pMesh->ntri,sizeof(int));
    if (pTri==NULL)
    {
        PRINT_ERROR("In writingDomainInObjFile: could not allocate memory ");
        fprintf(stderr,"for the local (int*) pTri=%p variable.\n",(void*)pTri);
        closeTheFile(&objFile);
        return 0;
    }

    // Write the vertices by setting a triangle adjacency
    nVer=0;
    iMax=pMesh->ntri;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptri[i].label==LABEL_BOUNDARY)
        {
            // Check for repetition of p1 before saving the point
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
                pTri[3*i]=nVer;
                nVer++;
#if (BOHR_OR_NOT)
                fprintf(objFile,"v %.8le ",
                                pMesh->pver[pMesh->ptri[i].p1-1].x*BOHR_RADIUS);
                fprintf(objFile,"%.8le ",
                                pMesh->pver[pMesh->ptri[i].p1-1].y*BOHR_RADIUS);
                fprintf(objFile,"%.8le \n",
                                pMesh->pver[pMesh->ptri[i].p1-1].z*BOHR_RADIUS);
#else
                fprintf(objFile,"v %.8le ",pMesh->pver[pMesh->ptri[i].p1-1].x);
                fprintf(objFile,"%.8le ",pMesh->pver[pMesh->ptri[i].p1-1].y);
                fprintf(objFile,"%.8le \n",pMesh->pver[pMesh->ptri[i].p1-1].z);
#endif
            }

            // Check for repetition of p2 before saving the point
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
                pTri[3*i+1]=nVer;
                nVer++;
#if (BOHR_OR_NOT)
                fprintf(objFile,"v %.8le ",
                                pMesh->pver[pMesh->ptri[i].p2-1].x*BOHR_RADIUS);
                fprintf(objFile,"%.8le ",
                                pMesh->pver[pMesh->ptri[i].p2-1].y*BOHR_RADIUS);
                fprintf(objFile,"%.8le \n",
                                pMesh->pver[pMesh->ptri[i].p2-1].z*BOHR_RADIUS);
#else
                fprintf(objFile,"v %.8le ",pMesh->pver[pMesh->ptri[i].p2-1].x);
                fprintf(objFile,"%.8le ",pMesh->pver[pMesh->ptri[i].p2-1].y);
                fprintf(objFile,"%.8le \n",pMesh->pver[pMesh->ptri[i].p2-1].z);
#endif
            }

            // Check for repetition of p3 before saving the point
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
                pTri[3*i+2]=nVer;
                nVer++;
#if (BOHR_OR_NOT)
                fprintf(objFile,"v %.8le ",
                                pMesh->pver[pMesh->ptri[i].p3-1].x*BOHR_RADIUS);
                fprintf(objFile,"%.8le ",
                                pMesh->pver[pMesh->ptri[i].p3-1].y*BOHR_RADIUS);
                fprintf(objFile,"%.8le \n",
                                pMesh->pver[pMesh->ptri[i].p3-1].z*BOHR_RADIUS);
#else
                fprintf(objFile,"v %.8le ",pMesh->pver[pMesh->ptri[i].p3-1].x);
                fprintf(objFile,"%.8le ",pMesh->pver[pMesh->ptri[i].p3-1].y);
                fprintf(objFile,"%.8le \n",pMesh->pver[pMesh->ptri[i].p3-1].z);
#endif
            }
        }
    }
    fprintf(objFile,"\n");

    // Saving triangles with the right adjacency
    nTri=0;
    iMax=pMesh->ntri;
    for (i=0; i<iMax; i++)
    {
        if (pMesh->ptri[i].label==LABEL_BOUNDARY)
        {
            if (pTri[3*i]<0 || pTri[3*i]>nVer-1)
            {
                PRINT_ERROR("In writingDomainInObjFile: the first vertex ");
                fprintf(stderr,"reference (=%d) of the %d-th ",pTri[3*i],i+1);
                fprintf(stderr,"triangle should a positive integer not ");
                fprintf(stderr,"strictly greater than the total number of ");
                fprintf(stderr,"mesh vertices (=%d).\n",nVer);
                closeTheFile(&objFile);
                free(pTri);
                pTri=NULL;
                return 0;
            }
            if (pTri[3*i+1]<0 || pTri[3*i+1]>nVer-1)
            {
                PRINT_ERROR("In writingDomainInObjFile: the second vertex ");
                fprintf(stderr,"reference (=%d) of the %d-th ",pTri[3*i+1],i+1);
                fprintf(stderr,"triangle should a positive integer not ");
                fprintf(stderr,"strictly greater than the total number of ");
                fprintf(stderr,"mesh vertices (=%d).\n",nVer);
                closeTheFile(&objFile);
                free(pTri);
                pTri=NULL;
                return 0;
            }
            if (pTri[3*i+2]<0 || pTri[3*i+2]>nVer-1)
            {
                PRINT_ERROR("In writingDomainInObjFile: the third vertex ");
                fprintf(stderr,"reference (=%d) of the %d-th ",pTri[3*i+2],i+1);
                fprintf(stderr,"triangle should a positive integer not ");
                fprintf(stderr,"strictly greater than the total number of ");
                fprintf(stderr,"mesh vertices (=%d).\n",nVer);
                closeTheFile(&objFile);
                free(pTri);
                pTri=NULL;
                return 0;
            }
            fprintf(objFile,"f %d %d ",pTri[3*i]+1,pTri[3*i+1]+1);
            fprintf(objFile,"%d \n",pTri[3*i+2]+1);
            nTri++;
        }
    }
    fprintf(objFile,"\n");

    // Close the *.obj file
    if (fclose(objFile))
    {
        fprintf(stdout,"\nWarning in writingDomainInObjFile function: we ");
        fprintf(stdout,"could not close the %s file properly.\n",nameFile);
        free(pTri);
        pTri=NULL;
        objFile=NULL;
        return 0;
    }
    objFile=NULL;
    fprintf(stdout,"Closing file.\nThe boundary of the internal domain ");
    fprintf(stdout,"contains %d vertices and %d triangles.\n",nVer,nTri);

    // Free the memory allocated
    free(pTri);
    pTri=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    char extension[2][13]={"input *.mesh","output *.obj"};
    int i=0, length=0, extensionLength[2]={7,6}, boolean=0, nTri=0;
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
            freeMeshMemory(&mesh);
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

    // Allocate memory for point coordinates and triangle/tetrahedron references
    mesh.pver=(Point*)calloc(mesh.nver,sizeof(Point));
    mesh.ptri=(Triangle*)calloc(mesh.ntri,sizeof(Triangle));
    mesh.ptet=(Tetrahedron*)calloc(mesh.ntet,sizeof(Tetrahedron));
    if (mesh.pver==NULL || mesh.ptri==NULL || mesh.ptet==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"variables mesh.pver=%p ",(void*)mesh.pver);
        fprintf(stderr,"mesh.ptri=%p, and ",(void*)mesh.ptri);
        fprintf(stderr,"mesh.ptet=%p in Mesh structure.\n",(void*)mesh.ptet);
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

    // Check that an internal domain is encoded in the mesh
    nTri=0;
    for (i=0; i<mesh.ntri; i++)
    {
        if (mesh.ptri[i].label==LABEL_BOUNDARY)
        {
            nTri++;
        }
    }
    if (nTri<1)
    {
        PRINT_ERROR("In main: the input tetrahedral mesh of the ");
        fprintf(stderr,"%s file does not seem to contain any ",argv[1]);
        fprintf(stderr,"internal domain. The number of boundary triangles ");
        fprintf(stderr,"(=%d) labelled %d should be a ",nTri,LABEL_BOUNDARY);
        fprintf(stderr,"positive integer.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }
    // Compute the volume of the internal domain
    computeVolumeOfDomain(&mesh);

    // Read the *.sol file associated with the input *.mesh file
    if(!readLsSolFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readLsSolFile function returned zero instead ");
        fprintf(stderr,"of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Adjust the label of boundary box inside the domain
    for (i=0; i<mesh.ntri; i++)
    {
        if (mesh.ptri[i].label!=LABEL_BOUNDARY)
        {
            if (mesh.pver[mesh.ptri[i].p1-1].value<=0. &&
                                       mesh.pver[mesh.ptri[i].p2-1].value<=0. &&
                                        mesh.pver[mesh.ptri[i].p3-1].value<=0.)
            {
                // Inside
                mesh.ptri[i].label=LABEL_BOUNDARY;
            }
            else if (mesh.pver[mesh.ptri[i].p1-1].value>=0. &&
                                       mesh.pver[mesh.ptri[i].p2-1].value>=0. &&
                                        mesh.pver[mesh.ptri[i].p3-1].value>=0.)
            {
                // Outside
                mesh.ptri[i].label=2;
            }
            else
            {
                PRINT_ERROR("In main: the ");
                fprintf(stderr,"%d-th triangle should be inside or ",i+1);
                fprintf(stderr,"outside the domain.\n");
                freeMeshMemory(&mesh);
                return EXIT_FAILURE;
            }
        }
    }
    fprintf(stdout,"Extracting the domain boundary (and closing the ");
    fprintf(stdout,"surface if necessary).\n\n");

    // Close the domain and save the domain boundary in the output *.obj file
    if(!writingDomainInObjFile(argv[2],&mesh))
    {
        PRINT_ERROR("In main: writingDomainInObjFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }
    freeMeshMemory(&mesh);

    return EXIT_SUCCESS;
}

