#include "pch.h"
#include "GoodGreatAmazing.h"

void GoodGreatAmazing::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string GoodGreatAmazing::GetPluginName() {
	return "GoodGreatAmazing Plugin";
}

void GoodGreatAmazing::RenderSettings() {
	CVarWrapper enableCvar = cvarManager->getCvar("goodgreatamazingplugin_enabled");
	if (!enableCvar) { return; }
	bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox("Enable plugin", &enabled)) {
		enableCvar.setValue(enabled);
	}
}

/* Plugin Settings Window code here
std::string GoodGreatAmazing::GetPluginName() {
	return "GoodGreatAmazing";
}

void GoodGreatAmazing::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> GoodGreatAmazing
void GoodGreatAmazing::RenderSettings() {
	ImGui::TextUnformatted("GoodGreatAmazing plugin settings");
}
*/

/*
// Do ImGui rendering here
void GoodGreatAmazing::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string GoodGreatAmazing::GetMenuName()
{
	return "GoodGreatAmazing";
}

// Title to give the menu
std::string GoodGreatAmazing::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void GoodGreatAmazing::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool GoodGreatAmazing::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool GoodGreatAmazing::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void GoodGreatAmazing::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void GoodGreatAmazing::OnClose()
{
	isWindowOpen_ = false;
}
*/
