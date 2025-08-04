#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
using namespace std;

int main() {
    ifstream fin("02_input.asm");
    ofstream fout("02_output.txt");

    if (!fin) {
        cout << "Cannot open 02_input.asm\n";
        return 1;
    }

    set<string> MOT = {"MOV", "JMP", "ADD", "SUB"};
    vector<string> LT;
    set<string> POT;
    map<string, int> ST;
    vector<string> fileLines;

    string line;
    int lineNum = 0;

    while (getline(fin, line)) {
        fileLines.push_back(line);
    }

    fout << "=========== INPUT FILE CONTENT ===========\n";
    for (const auto& l : fileLines) {
        fout << l << "\n";
    }

    fout << "\n=========== OUTPUT ===========\n";

    fin.clear();
    fin.seekg(0);

    while (getline(fin, line)) {
        lineNum++;
        stringstream ss(line);
        string word;

        ss >> word;
        if (!word.empty() && word.back() == ':') {
            word.pop_back();
            LT.push_back(word);
            ST[word] = lineNum;
            ss >> word;
        }

        for (auto &c : word) c = toupper(c);

        if (word.empty()) continue;

        if (MOT.find(word) == MOT.end()) {
            POT.insert(word);
        }
    }

    fout << "\n--- Label Table (LT) ---\n";
    for (auto &l : LT) fout << l << "\n";

    fout << "\n--- Symbol Table (ST) ---\n";
    for (const auto &s : ST) {
        fout << s.first << " -> Line " << s.second << "\n";
    }

    fout << "\n--- Machine Opcode Table (MOT) ---\n";
    for (auto &m : MOT) fout << m << "\n";

    fout << "\n--- Pseudo Opcode Table (POT) ---\n";
    for (auto &p : POT) fout << p << "\n";

    cout << "Done! Check 02_output.txt\n";
    return 0;
}