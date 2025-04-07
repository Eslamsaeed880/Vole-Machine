#include "A1-Task4-S21-20230021_20230444_20230060.h"
using namespace std;

// Memory
Memory::Memory() : memory(256,"00") {}

Memory::~Memory() {}

string Memory::getCellM(int address)
{
    return memory[address];
}

void Memory::setCellM(int address, string val)
{
    memory[address] = val;
}

// Register
Register::Register():reg(16 , "00"){}

Register::~Register() {}

void Register::setCellR(int address, string val)
{
    reg[address] = val;
}

string Register::getCellR(int address)
{
    return reg[address];
}

// ALU
string ALU::hexToDec(string hexa)
{
    int decimal = 0;
    int base = 1;

    reverse(hexa.begin(), hexa.end());

    for(auto i : hexa)
    {

        decimal += base * hex.find(i);
        base *= 16;
    }
    return to_string(decimal);
}

string ALU::decToHex(string decimal)
{
    int decimal2 = min(stoi(decimal), 255);
    string hexa;

    while(decimal2 != 0)
    {
        hexa += hex[decimal2 % 16];
        decimal2 /= 16;
    }

    reverse(hexa.begin(), hexa.end());

    if(hexa.size() < 2)
    {
        if(hexa == "")
        {
            return "00";
        }
        else
        {
            return "0" + hexa;
        }
    }

    return hexa;
}

bool ALU::isValid(string &instruction)
{
    for(auto &i : instruction)
    {
        i = toupper(i);
    }

    if((isdigit(instruction[0]) && (instruction[0] > '9' || instruction[0] < '1')) || (isalpha(instruction[0]) && (instruction[0] > 'D' || instruction[0] < 'A')) || instruction.size() != 4)
    {
        return false;
    }
    if((instruction[0] == 'C' && instruction != "C000"))
    {
        return false;
    }
    if(instruction[0] == '4' && instruction[1] != '0')
    {
        return false;
    }
    if(instruction[0] == 'A' && instruction[2] != '0')
    {
        return false;
    }

    for(auto i : instruction)
    {
        if(!isdigit(i) && !isalpha(i))
        {
            return false;
        }
        else if(!isdigit(i))
        {
            if(i > 'F' || i < 'A')
                return false;
        }
    }
    return true;
}

string ALU::decToBin(string dec)
{
    int decimal = min(stoi(dec), 255);

    string bin;

    while(decimal != 0)
    {
        bin += to_string(decimal % 2);
        decimal /= 2;
    }
    while(bin.size() < 8)
    {
        bin += "0";
    }

    reverse(bin.begin(), bin.end());
    return bin;
}

string ALU::binToDec(string bin)
{
    int decimal = 0, counter = 0;
    reverse(bin.begin(), bin.end());
    for(auto i : bin)
    {
        decimal += (i - '0')* pow(2, counter++);
    }

    return to_string(decimal);
}

float ALU::binToFloat(string bin)
{
    float fl = 0.0;
    int count = -1;
    for(int i = 0; i < bin.size(); i++)
    {
        fl += (bin[i] - '0') * pow(2, count--);
    }
    return fl;
}

void ALU::add5(int idx1, int idx2, int storeidx, Register &R)
{
    int decimal1 = stoi(hexToDec(R.getCellR(idx1))), decimal2 = stoi(hexToDec(R.getCellR(idx2)));
    if((decimal1 >> 7) & 1)
    {
        decimal1 -= 256;
    }
    if((decimal2 >> 7) & 1)
    {
        decimal2 -= 256;
    }
    int res = decimal1 + decimal2;
    if(res >> 7 & 1)
    {
        res += 256;
    }

    string hexa = decToHex(to_string(res));
    R.setCellR(storeidx, hexa);
}

string ALU::floatToBin(float num)
{
    string s;
    int precision = 8;
    while (num > 0 && precision--)
    {
        num *= 2;
        if (num >= 1.0)
        {
            s += "1";
            num -= 1.0;
        }
        else
        {
            s += "0";
        }
    }
    return s;
}

string ALU::eraseLeftZeros(string s)
{
    bool b = false;
    string s2;
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] == '1')
        {
            b = true;
        }
        if(b)
        {
            s2 += s[i];
        }
    }
    if(s.empty())
    {
        s = "0";
    }
    return s2;
}

int ALU::countExponent(string bin, string frac)
{
    if(bin.empty() || bin == "0")
    {
        for(int i = 0; i < frac.size(); i++)
        {
            if(frac[i] == '1')
            {
                return -i;
            }
        }
    }
    else
    {
        return bin.size();
    }
    return 0;
}

void ALU::add6(int idx1, int idx2, int storeidx, Register &R)
{
    string decimal1 = hexToDec(R.getCellR(idx1)), decimal2 = hexToDec(R.getCellR(idx2)), decimal3;
    string hex3;
    string bin1 = decToBin(decimal1), bin2 = decToBin(decimal2), binint3, fracPointBin3, bin3;
    int sign1 = (bin1[0] == '1' ? -1 : 1), sign2 = (bin2[0] == '1' ? -1 : 1), sign3;
    string frexp1, frexp2, frexp3, frmantissa1, frmantissa2, frmantissa3;
    int exp1, exp2, exp3;
    float mantissa1, mantissa2;
    float fl1, fl2, fl3, fracPoint;
    int intpoint;

    for(int i = 1; i < 4; i++)
    {
        frexp1 += bin1[i];
        frexp2 += bin2[i];
    }
    for(int i = 4; i < 8; i++)
    {
        frmantissa1 += bin1[i];
        frmantissa2 += bin2[i];
    }

    exp1 = stoi(binToDec(frexp1));
    exp2 = stoi(binToDec(frexp2));
    mantissa1 = binToFloat(frmantissa1);
    mantissa2 = binToFloat(frmantissa2);

    fl1 = mantissa1 * sign1 * pow(2,exp1 - 4);
    fl2 = mantissa2 * sign2 * pow(2,exp2 - 4);

    fl3 = fl1 + fl2;
    sign3 = (fl3 < 0 ? -1 : 1);
    fl3 *= (float)sign3;
    fl3 = (fl3 > 7.5 ? 7.5 : fl3);
    intpoint = (int)fl3;
    fracPoint = fl3 - intpoint;
    binint3 = decToBin(to_string(intpoint));
    fracPointBin3 = floatToBin(fracPoint);
    binint3 = eraseLeftZeros(binint3); //erase left zeros
    bin3 = binint3 + fracPointBin3;
    bin3 = eraseLeftZeros(bin3); // erase left zeros
    cout << fl3 << ' ' << binint3 << ' ' << fracPointBin3 << endl;
    exp3 = countExponent(binint3, fracPointBin3);
    exp3 += 4;
    if(exp3 > 7)
    {
        exp3 = 7;
    }
    frmantissa3 = bin3;
    frexp3 = decToBin(to_string(exp3));
    frexp3 = eraseLeftZeros(frexp3); // erase left zeros
    while(frmantissa3.size() < 4)
    {
        frmantissa3 += "0";
    }
    while(frmantissa3.size() > 4)
    {
        frmantissa3.pop_back();
    }
    while(frexp3.size() < 3)
    {
        frexp3 = "0" + frexp3;
    }
    bin3 = (sign3 == -1 ? "1" : "0") + frexp3 + frmantissa3;
    decimal3 = binToDec(bin3);
    hex3 = decToHex(decimal3);
    R.setCellR(storeidx, hex3);
}

void ALU::bitwiseAND(int idx1, int idx2, int storeidx, Register &R)
{
    string hexa;
    string hexa1 = R.getCellR(idx1);
    string hexa2 = R.getCellR(idx2);
    int decimal;
    string decimal1 = hexToDec(hexa1);
    string decimal2 = hexToDec(hexa2);

    decimal = (stoi(decimal1) & stoi(decimal2));
    hexa = decToHex(to_string(decimal));
    R.setCellR(storeidx, hexa);
}

void ALU::bitwiseXOR(int idx1, int idx2, int storeidx, Register &R)
{
    string hexa;
    string hexa1 = R.getCellR(idx1);
    string hexa2 = R.getCellR(idx2);
    int decimal;
    string decimal1 = hexToDec(hexa1);
    string decimal2 = hexToDec(hexa2);

    decimal = (stoi(decimal1) ^ stoi(decimal2));
    hexa = decToHex(to_string(decimal));
    R.setCellR(storeidx, hexa);
}

void ALU::rotateBits(int idx, int shiftVal , Register &R) {
    string val = R.getCellR(idx);
    auto hexaToBin = [](char dec)-> string {
        string bin = "";
        int num = dec-'0';
        if (num > 9)
            num-=7;
        while (num)
        {
            if (num&1)
                bin = '1' + bin;
            else bin = '0' + bin;
            num >>= 1;
        }
        while (bin.size() < 4)
            bin = '0' + bin;
        return bin;
    };
    string valBin = "";
    for (auto i : val)
        valBin+=hexaToBin(i);
    string shiftedValBin = string(valBin.size() , '0');
    for (int i = 0; i < valBin.size(); ++i) {
        shiftedValBin[(i+shiftVal)%valBin.size()] = valBin[i];
    }
    auto binToHexa = [](string b)-> string{
        string hexa = "";
        for (int i = 0; i < b.size(); i+=4) {
            string bit = b.substr(i , 4);
            reverse(bit.begin() , bit.end());
            int dec = 0;
            for (int j = 0; j < bit.size(); ++j) {
                if (bit[j] == '1')
                    dec += (1 << j);
            }
            if (dec < 10)
                hexa += to_string(dec);
            else
                hexa += char(dec-10+65);
        }
        return hexa;
    };
    string hexaShiftedVal = binToHexa(shiftedValBin);
    R.setCellR(idx , hexaShiftedVal);
}

void ALU::bitwiseOR(int idx1, int idx2, int storeidx, Register &R) {
    string hexa;
    string hexa1 = R.getCellR(idx1);
    string hexa2 = R.getCellR(idx2);
    int decimal;
    string decimal1 = hexToDec(hexa1);
    string decimal2 = hexToDec(hexa2);

    decimal = (stoi(decimal1) | stoi(decimal2));
    hexa = decToHex(to_string(decimal));
    R.setCellR(storeidx, hexa);
}

// CU
void CU::store(int idxReg, int idxMem, Register &R, Memory &M) {
    M.setCellM(idxMem , R.getCellR(idxReg));
}

void CU::Move(int idxReg1, int idxReg2, Register &R) {
    R.setCellR(idxReg2 , R.getCellR(idxReg1));
}

void CU::load(int idxReg, int intMem, Register &R, Memory &M)
{
    R.setCellR(idxReg,M.getCellM(intMem));
}

void CU::load(int idxReg, string val, Register &R)
{
    R.setCellR(idxReg,val);
}

void CU::jump(int idxReg, int idxMem, Register &R, int &PC, char op_code)
{
    if(op_code == 'B')
    {
        if(R.getCellR(idxReg) == R.getCellR(0))
            PC = idxMem;
    }
    else if(op_code == 'D')
    {
        if(R.getCellR(idxReg) > R.getCellR(0))
            PC = idxMem;
    }
}

// CPU
CPU::CPU() : programCounter(16) , instructionRegister(""){}

CPU::~CPU() {}

int& CPU::getPc()
{
    return programCounter;
}

string& CPU::getIr()
{
    return instructionRegister;
}

void CPU::setPc(int idx)
{
    programCounter = idx;
}

void CPU::setIr(string instruction)
{
    instructionRegister = instruction;
}

Register& CPU::getRegister()
{
    return reg;
}

void CPU::fetch(Memory &M , bool dis)
{
    string x = M.getCellM(programCounter) , y = M.getCellM(programCounter+1);
    instructionRegister = x + y;
    if(dis)
        displayInfo(M,instructionRegister);
    if(instructionRegister[0] == '0')
        return;
    programCounter += 2;
    decode(instructionRegister,M,dis);
}

void CPU::decode(string instruction , Memory &M, bool dis)
{
    char op_code = instruction[0];
    string operand = instruction.substr(1,3);
    execute(reg,M,op_code,operand,dis);
}

void CPU::execute(Register &R, Memory &M, char op_code , string operand, bool dis)
{
    if(op_code == '1')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cu.load(idxReg,idxMem,R,M);
    }
    else if(op_code == '2')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1)));
        string val = operand.substr(1,2);
        cu.load(idxReg,val,R);
    }
    else if(op_code == '3')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cu.store(idxReg,idxMem,R,M);
        if(operand.substr(1,2) == "00" && !dis)
            getIns(R,M,op_code,operand);
    }
    else if(op_code == '4')
    {
        int idxReg1 = stoi(alu.hexToDec(operand.substr(1,1))) , idxReg2 = stoi(alu.hexToDec(operand.substr(2,1)));
        cu.Move(idxReg1,idxReg2,R);
    }
    else if(op_code == '5')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        alu.add5(idx1,idx2,store,R);
    }
    else if(op_code == '6')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        alu.add6(idx1,idx2,store,R);
    }
    else if(op_code == '7')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        alu.bitwiseOR(idx1,idx2,store,R);
    }
    else if(op_code == '8')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        alu.bitwiseAND(idx1,idx2,store,R);
    }
    else if(op_code == '9')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        alu.bitwiseXOR(idx1,idx2,store,R);
    }
    else if(op_code == 'A')
    {
        int idx = stoi(alu.hexToDec(operand.substr(0,1))) , val = stoi(alu.hexToDec(operand.substr(2,1)));
        alu.rotateBits(idx,val,R);
    }
    else if(op_code == 'B')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cu.jump(idxReg,idxMem,R,programCounter,'B');
    }
    else if(op_code == 'C' || programCounter > 255)
    {
        return;
    }
    else if(op_code == 'D')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cu.jump(idxReg,idxMem,R,programCounter,'D');
    }
    fetch(M,dis);
}

void CPU::displayInfo(Memory M, string instruction) {
    string idx = "0123456789ABCDEF";
    char op_code = instruction[0];
    string operand = instruction.substr(1,3);
    cout << "The Register:" <<endl;
    for (int i = 0; i < 16; ++i) {
        cout << "Register " << idx[i] << ": " << reg.getCellR(i) <<endl;
    }
    cout << "------------------------------------\n";
    cout << "PC = " << programCounter <<endl;
    cout << "IR = " << instructionRegister << " - ";
    getIns(reg,M,op_code,operand);
    cout << "------------------------------------\n";
    cout << "The Memory:" <<endl;
    for (int i = 0; i < 16; ++i) {
        cout << "  " << idx[i];
    }
    cout << endl;
    for (int i = 0; i < 16; ++i) {
        cout << idx[i] << " ";
        for (int j = 0; j < 16; ++j) {
            cout << M.getCellM(j+(16*i)) << ' ';
        }
        cout << endl;
    }
    cout << "------------------------------------\n";
}

ALU &CPU::getAlu()
{
    return alu;
}

CU &CPU::getCu()
{
    return cu;
}

void CPU::getIns(Register &R, Memory &M, char op_code, string operand) {
    if(op_code == '1')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cout << "Copy bits at cell " << operand.substr(1,2) << " to register " << alu.hexToDec(to_string(idxReg)) <<endl;
    }
    else if(op_code == '2')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1)));
        string val = operand.substr(1,2);
        cout << "Copy bit-string " << val << " to register " << alu.hexToDec(to_string(idxReg)) <<endl;
    }
    else if(op_code == '3')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cout << "Copy bits in register " << alu.hexToDec(to_string(idxReg)) << " to cell " << operand.substr(1,2);
        if(operand.substr(1,2) == "00")
            cout << " and print to screen \n";
        if(operand.substr(1,2) == "00")
        {
            cout << "Screen \"cell 00\":\nHexa: " << R.getCellR(stoi(operand.substr(0,1))) << "  ASCII: " << char(stoi(alu.hexToDec(R.getCellR(stoi(operand.substr(0,1)))))) << endl;
        }
    }
    else if(op_code == '4')
    {
        int idxReg1 = stoi(alu.hexToDec(operand.substr(1,1))) , idxReg2 = stoi(alu.hexToDec(operand.substr(2,1)));
        cout << "Copy bits in register " << alu.hexToDec(to_string(idxReg1)) << " to register " << alu.hexToDec(to_string(idxReg2)) <<endl;
    }
    else if(op_code == '5')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        cout << "Add bits in registers " << alu.hexToDec(to_string(idx1)) << " and " << alu.hexToDec(to_string(idx2)) << " (two's-complement), put in " << store <<endl;
    }
    else if(op_code == '6')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        cout << "Add bits in registers " << alu.hexToDec(to_string(idx1)) << " and " << alu.hexToDec(to_string(idx2)) << " (float), put in " << store <<endl;
    }
    else if(op_code == '7')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        cout << "Bitwise OR bits in registers " << alu.hexToDec(to_string(idx1)) << " and " << alu.hexToDec(to_string(idx2)) << ", put in register " << store <<endl;
    }
    else if(op_code == '8')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        cout << "Bitwise AND bits in registers " << alu.hexToDec(to_string(idx1)) << " and " << alu.hexToDec(to_string(idx2)) << ", put in register " << store <<endl;
    }
    else if(op_code == '9')
    {
        int store = stoi(alu.hexToDec(operand.substr(0,1))) , idx1 = stoi(alu.hexToDec(operand.substr(1,1))) , idx2 = stoi(alu.hexToDec(operand.substr(2,1)));
        cout << "Bitwise XOR bits in registers " << alu.hexToDec(to_string(idx1)) << " and " << alu.hexToDec(to_string(idx2)) << ", put in register " << store <<endl;
    }
    else if(op_code == 'A')
    {
        int idx = stoi(alu.hexToDec(operand.substr(0,1))) , val = stoi(alu.hexToDec(operand.substr(2,1)));
        cout << "Rotate bits in register " << alu.hexToDec(to_string(idx)) << "cyclically right " << val << " steps" <<endl;
    }
    else if(op_code == 'B')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cout << "Jump to cell " << operand.substr(1,2) << " if register " << alu.hexToDec(to_string(idxReg)) << " equals register 0" <<endl;
    }
    else if(op_code == 'C' && operand == "000")
    {
        cout << "Halt" <<endl;
    }
    else if(op_code == 'D')
    {
        int idxReg = stoi(alu.hexToDec(operand.substr(0,1))) , idxMem = stoi(alu.hexToDec(operand.substr(1,2)));
        cout << "Jump to cell " << operand.substr(1,2) << " if register " << alu.hexToDec(to_string(idxReg)) << " greater than register 0" <<endl;
    }
    else
    {
        cout << "Opcode is not correct. Halt!" <<endl;
    }
}


// Machine
Memory& Machine::getMemory()
{
    return ram;
}

CPU& Machine::getCPU()
{
    return processor;
}

// MainUI
void MainUI::DisplayMenu()
{
    cout << "Please, Enter a choice:\n"
            "1. Enter the instruction file name.\n"
            "2. Run the program and display step by step.\n"
            "3. Run the program and display at the end.\n"
            "4. Exit\n";
}

void MainUI::reset()
{
    machine.getCPU().setPc(16);
    machine.getCPU().setIr("");
    for (int i = 0; i < 256; ++i)
        machine.getMemory().setCellM(i,"00");
    for (int i = 0; i < 16; ++i)
        machine.getCPU().getRegister().setCellR(i,"00");
}

string MainUI::inputFileName()
{
    ifstream file;
    string fileName;
    while(1)
    {
        cout << "Please, Enter the Instruction file name (without extension): \n";
        getline(cin,fileName);
        fileName+=".txt";
        file.open(fileName , ios::in);
        if(!file.is_open())
        {
            cout << "Error!\n";
            continue;
        }
        else {
            cout << "File loaded successfully." <<endl;
            break;
        }
    }
    return fileName;
}

string MainUI::inputChoice()
{
    string choice;
    cout << "Enter your choice:" <<endl;
    while(true)
    {
        getline(cin,choice);
        bool isnINT = 0;
        for(auto c : choice)
            if(!isdigit(c)) isnINT = 1;
        if(isnINT || choice == "" || stoi(choice) < 1 || stoi(choice) > 4)
        {
            cout << "Invalid input!\nPlease, Enter a choice:\n";
            continue;
        }
        break;
    }
    return choice;
}

Machine& MainUI::getMachine() {
    return machine;
}

int &MainUI::getPc() {
    return machine.getCPU().getPc();
}

string &MainUI::getIr() {
    return machine.getCPU().getIr();
}

Register &MainUI::getRegister() {
    return machine.getCPU().getRegister();
}

ALU &MainUI::getAlu() {
    return machine.getCPU().getAlu();
}

CU &MainUI::getCu() {
    return machine.getCPU().getCu();
}

Memory &MainUI::getMemory() {
    return  machine.getMemory();
}

CPU &MainUI::getCPU() {
    return  machine.getCPU();
}

void MainUI::inputInstructionsFile(string fileName, MainUI &x, int &right, int &wrong, int &counter) {
    ifstream file{fileName};
    string instruction = "";
    bool c = 0;
    while (!file.eof())
    {
        file >> instruction;
        if (x.getAlu().isValid(instruction))
        {
            c = 0;
            if(instruction == "C000")
                c = 1;
            right++;
            x.getMemory().setCellM(counter , instruction.substr(0 , 2));
            x.getMemory().setCellM(counter+1 , instruction.substr(2 , 4));
            counter+=2;
        }
        else wrong++;
    }
    if(!c)
    {
        x.getMemory().setCellM(counter , "C0");
        x.getMemory().setCellM(counter+1 , "00");
    }
}
