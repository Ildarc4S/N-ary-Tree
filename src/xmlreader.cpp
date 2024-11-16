#include "xmlreader.hpp"

void XMLReader::loadFile(const std::string& file_name) {
    file.open(file_name);
    if (!file.is_open()) {
        std::cerr << "File " << file_name << " not open!" << std::endl;
    } else {
        std::cout << "XML file is open!" << std::endl;
        if (checkTags()) {
            std::cerr << "XML file format correct!" << std::endl;
            file.clear();
            file.seekg(0);
            parseProperties();
        } else {
            std::cerr << "XML file format not correct!" << std::endl;
        }
        file.close();
    }
}

void XMLReader::parseProperties() {
    std::string line;
    bool inProperty = false;
    bool stop = false;

    while (std::getline(file, line) && !stop) {
        trim(line);
        if (line == "<property>") {
            inProperty = true;
        } else if (line == "</property>") {
            inProperty = false;
        } else if (inProperty) {
            if (line.find("<name>depth</name>") != std::string::npos) {
                processProperty("depth");
                inProperty = false;
            } else if (line.find("<name>probability</name>") != std::string::npos) {
                processProperty("probability");
                inProperty = false;
            } else {
                std::cerr << "Invalid name format in XML file." << std::endl;
                stop = true;
            }
        } else if (line.find("<description>") != std::string::npos) {
            tree_description = extractElement(line, "description");
        }
    }
}

void XMLReader::processProperty(const std::string& property_name) {
    std::string line;
    std::getline(file, line);
    trim(line);
    if (property_name == "depth") {
        depth_description = extractElement(line, "description");
    } else if (property_name == "probability") {
        probability_description = extractElement(line, "description");
    } else {
        std::cerr << "Not found property name!" << std::endl;
    }

    std::getline(file, line);
    trim(line);
    std::string str = extractElement(line, "value");

    if (property_name == "depth") {
        depth = str != "" ? std::stoi(str) : 0;
    } else if (property_name == "probability") {
        probability = str != "" ? std::stof(str) : 0.0f;
        if (probability < 0.0f || probability > 1.0f) {
            std::cerr << "Probability value out of range: " << probability << ". Setting to default (0.0)."
                      << std::endl;
            probability = 0.0f;
        }
    } else {
        std::cerr << "Not found property name!" << std::endl;
    }
}

std::string XMLReader::extractElement(const std::string& line, const std::string& element_name) {
    std::string search_str_begin = "<" + element_name + ">";
    std::string search_str_end = "</" + element_name + ">";

    size_t start = line.find(search_str_begin) + search_str_begin.size();
    size_t end = line.find(search_str_end);

    std::string result = "";
    if (start == std::string::npos || end == std::string::npos || start >= end) {
        std::cerr << "Invalid " << element_name << " format in XML file." << std::endl;
    } else {
        result = line.substr(start, end - start);
    }

    return result;
}

bool XMLReader::checkTags() {
    std::stack<std::string> tags;
    std::string line;

    while (std::getline(file, line)) {
        trim(line);
        if (line == "<tree>" || line == "<property>") {
            tags.push(line);
        } else if (line == "</tree>" || line == "</property>") {
            if (tags.empty()) return false;
            std::string openTag = tags.top();
            tags.pop();
            if ((line == "</tree>" && openTag != "<tree>") ||
                (line == "</property>" && openTag != "<property>")) {
                return false;
            }
        }
    }
    return tags.empty();
}

void XMLReader::trim(std::string& str) {
    str.erase(0, str.find_first_not_of(" \t\n\r"));
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
}
