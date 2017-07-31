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
        const int nbyteword = 4;
        char buf[nbyteword];

        for( nread = 0; nread < nbyteword && cin.get(buf[nread]); nread++ );
        if( nread == 0 ) break;

        // Show the address
        cout << setw(nbyteword) << address;

        // Show the hex codes
        for( int i = 0; i < nbyteword; i++ )
        {
            if( i % nbyteword == 0 ) cout << ' ';
            if( i < nread )
            {
                unsigned char c = buf[i];
                //cout << uppercase << ' ' << setw(2) << unsigned(c);
                cout << ' ' << setw(2) << unsigned(c);
            }
            //else 
            //    cout << "   ";
        }

        for (int i = 0; i < nbyteword-nread; i++)
        {
            cout << ' ' << setw(2) << "";
        }

        // Show printable characters
        cout << "  ";
        for( int i = 0; i < nread; i++)
        {
            if( buf[i] < 32 ) cout << '.';
            else cout << buf[i];
        }

        cout << "\n";
        address += nbyteword;
    }
    return 0;
}
