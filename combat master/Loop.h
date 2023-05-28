#include <Windows.h>
#include <string>
#include "classes.h"
#include "def.h"
#include "functions.h"
#include "utils.h"
#include "vars.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

class Looper {
public:
	static void Entities();

	inline static ImVec4 visible = { 0, 255, 0,255 }, unVisible = { 255,0,0,255 };
};
