#include "header.h"
#include <iostream>
#include <sstream>


// Konwersja TXT -> XML
std::string conv::FileConverter::txtToXml(const std::string &fileContent) {
    std::istringstream inFile(fileContent);
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");

    std::string line;
    while (std::getline(inFile, line)) {
        pugi::xml_node row = root.append_child("row");
        std::istringstream iss(line);
        std::string word;

        while (iss >> word) {
            pugi::xml_node col = row.append_child("col");
            col.text().set(word.c_str());  
        }
    }
    std::ostringstream out;
    doc.save(out);  
    return out.str();
}


std::string conv::FileConverter::xmlToTxt(const std::string &fileContent) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(fileContent.c_str());

    if (!result) {
        std::cerr << "Invalid XML format" << std::endl;
        throw std::runtime_error("XML conversion error " + std::string(result.description()));
    }

    std::ostringstream outFile;    
    for (pugi::xml_node row = doc.child("row"); row; row = row.next_sibling("row")) {
        std::string line;

       
        for (pugi::xml_node col = row.child("col"); col; col = col.next_sibling("col")) {
            line += std::string(col.child_value()) + " ";  
        }

        if (!line.empty()) {
            line.pop_back();  
            outFile << line << "\n"; 
        }
    }

    return outFile.str(); 
}

