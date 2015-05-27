#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

void test(string filename);

int main()
{
    cout << "********************测试整型数********************" << endl;
    test("testInteger.txt");   
    cout << "********************测试浮点数********************" << endl;
    test("testFloat.txt");   
    cout << "********************测试字符型********************" << endl;
    test("testChar.txt");   
    cout << "********************测试标识符********************" << endl;
    test("testIdentifier.txt");   
    cout << "********************测试保留字********************" << endl;
    test("testReservedWords.txt");   
    cout << "********************测试字符串********************" << endl;
    test("testString.txt");   
    
    return 0;
}

void test(string filename)
{
    string cmd;
    string parameter;
    string testFile;
    ifstream fin;
    
    cmd = "..\\C-Mini.exe";
    testFile = filename;
    fin.open(testFile.c_str());
    if (fin.fail())
    {
        cout << "文件" << testFile << "不存在" << endl; 
    }
    else
    {
        cout << "文件内容为: " << endl;
        string txt;
        getline(fin, txt);
        while (fin)
        {
            cout << txt << endl;
            getline(fin, txt);
        }
        fin.close();
        cout << "\ntoken为: " << endl;
        cmd = cmd + " " + testFile;
        system(cmd.c_str());
        ifstream scannerOutput;
        scannerOutput.open("ScannerOutput.txt");
        if (scannerOutput.fail())
        {
            cout << "输出文件不存在" << endl;
        }
        else
        {
            string out;
            while(scannerOutput)
            {
                getline(scannerOutput, out);
                cout << out << endl;
            }
            scannerOutput.close();
        }
    }    
}

