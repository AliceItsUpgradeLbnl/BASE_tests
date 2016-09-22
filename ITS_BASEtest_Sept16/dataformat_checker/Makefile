# Assumes that the script is in MyScript.C
# and it must contain a main() function

SOURCE_A = CompositeOperationParameters.cpp \
         CompositeOperation.cpp             \
         DatatypeConversion.cpp             \
         FileReader.cpp                     \
         SingleOperation.cpp                \
         SingleOperationParameters.cpp      \
         DataFormatChecker.cpp              \
         Decoder.cpp                        \
         DecoderParameters.cpp              \
         Statistics.cpp

SOURCE_B = ./test_files/DatatypeConversion.cpp \
         ./test_files/DataGenerator.cpp

SOURCE_C = CompositeOperationParameters.cpp \
         CompositeOperation.cpp             \
         DatatypeConversion.cpp             \
         FileReader.cpp                     \
         SingleOperation.cpp                \
         SingleOperationParameters.cpp      \
         DataAnalysis.cpp                   \
         Decoder.cpp                        \
         DecoderParameters.cpp              \
         Statistics.cpp                     \
         Tests.cpp                          \
         Display.cpp                        \
         DisplayStructures.cpp

dataformat_checker :
	g++ -w -std=c++0x $(SOURCE_A) `root-config --cflags --ldflags --glibs` -o ./test_files/dataformat_checker

dataformat_generator :
	g++ -w -std=c++11 $(SOURCE_B) `root-config --cflags --ldflags --glibs` -o ./test_files/dataformat_generator

data_analysis :
	g++ -w -std=c++0x $(SOURCE_C) `root-config --cflags --ldflags --glibs` -o ./test_files/data_analysis

all : 
#	g++ -w -std=c++11 *.cpp `root-config --cflags --ldflags --glibs` -o dataformat_checker
