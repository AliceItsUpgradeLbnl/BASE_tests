//==========================================================================
// File Reader class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/26/2016
// Last modified: 8/26/2016
// Description: contains methods to read data from an input file and to
// transfer that data to a buffer (queue) 
//==========================================================================

#include "FileReader.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

FileReader::FileReader(char* filename)
{
  Initialize(filename);
}

FileReader::~FileReader()
{
  Destroy();
}

//===========================================================================
//                               Inner Methods
//===========================================================================

// Public

bool FileReader::ReadDataFromFile(unsigned int targetWord, unsigned int position, unsigned int numberOfWordsToRead)
{
  FindWordInFile(targetWord, position);
  TransferDataFromFileToBuffer(numberOfWordsToRead);
  RewindFile();
}

bool FileReader::FindWordInFile(unsigned int targetWord, unsigned int position)
{
  for (int i = 0; i < position; i++) {
    while (!CompareWordInFileToArgument(targetWord)) {
      if (endOfFile) {
        std::cout << "FileReader: Unexpected end of file while finding a keyword in the input file.\n" << std::endl;
        exit(EXIT_FAILURE);
      }
      ReadWordFromFile();
    }
  }

  return true;
}

bool FileReader::TransferDataFromFileToBuffer(unsigned int numberOfWords)
{
  unsigned int wordCounter = 0;
  bool readAllFile = (numberOfWords == 0xFFFFFFFF);

  unsigned int wordToWriteToBuffer = 0;
  while ( wordCounter != numberOfWords ){
    wordToWriteToBuffer = ReadWordFromFile();
    if (!endOfFile) dataBuffer->push(wordToWriteToBuffer);
    else {
      if (readAllFile) {
        dataBuffer->push(0xFFFFFFFF);
        return endOfFile;
      }
      else {
        std::cout << "FileReader: Unexpected end of file while tranferring data from file to queue.\n" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    if (!readAllFile) wordCounter++;
  }

  return !endOfFile;
}

unsigned int FileReader::PopWordFromDataBuffer()
{
  unsigned int valueFromBuffer = dataBuffer->front();
  dataBuffer->pop();

  return valueFromBuffer;
}

// Private

void FileReader::Initialize(char* filename)
{

  dataFile = new std::ifstream(filename, std::fstream::in);
  if (!dataFile->good()) {
    std::cout << "FileReader: No valid input file provided. The input file either does not exist or it is corrupted.\n" << std::endl;
    exit(EXIT_FAILURE);
  }

  endOfFile = false;

  dataBuffer = new std::queue <unsigned int>;
}

void FileReader::Destroy()
{
  delete dataBuffer;
  delete dataFile;
}

unsigned int FileReader::ReadWordFromFile()
{
  char a[] = {'\0','\0','\0','\0','\0'};

  for (int i = 0; i < 4; i++){
    if (dataFile->eof()) {
      endOfFile = true;
    }
    dataFile->get(a[i]);
  }

  return DatatypeConversion::Stoi(a, 4, DatatypeConversion::IsLittleEndian, true);
}

bool FileReader::CompareWordInFileToArgument(unsigned int wordToCompareTo)
{
  char a[] = {'\0','\0','\0','\0','\0'};
  unsigned int wordReadFromFile = 0;
  for (int i = 0; i < 4; i++){
    if (dataFile->eof()) endOfFile = true;
    dataFile->get(a[i]);
  }
  for (int i = 0; i < 4; i++){ // Want to bring the read pointer to the initial value
    dataFile->putback(a[3-i]);
  }
  wordReadFromFile = DatatypeConversion::Stoi(a, 4, DatatypeConversion::IsLittleEndian, true);

  return (wordReadFromFile == wordToCompareTo);
}

void FileReader::RewindFile()
{
  dataFile->clear();
  dataFile->seekg(0);
}
