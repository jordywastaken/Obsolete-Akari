
#include "Menu.hpp"
#include "Base.hpp"

#include "Tabs/Main.hpp"
#include "Tabs/Settings.hpp"
#include "Tabs/CcapiRev5.hpp"

void AkariMenu()
{
	g_Menu.MenuTab(L"Main", TabMain);
	g_Menu.MenuTab(L"Settings", TabSettings);
	g_Menu.MenuTab(L"Experimental CCAPI Rev5", TabCcapiRev5);

	/*g_Menu.MenuTab(L"Main", []
	{
		g_Menu.Submenu(L"Weapon", []
		{
			g_Menu.Option(L"No recoil");
			g_Menu.Option(L"No sway");
			g_Menu.Option(L"No leaning");
			g_Menu.Option(L"No bob");
			g_Menu.Option(L"No spread");
			g_Menu.Option(L"No flinch");
			g_Menu.Option(L"No bullet impacts");
			g_Menu.Option(L"Rapid fire");
			g_Menu.Option(L"Quick reload");
		});
		g_Menu.Submenu(L"View", []
		{
			g_Menu.Option(L"Wallhack").InternalSubmenu([]
			{
				g_Menu.Option(L"Players");
				g_Menu.Option(L"Corpses");
				g_Menu.Option(L"Items");
				g_Menu.Option(L"Lethal/Tactical equipments");
				g_Menu.Option(L"Killstreaks");
			});
			g_Menu.Option(L"Field of view");
			g_Menu.Option(L"Third person");
			g_Menu.Option(L"Third person distance");
			g_Menu.Option(L"Third person angle");
		});
		g_Menu.Submenu(L"Misc", []
		{
			g_Menu.Option(L"End game");
			g_Menu.Option(L"Leave game");
		});
	});
	g_Menu.MenuTab(L"Aim", []
	{
		g_Menu.Submenu(L"Aimbot", []
		{
			g_Menu.Option(L"Enable");
			g_Menu.Option(L"Aim type");
			g_Menu.Option(L"Aim hit-box");
			g_Menu.Option(L"On screen only");
			g_Menu.Option(L"Auto-shoot");
			g_Menu.Option(L"Auto-aim");
		});
		g_Menu.Submenu(L"Anti-aim", nullptr);
	});
	g_Menu.MenuTab(L"Visuals", nullptr);
	g_Menu.MenuTab(L"Host", nullptr);
	g_Menu.MenuTab(L"Settings", []
	{
		g_Menu.Submenu(L"Menu layout", []
		{
			g_Menu.Option(L"Horizontal position").Slider(g_Config.menu.layout.position.x, 0, 1280, 5, 20);
			g_Menu.Option(L"Vertical position").Slider(g_Config.menu.layout.position.y, 0, 720, 5, 20);
			g_Menu.Option(L"Width").Slider(g_Config.menu.layout.width, 550, 1280, 5, 20);
			g_Menu.Option(L"Option size").Slider(g_Config.menu.layout.optionSize, 16.0, 30.0, 0.1, 20);
			g_Menu.Option(L"Option spacing").Slider(g_Config.menu.layout.optionSpacing, 0.0, 5.0, 0.1, 20);
		});
		g_Menu.Submenu(L"Menu colors", []
		{
			g_Menu.Option(L"Outline color").InternalSubmenu([]
			{
				g_Menu.Option(L"Red amount").Slider(g_Config.menu.color.outline.r, 0.0, 255.0, 1.0, 20);
				g_Menu.Option(L"Green amount").Slider(g_Config.menu.color.outline.g, 0.0, 255.0, 1.0, 20);
				g_Menu.Option(L"Blue amount").Slider(g_Config.menu.color.outline.b, 0.0, 255.0, 1.0, 20);
				g_Menu.Option(L"Fade").Toggle(g_Config.menu.color.outlineFade);
				g_Menu.Option(L"Preview").ColorPreview(g_Config.menu.color.outline);
			});
			g_Menu.Option(L"Text color").InternalSubmenu([]
			{
				g_Menu.Option(L"Red amount").Slider(g_Config.menu.color.text.r, 0.0, 255.0, 1.0, 20);
				g_Menu.Option(L"Green amount").Slider(g_Config.menu.color.text.g, 0.0, 255.0, 1.0, 20);
				g_Menu.Option(L"Blue amount").Slider(g_Config.menu.color.text.b, 0.0, 255.0, 1.0, 20);
				g_Menu.Option(L"Fade").Toggle(g_Config.menu.color.textFade);
				g_Menu.Option(L"Preview").ColorPreview(g_Config.menu.color.text);
			});
		});
	});*/
}