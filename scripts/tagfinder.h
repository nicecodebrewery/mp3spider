#include <string>
#include <vector>
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
            cout<< frameID << " : " << content << "( "<< frameSize << " )" <<"\n";
        }
        delete[] frameData;
    }
    file.close();
    return tagSize;
}

int encodeSynchsafe(int val) {
    return ((val & 0x0FE00000) << 3) |
           ((val & 0x001FC000) << 2) |
           ((val & 0x00003F80) << 1) |
           ((val & 0x0000007F));
}

vector<char> makeTextFrame(const string& id, const string& value) {
    vector<char> frame;

    // Frame ID
    frame.insert(frame.end(), id.begin(), id.end());

    // Frame content: encoding byte + text
    int contentSize = 1 + value.size();
    frame.push_back((contentSize >> 24) & 0xFF);
    frame.push_back((contentSize >> 16) & 0xFF);
    frame.push_back((contentSize >> 8) & 0xFF);
    frame.push_back(contentSize & 0xFF);

    // Frame flags (2 bytes)
    frame.push_back(0x00);
    frame.push_back(0x00);

    // Encoding: 0x00 = ISO-8859-1
    frame.push_back(0x00);
    frame.insert(frame.end(), value.begin(), value.end());

    return frame;
}

void buildID3v2HeaderOnly(const string& outFile,
                          const string& title,
                          const string& artist,
                          const string& album,
                          const string& year,
                          const string& publisher) {
    vector<char> tag;

    // Add frames
    auto add = [&](const string& id, const string& value) {
        auto frame = makeTextFrame(id, value);
        tag.insert(tag.end(), frame.begin(), frame.end());
    };

    add("TIT2", title);
    add("TPE1", artist);
    add("TALB", album);
    add("TYER", year);
    add("TPUB", publisher);

    // Create header
    int tagSize = tag.size();
    int synchsafe = encodeSynchsafe(tagSize);

    ofstream file(outFile, ios::binary);
    file.write("ID3", 3);      // ID3 tag
    file.put(0x03);            // Version 2.3
    file.put(0x00);            // Revision
    file.put(0x00);            // Flags
    file.put((synchsafe >> 24) & 0x7F);
    file.put((synchsafe >> 16) & 0x7F);
    file.put((synchsafe >> 8) & 0x7F);
    file.put(synchsafe & 0x7F);

    file.write(tag.data(), tag.size());
    file.close();
}

void appendAudioFromOriginal(const string& originalFile, const string& newTagFile, const string& finalOutput) {
    ifstream original(originalFile, ios::binary);
    ofstream final(finalOutput, ios::binary);

    // Step 1: Write the new tag from newTagFile
    ifstream tag(newTagFile, ios::binary);
    final << tag.rdbuf(); // copy entire tag
    tag.close();

    // Step 2: Skip old ID3 tag in original.mp3
    char header[10];
    original.read(header, 10);
    if (string(header, 3) != "ID3") {
        cerr << "Original file has no ID3 tag!\n";
        return;
    }

    int tagSize = ((header[6] & 0x7F) << 21) |
                  ((header[7] & 0x7F) << 14) |
                  ((header[8] & 0x7F) << 7)  |
                  (header[9] & 0x7F);
    int skipBytes = 10 + tagSize;

    // Step 3: Skip the tag bytes
    original.seekg(skipBytes, ios::beg);

    // Step 4: Copy the remaining audio data
    final << original.rdbuf();

    original.close();
    final.close();
}