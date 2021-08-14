#include "pch.h"
#include "GoodGreatAmazing.h"


BAKKESMOD_PLUGIN(GoodGreatAmazing, "Good Great AMAZING", plugin_version, PERMISSION_CUSTOM_TRAINING | PERMISSION_FREEPLAY | PERMISSION_OFFLINE | PERMISSION_ONLINE | PERMISSION_MENU | PERMISSION_PAUSEMENU_CLOSED | PERMISSION_SOCCAR)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void GoodGreatAmazing::onLoad()
{
	cvarManager->log("GoodGreatAmazing plugged in");

	overlayOn = std::make_shared<bool>(false);

	cvarManager->registerCvar("goodgreatamazingplugin_enabled", "1", "Enables the goodgreatamazing plugin.", true, false, 0.0F, false, 0.0F).bindTo(overlayOn);
	cvarManager->getCvar("goodgreatamazingplugin_enabled").addOnValueChanged(std::bind(&GoodGreatAmazing::OnOverlayChanged, this, std::placeholders::_1, std::placeholders::_2));

	*overlayOn = true;

	if (*overlayOn)
		HookAllEvents();

	cvarManager->log("GoodGreatAmazing plugged in");
}

void GoodGreatAmazing::HookAllEvents()
{
	if (hooked)
		return;

	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", bind(&GoodGreatAmazing::OnGameLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", bind(&GoodGreatAmazing::OnGameDestroy, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", bind(&GoodGreatAmazing::OnGameDestroy, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.Ball_TA.EventExploded", bind(&GoodGreatAmazing::OnGameDestroy, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Team_TA.AddPlayerToTeam", bind(&GoodGreatAmazing::OnGameLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Team_TA.RemovePlayerFromTeam", bind(&GoodGreatAmazing::OnGameDestroy, this, std::placeholders::_1));

	hooked = true;
}

void GoodGreatAmazing::UnhookAllEvents()
{
	if (!hooked)
		return;

	gameWrapper->UnhookEvent("Function GameEvent_Soccar_TA.Active.StartRound");
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded");
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed");
	gameWrapper->UnhookEvent("Function TAGame.Ball_TA.EventExploded");
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Team_TA.AddPlayerToTeam");
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Team_TA.RemovePlayerFromTeam");

	hooked = false;
}

//void GoodGreatAmazing::DisablePsyUI(bool disabled)
//{
//	if (cvarManager->getCvar("cl_rendering_scaleform_disabled").getIntValue() != disabled)
//		cvarManager->getCvar("cl_rendering_scaleform_disabled").setValue(disabled);
//}

int GoodGreatAmazing::getBoostAmount()
{
	CarWrapper localCar = gameWrapper->GetLocalCar();
	if (localCar.IsNull())
		return -1;

	BoostWrapper boostComponent = localCar.GetBoostComponent();
	if (boostComponent.IsNull())
		return -1;

	return boostComponent.GetCurrentBoostAmount() * 100;
}

int GoodGreatAmazing::getGameTime()
{
	ServerWrapper localServer = gameWrapper->GetGameEventAsServer();
	ServerWrapper onlineServer = gameWrapper->GetOnlineGame();

	if (!gameWrapper->IsInGame())
	{
		if (!gameWrapper->IsInOnlineGame())
			return -1;
		else if (onlineServer.IsNull())
			return -1;

		return onlineServer.GetSecondsRemaining();
	}
	else if (localServer.IsNull())
		return -1;

	return localServer.GetSecondsRemaining();
}

TeamWrapper GoodGreatAmazing::getMyTeam()
{
	ServerWrapper localServer = gameWrapper->GetGameEventAsServer();
	ServerWrapper onlineServer = gameWrapper->GetOnlineGame();

	if (gameWrapper->IsInGame() && !localServer.IsNull())
	{
		ArrayWrapper<TeamWrapper> localServerTeams = localServer.GetTeams();
		PlayerControllerWrapper localServerLocalPrimaryPlayer = localServer.GetLocalPrimaryPlayer();

		if (!localServerTeams.IsNull() && !localServerLocalPrimaryPlayer.IsNull())
			for (TeamWrapper team : localServerTeams)
				if (localServerLocalPrimaryPlayer.GetTeamNum2() == team.GetTeamNum2())
					return team;
	}
	else if (gameWrapper->IsInOnlineGame() && !onlineServer.IsNull())
	{
		ArrayWrapper<TeamWrapper> onlineServerTeams = onlineServer.GetTeams();
		PlayerControllerWrapper onlineServerLocalPrimaryPlayer = onlineServer.GetLocalPrimaryPlayer();

		if (!onlineServerTeams.IsNull() && !onlineServerLocalPrimaryPlayer.IsNull())
			for (TeamWrapper team : onlineServerTeams)
				if (onlineServerLocalPrimaryPlayer.GetTeamNum2() == team.GetTeamNum2())
					return team;
	}
}

TeamWrapper GoodGreatAmazing::getOpposingTeam()
{
	ServerWrapper localServer = gameWrapper->GetGameEventAsServer();
	ServerWrapper onlineServer = gameWrapper->GetOnlineGame();

	if (gameWrapper->IsInGame() && !localServer.IsNull())
	{
		ArrayWrapper<TeamWrapper> localServerTeams = localServer.GetTeams();
		PlayerControllerWrapper localServerLocalPrimaryPlayer = localServer.GetLocalPrimaryPlayer();

		if (!localServerTeams.IsNull() && !localServerLocalPrimaryPlayer.IsNull())
			for (TeamWrapper team : localServerTeams)
				if (localServerLocalPrimaryPlayer.GetTeamNum2() != team.GetTeamNum2())
					return team;
	}
	else if (gameWrapper->IsInOnlineGame() && !onlineServer.IsNull())
	{
		ArrayWrapper<TeamWrapper> onlineServerTeams = onlineServer.GetTeams();
		PlayerControllerWrapper onlineServerLocalPrimaryPlayer = onlineServer.GetLocalPrimaryPlayer();

		if (!onlineServerTeams.IsNull() && !onlineServerLocalPrimaryPlayer.IsNull())
			for (TeamWrapper team : onlineServerTeams)
				if (onlineServerLocalPrimaryPlayer.GetTeamNum2() != team.GetTeamNum2())
					return team;
	}
}

LinearColor GoodGreatAmazing::getTeamColor(TeamWrapper team)
{
	if (!team.IsNull())
		return team.GetCurrentColorList().Get(0);
}

void GoodGreatAmazing::UpdateVars()
{
	if (!*overlayOn)
		return;

	boost = getBoostAmount();
}

void GoodGreatAmazing::OnGameLoad(std::string eventName)
{
	if (!gameWrapper->IsInGame())
		if (!gameWrapper->IsInOnlineGame())
			return;

	if (!loaded)
	{
		if (*overlayOn)
			gameWrapper->RegisterDrawable(std::bind(&GoodGreatAmazing::Render, this, std::placeholders::_1));

		loaded = true;
	}

	//DisablePsyUI(true);
}

void GoodGreatAmazing::OnGameDestroy(std::string eventName)
{
	if (loaded)
	{
		gameWrapper->UnregisterDrawables();

		loaded = false;
	}

	//DisablePsyUI(false);
}

void GoodGreatAmazing::OnOverlayChanged(std::string oldValue, CVarWrapper cvar)
{
	if (oldValue.compare("0") == 0 && cvar.getBoolValue())
	{
		HookAllEvents();
		OnGameLoad("Load");
	}
	else if (oldValue.compare("1") == 0 && !cvar.getBoolValue())
	{
		UnhookAllEvents();
		OnGameDestroy("Destroy");
	}
}

void GoodGreatAmazing::Render(CanvasWrapper canvas)
{
	if (!*overlayOn)
		return;

	if (!gameWrapper->IsInOnlineGame())
		if (!gameWrapper->IsInGame())
			return;

	UpdateVars();

	screenSize = canvas.GetSize();

	boostBoxSize.X = (int)((screenSize.X * 8.80F) / 100);
	boostBoxSize.Y = (int)((screenSize.X * 4.375F) / 100);
	boostBoxPosition.X = (screenSize.X - boostBoxSize.X) - (int)((screenSize.X * 5.94F) / 100);
	boostBoxPosition.Y = (screenSize.Y - boostBoxSize.Y) - (int)((screenSize.X * 6.98F) / 100);

	canvas.SetPosition(boostBoxPosition);

	std::string boostAmount = "";

	if (boost > 80)
	{
		canvas.SetColor(0, 255, 0, 255);
		boostAmount = "Awesome";
	}
	else if (boost < 24)
	{
		canvas.SetColor(255, 0, 0, 255);
		boostAmount = "  Good  ";
	}
	else
	{
		canvas.SetColor(255, 255, 0, 255);
		boostAmount = "  Great  ";
	}

	boostTextSize = canvas.GetStringSize(boostAmount, 20.0F, 1.0F);
	boostTextPosition.X = boostBoxPosition.X + 12; 
	boostTextPosition.Y = boostBoxPosition.Y + (int)(boostBoxSize.X/6.0F);

	canvas.FillBox(boostBoxSize);
	canvas.SetColor(0, 0, 0, 255);

	canvas.SetPosition(boostTextPosition);

	canvas.DrawString(boostAmount, 2.4F, 2.0F);
}

void GoodGreatAmazing::onUnload()
{
	cvarManager->log("We want numbers back");
}