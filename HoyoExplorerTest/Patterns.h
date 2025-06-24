#pragma once

namespace Patterns
{
	namespace ZenlessZoneZero
	{
		auto GetTypeDefinitionFromIndex = "55 41 57 48 0F BD EC";
		auto GetClassNamespaze          = "F6 81 ? ? ? ? ? 75 ? 0F B6 81 ? ? ? ? C1 E0 10 3D 00 00 0F 00 75 ? 8B 89 ? ? ? ? 48 8B 05";
		auto GetMethodsInfo             = "E9 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 93"; //Might be unstable as function is short and this leaves it
		auto GetMethodsInfoAlt          = "55 48 0F BD C3"; //Alt to one above as it just jumpts to this
		auto GetMethodName              = "55 56 57 53 48 83 EC 28 48 8D 6C 24 ? 48 C7 45 ? ? ? ? ? 48 89 CE 48 8B 0D ? ? ? ? FF 15 ? ? ? ? 48 8B 46";
		auto GetFieldInfo               = "55 56 57 48 83 EC 30 48 8D 6C 24 ? 48 C7 45 ? ? ? ? ? 48 85 D2 74 ? 48 89 D6 48 89 CF 48 8B 02 48 85 C0 74 ? 48 83 C0 20";
		auto GetTypeDefinition          = "E9 ? ? ? ? 48 E0 ? F3 12 97"; //Might be unstable as function is short and this leaves it;
		auto GetTypeDefinitionAlt       = "55 41 57 48 0F BD EC"; //Alt to one above as it just jumpts to this
		auto FromIl2CppType             = "56 57 48 83 EC 28 48 89 CF 0F BE 49";
		auto GetMethodReturnType        = "55 41 56 56 57 53 48 83 EC 30 48 8D 6C 24 ? 48 C7 45 ? ? ? ? ? 48 89 CE 48 8B 0D";
		auto GetMethodParamType         = "55 41 57 41 56 41 55 41 54 56 57 53 48 83 EC 38 48 8D 6C 24 ? 48 C7 45 ? ? ? ? ? 48 89 CE 48 8B 0D";
	}

	namespace GenshinImpact
	{

	}
}

