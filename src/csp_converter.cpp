#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string escapeAndQuote(const std::string &str)
{
    std::ostringstream oss;
    oss << "\"" << str << "\"";
    return oss.str();
}

void convertToTsv(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);
    if (!input.is_open())
    {
        std::cerr << "Error: Could not open input file: " << inputFile << std::endl;
        return;
    }
    if (!output.is_open())
    {
        std::cerr << "Error: Could not open output file: " << outputFile << std::endl;
        return;
    }

    std::vector<std::string> header = {
        "document_uri", "referrer", "violated_directive", "effective_directive",
        "original_policy", "disposition", "blocked_uri", "status_code",
        "source_file", "line_number", "column_number", "script_sample",
        "from_user", "date", "CLIENTID", "USERNAME", "handler"};

    for (size_t i = 0; i < header.size(); ++i)
    {
        output << escapeAndQuote(header[i]);
        if (i < header.size() - 1)
        {
            output << "\t";
        }
    }
    output << "\n";

    std::string line;
    std::getline(input, line);

    while (std::getline(input, line))
    {
        try
        {

            json entry = json::parse(line);
            json cspReport = entry.value("csp-report", json::object());
            json fromInfo = entry.value("from", json::object());
            json info = entry.value("info", json::object());

            std::vector<std::string> row = {
                escapeAndQuote(cspReport.value("document-uri", "-")),
                escapeAndQuote(cspReport.value("referrer", "-")),
                escapeAndQuote(cspReport.value("violated-directive", "-")),
                escapeAndQuote(cspReport.value("effective-directive", "-")),
                escapeAndQuote(cspReport.value("original-policy", "-")),
                escapeAndQuote(cspReport.value("disposition", "-")),
                escapeAndQuote(cspReport.value("blocked-uri", "-")),
                std::to_string(cspReport.value("status-code", 0)),
                escapeAndQuote(cspReport.value("source-file", "-")),
                std::to_string(cspReport.value("line-number", 0)),
                std::to_string(cspReport.value("column-number", 0)),
                escapeAndQuote(cspReport.value("script-sample", "-")),
                escapeAndQuote(fromInfo.value("user", "-")),
                std::to_string(fromInfo.value("date", 0)),
                std::to_string(info.value("CLIENTID", 0)),
                escapeAndQuote(info.value("USERNAME", "-")),
                escapeAndQuote(info.value("handler", "-"))};

            // Write the row to the output file
            for (size_t i = 0; i < row.size(); ++i)
            {
                output << row[i];
                if (i < row.size() - 1)
                {
                    output << "\t";
                }
            }
            output << "\n";
        }
        catch (const json::parse_error &e)
        {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

    input.close();
    output.close();
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    convertToTsv(inputFile, outputFile);

    return 0;
}
