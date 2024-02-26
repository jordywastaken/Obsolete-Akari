
#include "Settings.hpp"
#include "../Base.hpp"
#include "../../Config.hpp"

void TabSettings()
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
}