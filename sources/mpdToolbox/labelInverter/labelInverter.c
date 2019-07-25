#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximal size allowed for the length of the file names
// (including the terminating nul character '\0')
#define NAME_SIZE_MAX 501

// Label of the internal tetrahedra/hexahedra belonging to the domain
#define LABEL_INTERNAL '3'

// Label of the external tetrahedra/hexahedra not belonging to the domain
#define LABEL_EXTERNAL '2'

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
int main(int argc, char* argv[])
{
    size_t stringLength=0;
    char extension[2][7]={"input","output"}, *readStringOut=NULL;
    char keyword[2*NAME_SIZE_MAX+10]={'\0'};
    int length=0, boolean=0, readChar=0, readIntegerOut=0, i=0, iMax=0, j=0;
    int jMax=0, readIntegerIn=0;
    FILE *meshFile=NULL;

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
            fprintf(stderr,"*.mesh extension does not correspond to the one ");
            fprintf(stderr,"of the %s file name.\n",argv[i]);
            return EXIT_FAILURE;
        }
    }

    // Build the system command if necessary: cp input.mesh output.mesh
    if (strcmp(argv[1],argv[2]))
    {
        strncpy(keyword,"cp ",2*NAME_SIZE_MAX+10);
        strcat(keyword,argv[1]);
        stringLength=strlen(keyword);
        keyword[stringLength]=' ';
        keyword[stringLength+1]='\0';
        strcat(keyword,argv[2]);

        fprintf(stdout,"%s\n",keyword);
        if (system(keyword))
        {
            PRINT_ERROR("In main: wrong return of the standard system ");
            fprintf(stderr,"c-function.\n");
            return EXIT_FAILURE;
        }
    }

    // Open the output *.mesh file: file must exists before in this reading mode
    fprintf(stdout,"Opening the %s file. ",argv[2]);
    meshFile=fopen(argv[2],"r+");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In main: we could not open and write into the ");
        fprintf(stderr,"%s file properly.\n",argv[2]);
        return EXIT_FAILURE;
    }
    fprintf(stdout,"Inverting label %c and %c. ",LABEL_EXTERNAL,LABEL_INTERNAL);

    // Skip all characters until the 'T' or 'H' of Tetrahedra/Hexahedra is found
    boolean=0;
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
                PRINT_ERROR("In main: wrong return of the fseek c-function ");
                fprintf(stderr,"while checking if the '%c' ",(char)readChar);
                fprintf(stderr,"encountered corresponds to the one of ");
                fprintf(stderr,"the Tetrahedra keyword.\n");
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }

            // Check if it is 'T' of Tetrahedra (and not RequiredTetrahedra)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=1;
            }

            // Reading again the 'T' of Tetrahedra
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In main: wrong end-of-file return ");
                fprintf(stderr,"(=%d) of the fgetc c-function while ",readChar);
                fprintf(stderr,"attempting to read the 'T' of the Tetrahedra ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }
            else if (readChar!='T')
            {
                PRINT_ERROR("In main: expecting 'T' instead of ");
                fprintf(stderr,"%c while attempting to read ",(char)readChar);
                fprintf(stderr,"to the first character of the Tetrahedra ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }

            if (boolean)
            {
                // Reading the letter after 'T' to distinguish
                // Tetrahedra from Triangles, Tangent, Time, ...
                readChar=fgetc(meshFile);
                if (readChar==EOF)
                {
                    PRINT_ERROR("In main: wrong end-of-file return ");
                    fprintf(stderr,"(=%d) of the fgetc c-function ",readChar);
                    fprintf(stderr,"while attempting to read the letter ");
                    fprintf(stderr,"after 'T' during the search of the ");
                    fprintf(stderr,"Tetrahedra keyword.\n");
                    closeTheFile(&meshFile);
                    return EXIT_FAILURE;
                }
                else if (readChar!='e')
                {
                    boolean=0;
                }

                if (fseek(meshFile,-1,SEEK_CUR))
                {
                    PRINT_ERROR("In main: wrong return of the fseek ");
                    fprintf(stderr,"c-function while checking if the ");
                    fprintf(stderr,"'%c' encountered ",(char)readChar);
                    fprintf(stderr,"corresponds to the one of the Tetrahedra ");
                    fprintf(stderr,"keyword.\n");
                    closeTheFile(&meshFile);
                    return EXIT_FAILURE;
                }
            }
        }
        else if (readChar=='H')
        {
            if (fseek(meshFile,-2,SEEK_CUR))
            {
                PRINT_ERROR("In main: wrong return of the fseek c-function ");
                fprintf(stderr,"while checking if the '%c' ",(char)readChar);
                fprintf(stderr,"encountered corresponds to the one of ");
                fprintf(stderr,"the Hexahedra keyword.\n");
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }

            // Check if it is 'H' of Hexahedra (and not RequiredHexahedra)
            readChar=fgetc(meshFile);
            if (readChar<33 || readChar>126)
            {
                boolean=2;
            }

            // Reading again the 'H' of Hexahedra
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In main: wrong end-of-file return ");
                fprintf(stderr,"(=%d) of the fgetc c-function while ",readChar);
                fprintf(stderr,"attempting to read the 'H' of the Hexahedra ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }
            else if (readChar!='H')
            {
                PRINT_ERROR("In main: expecting 'H' instead of ");
                fprintf(stderr,"%c while attempting to read ",(char)readChar);
                fprintf(stderr,"to the first character of the Hexahedra ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }

            if (boolean)
            {
                // Reading the letter after 'H' to be sure
                readChar=fgetc(meshFile);
                if (readChar==EOF)
                {
                    PRINT_ERROR("In main: wrong end-of-file return ");
                    fprintf(stderr,"(=%d) of the fgetc c-function ",readChar);
                    fprintf(stderr,"while attempting to read the letter ");
                    fprintf(stderr,"after 'H' during the search of the ");
                    fprintf(stderr,"Hexahedra keyword.\n");
                    closeTheFile(&meshFile);
                    return EXIT_FAILURE;
                }
                else if (readChar!='e')
                {
                    boolean=0;
                }

                if (fseek(meshFile,-1,SEEK_CUR))
                {
                    PRINT_ERROR("In main: wrong return of the fseek ");
                    fprintf(stderr,"c-function while checking if the ");
                    fprintf(stderr,"'%c' encountered ",(char)readChar);
                    fprintf(stderr,"corresponds to the one of the Hexahedra ");
                    fprintf(stderr,"keyword.\n");
                    closeTheFile(&meshFile);
                    return EXIT_FAILURE;
                }
            }
        }

    } while (!boolean && readChar!=EOF);

    if (readChar==EOF)
    {
        PRINT_ERROR("In main: wrong return ");
        fprintf(stderr,"(=%d) of fgetc c-function or end-of-file ",readChar);
        fprintf(stderr,"reached without any tetrahedra or hexahedra found.\n");
        closeTheFile(&meshFile);
        return EXIT_FAILURE;
    }

    // Read the number of tetrahedra or hexahedra
    if (boolean==1)
    {
        readStringOut=fgets(keyword,10,meshFile);
        readIntegerOut=fscanf(meshFile," %d ",&iMax);
        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In main: wrong return of the ");
            fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-functions in the attempt ",readIntegerOut);
            fprintf(stderr,"of reading the number of tetrahedra.\n");
            closeTheFile(&meshFile);
            return EXIT_FAILURE;
        }

        if (strcmp(keyword,"etrahedra") || iMax<1)
        {
            PRINT_ERROR("In main: expecting Tetrahedra Ntet instead of ");
            fprintf(stderr,"%c%s %d (positive ",(char)readChar,keyword,iMax);
            fprintf(stderr,"value).\n");
            closeTheFile(&meshFile);
            return EXIT_FAILURE;
        }
        jMax=4;
    }
    else
    {
        readStringOut=fgets(keyword,9,meshFile);
        readIntegerOut=fscanf(meshFile," %d ",&iMax);
        if (readStringOut==NULL || readIntegerOut!=1)
        {
            PRINT_ERROR("In main: wrong return of the ");
            fprintf(stderr,"fgets=%p or fscanf ",(void*)readStringOut);
            fprintf(stderr,"(=%d) c-functions in the attempt ",readIntegerOut);
            fprintf(stderr,"of reading the number of hexahedra.\n");
            closeTheFile(&meshFile);
            return EXIT_FAILURE;
        }

        if (strcmp(keyword,"exahedra") || iMax<1)
        {
            PRINT_ERROR("In main: expecting Hexahedra Nhex instead of ");
            fprintf(stderr,"%c%s %d (positive ",(char)readChar,keyword,iMax);
            fprintf(stderr,"value).\n");
            closeTheFile(&meshFile);
            return EXIT_FAILURE;
        }
        jMax=8;
    }

    for (i=0; i<iMax; i++)
    {
        // Read the vertex references of tetrahedra or hexahedra
        for (j=0; j<jMax; j++)
        {
            readIntegerOut=fscanf(meshFile,"%d ",&readIntegerIn);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In main: wrong return ");
                fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
                fprintf(stderr,"c-function in the attempt ");
                fprintf(stderr,"of reading %d-th vertex reference of the ",j+1);
                if (boolean==1)
                {
                    fprintf(stderr,"%d-th tetrahedra.\n",i+1);
                }
                else
                {
                    fprintf(stderr,"%d-th hexahedra.\n",i+1);
                }
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }
        }

        // Read then invert the label
        readChar=fgetc(meshFile);
        if (readChar==EOF)
        {
            PRINT_ERROR("In main: wrong end-of-file return ");
            fprintf(stderr,"(=%d) of the fgetc c-function ",readChar);
            fprintf(stderr,"while attempting to read the label of the ");
            if (boolean==1)
            {
                fprintf(stderr,"%d-th tetrahedra.\n",i+1);
            }
            else
            {
                fprintf(stderr,"%d-th hexahedra.\n",i+1);
            }
            closeTheFile(&meshFile);
            return EXIT_FAILURE;
        }
        else if (readChar=='-')
        {
            readChar=fgetc(meshFile);
            if (readChar==EOF)
            {
                PRINT_ERROR("In main: wrong end-of-file return ");
                fprintf(stderr,"(=%d) of the fgetc c-function ",readChar);
                fprintf(stderr,"while attempting to read the label of the ");
                if (boolean==1)
                {
                    fprintf(stderr,"%d-th tetrahedra.\n",i+1);
                }
                else
                {
                    fprintf(stderr,"%d-th hexahedra.\n",i+1);
                }
                closeTheFile(&meshFile);
                return EXIT_FAILURE;
            }
        }

        if (fseek(meshFile,-1,SEEK_CUR))
        {
            PRINT_ERROR("In main: wrong return of the fseek c-function while ");
            fprintf(stderr,"attempting to read the label of the ");
            if (boolean==1)
            {
                fprintf(stderr,"%d-th tetrahedra.\n",i+1);
            }
            else
            {
                fprintf(stderr,"%d-th hexahedra.\n",i+1);
            }
            closeTheFile(&meshFile);
            return EXIT_FAILURE;
        }

        if (readChar==LABEL_EXTERNAL)
        {
            fputc(LABEL_INTERNAL,meshFile);
        }
        else if (readChar==LABEL_INTERNAL)
        {
            fputc(LABEL_EXTERNAL,meshFile);
        }
    }

    // Close the output *.mesh file
    if (fclose(meshFile))
    {
        fprintf(stdout,"\nWarning in main function: we could not close the ");
        fprintf(stdout,"%s file properly.\n",argv[2]);
        meshFile=NULL;
        return EXIT_FAILURE;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    return EXIT_SUCCESS;
}

