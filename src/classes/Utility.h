#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vector>
using namespace std;

class Utility
{
public:
    static string queryFile();
    static void removeStr(string &str, string target);
    static string *split(string str, string seperator, int &size);
    static string headerBuilder(int size);
};

string Utility::queryFile()
{
    string fileName;
    cout << "Enter file name: ";
    getline(cin, fileName);

    return fileName;
}

string *Utility::split(string str, string seperator, int &size)
{
    string *val;
    size = 1;

    // Find the size of the array first
    for (int i = 0; i <= str.length(); i++)
    {
        // Conditionally increase size depending on the occurences
        // of the seperator.
        if (str.substr(i, seperator.length()) == seperator)
        {
            size++;

            // Skip to the next index.
            i += seperator.length() - 1;
        };
    }

    // Initialise a new string dynamically
    val = new string[size];

    int prev = 0, curr = 0;
    int index = 0;

    for (int i = 0; i <= str.length(); i++)
    {
        if (str.substr(i, seperator.length()) == seperator)
        {
            // Record current index
            curr = i;

            // Assign the substring in to the array
            // Position: Previous and the current index
            val[index++] = str.substr(prev, curr - prev);

            // Update previous position
            prev = i + seperator.length();

            // Skip iteration
            i += seperator.length() - 1;
        }
    }

    // Add the last substring after the final separator
    val[index++] = str.substr(prev);

    return val;
}

void Utility::removeStr(string &str, string target)
{
    for (int i = 0; i < str.length();)
    {
        if (str.substr(i, target.length()) == target)
        {
            str.erase(i, target.length());
        }
        else
        {
            i++;
        }
    }
}

string Utility::headerBuilder(int size) { return string(size, '-') + "\n"; }

#endif