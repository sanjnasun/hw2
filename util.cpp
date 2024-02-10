#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{
    std::set<std::string> keywords;
    std::stringstream ss(rawWords);
    std::string word;

    while (ss >> word) {
        // Trim leading and trailing whitespace from the word
        word = trim(word);
        
        // Check if the word has at least 2 characters
        if (word.length() >= 2) {
            std::string keyword;
            bool has_keyword = false;
            // Iterate over each character of the word
            for (char c : word) {
                // If the character is alphanumeric, add it to the current keyword
                if (std::isalnum(c)) {
                    keyword += c;
                    has_keyword = true;
                } else {
                    // If the keyword has at least 2 characters, add it to the set of keywords
                    if (keyword.length() >= 2) {
                        keywords.insert(keyword);
                    }
                    // Reset the keyword
                    keyword.clear();
                }
            }
            // If the word ends with an alphanumeric character, add it to the set of keywords
            if (has_keyword && keyword.length() >= 2) {
                keywords.insert(keyword);
            }
        }
    }

    return keywords;
}







/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
