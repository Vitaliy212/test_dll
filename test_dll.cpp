
#include <iostream>
#include <iostream>
#include <fstream>
#include "Dll1.h"

int main()
{

    char input[] = "";

    ifstream inputFile("json_env_clohtes.json");
    if (!inputFile.is_open()) {
        cerr << "Failed to open file!" << endl;
        return -1;
    }

    string jsonInput((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    const char* jsonInputCStr = jsonInput.c_str();

    const char* result = Main(const_cast<char*>(jsonInputCStr));

    return 0;
}