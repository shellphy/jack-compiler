#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "VritualMachion.h"

using namespace std;

vector<string> filenames;

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "Usage: vm sourcefiles\n";
        exit(-1);
    }
    ifstream fin;
    for (int i = 1; i < argc; i++)
    {
        fin.open(argv[i]);
        if (fin.fail())
        {
            cerr << "file '" << argv[i] << "' not exist!";
            exit(-1);
        }
        filenames.push_back(argv[i]);
        fin.close();
    }
    // 未判断文件是否存在
    filenames.push_back("./api/Array.vm");
    filenames.push_back("./api/Memory.vm");
    filenames.push_back("./api/Math.vm");
    filenames.push_back("./api/Input.vm");
    filenames.push_back("./api/Output.vm");
    filenames.push_back("./api/Sys.vm");
    filenames.push_back("./api/String.vm");
    filenames.push_back("./api/IO.vm");

    loadProgram();
    run();

    return 0;
}
