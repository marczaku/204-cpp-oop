#pragma once

using namespace std;

class String
{
public:

    int maxSize;
    int length;
    char* buffer;
    
    String(int maxSize);
    String(const char* defaultText, int maxSize);
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
