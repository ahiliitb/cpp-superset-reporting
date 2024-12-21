#ifndef UTILS_H
#define UTILS_H

#include<bits/stdc++.h>

// Function to split a string by a delimiter
inline std::vector<std::string> split(const std::string &line, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

// Function to display a progress bar
inline void print_progress_bar(float progress)
{
    int bar_width = 50; // Width of the progress bar
    std::cout << "[";
    int pos = bar_width * progress;
    for (int i = 0; i < bar_width; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

// Helper function to join strings with a delimiter
inline std::string join(const std::vector<std::string> &parts, const std::string &delimiter)
{
    std::ostringstream joined;
    for (size_t i = 0; i < parts.size(); ++i)
    {
        joined << parts[i];
        if (i < parts.size() - 1)
        {
            joined << delimiter;
        }
    }
    return joined.str();
}

#endif // UTILS_H