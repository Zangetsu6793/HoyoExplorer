#pragma once
#include "Windows.h"

namespace offsets
{

    inline uintptr_t domain_assembly_open = 0x5469F0;
    inline uintptr_t get_Methods = 0x3A97C0;
    inline uintptr_t get_Class = 0x3ADEB0;
    inline uintptr_t get_Field = 0x0;
    inline int Image_Offset = 80;
    inline intptr_t Image_Shift = 0x2529D8D37911169CLL;

	inline uintptr_t all_Assemblies_Array_Offset = 0x4535FC0;
	inline int assemblies_aname_Offset = 32LL;
	inline intptr_t aname_Name = -0x16673F8C6A301583LL;

    /// <summary>
    /// Below lies the offsets needed for full dumps 
    /// </summary>
    inline uintptr_t GetTypeOffset;
    inline uintptr_t GetClassNamespazeOffset;
    inline uintptr_t GetMethodsInfoOffset;
    inline uintptr_t GetMethodNameOffset;
    inline uintptr_t GetFieldInfoOffset;
    inline uintptr_t FromIl2CppTypeOffset;
    inline uintptr_t GetMethodReturnTypeOffset;
    inline uintptr_t GetMethodParamTypeOffset;
    // Normal Offsets
    inline int NameFromKlassOffset;
    inline int classParentOffset;
    //Field
    inline int fieldNameOffset;
    inline uintptr_t fieldNameXor;
    inline uintptr_t fieldFlagsXor;
    inline uintptr_t fieldIl2CppTypeOffset;
    inline int fieldTypeOffset;
    inline int fieldOffsetOffset;
    inline int fieldOffsetShift;
    inline int fieldStaticAddrOffset;
    //Methods
    inline int methodFlagsOffset;
    inline int methodArgsCountOffset;
    inline int methodParameterNameOffset;
}