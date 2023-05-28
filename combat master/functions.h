#pragma once
#include "def.h"
#include "utils.h"
#include "vars.h"
#include "classes.h"
#include "ImGui/imgui.h"

inline auto base = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

inline bool get_IsVisible(uintptr_t entity) {
	if (!entity)
		return 0x0;
	return call<bool>(base + 0x4710600, entity);
}

// 0x471CFE0

inline __int64 get_transform(uintptr_t entity) {
	if (!entity)
		return 0x0;
	// Unity.CoreModule:Component.GetTransform()
	return call<__int64>(Sigs::get_transform_addr, entity);
}

inline Vec3 get_transform_position(__int64 transform) {
	Vec3 pos;
	// Unity.CoreModule:Transform.GetPosition()
	call<__int64>(Sigs::get_transform_pos_addr, &pos, transform);
	return pos;
}

inline __int32 get_teamid(uintptr_t entity) {
	if (!entity)
		return 0x0;
	return call<__int32>(base + 0x471CFE0, entity);
}

inline Camera* get_main_camera() {
	// UnityEngine.CoreModule:Camera.get_main()
	return call<Camera*>(Sigs::camera_get_main);
}

inline void draw_line(float x, float y, float x2, float y2, ImVec4 color, float thickness) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x2, y2), ImGui::GetColorU32(color), thickness);
}

inline void two_d_bounding(float x, float y, float w, float h, ImVec4 color, float thickness) {
	draw_line(x + (w / 2), y + h, x + (w / 2) - (w / 1), y + h, color, thickness);
	draw_line(x - (w / 2), y + h, x - (w / 2) + (w / 1), y + h, color, thickness);

	draw_line(x - (w / 2), y, x - (w / 2) + (w / 1), y, color, thickness);
	draw_line(x + (w / 2), y, x + (w / 2) - (w / 1), y, color, thickness);

	draw_line(x - (w / 2), y, x - (w / 2), y + (w / 1), color, thickness);
	draw_line(x + (w / 2), y, x + (w / 2), y + (w / 1), color, thickness);

	draw_line(x - (w / 2), y + h, x - (w / 2), y + h - (w / 1), color, thickness);
	draw_line(x + (w / 2), y + h, x + (w / 2), y + h - (w / 1), color, thickness);
}

inline bool project_world_to_screen_position(Vec3 world, Vec3* screen) {
	matrix<float> view_matrix = get_main_camera()->m_cashedptr->view_matrix;
	matrix<float> fixed;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			fixed.m_elements[i][j] = view_matrix.m_elements[j][i];
		}
	}

	float x = fixed.m_left.dot_product(world) + fixed.m_left.w;
	float y = fixed.m_up.dot_product(world) + fixed.m_up.w;
	float w = fixed.m_translation.dot_product(world) + fixed.m_translation.w;

	if (w < 0.098f) return false;

	screen->x = (ImGui::GetIO().DisplaySize.x / 2.f) * (1.f + x / w);
	screen->y = (ImGui::GetIO().DisplaySize.y / 2.f) * (1.f - y / w);
	return true;
}