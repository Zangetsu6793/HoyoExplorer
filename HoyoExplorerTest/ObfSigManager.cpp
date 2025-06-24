#include "ObfSigManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

namespace SigManagers {
    ObfSigMan::ObfSigMan()
    {
         
    }
    ObfSigMan::~ObfSigMan()
    {
       
        oldToNewClassMap.clear();
        
        oldToNewMethodMap.clear();
       
        atrNum.clear();
        mask = "";
       
    }
    ObfSigMan ObfSigMan::Init(const char* className) {
        // Initialize or reset any class-specific data here
        //fprintf(stderr, "%s", className);
        return ObfSigMan::ObfSigMan();;  // Return reference to the current object
    }


    

    void ObfSigMan::IncAtr(const char* Atribute) {
        atrNum[Atribute]++;
        mask += toupper(Atribute[0]) + Atribute[1];
    }

    std::string ObfSigMan::GetSig() {
        std::string result;
        
        for (const auto& pair : atrNum) {
            result += pair.first + ":" + std::to_string(pair.second) + ";";
        }
        return result + mask;
    }

    void ObfSigMan::LoadOldDump(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open old dump file: " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Check if the line is a signature line
            if (line.find("// SigMakerSig:") == 0) {
                std::string sig = line.substr(14); // Extract signature after "// SigMakerSig: "
                std::string oldClassName;
                std::getline(file, oldClassName); // Assuming the next line is the class name
                oldToNewClassMap[sig] = oldClassName; // Map signature to old class name
            }
        }
        file.close();
    }

    void ObfSigMan::CompareAndMap(const std::unordered_map<std::string, std::string>& newClassSigs) {
        for (const auto& [newClassName, newSig] : newClassSigs) {
            auto it = oldToNewClassMap.find(newSig);
            if (it != oldToNewClassMap.end()) {
                std::string oldClassName = it->second;
                std::cout << "Matched class: " << oldClassName << " -> " << newClassName << std::endl;

                // Optionally, compare methods similarly
            }
        }
    }

    void ObfSigMan::DisplayMappings() {
        std::cout << "Class Mappings:" << std::endl;
        for (const auto& pair : oldToNewClassMap) {
            std::cout << "Old: " << pair.first << " -> New: " << pair.second << std::endl;
        }

        std::cout << "\nMethod Mappings:" << std::endl;
        for (const auto& [oldClass, methods] : oldToNewMethodMap) {
            std::cout << "Class: " << oldClass << std::endl;
            for (const auto& [oldMethod, newMethod] : methods) {
                std::cout << "    Old: " << oldMethod << " -> New: " << newMethod << std::endl;
            }
        }
    }

    // Function to scan a line for a signature comment and try to match it to old class names
    void ObfSigMan::ScanForSigAndMatch(const std::string& line, const std::unordered_map<std::string, std::string>& newClassSigs) {
        // Check if the line starts with "// SigMakerSig:"
        if (line.find("// SigMakerSig:") == 0) {
            std::string sig = line.substr(14); // Extract the signature part
            auto it = oldToNewClassMap.find(sig);
            if (it != oldToNewClassMap.end()) {
                std::string oldClassName = it->second;
                // Assuming class name follows signature
                std::cout << "Matched signature: " << sig << " -> Old Class Name: " << oldClassName << std::endl;
            }
        }
    }
}
