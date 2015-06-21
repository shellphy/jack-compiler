#include <iostream>
#include <cstdlib>
#include "Parser.h"

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

	return 0;
}
