#include "PatternScanner.h"
#include <Psapi.h>

namespace MemoryScanner {

    uint8_t SigScanner::ToHexadecimal(char character) {
        if (character >= '0' && character <= '9') return character - '0';
        else if (character >= 'a' && character <= 'f') return character - 'a' + 10;
        else if (character >= 'A' && character <= 'F') return character - 'A' + 10;
        return 0xFF;
    }

    std::pair<std::vector<uint8_t>, std::vector<bool>> SigScanner::ConvertSignature(const std::string& signature) {
        std::string sanitized = signature;
        sanitized.erase(std::remove(sanitized.begin(), sanitized.end(), ' '), sanitized.end());

        std::vector<uint8_t> byteVector;
        std::vector<bool> mask;
        byteVector.reserve(sanitized.size() / 2);
        mask.reserve(sanitized.size() / 2);

        for (size_t i = 0; i < sanitized.size(); i += 2) {
            if (sanitized[i] == '?') {
                byteVector.push_back(0x00); // Placeholder for wildcard
                mask.push_back(true);       // True if this is a wildcard byte
                --i; // Adjust for single '?' cases
            }
            else {
                byteVector.push_back((ToHexadecimal(sanitized[i]) << 4) | ToHexadecimal(sanitized[i + 1]));
                mask.push_back(false); // Not a wildcard
            }
        }
        return { byteVector, mask };
    }

    uintptr_t SigScanner::FindPattern(const std::vector<uint8_t>& pattern, const std::vector<bool>& mask, uint8_t* bytes, size_t size, uintptr_t base) {
        size_t patternSize = pattern.size();

        for (size_t i = 0; i <= size - patternSize; ++i) {
            bool found = true;

            for (size_t j = 0; j < patternSize; ++j) {
                // Check if the current byte matches or if it's a wildcard
                if (!mask[j] && bytes[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }

            // If the pattern is fully matched
            if (found) {
                return base + i;
            }
        }
        return 0; // Pattern not found
    }

    uintptr_t SigScanner::ScanForSignature(const std::string& signature, std::string ModuleName) {
        HMODULE hModule = 0;
        if (ModuleName.length() == 0)
            hModule = GetModuleHandleA(Utils::get_Base_Name().c_str());
        else
            hModule = GetModuleHandleA(ModuleName.c_str());
        if (!hModule) return 0;

        MODULEINFO moduleInfo;
        if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO))) return 0;

        uint8_t* moduleBytes = reinterpret_cast<uint8_t*>(moduleInfo.lpBaseOfDll);
        uintptr_t base = reinterpret_cast<uintptr_t>(moduleBytes);
        size_t size = moduleInfo.SizeOfImage;

        auto [pattern, mask] = ConvertSignature(signature); // Get both pattern and mask
        return FindPattern(pattern, mask, moduleBytes, size, base);
    }
}
