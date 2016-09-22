//==========================================================================
// Datatype conversion class - Header file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 5/16/2016
// Last modified: 5/16/2016
// Description: contains methods to make conversions between data types
//==========================================================================

#ifndef DATATYPECONVERSION_H
#define DATATYPECONVERSION_H

#include <iostream>
#include <assert.h>
 
class DatatypeConversion
{
  public:
    DatatypeConversion(bool initializeDataMembers);
    ~DatatypeConversion();

    bool littleEndian;

    static bool IsLittleEndian(); // checks endianness of the machine, returns true if little
    void Stoc(std::string str, char *chArray); // string to char array
    static unsigned int Stoi(std::string str); // string to unsigned integer
    unsigned int Stoi(std::string str, unsigned int strLen); // string to unsigned integer (check also string length and if string longer than uint)
    unsigned int Stoi(char *str, unsigned int strLen); // string (array of char) to unsigned integer 
    unsigned int Stoi(char *str, unsigned int strLen, bool invertBytes);
    static unsigned int Stoi(char *str, unsigned int strLen, bool littleEndian, bool invertBytes);
    static std::string Itos(unsigned int integ); // unsigned int to string
    unsigned int Htoi(std::string str, unsigned int strLen); // hexadecimal string to unsigned integer  --> To be tested
//    static std::string Itoh(unsigned int integ); // unsigned integer to hexadecimal string  --> To be tested
    static std::string Itoh(unsigned int integ, bool littleEndian);
    static unsigned int Ctoi(char c); // character to unsigned integer
    static char Itoc(unsigned int integ); // unsigned integer (least significant byte) to char
    char Htoc(char c); // returns hexadecimal char of a char int if possible ('\0' otherwise)
    static char Ctoh(char c); // returns char int value of a hexadecimal char if possible ('\0' otherwise)
    bool CheckStringLength(std::string str, unsigned int strLen); // checks if string length matches argument and returns true if this is the case
    bool CheckStringLength(char *str, unsigned int strLen); // checks if string (array of char) length matches argument and returns true in this case
    bool CharIsHex(char c); // checks if argument is an hexadecimal char and returns true in this case
};

#endif
