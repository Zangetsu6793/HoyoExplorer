#ifndef MEMORYSCANNER_H
#define MEMORYSCANNER_H

#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include "Utils.h"

namespace MemoryScanner {

    class SigScanner {
    public:
        SigScanner() = default;
        uintptr_t ScanForSignature(const std::string& signature, std::string ModuleName ="");

    private:
        uint8_t ToHexadecimal(char character);
        std::pair<std::vector<uint8_t>, std::vector<bool>> ConvertSignature(const std::string& signature);
        uintptr_t FindPattern(const std::vector<uint8_t>& pattern, const std::vector<bool>& mask, uint8_t* bytes, size_t size, uintptr_t base);
    };

}

#endif
