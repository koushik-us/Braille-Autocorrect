#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

unordered_map<char, int> qwertyToDot = {
    {'D', 1},
    {'W', 2},
    {'Q', 3},
    {'K', 4},
    {'O', 5},
    {'P', 6}
};

unordered_map<char, int> charToBrailleCode = {
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

int parseBrailleInput(const string& brailleKeys) {
    int brailleMask = 0;
    for(char ch : brailleKeys) {
        ch = toupper(ch);
        if(qwertyToDot.count(ch)) {
            int bitPosition = qwertyToDot[ch];
            brailleMask |= (1 << (bitPosition - 1));
        }
    }
    return brailleMask;
}

vector<int> convertWordToBraille(const string& word) {
    vector<int> brailleVector;
    for(char ch : word) {
        if(charToBrailleCode.count(tolower(ch)))
            brailleVector.push_back(charToBrailleCode[tolower(ch)]);
        else
            brailleVector.push_back(0);
    }
    return brailleVector;
}

int calculateDifference(const vector<int>& a, const vector<int>& b) {
    int len = min(a.size(), b.size());
    int difference = 0;
    for(int i = 0; i < len; ++i) {
        if(a[i] != b[i]) difference++;
    }
    difference += abs((int)a.size() - (int)b.size());
    return difference;
}

string findClosestMatch(
    const vector<vector<int>>& brailleDictionary,
    const vector<string>& wordDictionary,
    const vector<int>& userBrailleInput
) {
    int smallestDifference = 1e9;
    string closestWord = "";
    for(size_t i = 0; i < brailleDictionary.size(); ++i) {
        int diff = calculateDifference(userBrailleInput, brailleDictionary[i]);
        if(diff < smallestDifference) {
            smallestDifference = diff;
            closestWord = wordDictionary[i];
        }
    }
    return closestWord;
}

int main() {
    vector<string> wordList = {"cat", "bat", "rat", "mat"};
    vector<vector<int>> brailleEncodings;
    for(const string& word : wordList)
        brailleEncodings.push_back(convertWordToBraille(word));

    cout << "Type your Braille input (e.g. D K|D|D O K for 'cat')\n";
    cout << "Use | as a separator between letters.\n";
    cout << "Input: ";
    string inputLine;
    getline(cin, inputLine);

    vector<int> userInputBraille;
    size_t start = 0, delim;
    while ((delim = inputLine.find('|', start)) != string::npos) {
        string letterKeys = inputLine.substr(start, delim - start);
        letterKeys.erase(remove_if(letterKeys.begin(), letterKeys.end(), ::isspace), letterKeys.end());
        userInputBraille.push_back(parseBrailleInput(letterKeys));
        start = delim + 1;
    }
    string lastLetter = inputLine.substr(start);
    lastLetter.erase(remove_if(lastLetter.begin(), lastLetter.end(), ::isspace), lastLetter.end());
    userInputBraille.push_back(parseBrailleInput(lastLetter));

    string suggestion = findClosestMatch(brailleEncodings, wordList, userInputBraille);
    cout << "Did you mean: " << suggestion << "?\n";

    return 0;
}

