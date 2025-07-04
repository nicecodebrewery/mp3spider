#include <string>
#include <fstream>
#include <iostream>
using namespace std;

ifstream file;

int getHeader(string filename){
    file.open(filename,ios::binary);
    char header[10];
    file.read(&header[0],10);
    if (string(header, 3) != "ID3") {
        cerr << "Not an ID3v2 tag.\n";
        file.close();
        return 0;
    }
    int tagSize = ((header[6] & 0x7F) << 21) |
              ((header[7] & 0x7F) << 14) |
              ((header[8] & 0x7F) << 7)  |
              (header[9] & 0x7F);

    int bytesRead = 10;

    while (bytesRead < tagSize + 10)
    {
        if (file.eof() || !file.good()) break;

        char frameHeader[10];
        file.read(frameHeader,10);
        if (file.gcount() < 10) break;
        bytesRead +=10;

        string frameID(frameHeader, frameHeader + 4);
        if (frameID[0] == 0 ) break;

        int frameSize = (unsigned char)frameHeader[4] << 24 |
                    (unsigned char)frameHeader[5] << 16 |
                    (unsigned char)frameHeader[6] << 8  |
                    (unsigned char)frameHeader[7];

        if (frameSize <= 0 || frameSize > (tagSize - bytesRead)) {
            cerr << "Invalid frame size: " << frameSize << endl;
            break;
        }

        char * frameData = new char[frameSize];
        file.read(frameData,frameSize);
        if (file.gcount() < frameSize) {
            delete[] frameData;
            cerr << "Could not read full frame.\n";
            break;
        }
        bytesRead += frameSize;

        if (frameID[0] == 'T'){
            uint8_t encoding = frameData[0];
            string content(frameData+1,frameSize-1);
            cout<< frameID << " : " << content << "\n";
        }
        delete[] frameData;
    }
    file.close();
    return tagSize;
}