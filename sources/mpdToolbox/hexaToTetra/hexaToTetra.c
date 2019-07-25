#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

// Tolerance allowed on the variation of discretization step between vertices
// when extracting the discretization parameters from the hexahedral mesh
#define TOLERANCE_DISCRETIZATION 0.000001

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
} Point;

typedef struct {
    int p;
    double x;
    double y;
    double z;
} Vector;

typedef struct {
    int p1;
    int p2;
    int label;
} Edge;

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
    int p1;
    int p2;
    int p3;
    int p4;
    int label;
} Quadrilateral;

typedef struct {
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
    int p6;
    int p7;
    int p8;
    int label;
} Hexahedron;

typedef struct {
    int nver;
    Point* pver;
    int nnorm;
    Vector* pnorm;
    int ntan;
    Vector* ptan;
    int nedg;
    Edge* pedg;
    int ntri;
    Triangle* ptri;
    int ntet;
    Tetrahedron* ptet;
    int nqua;
    Quadrilateral* pqua;
    int nhex;
    Hexahedron* phex;
} Mesh;

char* endTimerAtError(void);
void closeTheFile(FILE** pFileToClose);
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                             int maximumLength);
void initializeMeshStructure(Mesh* pMesh);
void freeMeshMemory(Mesh* pMesh);
int getNumberOfVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int readVerticesInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int getNumberOfQuadrilateralsInMeshFile(char nameFile[NAME_SIZE_MAX],
                                                                   Mesh* pMesh);
int readQuadrilateralsInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int getNumberOfHexahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int readHexahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);
int getNumberOfDiscretizationPoints(Mesh* pMesh, int* pNx, int* pNy, int* pNz);
int labelPoint(int i, int j, int k, int nX, int nY, int nZ);
int buildingTetrahedralMesh(Mesh* pMesh, int nX, int nY, int nZ);
int writingTetrahedralMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);

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

        pMesh->nnorm=0;
        pMesh->pnorm=NULL;

        pMesh->ntan=0;
        pMesh->ptan=NULL;

        pMesh->nedg=0;
        pMesh->pedg=NULL;

        pMesh->ntri=0;
        pMesh->ptri=NULL;

        pMesh->ntet=0;
        pMesh->ptet=NULL;

        pMesh->nqua=0;
        pMesh->pqua=NULL;

        pMesh->nhex=0;
        pMesh->phex=NULL;
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

        free(pMesh->pnorm);
        pMesh->pnorm=NULL;

        free(pMesh->ptan);
        pMesh->ptan=NULL;

        free(pMesh->pedg);
        pMesh->pedg=NULL;

        free(pMesh->ptri);
        pMesh->ptri=NULL;

        free(pMesh->ptet);
        pMesh->ptet=NULL;

        free(pMesh->pqua);
        pMesh->pqua=NULL;

        free(pMesh->phex);
        pMesh->phex=NULL;
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
int getNumberOfQuadrilateralsInMeshFile(char nameFile[NAME_SIZE_MAX],
                                                                    Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[14]={'\0'};
    int readChar=0, readIntegerOut=0, nQua=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: the input ");
        fprintf(stderr,"(Mesh*) variable pMesh=%p does not ",(void*)pMesh);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: we could not ");
        fprintf(stderr,"open the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the total number of mesh quadrilaterals. ");

    // Skip all characters until the 'Q' of Quadrilaterals is found
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
        else if (readChar=='Q')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: wrong ");
                fprintf(stderr,"return of the fseek c-function while ");
                fprintf(stderr,"checking if the '%c' ",(char)readChar);
                fprintf(stderr,"encountered corresponds to the one of the ");
                fprintf(stderr,"Quadrilaterals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'Q' of Quadrilaterals (not NormalAtQuadrilaterals)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'Q' of Quadrilaterals
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'Q' of the Quadrilaterals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='Q')
            {
                PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: ");
                fprintf(stderr,"expecting 'Q' instead of %c ",(char)readChar);
                fprintf(stderr,"while attempting to read to the first ");
                fprintf(stderr,"character of the Quadrilaterals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any quadrilaterals found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of quadrilaterals
    readStringOut=fgets(keyword,14,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&nQua);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: wrong return of ");
        fprintf(stderr,"the fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of quadrilaterals.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"uadrilaterals") || nQua<1)
    {
        PRINT_ERROR("In getNumberOfQuadrilateralsInMeshFile: expecting ");
        fprintf(stderr,"Quadrilaterals Nqua instead of ");
        fprintf(stderr,"%c%s %d (positive ",(char)readChar,keyword,nQua);
        fprintf(stderr,"value).\n");
        closeTheFile(&meshFile);
        return 0;
    }
    pMesh->nqua=nQua;

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfQuadrilateralsInMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file (%d quadrilaterals found).\n",pMesh->nqua);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readQuadrilateralsInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[14]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the number of vertices
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: the pMesh->nver ");
        fprintf(stderr,"variable (=%d) should be a positive ",pMesh->nver);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    // Check the number of quadrilaterals and its associated pointer
    if (pMesh->nqua<1)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: the pMesh->nqua ");
        fprintf(stderr,"variable (=%d) should be a positive ",pMesh->nqua);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    if (pMesh->pqua==NULL)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: the (Quadrilateral*) ");
        fprintf(stderr,"variable pMesh->pqua=%p does not ",(void*)pMesh->pqua);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading quadrilaterals. ");

    // Skip all characters until the 'Q' of Quadrilaterals is found
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
        else if (readChar=='Q')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong ");
                fprintf(stderr,"return of the fseek c-function while ");
                fprintf(stderr,"checking if the '%c' ",(char)readChar);
                fprintf(stderr,"encountered corresponds to the one of the ");
                fprintf(stderr,"Quadrilaterals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'Q' of Quadrilaterals (not NormalAtQuadrilaterals)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'Q' of Quadrilaterals
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'Q' of the Quadrilaterals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='Q')
            {
                PRINT_ERROR("In readQuadrilateralsInMeshFile: ");
                fprintf(stderr,"expecting 'Q' instead of %c ",(char)readChar);
                fprintf(stderr,"while attempting to read to the first ");
                fprintf(stderr,"character of the Quadrilaterals keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any quadrilaterals found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of quadrilaterals
    readStringOut=fgets(keyword,14,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong return of ");
        fprintf(stderr,"the fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of quadrilaterals.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"uadrilaterals") || readIntegerIn!=pMesh->nqua)
    {
        PRINT_ERROR("In readQuadrilateralsInMeshFile: expecting ");
        fprintf(stderr,"Quadrilaterals %d instead of ",pMesh->nqua);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<pMesh->nqua; i++)
    {
        // Reading the first vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the first vertex ");
            fprintf(stderr,"reference of the %d-th quadrilateral.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: the first vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"quadrilateral should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pqua[i].p1=readIntegerIn;

        // Reading the second vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the second vertex ");
            fprintf(stderr,"reference of the %d-th quadrilateral.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: the second vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"quadrilateral should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pqua[i].p2=readIntegerIn;

        // Reading the third vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the third vertex ");
            fprintf(stderr,"reference of the %d-th quadrilateral.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: the third vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"quadrilateral should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pqua[i].p3=readIntegerIn;

        // Reading the fourth vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the fourth vertex ");
            fprintf(stderr,"reference of the %d-th quadrilateral.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: the fourth vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"quadrilateral should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pqua[i].p4=readIntegerIn;

        // Reading the vertex label
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readQuadrilateralsInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the label associated with ");
            fprintf(stderr,"the %d-th quadrilateral.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->pqua[i].label=readIntegerIn;
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in readQuadrilateralsInMeshFile function: ");
        fprintf(stdout,"we could not close the %s file properly.\n",nameFile);
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int getNumberOfHexahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[9]={'\0'};
    int readChar=0, readIntegerOut=0, nHex=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: the input ");
        fprintf(stderr,"(Mesh*) variable pMesh=%p does not ",(void*)pMesh);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: we could not ");
        fprintf(stderr,"open the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading the total number of mesh hexahedra. ");

    // Skip all characters until the 'H' of Hexahedra is found
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
        else if (readChar=='H')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong ");
                fprintf(stderr,"return of the fseek c-function while ");
                fprintf(stderr,"checking if the '%c' ",(char)readChar);
                fprintf(stderr,"encountered corresponds to the one of the ");
                fprintf(stderr,"Hexahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'H' of Hexahedra (not SolAtHexahedra)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'H' of Hexahedra
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'Q' of the Hexahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='H')
            {
                PRINT_ERROR("In getNumberOfHexahedraInMeshFile: ");
                fprintf(stderr,"expecting 'Q' instead of %c ",(char)readChar);
                fprintf(stderr,"while attempting to read to the first ");
                fprintf(stderr,"character of the Hexahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any hexahedra found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of hexahedra
    readStringOut=fgets(keyword,9,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&nHex);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong return of ");
        fprintf(stderr,"the fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of hexahedra.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"exahedra") || nHex<1)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: expecting Hexahedra ");
        fprintf(stderr,"Nhex instead of %c%s %d ",(char)readChar,keyword,nHex);
        fprintf(stderr,"(positive value).\n");
        closeTheFile(&meshFile);
        return 0;
    }
    pMesh->nhex=nHex;

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfHexahedraInMeshFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file (%d hexahedra found).\n",pMesh->nhex);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readHexahedraInMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    char *readStringOut=NULL, keyword[14]={'\0'};
    int i=0, readChar=0, readIntegerOut=0, readIntegerIn=0, boolean=0;
    FILE *meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In readHexahedraInMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the number of vertices
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In readHexahedraInMeshFile: the pMesh->nver ");
        fprintf(stderr,"variable (=%d) should be a positive ",pMesh->nver);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    // Check the number of hexahedra and its associated pointer
    if (pMesh->nhex<1)
    {
        PRINT_ERROR("In readHexahedraInMeshFile: the pMesh->nhex ");
        fprintf(stderr,"variable (=%d) should be a positive ",pMesh->nhex);
        fprintf(stderr,"integer.\n");
        return 0;
    }

    if (pMesh->phex==NULL)
    {
        PRINT_ERROR("In readHexahedraInMeshFile: the (Hexahedra*) ");
        fprintf(stderr,"variable pMesh->phex=%p does not ",(void*)pMesh->phex);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Open *.mesh file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In readHexahedraInMeshFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading hexahedra. ");

    // Skip all characters until the 'H' of Hexahedra is found
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
        else if (readChar=='H')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong ");
                fprintf(stderr,"return of the fseek c-function while ");
                fprintf(stderr,"checking if the '%c' ",(char)readChar);
                fprintf(stderr,"encountered corresponds to the one of the ");
                fprintf(stderr,"Hexahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }

            // Check if it is 'H' of Hexahedra (not SolAtHexahedra)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=0;
            }

            // Reading again the 'H' of Hexahedra
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong ");
                fprintf(stderr,"end-of-file return (=%d) of the ",readChar);
                fprintf(stderr,"fgetc c-function while attempting to read ");
                fprintf(stderr,"the 'Q' of the Hexahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            else if (readChar!='H')
            {
                PRINT_ERROR("In getNumberOfHexahedraInMeshFile: ");
                fprintf(stderr,"expecting 'Q' instead of %c ",(char)readChar);
                fprintf(stderr,"while attempting to read to the first ");
                fprintf(stderr,"character of the Hexahedra keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        }

    } while (boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any hexahedra found.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of hexahedra
    readStringOut=fgets(keyword,9,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: wrong return of ");
        fprintf(stderr,"the fgets=%p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-functions in the attempt of ",readIntegerOut);
        fprintf(stderr,"reading the number of hexahedra.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(keyword,"exahedra") || readIntegerIn!=pMesh->nhex)
    {
        PRINT_ERROR("In getNumberOfHexahedraInMeshFile: expecting Hexahedra ");
        fprintf(stderr,"%d instead of ",pMesh->nhex);
        fprintf(stderr,"%c%s %d.\n",(char)readChar,keyword,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    for (i=0; i<pMesh->nhex; i++)
    {
        // Reading the first vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the first vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the first vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p1=readIntegerIn;

        // Reading the second vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the second vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the second vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p2=readIntegerIn;

        // Reading the third vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the third vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the third vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p3=readIntegerIn;

        // Reading the fourth vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the fourth vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the fourth vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p4=readIntegerIn;

        // Reading the fifth vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the fifth vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the fifth vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p5=readIntegerIn;

        // Reading the sixth vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the sixth vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the sixth vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p6=readIntegerIn;

        // Reading the seventh vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the seventh vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the seventh vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p7=readIntegerIn;

        // Reading the eighth vertex reference
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the eighth vertex ");
            fprintf(stderr,"reference of the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: the eighth vertex ");
            fprintf(stderr,"reference (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"hexahedron should be a positive integer not ");
            fprintf(stderr,"strictly greater than the total number of mesh ");
            fprintf(stderr,"vertices (=%d).\n",pMesh->nver);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].p8=readIntegerIn;

        // Reading the vertex label
        readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readHexahedraInMeshFile: wrong return ");
            fprintf(stderr,"(=%d) of the fscanf c-function in ",readIntegerOut);
            fprintf(stderr,"the attempt of reading the label associated with ");
            fprintf(stderr,"the %d-th hexahedron.\n",i+1);
            closeTheFile(&meshFile);
            return 0;
        }
        pMesh->phex[i].label=readIntegerIn;
    }

    // Close the *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in readHexahedraInMeshFile function: ");
        fprintf(stdout,"we could not close the %s file properly.\n",nameFile);
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int getNumberOfDiscretizationPoints(Mesh* pMesh, int* pNx, int* pNy, int* pNz)
{
    int nX=0, nY=0, nZ=0, i=0, iMax=0, j=0, k=0, boolean=0;
    double deltaX=0., deltaY=0., deltaZ=0., xMin=0., yMin=0., zMin=0., xMax=0.;
    double yMax=0., zMax=0., cx=0., cy=0., cz=0.;
    Point *pVertex=NULL;

    // Check if the input pParameters or pMesh variable is pointing to NULL
    if (pMesh==NULL || pNx==NULL || pNy==NULL || pNz==NULL)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: at least one of the ");
        fprintf(stderr,"input variables pMesh=%p, ",(void*)pMesh);
        fprintf(stderr,"pNx=%p, pNy=%p and/or ",(void*)pNx,(void*)pNy);
        fprintf(stderr,"pNz=%p does not point to a valid adress.\n",(void*)pNz);
        return 0;
    }

    // Check the pMesh->pver variable
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: no Point data have ");
        fprintf(stderr,"been saved in the structure pointed by pMesh since ");
        fprintf(stderr,"its pver variable is pointing to the ");
        fprintf(stderr,"%p adress.\n",(void*)pMesh->pver);
        return 0;
    }

    // Check the number of vertices
    if (pMesh->nver<9)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: we were expecting ");
        fprintf(stderr,"at least 9(=3x3x3) instead of %d for the ",pMesh->nver);
        fprintf(stderr,"total number of vertices stored in the structure ");
        fprintf(stderr,"pointed by pMesh.\n");
        return 0;
    }

    // Getting the bounding computational box
    xMin=pMesh->pver[0].x;
    yMin=pMesh->pver[0].y;
    zMin=pMesh->pver[0].z;

    xMax=xMin;
    yMax=yMin;
    zMax=zMin;

    iMax=pMesh->nver;
    for (i=1; i<iMax; i++)
    {
        cx=pMesh->pver[i].x;
        if (cx<xMin)
        {
            xMin=cx;
        }
        else if (cx>xMax)
        {
            xMax=cx;
        }

        cy=pMesh->pver[i].y;
        if (cy<yMin)
        {
            yMin=cy;
        }
        else if (cy>yMax)
        {
            yMax=cy;
        }

        cz=pMesh->pver[i].z;
        if (cz<zMin)
        {
            zMin=cz;
        }
        else if (cz>zMax)
        {
            zMax=cz;
        }
    }

    // Check that the bounding computational box is not empty
    if (xMin>=xMax || yMin>=yMax || zMin>=zMax)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: expecting\n");
        fprintf(stderr,"(xMin==%lf) < (xMax==%lf)\n",xMin,xMax);
        fprintf(stderr,"(yMin==%lf) < (yMax==%lf)\n",yMin,yMax);
        fprintf(stderr,"(zMin==%lf) < (zMax==%lf)\n",zMin,zMax);
        fprintf(stderr,"for the bounding computational box associated with");
        fprintf(stderr,"the mesh discretization stored in the structure ");
        fprintf(stderr,"pointed by pMesh.\n");
        return 0;
    }
    fprintf(stdout,"\nBounding box of the mesh: [%lf,%lf]x",xMin,xMax);
    fprintf(stdout,"[%lf,%lf]x[%lf,%lf].\n",yMin,yMax,zMin,zMax);

    // Detect an hexahedral structure in the vertex description
    boolean=0;
    deltaX=DEF_ABS(pMesh->pver[1].x-pMesh->pver[0].x);
    deltaY=DEF_ABS(pMesh->pver[1].y-pMesh->pver[0].y);
    deltaZ=DEF_ABS(pMesh->pver[1].z-pMesh->pver[0].z);
    if (deltaX<1.e-15 && deltaY<1.e-15)
    {
        cx=DEF_ABS(pMesh->pver[2].x-pMesh->pver[1].x);
        cy=DEF_ABS(pMesh->pver[2].y-pMesh->pver[1].y);
        cz=DEF_ABS(pMesh->pver[2].z-pMesh->pver[1].z);
        if (cx<1.e-15 && cy<1.e-15 && DEF_ABS(cz-deltaZ)<1.e-15)
        {
            boolean=1;
        }
    }

    if (!boolean)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: no hexahedral grid ");
        fprintf(stderr,"structure detected in the mesh while checking the ");
        fprintf(stderr,"coordinates of the first three vertices to get ");
        fprintf(stderr,"the discretization step (=%lf) in the third ",deltaZ);
        fprintf(stderr,"coordinate direction.\n");
        return 0;
    }

    // Get some guess for the local nX, nY and nY variables
    iMax=pMesh->nver;

    i=0;
    do {
        i++;
        deltaY=DEF_ABS(pMesh->pver[i].y-pMesh->pver[i-1].y);
    } while (deltaY<1.e-16 && i<iMax);
    nZ=i;

    i=0;
    do {
        i++;
        deltaX=DEF_ABS(pMesh->pver[i].x-pMesh->pver[i-1].x);
    } while (deltaX<1.e-16 && i<iMax);
    nY=i/nZ;

    if (i%nZ!=0)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: expecting ");
        fprintf(stderr,"%d instead of %d for the local variable ",nY*nZ,i);
        fprintf(stderr,"i, in the attempt of updating the local nX and ");
        fprintf(stderr,"nZ variables.\n");
        return 0;
    }
    nX=iMax/i;

    if (iMax%i!=0)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: expecting ");
        fprintf(stderr,"%d instead of %d for the local ",nX*nY*nZ,iMax);
        fprintf(stderr,"variable i, in the attempt of updating the local ");
        fprintf(stderr,"nX variable.\n");
        return 0;
    }

    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In getNumberOfDiscretizationPoints: expecting ");
        fprintf(stderr,"that the values (nX=%d, nY=%d, nZ=%d) ",nX,nY,nZ);
        fprintf(stderr,"suggested for the local nX, nY and nZ variables");
        fprintf(stderr,"would be integers (strictly) greater than two.\n");
        return 0;
    }

    // Get a guess for the deltaX, deltaY and deltaZ variables of pMesh
    deltaX=(xMax-xMin)/(double)(nX-1);
    deltaY=(yMax-yMin)/(double)(nY-1);
    deltaZ=(zMax-zMin)/(double)(nZ-1);

    // Check if the guessed values are correct
    for (i=0; i<nX; i++)
    {
        for (j=0; j<nY; j++)
        {
            for (k=0; k<nZ; k++)
            {
                iMax=(i*nY+j)*nZ+k;
                pVertex=&pMesh->pver[iMax];

                cx=xMin+i*deltaX;
                cy=yMin+j*deltaY;
                cz=zMin+k*deltaZ;

                if (DEF_ABS(pVertex->x-cx)>=TOLERANCE_DISCRETIZATION)
                {
                    PRINT_ERROR("getNumberOfDiscretizationPoints: ");
                    fprintf(stderr,"expecting %.18lf instead of ",cx);
                    fprintf(stderr,"%.18lf (up to 15 ",pVertex->x);
                    fprintf(stderr,"decimals) for the first coordinate ");
                    fprintf(stderr,"of the %d-th vertex ",iMax+1);
                    fprintf(stderr,"associated with the hexahedral grid ");
                    fprintf(stderr,"structure detected and pointed by ");
                    fprintf(stderr,"pMesh.\n");
                    return 0;
                }

                if (DEF_ABS(pVertex->y-cy)>=TOLERANCE_DISCRETIZATION)
                {
                    PRINT_ERROR("getNumberOfDiscretizationPoints: ");
                    fprintf(stderr,"expecting %.18lf instead of ",cy);
                    fprintf(stderr,"%.18lf (up to 15 ",pVertex->y);
                    fprintf(stderr,"decimals) for the second coordinate ");
                    fprintf(stderr,"of the %d-th vertex ",iMax+1);
                    fprintf(stderr,"associated with the hexahedral grid ");
                    fprintf(stderr,"structure detected and pointed by ");
                    fprintf(stderr,"pMesh.\n");
                    return 0;
                }

                if (DEF_ABS(pVertex->z-cz)>=TOLERANCE_DISCRETIZATION)
                {
                    PRINT_ERROR("getNumberOfDiscretizationPoints: ");
                    fprintf(stderr,"expecting %.18lf instead of ",cz);
                    fprintf(stderr,"%.18lf (up to 15 ",pVertex->z);
                    fprintf(stderr,"decimals) for the third coordinate ");
                    fprintf(stderr,"of the %d-th vertex ",iMax+1);
                    fprintf(stderr,"associated with the hexahedral grid ");
                    fprintf(stderr,"structure detected and pointed by ");
                    fprintf(stderr,"pMesh.\n");
                    return 0;
                }
            }
        }
    }

    fprintf(stdout,"\nHexahedral grid structure detected in the ");
    fprintf(stdout,"mesh as expected:\n(%lf)x(%lf)x",deltaX,deltaY);
    fprintf(stdout,"(%lf) discretization steps with ",deltaZ);
    fprintf(stdout,"(%d)x(%d)x(%d) points.\n",nX,nY,nZ);

    // Updating the output variables
    *pNx=nX;
    *pNy=nY;
    *pNz=nZ;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int labelPoint(int i, int j, int k, int nX, int nY, int nZ)
{
    int returnValue=0;

    if (!i)
    {
        if (!j)
        {
            if (!k)
            {
                returnValue=145;
            }
            else if (k==nZ-1)
            {
                returnValue=146;
            }
            else
            {
                returnValue=14;
            }
        }
        else if (j==nY-1)
        {
            if (!k)
            {
                returnValue=345;
            }
            else if (k==nZ-1)
            {
                returnValue=346;
            }
            else
            {
                returnValue=34;
            }
        }
        else
        {
            if (!k)
            {
                returnValue=45;
            }
            else if (k==nZ-1)
            {
                returnValue=46;
            }
            else
            {
                returnValue=4;
            }
        }
    }
    else if (i==nX-1)
    {
        if (!j)
        {
            if (!k)
            {
                returnValue=125;
            }
            else if (k==nZ-1)
            {
                returnValue=126;
            }
            else
            {
                returnValue=12;
            }
        }
        else if (j==nY-1)
        {
            if (!k)
            {
                returnValue=235;
            }
            else if (k==nZ-1)
            {
                returnValue=236;
            }
            else
            {
                returnValue=23;
            }
        }
        else
        {
            if (!k)
            {
                returnValue=25;
            }
            else if (k==nZ-1)
            {
                returnValue=26;
            }
            else
            {
                returnValue=2;
            }
        }
    }
    else
    {
        if (!j)
        {
            if (!k)
            {
                returnValue=15;
            }
            else if (k==nZ-1)
            {
                returnValue=16;
            }
            else
            {
                returnValue=1;
            }
        }
        else if (j==nY-1)
        {
            if (!k)
            {
                returnValue=35;
            }
            else if (k==nZ-1)
            {
                returnValue=36;
            }
            else
            {
                returnValue=3;
            }
        }
        else
        {
            if (!k)
            {
                returnValue=5;
            }
            else if (k==nZ-1)
            {
                returnValue=6;
            }
            else
            {
                returnValue=0;
            }
        }
    }

    return returnValue;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int buildingTetrahedralMesh(Mesh* pMesh, int nX, int nY, int nZ)
{
    int i=0, i0=0, j=0, k=0, kMax=0, counter=0;

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the other input variables
    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting ");
        fprintf(stderr,"that the values (nX=%d, nY=%d, nZ=%d) ",nX,nY,nZ);
        fprintf(stderr,"suggested for the local nX, nY and nZ variables");
        fprintf(stderr,"would be integers (strictly) greater than two.\n");
        return 0;
    }

    // Check the number of vertices and its associated pointer
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->nver variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nver);
        return 0;
    }
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Point*) variable ");
        fprintf(stderr,"pMesh->pver=%p does not point to ",(void*)pMesh->pver);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check the number of normal vectors and its associated pointer
    if (pMesh->nnorm<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->nnorm variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nnorm);
        return 0;
    }
    if (pMesh->pnorm==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Vector*) variable ");
        fprintf(stderr,"pMesh->pnorm=%p does not point ",(void*)pMesh->pnorm);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of tangent vectors and its associated pointer
    if (pMesh->ntan<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->ntan variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntan);
        return 0;
    }
    if (pMesh->ptan==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Vector*) variable ");
        fprintf(stderr,"pMesh->ptan=%p does not point ",(void*)pMesh->ptan);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of edges and its associated pointer
    if (pMesh->nedg<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->nedg variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nedg);
        return 0;
    }
    if (pMesh->pedg==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Edge*) variable ");
        fprintf(stderr,"pMesh->pedg=%p does not point ",(void*)pMesh->pedg);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of triangles and its associated pointer
    if (pMesh->ntri<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->ntri variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntri);
        return 0;
    }
    if (pMesh->ptri==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Triangle*) variable ");
        fprintf(stderr,"pMesh->ptri=%p does not point ",(void*)pMesh->ptri);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of tetrahedra and its associated pointer
    if (pMesh->ntet<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->ntet variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntet);
        return 0;
    }
    if (pMesh->ptet==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Tetrahedron*) variable ");
        fprintf(stderr,"pMesh->ptet=%p does not point ",(void*)pMesh->ptet);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of quadrilaterals and its associated pointer
    if (pMesh->nqua<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->nqua variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nqua);
        return 0;
    }
    if (pMesh->pqua==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Quadrilateral*) ");
        fprintf(stderr,"variable pMesh->ptet=%p does not ",(void*)pMesh->ptet);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Check the number of hexahedra and its associated pointer
    if (pMesh->nhex<1)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the pMesh->nhex variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nhex);
        return 0;
    }
    if (pMesh->phex==NULL)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: the (Hexahedron*) ");
        fprintf(stderr,"variable pMesh->ptet=%p does not ",(void*)pMesh->phex);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Checking compatibility between mesh variables and the other input ones
    if (pMesh->nver!=nX*nY*nZ)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting ");
        fprintf(stderr,"%d instead of %d for the total ",nX*nY*nZ,pMesh->nver);
        fprintf(stderr,"number of mesh vertices.\n");
        return 0;
    }
    if (pMesh->nnorm<2*((nX-2)*(nY-2)+(nX-2)*(nZ-2)+(nY-2)*(nZ-2)))
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting at least ");
        fprintf(stderr,"%d ",2*((nX-2)*(nY-2)+(nX-2)*(nZ-2)+(nY-2)*(nZ-2)));
        fprintf(stderr,"instead of %d for the total number of ",pMesh->nnorm);
        fprintf(stderr,"normal vectors in the mesh.\n");
        return 0;
    }
    if (pMesh->ntan!=4*((nX-2)+(nY-2)+(nZ-2)))
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting ");
        fprintf(stderr,"%d instead of ",4*((nX-2)+(nY-2)+(nZ-2)));
        fprintf(stderr,"%d for the total number of tangent ",pMesh->ntan);
        fprintf(stderr,"vectors in the mesh.\n");
        return 0;
    }
    if (pMesh->nedg!=4*((nX-1)+(nY-1)+(nZ-1)))
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting ");
        fprintf(stderr,"%d instead of ",4*((nX-1)+(nY-1)+(nZ-1)));
        fprintf(stderr,"%d for the total number of mesh edges.\n",pMesh->nedg);
        return 0;
    }
    if (pMesh->ntri<4*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1)))
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting at least ");
        fprintf(stderr,"%d ",4*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1)));
        fprintf(stderr,"instead of %d for the total number of ",pMesh->ntri);
        fprintf(stderr,"mesh triangles.\n");
        return 0;
    }
    if (pMesh->ntet!=6*(nX-1)*(nY-1)*(nZ-1))
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting ");
        fprintf(stderr,"%d instead of %d ",6*(nX-1)*(nY-1)*(nZ-1),pMesh->ntet);
        fprintf(stderr,"for the total number of mesh tetrahedra.\n");
        return 0;
    }
    if (pMesh->nqua<2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1)))
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting at least ");
        fprintf(stderr,"%d ",2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1)));
        fprintf(stderr,"instead of %d for the total number of ",pMesh->nqua);
        fprintf(stderr,"mesh quadrilaterals.\n");
        return 0;
    }
    if (pMesh->nhex!=(nX-1)*(nY-1)*(nZ-1))
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting ");
        fprintf(stderr,"%d instead of %d ",(nX-1)*(nY-1)*(nZ-1),pMesh->nhex);
        fprintf(stderr,"for the total number of mesh hexahedra.\n");
        return 0;
    }

/*
    // Face 1 (j=0) with reference (a,b,c) in each quadrilateral corresponding
    // to square cell: 1=(i-1,k-1) 2=(i,k-1) 3=(i,k) 4=(i-1,k)
    for (i=0; i<nX-1; i++)
        {
        for (k=0; k<nZ-1; k++)
        {
            pMesh->phex[i*(nY-1)*(nZ-1)+k].label=2;
        }
    }

    // Face 2 (i=nX-2) with reference (a,b,c) in square cell:
    // 1=(j-1,k-1) 2=(j,k-1) 3=(j,k) 4=(j-1,k)
    for (j=0; j<nY-1; j++)
        {
        for (k=0; k<nZ-1; k++)
        {
            pMesh->phex[((nX-2)*(nY-1)+j)*(nZ-1)+k].label=2;
        }
    }

    // Face 3 (j=nY-2) with reference (a,b,c) in square cell:
    // 1=(i-1,k-1) 2=(i,k-1) 3=(i,k) 4=(i-1,k)
    for (i=0; i<nX-1; i++)
        {
        for (k=0; k<nZ-1; k++)
        {
            pMesh->phex[(i*(nY-1)+(nY-2))*(nZ-1)+k].label=2;
        }
    }

    // Face 4 (i=0) with reference (a,b,c) in square cell:
    // 1=(j-1,k-1) 2=(j,k-1) 3=(j,k) 4=(j-1,k)
    for (j=0; j<nY-1; j++)
        {
        for (k=0; k<nZ-1; k++)
        {
            pMesh->phex[j*(nZ-1)+k].label=2;
        }
    }

    // Face 5 (k=0) with reference (a,b,c) in square cell:
    // 1=(i-1,j-1) 2=(i,j-1) 3=(i,j) 4=(i-1,j)
    for (i=0; i<nX-1; i++)
        {
        for (j=0; j<nY-1; j++)
        {
            pMesh->phex[(i*(nY-1)+j)*(nZ-1)].label=2;
        }
    }

    // Face 6 (k=nZ-2) with reference (a,b,c) in square cell:
    // 1=(i-1,j-1) 2=(i,j-1) 3=(i,j) 4=(i-1,j)
    for (i=0; i<nX-1; i++)
        {
        for (j=0; j<nY-1; j++)
        {
            pMesh->phex[(i*(nY-1)+j)*(nZ-1)+(nZ-2)].label=2;
        }
    }
*/

    // Relabelling the points
    for (i=0; i<nX; i++)
    {
        for (j=0; j<nY; j++)
        {
            for (k=0; k<nZ; k++)
            {
                pMesh->pver[(i*nY+j)*nZ+k].label=labelPoint(i,j,k,nX,nY,nZ);
            }
        }
    }

    // Saving the six tetrahedra (positively oriented) inside each cube cell.
    // Point is saved as (a*ny+b)*nz+c a=0...nx-1, b=0...ny-1, c=0...nz-1
    // Point reference (a,b,c) in cube cell: 1=(i-1,j-1,k-1) 2=(i,j-1,k-1)
    // 3=(i,j,k-1) 4=(i-1,j,k-1) 5=(i-1,j-1,k) 6=(i,j-1,k) 7=(i,j,k)
    // 8=(i-1,j,k) i=1..nx-1, j=1..ny-1, k=1..nz-1
    fprintf(stdout,"\nSaving tetrahedra. ");
    kMax=pMesh->nhex;
    for (k=0; k<kMax; k++)
    {
        // Tetrahedron 1524
        pMesh->ptet[counter].p1=pMesh->phex[k].p1;
        pMesh->ptet[counter].p2=pMesh->phex[k].p5;
        pMesh->ptet[counter].p3=pMesh->phex[k].p2;
        pMesh->ptet[counter].p4=pMesh->phex[k].p4;
        pMesh->ptet[counter].label=abs(pMesh->phex[k].label);
        counter++;

        // Tetrahedron 6254
        pMesh->ptet[counter].p1=pMesh->phex[k].p6;
        pMesh->ptet[counter].p2=pMesh->phex[k].p2;
        pMesh->ptet[counter].p3=pMesh->phex[k].p5;
        pMesh->ptet[counter].p4=pMesh->phex[k].p4;
        pMesh->ptet[counter].label=abs(pMesh->phex[k].label);
        counter++;

        // Tetrahedron 2346
        pMesh->ptet[counter].p1=pMesh->phex[k].p2;
        pMesh->ptet[counter].p2=pMesh->phex[k].p3;
        pMesh->ptet[counter].p3=pMesh->phex[k].p4;
        pMesh->ptet[counter].p4=pMesh->phex[k].p6;
        pMesh->ptet[counter].label=abs(pMesh->phex[k].label);
        counter++;

        // Tetrahedron 7836
        pMesh->ptet[counter].p1=pMesh->phex[k].p7;
        pMesh->ptet[counter].p2=pMesh->phex[k].p8;
        pMesh->ptet[counter].p3=pMesh->phex[k].p3;
        pMesh->ptet[counter].p4=pMesh->phex[k].p6;
        pMesh->ptet[counter].label=abs(pMesh->phex[k].label);
        counter++;

        // Tetrahedron 6483
        pMesh->ptet[counter].p1=pMesh->phex[k].p6;
        pMesh->ptet[counter].p2=pMesh->phex[k].p4;
        pMesh->ptet[counter].p3=pMesh->phex[k].p8;
        pMesh->ptet[counter].p4=pMesh->phex[k].p3;
        pMesh->ptet[counter].label=abs(pMesh->phex[k].label);
        counter++;

        // Tetrahedron 8654
        pMesh->ptet[counter].p1=pMesh->phex[k].p8;
        pMesh->ptet[counter].p2=pMesh->phex[k].p6;
        pMesh->ptet[counter].p3=pMesh->phex[k].p5;
        pMesh->ptet[counter].p4=pMesh->phex[k].p4;
        pMesh->ptet[counter].label=abs(pMesh->phex[k].label);
        counter++;
    }

    // Saving the boundary triangles on each faces (trigonometric orientation
    // in front of it looking from the exterior of the cube)
    fprintf(stdout,"Saving boundary triangles. ");
    counter=0;

    // Face 1 (j=0) with reference (a,b,c) in square cell:
    // 1=(i-1,k-1) 2=(i,k-1) 3=(i,k) 4=(i-1,k)
    for (i=1; i<nX; i++)
    {
        for (k=1; k<nZ; k++)
        {
            // Triangle 124
            pMesh->ptri[counter].p1=1+(i-1)*nY*nZ+(k-1);
            pMesh->ptri[counter].p2=1+i*nY*nZ+(k-1);
            pMesh->ptri[counter].p3=1+(i-1)*nY*nZ+k;
            pMesh->ptri[counter].label=1;
            counter++;

            // Triangle 234
            pMesh->ptri[counter].p1=1+i*nY*nZ+(k-1);
            pMesh->ptri[counter].p2=1+i*nY*nZ+k;
            pMesh->ptri[counter].p3=1+(i-1)*nY*nZ+k;
            pMesh->ptri[counter].label=1;
            counter++;
        }
    }

    // Face 2 (i=nX-1) with reference (a,b,c) in square cell:
    // 1=(j-1,k-1) 2=(j,k-1) 3=(j,k) 4=(j-1,k)
    for (j=1; j<nY; j++)
    {
        for (k=1; k<nZ; k++)
        {
            // Triangle 124
            pMesh->ptri[counter].p1=1+((nX-1)*nY+(j-1))*nZ+(k-1);
            pMesh->ptri[counter].p2=1+((nX-1)*nY+j)*nZ+(k-1);
            pMesh->ptri[counter].p3=1+((nX-1)*nY+(j-1))*nZ+k;
            pMesh->ptri[counter].label=2;
            counter++;

            // Triangle 234
            pMesh->ptri[counter].p1=1+((nX-1)*nY+j)*nZ+(k-1);
            pMesh->ptri[counter].p2=1+((nX-1)*nY+j)*nZ+k;
            pMesh->ptri[counter].p3=1+((nX-1)*nY+(j-1))*nZ+k;
            pMesh->ptri[counter].label=2;
            counter++;
        }
    }

    // Face 3 (j=nY-1) with reference (a,b,c) in square cell:
    // 1=(i-1,k-1) 2=(i,k-1) 3=(i,k) 4=(i-1,k)
    for (i=1; i<nX; i++)
    {
        for (k=1; k<nZ; k++)
        {
            // Triangle 142
            pMesh->ptri[counter].p1=1+((i-1)*nY+(nY-1))*nZ+(k-1);
            pMesh->ptri[counter].p2=1+((i-1)*nY+(nY-1))*nZ+k;
            pMesh->ptri[counter].p3=1+(i*nY+(nY-1))*nZ+(k-1);
            pMesh->ptri[counter].label=3;
            counter++;

            // Triangle 243
            pMesh->ptri[counter].p1=1+(i*nY+(nY-1))*nZ+(k-1);
            pMesh->ptri[counter].p2=1+((i-1)*nY+(nY-1))*nZ+k;
            pMesh->ptri[counter].p3=1+(i*nY+(nY-1))*nZ+k;
            pMesh->ptri[counter].label=3;
            counter++;
        }
    }

    // Face 4 (i=0) with reference (a,b,c) in square cell:
    // 1=(j-1,k-1) 2=(j,k-1) 3=(j,k) 4=(j-1,k)
    for (j=1; j<nY; j++)
    {
        for (k=1; k<nZ; k++)
        {
            // Triangle 142
            pMesh->ptri[counter].p1=1+(j-1)*nZ+(k-1);
            pMesh->ptri[counter].p2=1+(j-1)*nZ+k;
            pMesh->ptri[counter].p3=1+j*nZ+(k-1);
            pMesh->ptri[counter].label=4;
            counter++;

            // Triangle 243
            pMesh->ptri[counter].p1=1+j*nZ+(k-1);
            pMesh->ptri[counter].p2=1+(j-1)*nZ+k;
            pMesh->ptri[counter].p3=1+j*nZ+k;
            pMesh->ptri[counter].label=4;
            counter++;
        }
    }

    // Face 5 (k=0) with reference (a,b,c) in square cell:
    // 1=(i-1,j-1) 2=(i,j-1) 3=(i,j) 4=(i-1,j)
    for (i=1; i<nX; i++)
    {
        for (j=1; j<nY; j++)
        {
            // Triangle 142
            pMesh->ptri[counter].p1=1+((i-1)*nY+(j-1))*nZ;
            pMesh->ptri[counter].p2=1+((i-1)*nY+j)*nZ;
            pMesh->ptri[counter].p3=1+(i*nY+(j-1))*nZ;
            pMesh->ptri[counter].label=5;
            counter++;

            // Triangle 243
            pMesh->ptri[counter].p1=1+(i*nY+(j-1))*nZ;
            pMesh->ptri[counter].p2=1+((i-1)*nY+j)*nZ;
            pMesh->ptri[counter].p3=1+(i*nY+j)*nZ;
            pMesh->ptri[counter].label=5;
            counter++;
        }
    }

    // Face 6 (k=nZ-1) with reference (a,b,c) in square cell:
    // 1=(i-1,j-1) 2=(i,j-1) 3=(i,j) 4=(i-1,j)
    for (i=1; i<nX; i++)
    {
        for (j=1; j<nY; j++)
        {
            // Triangle 124
            pMesh->ptri[counter].p1=1+((i-1)*nY+(j-1))*nZ+(nZ-1);
            pMesh->ptri[counter].p2=1+(i*nY+(j-1))*nZ+(nZ-1);
            pMesh->ptri[counter].p3=1+((i-1)*nY+j)*nZ+(nZ-1);
            pMesh->ptri[counter].label=6;
            counter++;

            // Triangle 234
            pMesh->ptri[counter].p1=1+(i*nY+(j-1))*nZ+(nZ-1);
            pMesh->ptri[counter].p2=1+(i*nY+j)*nZ+(nZ-1);
            pMesh->ptri[counter].p3=1+((i-1)*nY+j)*nZ+(nZ-1);
            pMesh->ptri[counter].label=6;
            counter++;
        }
    }

    // Counting the triangles of the internal surface
    counter=0;
    for (i=0; i<pMesh->nqua; i++)
    {
        if (pMesh->pqua[i].label==10)
        {
            counter++;
        }
    }
    counter*=2;
    if (counter+4*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1))!=pMesh->ntri)
    {
        PRINT_ERROR("In buildingTetrahedralMesh: expecting ");
        fprintf(stderr,"%d ",
                     pMesh->ntri-4*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1)));
        fprintf(stderr,"instead of %d for the number of ",counter);
        fprintf(stderr,"triangles associated with the boundary of the ");
        fprintf(stderr,"internal domain.\n");
        return 0;
    }

    i0=pMesh->ntri-counter;
    counter=0;
    for (k=0; k<pMesh->nqua; k++)
    {
        if (pMesh->pqua[k].label==10)
        {
            // Triangle 124
            pMesh->ptri[i0+counter].p1=pMesh->pqua[k].p1;
            pMesh->ptri[i0+counter].p2=pMesh->pqua[k].p2;
            pMesh->ptri[i0+counter].p3=pMesh->pqua[k].p4;
            pMesh->ptri[i0+counter].label=10;
            counter++;

            // Triangle 234
            pMesh->ptri[i0+counter].p1=pMesh->pqua[k].p2;
            pMesh->ptri[i0+counter].p2=pMesh->pqua[k].p3;
            pMesh->ptri[i0+counter].p3=pMesh->pqua[k].p4;
            pMesh->ptri[i0+counter].label=10;
            counter++;
        }
    }

    // Saving the boundary edges called ridges and labelled
    // 10*face1+ face2 (with label face1<face2)
    fprintf(stdout,"Saving geometry (ridges, corners, tangent and normal ");
    fprintf(stdout,"vectors).\n");
    counter=0;
    for (i=1; i<nX; i++)
    {
        // Edge 1 (j=0,k=0)
        pMesh->pedg[counter].p1=1+(i-1)*nY*nZ;
        pMesh->pedg[counter].p2=1+i*nY*nZ;
        pMesh->pedg[counter].label=15;
        counter++;
        // Edge 2 (j=nY-1,k=0)
        pMesh->pedg[counter].p1=1+((i-1)*nY+(nY-1))*nZ;
        pMesh->pedg[counter].p2=1+(i*nY+(nY-1))*nZ;
        pMesh->pedg[counter].label=35;
        counter++;
        // Edge 3 (j=nY-1,k=nZ-1)
        pMesh->pedg[counter].p1=1+((i-1)*nY+(nY-1))*nZ+(nZ-1);
        pMesh->pedg[counter].p2=1+(i*nY+(nY-1))*nZ+(nZ-1);
        pMesh->pedg[counter].label=36;
        counter++;
        // Edge 4 (j=0,k=nZ-1)
        pMesh->pedg[counter].p1=1+(i-1)*nY*nZ+(nZ-1);
        pMesh->pedg[counter].p2=1+i*nY*nZ+(nZ-1);
        pMesh->pedg[counter].label=16;
        counter++;
    }

    for (j=1; j<nY; j++)
    {
        // Edge 5 (i=0, k=0)
        pMesh->pedg[counter].p1=1+(j-1)*nZ;
        pMesh->pedg[counter].p2=1+j*nZ;
        pMesh->pedg[counter].label=45;
        counter++;
        // Edge 6 (i=nX-1, k=0)
        pMesh->pedg[counter].p1=1+((nX-1)*nY+(j-1))*nZ;
        pMesh->pedg[counter].p2=1+((nX-1)*nY+j)*nZ;
        pMesh->pedg[counter].label=25;
        counter++;
        // Edge 7 (i=nX-1, k=nZ-1)
        pMesh->pedg[counter].p1=1+((nX-1)*nY+(j-1))*nZ+(nZ-1);
        pMesh->pedg[counter].p2=1+((nX-1)*nY+j)*nZ+(nZ-1);
        pMesh->pedg[counter].label=26;
        counter++;
        // Edge 8 (i=0, k=nZ-1)
        pMesh->pedg[counter].p1=1+(j-1)*nZ+(nZ-1);
        pMesh->pedg[counter].p2=1+j*nZ+(nZ-1);
        pMesh->pedg[counter].label=46;
        counter++;
    }

    for (k=1; k<nZ; k++)
    {
        // Edge 9 (i=0,j=0)
        pMesh->pedg[counter].p1=1+(k-1);
        pMesh->pedg[counter].p2=1+k;
        pMesh->pedg[counter].label=14;
        counter++;
        // Edge 10 (i=nX-1,j=0)
        pMesh->pedg[counter].p1=1+(nX-1)*nY*nZ+(k-1);
        pMesh->pedg[counter].p2=1+(nX-1)*nY*nZ+k;
        pMesh->pedg[counter].label=12;
        counter++;
        // Edge 11 (i=nX-1, j=nY-1)
        pMesh->pedg[counter].p1=1+((nX-1)*nY+(nY-1))*nZ+(k-1);
        pMesh->pedg[counter].p2=1+((nX-1)*nY+(nY-1))*nZ+k;
        pMesh->pedg[counter].label=23;
        counter++;
        // Edge 12 (i=0, j=nY-1)
        pMesh->pedg[counter].p1=1+(nY-1)*nZ+(k-1);
        pMesh->pedg[counter].p2=1+(nY-1)*nZ+k;
        pMesh->pedg[counter].label=34;
        counter++;
    }

    // Saving the normal vectors (pointing outwards the cube)
    counter=0;

    // Face 1 (j=0)
    for (i=1;i<nX-1;i++)
    {
        for (k=1;k<nZ-1;k++)
        {
            pMesh->pnorm[counter].x=0.;
            pMesh->pnorm[counter].y=-1.;
            pMesh->pnorm[counter].z=0.;
            pMesh->pnorm[counter].p=1+i*nY*nZ+k;
            counter++;
        }
    }

    // Face 2 (i=nX-1)
    for (j=1;j<nY-1;j++)
    {
        for (k=1;k<nZ-1;k++)
        {
            pMesh->pnorm[counter].x=1.;
            pMesh->pnorm[counter].y=0.;
            pMesh->pnorm[counter].z=0.;
            pMesh->pnorm[counter].p=1+((nX-1)*nY+j)*nZ+k;
            counter++;
        }
    }

    // Face 3 (j=nY-1)
    for (i=1;i<nX-1;i++)
    {
        for (k=1;k<nZ-1;k++)
        {
            pMesh->pnorm[counter].x=0.;
            pMesh->pnorm[counter].y=1.;
            pMesh->pnorm[counter].z=0.;
            pMesh->pnorm[counter].p=1+(i*nY+(nY-1))*nZ+k;
            counter++;
        }
    }

    // Face 4 (i=0)
    for (j=1;j<nY-1;j++)
    {
        for (k=1;k<nZ-1;k++)
        {
            pMesh->pnorm[counter].x=-1.;
            pMesh->pnorm[counter].y=0.;
            pMesh->pnorm[counter].z=0.;
            pMesh->pnorm[counter].p=1+j*nZ+k;
            counter++;
        }
    }

    // Face 5 (k=0)
     for (i=1;i<nX-1;i++)
    {
        for (j=1;j<nY-1;j++)
        {
            pMesh->pnorm[counter].x=0.;
            pMesh->pnorm[counter].y=0.;
            pMesh->pnorm[counter].z=-1.;
            pMesh->pnorm[counter].p=1+(i*nY+j)*nZ;
            counter++;
        }
    }

    // Face 6 (k=nZ-1)
     for (i=1;i<nX-1;i++)
    {
        for (j=1;j<nY-1;j++)
        {
            pMesh->pnorm[counter].x=0.;
            pMesh->pnorm[counter].y=0.;
            pMesh->pnorm[counter].z=1.;
            pMesh->pnorm[counter].p=1+(i*nY+j)*nZ+(nZ-1);
            counter++;
        }
    }

    // Saving the tangent vectors
    counter=0;
    for (i=1; i<nX-1; i++)
    {
        // Edge 1 (j=0,k=0)
        pMesh->ptan[counter].x=1.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+i*nY*nZ;
        counter++;
        // Edge 2 (j=nY-1,k=0)
        pMesh->ptan[counter].x=1.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+(i*nY+(nY-1))*nZ;
        counter++;
        // Edge 3 (j=nY-1,k=nZ-1)
        pMesh->ptan[counter].x=1.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+(i*nY+(nY-1))*nZ+(nZ-1);
        counter++;
        // Edge 4 (j=0,k=nZ-1)
        pMesh->ptan[counter].x=1.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+i*nY*nZ+(nZ-1);
        counter++;
    }

    for (j=1; j<nY-1; j++)
    {
        // Edge 5 (i=0, k=0)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=1.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+j*nZ;
        counter++;
        // Edge 6 (i=nX-1, k=0)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=1.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+((nX-1)*nY+j)*nZ;
        counter++;
        // Edge 7 (i=nX-1, k=nZ-1)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=1.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+((nX-1)*nY+j)*nZ+(nZ-1);
        counter++;
        // Edge 8 (i=0, k=nZ-1)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=1.;
        pMesh->ptan[counter].z=0.;
        pMesh->ptan[counter].p=1+j*nZ+(nZ-1);
        counter++;
    }

    for (k=1; k<nZ-1; k++)
    {
        // Edge 9 (i=0,j=0)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=1.;
        pMesh->ptan[counter].p=1+k;
        counter++;
        // Edge 10 (i=nX-1,j=0)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=1.;
        pMesh->ptan[counter].p=1+(nX-1)*nY*nZ+k;
        counter++;
        // Edge 11 (i=nX-1, j=nY-1)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=1.;
        pMesh->ptan[counter].p=1+((nX-1)*nY+(nY-1))*nZ+k;
        counter++;
        // Edge 12 (i=0, j=nY-1)
        pMesh->ptan[counter].x=0.;
        pMesh->ptan[counter].y=0.;
        pMesh->ptan[counter].z=1.;
        pMesh->ptan[counter].p=1+(nY-1)*nZ+k;
        counter++;
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingTetrahedralMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh)
{
    int i=0, iMax=0;
    FILE *meshFile=NULL;

    Point *pVertex=NULL;
    Vector *pNormal=NULL, *pTangent=NULL;
    Edge *pEdge=NULL;
    Triangle *pTriangle=NULL;
    Tetrahedron *pTetrahedron=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

     // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the number of vertices and its associated pointer
    if (pMesh->nver<1)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the pMesh->nver variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nver);
        return 0;
    }
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the (Point*) variable ");
        fprintf(stderr,"pMesh->pver=%p does not point to ",(void*)pMesh->pver);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Check the number of normal vectors and its associated pointer
    if (pMesh->nnorm<1)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the pMesh->nnorm ");
        fprintf(stderr,"variable (=%d) should be a positive ",pMesh->nnorm);
        fprintf(stderr,"integer.\n");
        return 0;
    }
    if (pMesh->pnorm==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the (Vector*) variable ");
        fprintf(stderr,"pMesh->pnorm=%p does not point ",(void*)pMesh->pnorm);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of tangent vectors and its associated pointer
    if (pMesh->ntan<1)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the pMesh->ntan variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntan);
        return 0;
    }
    if (pMesh->ptan==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the (Vector*) variable ");
        fprintf(stderr,"pMesh->ptan=%p does not point ",(void*)pMesh->ptan);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of edges and its associated pointer
    if (pMesh->nedg<1)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the pMesh->nedg variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->nedg);
        return 0;
    }
    if (pMesh->pedg==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the (Edge*) variable ");
        fprintf(stderr,"pMesh->pedg=%p does not point ",(void*)pMesh->pedg);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of triangles and its associated pointer
    if (pMesh->ntri<1)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the pMesh->ntri variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntri);
        return 0;
    }
    if (pMesh->ptri==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the (Triangle*) variable ");
        fprintf(stderr,"pMesh->ptri=%p does not point ",(void*)pMesh->ptri);
        fprintf(stderr,"to a valid address.\n");
        return 0;
    }

    // Check the number of tetrahedra and its associated pointer
    if (pMesh->ntet<1)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the pMesh->ntet variable ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",pMesh->ntet);
        return 0;
    }
    if (pMesh->ptet==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the (Tetrahedron*) ");
        fprintf(stderr,"variable pMesh->ptet=%p does not ",(void*)pMesh->ptet);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Opening *.mesh file (warning: reset and overwrite file if already exists)
    fprintf(stdout,"Opening %s file. ",nameFile);
    meshFile=fopen(nameFile,"w+");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: we were not able to open ");
        fprintf(stderr,"and write into the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing tetrahedral mesh. ");

    // Writing *.mesh file starting by Vertices
    fprintf(meshFile,"MeshVersionFormatted 2\n\nDimension 3\n");

    iMax=pMesh->nver;
    fprintf(meshFile,"\nVertices\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pVertex=&pMesh->pver[i];
        fprintf(meshFile,"%.8le %.8le ",pVertex->x,pVertex->y);
        fprintf(meshFile,"%.8le %d \n",pVertex->z,pVertex->label);
    }

    // Writing Triangles
    iMax=pMesh->ntri;
    fprintf(meshFile,"\nTriangles\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pTriangle=&pMesh->ptri[i];
        if (pTriangle->p1<1 || pTriangle->p1>pMesh->nver ||
                             pTriangle->p2<1 || pTriangle->p2>pMesh->nver ||
                                   pTriangle->p3<1 || pTriangle->p3>pMesh->nver)
        {
            PRINT_ERROR("In writingTetrahedralMeshFile: the point references ");
            fprintf(stderr,"(%d,%d,",pTriangle->p1,pTriangle->p2);
            fprintf(stderr,"%d) ",pTriangle->p3);
            fprintf(stderr,"associated with the vertices of the ");
            fprintf(stderr,"%d-th triangle must be positive ",i+1);
            fprintf(stderr,"integers not (strictly) greater than the ");
            fprintf(stderr,"total number of vertices %d ",pMesh->nver);
            fprintf(stderr,"related to the mesh discretization stored ");
            fprintf(stderr,"in the structure pointed by pMesh.\n");
            closeTheFile(&meshFile);
            return 0;
        }
        fprintf(meshFile,"%d %d ",pTriangle->p1,pTriangle->p2);
        fprintf(meshFile,"%d %d \n",pTriangle->p3,pTriangle->label);
    }

    // Writing edges/ridges
    iMax=pMesh->nedg;
    fprintf(meshFile,"\nEdges\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pEdge=&pMesh->pedg[i];
        if (pEdge->p1<1 || pEdge->p1>pMesh->nver || pEdge->p2<1 ||
                                                          pEdge->p2>pMesh->nver)
        {
            PRINT_ERROR("In writingTetrahedralMeshFile: the point references ");
            fprintf(stderr,"(%d,%d,) associated ",pEdge->p1,pEdge->p2);
            fprintf(stderr,"with the vertices of the %d-th edge ",i+1);
            fprintf(stderr,"must be positive integers not (strictly) ");
            fprintf(stderr,"greater than the total number of ");
            fprintf(stderr,"vertices %d related to the ",pMesh->nver);
            fprintf(stderr,"mesh discretization stored in the ");
            fprintf(stderr,"structure pointed by pMesh.\n");
            closeTheFile(&meshFile);
            return 0;
        }
        fprintf(meshFile,"%d %d ",pEdge->p1,pEdge->p2);
        fprintf(meshFile,"%d \n",pEdge->label);
    }

    fprintf(meshFile,"\nRidges\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        fprintf(meshFile,"%d \n",i+1);
    }

    // Writing corners/requiredVertices
    fprintf(meshFile,"\nCorners\n%d\n",8);
    iMax=pMesh->nver;
    for (i=0; i<iMax; i++)
    {
        pVertex=&pMesh->pver[i];
        if (pVertex->label>100)
        {
            fprintf(meshFile,"%d \n",i+1);
        }
    }
    fprintf(meshFile,"\nRequiredVertices\n%d\n",8);
    for (i=0; i<iMax; i++)
    {
        pVertex=&pMesh->pver[i];
        if (pVertex->label>100)
        {
            fprintf(meshFile,"%d \n",i+1);
        }
    }

    // Writing tetrahedra
    iMax=pMesh->ntet;
    fprintf(meshFile,"\nTetrahedra\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pTetrahedron=&pMesh->ptet[i];
        if (pTetrahedron->p1<1 || pTetrahedron->p1>pMesh->nver ||
                       pTetrahedron->p2<1 || pTetrahedron->p2>pMesh->nver ||
                       pTetrahedron->p3<1 || pTetrahedron->p3>pMesh->nver ||
                         pTetrahedron->p4<1 || pTetrahedron->p4>pMesh->nver)
        {
            PRINT_ERROR("In writingTetrahedralMeshFile: the point references ");
            fprintf(stderr,"(%d,%d,",pTetrahedron->p1,pTetrahedron->p2);
            fprintf(stderr,"%d,%d) ",pTetrahedron->p3,pTetrahedron->p4);
            fprintf(stderr,"associated with the vertices of the ");
            fprintf(stderr,"%d-th tetrahedron must be positive ",i+1);
            fprintf(stderr,"integers not (strictly) greater than the ");
            fprintf(stderr,"total number of vertices %d ",pMesh->nver);
            fprintf(stderr,"related to the mesh discretization stored ");
            fprintf(stderr,"in the structure pointed by pMesh.\n");
            closeTheFile(&meshFile);
            return 0;
        }
        fprintf(meshFile,"%d %d ",pTetrahedron->p1,pTetrahedron->p2);
        fprintf(meshFile,"%d %d ",pTetrahedron->p3,pTetrahedron->p4);
        fprintf(meshFile,"%d \n",pTetrahedron->label);
    }

    // Writing normal vectors
    iMax=pMesh->nnorm;
    fprintf(meshFile,"\nNormals\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pNormal=&pMesh->pnorm[i];
        fprintf(meshFile,"%.8le %.8le ",pNormal->x,pNormal->y);
        fprintf(meshFile,"%.8le \n",pNormal->z);
    }
    fprintf(meshFile,"\nNormalAtVertices\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pNormal=&pMesh->pnorm[i];
        if (pNormal->p<1 || pNormal->p>pMesh->nver)
        {
            PRINT_ERROR("In writingTetrahedralMeshFile: the point reference ");
            fprintf(stderr,"(=%d) associated with the ",pNormal->p);
            fprintf(stderr,"vertex where the %d-th normal vector ",i+1);
            fprintf(stderr,"originated from must be positive ");
            fprintf(stderr,"integers not (strictly) greater than the ");
            fprintf(stderr,"total number of vertices %d ",pMesh->nver);
            fprintf(stderr,"related to the mesh discretization ");
            fprintf(stderr,"stored in the structure pointed by ");
            fprintf(stderr,"pMesh.\n");
            closeTheFile(&meshFile);
            return 0;
        }
        fprintf(meshFile,"%d %d \n",pNormal->p,i+1);
    }

    // Writing tangent vectors
    iMax=pMesh->ntan;
    fprintf(meshFile,"\nTangents\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pTangent=&pMesh->ptan[i];
        if (pTangent->p<1 || pTangent->p>pMesh->nver)
        {
            PRINT_ERROR("In writingTetrahedralMeshFile: the point reference ");
            fprintf(stderr,"(=%d) associated with the ",pTangent->p);
            fprintf(stderr,"vertex where the %d-th tangent ",i+1);
            fprintf(stderr,"vector originated from must be positive ");
            fprintf(stderr,"integers not (strictly) greater than the ");
            fprintf(stderr,"total number of vertices %d ",pMesh->nver);
            fprintf(stderr,"related to the mesh discretization ");
            fprintf(stderr,"stored in the structure pointed by ");
            closeTheFile(&meshFile);
            return 0;
        }
        fprintf(meshFile,"%.8le %.8le ",pTangent->x,pTangent->y);
        fprintf(meshFile,"%.8le \n",pTangent->z);
    }
    fprintf(meshFile,"\nTangentAtVertices\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pTangent=&pMesh->ptan[i];
        fprintf(meshFile,"%d %d \n",pTangent->p,i+1);
    }
    fprintf(meshFile,"\nEnd\n\n");

    // Closing the *.mesh file
    if (fclose(meshFile))
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the ");
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
    char extension[2][14]={"input","output"};
    int length=0, i=0, boolean=0, nX=0, nY=0, nZ=0;
    Mesh mesh;

    // Initialize structure to zero
    initializeMeshStructure(&mesh);

    // Check the number of arguments
    if (argc!=3)
    {
        PRINT_ERROR("In main: the program only takes two arguments ");
        fprintf(stderr,"instead of %d.\n",argc-1);
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
            fprintf(stderr,"%s file name should be a string ",extension[i-1]);
            fprintf(stderr,"of at most %d characters (and ",NAME_SIZE_MAX-1);
            fprintf(stderr,"strictly more than 5 to contain at least ");
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
            fprintf(stderr,"*.mesh extension does not correspond ");
            fprintf(stderr,"to the one of the %s file name.\n",argv[i]);
            freeMeshMemory(&mesh);
            return EXIT_FAILURE;
        }
    }

    // Get the number of mesh elements
    if (!getNumberOfVerticesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: getNumberOfVerticesInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }
    if (!getNumberOfQuadrilateralsInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: getNumberOfQuadrilateralsInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    if (!getNumberOfHexahedraInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: getNumberOfHexahedraInMeshFile function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Allocate memory for the hexahedral mesh
    mesh.pver=(Point*)calloc(mesh.nver,sizeof(Point));
    mesh.pqua=(Quadrilateral*)calloc(mesh.nqua,sizeof(Quadrilateral));
    mesh.phex=(Hexahedron*)calloc(mesh.nhex,sizeof(Hexahedron));
    if (mesh.pver==NULL || mesh.pqua==NULL || mesh.phex==NULL)
    {
        PRINT_ERROR("In main: could not allocate memory for the hexahedral ");
        fprintf(stderr,"mesh variables.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Load the hexahedral mesh
    if(!readVerticesInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readVerticesInMeshFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }
    if(!readQuadrilateralsInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readQuadrilateralsInMeshFile function returned ");
        fprintf(stderr,"zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }
    if(!readHexahedraInMeshFile(argv[1],&mesh))
    {
        PRINT_ERROR("In main: readHexahedraInMeshFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Extract the discretization parameters
    if (!getNumberOfDiscretizationPoints(&mesh,&nX,&nY,&nZ))
    {
        PRINT_ERROR("In main: getNumberOfDiscretizationPoints function ");
        fprintf(stderr,"returned zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Allocate memory for the tetrahedral mesh
    mesh.ntet=6*mesh.nhex;
    mesh.ptet=(Tetrahedron*)calloc(mesh.ntet,sizeof(Tetrahedron));
    if (mesh.ptet==NULL)
    {
        PRINT_ERROR("In main: could not allocate memory for the mesh.ptet ");
        fprintf(stderr,"variable.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    mesh.ntri=2*mesh.nqua;
    mesh.ptri=(Triangle*)calloc(mesh.ntri,sizeof(Triangle));
    if (mesh.ptri==NULL)
    {
        PRINT_ERROR("In main: could not allocate memory for the mesh.ptri ");
        fprintf(stderr,"variable.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    mesh.nedg=4*((nX-1)+(nY-1)+(nZ-1));
    mesh.pedg=(Edge*)calloc(mesh.nedg,sizeof(Edge));
    if (mesh.pedg==NULL)
    {
        PRINT_ERROR("In main: could not allocate memory for the mesh.pedg ");
        fprintf(stderr,"variable.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    mesh.nnorm=2*((nX-2)*(nY-2)+(nX-2)*(nZ-2)+(nY-2)*(nZ-2));
    mesh.pnorm=(Vector*)calloc(mesh.nnorm,sizeof(Vector));
    if (mesh.pnorm==NULL)
    {
        PRINT_ERROR("In main: could not allocate memory for the mesh.pnorm ");
        fprintf(stderr,"variable.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    mesh.ntan=4*((nX-2)+(nY-2)+(nZ-2));
    mesh.ptan=(Vector*)calloc(mesh.ntan,sizeof(Vector));
    if (mesh.ptan==NULL)
    {
        PRINT_ERROR("In main: could not allocate memory for the mesh.ptan ");
        fprintf(stderr,"variable.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Build the tetrahedral mesh from the hexahedral one
    if (!buildingTetrahedralMesh(&mesh,nX,nY,nZ))
    {
        PRINT_ERROR("In main: buildingTetrahedralMesh function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Save the tetrahedral in the ouput *.mesh file
    if(!writingTetrahedralMeshFile(argv[2],&mesh))
    {
        PRINT_ERROR("In main: writingTetrahedralMeshFile function returned ");
        fprintf(stderr,"zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Free the memory dynamically allocated for the mesh variables
    freeMeshMemory(&mesh);

    return EXIT_SUCCESS;
}

