#pragma once
#include <vector>
#include "sdk.h"
#include "Utils.h"
#include "Il2CppStructs.h"
static FILE* outFile;
#define DUMPER
#ifdef DUMPER
#include "ObfSigManager.h"
#include <unordered_map>
#include <string>
namespace Dumpers{
    




   


    typedef Il2CppClass* (*GetTypeInfoFromTypeDefinitionIndex)(int32_t index);
    typedef const MethodInfo* (*GetMethods)(Il2CppClass* klass, void** iter);
    typedef FieldInfo* (*GetFields)(Il2CppClass* klass, void** iter);
    typedef Il2CppClass* (*GetTypeDefinition)(Il2CppGenericClass* gclass);

    typedef uint64_t(*GetMethodName)(uint64_t a1);
    typedef const char* (*GetClassNamespaze)(Il2CppClass*);
    typedef int64_t(*GetMethodReturnType)(int64_t methodInfo);
    typedef int64_t(*GetMethodParamType)(int64_t methodInfo);


    typedef const char* (*GetData)(FieldInfo* field);
    typedef Il2CppClass* (*FromIl2CppType)(const Il2CppType* type, bool throwOnError);
   
    // GameDumpInfo class manages offsets and function pointers related to game data retrieval
    class Dumper {
    public:
        // Constructor and destructor
        Dumper();
        ~Dumper();
        static Dumper Init();
        // Function accessors
        std::string getClassInfoFromIndex(int index);
        std::string BuildClassMethodsString(Il2CppClass* classPntr);

        void FullDump();
        
        void DumpAllMethods();
        void DumpAllLoadedAssemblies();
        void DumpAssemblyImage(std::string AssemblyName);
        const char* getClassNameFromKlass(Il2CppClass* klass);
        SigManagers::ObfSigMan sigMan;

        //Using API
        void FullDumpUsingAPI();
        void DumpAssemblyImageUsingAPI(std::string AssemblyName);

    private:
        std::string resolveMethodAttributes(uint16_t flags);
        std::string resolveFieldAttributes(uint32_t flags);
        Il2CppClass* getTypeInfoFromIndex(int32_t index);
        const MethodInfo* getMethods(Il2CppClass* klass, void** iter);
        FieldInfo* getFields(Il2CppClass* klass, void** iter);
        Il2CppClass* getTypeDefinition(Il2CppGenericClass* gclass);

        uint64_t getMethodName(uint64_t a1);
        const char* getClassNamespaze(Il2CppClass* klass);
        int64_t getMethodReturnType(int64_t methodInfo);
        int64_t getMethodParamType(int64_t methodInfo);
        const char* getData(FieldInfo* field);
        Il2CppClass* fromIl2CppType(const Il2CppType* type, bool throwOnError);

        void SetupOffsets();
        void SetupExternalCalls();

        void CreateDummyDll();

        std::string ProcessNamespace(const std::string& namespaceName,
            std::unordered_map<std::string, std::vector<int>>& namespaceToClasses,
            std::unordered_map<std::string, std::vector<int>>& classNameToInts);

        std::string BuildClass(const std::string& className, const std::vector<int>& classIndices);

        
        std::string BuildClassFieldsString(Il2CppClass* classPntr);

        // Pointers to external functions
        GetTypeInfoFromTypeDefinitionIndex GetTypeDefFromIndex;
        GetMethods GetMethodsInfoFunct;
        GetFields GetClassFieldsInfo;
        GetTypeDefinition GetTypeDefenitionFunct;

        GetMethodName GetMethodNameFunct;
        GetClassNamespaze GetClassNamespazeFunct;
        GetMethodReturnType GetMethodReturnTypeFunct;
        GetMethodParamType GetMethodParamTypeFunct;
        GetData GetDataFromFieldInfo;
        FromIl2CppType FromIl2CppTypeFunct;
        
    };

    struct Offsets {
        uint64_t GetTypeOffset;             //1
        uint64_t GetClassNamespazeOffset;   //2
        uint64_t GetMethodsInfoOffset;      //3
        uint64_t GetMethodNameOffset;       //4
        uint64_t GetFieldInfoOffset;        //5
        uint64_t FromIl2CppTypeOffset;      //6
        uint64_t GetMethodReturnTypeOffset; //7
        uint64_t GetMethodParamTypeOffset;  //8

        // Normal Offsets
        uint64_t NameFromKlassOffset;       //9
        uint64_t classParentOffset;         //10

        // Field Offsets
        uint64_t fieldNameOffset;           //11
        uint64_t fieldNameXor;              //12
        uint64_t fieldFlagsXor;             //13
        uint64_t fieldIl2CppTypeOffset;     //14
        uint64_t fieldTypeOffset;           //15
        uint64_t fieldOffsetOffset;         //16
        uint64_t fieldOffsetShift;          //17
        int64_t fieldStaticAddrOffset;     //18

        // Method Offsets
        uint64_t methodFlagsOffset;         //19
        uint64_t methodArgsCountOffset;     //20
        uint64_t methodParameterNameOffset; //21
    };

    inline std::unordered_map<std::string, Offsets> gameOffsets = {
    {"ZenlessZoneZero.exe", {
        0xDB13B0,                           //1*
        0xD65B30,                           //2*
        0xDA5DF0,                           //3*
        0xDBBEC0,                           //4*
        0xDA59A0,                           //5*
        0xDA3060,                           //6*
        0xDBBFC0,                           //7*
        0xDBC210,                           //8*
        88,                                 //9*
        168,                                //10
        0,                                  //11*
        0x431554713779A363LL,               //12*
        0x10F11F07074DCCC3LL,               //13*
        0x10F11F07074DCCCBLL,               //14*
        16,                                 //15*
        24,                                 //16*
        7253929,                            //17*
        -0x709A42D160394E8ALL,                                 //18*
        48,                                 //19*
        50,                                 //20*
        24                                  //21*
    }},
    {"GenshinImpact.exe", {
        0x5B8A0,                            //1
        0x5EC50,                            //2*
        0x507D0,                            //3
        0x65EA0,                            //4
        0x501D0,                            //5
        0x4D790,                            //6
        0x65FB0,                            //7
        0x3BFD20,                           //8
        112,                                //9*
        164,                                //10
        8,                                  //11
        0x4C21D3B85D44195BLL,               //12
        0x5642573B4B38EB1ELL,               //13
        0x5642573B4B38EB26LL,               //14
        16,                                 //15
        24,                                 //16
        14099979,                           //17
        80,                                 //18
        40,                                 //19
        46,                                 //20
        24                                  //21
    }}
    };

	


    /*
     * Method Attributes (22.1.9)
     */

#define METHOD_IMPL_ATTRIBUTE_CODE_TYPE_MASK       0x0003
#define METHOD_IMPL_ATTRIBUTE_IL                   0x0000
#define METHOD_IMPL_ATTRIBUTE_NATIVE               0x0001
#define METHOD_IMPL_ATTRIBUTE_OPTIL                0x0002
#define METHOD_IMPL_ATTRIBUTE_RUNTIME              0x0003

#define METHOD_IMPL_ATTRIBUTE_MANAGED_MASK         0x0004
#define METHOD_IMPL_ATTRIBUTE_UNMANAGED            0x0004
#define METHOD_IMPL_ATTRIBUTE_MANAGED              0x0000

#define METHOD_IMPL_ATTRIBUTE_FORWARD_REF          0x0010
#define METHOD_IMPL_ATTRIBUTE_PRESERVE_SIG         0x0080
#define METHOD_IMPL_ATTRIBUTE_INTERNAL_CALL        0x1000
#define METHOD_IMPL_ATTRIBUTE_SYNCHRONIZED         0x0020
#define METHOD_IMPL_ATTRIBUTE_NOINLINING           0x0008
#define METHOD_IMPL_ATTRIBUTE_MAX_METHOD_IMPL_VAL  0xffff

#define METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK        0x0007
#define METHOD_ATTRIBUTE_COMPILER_CONTROLLED       0x0000
#define METHOD_ATTRIBUTE_PRIVATE                   0x0001
#define METHOD_ATTRIBUTE_FAM_AND_ASSEM             0x0002
#define METHOD_ATTRIBUTE_ASSEM                     0x0003
#define METHOD_ATTRIBUTE_FAMILY                    0x0004
#define METHOD_ATTRIBUTE_FAM_OR_ASSEM              0x0005
#define METHOD_ATTRIBUTE_PUBLIC                    0x0006

#define METHOD_ATTRIBUTE_STATIC                    0x0010
#define METHOD_ATTRIBUTE_FINAL                     0x0020
#define METHOD_ATTRIBUTE_VIRTUAL                   0x0040
#define METHOD_ATTRIBUTE_HIDE_BY_SIG               0x0080

#define METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK        0x0100
#define METHOD_ATTRIBUTE_REUSE_SLOT                0x0000
#define METHOD_ATTRIBUTE_NEW_SLOT                  0x0100

#define METHOD_ATTRIBUTE_STRICT                    0x0200
#define METHOD_ATTRIBUTE_ABSTRACT                  0x0400
#define METHOD_ATTRIBUTE_SPECIAL_NAME              0x0800

#define METHOD_ATTRIBUTE_PINVOKE_IMPL              0x2000
#define METHOD_ATTRIBUTE_UNMANAGED_EXPORT          0x0008

     /*
      * For runtime use only
      */
#define METHOD_ATTRIBUTE_RESERVED_MASK             0xd000
#define METHOD_ATTRIBUTE_RT_SPECIAL_NAME           0x1000
#define METHOD_ATTRIBUTE_HAS_SECURITY              0x4000
#define METHOD_ATTRIBUTE_REQUIRE_SEC_OBJECT        0x8000


#define FIELD_ATTRIBUTE_FIELD_ACCESS_MASK     0x0007
#define FIELD_ATTRIBUTE_COMPILER_CONTROLLED   0x0000
#define FIELD_ATTRIBUTE_PRIVATE               0x0001
#define FIELD_ATTRIBUTE_FAM_AND_ASSEM         0x0002
#define FIELD_ATTRIBUTE_ASSEMBLY              0x0003
#define FIELD_ATTRIBUTE_FAMILY                0x0004
#define FIELD_ATTRIBUTE_FAM_OR_ASSEM          0x0005
#define FIELD_ATTRIBUTE_PUBLIC                0x0006

#define FIELD_ATTRIBUTE_STATIC                0x0010
#define FIELD_ATTRIBUTE_INIT_ONLY             0x0020
#define FIELD_ATTRIBUTE_LITERAL               0x0040
#define FIELD_ATTRIBUTE_NOT_SERIALIZED        0x0080
#define FIELD_ATTRIBUTE_SPECIAL_NAME          0x0200
#define FIELD_ATTRIBUTE_PINVOKE_IMPL          0x2000

      /* For runtime use only */
#define FIELD_ATTRIBUTE_RESERVED_MASK         0x9500
#define FIELD_ATTRIBUTE_RT_SPECIAL_NAME       0x0400
#define FIELD_ATTRIBUTE_HAS_FIELD_MARSHAL     0x1000
#define FIELD_ATTRIBUTE_HAS_DEFAULT           0x8000
#define FIELD_ATTRIBUTE_HAS_FIELD_RVA         0x0100

    enum
    {
        BFLAGS_IgnoreCase = 1,
        BFLAGS_DeclaredOnly = 2,
        BFLAGS_Instance = 4,
        BFLAGS_Static = 8,
        BFLAGS_Public = 0x10,
        BFLAGS_NonPublic = 0x20,
        BFLAGS_FlattenHierarchy = 0x40,
        BFLAGS_InvokeMethod = 0x100,
        BFLAGS_CreateInstance = 0x200,
        BFLAGS_GetField = 0x400,
        BFLAGS_SetField = 0x800,
        BFLAGS_GetProperty = 0x1000,
        BFLAGS_SetProperty = 0x2000,
        BFLAGS_ExactBinding = 0x10000,
        BFLAGS_SuppressChangeType = 0x20000,
        BFLAGS_OptionalParamBinding = 0x40000
    };


    typedef enum Il2CppTypeEnum
    {
        IL2CPP_TYPE_END = 0x00,  // End of List
        IL2CPP_TYPE_VOID = 0x01,  // Void type
        IL2CPP_TYPE_BOOLEAN = 0x02,  // Boolean type
        IL2CPP_TYPE_CHAR = 0x03,  // Character type
        IL2CPP_TYPE_I1 = 0x04,  // 8-bit signed integer
        IL2CPP_TYPE_U1 = 0x05,  // 8-bit unsigned integer
        IL2CPP_TYPE_I2 = 0x06,  // 16-bit signed integer
        IL2CPP_TYPE_U2 = 0x07,  // 16-bit unsigned integer
        IL2CPP_TYPE_I4 = 0x08,  // 32-bit signed integer
        IL2CPP_TYPE_U4 = 0x09,  // 32-bit unsigned integer
        IL2CPP_TYPE_I8 = 0x0a,  // 64-bit signed integer
        IL2CPP_TYPE_U8 = 0x0b,  // 64-bit unsigned integer
        IL2CPP_TYPE_R4 = 0x0c,  // 32-bit floating-point
        IL2CPP_TYPE_R8 = 0x0d,  // 64-bit floating-point
        IL2CPP_TYPE_STRING = 0x0e,  // String type
        IL2CPP_TYPE_PTR = 0x0f,  // Pointer type
        IL2CPP_TYPE_BYREF = 0x10,  // By-reference type
        IL2CPP_TYPE_VALUETYPE = 0x11,  // Value type
        IL2CPP_TYPE_CLASS = 0x12,  // Class type
        IL2CPP_TYPE_VAR = 0x13,  // Generic type parameter
        IL2CPP_TYPE_ARRAY = 0x14,  // Array type
        IL2CPP_TYPE_GENERICINST = 0x15, // Generic instance type
        IL2CPP_TYPE_TYPEDBYREF = 0x16,  // Typed by-reference
        IL2CPP_TYPE_I = 0x18,  // Platform-specific integer
        IL2CPP_TYPE_U = 0x19,  // Platform-specific unsigned integer
        IL2CPP_TYPE_FNPTR = 0x1b,  // Function pointer type
        IL2CPP_TYPE_OBJECT = 0x1c,  // Object type
        IL2CPP_TYPE_SZARRAY = 0x1d,  // Single-dimensional array
        IL2CPP_TYPE_MVAR = 0x1e,  // Generic method parameter
        IL2CPP_TYPE_CMOD_REQD = 0x1f,  // Required type modifier
        IL2CPP_TYPE_CMOD_OPT = 0x20,  // Optional type modifier
        IL2CPP_TYPE_INTERNAL = 0x21,  // Internal type

        IL2CPP_TYPE_MODIFIER = 0x40,  // Modifier
        IL2CPP_TYPE_SENTINEL = 0x41,  // Sentinel
        IL2CPP_TYPE_PINNED = 0x45,  // Pinned

        IL2CPP_TYPE_ENUM = 0x55   // Enumeration type
    } Il2CppTypeEnum;

#include <string>

    inline std::string Il2CppTypeToString(Il2CppTypeEnum type) {
        switch (type) {
        case IL2CPP_TYPE_END: return "IL2CPP_TYPE_END";
        case IL2CPP_TYPE_VOID: return "IL2CPP_TYPE_VOID";
        case IL2CPP_TYPE_BOOLEAN: return "IL2CPP_TYPE_BOOLEAN";
        case IL2CPP_TYPE_CHAR: return "IL2CPP_TYPE_CHAR";
        case IL2CPP_TYPE_I1: return "IL2CPP_TYPE_I1";
        case IL2CPP_TYPE_U1: return "IL2CPP_TYPE_U1";
        case IL2CPP_TYPE_I2: return "IL2CPP_TYPE_I2";
        case IL2CPP_TYPE_U2: return "IL2CPP_TYPE_U2";
        case IL2CPP_TYPE_I4: return "IL2CPP_TYPE_I4";
        case IL2CPP_TYPE_U4: return "IL2CPP_TYPE_U4";
        case IL2CPP_TYPE_I8: return "IL2CPP_TYPE_I8";
        case IL2CPP_TYPE_U8: return "IL2CPP_TYPE_U8";
        case IL2CPP_TYPE_R4: return "IL2CPP_TYPE_R4";
        case IL2CPP_TYPE_R8: return "IL2CPP_TYPE_R8";
        case IL2CPP_TYPE_STRING: return "IL2CPP_TYPE_STRING";
        case IL2CPP_TYPE_PTR: return "IL2CPP_TYPE_PTR";
        case IL2CPP_TYPE_BYREF: return "IL2CPP_TYPE_BYREF";
        case IL2CPP_TYPE_VALUETYPE: return "IL2CPP_TYPE_VALUETYPE";
        case IL2CPP_TYPE_CLASS: return "IL2CPP_TYPE_CLASS";
        case IL2CPP_TYPE_VAR: return "IL2CPP_TYPE_VAR";
        case IL2CPP_TYPE_ARRAY: return "IL2CPP_TYPE_ARRAY";
        case IL2CPP_TYPE_GENERICINST: return "IL2CPP_TYPE_GENERICINST";
        case IL2CPP_TYPE_TYPEDBYREF: return "IL2CPP_TYPE_TYPEDBYREF";
        case IL2CPP_TYPE_I: return "IL2CPP_TYPE_I";
        case IL2CPP_TYPE_U: return "IL2CPP_TYPE_U";
        case IL2CPP_TYPE_FNPTR: return "IL2CPP_TYPE_FNPTR";
        case IL2CPP_TYPE_OBJECT: return "IL2CPP_TYPE_OBJECT";
        case IL2CPP_TYPE_SZARRAY: return "IL2CPP_TYPE_SZARRAY";
        case IL2CPP_TYPE_MVAR: return "IL2CPP_TYPE_MVAR";
        case IL2CPP_TYPE_CMOD_REQD: return "IL2CPP_TYPE_CMOD_REQD";
        case IL2CPP_TYPE_CMOD_OPT: return "IL2CPP_TYPE_CMOD_OPT";
        case IL2CPP_TYPE_INTERNAL: return "IL2CPP_TYPE_INTERNAL";
        case IL2CPP_TYPE_MODIFIER: return "IL2CPP_TYPE_MODIFIER";
        case IL2CPP_TYPE_SENTINEL: return "IL2CPP_TYPE_SENTINEL";
        case IL2CPP_TYPE_PINNED: return "IL2CPP_TYPE_PINNED";
        case IL2CPP_TYPE_ENUM: return "IL2CPP_TYPE_ENUM";
        default: return "UNKNOWN";
        }
    }

}
#endif