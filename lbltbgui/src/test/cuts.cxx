cuts.cxx


//0xC3000009
Bool_t Decoder::OnlineMonitoringADCRO()
{
    unsigned int nHeaders = 0;
    while (SeekHeader())
    {
        nHeaders++;
        cout << DbgS() << "nHeader found: " << nHeaders << endl;

        unsigned int ibegin = fPos;
        unsigned int iend   = fPos;
        if (SeekTrailer())
        {
            iend = fPos;
        }
        else
        {
            cout << "[e] missing trailer." << endl;
            return kFALSE;
        }
        cout << DbgS() << "found block [" << ibegin << " - " << iend << "]" << endl;
        fPos = ibegin + 1;
        unsigned int nValidADCdata = 0;
        while (SeekADCHeader(iend))
        {
            PayloadADC l(fWords, fPos);
            if (l.IsValid())
            {
                //cout << DbgS() << "valid ADC data read Nwords: " << dec << l.GetNWords() << endl;
                fPos += l.GetNWords();
                nValidADCdata++;
                // add the XY data
                fX.push_back(l.GetTimeStamp() * 1.);
                std::vector<double> v = l.GetADCf();
                for (unsigned int iadc = 0; iadc < v.size(); iadc++)
                {
                    fY[iadc].push_back(v[iadc]);
                }               
            }
            else
            {
                cout << DbgS() << "invalid ADC data read... context:" << endl;
                DumpContentWords(fPos, fPos + 2);
                fPos += 1; // advance anyways
            }
        }
        cout << DbgS() << "N valid ADC data: " << nValidADCdata << endl;
    }
    return kTRUE;
}

------------------
 ifstream is;
 is.open ("C:\\Final.gif", ios::binary );

 // get length of file:
 is.seekg (0, ios::end);
 length = is.tellg();
 is.seekg (0, ios::beg);

 // allocate memory:
 buffer = new char [length];

 // read data as a block:
 is.read (buffer,length);
 is.close();

 FILE *pFile;
 pFile = fopen ("C:\\myfile.gif", "w");
 fwrite (buffer , 1 , sizeof(buffer) , pFile );

 ///

#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    unsigned long address = 0;
    char c;

    cout << hex << setfill('0');
    while( cin.good() )
    {
        int nread;
        char buf[16];

        for( nread = 0; nread < 16 && cin.get(buf[nread]); nread++ );
        if( nread == 0 ) break;

        // Show the address
        cout << setw(8) << address;

        // Show the hex codes
        for( int i = 0; i < 16; i++ )
        {
            if( i % 8 == 0 ) cout << ' ';
            if( i < nread )
                cout << ' ' << setw(2) << (unsigned)buf[i];
            else 
                cout << "   ";
        }

        // Show printable characters
        cout << "  ";
        for( int i = 0; i < nread; i++)
        {
            if( buf[i] < 32 ) cout << '.';
            else cout << buf[i];
        }

        cout << "\n";
        address += 16;
    }
    return 0;
}