#ifndef __LogFile__HH
#define __LogFile__HH

#include <iostream>
#include <fstream>
#include <string>

#include <TObject.h>

class LogFile : public TObject
{
public:
   LogFile()
   : TObject()
   , fname("./ED.log")
   , coutbuf(0)
   , cerrbuf(0)
   , outf(fname.c_str())
   {
      ;
   }
   LogFile(const char *fn)
   : TObject()
   , fname(fn)
   , coutbuf(0)
   , cerrbuf(0)
   , outf(fname.c_str())
   {
      ;
   }

   void Redirect()
   {
       coutbuf = std::cout.rdbuf(); //save old buf
       std::cout.rdbuf(outf.rdbuf()); //redirect std::cout to out.txt!
       cerrbuf = std::cerr.rdbuf(); //save old buf
       std::cerr.rdbuf(outf.rdbuf()); //redirect std::cout to out.txt!
   }

   void Restore()
   {
      if (coutbuf != 0)
          std::cout.rdbuf(coutbuf);
       if (cerrbuf != 0)
          std::cerr.rdbuf(cerrbuf);      
   }

   ~LogFile()
   {
      Restore();
      outf.close();
   }

private:
   std::string fname;
   std::streambuf *coutbuf;
   std::streambuf *cerrbuf;
   std::ofstream  outf;

   ClassDef(LogFile, 0)
};

#endif