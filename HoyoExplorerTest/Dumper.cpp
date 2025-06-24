#include "Dumper.h"

namespace Dumpers
{
	Dumper::Dumper() {

	}

	Dumper Dumper::Init()
	{
		auto d = Dumpers::Dumper();
		d.SetupOffsets();
		d.SetupExternalCalls();
		d.CreateDummyDll();
		//std::cerr << "setup\n";
		return d;
	}

	Dumper::~Dumper() {
		// Cleanup if necessary
	}

	std::string Dumper::getClassInfoFromIndex(int index)
	{
		Il2CppClass* classPntr = Dumper::getTypeInfoFromIndex(index);
		if (classPntr) {
			return std::string(Dumper::getClassNameFromKlass(classPntr));
		}
		return "";
	}

	void Dumper::CreateDummyDll()
	{
		if (Utils::CheckFileExists("DumperDummy.dll"))
			return;
		std::ostringstream dummyCode;

		dummyCode << "using System;\n\n"
			<< "namespace DumperDummyDll\n"
			<< "{\n"
			<< "\tpublic class AddressAttribute : Attribute\n"
			<< "\t{\n"
			<< "\t\tpublic string RVA;\n\n"
			<< "\t\tpublic string Offset;\n\n"
			<< "\t\tpublic string VA;\n\n"
			<< "\t\tpublic string Slot;\n"
			<< "\t}\n"
			<< "}\n";
		Utils::writeToCSharpFile(dummyCode, "DummyDll");
	}

	void Dumper::FullDump()
	{
		system("cls");
		Utils::Print("Wait untill login screen (Will crash if you do not)\n");
		system("pause");
		


		std::unordered_map<std::string, std::vector<int>> namespaceToClasses;
		std::unordered_map<std::string, std::vector<int>> classNameToInts;



		fprintf(stderr, "Init\n");
		std::ostringstream csharpCode;
		int toDump = 0;

		if (Utils::GetProcName() == "ZenlessZoneZero.exe")
			toDump = 68653;
		else
			toDump = 100;
		for (int i = 0; i <= toDump; i++)
		{
			if (i > 500000000)
				fprintf(stderr, "%d\n", i);
			Il2CppClass* classPntr = GetTypeDefFromIndex(i);
			//fprintf(stderr, "ClassPntr: %p\n", classPntr);
			if (classPntr != NULL)
			{
				std::string name = getClassNameFromKlass(classPntr);
				std::string namespaze = getClassNamespaze(classPntr);


				
				if (namespaceToClasses.count(namespaze)) {
					namespaceToClasses[namespaze].push_back(i);
				}
				else {
					namespaceToClasses[namespaze] = std::vector<int>{ i };
				}

				if (classNameToInts.count(name)) {
					classNameToInts[name].push_back(i);
				}
				else {
					classNameToInts[name] = std::vector<int>{ i };
				}


			}
			else
			{
				fprintf(stderr, "classPntr is nullptr\n");
			}




		}
		fprintf(stderr, "Init done\n");
		fprintf(stderr, "Starting Collection(May take a while)\n");
		
		for (const auto& entry : namespaceToClasses)
		{
			
			Sleep(1);
			std::string key = entry.first;

			//system("pause");
			csharpCode << ProcessNamespace(key, namespaceToClasses, classNameToInts);

		}
		fprintf(stderr, "Collection Done\n");
		fprintf(stderr, "Writing to file (May take a while)\n");
		Utils::writeToCSharpFile(csharpCode);
		fprintf(stderr, "Writing done\n");
		fprintf(stderr, "Exit?\n");

		system("pause");

		std::string codeString = csharpCode.str();


		fprintf(stderr, "\n");


		std::this_thread::sleep_for(std::chrono::seconds(1));
		exit(1);
	}

	void Dumper::FullDumpUsingAPI()
	{
	}

	void Dumper::DumpAssemblyImageUsingAPI(std::string AssemblyName)
	{
		std::ostringstream csharps;
		
		// Typedefs for Il2Cpp API functions
		typedef const Il2CppImage* (*il2cpp_assembly_get_image_t)(const Il2CppAssembly* assembly);
		typedef const Il2CppAssembly* (*il2cpp_domain_assembly_open_t)(void* domain, const char* name);

		typedef size_t (*il2cpp_image_get_class_count_t)(const Il2CppImage* image);
		typedef const Il2CppClass* (*il2cpp_image_get_class_t)(const Il2CppImage* image, size_t index);

		typedef const char* (*il2cpp_class_get_name_t)(Il2CppClass* klass);
		typedef const char* (*il2cpp_class_get_namespace_t)(Il2CppClass* klass);
		typedef Il2CppClass* (*il2cpp_class_get_parent_t)(Il2CppClass* klass);
		
		// Get the base address of GameAssembly.dll
		HMODULE gameAssembly = GetModuleHandleA("GameAssembly.dll");
		if (!gameAssembly) {
			std::cerr << "Failed to load GameAssembly.dll!" << std::endl;
			return;
		}
		
		// Resolve function addresses
		auto il2cpp_assembly_get_image = (il2cpp_assembly_get_image_t)GetProcAddress(gameAssembly, "il2cpp_assembly_get_image");
		auto il2cpp_domain_assembly_open = (il2cpp_domain_assembly_open_t)GetProcAddress(gameAssembly, "il2cpp_domain_assembly_open");
		auto il2cpp_image_get_class_count = (il2cpp_image_get_class_count_t)GetProcAddress(gameAssembly, "il2cpp_image_get_class_count");
		auto il2cpp_image_get_class = (il2cpp_image_get_class_t)GetProcAddress(gameAssembly, "il2cpp_image_get_class");
		auto il2cpp_class_get_name = (il2cpp_class_get_name_t)GetProcAddress(gameAssembly, "il2cpp_class_get_name");
		auto il2cpp_class_get_namespace = (il2cpp_class_get_namespace_t)GetProcAddress(gameAssembly, "il2cpp_class_get_namespace");
		auto il2cpp_class_get_parent = (il2cpp_class_get_parent_t)GetProcAddress(gameAssembly, "il2cpp_class_get_parent");
		
		// Validate API functions
		if (!il2cpp_assembly_get_image || !il2cpp_domain_assembly_open || !il2cpp_image_get_class_count ||
			!il2cpp_image_get_class || !il2cpp_class_get_name || !il2cpp_class_get_namespace || !il2cpp_class_get_parent) {
			std::cerr << "Failed to resolve required Il2Cpp exports!" << std::endl;
			return;
		}

		// Open the assembly
		void* domain = nullptr;  // Initialize this using the correct API, if available.
		const Il2CppAssembly* assembly = il2cpp_domain_assembly_open(domain, AssemblyName.c_str());
		if (!assembly) {
			std::cerr << "Failed to open assembly: " << AssemblyName << std::endl;
			return;
		}
		
		// Get the image from the assembly
		const Il2CppImage* image = il2cpp_assembly_get_image(assembly);
		if (!image) {
			std::cerr << "Failed to get image from assembly!" << std::endl;
			return;
		}
		
		// Get class count
		size_t countPtr = il2cpp_image_get_class_count(image);
		if (!countPtr) {
			std::cerr << "Failed to retrieve class count!" << std::endl;
			return;
		}

		size_t count = countPtr;

		
		// Iterate through classes
		for (size_t i = 0; i < count; ++i) {
			auto klass = (Il2CppClass*)il2cpp_image_get_class(image, i);
			if (!klass) continue;

			const char* klassName = il2cpp_class_get_name(klass);
			const char* klassNamespace = il2cpp_class_get_namespace(klass);
			auto parent = il2cpp_class_get_parent(klass);
			const char* parentName = "";
			if (parent)
				 parentName = il2cpp_class_get_name(parent);
			
			csharps << "Image: " << AssemblyName
				<< "  NameSpace: " << (klassNamespace ? klassNamespace : "(null)")
				<< "  ClassName: " << (klassName ? klassName : "(null)")
				<< "  Parent: " << parentName
				<< std::endl;

			//std::cerr << csharps.str();
			//csharps.str(""); // Clear stream
			//csharps.clear(); // Reset state
		}
		Utils::writeToCSharpFile(csharps, AssemblyName);
	}


	void Dumper::DumpAllMethods()
	{

		std::ostringstream cCode;

		for (int i = 0; i < 50000; i++)
		{

			Il2CppClass* classPntr = getTypeInfoFromIndex(i);
			auto cName = getClassNameFromKlass(classPntr);
			sigMan = SigManagers::ObfSigMan::Init(cName);
			//fprintf(stderr, "BeoreBuild\n");
			cCode << BuildClassMethodsString(classPntr);
			//fprintf(stderr, "AfterBuild\n");
			fprintf(stderr, "Class: %s Sig: %s\n", cName, sigMan.GetSig().c_str());
			//delete &sigMan;

		}

		Utils::writeToCSharpFile(cCode);
	}

	void Dumper::DumpAllLoadedAssemblies()
	{
		size_t size = 0;
		auto assemblies = il2cpp_domain_get_assemblies(&size);
		std::ostringstream csharp;
		for (int i = 0; i <= size; i++)
		{
			auto name = il2cpp_get_assembly_name((intptr_t*)assemblies[i]);
			DumpAssemblyImage(name);
		}

	}

	void Dumper::DumpAssemblyImage(std::string AssemblyName)
	{
		std::ostringstream csharps;
		auto name = AssemblyName.c_str();
		auto ass = il2cpp_domain_assembly_open(name);
		//fprintf(stderr, "Ass: %p\n", ass);
		auto img = ass->get_Image();
		//fprintf(stderr, "Image: %p\n", img);
		auto typeCount = il2cpp_get_image_type_count(img);
		//fprintf(stderr, "Image: %p  Name: %-40s  TypeCount: %zu\n", img, AssemblyName, typeCount);
		auto typeStart = il2cpp_get_image_type_start(img);
		auto endAddress = typeStart + (typeCount * sizeof(int32_t));

		for (int k = 1; k < typeCount; k++)
		{
			auto v405 = typeStart + k;

			//fprintf(stderr, "typestart %d Index : %d", typeStart ,v405);
			auto klass = getTypeInfoFromIndex(v405);
			const char* nameS = getClassNamespaze(klass);
			const char* cName = getClassNameFromKlass(klass);
			const char* parentClass = "";

			auto v5 = *(unsigned int*)(klass + 164);
			if (!v5)
				parentClass = "";
			else
			{
				auto v3 = *(int64_t*)(*Utils::get_offset<uint64_t*>(0x45360D0)) + v5;
				//fprintf(stderr, "V3 %p\n", v3);
				//parentClass = getClassNameFromKlass((Il2CppClass*)v3);

			}


			csharps << "Image: " << AssemblyName
				<< "  Pointer: " << v405
				<< "  value: " << v405
				<< "  ClassPntr: " << klass
				<< "  NameSpace: " << nameS
				<< "  ClassName: " << cName
				<< "  Parent: " << parentClass
				<< std::endl;
			auto dup = Dumpers::Dumper::Init();
			csharps << dup.BuildClassFieldsString((Il2CppClass*)klass).c_str() << std::endl;
			csharps << dup.BuildClassMethodsString((Il2CppClass*)klass).c_str() << std::endl;
			//fprintf(stderr, "Image: %s  Pointer: %p  value: %d  ClassPntr: %p NameSpace: %s ClassName: %s\n", "UnityEngine.CoreModule", v405, v405, klass, nameS, cName);
			//fprintf(stderr, "Pointer: %p  value: %d\n", v405,v405);

			//fprintf(stderr, "%s", dup.BuildClassMethodsString((Il2CppClass*)klass).c_str());
			//fprintf(stderr, "\n");

		}
		Utils::writeToCSharpFile(csharps, AssemblyName);
	}

	//Private
	const char* Dumper::getClassNameFromKlass(Il2CppClass* klass)
	{
		const char* strAtOffset = reinterpret_cast<const char*>(*reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(klass) + offsets::NameFromKlassOffset));
		return strAtOffset;
	}
	Il2CppClass* Dumper::getTypeInfoFromIndex(int32_t index) {
		return GetTypeDefFromIndex ? GetTypeDefFromIndex(index) : nullptr;
	}

	const MethodInfo* Dumper::getMethods(Il2CppClass* klass, void** iter) {
		return GetMethodsInfoFunct ? GetMethodsInfoFunct(klass, iter) : nullptr;
	}

	FieldInfo* Dumper::getFields(Il2CppClass* klass, void** iter) {
		return GetClassFieldsInfo ? GetClassFieldsInfo(klass, iter) : nullptr;
	}


	uint64_t Dumper::getMethodName(uint64_t a1) {
		return GetMethodNameFunct ? GetMethodNameFunct(a1) : 0;
	}

	const char* Dumper::getClassNamespaze(Il2CppClass* klass) {
		return GetClassNamespazeFunct ? GetClassNamespazeFunct(klass) : nullptr;
	}

	int64_t Dumper::getMethodReturnType(int64_t methodInfo) {
		return GetMethodReturnTypeFunct ? GetMethodReturnTypeFunct(methodInfo) : 0;
	}

	int64_t Dumper::getMethodParamType(int64_t methodInfo) {
		return GetMethodParamTypeFunct ? GetMethodParamTypeFunct(methodInfo) : 0;
	}

	const char* Dumper::getData(FieldInfo* field) {
		return GetDataFromFieldInfo ? GetDataFromFieldInfo(field) : nullptr;
	}

	Il2CppClass* Dumper::fromIl2CppType(const Il2CppType* type, bool throwOnError) {
		return FromIl2CppTypeFunct ? FromIl2CppTypeFunct(type, throwOnError) : nullptr;
	}

	void Dumper::SetupOffsets()
	{
		std::string procName = Utils::GetProcName();
		if (gameOffsets.find(procName) != gameOffsets.end()) {
			Offsets offsetsConfig = gameOffsets[procName];
			offsets::GetTypeOffset = offsetsConfig.GetTypeOffset;
			offsets::GetClassNamespazeOffset = offsetsConfig.GetClassNamespazeOffset;
			offsets::GetMethodsInfoOffset = offsetsConfig.GetMethodsInfoOffset;
			offsets::GetMethodNameOffset = offsetsConfig.GetMethodNameOffset;
			offsets::GetFieldInfoOffset = offsetsConfig.GetFieldInfoOffset;
			offsets::FromIl2CppTypeOffset = offsetsConfig.FromIl2CppTypeOffset;
			offsets::GetMethodReturnTypeOffset = offsetsConfig.GetMethodReturnTypeOffset;
			offsets::GetMethodParamTypeOffset = offsetsConfig.GetMethodParamTypeOffset;

			offsets::NameFromKlassOffset = offsetsConfig.NameFromKlassOffset;
			offsets::classParentOffset = offsetsConfig.classParentOffset;

			offsets::fieldNameOffset = offsetsConfig.fieldNameOffset;
			offsets::fieldNameXor = offsetsConfig.fieldNameXor;
			offsets::fieldFlagsXor = offsetsConfig.fieldFlagsXor;
			offsets::fieldIl2CppTypeOffset = offsetsConfig.fieldIl2CppTypeOffset;
			offsets::fieldTypeOffset = offsetsConfig.fieldTypeOffset;
			offsets::fieldOffsetOffset = offsetsConfig.fieldOffsetOffset;
			offsets::fieldOffsetShift = offsetsConfig.fieldOffsetShift;
			offsets::fieldStaticAddrOffset = offsetsConfig.fieldStaticAddrOffset;


			offsets::methodFlagsOffset = offsetsConfig.methodFlagsOffset;
			offsets::methodArgsCountOffset = offsetsConfig.methodArgsCountOffset;
			offsets::methodParameterNameOffset = offsetsConfig.methodParameterNameOffset;
			
		}
		else {
			// Handle unknown game case
			//throw std::runtime_error("Unknown game executable!");




		}
	}

	void Dumper::SetupExternalCalls()
	{
		GetTypeDefFromIndex = (GetTypeInfoFromTypeDefinitionIndex)Utils::get_offset(offsets::GetTypeOffset);
		GetMethodsInfoFunct = (GetMethods)Utils::get_offset(offsets::GetMethodsInfoOffset);
		GetClassFieldsInfo = (GetFields)Utils::get_offset(offsets::GetFieldInfoOffset);
		GetMethodNameFunct = (GetMethodName)Utils::get_offset(offsets::GetMethodNameOffset);
		FromIl2CppTypeFunct = (FromIl2CppType)Utils::get_offset(offsets::FromIl2CppTypeOffset);
		GetClassNamespazeFunct = (GetClassNamespaze)Utils::get_offset(offsets::GetClassNamespazeOffset);
		GetMethodReturnTypeFunct = (GetMethodReturnType)Utils::get_offset(offsets::GetMethodReturnTypeOffset);
		GetMethodParamTypeFunct = (GetMethodParamType)Utils::get_offset(offsets::GetMethodParamTypeOffset);
	}


	std::string Dumper::BuildClassMethodsString(Il2CppClass* classPntr) {
		std::ostringstream csharpMethods;
		void* iter = nullptr;
		const MethodInfo* method;

		while ((method = Dumper::getMethods(classPntr, &iter)) != nullptr) {
			uintptr_t methodAddr = reinterpret_cast<uintptr_t>(method);

		
			const char* method_Name = nullptr;
			std::string method_Attribute = "";
			std::string method_return_type = "";

			int extraNameOffset = 8;
			if (Utils::GetProcName() != "ZenlessZoneZero.exe")
				extraNameOffset = 8;

			method_Name = reinterpret_cast<const char*>(Dumper::getMethodName(methodAddr));


			uint32_t argsCount;

			//Flags
			uint16_t flags = *reinterpret_cast<uint16_t*>(methodAddr + offsets::methodFlagsOffset);
			argsCount = *reinterpret_cast<uint8_t*>(methodAddr + offsets::methodArgsCountOffset);

			uintptr_t return_type_offset;		

			return_type_offset = Dumper::getMethodReturnType(methodAddr);
			const char* return_type;
			Il2CppClass* klass;

			if (return_type_offset != 0x0) {
				klass = Dumper::fromIl2CppType((Il2CppType*)return_type_offset, false);
				return_type = Dumper::getClassNameFromKlass(klass);
			}
			else {
				return_type = "unknown";
			}

			//fprintf(stderr, "here1\n");
			std::ostringstream paramtype;
			for (int i = 0; i < argsCount; i++)
			{
				//fprintf(stderr, "args1\n");
				uint64_t methodParamTypeInfo = 0;

				methodParamTypeInfo = Dumper::getMethodParamType((methodAddr));

				uintptr_t** v14 = reinterpret_cast<uintptr_t**>(methodParamTypeInfo + 0);

				auto pointer = *(v14 + (i * 3));
				//fprintf(stderr, "args2\n");
				klass = Dumper::fromIl2CppType(reinterpret_cast<Il2CppType*>(pointer), false);


				int extraOffset = 8;
				if (Utils::GetProcName() != "ZenlessZoneZero.exe")
					extraOffset = 8;
				uintptr_t parameters_name = *reinterpret_cast<uintptr_t*>(methodParamTypeInfo + offsets::methodParameterNameOffset * static_cast<uint64_t>(i) + extraOffset);
				//fprintf(stderr, "args3\n");
				//fprintf(stderr, "Name %s\n", parameters_name);

				paramtype << Dumper::getClassNameFromKlass(klass) << " " << reinterpret_cast<const char*>(parameters_name);

				if ((i + 1) < argsCount)
					paramtype << " , ";
				//fprintf(stderr, "args4\n");

			}

			//fprintf(stderr, "here2\n");
			method_Attribute = resolveMethodAttributes(flags);

			sigMan.IncAtr(Il2CppTypeToString((Il2CppTypeEnum)return_type_offset).c_str());
			//sigMan.IncAtr(return_type);
			if (method_Name) {
				csharpMethods << "    " << method_Attribute << " " << return_type << " " << method_Name << "(" << paramtype.str() << ");";
			}
			else {
				csharpMethods << "    " << method_Attribute << " " << return_type << " UnknownMethod(" << paramtype.str() << " );";
			}




			//fprintf(stderr, "%s\n", csharpMethods.str().c_str());
			//system("pause");

			uintptr_t result;
			//fprintf(stderr, "here3\n");
			if (Utils::GetProcName() == "ZenlessZoneZero.exe")
				result = reinterpret_cast<uintptr_t>(method->methodPointer) - Utils::getBase();
			else
				result = reinterpret_cast<uintptr_t>(method->methodPointers) - Utils::getBase();
			//fprintf(stderr, "Before Image\n");
			Il2CppMethodPointer adjustedPointer = reinterpret_cast<Il2CppMethodPointer>(result);
			//csharpMethods << "    // Image: 0x" << il2cpp_get_image_name((uintptr_t*)classPntr) << "\n";
			csharpMethods << "    // RVA: 0x" << adjustedPointer << "\n";
			//csharpMethods << "    // SigMaker: " << sigMan.GetSig().c_str() << "\n";



		}

		return csharpMethods.str();
	}


	std::string Dumper::resolveMethodAttributes(uint16_t flags) {
		std::ostringstream attributes;

		// Access Modifier
		switch (flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) {
		case METHOD_ATTRIBUTE_PRIVATE:
			attributes << "private";
			if (&sigMan)
				sigMan.IncAtr("private");
			break;
		case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
			attributes << "protected internal";
			if (&sigMan)
				sigMan.IncAtr("protected internal");
			break;
		case METHOD_ATTRIBUTE_ASSEM:
			attributes << "internal";
			if (&sigMan)
				sigMan.IncAtr("internal");
			break;
		case METHOD_ATTRIBUTE_FAMILY:
			attributes << "protected";
			if (&sigMan)
				sigMan.IncAtr("protected");
			break;
		case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
			attributes << "private protected";
			if (&sigMan)
				sigMan.IncAtr("private protected");
			break;
		case METHOD_ATTRIBUTE_PUBLIC:
			attributes << "public";
			if (&sigMan)
				sigMan.IncAtr("public");
			break;
		default:
			attributes << "unknown";
			break;
		}

		// Additional Modifiers
		if (flags & METHOD_ATTRIBUTE_STATIC) {
			attributes << " static";
			if (&sigMan)
				sigMan.IncAtr("static");
		}
		if (flags & METHOD_ATTRIBUTE_FINAL) {
			//attributes << " final";
		}
		if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
			attributes << " virtual";
			if (&sigMan)
				sigMan.IncAtr("virtual");
		}
		if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
			//attributes << " abstract";
		}
		if (flags & METHOD_ATTRIBUTE_SPECIAL_NAME) {
			//attributes << " special";
		}
		if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
			//attributes << " pinvoke";
		}

		// Implementation Attributes
		if (flags & METHOD_IMPL_ATTRIBUTE_INTERNAL_CALL) {
			//attributes << " internal_call";
		}
		if (flags & METHOD_IMPL_ATTRIBUTE_SYNCHRONIZED) {
			//attributes << " synchronized";
		}
		if (flags & METHOD_IMPL_ATTRIBUTE_NOINLINING) {
			//attributes << " no_inlining";
		}

		return attributes.str();
	}



	std::string Dumper::BuildClassFieldsString(Il2CppClass* classPntr) {
		std::ostringstream csharpFields;
		void* iterr = nullptr;
		const FieldInfo* fieldinfo;
		std::string field_Attribute = "";

		Il2CppClass* check = nullptr;

		check = classPntr;

		// Process fields
		while ((fieldinfo = Dumper::getFields(check, &iterr)) != nullptr) {


			uintptr_t fieldAddress = reinterpret_cast<uintptr_t>(fieldinfo);

			const char* fieldName = reinterpret_cast<const char*>(*reinterpret_cast<uint64_t*>(fieldAddress + offsets::fieldNameOffset) - offsets::fieldNameXor);//
			

			uint32_t flags = *reinterpret_cast<uint32_t*>(*reinterpret_cast<uint64_t*>(fieldAddress + offsets::fieldTypeOffset) - offsets::fieldFlagsXor);

			uint64_t fieldIl2cppType;

			fieldIl2cppType = *reinterpret_cast<uint64_t*>(fieldAddress + offsets::fieldTypeOffset) - offsets::fieldIl2CppTypeOffset;

			auto klass = Dumper::fromIl2CppType((Il2CppType*)fieldIl2cppType, false);
			const char* fieldType = Dumper::getClassNameFromKlass(klass);
			auto typeS = Il2CppTypeToString((Il2CppTypeEnum)fieldIl2cppType);

			field_Attribute = resolveFieldAttributes(flags);

			

			sigMan.IncAtr(typeS.c_str());
			int offsetVal = ((*(uint32_t*)(fieldinfo + offsets::fieldOffsetOffset) + offsets::fieldOffsetShift) & 0xFFFFFF);
			intptr_t offset = offsetVal;
			//if(Utils::GetProcName() != "ZenlessZoneZero.exe")
			auto stFAddr = Utils::getBase() - (uintptr_t)(classPntr + offsets::fieldStaticAddrOffset);
			//fprintf(stderr, " offsetVal: %d static fields addr: %p\n", offsetVal, stFAddr);
			
			std::string offsetStr;
			if (offset == static_cast<uintptr_t>(-1)) {
				offsetStr = "//Offset: 0xFFFFFFFF ///Offset is -1, its thread is static";
			}
			else {

				if (field_Attribute.find("static") != std::string::npos)
				{
					//fprintf(stderr, "Here1\n");
					offsetStr = "   //[(FieldInfo_Offset: 0x" + std::to_string(offset) + "),(Il2CppClass_static_fields: 0x" + std::to_string(stFAddr) + "),(RVA: 0x" + std::to_string(offset + stFAddr)+")]\n";
				}
				else
				{
					//fprintf(stderr, "Here2\n");
					offsetStr = "   //[(FieldInfo_Offset: 0x" + std::to_string(offset) + "),(RVA: InstancePointer + 0x" + std::to_string(offset)+")]\n";
				}
				
			}


			//sigMan.IncAtr(fieldName);
		
			csharpFields << offsetStr.c_str();
			//fprintf(stderr, "FieldName: %s\n", fieldName);
			csharpFields << "    " << field_Attribute << " " << fieldType << " " << fieldName << ";";
			csharpFields << "\n";
		}
		return csharpFields.str();
	}


	std::string Dumper::resolveFieldAttributes(uint32_t flags) {
		std::ostringstream attributes;

		// Access Modifiers
		switch (flags & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK) {
		case FIELD_ATTRIBUTE_PRIVATE:
			attributes << "private";
			if (&sigMan)
				sigMan.IncAtr("private");
			break;
		case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
			attributes << "protected internal";
			if (&sigMan)
				sigMan.IncAtr("protected internal");
			break;
		case FIELD_ATTRIBUTE_ASSEMBLY:
			attributes << "internal";
			if (&sigMan)
				sigMan.IncAtr("internal");
			break;
		case FIELD_ATTRIBUTE_FAMILY:
			attributes << "protected";
			if (&sigMan)
				sigMan.IncAtr("protected");
			break;
		case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
			attributes << "private protected";
			if (&sigMan)
				sigMan.IncAtr("private protected");
			break;
		case FIELD_ATTRIBUTE_PUBLIC:
			attributes << "public";
			if (&sigMan)
				sigMan.IncAtr("public");
			break;
		default:
			attributes << "unknown";
			break;
		}

		// Additional Field Modifiers
		if (flags & FIELD_ATTRIBUTE_STATIC) {
			attributes << " static";
			if (&sigMan)
				sigMan.IncAtr("static");
		}
		if (flags & FIELD_ATTRIBUTE_INIT_ONLY) {
			//attributes << " initonly";
		}
		if (flags & FIELD_ATTRIBUTE_LITERAL) {
			//attributes << " literal";
		}
		if (flags & FIELD_ATTRIBUTE_NOT_SERIALIZED) {
			//attributes << " not serialized";
		}
		if (flags & FIELD_ATTRIBUTE_SPECIAL_NAME) {
			//attributes << " special";
		}
		if (flags & FIELD_ATTRIBUTE_PINVOKE_IMPL) {
			//attributes << " pinvoke";
		}

		// Reserved and runtime flags (optional)
		if (flags & FIELD_ATTRIBUTE_RT_SPECIAL_NAME) {
			//attributes << " rt_special";
		}
		if (flags & FIELD_ATTRIBUTE_HAS_FIELD_MARSHAL) {
			//attributes << " has_field_marshal";
		}
		if (flags & FIELD_ATTRIBUTE_HAS_DEFAULT) {
			//attributes << " has_default";
		}
		if (flags & FIELD_ATTRIBUTE_HAS_FIELD_RVA) {
			//attributes << " has_field_rva";
		}

		return attributes.str();
	}

	std::string Dumper::BuildClass(const std::string& className, const std::vector<int>& classIndices) {
		std::ostringstream csharpCode;

		csharpCode << "class " << " " << className;// " : " << classParentName;
		csharpCode << " {\n";
	
		for (int index : classIndices) {

			
			Il2CppClass* classPntr = GetTypeDefFromIndex(index);
			//fprintf(stderr, "Field\n");
			csharpCode << BuildClassFieldsString(classPntr);
			//fprintf(stderr, "Methods\n");
			csharpCode << BuildClassMethodsString(classPntr);


		}
		csharpCode << " }\n";
		std::string code = csharpCode.str();
		if (code.find("public Int32 value__;") != std::string::npos) {
			std::cout << "Found the substring!" << std::endl;

			// Replace "class" with "struct"
			std::string toReplace = "class";
			std::string replaceWith = "enum";
			size_t pos = 0;
			while ((pos = code.find(toReplace, pos)) != std::string::npos) {
				code.replace(pos, toReplace.length(), replaceWith);
				pos += replaceWith.length(); // Move past the replacement
			}

			// Clear and update the ostringstream with modified content
			csharpCode.str("");  // Clear the current content
			csharpCode.clear();  // Reset error state
			csharpCode << code;  // Update with the modified string
		}
		else {
			std::cout << "Substring not found." << std::endl;
		}


		return csharpCode.str();
	}




	std::string Dumper::ProcessNamespace(const std::string& namespaceName,
		std::unordered_map<std::string, std::vector<int>>& namespaceToClasses,
		std::unordered_map<std::string, std::vector<int>>& classNameToInts) {

		// Get the class indices for the current namespace
		std::vector<int>& classIndices = namespaceToClasses[namespaceName];

		std::ostringstream csharpCode;
		csharpCode << "namespace " << namespaceName;
		csharpCode << "{\n";

		// Track which classes have been processed to avoid duplicates
		std::unordered_map<std::string, std::vector<int>> classesToProcess;
		// Loop through all class indices for this namespace
		for (int classIndex : classIndices) {
			Il2CppClass* classPntr = GetTypeDefFromIndex(classIndex);
			if (classPntr != nullptr) {
				std::string className = getClassNameFromKlass(classPntr);

				// Collect all class indices with the same class name
				classesToProcess[className].push_back(classIndex);
			}
		}

		// Now process each group of classes with the same name
		for (const auto& entry : classesToProcess) {
			const std::string& className = entry.first;
			const std::vector<int>& indicesForClass = entry.second;

			// Build the class with all the indices that share the same class name
			csharpCode << BuildClass(className, indicesForClass);

			// Remove the processed class from classNameToInts
			classNameToInts.erase(className);
		}

		// Optionally, remove the namespace from namespaceToClasses after processing
		namespaceToClasses.erase(namespaceName);
		csharpCode << "}\n\n\n";
		//fprintf(stderr, "%s\n", csharpCode.str().c_str());
		//system("pause");
		return csharpCode.str();
	}

}