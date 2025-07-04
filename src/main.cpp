#include "../scripts/tagfinder.h"
#include "cstdio"
using namespace std;

int main(int argc, char const *argv[])
{
    cout << getHeader("out.mp3") << endl;
    buildID3v2HeaderOnly("temp.mp3","Vijay2023","Vijay","nil","2025","Me");
    appendAudioFromOriginal("out.mp3","temp.mp3","out2.mp3");
    if (remove("temp.mp3") != 0){
        cerr << "Sorry , The tempfile is not deleted, do it yourself.\n";
    }
    return 0;
}
