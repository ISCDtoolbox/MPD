#ifndef DEF_LOAD_CHEMISTRY
#define DEF_LOAD_CHEMISTRY

/**
* \file loadChemistry.h
* \brief Prototypes related to the initial loading of the chemical informations
*        in the MPD algorithm.
* \author Jeremy DALPHIN
* \version 3.0
* \date August 1st, 2019
*
* This file contains the description of all the non-static function prototypes
* that are used to initially load the chemical data from a *.wfn/ *.chem file in
* the MPD algorithm.
*/

#include "main.h"

////////////////////////////////////////////////////////////////////////////////
// Prototypes of non-static functions
////////////////////////////////////////////////////////////////////////////////
/**
* \fn void initializeChemicalStructure(ChemicalSystem* pChemicalSystem)
* \brief It initializes to zero the ChemicalSystem structure pointed by
*        pChemicalSystem.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure (defined in main.h) of the \ref main
*                            function.
*
* The function \ref initializeChemicalStructure sets to zero all the variables
* of the structure pointed by pChemicalSystem (and pointers are set to NULL).
* This function does not return any value (void output).
*/
void initializeChemicalStructure(ChemicalSystem* pChemicalSystem);

/**
* \fn void freeChemicalMemory(ChemicalSystem* pChemicalSystem)
* \brief It frees the memory dynamically allocated for the ChemicalSystem
*        structure pointed by pChemicalSystem.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                            structure (defined in main.h file) of the \ref main
*                            function.
*
* The function \ref freeChemicalMemory frees the memory dynamically allocated
* with calloc/malloc/realloc for the ChemicalSystem structure pointed by
* pChemicalSystem. All pointers of the ChemicalSystem structure that have been
* freed up are also set to NULL (but other variables than pointers are not reset
* to zero). This function does not return any value (void output).
*/
void freeChemicalMemory(ChemicalSystem* pChemicalSystem);

/**
* \fn int getChemicalFormat(char* fileLocation, int nameLength)
* \brief It determines if the file located at fileLocation exists and if its
*        name ends by the *.chem or *.wfn extension, with a length (strictly)
*        lower than the nameLength variable.
*
* \param[in] fileLocation A pointer that points to a string that is intended to
*                         store the path name of an existing *.chem or *.wfn
*                         (chemical) file. Its length must not be (strictly)
*                         greater than the nameLength variable (including the
*                         terminating nul character), and lower than six
*                         (to be able to store at least something more than the
*                         *.chem or *.wfn extension). Otherwise, an error is
*                         returned by the \ref getChemicalFormat function.
*
* \param[in] nameLength A positive integer representing the maximal length
*                       allowed for the name pointed by the fileLocation
*                       variable (including the terminating nul character). It
*                       must be (strictly) greater than six, otherwise an error
*                       is returned by the \ref getChemicalFormat function.
*
* \return It returns one (respectively minus one) if the location pointed by
*         fileLocation corresponds to the name of an existing *.chem (resp.
*         *.wfn) file. In any other situation (i.e. if the input variables do
*         not have the expected content), an error is displayed in the standard
*         error stream and zero is returned by the \ref getChemicalFormat
*         function.
*
* The \ref getChemicalFormat function should be static but has been defined as
* non-static in order to perform unit-test on it.
*/
int getChemicalFormat(char* fileLocation, int nameLength);

/**
* \fn int readChemFileandAllocateChemicalSystem(char* fileLocation,
*                                               int nameLength,
*                                               ChemicalSystem* pChemicalSystem)
* \brief It reads the *.chem file located at fileLocation, a name of length
*        (strictly) less than nameLength and having the *.chem extension, then
*        checks the *.chem syntax, dynamically allocates the required memory,
*        and fills the structure pointed by pChemicalSystem with the values
*        contained in the *.chem file.
*
* \param[in] fileLocation A pointer that points to a string that is intended to
*                         store the path name of an existing *.chem (chemical)
*                         file. Its length must not be (strictly) greater than
*                         the nameLength variable (including the terminating
*                         nul character), and lower than six (to be able to
*                         store at least something more than the *.chem
*                         extension). Otherwise, an error is returned by the
*                         \ref readChemFileandAllocateChemicalSystem function.
*
* \param[in] nameLength A positive integer representing the maximal length
*                       allowed for the name pointed by the fileLocation
*                       variable (including the terminating nul character). It
*                       must be (strictly) greater than six, otherwise an error
*                       is returned by the \ref
*                       readChemFileandAllocateChemicalSystem function.
*
* \param[out] pChemicalSystem A pointer that points to the ChemicalSystem
*                             structure (defined in main.h file) of the \ref
*                             main function, and that is intended to be filled
*                             with the values contained in the *.chem file
*                             pointed by the fileLocation variable. It must not
*                             point to NULL otherwise an error is returned by
*                             the \ref readChemFileandAllocateChemicalSystem
*                             function.
*
* \return It returns one (respectively minus one) if the chemical data contained
*         in the *.chem file pointed by the fileLocation variable have been
*         successfully loaded in the structure pointed by pChemicalSystem, and
*         if the underlying chemical structure corresponds to a restricted
*         (resp. unrestricted) Hartree-Fock one. In any other situation (i.e. if
*         the input variables, including the syntax inside the *.chem file do
*         not have the expected content), an error is displayed in the standard
*         error stream and zero is returned by the \ref
*         readChemFileandAllocateChemicalSystem function.
*
* The \ref readChemFileandAllocateChemicalSystem function should be static but
* has been defined as non-static in order to perform unit-test on it.
*/
int readChemFileandAllocateChemicalSystem(char* fileLocation, int nameLength,
                                               ChemicalSystem* pChemicalSystem);


// The function readAndConvertWfnFile reads a *.temp file (which is intended
// to be the copy of a *.wfn file) at fileLocation (such a file must have been
// previously created and of length strictly less than nameLength), except the
// first line, and converts all charToRemove encountered into charToPutInstead.
// If verbose is set to a positive value, then it prints some informations in
// the standard output stream; otherwise, nothing is displayed. It has a char*
// variable (fileLocation), two char variables (charToRemove, charToPutInstead),
// and two int variables (verbose, nameLength) as input arguments. It returns
// zero if an error occurred, otherwise one is returned in case of success

/**
* \fn int readAndConvertWfnFile(char* fileLocation, char charToRemove,
*                            char charToPutInstead, int verbose, int nameLength)
* \brief It tries to read an existing file specified at fileLocation (a path
*        name of (positive) length (strictly) less than nameLength). Such a file
*        is expected to have the *.wfn format and then, except for the first
*        line (comment line in the *.wfn file), all charToRemove encountered is
*        replaced by charToPutInstead. If verbose is set to a positive value,
*        then it prints some informations in the standard output stream;
*        otherwise, nothing is displayed.
*
* \param[in] fileLocation A pointer that points to a string that is intended to
*                         store the path name of an existing chemical file
*                         having the *.wfn format. Its length must not be
*                         (strictly) greater than the nameLength variable
*                         (including the terminating nul character '\0'), and
*                         lower than one (to be able to store at least
*                         something more than the terminating nul character
*                         '\0'). Otherwise, an error is returned by the \ref
*                         readAndConvertWfnFile function.
*
* \param[in] charToRemove Except for the first line, which corresponds to a
*                         comment line in the *.wfn format, it represents the
*                         character that is going to be tracked throughout the
*                         file pointed by the fileLocation variable, and that
*                         is intended to be replaced by the charToPutInstead
*                         character.
*
* \param[in] charToPutInstead It corresponds to the character that is going to
*                             replace each charToRemove encountered in the file
*                             specified at fileLocation, except for the first
*                             line, which is a comment line in the *.wfn format.
*
* \param[in] verbose An integer playing the role of a boolean. If set to a
*                    (strictly) positive value, then it prints in the standard
*                    output stream some informations about what is executed.
*                    Otherwise, nothing is displayed in the standard output
*                    stream.
*
* \param[in] nameLength A positive integer representing the maximal length
*                       allowed for the name pointed by the fileLocation
*                       variable (including the terminating nul character '\0').
*                       It must be (strictly) greater than one, otherwise an
*                       error is returned by the \ref readAndConvertWfnFile
*                       function.
*
* \return It returns one if every charToRemove has been successfully replaced by
*         charToPutInstead in the file located at fileLocation. Otherwise, an
*         error is returned in the standard error stream an zero is returned by
*         the \ref readAndConvertWfnFile function.
*
* First, we recall that in the \ref loadChemistry function, the string pointed
* by fileLocation in the \ref readAndConvertWfnFile function is intended to
* have the *.wfn format. Since we wish to modify the original *.wfn file, we
* have introduced a security here and copied the original *.wfn file into an
* *.temp file in the case an error would occur in the process. We also recall
* that we need to modify the (copy *.temp of the) *.wfn file because such a file
* is the output of the Gaussian software, which is written in Fortran. In
* particular, the scientific notation uses the 'D' symbol, which is not
* recognized by the standard fscanf c-function. Fortunately, one can notice that
* apart form the first line, which is a comment in the *.wfn format, the 'D'
* symbol now only appears either for scientific notation (Fortran 1.0D+01) or
* for the END DATA keyword so it can be easily converted into the C syntax
* (1.0e+01). We finally recall that the \ref readAndConvertWfnFile function
* should be static but has been defined as non-static in order to perform
* unit-test on it.
*/
int readAndConvertWfnFile(char* fileLocation, char charToRemove,
                            char charToPutInstead, int verbose, int nameLength);

/**
* \fn int readWfnFileAndAllocateChemicalSystem(char* fileLocation,
*                                              int nameLength, int verbose,
*                                               ChemicalSystem* pChemicalSystem)
* \brief It reads the wfn-format file located at fileLocation, a name of
*        (positive) length (strictly) less than nameLength, then checks the
*        *.wfn syntax, dynamically allocates the required memory,
*        and fills the structure pointed by pChemicalSystem with the values
*        contained in the wfn-format file. If verbose is set to a positive
*        value, then it prints some informations in the standard output stream;
*        otherwise, nothing is displayed.
*
* \param[in] fileLocation A pointer that points to a string that is intended to
*                         store the path name of an existing *.temp (chemical)
*                         file. We recall that for technical purpose, we have
*                         copied the *.wfn file into an *.temp file in order not
*                         to alter the original file in case of error. Its
*                         length must not be (strictly) greater than the
*                         nameLength variable (including the terminating nul
*                         character '\0'), and lower than six (to be able to
*                         store at least something more than the *.temp
*                         extension). Otherwise, an error is returned by the
*                         \ref readWfnFileAndAllocateChemicalSystem function.
*
* \param[in] nameLength A positive integer representing the maximal length
*                       allowed for the name pointed by the fileLocation
*                       variable (including the terminating nul character '\0').
*                       It must be (strictly) greater than six, otherwise an
*                       error is returned by the \ref
*                       readWfnFileAndAllocateChemicalSystem function.
*
* \param[in] verbose An integer playing the role of a boolean. If set to a
*                    (strictly) positive value, then it prints in the standard
*                    output stream some informations about what is executed.
*                    Otherwise, nothing is displayed in the standard output
*                    stream.
*
* \param[out] pChemicalSystem A pointer that points to the ChemicalSystem
*                             structure (defined in main.h file) of the \ref
*                             main function, and that is intended to be filled
*                             with the values contained in the (copy *.temp of
*                             the) *.wfn file pointed by the fileLocation
*                             variable. It must not point to NULL otherwise an
*                             error is returned by the \ref
*                             readWfnFileAndAllocateChemicalSystem function.
*
* \return It returns one (respectively minus one) if the chemical data contained
*         in the (copy *.temp of the) *.wfn file pointed by the fileLocation
*         variable have been successfully loaded in the structure pointed by
*         pChemicalSystem, and if the underlying chemical structure corresponds
*         to a restricted (resp. unrestricted) Hartree-Fock one. In any other
*         situation (i.e. if the input variables, including the syntax inside
*         the *.wfn file do not have the expected content), an error is
*         displayed in the standard error stream and zero is returned by the
*         \ref readWfnFileAndAllocateChemicalSystem function.
*
* The \ref readWfnFileAndAllocateChemicalSystem function should be static but
* has been defined as non-static in order to perform unit-test on it.
*/
int readWfnFileAndAllocateChemicalSystem(char* fileLocation, int nameLength,
                                            int verbose,
                                               ChemicalSystem* pChemicalSystem);

/**
* \fn int writingChemicalFile(char* fileLocation, int nameLength, int verbose,
*                                               ChemicalSystem* pChemicalSystem)
* \brief Following the *.chem format, it writes all the data contained in the
*        structure pointed by pChemicalSystem in a file located at fileLocation,
*        which is a path name of (positive) length (strictly) less than
*        nameLength. If verbose is set to a positive value, then it prints some
*        informations in the standard output stream; otherwise, nothing is
*        displayed.
*
* \param[in] fileLocation A pointer that points to a string that is intended to
*                         store the path name of the *.chem file that is
*                         intended to be filled with the values contained in the
*                         structure pointed by pParameters. Its length must not
*                         be (strictly) greater than the nameLength variable
*                         (including the terminating nul character '\0'), and
*                         lower than six (in order to be able to store at least
*                         something more than the *.chem extension). Otherwise,
*                         an error is returned by the \ref writingChemicalFile
*                         function. We also mention that if the path name for
*                         the file pointed by FileLocation is valid, such a
*                         *.chem file will be created if it does not already
*                         exists, and if it does not, it will be overwritten.
*
* \param[in] nameLength A positive integer representing the maximal length
*                       allowed for the name pointed by the fileLocation
*                       variable (including the terminating nul character '\0').
*                       It must be (strictly) greater than six, otherwise an
*                       error is returned by the \ref writingChemicalFile
*                       function.
*
* \param[in] verbose An integer playing the role of a boolean. If set to a
*                    (strictly) positive value, then it prints in the standard
*                    output stream some informations about what is executed.
*                    Otherwise, nothing is displayed in the standard output
*                    stream.
*
* \param[in] pChemicalSystem A pointer that points to the ChemicalSystem
*                             structure (defined in main.h file) of the \ref
*                             main function, and whose content is intended to be
*                             saved in the *.chem format at fileLocation. It
*                             must not point to NULL otherwise an error is
*                             returned by the \ref writingChemicalFile function.
*
* \return It returns one if the chemical data contained the structure pointed by
*         pChemicalSystem have been successfully saved in the *.chem file at
*         fileLocation. In any other situation (in particular if the input
*         variables do not have the expected content), an error is displayed in
*         the standard error stream and zero is returned by the \ref
*         writingChemicalFile function.
*
* The \ref writingChemicalFile function should be static but has been defined
* as non-static in order to perform unit-test on it.
*/
int writingChemicalFile(char* fileLocation, int nameLength, int verbose,
                                               ChemicalSystem* pChemicalSystem);

/**
* \fn int loadChemistry(Parameters* pParameters,
*                                               ChemicalSystem* pChemicalSystem)
* \brief It loads the values given in a existing *.chem or *.wfn (chemical)
*        file given by the name_chem variable of the structure pointed by
*        pParameters, and store them in the variables of the structure pointed
*        by pChemicalSystem.
*
* \param[in] pParameters A pointer that points to the Parameters structure
*                        (defined in main.h file) of the \ref main function. Its
*                        name_chem variable is expected to store the name of an
*                        existing *.chem or *.wfn file whose associated values
*                        are intended to be loaded and stored in the structure
*                        pointed by pChemicalSystem.
*
* \param[out] pChemicalSystem A pointer that points to the ChemicalSystem
*                             structure (defined in main.h file) of the \ref
*                             main function.
*
* \return It returns one if the structure pointed by pChemicalSystem has been
*         successfully initialized to its prescribed values. Otherwise, zero is
*         returned if any error is encountered during the process.

* The function \ref loadChemistry takes the informations given in a *.chem or
* *.wfn (chemical) file, pointed by the name_chem variable of the Parameters
* structure associated with the \ref main funtion and pointed by pParameters,
* in order to initialize all the (double/integer) variables of the
* ChemicalSystem structure of the \ref main function, pointed by the
* pChemicalSystem variable.
*/
int loadChemistry(Parameters* pParameters, ChemicalSystem *pChemicalSystem);

#endif

