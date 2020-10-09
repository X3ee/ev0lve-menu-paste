#include "menu_kamaz.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "Config.h"

void c_menu::menu()
{

	ui::SetNextWindowSizeConstraints(ImVec2(730, 480), ImVec2(730, 480));
	ui::Begin("RA1NSHOT", 0, ImGuiWindowFlags_NoTitleBar || ImGuiWindowFlags_NoBackground || ImGuiWindowFlags_NoResize || ImGuiWindowFlags_NoScrollbar || ImGuiWindowFlags_NoScrollWithMouse);
	auto p = ui::GetCursorScreenPos();
	auto draw = ui::GetWindowDrawList();
	auto w = ui::GetWindowWidth();
	auto h = ui::GetWindowHeight();

	static int tab = 0;
	static int sub_tab_rage = 0;
	ui::TabButton_icon("RAGE", "A", &tab, 0, 5);
	ui::TabButton_icon("ANTI AIM", "B", &tab, 1, 5);
	ui::TabButton_icon("VISUALS", "E", &tab, 2, 5);
	ui::TabButton_icon("MISC", "D", &tab, 3, 5);
	ui::TabButton_icon("SKINS", "I", &tab, 4, 5);
	if (tab == 0)
	{
		ui::PushFont(ui::GetIO().Fonts->Fonts[1]);
		ui::TabButton("General", &sub_tab_rage, 0, 7);
		ui::PopFont();
		ui::PushFont(ui::GetIO().Fonts->Fonts[2]);
		ui::TabButton("I", &sub_tab_rage, 1, 7);
		ui::TabButton("J", &sub_tab_rage, 2, 7);
		ui::TabButton("K", &sub_tab_rage, 3, 7);
		ui::TabButton("L", &sub_tab_rage, 4, 7);
		ui::TabButton("M", &sub_tab_rage, 5, 7);
		ui::TabButton("N", &sub_tab_rage, 6, 7);
		ui::PopFont();
		if (sub_tab_rage == 0)
		{
			ui::SetNextWindowPos(ImVec2(p.x + 100, p.y + 100));
			ui::BeginChild("Ragebot", ImVec2(290, 300), false, ImGuiWindowFlags_NoScrollWithMouse);

			static bool r1 = false;
			ui::Checkbox("Enabled", &r1, 40);

			static bool r2 = false;
			ui::Checkbox("Auto shot", &r2, 40);

			static bool r3 = false;
			ui::Checkbox("Auto scope", &r3, 40);

			static bool r4 = false;
			ui::Checkbox("Backtrack", &r4, 40);

			static bool r5 = false;
			ui::Checkbox("On shot priority", &r5, 40);

			static bool r6 = false;
			ui::Checkbox("Delay shot", &r6, 40);

			static bool r7 = false;
			ui::Checkbox("Resolver", &r7, 40);

			static bool r8 = false;
			ui::Checkbox("Adaptive hitchance", &r8, 40);

			//static bool r9 = false;
			//ui::Checkbox("Force baim", &r9, 40);
			//static int key_forcebaim = 0;
			//static int forcebaim_style = 0;
			//ui::Keybind("Force baim", &key_forcebaim, &forcebaim_style);
			//ui::SliderInt("slider", &slider, 0, 100, "%d%");

			//static int combo = 0;
			//ui::SingleSelect("combo", &combo, {"Kamaz", "ya", "ch1rkov"});

			//static std::unordered_map<int, bool> multicombo[3];

			//ui::MultiSelect("MultiCombo", multicombo, { "1", "2", "3" });
			//static float col[4];
			//static float col2[4];
			//ui::Checkbox("checkbox 2", &r1, 65);
			//ui::ColorEdit4("##sds", col, 44, ImGuiWindowFlags_NoTitleBar);
			//ui::ColorEdit4("##sd", col2, 22, ImGuiWindowFlags_NoTitleBar);

			//ui::Button("test", ImVec2(100, 20));

			//static int key = 0;
			//static int keystyle = 0;
			//ui::Checkbox_keybind("checkbox1", &r1);
			//ui::Keybind("keybind test", &key, &keystyle);

			ui::EndChild();
		}

	}

	if (tab == 1)
	{

		ui::SetNextWindowPos(ImVec2(p.x + 100, p.y + 100));
		ui::BeginChild("AA", ImVec2(290, 300), false, ImGuiWindowFlags_NoScrollWithMouse);

		static bool a1 = false;
		ui::Checkbox("Enabled", &a1, 40);

		static int combo_pitch = 0;
		ui::SingleSelect("Pitch", &combo_pitch, { "None", "Down", "Up", "Zero", "Random" });

		static int combo_yaw = 0;
		ui::SingleSelect("Yaw", &combo_yaw, { "None", "Backward", "At target", "Spinbot", "Random" });

		static int combo_desync = 0;
		ui::SingleSelect("Desync type", &combo_yaw, { "None", "Static", "Balance", "Low delta" });

		static int slider_body_lean = 0;
		ui::SliderInt("Body lean", &slider_body_lean, -45, 45, "%d%");

		static int slider_inverted_body_lean = 0;
		ui::SliderInt("Inverted body lean", &slider_inverted_body_lean, -45, 45, "%d%");

		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(p.x + 400, p.y + 100));
		ui::BeginChild("Fake lag", ImVec2(290, 300), false, ImGuiWindowFlags_NoScrollWithMouse);

		static bool f1 = false;
		ui::Checkbox("Enabled", &f1, 40);

		static int combo_fakelag_type = 0;
		ui::SingleSelect("Pitch", &combo_fakelag_type, { "Off", "Factor", "Fluctuate" });

		static int slider_fakelag_amount = 0;
		ui::SliderInt("Inverted body lean", &slider_fakelag_amount, 0, 14, "%d%");

		static bool f2 = false;
		ui::Checkbox("On shot fake lag", &f2, 40);

		//static int key_slowwalk = 0;
		//static int keystyle_slowwalk = 0;
		//ui::Checkbox_keybind("Slowwalk", &f1);
		//ui::Keybind("Slowwalk", &key_slowwalk, &keystyle_slowwalk);

		ui::EndChild();
	}

	if (tab == 2)
	{
		ui::SetNextWindowPos(ImVec2(p.x + 100, p.y + 100));
		ui::BeginChild("ESP", ImVec2(290, 300), false, ImGuiWindowFlags_NoScrollWithMouse);

		static bool p1 = false;
		ui::Checkbox("Enable", &p1, 40);

		static bool p2 = false;
		ui::Checkbox("On Dormant", &p2, 40);

		static bool p3 = false;
		ui::Checkbox("Box", &p3, 40);

		static float box[4];
		ui::ColorEdit4("##sds", box, 22, ImGuiWindowFlags_NoTitleBar);

		static bool p4 = false;
		ui::Checkbox("Skeleton", &p4, 40);

		static float skeleton[4];
		ui::ColorEdit4("##sds1", skeleton, 22, ImGuiWindowFlags_NoTitleBar);

		static bool p5 = false;
		ui::Checkbox("Health", &p5, 40);

		static bool p6 = false;
		ui::Checkbox("Override health", &p6, 40);

		static float health[4];
		ui::ColorEdit4("##sds2", health, 22, ImGuiWindowFlags_NoTitleBar);

		static bool p7 = false;
		ui::Checkbox("Name", &p7, 40);

		static float name[4];
		ui::ColorEdit4("##sds3", name, 22, ImGuiWindowFlags_NoTitleBar);

		static bool p8 = false;
		ui::Checkbox("Weapon", &p8, 40);

		static float weapon[4];
		ui::ColorEdit4("##sds4", weapon, 22, ImGuiWindowFlags_NoTitleBar);

		static bool p9 = false;
		ui::Checkbox("Ammo", &p9, 40);

		static float ammo[4];
		ui::ColorEdit4("##sds5", ammo, 22, ImGuiWindowFlags_NoTitleBar);

		static std::unordered_map<int, bool> multicombo[3];

		ui::MultiSelect("Flags", multicombo, { "Armor", "Scoped", "Flashed", "Can Hit", "Resolver Mode", "Choke Count" });

		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(p.x + 400, p.y + 100));
		ui::BeginChild("World", ImVec2(290, 300), false, ImGuiWindowFlags_NoScrollWithMouse);

		static bool w1 = false;
		ui::Checkbox("Tracers", &w1, 40);

		static float tracers[4];
		ui::ColorEdit4("##sds6", tracers, 22, ImGuiWindowFlags_NoTitleBar);

		static bool w2 = false;
		ui::Checkbox("Effects", &w2, 40);

		static bool w3 = false;
		ui::Checkbox("Night Mode", &w3, 40);

		static float nightmode[4];
		ui::ColorEdit4("##sds7", nightmode, 22, ImGuiWindowFlags_NoTitleBar);

		static bool w4 = false;
		ui::Checkbox("Force Crosshair", &w4, 40);

		static bool w5 = false;
		ui::Checkbox("Kill Effect", &w5, 40);

		static int slider_world_fov = 0;
		ui::SliderInt("World fov", &slider_world_fov, 90, 145, "%d%");

		static int slider_viewmodel_fov = 0;
		ui::SliderInt("Viewmodel fov", &slider_viewmodel_fov, 68, 145, "%d%");

		ui::EndChild();
	}

	if (tab == 3)
	{
		ui::SetNextWindowPos(ImVec2(p.x + 100, p.y + 100));
		ui::BeginChild("Misc", ImVec2(290, 300), false, ImGuiWindowFlags_NoScrollWithMouse);

		static bool m1 = false;
		ui::Checkbox("Bunny hop", &m1, 40);

		static bool m2 = false;
		ui::Checkbox("Auto strafe", &m2, 40);

		static bool m3 = false;
		ui::Checkbox("Knife bot", &m3, 40);

		static bool m4 = false;
		ui::Checkbox("Clantag", &m4, 40);

		static bool m5 = false;
		ui::Checkbox("Warermark", &m5, 40);

		static bool m6 = false;
		ui::Checkbox("Buybot", &m6, 40);

		static int combo_first_weapon = 0;
		ui::SingleSelect("First weapon", &combo_first_weapon, { "SCAR-20", "SSG-08", "AWP", "AK-47/M4A4" });

		static int combo_second_weapon = 0;
		ui::SingleSelect("Second weapon", &combo_second_weapon, { "Eagle/R8", "CZ75/Tec-9", "P250", "Dual Berettas" });

		static int combo_third_weapon = 0;
		ui::SingleSelect("Third weapon", &combo_third_weapon, { "Helmet", "Body", "He Grenade", "Molotov", "Smoke", "Taser", "Defuse Kit" });

		ui::EndChild();
	}
	ui::End();
}