
#pragma once

#include "Akari/Config.hpp"
#include "Akari/Hooks.hpp"

#include "Akari/Framework/Keyboard.hpp"
#include "Akari/Framework/Pad.hpp"
#include "Akari/Framework/Renderer.hpp"

#include "Akari/Menu/Base.hpp"
#include "Akari/Menu/Menu.hpp"

#include "Utils/Timer.hpp"

class Akari
{
public:
	static void Initialize()
	{
		// Wait until the XMB is fully loaded before initializing our module
		do
		{
			Timer::Sleep(1000);
		} while (!paf::View::Find("explore_plugin"));

		// Utilities
		g_Timer = Timer();

		// Framework
		g_Pad = Pad();
		g_Render = Renderer();

		// User interface
		g_Config = Config();
		g_Menu = Menu(AkariMenu, nullptr);

		InstallHooks();

		vsh::puts("[Akari]: Initialize() executed successfully");
	}

	static void Shutdown()
	{
		RemoveHooks();

		g_Menu.~Menu();
		g_Render.~Renderer();

		vsh::puts("[Akari]: Shutdown() executed successfully");
	}
};