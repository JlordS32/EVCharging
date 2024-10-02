#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vector>

using namespace std;

/**
 * @class Utility
 * @brief A static utility class for common helper functions.
 */
class Utility
{
public:
    /**
     * @brief Prompts the user to enter a file name.
     * @return `string` - The file name entered by the user.
     */
    static string queryFile();

    /**
     * @brief Removes all occurrences of a target substring from a string.
     *
     * @param str The original string from which to remove substrings.
     * @param target The substring to be removed.
     */
    static void removeStr(string &str, string target);

    /**
     * @brief Splits a string into an array of substrings based on a separator.
     *
     * @param str The string to be split.
     * @param separator The separator string.
     * @param size The number of substrings after splitting.
     *
     * @return `string[size]` - A dynamically allocated array of substrings.
     *
     * @note The caller is responsible for deleting the returned array to prevent memory leaks.
     */
    static string *split(string str, string separator, int &size);

    /**
     * @brief Builds a header string consisting of repeated hyphens.
     *
     * @param size The number of dashes in the header.
     *
     * @return A string containing the header.
     */
    static string headerBuilder(int size);
};

//
// Implementation of Utility class methods
//
string Utility::queryFile()
{
    string fileName;
    cout << "Enter file name: ";
    getline(cin, fileName);

    return fileName;
}

string *Utility::split(string str, string separator, int &size)
{
    string *val;
    size = 1;

    // Find the size of the array first
    for (int i = 0; i <= str.length(); i++)
    {
        // Conditionally increase size depending on the occurrences
        // of the separator.
        if (str.substr(i, separator.length()) == separator)
        {
            size++;

            // Skip to the next index.
            i += separator.length() - 1;
        }
    }

    // Initialize a new string array dynamically
    val = new string[size];

    int prev = 0, curr = 0;
    int index = 0;

    for (int i = 0; i <= str.length(); i++)
    {
        if (str.substr(i, separator.length()) == separator)
        {
            // Record current index
            curr = i;

            // Assign the substring into the array
            // Position: Previous and the current index
            val[index++] = str.substr(prev, curr - prev);

            // Update previous position
            prev = i + separator.length();

            // Skip iteration
            i += separator.length() - 1;
        }
    }

    // Add the last substring after the final separator
    val[index++] = str.substr(prev);

    return val;
}

void Utility::removeStr(string &str, string target)
{
    // Loop through each character in the string
    for (int i = 0; i < str.length();)
    {
        // If the string[i]..string[i + target.length() - 1] == target
        // We remove that substring from the string.
        if (str.substr(i, target.length()) == target)
        {
            str.erase(i, target.length());
        }
        // Skip to the next character
        else
        {
            i++;
        }
    }
}

string Utility::headerBuilder(int size)
{
    return string(size, '-') + "\n";
}

#endif
