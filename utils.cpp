#include "utils.h"

int binToDigit (char c) {
    if ('0' <= c && c <= '1') { return c - '0'; }
    return -1;
}

int digitToDigit (char c) {
    if ('0' <= c && c <= '9') { return c - '0'; }
    return -1;
}

int hexToDigit (char c) {
    if ('0' <= c && c <= '9') { return c - '0'; }
    if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
    if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
    return -1;
}

int debinify (std::string s) {
    int res = 0;
    for (int i=0; i < s.size(); i++) {
        int d = binToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 2, (double) s.size() - 1 - i);
    }
    return res;
}

int dedigify (std::string s) {
    int res = 0;
    for (int i=0; i < s.size(); i++) {
        int d = digitToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 10,(double) s.size() - 1 - i);
    }
    return res;
}

int dehexify(char msc, char lsc) {
    int msi = hexToDigit(msc), lsi = hexToDigit(lsc);
    if (msi == -1 or lsi == -1) { return -1; }
    return hexToDigit(msc) * 16 + hexToDigit(lsc);
}

int dehexify(std::string s) {
    int res = 0;
    for (int i=0; i < s.size(); i++) {
        int d = hexToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 16, (double) s.size() - 1 - i);
    }
    return res;
}

void displayToken (int token, int lineno, int charno) {
    std::cout << lineno << "," << charno << "," << std::endl;
}