#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <iostream>
#include <unordered_map>
#include <string.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <iterator>  // make sure to delete this after testing


using namespace std;

class assembler{
    private:
        // TODO - maybe change these to <string, string>
        static unordered_map<string, string> opcode;
        static unordered_map<string, string> reg;
        static unordered_map<string, string> reg_num;
        static unordered_map<string, string> func;
        static unordered_map<string, char> Hex;
        unordered_map<string,int> labelMap;
        int curLine = 1;

        // private methods
        //void createMap(string input_filename); // create label map
        string arith(string line);
        string shift(string line);
        string load_save(string line);
        string syscall(string line);
        string branch(string line);
        string jump(string line);
        string special(string line); // this is for jalr and jr
        string remove_space(string str);
        string DecToBinary(int input, int binNum);
        vector<string> parse(string str);
        vector<string> BinaryToHex(vector<string> input);

    public:
        assembler();
        assembler(string infile);
        ~assembler();
        
        // main methods
        void createMap(string input_filename); // create label map
        vector<string> run(string input_filename); // will write to output file

        
        // test methods
        void clean(string str);
        string testBintoHex(string str);
        //int binConv(string binary);

        // todo - move to private
        // string arith(string line);
        // string shift(string line);
        // string load(string line);
        // string syscall(string line);
        // string branch(string line);
        // string jump(string line);
        // string special(string line); // this is for jalr and jr
        // string remove_space(string str);
        // string DecToBinary(int input, int binNum);
        // vector<string> parse(string str);
        // vector<string> BinaryToHex(vector<string> input);
};

#endif