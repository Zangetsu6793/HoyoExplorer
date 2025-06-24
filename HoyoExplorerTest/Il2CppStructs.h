#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include "sdk.h"
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
#include <functional>
#include "Utils.h"
#ifndef IL2CPP_STRUCTS_H
#define IL2CPP_STRUCTS_H

// Forward declarations

typedef struct FieldInfo FieldInfo;
typedef struct Il2CppClass Il2CppClass;
typedef struct Il2CppType Il2CppType;
typedef struct Il2CppMethodDefinition Il2CppMethodDefinition;
typedef struct Il2CppFieldDefinition Il2CppFieldDefinition;
typedef struct Il2CppPropertyDefinition Il2CppPropertyDefinition;
typedef struct Il2CppEventDefinition Il2CppEventDefinition;
typedef struct CustomAttribute CustomAttribute;
typedef struct Il2CppImage Il2CppImage;
typedef struct Il2CppImageMetadata Il2CppImageMetadata;
typedef struct Il2CppAssembly Il2CppAssembly;
typedef struct Il2CppTypeDefinition Il2CppTypeDefinition;
typedef struct Il2CppGenericClass Il2CppGenericClass;
typedef struct ParameterInfo ParameterInfo;
typedef struct MethodInfo MethodInfo;

typedef struct VirtualInvokeData VirtualInvokeData;
typedef struct Il2CppInteropData Il2CppInteropData;
typedef void(*Il2CppMethodPointer)();
typedef void* (*InvokerMethod)(Il2CppMethodPointer, const MethodInfo*, void*, void**);


typedef struct FieldInfo {

    
    uint32_t get_field_pointer() const {

        uint32_t value = *reinterpret_cast<uint32_t*>((uintptr_t*)this + 28);
        uint32_t result = (value + 4362740) & 0xFFFFFF;
 
        auto v = ((*(int64_t*)(this + 28) + 4362740) & 0xFFFFFF);

        return v;// & 0xFFFFFF)); Cast methodPointer directly
    }

} FieldInfo;

typedef struct Il2CppDomain
{
} Il2CppDomain;


typedef struct Il2CppClass
{

    const Il2CppImage* image;
   
    const FieldInfo* get_FieldInfoFromName(const char* name) const
    {
        //fprintf(stderr, "Fieldinfocall\n");
        return ((const FieldInfo * (*) (const Il2CppClass*, const char*))Utils::get_offset(offsets::get_Field))(this, name);
    }

    const uintptr_t* get_Field_Address(const char* name)const
    {
        //fprintf(stderr, "ClassPntr %p\n", this);
        auto offset = get_FieldInfoFromName(name);
        //fprintf(stderr, "FieldInfo %p FieldOffset %p\n", fieldI, this+fieldI->get_field_pointer());
        auto addr = (uintptr_t*)(this + offset->get_field_pointer());
        fprintf(stderr, "Adres %p\n", addr);
        return addr ;
    }

    const MethodInfo* get_Method(const char* name, int argsCount) const
    {
        //fprintf(stderr, "CLass %p\n", this);
        //return Il2Cpp::base::il2cpp_class_get_method_from_name((Il2CppClass*)this, name, argsCount);
        return ((const MethodInfo * (*)(Il2CppClass*, const char*, int,int,int))Utils::get_offset(offsets::get_Methods))((Il2CppClass*)this, name, argsCount,0,0);
    }



};


// Definition of Il2CppAssembly
typedef struct Il2CppAssembly {
public:
    const Il2CppImage* get_Image() const
    {
        
        //return Il2Cpp::base::il2cpp_assembly_get_image(this);
        uint64_t value_at_offset = *reinterpret_cast<const uint64_t*>(reinterpret_cast<const char*>(this) + offsets::Image_Offset);
        fprintf(stderr, "GetImage called offset : %p  shift: %p\n", value_at_offset, value_at_offset + offsets::Image_Shift);
        return reinterpret_cast<const Il2CppImage*>(value_at_offset ^ offsets::Image_Shift);
    }
} Il2CppAssembly;

// Definition of Il2CppImage
typedef struct Il2CppImage {
    const Il2CppClass* get_Class(const char* namespaze, const char* name) const
    {
        //return Il2Cpp::base::il2cpp_class_from_name(this, namespaze, name);
        return ((const Il2CppClass * (*)(const Il2CppImage*, const char*, const char*))Utils::get_offset(offsets::get_Class))(this, namespaze, name);
    }
} Il2CppImage;

//Needed
inline const Il2CppAssembly* il2cpp_domain_assembly_open(const char* name)
{
    return ((const Il2CppAssembly * (*)(const char*))Utils::get_offset(offsets::domain_assembly_open))(name);
}
inline char* il2cpp_get_assembly_name(intptr_t* currentAssembly)
{
    return (char*)(*(int64_t*)(*(int64_t*)currentAssembly + offsets::assemblies_aname_Offset) + offsets::aname_Name);
}
inline const std::vector<Il2CppAssembly*> il2cpp_domain_get_assemblies(size_t* size) {
    
    auto Assemblys = Utils::get_offset<uint64_t**>(offsets::all_Assemblies_Array_Offset);
    std::vector<Il2CppAssembly*> assem;
    if (Assemblys != nullptr)
    {
        auto endAdress = *(Assemblys + 1);
        auto currentAssembly = *Assemblys;
        int count = (endAdress - currentAssembly) / 8;
        *size = count;
        for (int i = 0; i <= count; i++)
        {  
            assem.push_back((Il2CppAssembly*)currentAssembly);
            fprintf(stderr, "Ass: %s\n", il2cpp_get_assembly_name((intptr_t*)currentAssembly));
            currentAssembly += 8;
        }
    }

    return assem;
}



inline size_t il2cpp_get_image_type_count(const Il2CppImage* Image)
{
    fprintf(stderr, "Typecount\n");



    uint32_t value = *((uint32_t*)Image + 1);

    //for (unsigned int i = 0; i < 32; i++)
    //{
    //    uint32_t values = *((uint32_t*)Image + i);
    //     // Pointer arithmetic to access 4 bytes at a time
    //    fprintf(stderr, "%d : %d\n", i, values - 667345774);
    //}
    auto v1 = value - 667345774;
    return v1;
}

inline uintptr_t il2cpp_get_image_type_start(const Il2CppImage* Image)
{
    fprintf(stderr, "TypeStart\n");
    uintptr_t value = *((uint32_t*)Image + 2);
    //for (unsigned int i = 0; i < 32; i++)
    //{
    //    uintptr_t values = *((uint32_t*)Image + i);
    //     // Pointer arithmetic to access 4 bytes at a time
    //    fprintf(stderr, "%d : %d\n", i, values - 1109704030);
    //}

    auto v1 = value - 1109704030;
    
    return v1;

}

///



typedef void(*Il2CppMethodPointer)();

// Definition of MethodInfo
typedef struct MethodInfo
{
    Il2CppMethodPointer methodPointers;         // 8 bytes of padding to reach methodPointer
    Il2CppMethodPointer methodPointer;  // This is now located at offset +8
      // This is now located at offset +8

    
    template <typename T>
    T get_function_pointer() const {
        //fprintf(stderr, "MethodInfoPntr %p\n", this);
        return reinterpret_cast<T>(this->methodPointer);  // Cast methodPointer directly
    }

   
    template <typename Ret, typename Class, typename... Args>
    std::function<Ret(Class*, Args...)> get_instance_function() const {
        using MethodType = Ret(*)(Class*, Args...);
        MethodType func = reinterpret_cast<MethodType>(this->methodPointer);
        return [func](Class* instance, Args... args) -> Ret {
            return func(instance, args...);
            };
    }


}MethodInfo;


typedef struct Il2CppType {
} Il2CppType;


typedef struct ParameterInfo {
} ParameterInfo;



typedef struct Il2CppGenericClass {
} Il2CppGenericClass;

typedef struct Image {
} Image;

typedef struct Il2CppTypeDefinition {
} Il2CppTypeDefinition;


typedef struct Il2CppMethodDefinition {
} Il2CppMethodDefinition;

typedef struct Il2CppFieldDefinition {
} Il2CppFieldDefinition;

typedef struct Il2CppPropertyDefinition {
} Il2CppPropertyDefinition;

typedef struct Il2CppEventDefinition {
} Il2CppEventDefinition;

typedef struct CustomAttribute {
} CustomAttribute;

typedef struct VirtualInvokeData {
} VirtualInvokeData;


typedef struct Il2CppInteropData {
} Il2CppInteropData;

typedef wchar_t Il2CppChar;
typedef struct Il2CppObject{
} Il2CppObject;

typedef struct Il2CppString
{
    Il2CppObject object;
    int32_t length;                             ///< Length of string *excluding* the trailing null (which is included in 'chars').
    char pad[12];
    Il2CppChar chars[0];
} Il2CppString;

;
#endif // IL2CPP_STRUCTS_H
