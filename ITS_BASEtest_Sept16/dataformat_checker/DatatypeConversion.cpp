//==========================================================================
// Datatype conversion class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 5/16/2016
// Last modified: 5/16/2016
// Description: contains methods to make conversions between data types
//==========================================================================

#include "DatatypeConversion.h"

// Constructors and Destructor

DatatypeConversion::DatatypeConversion(bool initializeDataMembers)
{
  if (initializeDataMembers)
    littleEndian = IsLittleEndian();
}

DatatypeConversion::~DatatypeConversion()
{
};

// Methods:

// Checks endianness of the machine
bool DatatypeConversion::IsLittleEndian()
{
  unsigned int integ = 0x41;
  char *c = (char *) &integ;

  return (*c == 'A');
}

// Converts a string to a char array and returns a pointer to the array 
void DatatypeConversion::Stoc(std::string str, char *chArray)
{
  assert(chArray != NULL);

  for (int i = 0; i < str.size(); i++) {
    chArray[i] = str[i];
  }
  chArray[str.size()] = '\0';
} 

// Converts a string to an unsigned integer
unsigned int DatatypeConversion::Stoi(std::string str)
{
  unsigned int littleEndian = true;
  unsigned int result = 0;
  unsigned int numBytesToConvert = (str.size() > sizeof(unsigned int)) ? sizeof(unsigned int) : str.size();

  for (int i = 0; i < numBytesToConvert; i++) {
    if (littleEndian)
      *(((char *)&result) + i) = str[str.size() - i - 1];
    else
      *(((char *)&result) + sizeof(unsigned int) - i - 1) = str[str.size() - i - 1];
  }

  return result;
}

// Converts a string to an unsigned integer (str = string, strLen = lenght of the string excluded the termination character)
unsigned int DatatypeConversion::Stoi(std::string str, unsigned int strLen)
{
  assert(CheckStringLength(str, strLen)); // Actual string length doesn't match passed argument (strLen)
  assert(strLen <= sizeof(unsigned int)); // Passed string is longer than unsigned int 

  unsigned int result = 0;
  unsigned int numBytesToConvert = (strLen > sizeof(unsigned int)) ? sizeof(unsigned int) : strLen;

  for (int i = 0; i < numBytesToConvert; i++) {
    if (littleEndian)
      *(((char *)&result) + i) = str[strLen - i - 1];
    else
      *(((char *)&result) + sizeof(unsigned int) - i - 1) = str[strLen - i - 1];
  }

  return result;
}

// Converts a string (array of char) to an unsigned integer (str = string, strLen = lenght of the string excluded the termination character)
unsigned int DatatypeConversion::Stoi(char *str, unsigned int strLen)
{
  assert(str != NULL); // Pointer to char array is NULL
//  assert(CheckStringLength(str, strLen)); // Actual string length doesn't match passed argument (strLen)
  assert(strLen <= sizeof(unsigned int)); // Passed string is longer than unsigned int 

  unsigned int result = 0;
  unsigned int numBytesToConvert = (strLen > sizeof(unsigned int)) ? sizeof(unsigned int) : strLen;

  for (int i = 0; i < numBytesToConvert; i++) {
    if (littleEndian)
      *(((char *)&result) + i) = str[strLen - i - 1];
    else
      *(((char *)&result) + sizeof(unsigned int) - i - 1) = str[strLen - i - 1];
  }

  return result;
}

// Converts a string (array of char) to an unsigned integer (str = string, strLen = lenght of the string excluded the termination character)
unsigned int DatatypeConversion::Stoi(char *str, unsigned int strLen, bool invertBytes)
{
  assert(str != NULL); // Pointer to char array is NULL
//  assert(CheckStringLength(str, strLen)); // Actual string length doesn't match passed argument (strLen)
  assert(strLen <= sizeof(unsigned int)); // Passed string is longer than unsigned int 

  unsigned int result = 0;
  unsigned int numBytesToConvert = (strLen > sizeof(unsigned int)) ? sizeof(unsigned int) : strLen;

  for (int i = 0; i < numBytesToConvert; i++) {
    if (littleEndian) {
      if (invertBytes)
        *(((char *)&result) + sizeof(unsigned int) - i - 1) = str[strLen - i - 1];
      else
        *(((char *)&result) + i) = str[strLen - i - 1];
    }
    else {
      if (invertBytes)
        *(((char *)&result) + i) = str[strLen - i - 1];
      else
        *(((char *)&result) + sizeof(unsigned int) - i - 1) = str[strLen - i - 1];
    }
  }

  return result;
}

// Converts a string (array of char) to an unsigned integer (str = string, strLen = lenght of the string excluded the termination character)
unsigned int DatatypeConversion::Stoi(char *str, unsigned int strLen, bool littleEndian, bool invertBytes)
{
  assert(str != NULL); // Pointer to char array is NULL
//  assert(CheckStringLength(str, strLen)); // Actual string length doesn't match passed argument (strLen)
  assert(strLen <= sizeof(unsigned int)); // Passed string is longer than unsigned int 

  unsigned int result = 0;
  unsigned int numBytesToConvert = (strLen > sizeof(unsigned int)) ? sizeof(unsigned int) : strLen;

  for (int i = 0; i < numBytesToConvert; i++) {
    if (littleEndian) {
      if (invertBytes)
        *(((char *)&result) + sizeof(unsigned int) - i - 1) = str[strLen - i - 1];
      else
        *(((char *)&result) + i) = str[strLen - i - 1];
    }
    else {
      if (invertBytes)
        *(((char *)&result) + i) = str[strLen - i - 1];
      else
        *(((char *)&result) + sizeof(unsigned int) - i - 1) = str[strLen - i - 1];
    }
  }

  return result;
}

// Converts unsigned integer to string 
std::string DatatypeConversion::Itos(unsigned int integ)
{
  unsigned int littleEndian = true;

  std::string result(sizeof(unsigned int), '\0');

  for (int i = 0; i < sizeof(unsigned int); i++) {
    if (littleEndian)
      result[i] = *((char *)&integ + sizeof(unsigned int) - i - 1);
    else
      result[i] = *((char *)&integ + i);
  }
  result[sizeof(unsigned int)] = '\0';

  return result;
}
/*
// Converts unsigned integer to hexadecimal string
std::string DatatypeConversion::Itoh(unsigned int integ)
{
  std::string result(2*sizeof(unsigned int), '\0');
  
  unsigned int integer = integ;

  for (int i = 0; i < 2*sizeof(unsigned int); i++){

    if (i != 0) integer >>= 4; // Shift and overwrite if i is nonzero

    if (littleEndian)
      result[2*sizeof(unsigned int) - i - 1] = (*(char *)&(integer) & 0x0F);
    else
      result[i] = (*(char *)&(integer) & 0x0F);

    if (result[2*sizeof(unsigned int) - i - 1] <= 9)
      result[2*sizeof(unsigned int) - i - 1] += 48;
    else
      result[2*sizeof(unsigned int) - i - 1] += 55;

  }
  return result;
}
*/
// Converts unsigned integer to hexadecimal string
std::string DatatypeConversion::Itoh(unsigned int integ, bool littleEndian)
{
  std::string result(2*sizeof(unsigned int), '\0');
  
  unsigned int integer = integ;

  for (int i = 0; i < 2*sizeof(unsigned int); i++){

    if (i != 0) integer >>= 4; // Shift and overwrite if i is nonzero

    if (littleEndian)
      result[2*sizeof(unsigned int) - i - 1] = (*(char *)&(integer) & 0x0F);
    else
      result[i] = (*(char *)&(integer) & 0x0F);

    if (result[2*sizeof(unsigned int) - i - 1] <= 9)
      result[2*sizeof(unsigned int) - i - 1] += 48;
    else
      result[2*sizeof(unsigned int) - i - 1] += 55;

  }
  return result;
}

// Converts hexadecimal string to unsigned integer
unsigned int DatatypeConversion::Htoi(std::string str, unsigned int strLen)
{ 
  assert(CheckStringLength(str, strLen)); // Actual string length doesn't match passed argument (strLen)
  assert(strLen <= 2*sizeof(unsigned int));  // Passed string is longer than 2*sizeof(unsigned int) 

  unsigned int result = 0;
  unsigned int numNibblesToConvert = (strLen > 2*sizeof(unsigned int)) ? 2*sizeof(unsigned int) : 2*(strLen/2 + strLen%2);
  char char0 = '\0';
  char char1 = '\0';

  for (int i = 0; i < numNibblesToConvert; i+=2) {
    char0 = str[strLen - i - 1];
    char1 = str[strLen - i - 2];    
    assert(CharIsHex(char0));
    assert(CharIsHex(char1));    
    if (littleEndian)
      *(((char *)&result) + i/2) = Htoc(char1)*16 + Htoc(char0);
    else
      *(((char *)&result) + sizeof(unsigned int) - i/2 - 1) = Htoc(char1)*16 + Htoc(char0);
  }

  return result;
}

// Converts char to unsigned integer
unsigned int DatatypeConversion::Ctoi(char c)
{

  unsigned int result;
/*
  unsigned int littleEndian = false;

  if (littleEndian) {
    result = *(unsigned int *)&c & 0x000000FF;
  }
  else {
    for (int i = 0; i < sizeof(unsigned int); i++) {
      if (i == 0)
        *((char *)&result + 3) = c;
      else
        *((char *)&result + sizeof(unsigned int) - i - 1) = '\0';
    }
  }
*/
  result = (*(unsigned int *)&c) & 0x000000FF;

  return result;
}

// Converts unsigned integer (Least Significant Byte) to char
char DatatypeConversion::Itoc(unsigned int integ)
{
  char result;

  unsigned int littleEndian = true;

  if (littleEndian)
    result = *(char *)&integ;
  else
    result = *((char *)&integ + 3);

  return result;
}

// Converts a hexadecimal char to an integer (char)
char DatatypeConversion::Htoc(char c)
{
  char result;

  if (c >= 48 && c <= 57) {
    result = c - 48;
  }
  else if (c >= 65 && c <= 70) {
    result = c - 55;
  }
  else if (c >= 97 && c <= 102) {
    result = c - 87;
  }
  else {
    result = '\0';
  }

  return result;
}

// Converts an integer to an hexadecimal char
char DatatypeConversion::Ctoh(char c)
{
  char result;
  if (c >= 0 && c <= 9) {
    result = c + 48;
  }
  else if (c >= 10 && c <= 15) {
    result = c + 55;
  }
  else {
    result = '\0';
  }
  return result;
}

// Check matching of string length with the passed argument (strLen), returns true if they match
bool DatatypeConversion::CheckStringLength(std::string str, unsigned int strLen)
{
  bool result = true;
  if (str.size() != strLen)
    result = false;
  return result;
}

// Check matching of string length with the passed argument (strLen), returns true if they match
bool DatatypeConversion::CheckStringLength(char *str, unsigned int strLen)
{
  assert(str != NULL); // Pointer to char array is NULL
  bool result = true;
  for (int i = 0; i < strLen; i++) {
    if (str[i] == '\0') {
      result = false;
      break;
    }
  }
  if (str[strLen] != '\0')
    result = false;
  return result;
}

// Check if passed argument is an hexadecimal char, returns true in case
bool DatatypeConversion::CharIsHex(char c)
{
  if ( (c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102) )
    return true;
  else
    return false;
}
