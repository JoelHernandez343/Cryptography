#include <iostream>

#include <cryptopp/cryptlib.h> // Always
#include <cryptopp/secblock.h> // For SecByteBlock
#include <cryptopp/modes.h>    // For modes
#include <cryptopp/osrng.h>    // For AutoSeededRandomPool
#include <cryptopp/hex.h>      // For HexEncoder
#include <cryptopp/aes.h>

using namespace std;
using namespace CryptoPP;

int main(void) {

    AutoSeededRandomPool prng;

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    prng.GenerateBlock( key, key.size() );

    CryptoPP::byte iv[ AES::BLOCKSIZE ];
    prng.GenerateBlock( iv, sizeof(iv) );

    string plain = "CBC Mode Test";
    string cipher, encoded, recovered;

    /*********************************\
    \*********************************/

    try
    {
        cout << "plain text: " << plain << endl;

        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV( key, key.size(), iv );

        // The StreamTransformationFilter adds padding
        //  as required. ECB and CBC Mode must be padded
        //  to the block size of the cipher.
        StringSource ss( plain, true, 
            new StreamTransformationFilter( e,
                new StringSink( cipher )
            ) // StreamTransformationFilter      
        ); // StringSource
    }
    catch( const CryptoPP::Exception& e )
    {
        cerr << e.what() << endl;
        exit(1);
    }

    /*********************************\
    \*********************************/

    // Pretty print cipher text
    StringSource ss( cipher, true,
        new HexEncoder(
            new StringSink( encoded )
        ) // HexEncoder
    ); // StringSource
    cout << "cipher text: " << encoded << endl;

    /*********************************\
    \*********************************/

    try
    {
        CBC_Mode< AES >::Decryption d;
        d.SetKeyWithIV( key, key.size(), iv );

        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource ss( cipher, true, 
            new StreamTransformationFilter( d,
                new StringSink( recovered )
            ) // StreamTransformationFilter
        ); // StringSource

        cout << "recovered text: " << recovered << endl;
    }
    catch( const CryptoPP::Exception& e )
    {
        cerr << e.what() << endl;
        exit(1);
    }


    return 0;
}