#include "Utils.h"

namespace Utils
{
    uintptr_t get_offset(uintptr_t offset)
    {
        return Utils::getBase() + offset;
    }
    template< typename T>
    T get_offset(uintptr_t offset)
    {
        return reinterpret_cast<T>(Utils::getBase() + offset);
    }
    template unsigned __int64 const*** Utils::get_offset<unsigned __int64 const***>(uintptr_t offset);
    template uint64_t* Utils::get_offset<uint64_t*>(uintptr_t offset);
    template uint64_t** Utils::get_offset<uint64_t**>(uintptr_t offset);

    std::string GetProcName()
    {
        char path[MAX_PATH];
        if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
            std::string fullPath = path;

           
            size_t pos = fullPath.find_last_of("\\/"); 
            if (pos != std::string::npos) {
                std::string procName = fullPath.substr(pos + 1); 

                std::replace_if(procName.begin(), procName.end(), [](char c) {
                    return c == '?' || c == '*' || c == ':' || c == '"' || c == '<' || c == '>' || c == '|' || c == '\\' || c == '/';
                    }, '_');  // Replace invalid chars with '_'

                return procName;
            }
        }
        return "";
    }

    std::string GetProcLocation()
    {
        char path[MAX_PATH];
        if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
            std::string fullPath = path;

            
            size_t pos = fullPath.find_last_of("\\/");  
            if (pos != std::string::npos) {
                return fullPath.substr(0, pos);
            }
        }
        return "";
    }
  
    uintptr_t getBase()
    {
        uintptr_t base = 0;
        if (Utils::GetProcName() != "GenshinImpact.exe")
            while (!base) base = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
        else
            while (!base) base = (uintptr_t)GetModuleHandleA("GenshinImpact.exe");
        return base;
    }

    std::string get_Base_Name()
    {
        uintptr_t base = 0;
        base = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
        if (base)
            return "GameAssembly.dll";
        else
            return GetProcName();
    }
    uintptr_t getBase_Module_Address()
    {
        uintptr_t base = 0;
        auto procName = GetProcName();
        while (!base) base = (uintptr_t)GetModuleHandleA(procName.c_str());
        return base;
    }


    bool CheckFileExists(std::string filename)
    {
        std::string folderName = "Dumps\\" + filename;


        if (!std::filesystem::exists(folderName))
            return false;

        return true;
        
    }

    void writeToCSharpFile(const std::ostringstream& csharpCode, std::string filename) {
        
        //std::string folderName = GetProcLocation() + "/Dumps/" + GetProcName();
        std::string folderName = "Dumps/" + GetProcName();

        
        if (!std::filesystem::exists(folderName)) {
            std::filesystem::create_directory(folderName);
        }

      
        std::string dumpFileName;
        if (filename.empty())
            dumpFileName = GetProcName() + "_Dump.cs"; 
        else
            dumpFileName = filename + ".cs";

          std::string filePath = folderName + "/" + dumpFileName;

          std::ofstream outFile(filePath);

      
        if (outFile.is_open()) {
            outFile << csharpCode.str();
            outFile.close(); 
            fprintf(stderr, "Code written to: %s\n", filePath.c_str());
        }
        else {
            fprintf(stderr, "Error opening file for writing.\n");
        }
    }


   
    std::string readFromIniFile(const std::string& section, const std::string& key, const std::string& iniFilePath) {
        char buffer[256];
        DWORD size = GetPrivateProfileString(section.c_str(), key.c_str(), "", buffer, sizeof(buffer), iniFilePath.c_str());
        return std::string(buffer, size);
    }


    std::string getExeDirectory() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string exePath(buffer);

        // Remove the executable name to get the directory path
        size_t pos = exePath.find_last_of("\\/");
        if (pos != std::string::npos) {
            exePath = exePath.substr(0, pos);
        }
        return exePath;
    }
  
    void Print(std::string text)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD charsWritten;

        if (hConsole == INVALID_HANDLE_VALUE) {
            std::cerr << "Error getting console handle.\n";
        }

        WriteConsole(hConsole, text.c_str(), strlen(text.c_str()), &charsWritten, NULL);
    }


    std::string Utf16ToUtf8(const wchar_t* utf16String, int maximumSize)
    {
        const wchar_t* ptr = utf16String;
        size_t length = 0;
        while (*ptr)
        {
            ptr++;
            length++;
            if (maximumSize != -1 && length == maximumSize)
                break;
        }

        std::string utf8String;
        utf8String.reserve(length);
        utf16to8(utf16String, ptr, std::back_inserter(utf8String));

        return utf8String;
    }

    template <typename u16bit_iterator, typename octet_iterator>
    octet_iterator utf16to8(u16bit_iterator start, u16bit_iterator end, octet_iterator result)
    {
        const uint32_t SURROGATE_OFFSET = 0x10000u - (0xd800u << 10) - 0xdc00u;
        while (start != end) {
            uint32_t cp = static_cast<uint16_t>(0xffff & *start++);
            // Take care of surrogate pairs first
            if ((cp >= 0xd800u && cp <= 0xdbffu)) {
                uint32_t trail_surrogate = static_cast<uint16_t>(0xffff & *start++);
                cp = (cp << 10) + trail_surrogate + SURROGATE_OFFSET;
            }
            result = append(cp, result);
        }
        return result;
    }
    template <typename octet_iterator>
    octet_iterator append(uint32_t cp, octet_iterator result)
    {
        if (cp < 0x80)                        // one octet
            *(result++) = static_cast<uint8_t>(cp);
        else if (cp < 0x800) {                // two octets
            *(result++) = static_cast<uint8_t>((cp >> 6) | 0xc0);
            *(result++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
        }
        else if (cp < 0x10000) {              // three octets
            *(result++) = static_cast<uint8_t>((cp >> 12) | 0xe0);
            *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f) | 0x80);
            *(result++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
        }
        else {                                // four octets
            *(result++) = static_cast<uint8_t>((cp >> 18) | 0xf0);
            *(result++) = static_cast<uint8_t>(((cp >> 12) & 0x3f) | 0x80);
            *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f) | 0x80);
            *(result++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
        }
        return result;
    }
}
