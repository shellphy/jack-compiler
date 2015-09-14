all: jackc.exe jack.exe

jackc.exe: jackc/jackc.o jackc/Scanner.o jackc/Parser.o jackc/SymbolTable.o jackc/Analyzer.o jackc/CodeGen.o jackc/Error.o
	g++ -o jackc.exe jackc/*.o -std=c++11

jackc/jackc.o: jackc/jackc.cpp 
	g++ -c -o jackc/jackc.o jackc/jackc.cpp -std=c++11

jackc/Scanner.o: jackc/Scanner.cpp
	g++ -c -o jackc/Scanner.o jackc/Scanner.cpp -std=c++11
 
jackc/Parser.o: jackc/Parser.cpp
	g++ -c -o jackc/Parser.o jackc/Parser.cpp -std=c++11

jackc/SymbolTable.o: jackc/SymbolTable.cpp
	g++ -c -o jackc/SymbolTable.o jackc/SymbolTable.cpp -std=c++11

jackc/Analyzer.o: jackc/Analyzer.cpp
	g++ -c -o jackc/Analyzer.o jackc/Analyzer.cpp -std=c++11

jackc/CodeGen.o: jackc/CodeGen.cpp
	g++ -c -o jackc/CodeGen.o jackc/CodeGen.cpp -std=c++11

jackc/Error.o: jackc/Error.cpp
	g++ -c -o jackc/Error.o jackc/Error.cpp -std=c++11

jack.exe: jack/jack.o jack/VirtualMachion.o
	g++ -o jack.exe jack/*.o -std=c++11

jack/jack.o: jack/jack.cpp
	g++ -c -o jack/jack.o jack/jack.cpp -std=c++11

jack/VirtualMachion.o: jack/VirtualMachion.cpp
	g++ -c -o jack/VirtualMachion.o jack/VirtualMachion.cpp -std=c++11


clean:
	rm -rf jackc/*.o
	rm -rf jack/*.o
	rm -rf jackc.exe
	rm -rf jack.exe
