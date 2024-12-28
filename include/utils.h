#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

// Function to read a JSON file and convert it to a std::unordered_map
inline std::unordered_map<std::string, std::string> jsonToDict(const std::string &filePath)
{
    std::unordered_map<std::string, std::string> result;

    try
    {
        // Open the JSON file
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open file: " + filePath);
        }

        // Parse the JSON file
        json jsonObj;
        file >> jsonObj;

        // Check if it's a JSON object
        if (!jsonObj.is_object())
        {
            throw std::invalid_argument("JSON in the file is not an object.");
        }

        // Iterate through the JSON object
        for (auto &[key, value] : jsonObj.items())
        {
            result[key] = value.is_string() ? value.get<std::string>() : value.dump(); // Serialize non-strings
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return result;
}

#endif // UTILS_H