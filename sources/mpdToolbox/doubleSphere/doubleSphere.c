#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <math.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

// Definition of the two level-set spheres
#define LS1_X  0.0
#define LS1_Y  0.0
#define LS1_Z  2.36215767
#define LS1_R  1.0

#define LS2_X  0.0
#define LS2_Y  0.0
#define LS2_Z -2.36215767
#define LS2_R  1.0

// Definition of the operation to perform on the 2level-set functions of spheres
// Values: 0=min(reunion) 1=max(intersection), 2=product(symmetric difference)
#define LS_OPERATION 0

#define DEF_MIN(a,b) (((a) < (b)) ? (a) : (b))

#define DEF_MAX(a,b) (((a) > (b)) ? (a) : (b))

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
int writingLsSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* pLs);

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
int writingLsSolFile(char nameFile[NAME_SIZE_MAX], int nVer, double* pLs)
{
    int i=0;
    FILE *solFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if pLs is set to NULL
    if (pLs==NULL)
    {
        PRINT_ERROR("In writingLsSolFile: the input (double*) variable ");
        fprintf(stderr,"pLs=%p does not point to a valid ",(void*)pLs);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check the number of vertices
    if (nVer<1)
    {
        PRINT_ERROR("In writingLsSolFile: the input variable nVer ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nVer);
        return 0;
    }

    // Open the *.sol file (warning: overwrite the file if it already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    solFile=fopen(nameFile,"w+");
    if (solFile==NULL)
    {
        PRINT_ERROR("In writingLsSolFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing data. ");

    // Save the coordinates stored in px
    fprintf(solFile,"\nMeshVersionFormatted 2\n\nDimension 3\n\n");
    fprintf(solFile,"SolAtVertices\n%d\n1 1\n\n",nVer);
    for (i=0; i<nVer; i++)
    {
        fprintf(solFile,"%.8le \n",pLs[i]);
    }
    fprintf(solFile,"\nEnd\n\n");

    // Close the *.sol file
    if (fclose(solFile))
    {
        fprintf(stdout,"\nWarning in writingLsSolFile function: we could not ");
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
    int i=0, nVer=0, length=0, extensionLength[2]={7,6}, boolean=0;
    double *px=NULL, *py=NULL, *pz=NULL, *pLs1=NULL, *pLs2=NULL, *pLs=NULL;
    double dx=0., dy=0., dz=0.;

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

    // Get the total number of vertices in the *.mesh input file
    nVer=getNumberOfVerticesInMeshFile(argv[1]);
    if (nVer<1)
    {
        PRINT_ERROR("In main: the total number of mesh vertices ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",nVer);
        return 0;
    }

    // Allocate memory for point coordinates references
    px=(double*)calloc(nVer,sizeof(double));
    py=(double*)calloc(nVer,sizeof(double));
    pz=(double*)calloc(nVer,sizeof(double));
    pLs1=(double*)calloc(nVer,sizeof(double));
    pLs2=(double*)calloc(nVer,sizeof(double));
    pLs=(double*)calloc(nVer,sizeof(double));
    if (px==NULL || py==NULL || pz==NULL || pLs1==NULL || pLs2==NULL ||
                                                                      pLs==NULL)
    {
        PRINT_ERROR("In main: we could not allocate memory for the local ");
        fprintf(stderr,"(double*) variables px=%p py=%p, ",(void*)px,(void*)py);
        fprintf(stderr,"pz=%p, pLs1=%p, ",(void*)pz,(void*)pLs1);
        fprintf(stderr,"pLs2=%p, and/or pLs=%p.\n",(void*)pLs2,(void*)pLs);
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
        free(pLs1);
        free(pLs2);
        free(pLs);
        px=NULL;
        py=NULL;
        pz=NULL;
        pLs1=NULL;
        pLs2=NULL;
        pLs=NULL;
        return EXIT_FAILURE;
    }

    // Compute the level-set of the two spheres
    for (i=0; i<nVer; i++)
    {
        dx=px[i]-LS1_X;
        dy=py[i]-LS1_Y;
        dz=pz[i]-LS1_Z;
        pLs1[i]=sqrt(dx*dx+dy*dy+dz*dz)-LS1_R;

        dx=px[i]-LS2_X;
        dy=py[i]-LS2_Y;
        dz=pz[i]-LS2_Z;
        pLs2[i]=sqrt(dx*dx+dy*dy+dz*dz)-LS2_R;

#if (!LS_OPERATION)
        pLs[i]=DEF_MIN(pLs1[i],pLs2[i]);
#elif (LS_OPERATION==1)
        pLs[i]=DEF_MAX(pLs1[i],pLs2[i]);
#else
        pLs[i]=pLs1[i]*pLs2[i];
#endif
    }

    // Print a warning if the two spheres intersects (the level-set function
    // is not normalized in the region of the intersection in this case)
    dx=LS2_X-LS1_X;
    dy=LS2_Y-LS1_Y;
    dz=LS2_Z-LS1_Z;

    if (sqrt(dx*dx+dy*dy+dz*dz)<=LS1_R+LS2_R)
    {
        if (sqrt(dx*dx+dy*dy+dz*dz)<=DEF_MAX(LS1_R,LS2_R)-DEF_MIN(LS1_R,LS2_R))
        {
            fprintf(stdout,"\nWarning in main function: one sphere is ");
            fprintf(stdout,"contained in the other sphere.\n");
        }
        else
        {
            fprintf(stdout,"\nWarning in main function: the two spheres ");
            fprintf(stdout,"chosen intersect each other.\n");
        }
    }
    else
    {
        fprintf(stdout,"\nWarning in main function: the two spheres ");
        fprintf(stdout,"chosen do not intersect each other.\n");
    }
    fprintf(stdout,"\nWe recall that the level-set function built from the ");
    fprintf(stdout,"one associated with the two spheres is not normalized.\n");

#if (!LS_OPERATION)
    fprintf(stdout,"\nTaking the reunion of two spheres via their level-set ");
    fprintf(stdout,"functions (min operator).\n");
#elif (LS_OPERATION==1)
    fprintf(stdout,"\nIntersecting the two spheres via their level-set ");
    fprintf(stdout,"functions (max operator).\n");
#else
    fprintf(stdout,"\nTaking the symmetric difference of the two spheres via ");
    fprintf(stdout,"their level-set functions (product operator).\n");
#endif
    fprintf(stdout,"p1=(%.8lf %.8lf %.8lf) r1=%.8lf\n",LS1_X,LS1_Y,LS1_Z,LS1_R);
    fprintf(stdout,"p2=(%.8lf %.8lf %.8lf) r2=%.8lf\n",LS2_X,LS2_Y,LS2_Z,LS2_R);

    // Writing the *.sol output file
    if (!writingLsSolFile(argv[2],nVer,pLs))
    {
        PRINT_ERROR("In main: writingSolFile function returned zero instead ");
        fprintf(stderr,"of one.\n");
        free(px);
        free(py);
        free(pz);
        free(pLs1);
        free(pLs2);
        free(pLs);
        px=NULL;
        py=NULL;
        pz=NULL;
        pLs1=NULL;
        pLs2=NULL;
        pLs=NULL;
        return EXIT_FAILURE;
    }

    // Free the memory dynamically allocated
    free(px);
    free(py);
    free(pz);
    free(pLs1);
    free(pLs2);
    free(pLs);
    px=NULL;
    py=NULL;
    pz=NULL;
    pLs1=NULL;
    pLs2=NULL;
    pLs=NULL;

    return EXIT_SUCCESS;
}

