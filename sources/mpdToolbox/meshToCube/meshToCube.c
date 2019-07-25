#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

// Define the label of hexahedra belonging to the interior of the domain
#define LABEL_INTERIOR 3

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
int getNumberOfNucleiInWfnFile(char nameFile[NAME_SIZE_MAX]);
int readNucleiFromWfnFile(char nameFile[NAME_SIZE_MAX], int nNucl, int* pc,
                                            double* px, double* py, double* pz);
int initialFileExists(char nameFile[NAME_SIZE_MAX]);
int readLsSolFile(char nameFile[NAME_SIZE_MAX], Mesh* pMesh);

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
int getNumberOfNucleiInWfnFile(char nameFile[NAME_SIZE_MAX])
{
    int readIntegerOut=0, nMorb=0, nPrim=0, nNucl=0, readChar=0;
    FILE *wfnFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Open the *.sol file: it must have been previously created in reading mode
    fprintf(stdout,"\nOpening the %s file. ",nameFile);
    wfnFile=fopen(nameFile,"r");
    if (wfnFile==NULL)
    {
        PRINT_ERROR("In getNumberOfNucleiInWfnFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading nuclei. ");

    // Skip the first line
    do
    {
        readChar=fgetc(wfnFile);
    } while (readChar!='\n' && readChar!=EOF);
    if (readChar==EOF)
    {
        PRINT_ERROR("In getNumberOfNucleiInWfnFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any nucleus found.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    // Read the number of nuclei
    readIntegerOut=fscanf(wfnFile,
                         " GAUSSIAN %d MOL ORBITALS %d PRIMITIVES %d NUCLEI ",
                                                          &nMorb,&nPrim,&nNucl);
    if (readIntegerOut!=3)
    {
        PRINT_ERROR("In getNumberOfNucleiInWfnFile: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function in the attempt of reading the ");
        fprintf(stderr,"number of nuclei.\n");
        closeTheFile(&wfnFile);
        return 0;
    }
    if (nNucl<1)
    {
        PRINT_ERROR("In getNumberOfNucleiInWfnFile: expecting a positive ");
        fprintf(stderr,"number of nuclei instead of %d.\n",nNucl);
        closeTheFile(&wfnFile);
        return 0;
    }

    // Close input the *.wfn file
    if (fclose(wfnFile))
    {
        fprintf(stdout,"\nWarning in getNumberOfNucleiInWfnFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        wfnFile=NULL;
        return 0;
    }
    wfnFile=NULL;
    fprintf(stdout,"Closing file (%d nuclei found).\n",nNucl);

    return nNucl;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int readNucleiFromWfnFile(char nameFile[NAME_SIZE_MAX], int nNucl, int* pc,
                                             double* px, double* py, double* pz)
{
    char *readStringOut=NULL, keyword[3]={'\0'};
    int i=0, readIntegerOut=0, nMorb=0, nPrim=0, readIntegerIn=0, readChar=0;
    double readDouble=0.;
    FILE *wfnFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check the number of nuclei
    if (nNucl<1)
    {
        PRINT_ERROR("In readNucleiFromWfnFile: the total number of nuclei ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nNucl);
        return 0;
    }

    // Check the input pointers
    if (px==NULL || py==NULL || pz==NULL || pc==NULL)
    {
        PRINT_ERROR("In readNucleiFromWfnFile: at least one of the input ");
        fprintf(stderr,"variables px=%p, py=%p, ",(void*)px,(void*)py);
        fprintf(stderr,"variables py=%p and/or pc=%p ",(void*)pz,(void*)pc);
        fprintf(stderr,"does not point to a valid adress.\n");
        return 0;
    }

    // Open the *.sol file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    wfnFile=fopen(nameFile,"r");
    if (wfnFile==NULL)
    {
        PRINT_ERROR("In readNucleiFromWfnFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading nuclei charge and positions. ");

    // Skip the first line
    do
    {
        readChar=fgetc(wfnFile);

    } while (readChar!='\n' && readChar!=EOF);
    if (readChar==EOF)
    {
        PRINT_ERROR("In readNucleiFromWfnFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any nucleus found.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    // Read the number of nuclei
    readIntegerOut=fscanf(wfnFile,
                         " GAUSSIAN %d MOL ORBITALS %d PRIMITIVES %d NUCLEI ",
                                                  &nMorb,&nPrim,&readIntegerIn);
    if (readIntegerOut!=3)
    {
        PRINT_ERROR("In readNucleiFromWfnFile: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function in the attempt of reading the ");
        fprintf(stderr,"number of nuclei.\n");
        closeTheFile(&wfnFile);
        return 0;
    }
    if (nNucl!=readIntegerIn)
    {
        PRINT_ERROR("In readNucleiFromWfnFile: expecting ");
        fprintf(stderr,"%d instead of %d for the total ",nNucl,readIntegerIn);
        fprintf(stderr,"number of nuclei.\n");
        closeTheFile(&wfnFile);
        return 0;
    }

    for (i=0; i<nNucl; i++)
    {
        // Read the (i+1)-th atom name
        readStringOut=fgets(keyword,3,wfnFile);
        if (readStringOut==NULL)
        {
            PRINT_ERROR("In readNucleiFromWfnFile: wrong return of the ");
            fprintf(stderr,"standard fgets=%p ",(void*)readStringOut);
            fprintf(stderr,"c-function in the attempt of reading the ");
            fprintf(stderr,"atom associated with the %d-th nucleus.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        // Read the position and charge
        readIntegerOut=fscanf(wfnFile,
                              " %d (CENTRE %d) %lf %lf %lf CHARGE = %lf ",
                              &readIntegerIn, &readChar,&px[i],&py[i],&pz[i],
                                                                   &readDouble);
        if (readIntegerOut!=6)
        {
            PRINT_ERROR("In readNucleiFromWfnFile: wrong return ");
            fprintf(stderr,"(=%d instead of 6) of the ",readIntegerOut);
            fprintf(stderr,"fscanf c-function in the attempt of reading the ");
            fprintf(stderr,"charge and positions of the %d-th nucleus.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }

        if (readIntegerIn!=i+1 || readChar!=i+1)
        {
            PRINT_ERROR("In readNucleiFromWfnFile: expecting ");
            fprintf(stderr,"%d (CENTRE %d) instead of ",i+1,i+1);
            fprintf(stderr,"%d (CENTRE %d) while ",readIntegerIn,readChar);
            fprintf(stderr,"reading the charge and position of the ");
            fprintf(stderr,"%d-th nucleus.\n",i+1);
            closeTheFile(&wfnFile);
            return 0;
        }
        if (readDouble<1. || readDouble>100.)
        {
            PRINT_ERROR("In readNucleiFromWfnFile: the charge read ");
            fprintf(stderr,"(=%lf) for the %d-th nucleus ",readDouble,i+1);
            fprintf(stderr,"should correspond to an integer value comprised ");
            fprintf(stderr,"between one and one hundred.\n");
            closeTheFile(&wfnFile);
            return 0;
        }
        pc[i]=(int)readDouble;
    }

    // Close input the *.wfn file
    if (fclose(wfnFile))
    {
        fprintf(stdout,"\nWarning in readNucleiFromWfnFile ");
        fprintf(stdout,"function: we could not close the %s file ",nameFile);
        fprintf(stdout,"properly.\n");
        wfnFile=NULL;
        return 0;
    }
    wfnFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int initialFileExists(char nameFile[NAME_SIZE_MAX])
{
    FILE *pFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    pFile=fopen(nameFile,"r");
    if (pFile==NULL)
    {
        return -1;
    }

    if (fclose(pFile))
    {
        PRINT_ERROR("In initialFileExists: could not close ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        pFile=NULL;
        return 0;
    }
    pFile=NULL;

    return 1;
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

    readIntegerOut=initialFileExists(nameSolFile);
    if (abs(readIntegerOut)!=1)
    {
        PRINT_ERROR("In readLsSolFile: initialFileExists function returned ");
        fprintf(stderr,"zero instead of (+/-)one.\n");
        return 0;
    }
    else if (readIntegerOut==1)
    {
        // Open the input *.sol file: it must already exist in reading mode
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
            fprintf(stderr,"(=%d) c-functions in the attempt ",readIntegerOut);
            fprintf(stderr,"of reading the MeshVersionFormatted keyword.\n");
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
            fprintf(stderr,"(=%d) c-functions in the attempt ",readIntegerOut);
            fprintf(stderr,"of reading the Dimension keyword.\n");
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
            fprintf(stderr,"(=%d) c-functions in the attempt ",readIntegerOut);
            fprintf(stderr,"of reading the SolAtVertices keyword.\n");
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
            fprintf(stderr,"c-function in the attempt of reading the number ");
            fprintf(stderr,"of solutions.\n");
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
            fprintf(stdout,"\nWarning in readLsSolFile function: we could ");
            fprintf(stdout,"not close the %s file properly.\n",nameSolFile);
            solFile=NULL;
            return 0;
        }
        solFile=NULL;
        fprintf(stdout,"Closing file.\n");
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    char extension[3][14]={"input *.mesh","input *.wfn","output *.cube"};
    int length=0, lengthExtension[3]={7,6,7}, i=0, boolean=0, nX=0, nY=0, nZ=0;
    int nNucl=0, *pc=NULL, counter=0, ip1=0, ip2=0, ip3=0, ip4=0, ip5=0, ip6=0;
    int ip7=0, ip8=0;
    double deltaX=0., deltaY=0., deltaZ=0., xMin=0., yMin=0., zMin=0., *px=NULL;
    double *py=NULL, *pz=NULL, value=0.;
    FILE *cubeFile=NULL;
    Mesh mesh;

    // Initialize structure to zero
    initializeMeshStructure(&mesh);

    // Check the number of arguments
    if (argc!=4)
    {
        PRINT_ERROR("In main: the program only takes three arguments ");
        fprintf(stderr,"instead of %d.\n",argc-1);
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Check the length and extension of the input and output file names
    for (i=1; i<argc; i++)
    {
        length=checkStringFromLength(argv[i],lengthExtension[i-1],
                                                                 NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s file name should be a string ",extension[i-1]);
            fprintf(stderr,"of at most %d characters (and ",NAME_SIZE_MAX-1);
            fprintf(stderr,"strictly more than %d to ",lengthExtension[i-1]-2);
            fprintf(stderr,"contain at least something more than the ");
            fprintf(stderr,"%s extension).\n",extension[i-1]);
            return EXIT_FAILURE;
        }

        if (i==1)
        {
            boolean=(argv[i][length-6]==extension[i-1][7]);
            boolean=(boolean && argv[i][length-5]==extension[i-1][8]);
            boolean=(boolean && argv[i][length-4]==extension[i-1][9]);
            boolean=(boolean && argv[i][length-3]==extension[i-1][10]);
            boolean=(boolean && argv[i][length-2]==extension[i-1][11]);
            boolean=(boolean && argv[i][length-1]==extension[i-1][12]);
        }
        else if (i==2)
        {
            boolean=(argv[i][length-5]==extension[i-1][7]);
            boolean=(boolean && argv[i][length-4]==extension[i-1][8]);
            boolean=(boolean && argv[i][length-3]==extension[i-1][9]);
            boolean=(boolean && argv[i][length-2]==extension[i-1][10]);
            boolean=(boolean && argv[i][length-1]==extension[i-1][11]);
        }
        else
        {
            boolean=(argv[i][length-6]==extension[i-1][8]);
            boolean=(boolean && argv[i][length-5]==extension[i-1][9]);
            boolean=(boolean && argv[i][length-4]==extension[i-1][10]);
            boolean=(boolean && argv[i][length-3]==extension[i-1][11]);
            boolean=(boolean && argv[i][length-2]==extension[i-1][12]);
            boolean=(boolean && argv[i][length-1]==extension[i-1][13]);
        }

        if (!boolean)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s extension does not correspond ",extension[i-1]);
            fprintf(stderr,"to the one of the %s file name.\n",argv[i]);
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

    // Get the number of nuclei in the input *.wfn file
    nNucl=getNumberOfNucleiInWfnFile(argv[2]);
    if (nNucl<1)
    {
        PRINT_ERROR("In main: getNumberOfNucleiInWfnFile function returned ");
        fprintf(stderr,"zero instead of a positive value.\n");
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Allocate memory for nuclei
    px=(double*)calloc(nNucl,sizeof(double));
    py=(double*)calloc(nNucl,sizeof(double));
    pz=(double*)calloc(nNucl,sizeof(double));
    pc=(int*)calloc(nNucl,sizeof(int));
    if (px==NULL || py==NULL || pz==NULL || pc==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"variables associated with the nuclei.\n");
        free(px);
        free(py);
        free(pz);
        free(pc);
        px=NULL;
        py=NULL;
        pz=NULL;
        pc=NULL;
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Read the charge and positions of nuclei in the input *.wfn file
    if (!readNucleiFromWfnFile(argv[2],nNucl,pc,px,py,pz))
    {
        PRINT_ERROR("In main: readNucleiFromWfnFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(px);
        free(py);
        free(pz);
        free(pc);
        px=NULL;
        py=NULL;
        pz=NULL;
        pc=NULL;
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }

    // Check if there are non-zero labels on hexahedra
    counter=0;
    for (i=0; i<mesh.nhex; i++)
    {
        if (!mesh.phex[i].label)
        {
            counter++;
        }
    }

    // Load level-set function if necessary
    if (counter==mesh.nhex)
    {
        if (!readLsSolFile(argv[1],&mesh))
        {
            PRINT_ERROR("In main: readLsSolFile function returned zero ");
            fprintf(stderr,"instead of one.\n");
            free(px);
            free(py);
            free(pz);
            free(pc);
            px=NULL;
            py=NULL;
            pz=NULL;
            pc=NULL;
            freeMeshMemory(&mesh);
            return EXIT_FAILURE;
        }
    }

    // Writing the output *.cube file
    cubeFile=fopen(argv[3],"w+");
    if (cubeFile==NULL)
    {
        PRINT_ERROR("In main: we were not able to open and write into the ");
        fprintf(stderr,"%s file properly.\n",argv[3]);
        free(px);
        free(py);
        free(pz);
        free(pc);
        px=NULL;
        py=NULL;
        pz=NULL;
        pc=NULL;
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }
    fprintf(stdout,"Opening %s file. Writing cube data. ",argv[3]);

    fprintf(cubeFile,"Cube File\nNew grid\n    %d    %lf    %lf    %lf\n",
                            nNucl,xMin+.5*deltaX,yMin+.5*deltaY,zMin+.5*deltaZ);
    fprintf(cubeFile,"    %d    %lf    %lf    %lf\n",nX-1,deltaX,0.,0.);
    fprintf(cubeFile,"    %d    %lf    %lf    %lf\n",nY-1,0.,deltaY,0.);
    fprintf(cubeFile,"    %d    %lf    %lf    %lf\n",nZ-1,0.,0.,deltaZ);

    for (i=0; i<nNucl; i++)
    {
        fprintf(cubeFile,"    %d    %lf    %lf    %lf    %lf\n",
                                         pc[i],(double)pc[i],px[i],py[i],pz[i]);
    }

    if (counter==mesh.nhex)
    {
        for (i=0; i<mesh.nhex; i++)
        {
            ip1=mesh.phex[i].p1-1;
            ip2=mesh.phex[i].p2-1;
            ip3=mesh.phex[i].p3-1;
            ip4=mesh.phex[i].p4-1;
            ip5=mesh.phex[i].p5-1;
            ip6=mesh.phex[i].p6-1;
            ip7=mesh.phex[i].p7-1;
            ip8=mesh.phex[i].p8-1;

            value=mesh.pver[ip1].value;
            value+=mesh.pver[ip2].value;
            value+=mesh.pver[ip3].value;
            value+=mesh.pver[ip4].value;
            value+=mesh.pver[ip5].value;
            value+=mesh.pver[ip6].value;
            value+=mesh.pver[ip7].value;
            value+=mesh.pver[ip8].value;

            if (value<=0.)
            {
                fprintf(cubeFile,"    %lf",1.);
            }
            else
            {
                fprintf(cubeFile,"    %lf",0.);
            }

            if (i%6==5)
            {
                fprintf(cubeFile,"\n");
            }
        }
    }
    else
    {
        for (i=0; i<mesh.nhex; i++)
        {
            if (abs(mesh.phex[i].label)==LABEL_INTERIOR)
            {
                fprintf(cubeFile,"    %lf",1.);
            }
            else
            {
                fprintf(cubeFile,"    %lf",0.);
            }

            if (i%6==5)
            {
                fprintf(cubeFile,"\n");
            }
        }
    }

    if (fclose(cubeFile))
    {
        PRINT_ERROR("In main: we were not able to close the ");
        fprintf(stderr,"%s file properly.\n",argv[3]);
        free(px);
        free(py);
        free(pz);
        free(pc);
        px=NULL;
        py=NULL;
        pz=NULL;
        pc=NULL;
        cubeFile=NULL;
        freeMeshMemory(&mesh);
        return EXIT_FAILURE;
    }
    printf("Closing file.\n");
    cubeFile=NULL;

    // Free the memory dynamically allocated
    free(px);
    free(py);
    free(pz);
    free(pc);
    px=NULL;
    py=NULL;
    pz=NULL;
    pc=NULL;
    freeMeshMemory(&mesh);

    return EXIT_SUCCESS;
}

