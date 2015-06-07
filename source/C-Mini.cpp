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
	auto parser = Parser::getInstance(argv[1]);

	return 0;
}
