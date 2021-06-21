#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    ofstream outfile("../assets/tilemaps/plains.map");

    for (int row = 0; row < 9; row++)
    {
        for (int column = 0; column < 14; column++)
        {
            outfile << to_string(row) << to_string(column) << ", ";
        }

        outfile << endl;
    }

    outfile.close();
}

/*
What is this concept of pointers to pointers?

char ** cArgs
*/