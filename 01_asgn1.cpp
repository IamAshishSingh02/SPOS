#include <bits/stdc++.h>
using namespace std;

string removeCommentsWithRegex(const string &code) {
    regex singleLineComment(R"(//.*)");
    regex multiLineComment(R"(/\*[\s\S]*?\*/)");
    string noBlockComments = regex_replace(code, multiLineComment, "");
    string noComments = regex_replace(noBlockComments, singleLineComment, "");
    return noComments;
}

int main() {
    string inputFile = "input.cpp";
    string outputFile = "output.cpp";

    ifstream in(inputFile);
    if (!in.is_open()) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    stringstream buffer;
    buffer << in.rdbuf();
    string code = buffer.str();
    in.close();

    cout << "Input code:\n" << code << endl;

    string cleanCode = removeCommentsWithRegex(code);

    cout << "Code after removing comments:\n" << cleanCode << endl;

    ofstream out(outputFile);
    if (!out.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }

    out << cleanCode;
    out.close();

    cout << "Comments removed using regex and output written to " << outputFile << endl;
    return 0;
}