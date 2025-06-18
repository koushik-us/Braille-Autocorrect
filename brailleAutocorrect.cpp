#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

unordered_map<char, int> keyToDot = {
    {'D', 1},
    {'W', 2},
    {'Q', 3},
    {'K', 4},
    {'O', 5},
    {'P', 6}
};

unordered_map<char, int> letterToBraille = {
    {'a', 0b000001},
    {'b', 0b000011},
    {'c', 0b000101},
    {'d', 0b000111},
    {'e', 0b000110},
    {'f', 0b001101},
    {'g', 0b001111},
    {'h', 0b001110},
    {'i', 0b001011},
    {'j', 0b001001},
    {'t', 0b111000}
};

int inputToBraille(const string& input) {
    int mask = 0;
    for(char c : input) {
        c = toupper(c);
        if(keyToDot.count(c)) {
            int bit = keyToDot[c];
            mask |= (1 << (bit-1));
        }
    }
    return mask;
}

vector<int> wordToBrailleVec(const string& word) {
    vector<int> res;
    for(char c : word) {
        if(letterToBraille.count(tolower(c)))
            res.push_back(letterToBraille[tolower(c)]);
        else
            res.push_back(0);
    }
    return res;
}

int simpleDifference(const vector<int>& a, const vector<int>& b) {
    int n = min(a.size(), b.size());
    int diff = 0;
    for(int i = 0; i < n; ++i) {
        if(a[i] != b[i]) diff++;
    }
    diff += abs((int)a.size() - (int)b.size());
    return diff;
}

string suggestWord(
    const vector<vector<int>>& dictionaryBraille,
    const vector<string>& dictionaryWords,
    const vector<int>& userInputBraille
) {
    int minDiff = 1e9;
    string bestWord = "";
    for(size_t i = 0; i < dictionaryBraille.size(); ++i) {
        int diff = simpleDifference(userInputBraille, dictionaryBraille[i]);
        if(diff < minDiff) {
            minDiff = diff;
            bestWord = dictionaryWords[i];
        }
    }
    return bestWord;
}

int main() {
    vector<string> dictionary = {"cat", "bat", "rat", "mat"};
    vector<vector<int>> dictionaryBraille;
    for(const string& w : dictionary)
        dictionaryBraille.push_back(wordToBrailleVec(w));

    cout << "Type your Braille input (e.g. D K|D|D O K for 'cat')\n";
    cout << "Use | as a separator between letters.\n";
    cout << "Input: ";
    string line;
    getline(cin, line);

    vector<int> userBraille;
    size_t pos = 0, next;
    while ((next = line.find('|', pos)) != string::npos) {
        string keys = line.substr(pos, next-pos);
        keys.erase(remove_if(keys.begin(), keys.end(), ::isspace), keys.end());
        userBraille.push_back(inputToBraille(keys));
        pos = next + 1;
    }
    string keys = line.substr(pos);
    keys.erase(remove_if(keys.begin(), keys.end(), ::isspace), keys.end());
    userBraille.push_back(inputToBraille(keys));

    string suggestion = suggestWord(dictionaryBraille, dictionary, userBraille);
    cout << "Did you mean: " << suggestion << "?\n";

    return 0;
}
