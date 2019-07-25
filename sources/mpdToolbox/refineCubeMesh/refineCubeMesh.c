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
    double value;
} Point;

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
int getDiscretizationParameters(Mesh* pMesh, int* pNx, int* pNy, int* pNz,
                                double* pXmin, double* pYmin, double* pZmin,
                               double* pDeltax,double* pDeltay,double* pDeltaz);
int writingRefinedMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh, int nX,
                           int nY, int nZ, double xMin, double yMmin,
                           double zMmin, double deltaX, double deltaY,
                                                                 double deltaZ);

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
int getDiscretizationParameters(Mesh* pMesh, int* pNx, int* pNy, int* pNz,
                                double* pXmin, double* pYmin, double* pZmin,
                                double* pDeltax,double* pDeltay,double* pDeltaz)
{
    int nX=0, nY=0, nZ=0, i=0, iMax=0, j=0, k=0, boolean=0;
    double deltaX=0., deltaY=0., deltaZ=0., xMin=0., yMin=0., zMin=0., xMax=0.;
    double yMax=0., zMax=0., cx=0., cy=0., cz=0.;
    Point *pVertex=NULL;

    // Check if the input pParameters or pMesh variable is pointing to NULL
    if (pMesh==NULL || pNx==NULL || pNy==NULL || pNz==NULL || pXmin==NULL ||
                 pYmin==NULL || pZmin==NULL || pDeltax==NULL || pDeltay==NULL ||
                                                                  pDeltaz==NULL)
    {
        PRINT_ERROR("In getDiscretizationParameters: at least one of the ");
        fprintf(stderr,"input variables pMesh=%p, ",(void*)pMesh);
        fprintf(stderr,"pNx=%p, pNy=%p, ",(void*)pNx,(void*)pNy);
        fprintf(stderr,"pNz=%p, pXmin=%p, ",(void*)pNz,(void*)pXmin);
        fprintf(stderr,"pYmin=%p, pZmin=%p, ",(void*)pYmin,(void*)pZmin);
        fprintf(stderr,"pDeltax=%p, pDeltay=%p ",(void*)pDeltax,(void*)pDeltay);
        fprintf(stderr,"and/or pDeltaz=%p does not point to a ",(void*)pDeltaz);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the pMesh->pver variable
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In getDiscretizationParameters: no Point data have ");
        fprintf(stderr,"been saved in the structure pointed by pMesh since ");
        fprintf(stderr,"its pver variable is pointing to the ");
        fprintf(stderr,"%p adress.\n",(void*)pMesh->pver);
        return 0;
    }

    // Check the number of vertices
    if (pMesh->nver<9)
    {
        PRINT_ERROR("In getDiscretizationParameters: we were expecting ");
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
        PRINT_ERROR("In getDiscretizationParameters: expecting\n");
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
        PRINT_ERROR("In getDiscretizationParameters: no hexahedral grid ");
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
        PRINT_ERROR("In getDiscretizationParameters: expecting ");
        fprintf(stderr,"%d instead of %d for the local variable ",nY*nZ,i);
        fprintf(stderr,"i, in the attempt of updating the local nX and ");
        fprintf(stderr,"nZ variables.\n");
        return 0;
    }
    nX=iMax/i;

    if (iMax%i!=0)
    {
        PRINT_ERROR("In getDiscretizationParameters: expecting ");
        fprintf(stderr,"%d instead of %d for the local ",nX*nY*nZ,iMax);
        fprintf(stderr,"variable i, in the attempt of updating the local ");
        fprintf(stderr,"nX variable.\n");
        return 0;
    }

    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In getDiscretizationParameters: expecting ");
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
                    PRINT_ERROR("getDiscretizationParameters: ");
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
                    PRINT_ERROR("getDiscretizationParameters: ");
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
                    PRINT_ERROR("getDiscretizationParameters: ");
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

    *pXmin=xMin;
    *pYmin=yMin;
    *pZmin=zMin;

    *pDeltax=deltaX;
    *pDeltay=deltaY;
    *pDeltaz=deltaZ;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingRefinedMeshFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh, int nX,
                           int nY, int nZ, double xMin, double yMin,
                           double zMin, double deltaX, double deltaY,
                                                                  double deltaZ)
{
    int i=0, j=0, k=0, l=0, counter=0, ii=0, jj=0, kk=0, ll=0, nVer=0, nHex=0;
    int nQua=0, lPoint=0, *llabel=NULL;
    FILE* meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointer is not set to NULL
    if (pMesh==NULL)
    {
        PRINT_ERROR("In writingRefinedMeshFile: the input (Mesh*) ");
        fprintf(stderr,"variable pMesh=%p does not point to a ",(void*)pMesh);
        fprintf(stderr,"valid adress.\n");
        return 0;
    }

    // Check the number of hexahedra and its associated pointer
    if (pMesh->nhex<1)
    {
        PRINT_ERROR("In writingRefinedMeshFile: the pMesh->nhex ");
        fprintf(stderr,"variable (=%d) should be a positive ",pMesh->nhex);
        fprintf(stderr,"integer.\n");
        return 0;
    }
    if (pMesh->phex==NULL)
    {
        PRINT_ERROR("In writingRefinedMeshFile: the (Hexahedron*) ");
        fprintf(stderr,"variable pMesh->phex=%p does not ",(void*)pMesh->phex);
        fprintf(stderr,"point to a valid address.\n");
        return 0;
    }

    // Check the number of points
    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In writingRefinedMeshFile: expecting ");
        fprintf(stderr,"that the values (nX=%d, nY=%d, nZ=%d) ",nX,nY,nZ);
        fprintf(stderr,"suggested for the input nX, nY and nZ variables");
        fprintf(stderr,"would be integers (strictly) greater than two.\n");
        return 0;
    }

    // Check the discretization steps
    if (deltaX<=0. || deltaY<=0. || deltaZ<=0.)
    {
        PRINT_ERROR("In writingRefinedMeshFile: expecting ");
        fprintf(stderr,"that the values (deltaX=%lf, ",deltaX);
        fprintf(stderr,"deltaY=%lf, deltaZ=%lf) suggested for ",deltaY,deltaZ);
        fprintf(stderr,"input deltaX, deltaY and deltaZ variables");
        fprintf(stderr,"would be (strictly) positive.\n");
        return 0;
    }

    // Compute the number of vertices, hexahedra and quadrilaterals of the grid
    nVer=nX*nY*nZ;
    nHex=(nX-1)*(nY-1)*(nZ-1);
    nQua=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));
    if (nVer<1 || nHex<1 || nQua<1)
    {
        PRINT_ERROR("In writingRefinedMeshFile: expecting ");
        fprintf(stderr,"that the values (nVer=%d, nHex=%d, ",nVer,nHex);
        fprintf(stderr,"nQua=%d) suggested for the local nVer, nHex and ",nQua);
        fprintf(stderr,"nQua variables would be positive integers.\n");
        return 0;
    }

    // Allocate memory for labels
    llabel=(int*)calloc(nHex,sizeof(int));
    if (llabel==NULL)
    {
        PRINT_ERROR("In writingRefinedMeshFile: could not allocate memory ");
        fprintf(stderr,"for labels.\n");
        return 0;
    }

    // Opening *.mesh file (warning: reset and overwrite file if already exists)
    fprintf(stdout,"\nOpening %s file. ",nameFile);
    meshFile=fopen(nameFile,"w+");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In writingRefinedMeshFile: we were not able to open ");
        fprintf(stderr,"and write into the %s file properly.\n",nameFile);
        free(llabel);
        llabel=NULL;
        return 0;
    }
    fprintf(stdout,"Writing data.\n");

    // Saving Vertices
    fprintf(meshFile,"MeshVersionFormatted 2\n\nDimension 3\n");
    fprintf(meshFile,"\nVertices\n%d\n",nVer);
    for (i=0; i<nX; i++)
    {
        for (j=0; j<nY; j++)
        {
            for (k=0; k<nZ; k++)
            {
                fprintf(meshFile,"%.8le %.8le %.8le %d \n",xMin+i*deltaX,
                                                 yMin+j*deltaY,zMin+k*deltaZ,0);
            }
        }
    }

    // Saving Hexahedra
    counter=0;
    fprintf(meshFile,"\nHexahedra\n%d\n",nHex);
    for (i=1; i<nX; i++)
    {
        for (j=1; j<nY; j++)
        {
            for (k=1; k<nZ; k++)
            {
                fprintf(meshFile,"%d ",1+((i-1)*nY+(j-1))*nZ+(k-1));
                fprintf(meshFile,"%d ",1+(i*nY+(j-1))*nZ+(k-1));
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(k-1));
                fprintf(meshFile,"%d ",1+((i-1)*nY+j)*nZ+(k-1));
                fprintf(meshFile,"%d ",1+((i-1)*nY+(j-1))*nZ+k);
                fprintf(meshFile,"%d ",1+(i*nY+(j-1))*nZ+k);
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+k);
                fprintf(meshFile,"%d ",1+((i-1)*nY+j)*nZ+k);

                // Get the old references
                if (i%2)
                {
                    ii=i/2;
                }
                else
                {
                    ii=i/2-1;
                }

                if (j%2)
                {
                    jj=j/2;
                }
                else
                {
                    jj=j/2-1;
                }

                if (k%2)
                {
                    kk=k/2;
                }
                else
                {
                    kk=k/2-1;
                }
                ll=(ii*((nY-1)/2)+jj)*((nZ-1)/2)+kk;

                if (abs(pMesh->phex[ll].label)==3)
                {
                    fprintf(meshFile,"%d \n",3);
                    llabel[counter]=3;
                }
                else
                {
                    fprintf(meshFile,"%d \n",2);
                    llabel[counter]=2;
                }
                counter++;
            }
        }
    }

    // Counting the number of boundary quadrilaterals of the internal domain
    counter=0;
    for (l=0; l<nHex; l++)
    {
        k=l%(nZ-1);
        lPoint=l/(nZ-1);
        j=lPoint%(nY-1);
        i=lPoint/(nY-1);
        if (i>0)
        {
            lPoint=((i-1)*(nY-1)+j)*(nZ-1)+k;

            /// Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                counter++;
            }
        }
        if (i<nX-2)
        {
            lPoint=((i+1)*(nY-1)+j)*(nZ-1)+k;

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                counter++;
            }
        }
        if (j>0)
        {
            lPoint=(i*(nY-1)+(j-1))*(nZ-1)+k;

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                counter++;
            }
        }
        if (j<nY-2)
        {
            lPoint=(i*(nY-1)+(j+1))*(nZ-1)+k;

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                counter++;
            }
        }
        if (k>0)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k-1);

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                counter++;
            }
        }
        if (k<nZ-2)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k+1);

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                counter++;
            }
        }
    }
    fprintf(stdout,"Adding %d quadrilaterals for the internal ",counter);
    fprintf(stdout,"domains (total is now %d).\n",counter+nQua);


    // Saving quadrilaterals
    fprintf(meshFile,"\nQuadrilaterals\n%d\n",nQua+counter);

    // Face 1 (j=0) with reference (a,b,c) in each quadrilateral corresponding
    // to square cell: 1=(i-1,k-1) 2=(i,k-1) 3=(i,k) 4=(i-1,k)
    for (i=1; i<nX; i++)
    {
        for (k=1; k<nZ; k++)
        {
            // Square 1234
            fprintf(meshFile,"%d ",1+(i-1)*nY*nZ+(k-1));
            fprintf(meshFile,"%d ",1+i*nY*nZ+(k-1));
            fprintf(meshFile,"%d ",1+i*nY*nZ+k);
            fprintf(meshFile,"%d ",1+(i-1)*nY*nZ+k);
            fprintf(meshFile,"%d \n",1);
        }
    }

    // Face 2 (i=nX-1) with reference (a,b,c) in square cell:
    // 1=(j-1,k-1) 2=(j,k-1) 3=(j,k) 4=(j-1,k)
    for (j=1; j<nY; j++)
    {
        for (k=1; k<nZ; k++)
        {
            // Square 1234
            fprintf(meshFile,"%d ",1+((nX-1)*nY+(j-1))*nZ+(k-1));
            fprintf(meshFile,"%d ",1+((nX-1)*nY+j)*nZ+(k-1));
            fprintf(meshFile,"%d ",1+((nX-1)*nY+j)*nZ+k);
            fprintf(meshFile,"%d ",1+((nX-1)*nY+(j-1))*nZ+k);
            fprintf(meshFile,"%d \n",2);
        }
    }

    // Face 3 (j=nY-1) with reference (a,b,c) in square cell:
    // 1=(i-1,k-1) 2=(i,k-1) 3=(i,k) 4=(i-1,k)
    for (i=1; i<nX; i++)
    {
        for (k=1; k<nZ; k++)
        {
            // Square 1432
            fprintf(meshFile,"%d ",1+((i-1)*nY+(nY-1))*nZ+(k-1));
            fprintf(meshFile,"%d ",1+((i-1)*nY+(nY-1))*nZ+k);
            fprintf(meshFile,"%d ",1+(i*nY+(nY-1))*nZ+k);
            fprintf(meshFile,"%d ",1+(i*nY+(nY-1))*nZ+(k-1));
            fprintf(meshFile,"%d \n",3);
        }
    }

    // Face 4 (i=0) with reference (a,b,c) in square cell:
    // 1=(j-1,k-1) 2=(j,k-1) 3=(j,k) 4=(j-1,k)
    for (j=1; j<nY; j++)
    {
        for (k=1; k<nZ; k++)
        {
            // Square 1432
            fprintf(meshFile,"%d ",1+(j-1)*nZ+(k-1));
            fprintf(meshFile,"%d ",1+(j-1)*nZ+k);
            fprintf(meshFile,"%d ",1+j*nZ+k);
            fprintf(meshFile,"%d ",1+j*nZ+(k-1));
            fprintf(meshFile,"%d \n",4);
        }
    }

    // Face 5 (k=0) with reference (a,b,c) in square cell:
    // 1=(i-1,j-1) 2=(i,j-1) 3=(i,j) 4=(i-1,j)
    for (i=1; i<nX; i++)
    {
        for (j=1; j<nY; j++)
        {
            // Square 1432
            fprintf(meshFile,"%d ",1+((i-1)*nY+(j-1))*nZ);
            fprintf(meshFile,"%d ",1+((i-1)*nY+j)*nZ);
            fprintf(meshFile,"%d ",1+(i*nY+j)*nZ);
            fprintf(meshFile,"%d ",1+(i*nY+(j-1))*nZ);
            fprintf(meshFile,"%d \n",5);
        }
    }

    // Face 6 (k=nZ-1) with reference (a,b,c) in square cell:
    // 1=(i-1,j-1) 2=(i,j-1) 3=(i,j) 4=(i-1,j)
    for (i=1; i<nX; i++)
    {
        for (j=1; j<nY; j++)
        {
            // Square 1234
            fprintf(meshFile,"%d ",1+((i-1)*nY+(j-1))*nZ+(nZ-1));
            fprintf(meshFile,"%d ",1+(i*nY+(j-1))*nZ+(nZ-1));
            fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(nZ-1));
            fprintf(meshFile,"%d ",1+((i-1)*nY+j)*nZ+(nZ-1));
            fprintf(meshFile,"%d \n",6);
        }
    }

    // Saving boundary quadrilaterals corresponding to the internal domain
    for (l=0; l<nHex; l++)
    {
        k=l%(nZ-1);
        lPoint=l/(nZ-1);
        j=lPoint%(nY-1);
        i=lPoint/(nY-1);
        if (i>0)
        {
            lPoint=((i-1)*(nY-1)+j)*(nZ-1)+k;

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+k);
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(k+1));
                fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+(k+1));
                fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+k);
                fprintf(meshFile,"%d \n",10);
            }
        }
        if (i<nX-2)
        {
            lPoint=((i+1)*(nY-1)+j)*(nZ-1)+k;

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+k);
                fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+(k+1));
                fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+(k+1));
                fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+k);
                fprintf(meshFile,"%d \n",10);
            }
        }
        if (j>0)
        {
            lPoint=(i*(nY-1)+(j-1))*(nZ-1)+k;

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+k);
                fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+k);
                fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+(k+1));
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(k+1));
                fprintf(meshFile,"%d \n",10);
            }
        }
        if (j<nY-2)
        {
            lPoint=(i*(nY-1)+(j+1))*(nZ-1)+k;

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+k);
                fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+k);
                fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+(k+1));
                fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+(k+1));
                fprintf(meshFile,"%d \n",10);
            }
        }
        if (k>0)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k-1);

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+k);
                fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+k);
                fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+k);
                fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+k);
                fprintf(meshFile,"%d \n",10);
            }
        }
        if (k<nZ-2)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k+1);

            // Test not to count two times the quadrilaterals
            if (llabel[l]==3 && llabel[lPoint]==2)
            {
                fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(k+1));
                fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+(k+1));
                fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+(k+1));
                fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+(k+1));
                fprintf(meshFile,"%d \n",10);
            }
        }
    }
    fprintf(meshFile,"\nEnd\n\n");

    // Closing the *.mesh file
    if (fclose(meshFile))
    {
        PRINT_ERROR("In writingTetrahedralMeshFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",nameFile);
        free(llabel);
        llabel=NULL;
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    // Free the memory
    free(llabel);
    llabel=NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    char extension[2][7]={"input","output"};
    int length=0, i=0, boolean=0, nX=0, nY=0, nZ=0, nnX=0, nnY=0, nnZ=0;
    int nVer=0, nQua=0, nHex=0;
    double deltaX=0., deltaY=0., deltaZ=0., xMin=0., yMin=0., zMin=0., dX=0.;
    double dY=0., dZ=0.;
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

    // Check the length and extension of the input and output file names
    for (i=1; i<argc; i++)
    {
        length=checkStringFromLength(argv[i],7,NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s *.mesh file name should be a ",extension[i-1]);
            fprintf(stderr,"string of at most %d characters ",NAME_SIZE_MAX-1);
            fprintf(stderr,"(and strictly more than 5 to contain at least ");
            fprintf(stderr,"something more than the *.mesh extension).\n");
            return EXIT_FAILURE;
        }

        boolean=(argv[i][length-6]=='.' && argv[i][length-5]=='m');
        boolean=(boolean && argv[i][length-4]=='e' && argv[i][length-3]=='s');
        boolean=(boolean && argv[i][length-2]=='h' && argv[i][length-1]=='\0');
        if (!boolean)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"*.mesh extension does not correspond to the ");
            fprintf(stderr,"one of the %s file name.\n",argv[i]);
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
    if (!getDiscretizationParameters(&mesh,&nX,&nY,&nZ,&xMin,&yMin,&zMin,
                                                       &deltaX,&deltaY,&deltaZ))
    {
        PRINT_ERROR("In main: getDiscretizationParameters function returned ");
        fprintf(stderr,"zero instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    dX=.5*deltaX;
    dY=.5*deltaY;
    dZ=.5*deltaZ;

    nnX=2*nX-1;
    nnY=2*nY-1;
    nnZ=2*nZ-1;

    nVer=nnX*nnY*nnZ;
    nHex=(nnX-1)*(nnY-1)*(nnZ-1);
    nQua=2*((nnX-1)*(nnY-1)+(nnX-1)*(nnZ-1)+(nnY-1)*(nnZ-1));

    fprintf(stdout,"\nRefining the grid by two.\n");
    fprintf(stdout,"\nNew mesh: %d vertices %d hexahedra and ",nVer,nHex);
    fprintf(stdout,"%d boundary quadrilaterals\n",nQua);
    fprintf(stdout,"with (%d)x(%d)(%d) points and ",nnX,nnY,nnZ);
    fprintf(stdout,"(%lf)x(%lf)x(%lf) discretization steps.\n",dX,dY,dZ);

    if (!writingRefinedMeshFile(argv[2],&mesh,nnX,nnY,nnZ,xMin,yMin,zMin,dX,dY,
                                                                            dZ))
    {
        PRINT_ERROR("In main: writingRefinedMeshFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Free the memory
    freeMeshMemory(&mesh);

    return EXIT_SUCCESS;
}

