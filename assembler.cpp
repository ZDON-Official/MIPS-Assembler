#ifndef __ASSEMBLER_CPP__
#define __ASSEMBLER_CPP__

#include "assembler.hpp"
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cmath>

// initialize the maps
// TODO - add to these
unordered_map<string, string> assembler::opcode{
    {"add", "000000"},
    {"addi", "001000"},
    {"sub", "000000"},
    {"mult","000000"},
    {"div","000000"},
    {"mflo", "000000"},
    {"mfhi", "000000"},
    {"sll", "000000"},
    {"srl", "000000"},
    {"slt", "000000"},
    {"lw", "100011"},
    {"sw", "101011"},
    {"beq", "000100"},
    {"bne", "000101"},
    {"j", "000010"},
    {"jal", "000011"},
    {"jr", "000000"},
    {"jalr", "000000"}

};

// TODO - add to these
unordered_map<string, string> assembler::reg{
    {"zero", "00000"},
    {"at", "00001"},
    {"v0", "00010"},
    {"v1", "00011"},
    {"a0", "00100"},
    {"a1", "00101"},
    {"a2", "00110"},
    {"a3", "00111"},
    {"t0", "01000"},
    {"t1", "01001"},
    {"t2", "01010"},
    {"t3", "01011"},
    {"t4", "01100"},
    {"t5", "01101"},
    {"t6", "01110"},
    {"t7", "01111"},
    {"s0", "10000"},
    {"s1", "10001"},
    {"s2", "10010"},
    {"s3", "10011"},
    {"s4", "10100"},
    {"s5", "10101"},
    {"s6", "10110"},
    {"s7", "10111"},
    {"t8", "11000"},
    {"t9", "11001"},
    {"k0", "11010"},
    {"k1", "11011"},
    {"gp", "11100"},
    {"sp", "11101"},
    {"fp", "11110"},
    {"ra", "11111"}
};

unordered_map<string, string> assembler::reg_num{
    {"0", "00000"},
    {"1", "00001"},
    {"2", "00010"},
    {"3", "00011"},
    {"4", "00100"},
    {"5", "00101"},
    {"6", "00110"},
    {"7", "00111"},
    {"8", "01000"},
    {"9", "01001"},
    {"10", "01010"},
    {"11", "01011"},
    {"12", "01100"},
    {"13", "01101"},
    {"14", "01110"},
    {"15", "01111"},
    {"16", "10000"},
    {"17", "10001"},
    {"18", "10010"},
    {"19", "10011"},
    {"20", "10100"},
    {"21", "10101"},
    {"22", "10110"},
    {"23", "10111"},
    {"24", "11000"},
    {"25", "11001"},
    {"26", "11010"},
    {"27", "11011"},
    {"28", "11100"},
    {"29", "11101"},
    {"30", "11110"},
    {"31", "11111"}
};

unordered_map<string, string> assembler::func{
    {"add", "00000100000"},
    {"sub", "00000100010"},
    {"mult","0000000000011000"},
    {"div","0000000000011010"},
    {"mflo","00000010010"},
    {"mfhi", "00000010000"},
    {"slt", "00000101010"},
    {"sll", "000000"},
    {"srl", "000010"},
    {"jalr", "00000001001"},
    {"jr", "00000001000"}
};

unordered_map<string, char> assembler::Hex{
    {"0000",'0'},
    {"0001",'1'},
    {"0010",'2'},
    {"0011",'3'},
    {"0100",'4'},
    {"0101",'5'},
    {"0110",'6'},
    {"0111",'7'},
    {"1000",'8'},
    {"1001",'9'},
    {"1010",'A'},
    {"1011",'B'},
    {"1100",'C'},
    {"1101",'D'},
    {"1110",'E'},
    {"1111",'F'}
};



assembler::assembler(){}
assembler::assembler(string infile){}
assembler::~assembler(){}

// this will be the method that main.cpp will run
void assembler::createMap(string input_filename){
    ifstream Input_file(input_filename);

    //unordered_map<string, int> labelMap;
    string Line;
    string label;
    int lineNum = 0;

    bool start = false;

    while(getline(Input_file, Line)){
        // only start counting from "main:"

        if (false == start){

            if (Line.find("main:") != string::npos || Line.find("syscall:") != string::npos){
                start = true;
            }
            else{
                //cout << "else line -" << line << "-" << endl;
                continue;
            }
        }

        Line = remove_space(Line);

        // if (Line.find("main") != string::npos && Line.find_first_of("main:") == 0){
        //     lineNum = 0;
        // }
        // SLIGHT CHANGE HERE TO MAKE THE LABEL MAP, NOT COUNT FROM "MAIN:, BUT FROM ANY LABEL
        // if (Line.find(":") != string::npos){
        //     lineNum = 0;
        // }
        if(Line.empty()){
            continue;
        } else if(Line.find("#") != string::npos){
            Line = remove_space(Line);
            if(Line.find_first_of("#") == 0){
                continue;
            } 
        }

        // todo - error in line calculation
        size_t found = Line.find(":");
        if (found != string::npos){
            label = Line.substr(0,Line.find(":"));
            labelMap.insert(pair<string,int>(label,lineNum));
            continue;
        }
        lineNum++;
    }

    // TODO - comment or delete this part
    cout << "we finish creating the label map" << endl;
    cout << "this is to test if the map works" << endl;
    unordered_map<string, int>::iterator itr;
    cout << "\nThe map labelMap is : \n";
    cout << "\tKEY\tELEMENT\n";
    for (itr = labelMap.begin(); itr != labelMap.end(); ++itr) {
        cout << '\t' << itr->first << '\t' << itr->second
            << '\n';
    }
    cout << endl;

    Input_file.close();

}


// TODO return vector of instructions
vector<string> assembler::  run(string input_filename){

    cout << "We start running the program" << endl;

    ifstream Input_file(input_filename);

    string line;
    vector<string> output;

    bool start = false;

    while (getline(Input_file, line)){   


        // this is to ignore the first few lines before main: or syscall:
        if(false == start){

            if(line.find("main:") != string::npos || line.find("syscall:") != string::npos){
                start = true;
            } else{
                cout << "else line -" << line << "-" << endl;
                continue;
            }
        }

        if(line.empty()){
            continue;
        } else if(line.find("#") != string::npos){
            //cout << "found a comment" << endl;
            //line = remove_space(line);

            //cout << "line comment - " << line << endl;

            if(line.find_first_of("#") == 0){
                continue; // just a commnet line
            } else{
                // code with comment
                line = line.substr(0, line.find("#")); // remove comment from line
            }
            //cout << "comment removed - " << line << endl;
        } else if (line.find(":") != string::npos){
            //cout << "found a label - " << line << endl;
            continue;
        }

        if (line.find("add") != string::npos || line.find("addi") != string::npos || line.find("sub") != string::npos || line.find("mult") != string::npos || line.find("div") != string::npos || line.find("mflo") != string::npos|| line.find("mfhi") != string::npos|| line.find("slt") != string::npos){
            output.push_back(arith(line));
            curLine++;
        } else if(line.find("beq") != string::npos || line.find("bne") != string::npos){
            output.push_back(branch(line));
            curLine++;
        } else if(line.find("j ")!= string::npos || line.find("jal ") != string::npos){
            cout << line << endl;
            output.push_back(jump(line));
            curLine++;
        } else if(line.find("jalr") != string::npos || line.find("jr") != string::npos){
            output.push_back(special(line));
            curLine++;
        } else if(line.find("sll") != string::npos || line.find("srl") != string::npos){
            output.push_back(shift(line));
            curLine++;
        } else if(line.find("sw") != string::npos || line.find("lw") != string::npos){
            output.push_back(load_save(line)); //! <---------------- change to load_save()
            curLine++;
        } else {
            line = remove_space(line);
            //cout << "not an inst - " << line << endl;

            if(line == "syscall"){
                cout << "syscall -" << line << "-" << endl;

                output.push_back(syscall(line));
                curLine++;
            }
        }
        // else if (line.find("syscall") != string::npos) {
        //     cout << "syscall -" << line << "-" << endl;
        //     // cout << line.compare("syscall") << endl;
        //     // line = remove_space(line);
        //     // size_t pos = line.find("syscall");
        //     // cout << "SYSCALL BUG: pos is " << pos << endl;
        //     // line = line.substr(7);
        //     // cout << "SYSCALL BUG: line is " << line << endl;
        //     // if (line.empty()){
        //     output.push_back(syscall(line));
        //     // cout << "SYSCALL OCCURED YAY!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        //     // }
        //     curLine++;
        // }
    }
    Input_file.close();

    //output = BinaryToHex(output);
    return output;
}


string assembler::syscall(string line){
    return "00000000000000000000000000001100";
}


//TODO - error here
int binConv(string binary)
{
    //cout << "string - " << binary << endl;
    string bin = binary;
	int decimal = 0;
	int size = bin.size();
	
    //cout << "size " << size << endl;
	for (int counter = 0; counter < size; counter++)
	{
        // cout << "counter " << counter << endl;
		if (bin[counter] == '1'){
            // cout << "power " <<  pow(2.0,size-counter-1) << endl;
			decimal = decimal + (1 << (size-counter-1));
            //pow(2.0,size-counter-1));
        }
		else
            // cout << "in else  -  ";
            // cout << "binary[counter] - " << bin[counter] << endl;
			continue;
	}
    
    //1 << 4
	return decimal;
}


//! MAIN -----------------------------------------------
int main(int argc, char const *argv[])
{
    assembler assemble;

    ifstream infile {argv[1]};
    string input_filename = argv[1];
    string output_filename = argv[2];

    vector<string> instructions;

    // check if file is not empty
    if (!infile){
        cerr << "could not open file: " << argv[1] << endl;
        exit(1);
    }
    else if (argc != 3)
    {
        // checks for CL prompt: ./assemble [input file] [initial seed]
        cerr << "Usage: " << argv[0] << " [input file] [output file]" << endl;
        exit(1);
    }
    
    infile.close();
    

    // run the program
    cout << "WE OFFICIALLY START TESTING THE PROGRAM" << endl;
    assemble.createMap(input_filename);

    // stores all the instruction for output file
    instructions = assemble.run(input_filename);
    int instruction_num;
    //instructions = assemble.BinaryToHex(instructions);


    /*
    for(int i=0; i < instructions.size(); i++){
        cout << instructions[i] << endl;
    }
    */

    // Write to output file here
    ofstream outfile;

    outfile.open(output_filename, ios::binary);
    outfile.clear();
    cout << "WE ARE HERE"  << endl;
    
    for (size_t i = 0; i < instructions.size(); i++)
    {
        //outfile << instructions[i] << endl;
        //cout << instructions[i] << endl;
        string tmp = instructions[i];
        //cout << "tmp - " << tmp << endl;
        cout << assemble.testBintoHex(tmp);
        instruction_num = binConv(tmp);
        //this convert the instruction_num into a int
        //cout << "instruction_num - " << instruction_num << endl;
        //outfile << instruction_num << endl;

        outfile.write((char *) &instruction_num, sizeof(int));
    }       
    
    outfile.close();

    return 0;
}

// tester method
string assembler::testBintoHex(string str){

    cout << endl;
    int l = str.size();
    int s = 0;
    string hex;

    while(true){
        hex += Hex.at(str.substr(s, 4));
        //cout << str.substr(s, 4) << endl;

        s += 4;
        if (s == l){
            break;
        }
    }

    return hex;
}

// part of code snippet from geeksforgeeks
vector<string> assembler::BinaryToHex(vector<string> input){
    vector<string> output;
    int length;
    

    for (int i = 0; i < input.size(); i++){
        string hex = "";
        int s = 0;
        length = input[i].size();

        while(true){
            hex += Hex.at(input[i].substr(s, 4));

            s += 4;   
            if(s == length){
                break;
            }         
        }
        output.push_back(hex);
    }
    
    return output;
}

string assembler::branch(string line){
    string instruction;
    string rs;
    string rt;
    string offset;
    int label;
    int diff;

    line = remove_space(line);

    string key = line.substr(0, line.find("$"));
    string opc = opcode.at(key);
    line = line.substr(line.find_first_of("$"));


    vector<string> var = parse(line); 
    vector<string> regis(3, "");

    // loop through the var vector
    for (size_t i = 0; i < var.size(); i++){
        // is a register
        if(var.at(i).find("$") != string::npos){
            string tmp = var.at(i).substr(var.at(i).find_first_not_of("$"));
            if(reg.find(tmp) != reg.end()){
                // key in reg
                regis[i] = reg.at(tmp);

            } else if(reg_num.find(tmp) != reg_num.end()){
                // key in reg_num
                regis[i] = reg_num.at(tmp);
            }
        } 

    }

    rs = regis.at(0);
    rt = regis.at(1);

    string labelKey = var.at(2);
    label = labelMap.at(labelKey);
    diff = (label) - (curLine);

    offset = DecToBinary(diff, 16);

    instruction = opc +  rs  + rt  + offset;

    return instruction;
}

// error in here
string assembler::jump(string line){
    string instruction;
    string target;
    int label;
    string labelKey;

    vector<string> content;

    stringstream ss(line);

    //cout << "inside jumps - " << line << endl;

    while (ss.good()){
        string substr;
        getline(ss, substr, ' ');
        if(substr.empty()){
            continue;
        }
        else{
        content.push_back(substr);
        }
    }

    string key =content.at(0);
    string opc = opcode.at(key);
    labelKey = content.at(1);

    labelKey = remove_space(labelKey);
    label = labelMap.at(labelKey);

    target = DecToBinary(label,26);

    instruction = opc + target;

    return instruction;
}

string assembler::special(string line){
    string instruction;
    string rs;
    string rd;
    

    line = remove_space(line);

    string key = line.substr(0, line.find("$"));
    string opc = opcode.at(key);
    string function = func.at(key);
    line = line.substr(line.find_first_of("$"));

    vector<string> var = parse(line); 
    vector<string> regis(3, "");

    // loop through the var vector
    for (size_t i = 0; i < var.size(); i++){
        // is a register
        if(var.at(i).find("$") != string::npos){
            // removes the $ sign
            //! not neccesary as we can add the $ in out unordered_map
            string tmp = var.at(i).substr(var.at(i).find_first_not_of("$"));
            if(reg.find(tmp) != reg.end()){
                // key in reg
                regis[i] = reg.at(tmp);
            } else if(reg_num.find(tmp) != reg_num.end()){
                // key in reg_num
                regis[i] = reg_num.at(tmp);
            }
        }
    }

    if(key.compare("jr") == 0){
        rs = regis.at(0);
        rd = "00000";
    }
    else if(key.compare("jalr") == 0){
        rs = regis.at(0);
        rd = regis.at(1);
    }

    instruction = opc  + rs  + "00000" + rd + function;
    
    return instruction;
}

string assembler::load_save(string line){
    string instruction;
    string base;
    string rt;
    string offset;
    vector<string> regis(2, "");

    //cout << line << " - insreuction" << endl;

    line = remove_space(line);

    string key = line.substr(0, line.find("$"));
    string opc = opcode.at(key);
    
    line = line.substr(line.find_first_of("$"));

    vector<string> var = parse(line);

    for (size_t i = 0; i < var.size(); i++){
        if (var.at(i).find("$") != string::npos){
            string tmp = var.at(i).substr(var.at(i).find_first_of("$")+1);
            if(tmp.find(")") != string::npos){
                tmp.pop_back();
            }
            if (reg.find(tmp) != reg.end()){
                // key in reg
                regis[i] = reg.at(tmp);
            }
            else if (reg_num.find(tmp) != reg_num.end())
            {
                // key in reg_num
                regis[i] = reg_num.at(tmp);
            }
        }
    }

    rt = regis[0];

    offset = var.at(1);
    offset = offset.substr(0, offset.find("("));
    offset = DecToBinary(stoi(offset), 16);

    base = regis[1];

    // TODO - remove spaces
    instruction = opc + base + rt + offset;

    //! ----------------------------------------------------------
    //cout << "instruction - " << instruction << endl;
    //! ----------------------------------------------------------

    return instruction;
}


string assembler::shift(string line){
    string instruction;
    string shamt;
    string rt;
    string rd;
    string function;

    // removes all spaces
    line = remove_space(line);

    string key = line.substr(0, line.find("$"));
    string opc = opcode.at(key);
    line = line.substr(line.find_first_of("$"));


    vector<string> var = parse(line); 
    vector<string> regis(3, "");

    for (size_t i = 0; i < var.size(); i++){
        // is a register
        if(var.at(i).find("$") != string::npos){
            string tmp = var.at(i).substr(var.at(i).find_first_not_of("$"));
            if(reg.find(tmp) != reg.end()){
                // key in reg
                regis[i] = reg.at(tmp);
            } else if(reg_num.find(tmp) != reg_num.end()){
                // key in reg_num
                regis[i] = reg_num.at(tmp);
            }
        } else{
            // convert to 16 bit binary
            shamt = DecToBinary(stoi(var.at(i)), 5);
        }
    }
    // for sll && srl
    rt = regis.at(1);
    rd = regis.at(0);
    function = func.at(key);
    instruction = opc +  "00000" + rt + rd + shamt + function;

    return instruction;
}

string assembler::arith(string line) {
    string instruction;
    string imm;
    string rt;
    string rs;
    string rd;
    string function;

    // removes all spaces
    line = remove_space(line);

    string key = line.substr(0, line.find("$"));
    string opc = opcode.at(key);
    line = line.substr(line.find_first_of("$"));

    vector<string> var = parse(line); 
    vector<string> regis(3, "");

    // loop through the var vector
    for (size_t i = 0; i < var.size(); i++){
        //cout << var.at(i) << endl;

        // is a register
        if(var.at(i).find("$") != string::npos){
            string tmp = var.at(i).substr(var.at(i).find_first_not_of("$"));
            //cout << "add instruction tmp " << tmp << endl;
            if(reg.find(tmp) != reg.end()){
                // key in reg
                regis[i] = reg.at(tmp);
            } else if(reg_num.find(tmp) != reg_num.end()){
                // key in reg_num
                regis[i] = reg_num.at(tmp);
            }
        } else{
            // convert to 16 bit binary
            imm = DecToBinary(stoi(var.at(i)), 16);
        }
    }



    if(key.compare("addi") == 0){
        rs = regis.at(1);
        rt = regis.at(0);

        instruction = opc + rs + rt + imm;
    } else if(key.compare("add") == 0){
        rd = regis.at(0);  
        rt = regis.at(2);   
        rs = regis.at(1);  
        function = func.at(key);

        instruction = opc + rs + rt + rd + function; 
        //cout << "add instruction " << instruction << endl;
        //cout << "add instruction rs" << rs <<endl;
        //cout << "add instruction rt" << rt <<endl;
        //cout << "add instruction rd" << rd <<endl;
        //cout << "add instruction func" << function <<endl;

    } else if(key.compare("sub") == 0){
        rd = regis.at(0); 
        rt = regis.at(2); 
        rs = regis.at(1); 
        function = func.at(key);

        instruction = opc + rs + rt + rd + function;
    } else if(key.compare("mult") == 0){
        rs = regis.at(0);
        rt = regis.at(1);
        function = func.at(key);
        instruction = opc + rs + rt + function;
    } else if(key.compare("div") == 0){
        rs = regis.at(0);   // regis[0]
        rt = regis.at(1);   // regis[1]
        function = func.at(key);
        instruction = opc + rs + rt + function;
    }

    //for mfhi && mflo
    else if(key.compare("mflo") == 0){
        rs = "00000";
        rt = "00000";
        rd = regis.at(0);
        function = func.at(key);
        instruction = opc + rs  + rt + rd + function;
    }
    else if (key.compare("mfhi") == 0){
        rs = "00000";
        rt = "00000";
        rd = regis.at(0);
        function = func.at(key);
        instruction = opc + rs + rt + rd + function;
    }

    // This is slt
    else if(key.compare("slt") == 0){
        rs = regis.at(1);
        rt = regis.at(2);
        rd = regis.at(0);
        function = func.at(key);
        instruction = opc + rs  + rt + rd + function;
    }

    return instruction;
}

// found online
string assembler::remove_space(string str){
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}
 // also found online
vector<string> assembler::parse(string str){
    vector<string> temp;

    stringstream ss(str);

    while (ss.good()){
        string substr;
        getline(ss, substr, ',');
        temp.push_back(substr);
    }
    
    return temp;
}


// from GeeksforGeeks, added binNUM as a parameter
string assembler::DecToBinary(int n, int binNum){

    string binaryOutput;

    for (int i = binNum-1; i >= 0; i--)
    {
        int k = n >> i;
        if (k & 1)
            binaryOutput += "1";
        else
            binaryOutput += "0";
    }

    return binaryOutput;
}



void assembler::clean(string str){
    string line = str;

    if (line.find("#") != string::npos){

        line = remove_space(line);

        if (line.find_first_of("#") == 0){
            cout << "just a comment" << endl;
            //continue; // just a commnet line
        }
        else{
            // code with comment
            line = line.substr(0, line.find("#")); // remove comment from line
        }
    }
    cout << "str _" << line << "_" <<endl; 
}


#endif