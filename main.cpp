#include "Scanner.h"
#include "Parser.h"
#include "Analyzer.h"
#include "CodeGen.h"
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "usage: " << argv[0] << " <filename, filename ... >" << endl;
        exit(-1);
    }
    set<string> filenames;
    for (int i = 1; i < argc; i++)
        filenames.insert(argv[i]);
    if (filenames.find("Main.j") == filenames.end())
    {
        cerr << "Main class not exist" << endl;
        exit(-1);
    }
    Parser parser(filenames);
    parser.parse_program();
    parser.print();
    Analyzer analyzer(parser.getSyntaxTree());
    analyzer.check();
    CodeGen cgen;
    cgen.write(parser.getSyntaxTree());

    return 0;
}
