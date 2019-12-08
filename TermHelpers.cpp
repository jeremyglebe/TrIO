#include "Termio.h"

/*  _  _  ____  __    ____  ____  ____    ____  _  _  __ _   ___  ____  __  __   __ _    __  _  _  ____  __   
 * / )( \(  __)(  )  (  _ \(  __)(  _ \  (  __)/ )( \(  ( \ / __)(_  _)(  )/  \ (  ( \  (  )( \/ )(  _ \(  )  
 * ) __ ( ) _) / (_/\ ) __/ ) _)  )   /   ) _) ) \/ (/    /( (__   )(   )((  O )/    /   )( / \/ \ ) __// (_/\
 * \_)(_/(____)\____/(__)  (____)(__\_)  (__)  \____/\_)__) \___) (__) (__)\__/ \_)__)  (__)\_)(_/(__)  \____/
 * Graceful font
 * http://patorjk.com/software/taag/
 */

std::string Term::fuse(string left, string right, bool pad)
{
    string result = "";
    // split the strings, create vectors
    vector<string> lv = split(left, '\n');
    vector<string> rv = split(right, '\n');
    // correct the number of lines in the strings so they match
    if (lv.size() > rv.size())
    {
        int diff = lv.size() - rv.size();
        for (int i = 0; i < diff; i++)
        {
            rv.push_back("");
        }
    }
    else if (lv.size() < rv.size())
    {

        int diff = rv.size() - lv.size();
        for (int i = 0; i < diff; i++)
        {
            lv.push_back("");
        }
    }
    if (pad)
    {
        // find the largest line out of any of the vectors
        int max = 0;
        for (int i = 0; i < lv.size(); i++)
        {
            if (lv[i].size() > max)
                max = lv[i].size();
            if (rv[i].size() > max)
                max = rv[i].size();
        }
        // for each line in both vectors, pad the line for max size
        for (int i = 0; i < lv.size(); i++)
        {
            if (lv[i].size() < max)
            {
                int diff = max - lv[i].size();
                lv[i] += std::string(diff, ' ');
            }
            if (rv[i].size() < max)
            {
                int diff = max - rv[i].size();
                rv[i] += std::string(diff, ' ');
            }
        }
    }
    // loop through the vectors and produce the new string
    for (int i = 0; i < lv.size(); i++)
    {
        result += lv[i] + rv[i] + '\n';
    }
    return result;
}

std::string Term::fuse(std::initializer_list<string> strings, bool pad)
{
    // Variable for storing the resulting string
    string result = "";
    // Moving the initializer_list into a vector b/c I think they're easier
    vector<string> vstrings;
    vstrings.insert(vstrings.end(), strings.begin(), strings.end());
    // Loop through all strings and fuse them
    for (int i = 0; i < vstrings.size(); i++)
    {
        result = fuse(result, vstrings[i], pad);
    }
    return result;
}

// Just calls the regular expression split but delimeter does not have to be
// a regular expression
std::vector<std::string> Term::split(string text, char delim, bool include)
{
    return rsplit(text, string(1, delim), include);
}

// Part of the rsplit() implementation came from Stack Overflow user Marcin,
// see the following post:
// https://stackoverflow.com/questions/16749069/c-split-string-by-regex
std::vector<std::string> Term::rsplit(string text, string delim, bool include)
{
    std::vector<std::string> elems;
    std::regex rgx(delim);
    // Find parts of the string which do not match the regexp and append them
    // to the vector
    // code -1 at the end means find everything but the rgx string (delimeter)
    std::sregex_token_iterator iter(text.begin(), text.end(), rgx, -1);
    std::sregex_token_iterator end;
    while (iter != end)
    {
        elems.push_back(*(iter));
        ++iter;
    }

    // If we want to include the delimeter, then we need to go re-add it to
    // the beginning of each line.
    if (include)
    {
        // code 0 at the end means find instances of the rgx string (delimeter)
        std::sregex_token_iterator iter(text.begin(), text.end(), rgx, 0);
        std::sregex_token_iterator end;

        int i = 0;
        while (iter != end)
        {
            // We are inserting the delimiter at the start
            // of the NEXT string in the vector
            string temp = elems[i + 1];
            // assign the string the delimiter
            elems[i + 1] = *iter;
            // add back the original content
            elems[i + 1] += temp;
            // the iterator moves forward
            ++iter;
            // increment elems index
            i++;
        }
    }

    return elems;
}

std::string Term::replace_all(string text, string from, string to)
{
    size_t start_pos = 0;
    // While we can find the substring "from"
    // (start position changes each time we make a replacement)
    while ((start_pos = text.find(from, start_pos)) != string::npos)
    {
        // Make the replacement
        text.replace(start_pos, from.length(), to);
        // Move past the string replacement's length, we don't need to
        // check it
        start_pos += to.length();
    }
    return text;
}

std::wostream &Term::operator<<(wostream &wout, string text)
{
    /**
     * create a string <-> wide string converter
     * example from stack overflow
     * std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
     * std::string narrow = converter.to_bytes(wide_utf16_source_string);
     * std::wstring wide = converter.from_bytes(narrow_utf8_source_string);
     */
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide_text = converter.from_bytes(text);
    wout << wide_text;
    return wout;
}
