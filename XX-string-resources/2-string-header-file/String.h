#pragma once

using namespace std;

class String
{
public:

    // add data members here
    // xxx maxSize;
    // xxx length;
    // xxx buffer;
    
    // implement these in c++ file
    String(int maxSize);
    String(const char* defaultText, int maxSize);
    // recommended: String() : String{100} {}
    // recommended: String(const char* defaultText) : String{defaultText, 100} {}
    String(const String& other);
    String(String&& other) noexcept;

    void append(const char* text);
    void appendLine(const char* text);
    void print();
    const char* getString();

    bool operator ==(const String& other);
    bool operator !=(const String& other);
    bool operator <(const String& other);
    bool operator >(const String& other);
    bool operator <=(const String& other);
    bool operator >=(const String& other);

    String operator +(const String& other);
    String& operator +=(const String& other);

    char& operator[]();

    String& operator =(const String& other);
    String& operator =(String&& other) noexcept;

    void Replace(char a, char b);
};
