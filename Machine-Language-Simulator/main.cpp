#include <bits\stdc++.h>
#include "VoleMachine.cpp"
using namespace std;

signed main()
{
    MainUI x;
    cout << "Welcome to our program." <<endl;
    string fileName , start;
    while (true)
    {
        x.DisplayMenu();
        string choice = x.inputChoice();
        if (choice == "1")
        {
            fileName = x.inputFileName();
        }
        else if (choice == "2" || choice == "3")
        {
            if (fileName.empty())
            {
                cout << "You must enter the instruction file name first." <<endl;
                continue;
            }

            cout << "Enter an even number from 0 to 128 to start from in the memory:" <<endl;
            while(true)
            {
                getline(cin,start);
                bool isnINT = 0;
                for(auto c : start)
                    if(!isdigit(c)) isnINT = 1;
                if(isnINT || start == "" || stoi(start) < 0 || stoi(start) > 128 || stoi(start)&1)
                {
                    cout << "Invalid input!\nPlease, Enter a choice:\n";
                    continue;
                }
                break;
            }

            int right = 0 , wrong = 0 , counter = stoi(start);
            x.getCPU().setPc(stoi(start));
            x.inputInstructionsFile(fileName , x , right , wrong , counter);
            cout << "Running the program..." <<endl;
            bool stepByStep = (choice == "2");
            x.getCPU().fetch(x.getMemory(),stepByStep);
            if(!stepByStep)
                x.getCPU().displayInfo(x.getMemory(),x.getMemory().getCellM(x.getPc()-2) + x.getMemory().getCellM(x.getPc()));
            cout << "Accepted instructions = " << right <<endl;
            cout << "Refused instructions = " << wrong <<endl;
            cout << "------------------------------------\n";
            x.reset();
        }
        else
        {
            cout << "Thank you for using our program." <<endl;
            return 0;
        }
    }

}
