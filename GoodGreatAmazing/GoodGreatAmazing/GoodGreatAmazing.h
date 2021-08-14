#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class GoodGreatAmazing: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow/*, public BakkesMod::Plugin::PluginWindow*/
{
public:
	virtual void onLoad();
	virtual void onUnload();

	void HookAllEvents();
	void UnhookAllEvents();

	//void DisablePsyUI(bool disabled);

	int getBoostAmount();
	int getGameTime();

	TeamWrapper getMyTeam();
	TeamWrapper getOpposingTeam();
	LinearColor getTeamColor(TeamWrapper team);

	void UpdateVars();
	void OnGameLoad(std::string eventName);
	void OnGameDestroy(std::string eventName);
	void OnOverlayChanged(std::string oldValue, CVarWrapper cvar);
	void Render(CanvasWrapper canvas);

	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;

private:
	bool hooked;
	bool loaded;
	std::shared_ptr<bool> overlayOn;

	int boost;

	std::string gameTime;

	LinearColor myTeamColor;
	LinearColor opposingTeamColor;

	Vector2 screenSize;

	Vector2 boostBoxSize;
	Vector2 boostBoxPosition;
	Vector2F boostTextSize;
	Vector2 boostTextPosition;
};

