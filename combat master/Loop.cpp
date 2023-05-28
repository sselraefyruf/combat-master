#include "Loop.h"

void Looper::Entities()
{
	uintptr_t player = *(uintptr_t*)(GetAbsoluteAddress(Sigs::PlayerRoot, 3, 7)); 
	if (!player) return;

	auto player2 = *(uintptr_t*)(player + 0xB8);
	if (!player2) return;

	auto player3 = *(uintptr_t*)(player2 + 0x18);
	if (!player3) return;

	auto playerRoot = *(uintptr_t*)(player3 + 0x10);
	if (!playerRoot) return;

	auto localplayer_class = *(uintptr_t*)(player + 0xB8);
	if (!localplayer_class) return;

	auto LocalPlayer = *(uintptr_t*)(localplayer_class + 0x8);
	if (!LocalPlayer) return;

	auto WeaponOrgBase = *(uintptr_t*)(LocalPlayer + 0x90);
	if (!WeaponOrgBase) return;

	auto WeaponBase = *(uintptr_t*)(WeaponOrgBase + 0x4E0);
	if (!WeaponBase) return;

	auto CurrentWeaponBase = *(uintptr_t*)(WeaponBase + 0x1C0);
	if (!CurrentWeaponBase) return;

	auto ReloadBase = *(uintptr_t*)(CurrentWeaponBase + 0x68);
	if (!ReloadBase) return;

	if (Variables::UnAmmo)
	{
		*(int*)(CurrentWeaponBase + 0x5C) = 9999;
	}
	else {
		*(int*)(CurrentWeaponBase + 0x5C) = 4;
	}

	if (Variables::NoReload)
	{
		*(float*)(ReloadBase + 0x10) = 0.001f;
	}
	else {
		*(float*)(ReloadBase + 0x10) = 1;
	}

	auto RecoilBase = *(uintptr_t*)(CurrentWeaponBase + 0x78);
	if (!RecoilBase) return;

	if (Variables::RapidFire)
		*(int*)(RecoilBase + 0x14) = 999;

	if (Variables::NoRecoil) {
		//RecoilKickPower
		*(float*)(RecoilBase + 0x58) = 0.0001f;

		//SpreadReduceBullets
		*(float*)(RecoilBase + 0x40) = 1000.0f;

		//SpreadReducePercent
		*(float*)(RecoilBase + 0x44) = 1000.0f;

		//CrosshairIncreasePower
		*(BYTE*)(RecoilBase + 0x74) = 0;

		//CamRecoilDuration
		*(float*)(RecoilBase + 0x68) = 0.0001f;
	}
	else {
		//RecoilKickPower
		*(float*)(RecoilBase + 0x58) = 0.03333333507f;

		//SpreadVerMinMax
		*(BYTE*)(RecoilBase + 0x30) = 3178611343;

		//SpreadHorMinMax
		*(BYTE*)(RecoilBase + 0x38) = 3178611343;

		//SpreadReduceBullets
		*(float*)(RecoilBase + 0x40) = 3;

		//SpreadReducePercent
		*(float*)(RecoilBase + 0x44) = 0.6999999881f;

		//CrosshairIncreasePower
		*(BYTE*)(RecoilBase + 0x74) = 40;

		//CamRecoilDuration
		*(float*)(RecoilBase + 0x68) = 0.200000003;
	}

	auto size = *(int*)(playerRoot + 0x18);

	Vec3 Screen, ScreenHead;

	auto Localid = get_teamid(LocalPlayer);
	if (!Localid) return;

	for (int i = 0; i < size; ++i) {
		auto entity = *(uintptr_t*)(playerRoot + 0x20 + (i * 0x8));
		if (!entity) continue;

		auto arda31 = get_transform((uintptr_t)entity);
		if (!arda31) continue;

		auto health_class = *(uintptr_t*)(entity + 0xa0);
		if (!health_class) continue;

		auto teamid = get_teamid(entity);
		if (!teamid) continue;

		auto checkVisible = get_IsVisible(entity);

		if (Localid == teamid) continue;
		printf("Team id = %i\n", teamid);

		auto visiblecheck = get_IsVisible(entity);

		auto cur_health = *(int*)(health_class + 0xb8);
		if (!cur_health) continue;

		if (cur_health <= 0) continue;

		std::string stringHealth = std::to_string(cur_health);
		char const* pCharHealth = stringHealth.c_str();

		auto ardaninpozisyonu = get_transform_position(arda31);

		auto headpos_class = *(uintptr_t*)(entity + 0x70);
		if (!headpos_class) continue;

		auto headpos = *(uintptr_t*)(headpos_class + 0x30);
		if (!headpos) continue;

		auto headposition = get_transform(headpos);
		if (!headposition) continue;

		auto headpositionzort = get_transform_position(headposition);

		headpositionzort.y += 0.2f;

		if (project_world_to_screen_position(ardaninpozisyonu, &Screen) && project_world_to_screen_position(headpositionzort, &ScreenHead))
		{
			float width = fabs((ScreenHead.y - Screen.y) / 2.f);
			float h = width * 2.f;

			float BoxHeight = Screen.y - ScreenHead.y;
			float BoxWidth = BoxHeight / 2.4f;

			float x1 = ScreenHead.x - (BoxWidth / 2.f);
			float y1 = ScreenHead.y;
			float x2 = x1 + BoxWidth;
			float y2 = y1 + BoxHeight;

			float h_x1 = x2 + 3;
			float h_x2 = h_x1 + BoxWidth / 6;

			float h_y1 = y1;
			float h_y2 = y2;

			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(h_x1 - 1, h_y1 - 1), ImVec2(h_x1 + (BoxWidth / 6) + 1, h_y1 + (cur_health * (h_y2 - h_y1) / 100) + 1), ImColor(0, 255, 0));
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(h_x1 + 1, h_y1 + 1), ImVec2(h_x1 + (BoxWidth / 6) - 1, h_y1 + (cur_health * (h_y2 - h_y1) / 100) - 1), ImColor(0, 255, 0));
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(h_x1, h_y1), ImVec2(h_x1 + (BoxWidth / 6), h_y1 + (cur_health * (h_y2 - h_y1) / 100)), ImColor(0, 255, 0));

			if(visiblecheck)
			   two_d_bounding(ScreenHead.x, ScreenHead.y, width, h, visible, 1);
			if(!visiblecheck)
				two_d_bounding(ScreenHead.x, ScreenHead.y, width, h, unVisible, 1);
		}
	}


}