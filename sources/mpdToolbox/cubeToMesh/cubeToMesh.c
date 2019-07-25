#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

// Label of the internal tetrahedra belonging to the domain
#define LABEL_INTERNAL_TETRA 3

// Label of the external tetrahedra not belonging to the domain
#define LABEL_EXTERNAL_TETRA 2

// Label of the boundary triangles belonging to the domain boundary
#define LABEL_BOUNDARY 10

#define PRINT_ERROR(stringToPrint)                                             \
do {                                                                           \
        fflush(stdout);                                                        \
        fprintf(stderr,"\nError encountered at line %d in ",__LINE__);         \
        fprintf(stderr,"%s file on %s",__FILE__,endTimerAtError());            \
        fprintf(stderr,"%s",stringToPrint);                                    \
} while (0)

char* endTimerAtError(void);
void closeTheFile(FILE** pFileToClose);
int checkStringFromLength(char* stringToCheck, int minimumLength,
                                                             int maximumLength);
int readingCubeFile(char nameFile[NAME_SIZE_MAX], int* nX, int* nY, int* nZ,
                    double* xMin, double* yMin, double* zMin, double* deltaX,
                                                double* deltaY, double* deltaZ);
int gettingCubeLabel(char nameFile[NAME_SIZE_MAX], int nDom, int nHex,
                     int* pDom, int nX, int nY, int nZ, double xMin,
                     double yMin, double zMin, double deltaX, double deltaY,
                                                                 double deltaZ);
int writingHexahedralMesh(char nameFile[NAME_SIZE_MAX], int nDom, int* pDom,
                          int nVer, int nHex, int nQua, int nX, int nY, int nZ,
                          double xMin, double yMin, double zMin, double deltaX,
                                                  double deltaY, double deltaZ);

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
int readingCubeFile(char nameFile[NAME_SIZE_MAX], int* nX, int* nY, int* nZ,
                    double* xMin, double* yMin, double* zMin, double* deltaX,
                                                 double* deltaY, double* deltaZ)
{
    int i=0, iMax=0, nNucl=0, readIntegerOut=0, readIntegerIn=0, readChar=0;
    int nVer=0, boolean=0;
    double readDouble1=0., readDouble2=0., readDouble3=0.;
    FILE *cubeFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check if the input pointers are set to NULL
    if (nX==NULL || nY==NULL || nZ==NULL)
    {
        PRINT_ERROR("In readingCubeFile: at least one of the input ");
        fprintf(stderr,"(int*) variables nX=%p, nY=%p ",(void*)nX,(void*)nY);
        fprintf(stderr,"or nZ=%p does not point to a valid ",(void*)nZ);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    if (xMin==NULL || yMin==NULL || zMin==NULL)
    {
        PRINT_ERROR("In readingCubeFile: at least one of the input ");
        fprintf(stderr,"(double*) variables xMin=%p, ",(void*)xMin);
        fprintf(stderr,"yMin=%p or zMin=%p does not ",(void*)yMin,(void*)zMin);
        fprintf(stderr,"point to a valid adress.\n");
        return 0;
    }

    if (deltaX==NULL || deltaY==NULL || deltaZ==NULL)
    {
        PRINT_ERROR("In readingCubeFile: at least one of the input ");
        fprintf(stderr,"(double*) variables deltaX=%p, ",(void*)deltaX);
        fprintf(stderr,"deltaY=%p or ",(void*)deltaY);
        fprintf(stderr,"deltaZ=%p does not point to a valid ",(void*)deltaZ);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    // Open *.cube file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    cubeFile=fopen(nameFile,"r");
    if (cubeFile==NULL)
    {
        PRINT_ERROR("In readingCubeFile: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading grid data. ");

    // Skip the first two lines (containing Cube File and New grid generally)
    do
    {
        readChar=fgetc(cubeFile);

    } while (readChar!='\n' && readChar!=EOF);

    do
    {
        readChar=fgetc(cubeFile);

    } while (readChar!='\n' && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In readingCubeFile: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any discretization found.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of nuclei and minimal coordinates of the grid
    readIntegerOut=fscanf(cubeFile," %d %lf %lf %lf ",&nNucl,xMin,yMin,zMin);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In readingCubeFile: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of nuclei and minimal grid coordinates.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (nNucl<1)
    {
        PRINT_ERROR("In readingCubeFile: the total number of nuclei ");
        fprintf(stderr,"(=%d) must be a positive integer.\n",nNucl);
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of points and discretization step in the x-direction
    readIntegerOut=fscanf(cubeFile," %d %lf %lf %lf ",nX,deltaX,&readDouble1,
                                                                  &readDouble2);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In readingCubeFile: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of points and discretization step in the ");
        fprintf(stderr,"first coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (*nX<2 || *deltaX<=0. || readDouble1!=0. || readDouble2!=0.)
    {
        PRINT_ERROR("In readingCubeFile: the number of points in the first ");
        fprintf(stderr,"coordinate direction (=%d) must be an integer ",*nX);
        fprintf(stderr,"(strictly) greater than 1, the associated ");
        fprintf(stderr,"discretization step (=%lf) positive, and ",*deltaX);
        fprintf(stderr,"the other values read ");
        fprintf(stderr,"(=%lf, =%lf) must be set set ",readDouble1,readDouble2);
        fprintf(stderr,"to zero.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of points and discretization step in the y-direction
    readIntegerOut=fscanf(cubeFile,"%d %lf %lf %lf ",nY,&readDouble1,deltaY,
                                                                  &readDouble2);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In readingCubeFile: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of points and discretization step in the ");
        fprintf(stderr,"second coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (*nY<2 || *deltaY<=0. || readDouble1!=0. || readDouble2!=0.)
    {
        PRINT_ERROR("In readingCubeFile: the number of points in the second ");
        fprintf(stderr,"coordinate direction (=%d) must be an integer ",*nY);
        fprintf(stderr,"(strictly) greater than 1, the associated ");
        fprintf(stderr,"discretization step (=%lf) positive, and ",*deltaY);
        fprintf(stderr,"the other values read ");
        fprintf(stderr,"(=%lf, =%lf) must be set set ",readDouble1,readDouble2);
        fprintf(stderr,"to zero.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of points and discretization step in the z-direction
    readIntegerOut=fscanf(cubeFile,"%d %lf %lf %lf ",nZ,&readDouble1,
                                                           &readDouble2,deltaZ);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In readingCubeFile: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of points and discretization step in the ");
        fprintf(stderr,"third coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (*nZ<2 || *deltaZ<=0. || readDouble1!=0. || readDouble2!=0.)
    {
        PRINT_ERROR("In readingCubeFile: the number of points in the third ");
        fprintf(stderr,"coordinate direction (=%d) must be an integer ",*nZ);
        fprintf(stderr,"(strictly) greater than 1, the associated ");
        fprintf(stderr,"discretization step (=%lf) positive, and ",*deltaZ);
        fprintf(stderr,"the other values read ");
        fprintf(stderr,"(=%lf, =%lf) must be set set ",readDouble1,readDouble2);
        fprintf(stderr,"to zero.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the Nucleus data
    for (i=0; i<nNucl; i++)
    {
        // Read the charge of the (i+1)-th nucleus
        readIntegerOut=fscanf(cubeFile,"%d %lf ",&readIntegerIn,&readDouble1);
        if (readIntegerOut!=2)
        {
            PRINT_ERROR("In readingCubeFile: wrong return ");
            fprintf(stderr,"(=%d instead of 2) of the ",readIntegerOut);
            fprintf(stderr,"standard fscanf c-function while attempting to ");
            fprintf(stderr,"read the charge of the %d-th nucleus.\n",i+1);
            closeTheFile(&cubeFile);
            return 0;
        }

        // Read the coordinates of the (i+1)-th nucleus
        readIntegerOut=fscanf(cubeFile,"%lf %lf %lf ",&readDouble1,&readDouble2,
                                                                  &readDouble3);
        if (readIntegerOut!=3)
        {
            PRINT_ERROR("In readingCubeFile: wrong return ");
            fprintf(stderr,"(=%d instead of 3) of the ",readIntegerOut);
            fprintf(stderr,"standard fscanf c-function while attempting to ");
            fprintf(stderr,"read the coordinates of the %d-th nucleus.\n",i+1);
            closeTheFile(&cubeFile);
            return 0;
        }
    }

    // Check if there is a list of labels associated with the grid points
    readChar=fgetc(cubeFile);
    if (readChar!=EOF)
    {
        if (fseek(cubeFile,-1,SEEK_CUR))
        {
            PRINT_ERROR("In readingCubeFile: wrong return of the standard ");
            fprintf(stderr,"fseek c-function while checking if the ");
            fprintf(stderr,"%s file contains a list of labels ",nameFile);
            fprintf(stderr,"associated with the grid point.\n");
            closeTheFile(&cubeFile);
            return 0;
        }

        // Reading the labels successively
        nVer=0;
        boolean=0;
        iMax=(*nX)*(*nY)*(*nZ);
        for (i=0; i<iMax; i++)
        {
            readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble1);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In readingCubeFile: wrong return ");
                fprintf(stderr,"(=%d instead of 1) of the ",readIntegerOut);
                fprintf(stderr,"standard fscanf c-function while attempting ");
                fprintf(stderr,"to read the label (=%lf) of the ",readDouble1);
                fprintf(stderr,"%d-th grid point.\n",i+1);
                closeTheFile(&cubeFile);
                return 0;
            }
            if (readDouble1==1.)
            {
                nVer++;
            }
            else if (readDouble1!=0. && !boolean)
            {
                fprintf(stdout,"\nWarning in the readingCubeFile function: ");
                fprintf(stdout,"the labels associated with the grid points ");
                fprintf(stdout,"should be set to zero or one (the %d-th ",i+1);
                fprintf(stdout,"is set to %lf).\n",readDouble1);
            }
        }
        nVer++;
    }
    else
    {
        nVer=1;
    }

    // Close the input *.cube file
    if (fclose(cubeFile))
    {
        fprintf(stdout,"\nWarning in readingCubeFile function: we could not ");
        fprintf(stdout,"close the %s file properly.\n",nameFile);
        cubeFile=NULL;
        return 0;
    }
    cubeFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return nVer;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int gettingCubeLabel(char nameFile[NAME_SIZE_MAX], int nDom, int nHex,
                     int* pDom, int nX, int nY, int nZ, double xMin,
                     double yMin, double zMin, double deltaX, double deltaY,
                                                                  double deltaZ)
{
    int i=0, nNucl=0, readIntegerOut=0, readIntegerIn=0, readChar=0;
    int counter=0;
    double readDouble1=0., readDouble2=0., readDouble3=0.;
    FILE *cubeFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check the input variables
    if (nHex<1 || nDom<1 || pDom==NULL)
    {
        PRINT_ERROR("In gettingCubeLabel: the total number of hexahedra ");
        fprintf(stderr,"(=%d) or internal ones (=%d) corresponding ",nHex,nDom);
        fprintf(stderr,"to the domain should be positive integer, and ");
        fprintf(stderr,"the associated pointer pDom=%p should ",(void*)pDom);
        fprintf(stderr,"point to a valid adress.\n");
        return 0;
    }
    if (nDom>nHex)
    {
        PRINT_ERROR("In gettingCubeLabel: the total number of hexahedra ");
        fprintf(stderr,"corresponding to the internal domain (=%d) ",nDom);
        fprintf(stderr,"cannot be (strictly) greater than the total number");
        fprintf(stderr," of hexahedra in the mesh (=%d).\n",nHex);
        return 0;
    }

    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In gettingCubeLabel: the input variables ");
        fprintf(stderr,"nX (=%d), nY (=%d), and nZ (=%d) should be ",nX,nY,nZ);
        fprintf(stderr,"integers strictly greater than two.\n");
        return 0;
    }
    if (nHex!=(nX-1)*(nY-1)*(nZ-1))
    {
        PRINT_ERROR("In gettingCubeLabel: expecting ");
        fprintf(stderr,"%d instead of %d ",nHex,(nX-1)*(nY-1)*(nZ-1));
        fprintf(stderr,"for the total number of hexahedra in the grid ");
        fprintf(stderr,"(%d)x(%d)x(%d).\n",nX-1,nY-1,nZ-1);
        return 0;
    }

    if (deltaX<=0. || deltaY<=0. || deltaZ<=0.)
    {
        PRINT_ERROR("In gettingCubeLabel: the input variables ");
        fprintf(stderr,"deltaX (=%lf), deltaY (=%lf), or ",deltaX,deltaY);
        fprintf(stderr,"deltaZ (=%lf) should be positive values.\n",deltaZ);
        return 0;
    }

    // Open *.cube file: it must have been previously created in reading mode
    fprintf(stdout,"Opening the %s file. ",nameFile);
    cubeFile=fopen(nameFile,"r");
    if (cubeFile==NULL)
    {
        PRINT_ERROR("In gettingCubeLabel: we could not open the ");
        fprintf(stderr,"%s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Reading grid labels. ");

    // Skip the first two lines (containing Cube File and New grid generally)
    do
    {
        readChar=fgetc(cubeFile);

    } while (readChar!='\n' && readChar!=EOF);

    do
    {
        readChar=fgetc(cubeFile);

    } while (readChar!='\n' && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In gettingCubeLabel: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any discretization found.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of nuclei and minimal coordinates of the grid
    readIntegerOut=fscanf(cubeFile," %d %lf %lf %lf ",&nNucl,&readDouble1,
                                                     &readDouble2,&readDouble3);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In gettingCubeLabel: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of nuclei and minimal grid coordinates.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (nNucl<1)
    {
        PRINT_ERROR("In gettingCubeLabel: the total number of nuclei ");
        fprintf(stderr,"(=%d) must be a positive integer.\n",nNucl);
        closeTheFile(&cubeFile);
        return 0;
    }
    if (readDouble1!=xMin+.5*deltaX || readDouble2!=yMin+.5*deltaY ||
                                                    readDouble3!=zMin+.5*deltaZ)
    {
        PRINT_ERROR("In gettingCubeLabel: expecting\n");
        fprintf(stderr,"xMin (=%lf) == %lf\n",readDouble1,xMin+.5*deltaX);
        fprintf(stderr,"yMin (=%lf) == %lf\n",readDouble2,yMin+.5*deltaY);
        fprintf(stderr,"zMin (=%lf) == %lf\n",readDouble3,zMin+.5*deltaZ);
        fprintf(stderr,"for the minimal grid coordinates.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of points and discretization step in the x-direction
    readIntegerOut=fscanf(cubeFile," %d %lf %lf %lf ",&readIntegerIn,
                                                      &readDouble1,&readDouble2,
                                                                  &readDouble3);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In gettingCubeLabel: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of points and discretization step in the ");
        fprintf(stderr,"first coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (readIntegerIn!=nX-1 || readDouble1!=deltaX || readDouble2!=0. ||
                                                                readDouble3!=0.)
    {
        PRINT_ERROR("In gettingCubeLabel: expecting\n");
        fprintf(stderr,"nX (=%d) == %d\n",readIntegerIn,nX);
        fprintf(stderr,"deltaX (=%lf) == %lf\n",readDouble1,deltaX);
        fprintf(stderr,"readDouble2 (=%lf) == 0.0\n",readDouble2);
        fprintf(stderr,"readDouble3 (=%lf) == 0.0\n",readDouble3);
        fprintf(stderr,"for the number of points and discretization step in ");
        fprintf(stderr,"the first coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of points and discretization step in the y-direction
    readIntegerOut=fscanf(cubeFile," %d %lf %lf %lf ",&readIntegerIn,
                                                      &readDouble1,&readDouble2,
                                                                  &readDouble3);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In gettingCubeLabel: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of points and discretization step in the ");
        fprintf(stderr,"second coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (readIntegerIn!=nY-1 || readDouble1!=0. || readDouble2!=deltaY ||
                                                                readDouble3!=0.)
    {
        PRINT_ERROR("In gettingCubeLabel: expecting\n");
        fprintf(stderr,"nY (=%d) == %d\n",readIntegerIn,nY);
        fprintf(stderr,"readDouble1 (=%lf) == 0.0\n",readDouble1);
        fprintf(stderr,"deltaY (=%lf) == %lf\n",readDouble2,deltaY);
        fprintf(stderr,"readDouble3 (=%lf) == 0.0\n",readDouble3);
        fprintf(stderr,"for the number of points and discretization step in ");
        fprintf(stderr,"the second coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of points and discretization step in the y-direction
    readIntegerOut=fscanf(cubeFile," %d %lf %lf %lf ",&readIntegerIn,
                                                      &readDouble1,&readDouble2,
                                                                  &readDouble3);
    if (readIntegerOut!=4)
    {
        PRINT_ERROR("In gettingCubeLabel: wrong return ");
        fprintf(stderr,"(=%d instead of 4) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function while attempting to read the ");
        fprintf(stderr,"number of points and discretization step in the ");
        fprintf(stderr,"third coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    if (readIntegerIn!=nZ-1 || readDouble1!=0. || readDouble2!=0. ||
                                                            readDouble3!=deltaZ)
    {
        PRINT_ERROR("In gettingCubeLabel: expecting\n");
        fprintf(stderr,"nZ (=%d) == %d\n",readIntegerIn,nZ);
        fprintf(stderr,"readDouble1 (=%lf) == 0.0\n",readDouble1);
        fprintf(stderr,"readDouble2 (=%lf) == 0.0\n",readDouble2);
        fprintf(stderr,"deltaZ (=%lf) == %lf\n",readDouble3,deltaZ);
        fprintf(stderr,"for the number of points and discretization step in ");
        fprintf(stderr,"the third coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the Nucleus data
    for (i=0; i<nNucl; i++)
    {
        // Read the charge of the (i+1)-th nucleus
        readIntegerOut=fscanf(cubeFile,"%d %lf ",&readIntegerIn,&readDouble1);
        if (readIntegerOut!=2)
        {
            PRINT_ERROR("In gettingCubeLabel: wrong return ");
            fprintf(stderr,"(=%d instead of 2) of the ",readIntegerOut);
            fprintf(stderr,"standard fscanf c-function while attempting to ");
            fprintf(stderr,"read the charge of the %d-th nucleus.\n",i+1);
            closeTheFile(&cubeFile);
            return 0;
        }

        // Read the coordinates of the (i+1)-th nucleus
        readIntegerOut=fscanf(cubeFile,"%lf %lf %lf ",&readDouble1,&readDouble2,
                                                                  &readDouble3);
        if (readIntegerOut!=3)
        {
            PRINT_ERROR("In gettingCubeLabel: wrong return ");
            fprintf(stderr,"(=%d instead of 3) of the ",readIntegerOut);
            fprintf(stderr,"standard fscanf c-function while attempting to ");
            fprintf(stderr,"read the coordinates of the %d-th nucleus.\n",i+1);
            closeTheFile(&cubeFile);
            return 0;
        }
    }

    // Reading the labels successively
    counter=0;
    for (i=0; i<nHex; i++)
    {
        readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble1);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In gettingCubeLabel: wrong return ");
            fprintf(stderr,"(=%d instead of 1) of the ",readIntegerOut);
            fprintf(stderr,"standard fscanf c-function while attempting ");
            fprintf(stderr,"to read the label (=%lf) of the ",readDouble1);
            fprintf(stderr,"%d-th grid point.\n",i+1);
            closeTheFile(&cubeFile);
            return 0;
        }
        pDom[i]=(int)readDouble1;
    if (readDouble1-pDom[i]>.5)
    {
            pDom[i]++;
    }

        if (readDouble1==1.)
        {
            counter++;
        }
    }

    if (counter!=nDom)
    {
        PRINT_ERROR("In gettingCubeLabel: expecting ");
        fprintf(stderr,"%d instead of %d for the number of ",nDom,counter);
        fprintf(stderr,"mesh elements labelled one (inside the internal ");
        fprintf(stderr,"domain).\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Close the input *.cube file
    if (fclose(cubeFile))
    {
        fprintf(stdout,"\nWarning in gettingCubeLabel function: we could not ");
        fprintf(stdout,"close the %s file properly.\n",nameFile);
        cubeFile=NULL;
        return 0;
    }
    cubeFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return 1;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int writingHexahedralMesh(char nameFile[NAME_SIZE_MAX], int nDom, int* pDom,
                          int nVer, int nHex, int nQua, int nX, int nY, int nZ,
                          double xMin, double yMin, double zMin, double deltaX,
                                                   double deltaY, double deltaZ)
{
    int i=0, j=0, k=0, l=0, lPoint=0, counter=0;
    FILE* meshFile=NULL;

    // Add the terminating NUL characters '\0' to ensure nameFile is a string
    nameFile[NAME_SIZE_MAX-1]='\0';

    // Check the input variables
    if (nVer<1 || nHex<1 || nQua<1 || nDom<0)
    {
        PRINT_ERROR("In writingHexahedralMesh: the total number of vertices ");
        fprintf(stderr,"(=%d), hexahedra (=%d), quadrilaterals ",nVer,nHex);
        if (!nDom)
        {
            fprintf(stderr,"(=%d), and hexahedra (=%d) belonging ",nQua,nDom);
            fprintf(stderr,"to the internal domain should be positive ");
            fprintf(stderr,"integers.\n");
        }
        else
        {
            fprintf(stderr,"(=%d) should be positive integers.\n",nQua);

        }
        return 0;
    }

    if (pDom==NULL)
    {
        PRINT_ERROR("In writingHexahedralMesh: the input (int*) variable ");
        fprintf(stderr,"pDom=%p does not point to a valid ",(void*)pDom);
        fprintf(stderr,"adress.\n");
        return 0;
    }

    if (nDom>nHex)
    {
        PRINT_ERROR("In writingHexahedralMesh: the total number of hexahedra ");
        fprintf(stderr,"corresponding to the internal domain (=%d) ",nDom);
        fprintf(stderr,"cannot be (strictly) greater than the total number");
        fprintf(stderr," of hexahedra in the mesh (=%d).\n",nHex);
        return 0;
    }

    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In writingHexahedralMesh: the input variables ");
        fprintf(stderr,"nX (=%d), nY (=%d), and nZ (=%d) should be ",nX,nY,nZ);
        fprintf(stderr,"integers strictly greater than two.\n");
        return 0;
    }
    if (nVer!=nX*nY*nZ || nHex!=(nX-1)*(nY-1)*(nZ-1) ||
                            nQua!=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1)))
    {
        PRINT_ERROR("In writingHexahedralMesh: expecting ");
        fprintf(stderr,"%d instead of %d for the total number ",nX*nY*nZ,nVer);
        fprintf(stderr,"of vertices, %d instead of ",(nX-1)*(nY-1)*(nZ-1));
        fprintf(stderr,"%d for the total number of hexahedra, and ",nHex);
        fprintf(stderr,"%d ",2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1)));
        fprintf(stderr,"instead of %d for the total number of ",nQua);
        fprintf(stderr,"quadrilaterals.\n");
        return 0;
    }

    if (deltaX<=0. || deltaY<=0. || deltaZ<=0.)
    {
        PRINT_ERROR("In writingHexahedralMesh: the input variables ");
        fprintf(stderr,"deltaX (=%lf), deltaY (=%lf), or ",deltaX,deltaY);
        fprintf(stderr,"deltaZ (=%lf) should be positive values.\n",deltaZ);
        return 0;
    }

    // Opening *.mesh file (warning: reset and overwrite file if already exists)
    fprintf(stdout,"Opening the %s file. ",nameFile);
    meshFile=fopen(nameFile,"w+");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In writingHexahedralMesh: we were not able to open and ");
        fprintf(stderr,"write into the %s file properly.\n",nameFile);
        return 0;
    }
    fprintf(stdout,"Writing hexahedral mesh. ");

    // Writing vertices
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

    // Writing hexahedra
    fprintf(meshFile,"\nHexahedra\n%d\n",nHex);
    counter=0;
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
                if (nDom)
                {
                    if (pDom[counter]==1)
                    {
                        fprintf(meshFile,"%d \n",LABEL_INTERNAL_TETRA);
                    }
                    else
                    {
                        fprintf(meshFile,"%d \n",LABEL_EXTERNAL_TETRA);
                    }
                }
                else
                {
                    fprintf(meshFile,"%d \n",0);
                }
                counter++;
            }
        }
    }

    if (nDom)
    {
        // Counting the number of boundary quadrilaterals for the internal cube
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

                // Test not to count two times the quadrilaterals
                if (pDom[l]==0 && pDom[lPoint]==1)
                {
                    counter++;
                }
            }
            if (i<nX-2)
            {
                lPoint=((i+1)*(nY-1)+j)*(nZ-1)+k;

                // Test not to count two times the quadrilaterals
                if (pDom[l]==0 && pDom[lPoint]==1)
                {
                    counter++;
                }
            }
            if (j>0)
            {
                lPoint=(i*(nY-1)+(j-1))*(nZ-1)+k;

                // Test not to count two times the quadrilaterals
                if (pDom[l]==0 && pDom[lPoint]==1)
                {
                    counter++;
                }
            }
            if (j<nY-2)
            {
                lPoint=(i*(nY-1)+(j+1))*(nZ-1)+k;

                // Test not to count two times the quadrilaterals
                if (pDom[l]==0 && pDom[lPoint]==1)
                {
                    counter++;
                }
            }
            if (k>0)
            {
                lPoint=(i*(nY-1)+j)*(nZ-1)+(k-1);

                // Test not to count two times the quadrilaterals
                if (pDom[l]==0 && pDom[lPoint]==1)
                {
                    counter++;
                }
            }
            if (k<nZ-2)
            {
                 lPoint=(i*(nY-1)+j)*(nZ-1)+(k+1);

                // Test not to count two times the quadrilaterals
                if (pDom[l]==0 && pDom[lPoint]==1)
                {
                    counter++;
                }
            }
        }
        fprintf(stdout,"\nAdding %d quadrilaterals for the ",counter);
        fprintf(stdout,"internal domains (total is now %d). ",counter+nQua);
    }
    else
    {
        counter=0;
    }

    // Writing quadrilaterals
    fprintf(meshFile,"\nQuadrilaterals\n%d\n",nQua+counter);

    // Face 1 (j=0) with reference (a,b,c) in each quadrilateral
    // corresponding to square cell: 1=(i-1,k-1) 2=(i,k-1) 3=(i,k) 4=(i-1,k)
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

    if (nDom)
    {
        // Saving boundary quadrilaterals for the internal cube
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
                if (pDom[l]==1 && pDom[lPoint]==0)
                {
                    fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+k);
                    fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+k);
                    fprintf(meshFile,"%d \n",LABEL_BOUNDARY);
                }
            }
            if (i<nX-2)
            {
                lPoint=((i+1)*(nY-1)+j)*(nZ-1)+k;

                // Test not to count two times the quadrilaterals
                if (pDom[l]==1 && pDom[lPoint]==0)
                {
                    fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+k);
                    fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+k);
                    fprintf(meshFile,"%d \n",LABEL_BOUNDARY);
                }
            }
            if (j>0)
            {
                lPoint=(i*(nY-1)+(j-1))*(nZ-1)+k;

                // Test not to count two times the quadrilaterals
                if (pDom[l]==1 && pDom[lPoint]==0)
                {
                    fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+k);
                    fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+k);
                    fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(k+1));
                    fprintf(meshFile,"%d \n",LABEL_BOUNDARY);
                }
            }
            if (j<nY-2)
            {
                 lPoint=(i*(nY-1)+(j+1))*(nZ-1)+k;

                // Test not to count two times the quadrilaterals
                if (pDom[l]==1 && pDom[lPoint]==0)
                {
                    fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+k);
                    fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+k);
                    fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+(k+1));
                    fprintf(meshFile,"%d \n",LABEL_BOUNDARY);
                }
            }
            if (k>0)
            {
                lPoint=(i*(nY-1)+j)*(nZ-1)+(k-1);

                // Test not to count two times the quadrilaterals
                if (pDom[l]==1 && pDom[lPoint]==0)
                {
                    fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+k);
                    fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+k);
                    fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+k);
                    fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+k);
                    fprintf(meshFile,"%d \n",LABEL_BOUNDARY);
                }
            }
            if (k<nZ-2)
            {
                lPoint=(i*(nY-1)+j)*(nZ-1)+(k+1);

                // Test not to count two times the quadrilaterals
                if (pDom[l]==1 && pDom[lPoint]==0)
                {
                    fprintf(meshFile,"%d ",1+(i*nY+j)*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+((i+1)*nY+j)*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+((i+1)*nY+(j+1))*nZ+(k+1));
                    fprintf(meshFile,"%d ",1+(i*nY+(j+1))*nZ+(k+1));
                    fprintf(meshFile,"%d \n",LABEL_BOUNDARY);
                }
            }
        }
    }
    fprintf(meshFile,"\nEnd\n\n");

    // Closing the output *.mesh file
    if (fclose(meshFile))
    {
        PRINT_ERROR("In writingHexahedralMesh: the ");
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
    char extension[2][14]={"input *.cube","output *.mesh"};
    int length=0, i=0, nX=0, nY=0, nZ=0, nVer=0, nHex=0, nQua=0, nDom=0;
    int *pDom=NULL, boolean=0;
    double deltaX=0., deltaY=0., deltaZ=0., xMin=0., yMin=0., zMin=0., xMax=0.;
    double yMax=0., zMax=0.;

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
        length=checkStringFromLength(argv[i],7,NAME_SIZE_MAX);
        if (!length)
        {
            PRINT_ERROR("In main: the ");
            fprintf(stderr,"%s file name should be a string ",extension[i-1]);
            fprintf(stderr,"of at most %d characters (and ",NAME_SIZE_MAX-1);
            fprintf(stderr,"strictly more than 5 to contain at least ");
            fprintf(stderr,"something more than the ");
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

    nDom=readingCubeFile(argv[1],&nX,&nY,&nZ,&xMin,&yMin,&zMin,&deltaX,&deltaY,
                                                                       &deltaZ);
    if (nDom<1)
    {
        PRINT_ERROR("In main: readingCubeFile returned zero instead of a ");
        fprintf(stderr,"positive integer value.\n");
        return EXIT_FAILURE;
    }
    nDom--;

    // Converting cube data into mesh data
    nX++;
    nY++;
    nZ++;

    xMin-=.5*deltaX;
    yMin-=.5*deltaY;
    zMin-=.5*deltaZ;

    xMax=xMin+(nX-1)*deltaX;
    yMax=yMin+(nY-1)*deltaY;
    zMax=zMin+(nZ-1)*deltaZ;

    nVer=nX*nY*nZ;
    nHex=(nX-1)*(nY-1)*(nZ-1);
    nQua=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));

    // Check the values obtained in case they cannot be stored in an int
    if (nVer<1 || nHex<1 || nQua<1)
    {
        PRINT_ERROR("In main: the total number of mesh vertices ");
        fprintf(stderr,"(=%d), hexahedra (=%d) and ",nVer,nHex);
        fprintf(stderr,"quadrilaterals (=%d) should be positive ",nQua);
        fprintf(stderr,"integers.\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout,"\nCreating the computational box [%lf,%lf]x",xMin,xMax);
    fprintf(stdout,"[%lf,%lf]x[%lf,%lf]\nwith ",yMin,yMax,zMin,zMax);
    fprintf(stdout,"(%d)x(%d)x(%d) points and ",nX,nY,nZ);
    fprintf(stdout,"(%lf)x(%lf)x(%lf) discretization ",deltaX,deltaY,deltaZ);
    fprintf(stdout,"steps.\nMesh data: %d vertices, %d hexahedra ",nVer,nHex);
    fprintf(stdout,"and %d boundary quadrilaterals\n",nQua);
    if (nDom)
    {
        fprintf(stdout,"(internal domain detected on %d hexahedra).\n\n",nDom);
    }
    else
    {
        fprintf(stdout,"(no internal domain detected).\n\n");
    }

    // Allocate memory and set to zero the labels
    pDom=(int*)calloc(nHex,sizeof(int));
    if (pDom==NULL)
    {
        PRINT_ERROR("In main: could not allocate memory for the local (int*) ");
        fprintf(stderr,"pDom=%p variable.\n",(void*)pDom);
        return EXIT_FAILURE;
    }

    // Update the labels if necessary
    if (nDom)
    {
        if (!gettingCubeLabel(argv[1],nDom,nHex,pDom,nX,nY,nZ,xMin,yMin,zMin,
                                                          deltaX,deltaY,deltaZ))
        {
            PRINT_ERROR("In main: gettingCubeLabel function returned zero ");
            fprintf(stderr,"instead of one.\n");
            free(pDom);
            pDom=NULL;
            return EXIT_FAILURE;
        }
    }

    if (!writingHexahedralMesh(argv[2],nDom,pDom,nVer,nHex,nQua,nX,nY,nZ,xMin,
                                                yMin,zMin,deltaX,deltaY,deltaZ))
    {
        PRINT_ERROR("In main: writingHexahedralMesh function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(pDom);
        pDom=NULL;
        return EXIT_FAILURE;
    }

    // Free the allocated memory
    free(pDom);
    pDom=NULL;

    return EXIT_SUCCESS;
}

