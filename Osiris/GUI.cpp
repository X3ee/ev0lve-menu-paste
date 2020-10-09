#include <fstream>
#include <functional>
#include <string>
#include <ShlObj.h>
#include <Windows.h>
#include "tabs.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"

#include "imguiCustom.h"

#include "GUI.h"
#include "Config.h"
#include "Hooks.h"
#include "SDK/InputSystem.h"

constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;




GUI::GUI() noexcept
{
    ui::CreateContext();
    ImGui_ImplWin32_Init(FindWindowW(L"Valve001", NULL));

    ui::StyleColorsDark();
    ImGuiStyle& style = ui::GetStyle();

    style.ScrollbarSize = 0.0f;

    ImGuiIO& io = ui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    
    
    if (PWSTR pathToFonts; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &pathToFonts))) {
        const std::filesystem::path path{ pathToFonts };
        CoTaskMemFree(pathToFonts);

        static constexpr ImWchar ranges[]{ 0x0020, 0xFFFF, 0 };
        fonts.tahoma = io.Fonts->AddFontFromFileTTF((path / "verdana.ttf").string().c_str(), 13.0f, nullptr, ranges);
        fonts.segoeui = io.Fonts->AddFontFromFileTTF((path / "verdanab.ttf").string().c_str(), 13.0f, nullptr, ranges);
        fonts.tabs = io.Fonts->AddFontFromMemoryTTF(Evolve, sizeof(Evolve), 32.0f, nullptr, ranges);
    }
}

//void GUI::render() noexcept
//{
//    if (!config->style.menuStyle) {
//        renderMenuBar();
//        renderAimbotWindow();
//        renderAntiAimWindow();
//        renderTriggerbotWindow();
//        renderBacktrackWindow();
//        renderGlowWindow();
//        renderChamsWindow();
//        renderEspWindow();
//        renderVisualsWindow();
//        renderSkinChangerWindow();
//        renderSoundWindow();
//        renderStyleWindow();
//        renderMiscWindow();
//        renderReportbotWindow();
//        renderConfigWindow();
//    } else {
//        renderGuiStyle2();
//    }
//}
//
//void GUI::updateColors() const noexcept
//{
//    switch (config->style.menuColors) {
//    case 0: ui::StyleColorsDark(); break;
//    case 1: ui::StyleColorsLight(); break;
//    case 2: ui::StyleColorsClassic(); break;
//    }
//}
//
//void GUI::hotkey(int& key) noexcept
//{
//    key ? ui::Text("[ %s ]", interfaces->inputSystem->virtualKeyToString(key)) : ui::TextUnformatted("[ key ]");
//
//    if (!ui::IsItemHovered())
//        return;
//
//    ui::SetTooltip("Press any key to change keybind");
//    ImGuiIO& io = ui::GetIO();
//    for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
//        if (ui::IsKeyPressed(i) && i != config->misc.menuKey)
//            key = i != VK_ESCAPE ? i : 0;
//
//    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
//        if (ui::IsMouseDown(i) && i + (i > 1 ? 2 : 1) != config->misc.menuKey)
//            key = i + (i > 1 ? 2 : 1);
//}
//
//void GUI::renderMenuBar() noexcept
//{
//    if (ui::BeginMainMenuBar()) {
//        ui::MenuItem("Aimbot", nullptr, &window.aimbot);
//        ui::MenuItem("Anti aim", nullptr, &window.antiAim);
//        ui::MenuItem("Triggerbot", nullptr, &window.triggerbot);
//        ui::MenuItem("Backtrack", nullptr, &window.backtrack);
//        ui::MenuItem("Glow", nullptr, &window.glow);
//        ui::MenuItem("Chams", nullptr, &window.chams);
//        ui::MenuItem("Esp", nullptr, &window.esp);
//        ui::MenuItem("Visuals", nullptr, &window.visuals);
//        ui::MenuItem("Skin changer", nullptr, &window.skinChanger);
//        ui::MenuItem("Sound", nullptr, &window.sound);
//        ui::MenuItem("Style", nullptr, &window.style);
//        ui::MenuItem("Misc", nullptr, &window.misc);
//        ui::MenuItem("Reportbot", nullptr, &window.reportbot);
//        ui::MenuItem("Config", nullptr, &window.config);
//        ui::EndMainMenuBar();
//    }
//}
//
//void GUI::renderAimbotWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.aimbot)
//            return;
//        ui::SetNextWindowSize({ 600.0f, 0.0f });
//        ui::Begin("Aimbot", &window.aimbot, windowFlags);
//    }
//    static int currentCategory{ 0 };
//    ui::PushItemWidth(110.0f);
//    ui::PushID(0);
//    ui::Combo("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles\0");
//    ui::PopID();
//    ui::SameLine();
//    static int currentWeapon{ 0 };
//    ui::PushID(1);
//
//    switch (currentCategory) {
//    case 0:
//        currentWeapon = 0;
//        ui::NewLine();
//        break;
//    case 1: {
//        static int currentPistol{ 0 };
//        static constexpr const char* pistols[]{ "All", "Glock-18", "P2000", "USP-S", "Dual Berettas", "P250", "Tec-9", "Five-Seven", "CZ-75", "Desert Eagle", "Revolver" };
//
//        ui::Combo("", &currentPistol, [](void* data, int idx, const char** out_text) {
//            if (config->aimbot[idx ? idx : 35].enabled) {
//                static std::string name;
//                name = pistols[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = pistols[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(pistols));
//
//        currentWeapon = currentPistol ? currentPistol : 35;
//        break;
//    }
//    case 2: {
//        static int currentHeavy{ 0 };
//        static constexpr const char* heavies[]{ "All", "Nova", "XM1014", "Sawed-off", "MAG-7", "M249", "Negev" };
//
//        ui::Combo("", &currentHeavy, [](void* data, int idx, const char** out_text) {
//            if (config->aimbot[idx ? idx + 10 : 36].enabled) {
//                static std::string name;
//                name = heavies[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = heavies[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(heavies));
//
//        currentWeapon = currentHeavy ? currentHeavy + 10 : 36;
//        break;
//    }
//    case 3: {
//        static int currentSmg{ 0 };
//        static constexpr const char* smgs[]{ "All", "Mac-10", "MP9", "MP7", "MP5-SD", "UMP-45", "P90", "PP-Bizon" };
//
//        ui::Combo("", &currentSmg, [](void* data, int idx, const char** out_text) {
//            if (config->aimbot[idx ? idx + 16 : 37].enabled) {
//                static std::string name;
//                name = smgs[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = smgs[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(smgs));
//
//        currentWeapon = currentSmg ? currentSmg + 16 : 37;
//        break;
//    }
//    case 4: {
//        static int currentRifle{ 0 };
//        static constexpr const char* rifles[]{ "All", "Galil AR", "Famas", "AK-47", "M4A4", "M4A1-S", "SSG-08", "SG-553", "AUG", "AWP", "G3SG1", "SCAR-20" };
//
//        ui::Combo("", &currentRifle, [](void* data, int idx, const char** out_text) {
//            if (config->aimbot[idx ? idx + 23 : 38].enabled) {
//                static std::string name;
//                name = rifles[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = rifles[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(rifles));
//
//        currentWeapon = currentRifle ? currentRifle + 23 : 38;
//        break;
//    }
//    }
//    ui::PopID();
//    ui::SameLine();
//    ui::Checkbox("Enabled", &config->aimbot[currentWeapon].enabled);
//    ui::Separator();
//    ui::Columns(2, nullptr, false);
//    ui::SetColumnOffset(1, 220.0f);
//    ui::Checkbox("On key", &config->aimbot[currentWeapon].onKey);
//    ui::SameLine();
//    hotkey(config->aimbot[currentWeapon].key);
//    ui::SameLine();
//    ui::PushID(2);
//    ui::PushItemWidth(70.0f);
//    ui::Combo("", &config->aimbot[currentWeapon].keyMode, "Hold\0Toggle\0");
//    ui::PopItemWidth();
//    ui::PopID();
//    ui::Checkbox("Aimlock", &config->aimbot[currentWeapon].aimlock);
//    ui::Checkbox("Silent", &config->aimbot[currentWeapon].silent);
//    ui::Checkbox("Friendly fire", &config->aimbot[currentWeapon].friendlyFire);
//    ui::Checkbox("Visible only", &config->aimbot[currentWeapon].visibleOnly);
//    ui::Checkbox("Scoped only", &config->aimbot[currentWeapon].scopedOnly);
//    ui::Checkbox("Ignore flash", &config->aimbot[currentWeapon].ignoreFlash);
//    ui::Checkbox("Ignore smoke", &config->aimbot[currentWeapon].ignoreSmoke);
//    ui::Checkbox("Auto shot", &config->aimbot[currentWeapon].autoShot);
//    ui::Checkbox("Auto scope", &config->aimbot[currentWeapon].autoScope);
//    ui::Combo("Bone", &config->aimbot[currentWeapon].bone, "Nearest\0Best damage\0Head\0Neck\0Sternum\0Chest\0Stomach\0Pelvis\0");
//    ui::NextColumn();
//    ui::PushItemWidth(240.0f);
//    ui::SliderFloat("Fov", &config->aimbot[currentWeapon].fov, 0.0f, 255.0f, "%.2f", 2.5f);
//    ui::SliderFloat("Smooth", &config->aimbot[currentWeapon].smooth, 1.0f, 100.0f, "%.2f");
//    ui::SliderFloat("Max aim inaccuracy", &config->aimbot[currentWeapon].maxAimInaccuracy, 0.0f, 1.0f, "%.5f", 2.0f);
//    ui::SliderFloat("Max shot inaccuracy", &config->aimbot[currentWeapon].maxShotInaccuracy, 0.0f, 1.0f, "%.5f", 2.0f);
//    ui::InputInt("Min damage", &config->aimbot[currentWeapon].minDamage);
//    config->aimbot[currentWeapon].minDamage = std::clamp(config->aimbot[currentWeapon].minDamage, 0, 250);
//    ui::Checkbox("Killshot", &config->aimbot[currentWeapon].killshot);
//    ui::Checkbox("Between shots", &config->aimbot[currentWeapon].betweenShots);
//    ui::Columns(1);
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderAntiAimWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.antiAim)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Anti aim", &window.antiAim, windowFlags);
//    }
//    ui::Checkbox("Enabled", &config->antiAim.enabled);
//    ui::Checkbox("##pitch", &config->antiAim.pitch);
//    ui::SameLine();
//    ui::SliderFloat("Pitch", &config->antiAim.pitchAngle, -89.0f, 89.0f, "%.2f");
//    ui::Checkbox("Yaw", &config->antiAim.yaw);
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderTriggerbotWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.triggerbot)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Triggerbot", &window.triggerbot, windowFlags);
//    }
//    static int currentCategory{ 0 };
//    ui::PushItemWidth(110.0f);
//    ui::PushID(0);
//    ui::Combo("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles\0Zeus x27\0");
//    ui::PopID();
//    ui::SameLine();
//    static int currentWeapon{ 0 };
//    ui::PushID(1);
//    switch (currentCategory) {
//    case 0:
//        currentWeapon = 0;
//        ui::NewLine();
//        break;
//    case 5:
//        currentWeapon = 39;
//        ui::NewLine();
//        break;
//
//    case 1: {
//        static int currentPistol{ 0 };
//        static constexpr const char* pistols[]{ "All", "Glock-18", "P2000", "USP-S", "Dual Berettas", "P250", "Tec-9", "Five-Seven", "CZ-75", "Desert Eagle", "Revolver" };
//
//        ui::Combo("", &currentPistol, [](void* data, int idx, const char** out_text) {
//            if (config->triggerbot[idx ? idx : 35].enabled) {
//                static std::string name;
//                name = pistols[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = pistols[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(pistols));
//
//        currentWeapon = currentPistol ? currentPistol : 35;
//        break;
//    }
//    case 2: {
//        static int currentHeavy{ 0 };
//        static constexpr const char* heavies[]{ "All", "Nova", "XM1014", "Sawed-off", "MAG-7", "M249", "Negev" };
//
//        ui::Combo("", &currentHeavy, [](void* data, int idx, const char** out_text) {
//            if (config->triggerbot[idx ? idx + 10 : 36].enabled) {
//                static std::string name;
//                name = heavies[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = heavies[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(heavies));
//
//        currentWeapon = currentHeavy ? currentHeavy + 10 : 36;
//        break;
//    }
//    case 3: {
//        static int currentSmg{ 0 };
//        static constexpr const char* smgs[]{ "All", "Mac-10", "MP9", "MP7", "MP5-SD", "UMP-45", "P90", "PP-Bizon" };
//
//        ui::Combo("", &currentSmg, [](void* data, int idx, const char** out_text) {
//            if (config->triggerbot[idx ? idx + 16 : 37].enabled) {
//                static std::string name;
//                name = smgs[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = smgs[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(smgs));
//
//        currentWeapon = currentSmg ? currentSmg + 16 : 37;
//        break;
//    }
//    case 4: {
//        static int currentRifle{ 0 };
//        static constexpr const char* rifles[]{ "All", "Galil AR", "Famas", "AK-47", "M4A4", "M4A1-S", "SSG-08", "SG-553", "AUG", "AWP", "G3SG1", "SCAR-20" };
//
//        ui::Combo("", &currentRifle, [](void* data, int idx, const char** out_text) {
//            if (config->triggerbot[idx ? idx + 23 : 38].enabled) {
//                static std::string name;
//                name = rifles[idx];
//                *out_text = name.append(" *").c_str();
//            } else {
//                *out_text = rifles[idx];
//            }
//            return true;
//            }, nullptr, IM_ARRAYSIZE(rifles));
//
//        currentWeapon = currentRifle ? currentRifle + 23 : 38;
//        break;
//    }
//    }
//    ui::PopID();
//    ui::SameLine();
//    ui::Checkbox("Enabled", &config->triggerbot[currentWeapon].enabled);
//    ui::Separator();
//    ui::Checkbox("On key", &config->triggerbot[currentWeapon].onKey);
//    ui::SameLine();
//    hotkey(config->triggerbot[currentWeapon].key);
//    ui::Checkbox("Friendly fire", &config->triggerbot[currentWeapon].friendlyFire);
//    ui::Checkbox("Scoped only", &config->triggerbot[currentWeapon].scopedOnly);
//    ui::Checkbox("Ignore flash", &config->triggerbot[currentWeapon].ignoreFlash);
//    ui::Checkbox("Ignore smoke", &config->triggerbot[currentWeapon].ignoreSmoke);
//    ui::SetNextItemWidth(85.0f);
//    ui::Combo("Hitgroup", &config->triggerbot[currentWeapon].hitgroup, "All\0Head\0Chest\0Stomach\0Left arm\0Right arm\0Left leg\0Right leg\0");
//    ui::PushItemWidth(220.0f);
//    ui::SliderInt("Shot delay", &config->triggerbot[currentWeapon].shotDelay, 0, 250, "%d ms");
//    ui::InputInt("Min damage", &config->triggerbot[currentWeapon].minDamage);
//    config->triggerbot[currentWeapon].minDamage = std::clamp(config->triggerbot[currentWeapon].minDamage, 0, 250);
//    ui::Checkbox("Killshot", &config->triggerbot[currentWeapon].killshot);
//    ui::SliderFloat("Burst Time", &config->triggerbot[currentWeapon].burstTime, 0.0f, 0.5f, "%.3f s");
//
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderBacktrackWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.backtrack)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Backtrack", &window.backtrack, windowFlags);
//    }
//    ui::Checkbox("Enabled", &config->backtrack.enabled);
//    ui::Checkbox("Ignore smoke", &config->backtrack.ignoreSmoke);
//    ui::Checkbox("Recoil based fov", &config->backtrack.recoilBasedFov);
//    ui::PushItemWidth(220.0f);
//    ui::SliderInt("Time limit", &config->backtrack.timeLimit, 1, 200, "%d ms");
//    ui::PopItemWidth();
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderGlowWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.glow)
//            return;
//        ui::SetNextWindowSize({ 450.0f, 0.0f });
//        ui::Begin("Glow", &window.glow, windowFlags);
//    }
//    static int currentCategory{ 0 };
//    ui::PushItemWidth(110.0f);
//    ui::PushID(0);
//    ui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0C4\0Planted C4\0Chickens\0Defuse kits\0Projectiles\0Hostages\0Ragdolls\0");
//    ui::PopID();
//    static int currentItem{ 0 };
//    if (currentCategory <= 3) {
//        ui::SameLine();
//        static int currentType{ 0 };
//        ui::PushID(1);
//        ui::Combo("", &currentType, "All\0Visible\0Occluded\0");
//        ui::PopID();
//        currentItem = currentCategory * 3 + currentType;
//    } else {
//        currentItem = currentCategory + 8;
//    }
//
//    ui::SameLine();
//    ui::Checkbox("Enabled", &config->glow[currentItem].enabled);
//    ui::Separator();
//    ui::Columns(2, nullptr, false);
//    ui::SetColumnOffset(1, 150.0f);
//    ui::Checkbox("Health based", &config->glow[currentItem].healthBased);
//
//    ImGuiCustom::colorPicker("Color", config->glow[currentItem].color.color, nullptr, &config->glow[currentItem].color.rainbow, &config->glow[currentItem].color.rainbowSpeed);
//
//    ui::NextColumn();
//    ui::PushItemWidth(220.0f);
//    ui::SliderFloat("Thickness", &config->glow[currentItem].thickness, 0.0f, 1.0f, "%.2f");
//    ui::SliderFloat("Alpha", &config->glow[currentItem].alpha, 0.0f, 1.0f, "%.2f");
//    ui::SliderInt("Style", &config->glow[currentItem].style, 0, 3);
//    ui::Columns(1);
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderChamsWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.chams)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Chams", &window.chams, windowFlags);
//    }
//    static int currentCategory{ 0 };
//    ui::PushItemWidth(110.0f);
//    ui::PushID(0);
//    ui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0Hands\0Backtrack\0Sleeves\0");
//    ui::PopID();
//    static int currentItem{ 0 };
//
//    if (currentCategory <= 3) {
//        ui::SameLine();
//        static int currentType{ 0 };
//        ui::PushID(1);
//        ui::Combo("", &currentType, "All\0Visible\0Occluded\0");
//        ui::PopID();
//        currentItem = currentCategory * 3 + currentType;
//    } else {
//        currentItem = currentCategory + 8;
//    }
//
//    ui::SameLine();
//    static int material = 1;
//
//    if (ui::ArrowButton("##left", ImGuiDir_Left) && material > 1)
//        --material;
//    ui::SameLine();
//    ui::Text("%d", material);
//    ui::SameLine();
//    if (ui::ArrowButton("##right", ImGuiDir_Right) && material < int(config->chams[0].materials.size()))
//        ++material;
//
//    ui::SameLine();
//    auto& chams{ config->chams[currentItem].materials[material - 1] };
//
//    ui::Checkbox("Enabled", &chams.enabled);
//    ui::Separator();
//    ui::Checkbox("Health based", &chams.healthBased);
//    ui::Checkbox("Blinking", &chams.blinking);
//    ui::Combo("Material", &chams.material, "Normal\0Flat\0Animated\0Platinum\0Glass\0Chrome\0Crystal\0Silver\0Gold\0Plastic\0Glow\0");
//    ui::Checkbox("Wireframe", &chams.wireframe);
//    ImGuiCustom::colorPicker("Color", chams.color.color, nullptr, &chams.color.rainbow, &chams.color.rainbowSpeed);
//    ui::SetNextItemWidth(220.0f);
//    ui::SliderFloat("Alpha", &chams.alpha, 0.0f, 1.0f, "%.2f");
//
//    if (!contentOnly) {
//        ui::End();
//    }
//}
//
//void GUI::renderEspWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.esp)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Esp", &window.esp, windowFlags);
//    }
//
//    static int currentCategory = 0;
//    static int currentItem = 0;
//
//    if (ui::ListBoxHeader("##", { 125.0f, 300.0f })) {
//        static constexpr const char* players[]{ "All", "Visible", "Occluded" };
//
//        ui::Text("Allies");
//        ui::Indent();
//        ui::PushID("Allies");
//        ui::PushFont(fonts.segoeui);
//
//        for (int i = 0; i < IM_ARRAYSIZE(players); i++) {
//            bool isSelected = currentCategory == 0 && currentItem == i;
//
//            if ((i == 0 || !config->esp.players[0].enabled) && ui::Selectable(players[i], isSelected)) {
//                currentItem = i;
//                currentCategory = 0;
//            }
//        }
//
//        ui::PopFont();
//        ui::PopID();
//        ui::Unindent();
//        ui::Text("Enemies");
//        ui::Indent();
//        ui::PushID("Enemies");
//        ui::PushFont(fonts.segoeui);
//
//        for (int i = 0; i < IM_ARRAYSIZE(players); i++) {
//            bool isSelected = currentCategory == 1 && currentItem == i;
//
//            if ((i == 0 || !config->esp.players[3].enabled) && ui::Selectable(players[i], isSelected)) {
//                currentItem = i;
//                currentCategory = 1;
//            }
//        }
//
//        ui::PopFont();
//        ui::PopID();
//        ui::Unindent();
//        if (bool isSelected = currentCategory == 2; ui::Selectable("Weapons", isSelected))
//            currentCategory = 2;
//
//        ui::Text("Projectiles");
//        ui::Indent();
//        ui::PushID("Projectiles");
//        ui::PushFont(fonts.segoeui);
//        static constexpr const char* projectiles[]{ "Flashbang", "HE Grenade", "Breach Charge", "Bump Mine", "Decoy Grenade", "Molotov", "TA Grenade", "Smoke Grenade", "Snowball" };
//
//        for (int i = 0; i < IM_ARRAYSIZE(projectiles); i++) {
//            bool isSelected = currentCategory == 3 && currentItem == i;
//
//            if (ui::Selectable(projectiles[i], isSelected)) {
//                currentItem = i;
//                currentCategory = 3;
//            }
//        }
//
//        ui::PopFont();
//        ui::PopID();
//        ui::Unindent();
//
//        ui::Text("Danger Zone");
//        ui::Indent();
//        ui::PushID("Danger Zone");
//        ui::PushFont(fonts.segoeui);
//        static constexpr const char* dangerZone[]{ "Sentries", "Drones", "Cash", "Cash Dufflebag", "Pistol Case", "Light Case", "Heavy Case", "Explosive Case", "Tools Case", "Full Armor", "Armor", "Helmet", "Parachute", "Briefcase", "Tablet Upgrade", "ExoJump", "Ammobox", "Radar Jammer" };
//
//        for (int i = 0; i < IM_ARRAYSIZE(dangerZone); i++) {
//            bool isSelected = currentCategory == 4 && currentItem == i;
//
//            if (ui::Selectable(dangerZone[i], isSelected)) {
//                currentItem = i;
//                currentCategory = 4;
//            }
//        }
//
//        ui::PopFont();
//        ui::PopID();
//        ui::ListBoxFooter();
//    }
//    ui::SameLine();
//    if (ui::BeginChild("##child", { 400.0f, 0.0f })) {
//        switch (currentCategory) {
//        case 0:
//        case 1: {
//            int selected = currentCategory * 3 + currentItem;
//            ui::Checkbox("Enabled", &config->esp.players[selected].enabled);
//            ui::SameLine(0.0f, 50.0f);
//            ui::SetNextItemWidth(85.0f);
//            ui::InputInt("Font", &config->esp.players[selected].font, 1, 294);
//            config->esp.players[selected].font = std::clamp(config->esp.players[selected].font, 1, 294);
//
//            ui::Separator();
//
//            constexpr auto spacing{ 200.0f };
//            ImGuiCustom::colorPicker("Snaplines", config->esp.players[selected].snaplines);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Box", config->esp.players[selected].box);
//            ui::SameLine();
//            ui::SetNextItemWidth(95.0f);
//            ui::Combo("", &config->esp.players[selected].boxType, "2D\0""2D corners\0""3D\0""3D corners\0");
//            ImGuiCustom::colorPicker("Eye traces", config->esp.players[selected].eyeTraces);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Health", config->esp.players[selected].health);
//            ImGuiCustom::colorPicker("Head dot", config->esp.players[selected].headDot);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Health bar", config->esp.players[selected].healthBar);
//            ImGuiCustom::colorPicker("Name", config->esp.players[selected].name);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Armor", config->esp.players[selected].armor);
//            ImGuiCustom::colorPicker("Money", config->esp.players[selected].money);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Armor bar", config->esp.players[selected].armorBar);
//            ImGuiCustom::colorPicker("Outline", config->esp.players[selected].outline);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Distance", config->esp.players[selected].distance);
//            ImGuiCustom::colorPicker("Active Weapon", config->esp.players[selected].activeWeapon);
//            ui::SameLine(spacing);
//            ui::Checkbox("Dead ESP", &config->esp.players[selected].deadesp);
//            ui::SliderFloat("Max distance", &config->esp.players[selected].maxDistance, 0.0f, 200.0f, "%.2fm");
//            break;
//        }
//        case 2: {
//            ui::Checkbox("Enabled", &config->esp.weapon.enabled);
//            ui::SameLine(0.0f, 50.0f);
//            ui::SetNextItemWidth(85.0f);
//            ui::InputInt("Font", &config->esp.weapon.font, 1, 294);
//            config->esp.weapon.font = std::clamp(config->esp.weapon.font, 1, 294);
//
//            ui::Separator();
//
//            constexpr auto spacing{ 200.0f };
//            ImGuiCustom::colorPicker("Snaplines", config->esp.weapon.snaplines);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Box", config->esp.weapon.box);
//            ui::SameLine();
//            ui::SetNextItemWidth(95.0f);
//            ui::Combo("", &config->esp.weapon.boxType, "2D\0""2D corners\0""3D\0""3D corners\0");
//            ImGuiCustom::colorPicker("Name", config->esp.weapon.name);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Outline", config->esp.weapon.outline);
//            ImGuiCustom::colorPicker("Distance", config->esp.weapon.distance);
//            ui::SliderFloat("Max distance", &config->esp.weapon.maxDistance, 0.0f, 200.0f, "%.2fm");
//            break;
//        }
//        case 3: {
//            ui::Checkbox("Enabled", &config->esp.projectiles[currentItem].enabled);
//            ui::SameLine(0.0f, 50.0f);
//            ui::SetNextItemWidth(85.0f);
//            ui::InputInt("Font", &config->esp.projectiles[currentItem].font, 1, 294);
//            config->esp.projectiles[currentItem].font = std::clamp(config->esp.projectiles[currentItem].font, 1, 294);
//
//            ui::Separator();
//
//            constexpr auto spacing{ 200.0f };
//            ImGuiCustom::colorPicker("Snaplines", config->esp.projectiles[currentItem].snaplines);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Box", config->esp.projectiles[currentItem].box);
//            ui::SameLine();
//            ui::SetNextItemWidth(95.0f);
//            ui::Combo("", &config->esp.projectiles[currentItem].boxType, "2D\0""2D corners\0""3D\0""3D corners\0");
//            ImGuiCustom::colorPicker("Name", config->esp.projectiles[currentItem].name);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Outline", config->esp.projectiles[currentItem].outline);
//            ImGuiCustom::colorPicker("Distance", config->esp.projectiles[currentItem].distance);
//            ui::SliderFloat("Max distance", &config->esp.projectiles[currentItem].maxDistance, 0.0f, 200.0f, "%.2fm");
//            break;
//        }
//        case 4: {
//            int selected = currentItem;
//            ui::Checkbox("Enabled", &config->esp.dangerZone[selected].enabled);
//            ui::SameLine(0.0f, 50.0f);
//            ui::SetNextItemWidth(85.0f);
//            ui::InputInt("Font", &config->esp.dangerZone[selected].font, 1, 294);
//            config->esp.dangerZone[selected].font = std::clamp(config->esp.dangerZone[selected].font, 1, 294);
//
//            ui::Separator();
//
//            constexpr auto spacing{ 200.0f };
//            ImGuiCustom::colorPicker("Snaplines", config->esp.dangerZone[selected].snaplines);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Box", config->esp.dangerZone[selected].box);
//            ui::SameLine();
//            ui::SetNextItemWidth(95.0f);
//            ui::Combo("", &config->esp.dangerZone[selected].boxType, "2D\0""2D corners\0""3D\0""3D corners\0");
//            ImGuiCustom::colorPicker("Name", config->esp.dangerZone[selected].name);
//            ui::SameLine(spacing);
//            ImGuiCustom::colorPicker("Outline", config->esp.dangerZone[selected].outline);
//            ImGuiCustom::colorPicker("Distance", config->esp.dangerZone[selected].distance);
//            ui::SliderFloat("Max distance", &config->esp.dangerZone[selected].maxDistance, 0.0f, 200.0f, "%.2fm");
//            break;
//        }
//        }
//
//        ui::EndChild();
//    }
//
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderVisualsWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.visuals)
//            return;
//        ui::SetNextWindowSize({ 680.0f, 0.0f });
//        ui::Begin("Visuals", &window.visuals, windowFlags);
//    }
//    ui::Columns(2, nullptr, false);
//    ui::SetColumnOffset(1, 280.0f);
//    ui::Combo("T Player Model", &config->visuals.playerModelT, "Default\0Special Agent Ava | FBI\0Operator | FBI SWAT\0Markus Delrow | FBI HRT\0Michael Syfers | FBI Sniper\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0Third Commando Company | KSK\0'Two Times' McCoy | USAF TACP\0Dragomir | Sabre\0Rezan The Ready | Sabre\0'The Doctor' Romanov | Sabre\0Maximus | Sabre\0Blackwolf | Sabre\0The Elite Mr. Muhlik | Elite Crew\0Ground Rebel | Elite Crew\0Osiris | Elite Crew\0Prof. Shahmat | Elite Crew\0Enforcer | Phoenix\0Slingshot | Phoenix\0Soldier | Phoenix\0");
//    ui::Combo("CT Player Model", &config->visuals.playerModelCT, "Default\0Special Agent Ava | FBI\0Operator | FBI SWAT\0Markus Delrow | FBI HRT\0Michael Syfers | FBI Sniper\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0Third Commando Company | KSK\0'Two Times' McCoy | USAF TACP\0Dragomir | Sabre\0Rezan The Ready | Sabre\0'The Doctor' Romanov | Sabre\0Maximus | Sabre\0Blackwolf | Sabre\0The Elite Mr. Muhlik | Elite Crew\0Ground Rebel | Elite Crew\0Osiris | Elite Crew\0Prof. Shahmat | Elite Crew\0Enforcer | Phoenix\0Slingshot | Phoenix\0Soldier | Phoenix\0");
//    ui::Checkbox("Disable post-processing", &config->visuals.disablePostProcessing);
//    ui::Checkbox("Inverse ragdoll gravity", &config->visuals.inverseRagdollGravity);
//    ui::Checkbox("No fog", &config->visuals.noFog);
//    ui::Checkbox("No 3d sky", &config->visuals.no3dSky);
//    ui::Checkbox("No aim punch", &config->visuals.noAimPunch);
//    ui::Checkbox("No view punch", &config->visuals.noViewPunch);
//    ui::Checkbox("No hands", &config->visuals.noHands);
//    ui::Checkbox("No sleeves", &config->visuals.noSleeves);
//    ui::Checkbox("No weapons", &config->visuals.noWeapons);
//    ui::Checkbox("No smoke", &config->visuals.noSmoke);
//    ui::Checkbox("No blur", &config->visuals.noBlur);
//    ui::Checkbox("No scope overlay", &config->visuals.noScopeOverlay);
//    ui::Checkbox("No grass", &config->visuals.noGrass);
//    ui::Checkbox("No shadows", &config->visuals.noShadows);
//    ui::Checkbox("Wireframe smoke", &config->visuals.wireframeSmoke);
//    ui::NextColumn();
//    ui::Checkbox("Zoom", &config->visuals.zoom);
//    ui::SameLine();
//    hotkey(config->visuals.zoomKey);
//    ui::Checkbox("Thirdperson", &config->visuals.thirdperson);
//    ui::SameLine();
//    hotkey(config->visuals.thirdpersonKey);
//    ui::PushItemWidth(290.0f);
//    ui::PushID(0);
//    ui::SliderInt("", &config->visuals.thirdpersonDistance, 0, 1000, "Thirdperson distance: %d");
//    ui::PopID();
//    ui::PushID(1);
//    ui::SliderInt("", &config->visuals.viewmodelFov, -60, 60, "Viewmodel FOV: %d");
//    ui::PopID();
//    ui::PushID(2);
//    ui::SliderInt("", &config->visuals.fov, -60, 60, "FOV: %d");
//    ui::PopID();
//    ui::PushID(3);
//    ui::SliderInt("", &config->visuals.farZ, 0, 2000, "Far Z: %d");
//    ui::PopID();
//    ui::PushID(4);
//    ui::SliderInt("", &config->visuals.flashReduction, 0, 100, "Flash reduction: %d%%");
//    ui::PopID();
//    ui::PushID(5);
//    ui::SliderFloat("", &config->visuals.brightness, 0.0f, 1.0f, "Brightness: %.2f");
//    ui::PopID();
//    ui::PopItemWidth();
//    ui::Combo("Skybox", &config->visuals.skybox, "Default\0cs_baggage_skybox_\0cs_tibet\0embassy\0italy\0jungle\0nukeblank\0office\0sky_cs15_daylight01_hdr\0sky_cs15_daylight02_hdr\0sky_cs15_daylight03_hdr\0sky_cs15_daylight04_hdr\0sky_csgo_cloudy01\0sky_csgo_night_flat\0sky_csgo_night02\0sky_day02_05_hdr\0sky_day02_05\0sky_dust\0sky_l4d_rural02_ldr\0sky_venice\0vertigo_hdr\0vertigo\0vertigoblue_hdr\0vietnam\0");
//    ImGuiCustom::colorPicker("World color", config->visuals.world);
//    ImGuiCustom::colorPicker("Sky color", config->visuals.sky);
//    ui::Checkbox("Deagle spinner", &config->visuals.deagleSpinner);
//    ui::Combo("Screen effect", &config->visuals.screenEffect, "None\0Drone cam\0Drone cam with noise\0Underwater\0Healthboost\0Dangerzone\0");
//    ui::Combo("Hit effect", &config->visuals.hitEffect, "None\0Drone cam\0Drone cam with noise\0Underwater\0Healthboost\0Dangerzone\0");
//    ui::SliderFloat("Hit effect time", &config->visuals.hitEffectTime, 0.1f, 1.5f, "%.2fs");
//    ui::Combo("Hit marker", &config->visuals.hitMarker, "None\0Default (Cross)\0");
//    ui::SliderFloat("Hit marker time", &config->visuals.hitMarkerTime, 0.1f, 1.5f, "%.2fs");
//    ui::Checkbox("Color correction", &config->visuals.colorCorrection.enabled);
//    ui::SameLine();
//    bool ccPopup = ui::Button("Edit");
//
//    if (ccPopup)
//        ui::OpenPopup("##popup");
//
//    if (ui::BeginPopup("##popup")) {
//        ui::VSliderFloat("##1", { 40.0f, 160.0f }, &config->visuals.colorCorrection.blue, 0.0f, 1.0f, "Blue\n%.3f"); ui::SameLine();
//        ui::VSliderFloat("##2", { 40.0f, 160.0f }, &config->visuals.colorCorrection.red, 0.0f, 1.0f, "Red\n%.3f"); ui::SameLine();
//        ui::VSliderFloat("##3", { 40.0f, 160.0f }, &config->visuals.colorCorrection.mono, 0.0f, 1.0f, "Mono\n%.3f"); ui::SameLine();
//        ui::VSliderFloat("##4", { 40.0f, 160.0f }, &config->visuals.colorCorrection.saturation, 0.0f, 1.0f, "Sat\n%.3f"); ui::SameLine();
//        ui::VSliderFloat("##5", { 40.0f, 160.0f }, &config->visuals.colorCorrection.ghost, 0.0f, 1.0f, "Ghost\n%.3f"); ui::SameLine();
//        ui::VSliderFloat("##6", { 40.0f, 160.0f }, &config->visuals.colorCorrection.green, 0.0f, 1.0f, "Green\n%.3f"); ui::SameLine();
//        ui::VSliderFloat("##7", { 40.0f, 160.0f }, &config->visuals.colorCorrection.yellow, 0.0f, 1.0f, "Yellow\n%.3f"); ui::SameLine();
//        ui::EndPopup();
//    }
//    ui::Columns(1);
//
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderSkinChangerWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.skinChanger)
//            return;
//        ui::SetNextWindowSize({ 700.0f, 0.0f });
//        ui::Begin("nSkinz", &window.skinChanger, windowFlags);
//    }
//
//    static auto itemIndex = 0;
//
//    ui::PushItemWidth(110.0f);
//    ui::Combo("##1", &itemIndex, [](void* data, int idx, const char** out_text) {
//        *out_text = game_data::weapon_names[idx].name;
//        return true;
//        }, nullptr, IM_ARRAYSIZE(game_data::weapon_names), 5);
//    ui::PopItemWidth();
//
//    auto& selected_entry = config->skinChanger[itemIndex];
//    selected_entry.itemIdIndex = itemIndex;
//
//    {
//        ui::SameLine();
//        ui::Checkbox("Enabled", &selected_entry.enabled);
//        ui::Separator();
//        ui::Columns(2, nullptr, false);
//        ui::InputInt("Seed", &selected_entry.seed);
//        ui::InputInt("StatTrak", &selected_entry.stat_trak);
//        ui::SliderFloat("Wear", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);
//
//        ui::Combo("Paint Kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
//            *out_text = (itemIndex == 1 ? SkinChanger::gloveKits : SkinChanger::skinKits)[idx].name.c_str();
//            return true;
//            }, nullptr, (itemIndex == 1 ? SkinChanger::gloveKits : SkinChanger::skinKits).size(), 10);
//
//        ui::Combo("Quality", &selected_entry.entity_quality_vector_index, [](void* data, int idx, const char** out_text) {
//            *out_text = game_data::quality_names[idx].name;
//            return true;
//            }, nullptr, IM_ARRAYSIZE(game_data::quality_names), 5);
//
//        if (itemIndex == 0) {
//            ui::Combo("Knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
//                *out_text = game_data::knife_names[idx].name;
//                return true;
//                }, nullptr, IM_ARRAYSIZE(game_data::knife_names), 5);
//        } else if (itemIndex == 1) {
//            ui::Combo("Glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
//                *out_text = game_data::glove_names[idx].name;
//                return true;
//                }, nullptr, IM_ARRAYSIZE(game_data::glove_names), 5);
//        } else {
//            static auto unused_value = 0;
//            selected_entry.definition_override_vector_index = 0;
//            ui::Combo("Unavailable", &unused_value, "For knives or gloves\0");
//        }
//
//        ui::InputText("Name Tag", selected_entry.custom_name, 32);
//    }
//
//    ui::NextColumn();
//
//    {
//        ui::PushID("sticker");
//
//        static auto selectedStickerSlot = 0;
//
//        ui::PushItemWidth(-1);
//
//        ui::ListBox("", &selectedStickerSlot, [](void* data, int idx, const char** out_text) {
//            static char elementName[64];
//            auto kit_vector_index = config->skinChanger[itemIndex].stickers[idx].kit_vector_index;
//            sprintf_s(elementName, "#%d (%s)", idx + 1, SkinChanger::stickerKits[kit_vector_index].name.c_str());
//            *out_text = elementName;
//            return true;
//            }, nullptr, 5, 5);
//
//        ui::PopItemWidth();
//
//        auto& selected_sticker = selected_entry.stickers[selectedStickerSlot];
//
//        ui::Combo("Sticker Kit", &selected_sticker.kit_vector_index, [](void* data, int idx, const char** out_text) {
//            *out_text = SkinChanger::stickerKits[idx].name.c_str();
//            return true;
//            }, nullptr, SkinChanger::stickerKits.size(), 10);
//
//        ui::SliderFloat("Wear", &selected_sticker.wear, FLT_MIN, 1.0f, "%.10f", 5.0f);
//        ui::SliderFloat("Scale", &selected_sticker.scale, 0.1f, 5.0f);
//        ui::SliderFloat("Rotation", &selected_sticker.rotation, 0.0f, 360.0f);
//
//        ui::PopID();
//    }
//    selected_entry.update();
//
//    ui::Columns(1);
//
//    ui::Separator();
//
//    if (ui::Button("Update", { 130.0f, 30.0f }))
//        SkinChanger::scheduleHudUpdate();
//
//    ui::TextUnformatted("nSkinz by namazso");
//
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderSoundWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.sound)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Sound", &window.sound, windowFlags);
//    }
//    ui::SliderInt("Chicken volume", &config->sound.chickenVolume, 0, 200, "%d%%");
//
//    static int currentCategory{ 0 };
//    ui::PushItemWidth(110.0f);
//    ui::Combo("", &currentCategory, "Local player\0Allies\0Enemies\0");
//    ui::PopItemWidth();
//    ui::SliderInt("Master volume", &config->sound.players[currentCategory].masterVolume, 0, 200, "%d%%");
//    ui::SliderInt("Headshot volume", &config->sound.players[currentCategory].headshotVolume, 0, 200, "%d%%");
//    ui::SliderInt("Weapon volume", &config->sound.players[currentCategory].weaponVolume, 0, 200, "%d%%");
//    ui::SliderInt("Footstep volume", &config->sound.players[currentCategory].footstepVolume, 0, 200, "%d%%");
//
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderStyleWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.style)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Style", &window.style, windowFlags);
//    }
//
//    ui::PushItemWidth(150.0f);
//    if (ui::Combo("Menu style", &config->style.menuStyle, "Classic\0One window\0"))
//        window = { };
//    if (ui::Combo("Menu colors", &config->style.menuColors, "Dark\0Light\0Classic\0Custom\0"))
//        updateColors();
//    ui::PopItemWidth();
//
//    if (config->style.menuColors == 3) {
//        ImGuiStyle& style = ui::GetStyle();
//        for (int i = 0; i < ImGuiCol_COUNT; i++) {
//            if (i && i & 3) ui::SameLine(220.0f * (i & 3));
//
//            ImGuiCustom::colorPicker(ui::GetStyleColorName(i), (float*)&style.Colors[i]);
//        }
//    }
//
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderMiscWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.misc)
//            return;
//        ui::SetNextWindowSize({ 580.0f, 0.0f });
//        ui::Begin("Misc", &window.misc, windowFlags);
//    }
//    ui::Columns(2, nullptr, false);
//    ui::SetColumnOffset(1, 230.0f);
//    ui::TextUnformatted("Menu key");
//    ui::SameLine();
//    hotkey(config->misc.menuKey);
//
//    ui::Checkbox("Anti AFK kick", &config->misc.antiAfkKick);
//    ui::Checkbox("Auto strafe", &config->misc.autoStrafe);
//    ui::Checkbox("Bunny hop", &config->misc.bunnyHop);
//    ui::Checkbox("Fast duck", &config->misc.fastDuck);
//    ui::Checkbox("Moonwalk", &config->misc.moonwalk);
//    ui::Checkbox("Edge Jump", &config->misc.edgejump);
//    ui::SameLine();
//    hotkey(config->misc.edgejumpkey);
//    ui::Checkbox("Slowwalk", &config->misc.slowwalk);
//    ui::SameLine();
//    hotkey(config->misc.slowwalkKey);
//    ui::Checkbox("Sniper crosshair", &config->misc.sniperCrosshair);
//    ui::Checkbox("Recoil crosshair", &config->misc.recoilCrosshair);
//    ui::Checkbox("Auto pistol", &config->misc.autoPistol);
//    ui::Checkbox("Auto reload", &config->misc.autoReload);
//    ui::Checkbox("Auto accept", &config->misc.autoAccept);
//    ui::Checkbox("Radar hack", &config->misc.radarHack);
//    ui::Checkbox("Reveal ranks", &config->misc.revealRanks);
//    ui::Checkbox("Reveal money", &config->misc.revealMoney);
//    ui::Checkbox("Reveal suspect", &config->misc.revealSuspect);
//    ImGuiCustom::colorPicker("Spectator list", config->misc.spectatorList);
//    ImGuiCustom::colorPicker("Watermark", config->misc.watermark);
//    ui::Checkbox("Fix animation LOD", &config->misc.fixAnimationLOD);
//    ui::Checkbox("Fix bone matrix", &config->misc.fixBoneMatrix);
//    ui::Checkbox("Fix movement", &config->misc.fixMovement);
//    ui::Checkbox("Disable model occlusion", &config->misc.disableModelOcclusion);
//    ui::SliderFloat("Aspect Ratio", &config->misc.aspectratio, 0.0f, 5.0f, "%.2f");
//    ui::NextColumn();
//    ui::Checkbox("Disable HUD blur", &config->misc.disablePanoramablur);
//    ui::Checkbox("Animated clan tag", &config->misc.animatedClanTag);
//    ui::Checkbox("Clock tag", &config->misc.clocktag);
//    ui::Checkbox("Custom clantag", &config->misc.customClanTag);
//    ui::SameLine();
//    ui::PushItemWidth(120.0f);
//    ui::PushID(0);
//    if (ui::InputText("", &config->misc.clanTag))
//        Misc::updateClanTag(true);
//    ui::PopID();
//    ui::Checkbox("Kill message", &config->misc.killMessage);
//    ui::SameLine();
//    ui::PushItemWidth(120.0f);
//    ui::PushID(1);
//    ui::InputText("", &config->misc.killMessageString);
//    ui::PopID();
//    ui::Checkbox("Name stealer", &config->misc.nameStealer);
//    ui::PushID(3);
//    ui::SetNextItemWidth(100.0f);
//    ui::Combo("", &config->misc.banColor, "White\0Red\0Purple\0Green\0Light green\0Turquoise\0Light red\0Gray\0Yellow\0Gray 2\0Light blue\0Gray/Purple\0Blue\0Pink\0Dark orange\0Orange\0");
//    ui::PopID();
//    ui::SameLine();
//    ui::PushID(4);
//    ui::InputText("", &config->misc.banText);
//    ui::PopID();
//    ui::SameLine();
//    if (ui::Button("Setup fake ban"))
//        Misc::fakeBan(true);
//    ui::Checkbox("Fast plant", &config->misc.fastPlant);
//    ImGuiCustom::colorPicker("Bomb timer", config->misc.bombTimer);
//    ui::Checkbox("Quick reload", &config->misc.quickReload);
//    ui::Checkbox("Prepare revolver", &config->misc.prepareRevolver);
//    ui::SameLine();
//    hotkey(config->misc.prepareRevolverKey);
//    ui::Combo("Hit Sound", &config->misc.hitSound, "None\0Metal\0Gamesense\0Bell\0Glass\0");
//    ui::SetNextItemWidth(90.0f);
//    ui::InputInt("Choked packets", &config->misc.chokedPackets, 1, 5);
//    config->misc.chokedPackets = std::clamp(config->misc.chokedPackets, 0, 64);
//    ui::SameLine();
//    hotkey(config->misc.chokedPacketsKey);
//    ui::Text("Quick healthshot");
//    ui::SameLine();
//    hotkey(config->misc.quickHealthshotKey);
//    ui::Checkbox("Grenade Prediction", &config->misc.nadePredict);
//    ui::Checkbox("Fix tablet signal", &config->misc.fixTabletSignal);
//    ui::SetNextItemWidth(120.0f);
//    ui::SliderFloat("Max angle delta", &config->misc.maxAngleDelta, 0.0f, 255.0f, "%.2f");
//    ui::Checkbox("Fake prime", &config->misc.fakePrime);
//
//    if (ui::Button("Unhook"))
//        hooks->restore();
//
//    ui::Columns(1);
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderReportbotWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.reportbot)
//            return;
//        ui::SetNextWindowSize({ 0.0f, 0.0f });
//        ui::Begin("Reportbot", &window.reportbot, windowFlags);
//    }
//    ui::Checkbox("Enabled", &config->reportbot.enabled);
//    ui::SameLine(0.0f, 50.0f);
//    if (ui::Button("Reset"))
//        Reportbot::reset();
//    ui::Separator();
//    ui::PushItemWidth(80.0f);
//    ui::Combo("Target", &config->reportbot.target, "Enemies\0Allies\0All\0");
//    ui::InputInt("Delay (s)", &config->reportbot.delay);
//    config->reportbot.delay = (std::max)(config->reportbot.delay, 1);
//    ui::InputInt("Rounds", &config->reportbot.rounds);
//    config->reportbot.rounds = (std::max)(config->reportbot.rounds, 1);
//    ui::PopItemWidth();
//    ui::Checkbox("Abusive Communications", &config->reportbot.textAbuse);
//    ui::Checkbox("Griefing", &config->reportbot.griefing);
//    ui::Checkbox("Wall Hacking", &config->reportbot.wallhack);
//    ui::Checkbox("Aim Hacking", &config->reportbot.aimbot);
//    ui::Checkbox("Other Hacking", &config->reportbot.other);
//
//    if (!contentOnly)
//        ui::End();
//}
//
//void GUI::renderConfigWindow(bool contentOnly) noexcept
//{
//    if (!contentOnly) {
//        if (!window.config)
//            return;
//        ui::SetNextWindowSize({ 290.0f, 190.0f });
//        ui::Begin("Config", &window.config, windowFlags);
//    }
//
//    ui::Columns(2, nullptr, false);
//    ui::SetColumnOffset(1, 170.0f);
//
//    ui::PushItemWidth(160.0f);
//
//    auto& configItems = config->getConfigs();
//    static int currentConfig = -1;
//
//    if (static_cast<size_t>(currentConfig) >= configItems.size())
//        currentConfig = -1;
//
//    static std::string buffer;
//
//    if (ui::ListBox("", &currentConfig, [](void* data, int idx, const char** out_text) {
//        auto& vector = *static_cast<std::vector<std::string>*>(data);
//        *out_text = vector[idx].c_str();
//        return true;
//        }, &configItems, configItems.size(), 5) && currentConfig != -1)
//            buffer = configItems[currentConfig];
//
//        ui::PushID(0);
//        if (ui::InputText("", &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
//            if (currentConfig != -1)
//                config->rename(currentConfig, buffer.c_str());
//        }
//        ui::PopID();
//        ui::NextColumn();
//
//        ui::PushItemWidth(100.0f);
//
//        if (ui::Button("Create config", { 100.0f, 25.0f }))
//            config->add(buffer.c_str());
//
//        if (ui::Button("Reset config", { 100.0f, 25.0f }))
//            ui::OpenPopup("Config to reset");
//
//        if (ui::BeginPopup("Config to reset")) {
//            static constexpr const char* names[]{ "Whole", "Aimbot", "Triggerbot", "Backtrack", "Anti aim", "Glow", "Chams", "Esp", "Visuals", "Skin changer", "Sound", "Style", "Misc", "Reportbot" };
//            for (int i = 0; i < IM_ARRAYSIZE(names); i++) {
//                if (i == 1) ui::Separator();
//
//                if (ui::Selectable(names[i])) {
//                    switch (i) {
//                    case 0: config->reset(); updateColors(); Misc::updateClanTag(true); SkinChanger::scheduleHudUpdate(); break;
//                    case 1: config->aimbot = { }; break;
//                    case 2: config->triggerbot = { }; break;
//                    case 3: config->backtrack = { }; break;
//                    case 4: config->antiAim = { }; break;
//                    case 5: config->glow = { }; break;
//                    case 6: config->chams = { }; break;
//                    case 7: config->esp = { }; break;
//                    case 8: config->visuals = { }; break;
//                    case 9: config->skinChanger = { }; SkinChanger::scheduleHudUpdate(); break;
//                    case 10: config->sound = { }; break;
//                    case 11: config->style = { }; updateColors(); break;
//                    case 12: config->misc = { };  Misc::updateClanTag(true); break;
//                    case 13: config->reportbot = { }; break;
//                    }
//                }
//            }
//            ui::EndPopup();
//        }
//        if (currentConfig != -1) {
//            if (ui::Button("Load selected", { 100.0f, 25.0f })) {
//                config->load(currentConfig);
//                updateColors();
//                SkinChanger::scheduleHudUpdate();
//                Misc::updateClanTag(true);
//            }
//            if (ui::Button("Save selected", { 100.0f, 25.0f }))
//                config->save(currentConfig);
//            if (ui::Button("Delete selected", { 100.0f, 25.0f }))
//                config->remove(currentConfig);
//        }
//        ui::Columns(1);
//        if (!contentOnly)
//            ui::End();
//}
//
//void GUI::renderGuiStyle2() noexcept
//{
//    ui::SetNextWindowSize({ 600.0f, 0.0f });
//    ui::Begin("Osiris", nullptr, windowFlags | ImGuiWindowFlags_NoTitleBar);
//
//    if (ui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip)) {
//        if (ui::BeginTabItem("Aimbot")) {
//            renderAimbotWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Anti aim")) {
//            renderAntiAimWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Triggerbot")) {
//            renderTriggerbotWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Backtrack")) {
//            renderBacktrackWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Glow")) {
//            renderGlowWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Chams")) {
//            renderChamsWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Esp")) {
//            renderEspWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Visuals")) {
//            renderVisualsWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Skin changer")) {
//            renderSkinChangerWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Sound")) {
//            renderSoundWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Style")) {
//            renderStyleWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Misc")) {
//            renderMiscWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Reportbot")) {
//            renderReportbotWindow(true);
//            ui::EndTabItem();
//        }
//        if (ui::BeginTabItem("Config")) {
//            renderConfigWindow(true);
//            ui::EndTabItem();
//        }
//        ui::EndTabBar();
//    }
//
//    ui::End();
//}
