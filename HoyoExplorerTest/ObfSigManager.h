#ifndef OBFSIGMANAGER_H
#define OBFSIGMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>

namespace SigManagers {
    class ObfSigMan {
    private:
        
        std::unordered_map<std::string, std::string> oldToNewClassMap; // Maps old class names to new class names
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> oldToNewMethodMap; // Maps old method names to new for each class
        std::unordered_map<std::string, int> atrNum;
        std::string mask;
    public:
        ObfSigMan();
        ~ObfSigMan();
        static ObfSigMan Init(const char* className);
  
        void IncAtr(const char* Atribute);
        std::string GetSig();
        void LoadOldDump(const std::string& filePath);
        void CompareAndMap(const std::unordered_map<std::string, std::string>& newClassSigs);
        void DisplayMappings();

        // Helper functions for extracting signature from comments
        void ScanForSigAndMatch(const std::string& line, const std::unordered_map<std::string, std::string>& newClassSigs);
    };
}

#endif // OBFSIGMANAGER_H
