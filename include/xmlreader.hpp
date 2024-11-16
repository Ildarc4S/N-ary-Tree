#pragma once

#include <fstream>
#include <iostream>
#include <stack>

class XMLReader {
   public:
    XMLReader() : depth{0}, probability{0.0f} {}

    void loadFile(const std::string &file_name);
    float getProbability() { return probability; }
    int getDepth() { return depth; }
    std::string getTreeDescription() { return tree_description; }
    std::string getDepthDescription() { return depth_description; }
    std::string getProbabilityDescription() { return probability_description; }

   private:
    int depth = 0;
    float probability = 0.0f;
    std::ifstream file;
    std::string tree_description;
    std::string depth_description;
    std::string probability_description;

    void parseProperties();
    std::string extractElement(const std::string &line, const std::string &element_name);
    void processProperty(const std::string &property_name);
    bool checkTags();
    void trim(std::string &str);
};
