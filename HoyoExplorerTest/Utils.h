#pragma once
#include <string>
#include "Windows.h"
#include <sstream>
#include <ostream>
#include <Windows.h>

#include <cstdint>
#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#include <iostream>
#include <string>
#include <sstream>

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>


namespace Utils
{

    uintptr_t get_offset(uintptr_t offset);
    std::string GetProcName();

    /// <summary>
   /// Returns base address of GameAssembly
   /// </summary>
    uintptr_t getBase();


    template< typename T>
    T get_offset(uintptr_t offset);

    std::string get_Base_Name();

    /// <summary>
   /// Returns base address of current proccess
   /// </summary>
    uintptr_t getBase_Module_Address();
    std::string GetProcLocation();
    void writeToCSharpFile(const std::ostringstream& csharpCode, std::string filename = "");
    std::string getExeDirectory();
    std::string readFromIniFile(const std::string& section, const std::string& key, const std::string& iniFilePath);
    void Print(std::string text);

    bool CheckFileExists(std::string filename);

    std::string Utf16ToUtf8(const wchar_t* utf16String, int maximumSize);
    template <typename u16bit_iterator, typename octet_iterator>
    octet_iterator utf16to8(u16bit_iterator start, u16bit_iterator end, octet_iterator result);
    template <typename octet_iterator>
    octet_iterator append(uint32_t cp, octet_iterator result);

}