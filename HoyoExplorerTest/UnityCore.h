#pragma once
#include "Il2CppStructs.h"
#include "Dumper.h"
namespace UnityEngine
{
	using Void = void;
	using Single = float;
	using String = Il2CppString*;
	class Time
	{
	public:

		static Void set_timeScale(Single value)
		{
			static const auto setTimeScale = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Time")
				->get_Method(__func__, 1)
				->get_function_pointer<Void(*) (Single)>();
			fprintf(stderr, "%p", setTimeScale);
			system("pause");
			return setTimeScale(value);
		}
		static Single get_timeScale()
		{
			static const auto setTimeScale = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Time")
				->get_Method(__func__, 0)
				->get_function_pointer<Single(*)()>();
			return setTimeScale();
		}
	};


	class Type
	{
	public:

	};



	class Object : public Il2CppObject {

		std::uintptr_t cached_ptr;
	public:
		Il2CppString* ToString() {
			static const auto to_string = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Object")
				->get_Method(__func__, 0)
				->get_function_pointer<Il2CppString * (*)(Object*)>();
			return to_string(this);
		}

		template <typename T>
		static T* FindObjectOfType() {
			std::string name = typeid(T).name();
			size_t pos = name.find_last_of(':');
			if (pos != std::string::npos) {
				name = name.substr(pos + 1);
			}

			Il2CppClass* typeClass = (Il2CppClass*)il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", name.c_str());

			if (!typeClass) {
				fprintf(stderr, "Failed to resolve class for type %s\n", name.c_str());
				return nullptr;
			}

			auto method = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Object")
				->get_Method("FindObjectOfType", 1);

			if (!method) {
				fprintf(stderr, "Failed to resolve FindObjectOfType\n");
				return nullptr;
			}

			auto find_object_of_type = method->get_function_pointer<Object * (__cdecl*)(const Il2CppClass*)>();

			if (!find_object_of_type) {
				fprintf(stderr, "FindObjectOfType function pointer is null\n");
				return nullptr;
			}

			fprintf(stderr, "FindObjectOfType pointer: %p\n", find_object_of_type);
			fprintf(stderr, "TypeClass Pointer: %p, Class Name: %s\n", typeClass, name.c_str());

			try {
				Object* result = find_object_of_type(typeClass);
				fprintf(stderr, "Result: %p\n", result);
				return (T*)result;
			}
			catch (...) {
				fprintf(stderr, "Exception occurred in FindObjectOfType.\n");
			}

			return nullptr;
		}

	};

	

	class Application
	{
	public:
		static Il2CppString* get_unityVersion()
		{
			static const auto get = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Application")
				->get_Method(__func__, 0)
				->get_function_pointer<Il2CppString * (*) ()>();
			return get();
		}
	};

	class Marshal
	{
	public:
		static Il2CppString* PtrToStringAnsi(std::string string) {
			static const auto ptrToStringAnsi = il2cpp_domain_assembly_open("mscorlib")
				->get_Image()
				->get_Class("System.Runtime.InteropServices", "Marshal")
				->get_Method(__func__, 1)
				->get_function_pointer <Il2CppString * (*) (const char*)>();
			
			return ptrToStringAnsi((string).c_str());
		}
	};


	class ValueType : public Object
	{
	public:

	};

	class Scene : public ValueType
	{
	public:
		String get_name() {
			static const auto getName = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine.SceneManagement", "Scene")
				->get_Method(__func__, 0)
				->get_instance_function<String, Scene>();
			fprintf(stderr, "getname pointer %p\n", getName);
			return getName(this);
		}
	};


	class GameObject : public Object {
	public:
		static GameObject* Find(Il2CppString* name) {
			static const auto find = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "GameObject")
				->get_Method(__func__, 1)
				->get_function_pointer<GameObject * (*)(Il2CppString*)>();
			return find(name);
		}

		Scene* get_scene()
		{
			const auto find = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "GameObject")
				->get_Method(__func__, 0)
				->get_function_pointer<Scene * (*)(GameObject*)>();
			return find(this);
		}

		static GameObject* FindWithTag(Il2CppString* tag)
		{
			static const auto findWithTag = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "GameObject")
				->get_Method(__func__, 1)
				->get_function_pointer<GameObject * (*) (Il2CppString*)>();
			fprintf(stderr, "findWithTagPntr: %p", findWithTag);
			return findWithTag(tag);
		}


	};


	class SceneManager : public Object
	{
	public:
		static Scene* GetActiveScene()
		{
			static const auto GetActScene = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine.SceneManagement", "SceneManager")
				->get_Method(__func__, 0)
				->get_function_pointer<Scene* (*) ()>();
			return GetActScene();
		}

		static int* get_sceneCount()
		{
			static const auto GetActScene = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine.SceneManagement", "SceneManager")
				->get_Method(__func__, 0)
				->get_function_pointer<int * (*) ()>();
			return GetActScene();
		}
		static Scene* GetSceneAt(int index)
		{
			static const auto GetActScene = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine.SceneManagement", "SceneManager")
				->get_Method(__func__, 1)
				->get_function_pointer<Scene* (*) (int)>();
			return GetActScene(index);
		}
	};



	class StackTraceUtility
	{
	public:
		static uintptr_t* projectFolder(){
			static const auto var = il2cpp_domain_assembly_open("Assembly-CSharp-firstpass")
				->get_Image()
				->get_Class("Mdb", "MdbDefines")
				->get_Field_Address("MDB_Token_VersionId");
			return (uintptr_t*)var;
		}

		static uintptr_t* projectFolder2() {
			static const auto var = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Application")
				->get_Method("get_unityVersion", 0)
				->get_function_pointer<Il2CppString * (*) ()>();
			fprintf(stderr, "var %p\n", var);
			return (uintptr_t*)var();
		}
	};

	class Component : public Object
	{
	public:
		GameObject* get_gameObject()
		{
			static const auto var = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Component")
				->get_Method(__func__, 0)
				->get_function_pointer<GameObject * (*) (Component*)>();
			return var(this);
		}

		String get_tag()
		{
			static const auto var = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Component")
				->get_Method(__func__, 0)
				->get_function_pointer<String (*) (Component*)>();
			return var(this);
		}
	};

	class Behaviour : public Component
	{

	};


	class Camera : public Behaviour
	{
	public:
		static Camera* get_main()
		{
			static const auto var = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Camera")
				->get_Method(__func__, 0)
				->get_function_pointer<Camera * (*) ()>();
			return var();
		}

		Scene* get_scene()
		{
			static const auto get_scene = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Camera")
				->get_Method(__func__, 0)
				->get_instance_function<Scene*, Camera>();

			return get_scene(this);
		}

		static Camera* get_current()
		{
			static const auto var = il2cpp_domain_assembly_open("UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Camera")
				->get_Method(__func__, 0)
				->get_function_pointer<Camera * (*) ()>();
			return var();
		}
	};

}