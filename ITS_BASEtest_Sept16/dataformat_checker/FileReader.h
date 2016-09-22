//==========================================================================
// File Reader class - Header file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/26/2016
// Last modified: 8/26/2016
// Description: contains methods to read raw data from file in the format
// defined for 9/16 testbeam (each word is 32-bit)
//==========================================================================

#ifndef FILEREADER_H
#define FILEREADER_H

// C/C++
#include <assert.h>
#include <iostream>
#include <queue>
#include <fstream>

// User defined
#include "DatatypeConversion.h"
 
class FileReader
{
  public:
    // Contructors and destructor
     FileReader(char* filename);
    ~FileReader();

    bool                  ReadDataFromFile(unsigned int targetWord, unsigned int position, unsigned int numberOfWordsToRead);
    bool                  FindWordInFile(unsigned int targetWord, unsigned int position);
    bool                  TransferDataFromFileToBuffer(unsigned int numberOfWords);

    unsigned int          PopWordFromDataBuffer();

  private:
    void                  Initialize(char* filename);
    void                  Destroy();

    unsigned int          ReadWordFromFile(); 
    bool                  CompareWordInFileToArgument(unsigned int wordToCompareTo);
 
    void                  RewindFile();

    std::ifstream             *dataFile;

    std::queue <unsigned int> *dataBuffer;

    bool                       endOfFile;
};
 
#endif
