#include <iostream>
#include <cstdlib>
#include "Scanner.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " <filename>" << endl;
		exit(-1);
	}
	auto scanner = Scanner::getInstance(argv[1]);
	scanner->output();

	return 0;
}
