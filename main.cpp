#include <iostream>
#include <cstdlib>
#include "Parser.h"
#include "Analyzer.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " <filename>" << endl;
		exit(-1);
	}
	cout << "C-Mini Compiler" << endl << endl;
	Parser parser(argv[1]);
	parser.parse();
	parser.print();
	if (parser.hasError() != true)
	{
		Analyzer analyzer;
		cout << "\n开始语义分析: " << endl;
		analyzer.checkType(parser.getSyntaxTree());
	}

	return 0;
}
