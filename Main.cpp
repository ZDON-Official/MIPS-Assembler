#include "assembler.cpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    assembler assemble;

    ifstream infile{argv[1]};
    string input_filename = argv[1];
    string output_filename = argv[2];

    vector<string> instructions;

    // check if file is not empty
    if (!infile)
    {
        cerr << "could not open file: " << argv[1] << endl;
        exit(1);
    }
    else if (argc != 3)
    {
        // checks for CL prompt: ./assemble [input file] [output file]
        cerr << "Usage: " << argv[0] << " [input file] [output file]" << endl;
        exit(1);
    }

    infile.close();

    // run the program
    cout << "WE OFFICIALLY START TESTING THE PROGRAM" << endl;

    // stores all the instruction for output file
    instructions = assemble.run(input_filename);
    int instruction_num;

    // Write to output file here
    ofstream outfile;

    outfile.open(output_filename, ios::binary);
    outfile.clear();
    cout << "WRITING TO OUTPUT FILE" << endl;

    for (size_t i = 0; i < instructions.size(); i++)
    {
        string tmp = instructions[i];

        cout << assemble.testBintoHex(tmp);

        instruction_num = assemble.binConv(tmp);

        outfile.write((char *)&instruction_num, sizeof(int));
    }

    outfile.close();

    return 0;
}