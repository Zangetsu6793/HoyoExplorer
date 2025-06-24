#include "UnityCore.h"

namespace UnityEngine
{
    class Object : public Il2CppObject {
    
        std::uintptr_t cached_ptr;

        Il2CppString* ToString() {
            static const auto to_string = il2cpp_domain_assembly_open(nullptr, "UnityEngine.CoreModule")
                ->get_Image()
                ->get_Class("UnityEngine", "Object")
                ->get_Method(__func__, 0)
                ->get_function_pointer<Il2CppString * (*)(Object*)>();
            return to_string(this);
        }
    };

    class GameObject : public Object {
    
        static GameObject* Find(Il2CppString* name) {
            static const auto find = il2cpp_domain_assembly_open(nullptr, "UnityEngine.CoreModule")
                ->get_Image()
                ->get_Class("UnityEngine", "GameObject")
                ->get_Method(__func__, 1)
                ->get_function_pointer<GameObject * (*)(Il2CppString*)>();
            return find(name);
        }
    };

	class Time
	{
		Void set_timeScale(Single value)
		{
			static const auto setTimeScale = il2cpp_domain_assembly_open(nullptr, "UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Time")
				->get_Method(__func__, 1)
				->get_function_pointer<Void(*) (Single)>();

			return setTimeScale(value);
		}
		Single get_timeScale()
		{
			static const auto setTimeScale = il2cpp_domain_assembly_open(nullptr, "UnityEngine.CoreModule")
				->get_Image()
				->get_Class("UnityEngine", "Time")
				->get_Method(__func__, 0)
				->get_function_pointer<Single(*)()>();
			return setTimeScale();
		}
	};
}