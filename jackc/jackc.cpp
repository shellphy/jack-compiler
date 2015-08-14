#include "Scanner.h"
#include "Parser.h"
#include "Analyzer.h"
#include "CodeGen.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "usage: " << argv[0] << " <filename, filename ... >" << endl;
        exit(-1);
    }
    ifstream fin;
    vector<string> filenames;
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
    filenames.push_back("./api/Sys.jack");
    filenames.push_back("./api/Math.jack");
    filenames.push_back("./api/Array.jack");
    filenames.push_back("./api/Memory.jack");
    filenames.push_back("./api/String.jack");
    filenames.push_back("./api/Output.jack");
    filenames.push_back("./api/Input.jack");
    filenames.push_back("./api/IO.jack");
    Parser parser(filenames);
    parser.parse_program();
    if (!hasError())
    {
        Analyzer analyzer(parser.getSyntaxTree());
        analyzer.check();
    }
    if (!hasError())
    {
        CodeGen cgen;
        cgen.write(parser.getSyntaxTree());
    }

    return 0;
}
