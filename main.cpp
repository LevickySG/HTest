#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <stdexcept>
#include <utility>

using namespace std;

pair<map<string, int>, size_t> parseElements(const string& input, size_t& pos) {
    map<string, int> elements;
    
    while (pos < input.length()) {
        char ch = input[pos];
        
        // processing of open brackets
        if (ch == '(' || ch == '[') {
            char openBracket = ch;
            pos++; // skip opening bracket position
            
            // prosessing of elements inside brackets
            auto [innerElements, newPos] = parseElements(input, pos);
            
            // Checking for closing bracket
            if (pos >= input.length() || 
                (openBracket == '(' && input[pos] != ')') || 
                (openBracket == '[' && input[pos] != ']')) {
                throw runtime_error("Missing closing bracket");
            }
            
            pos++; // skip closing bracket position
            
            // processing of multiplier after brackets
            int multiplier = 1;
            if (pos < input.length() && isdigit(input[pos])) {
                string numStr;
                while (pos < input.length() && isdigit(input[pos])) {
                    numStr += input[pos++];
                }
                
                try {
                    multiplier = stoi(numStr);
                    if (multiplier <= 0) {
                        throw invalid_argument("Multiplier must be positive");
                    }
                } catch (const invalid_argument&) {
                    throw invalid_argument("Invalid multiplier after brackets");
                }
            }
            
            // multiplication of elements inside brackets
            for (auto& [name, count] : innerElements) {
                elements[name] += count * multiplier;
            }
            
        } 
        // processing of closing brackets
        else if (ch == ')' || ch == ']') {
            // Завершаем текущий уровень рекурсии
            return {elements, pos};
        } 
        // processing of element
        else if (isupper(ch)) {
            string element;
            element += input[pos++];
            
            while (pos < input.length() && islower(input[pos])) {
                element += input[pos++];
            }
            
            // parsing count of element
            int count = 1;
            if (pos < input.length() && isdigit(input[pos])) {
                string numStr;
                while (pos < input.length() && isdigit(input[pos])) {
                    numStr += input[pos++];
                }
                
                try {
                    count = stoi(numStr);
                    if (count <= 0) {
                        throw invalid_argument("Element count must be positive");
                    }
                } catch (...) {
                    throw invalid_argument("Invalid element count");
                }
            }
            
            elements[element] += count;
        } 
        // skipping other characters
        else {
            if (!isspace(ch) && ch!='*') {
                cerr << "Warning: Unexpected character '" << ch << "' at position " << pos << endl;
            }
            pos++;
        }
    }
    
    return {elements, pos};
}

int main() {
    string input;
    cout << "Enter chemical formula: ";
    getline(cin, input);
    
    map<string, int> elements;
    size_t pos = 0;
    
    try {
        auto [parsedElements, finalPos] = parseElements(input, pos);
        
        if (finalPos != input.length()) {
            cerr << "Warning: Unprocessed characters at the end of input" << endl;
        }
        
        elements = parsedElements;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    cout << "\nFound elements:\n";
    for (const auto& [name, count] : elements) {
        cout << name << ": " << count << "\n";
    }
    
    return 0;
}
