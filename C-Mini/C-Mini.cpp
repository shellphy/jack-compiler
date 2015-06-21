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
	auto parser = Parser::getInstance();
	parser->parse(argv[1]);
	parser->print();

	return 0;
}
