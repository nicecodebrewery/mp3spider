#include "../scripts/tagfinder.h"
#include "cstdio"
using namespace std;

int main(int argc, char const *argv[])
{
    if (argc == 1) {
        cout << "No input file!\n";
        //getHeader("out.mp3");
        return 0;
    }

    if (argc < 2 || argc > 8) {
        cerr << "Usage:\n"
             << argv[0] << " <original.mp3> <output.mp3> [title] [artist] [album] [year] [publisher]\n";
        return 1;
    }

    if (argc == 2)
    {
        getHeader(argv[1]);
        return 0;
    }
    

    string inputFile = argv[1];
    string outputFile = argv[2];
    string title = (argc > 3) ? argv[3] : "";
    string artist = (argc > 4) ? argv[4] : "";
    string album = (argc > 5) ? argv[5] : "";
    string year = (argc > 6) ? argv[6] : "";
    string publisher = (argc > 7) ? argv[7] : "";
    string tempFile = "temp.mp3";

    // Check if input file exists
    ifstream infile(inputFile, ios::binary);
    if (!infile) {
        cerr << "Error: Could not open input file '" << inputFile << "'.\n";
        return 1;
    }
    infile.close();

    // Step 1: Build new header
    buildID3v2HeaderOnly(tempFile, title, artist, album, year, publisher);

    // Step 2: Append audio
    appendAudioFromOriginal(inputFile, tempFile, outputFile);

    // Step 3: Cleanup temp file
    if (remove(tempFile.c_str()) != 0) {
        cerr << "Warning: Temporary file '" << tempFile << "' could not be deleted.\n";
    }

    cout << "Metadata written successfully to '" << outputFile << "'.\n";
    return 0;
}
