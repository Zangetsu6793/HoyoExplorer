#include "main.h"

BOOL WINAPI DllMain(const HMODULE instance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(instance);
		CreateThread(nullptr, 0, MainThread, instance, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:

		break;
	}


	return true;

}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitle("[ HoyoExp ]");
	FILE* f;

	//freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, ("CONOUT$"), "w", stderr);
	//AttachConsole(GetCurrentProcessId());

	fprintf(stderr, "Starting \n");
	
	//Utils::Print("Starting");

	
	auto scanner = MemoryScanner::SigScanner::SigScanner();


	

	Test();
	//Test2();


	
	
	//Byte[007] = 0x30 (int: 48)
	return 1;
}


void DumpBytesAsInts(std::string Pattern)
{
	auto scanner = MemoryScanner::SigScanner::SigScanner();
	uintptr_t baseAddress = scanner.ScanForSignature(Pattern);
	if (!baseAddress)
	{
		std::cerr << "BaseAddress empty" << std::endl;
		return;
	}

	unsigned char* bytePtr = reinterpret_cast<unsigned char*>(baseAddress);

	for (size_t i = 0; i < 150; ++i) {
		// Print byte as hex and as int
		std::cerr << "Byte[" << std::setw(3) << i << "] = "
			<< "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)bytePtr[i]
			<< " (int: " << std::dec << (int)bytePtr[i] << ")\n";
	}
}

inline void Test2()
{
	//wchar_t* wstr = reinterpret_cast<wchar_t*>(il2cppString->startChar);

	//auto ver = UnityEngine::Application::get_unityVersion();
	//wchar_t* wstr = reinterpret_cast<wchar_t*>(ver->chars);
	//fprintf(stderr, "wstr: %ls\n", wstr);

	/*Il2CppString* tag = UnityEngine::Marshal::PtrToStringAnsi("Player");
	wchar_t* wstr = reinterpret_cast<wchar_t*>(tag->chars);
	fprintf(stderr, "wstr: %ls\n", wstr);*/

	//auto player = UnityEngine::GameObject::FindWithTag(tag);
	//fprintf(stderr, "Player_Gameobject_Pntr: %p\n", player);

	system("pause");
	int i = 0;
	auto scene = UnityEngine::SceneManager::GetSceneAt(0);
	auto scene2 = UnityEngine::SceneManager::GetSceneAt(1);
	auto activeS = UnityEngine::SceneManager::GetActiveScene();
	fprintf(stderr, "Active Scene %p Scene 0 %p  Scene 0 %p Count %d\n",activeS,scene, scene2, UnityEngine::SceneManager::get_sceneCount());

	auto cam = UnityEngine::Object::FindObjectOfType<UnityEngine::Camera>();
	auto obj = cam->get_gameObject();
	auto tg = obj->get_scene();
	fprintf(stderr, "Cam %p Scene %p Obj %p ToStr %s\n", cam, tg, obj, Utils::Utf16ToUtf8(obj->ToString()->chars,-1).c_str());

	/*auto sceneName = scene->get_name();
	fprintf(stderr, "Scene Name called %p \n", scene);
	auto wstr = reinterpret_cast<wchar_t*>(sceneName->chars);
	fprintf(stderr, "Scene %p  SceneName %p\n", scene, sceneName);*/

	/*auto v = UnityEngine::StackTraceUtility::projectFolder();
	if (v) {
		Il2CppString* str = reinterpret_cast<Il2CppString*>(*v);
		fprintf(stderr, "Pointer %p\n", v);
		fprintf(stderr, "Pointer %p  Value %s\n", v, Utils::Utf16ToUtf8(str->chars,-1).c_str());
	}
	else {
		fprintf(stderr, "Pointer is nullptr or chars is nullptr.\n");
	}*/
}



inline void Test()
{
	system("pause");
	auto d = Dumpers::Dumper::Init();
	/*d.DumpAllLoadedAssemblies();
	d.DumpAssemblyImage("UnityEngine.CoreModule");
	d.DumpAssemblyImage("UnityEngine.AnimationModule");
	d.DumpAssemblyImage("UnityEngine.PhysicsModule");*/

	//d.DumpAllMethods();

	d.FullDump();
	//system("pause");
	//d.DumpAssemblyImageUsingAPI("Assembly-CSharp");//Assembly-CSharp
	
	
}

