#include <bits\stdc++.h>
#include <string>

using namespace std;

class Memory {
private:
    vector <string> memory;
public:
    Memory();
    ~Memory();
    void setCellM(int address, string val);
    string getCellM(int address);
};

class Register {
private:
    vector <string> reg;
public:
    Register();
    ~Register();
    void setCellR(int address, string val);
    string getCellR(int address);
};

class ALU {
private:
    string hex = "0123456789ABCDEF";
public:
    string hexToDec(string hexa);
    string decToHex(string decimal);
    string binToDec(string bin);
    string decToBin(string decimal);
    float binToFloat(string bin);
    string floatToBin(float num);
    string eraseLeftZeros(string s);
    int countExponent(string bin, string frac);
    bool isValid(string &instruction);
    void add5(int idx1, int idx2, int storeidx, Register &R);
    void add6(int idx1, int idx2, int storeidx, Register &R);
    void bitwiseAND(int idx1, int idx2, int storeidx, Register &R);
    void bitwiseXOR(int idx1, int idx2, int storeidx, Register &R);
    void bitwiseOR(int idx1, int idx2, int storeidx, Register &R);
    void rotateBits(int idx , int shiftVal , Register &R);
};

class CU{
private:
    ALU alu;
public:
    void load(int idxReg , int idxMem , Register &R , Memory &M);
    void load(int idxReg, string val , Register &R);
    void store(int idxReg, int idxMem, Register &R, Memory &M);
    void Move(int idxReg1, int idxReg2, Register &R);
    void jump(int idxReg, int idxMem, Register &R , int &PC, char op_code);
};

class CPU{
private:
    int programCounter;
    int excutedInstructions;
    string instructionRegister;
    Register reg;
    ALU alu;
    CU cu;
public:
    CPU();
    ~CPU();
    void setPc(int idx);
    void setIr(string instruction);
    int& getPc();
    string& getIr();
    Register& getRegister();
    ALU& getAlu();
    CU& getCu();
    void fetch(Memory &M, bool dis);
    void decode(string instruction, Memory &M, bool dis);
    void execute(Register &R, Memory &M, char op_code, string operand, bool dis);
    void getIns(Register &R, Memory &M, char op_code, string operand);
    void displayInfo(Memory M, string instruction);
};

class Machine{
protected:
    CPU processor;
    Memory ram;
public:
    Memory& getMemory();
    CPU& getCPU();
};

class MainUI{
private:
    Machine machine;
public:
    Machine& getMachine();
    int& getPc();
    string& getIr();
    Register& getRegister();
    ALU& getAlu();
    CU& getCu();
    Memory& getMemory();
    CPU& getCPU();
    void DisplayMenu();
    string inputFileName();
    void inputInstructionsFile(string file, MainUI &x, int &right, int &wrong, int &counter);
    string inputChoice();
    void reset();
};