#include <bits/stdc++.h>
using namespace std;

struct MacroProcessor {
    unordered_map<string,int> MNT;
    vector<string> MDT;
    vector<string> ALA_def, ALA_call;

    static vector<string> tokenize(const string &s, char delim=',') {
        vector<string> res;
        string token; 
        stringstream ss(s);
        while(getline(ss, token, delim)) {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            if(!token.empty()) res.push_back(token);
        }
        return res;
    }

    static vector<string> splitSpace(const string &s) {
        vector<string> res;
        stringstream ss(s);
        string t; while(ss >> t) res.push_back(t);
        return res;
    }

    string replaceParams(const string &line, const unordered_map<string,int> &map) {
        string res; 
        for(size_t i=0; i < line.size();) {
            if(isalnum(line[i]) || line[i]=='_') {
                size_t j=i; 
                while(j < line.size() && (isalnum(line[j])||line[j]=='_')) ++j;
                string w=line.substr(i,j-i);
                if(map.count(w)) res+="#"+to_string(map.at(w));
                else res+=w;
                i=j;
            } else res+=line[i++];
        }
        return res;
    }

    string substitute(const string &line, const vector<string> &actual) {
        string res=line;
        for(size_t i=0; i < actual.size(); ++i) {
            string ph = "#" + to_string(i);
            size_t pos=0;
            while((pos = res.find(ph, pos)) != string::npos) {
                res.replace(pos, ph.size(), actual[i]);
                pos += actual[i].size();
            }
        }
        return res;
    }

    void pass1(const string &fname) {
        ifstream fin(fname);
        ofstream ic("03_intermediate.txt");
        string line; bool inMacro=false;
        unordered_map<string,int> ALA_map;

        while(getline(fin,line)) {
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            if(line=="MACRO") {
                inMacro=true;
                getline(fin,line);
                auto tokens=splitSpace(line);
                string name=tokens[0];
                string params_str=line.substr(name.size());
                ALA_def=tokenize(params_str);
                ALA_map.clear();
                for(int i=0; i<(int)ALA_def.size(); ++i) ALA_map[ALA_def[i]]=i;
                MNT[name]=(int)MDT.size();
                continue;
            }
            if(line=="MEND") {
                MDT.push_back("MEND");
                inMacro=false;
                continue;
            }
            if(inMacro) MDT.push_back(replaceParams(line, ALA_map));
            else ic << line << "\n";
        }
    }

    void pass2() {
        ifstream ic("03_intermediate.txt");
        ofstream fout("03_expanded_code.asm");
        string line;
        while(getline(ic,line)) {
            if(line.empty()) continue;
            auto tokens=splitSpace(line);
            if(MNT.count(tokens[0])) {
                string args=line.substr(tokens[0].size());
                ALA_call=tokenize(args);
                int idx=MNT[tokens[0]];
                while(MDT[idx]!="MEND") {
                    fout << substitute(MDT[idx++], ALA_call) << "\n";
                }
            } else {
                fout << line << "\n";
            }
        }
    }
};

int main() {
    MacroProcessor mp;
    mp.pass1("03_input.asm");
    mp.pass2();
    cout << "Expansion done, output in 03_expanded_code.asm\n";
}