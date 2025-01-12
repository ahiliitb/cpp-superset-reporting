#include "base_log_parser.h"

BaseLogParser::BaseLogParser(DatabaseConnectionPool &database, std::string file_type)
: database(database),
  file_type(file_type)
  {}

void BaseLogParser::load_log_schema(std::string log_type, std::string xml_file_name)
{
    pugi::xml_document doc;

    pugi::xml_parse_result all_xml_content = doc.load_file(xml_file_name.c_str());
    if (!all_xml_content)
    {
        throw std::runtime_error("Failed to parse XML content: " + std::string(all_xml_content.description()));
    }

    pugi::xml_node root = doc.child("logs");
    if (!root)
    {
        throw std::runtime_error("Root element <logs> not found.");
    }

    pugi::xml_node log_schema_node = (pugi::xml_node) nullptr;
    for (pugi::xml_node log_type_node : root.children())
    {
        if (log_type == log_type_node.name())
        {
            log_schema_node = log_type_node;
            break;
        }
    }

    if (!log_schema_node)
    {
        throw std::runtime_error("Log type '" + log_type + "' not found in XML schema.");
    }

    file_type = log_schema_node.attribute("type").value();

    for (pugi::xml_node column : log_schema_node.children("column"))
    {
        std::string name = column.attribute("name").value();
        std::string datatype = column.attribute("datatype").value();
        std::string dataFormat = column.attribute("format").value();
        bool isArray = column.attribute("array").as_bool();
        bool isPrimary = column.attribute("primaryKey").as_bool();
        bool islookupcolumn = column.attribute("lookup").as_bool();

        this->log_schema.emplace_back(name, datatype, dataFormat, isArray, isPrimary, islookupcolumn);
    }
}

void BaseLogParser::insert_log_file(const std::string &file_path)
{

    if (file_path.substr(file_path.find_last_of(".") + 1) == "gz")
    {
        if (gzFile file = gzopen(file_path.c_str(), "rt"))
        {
            char buffer[1024];
            while (int bytesRead = gzread(file, buffer, sizeof(buffer) - 1))
            {
                buffer[bytesRead] = '\0'; 
                std::string content(buffer);
                std::stringstream ss(content);
                std::string line;
                std::getline(ss, line);
                                       
                int total_lines = 0;
                std::stringstream temp_ss(ss.str()); 
                while (std::getline(temp_ss, line))
                {
                    ++total_lines;
                }

                ss.clear();
                ss.seekg(0, std::ios::beg);

                int current_line = 0;

                while (std::getline(ss, line))
                {
                    // insert_log(line);

                    ++current_line;
                    UTILS::print_progress_bar(static_cast<float>(current_line) / total_lines);
                }

                std::cout << std::endl
                        << "Processing complete!" << std::endl;
            }
            gzclose(file);
        }
        else
        {
            throw std::runtime_error("Failed to open gzipped file: " + file_path);
        }
    }
    else
    {
        std::string line;

        int total_lines = 0;
        std::ifstream count_file(file_path); 
        if (!count_file.is_open())
        {
            throw std::runtime_error("Failed to open file for counting lines: " + file_path);
        }

        std::getline(count_file, line);
        while (std::getline(count_file, line)) total_lines++;

        count_file.close(); 

        std::ifstream file(file_path);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + file_path);
        }

        std::getline(file, line);

        int current_line = 0;

        std::vector<std::string> log_lines;

        while (std::getline(file, line))
        {
            if(log_lines.size() != CONFIG::NUM_QUERIES) log_lines.emplace_back(line);

            if(log_lines.size() == CONFIG::NUM_QUERIES || current_line == total_lines-1)
            {
                this->insert_log(log_lines);
                log_lines.clear();
            }
            

            ++current_line;
            UTILS::print_progress_bar(static_cast<float>(current_line) / total_lines);
        }

        std::cout << std::endl
                << "Processing complete!" << std::endl;
        file.close();
    }
}

void BaseLogParser::insert_log_files(const std::vector<std::string> log_file_name_list)
{
    for (std::string file_name : log_file_name_list)
    {
        this->insert_log_file(file_name);
    }
}

std::vector<std::string> BaseLogParser::parse_log_line(const std::string &log_line)
{
    try
    {
        std::istringstream f(log_line);
        std::vector<std::string> fields;

        if (file_type == "tsv")
        {
            fields = UTILS::split(log_line, '\t');

            for (std::string &field : fields)
            {
                if (field.front() == '"' && field.back() == '"')
                {
                    field = field.substr(1, field.length() - 2); 
                }
                if (field.find(',') != std::string::npos)
                {
                    field = "{" + field + "}";
                }
            }
        }
        else if (file_type == "csv")
        {
            fields = UTILS::split(log_line, ',');
        }
        else
        {
            throw std::invalid_argument("Log file type '" + file_type + "' not supported.");
        }
        
        return fields;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing file: " << e.what() << std::endl;
        return {};
    }
}

int BaseLogParser::get_log_schema_length()
{
    return this->log_schema.size();
}
