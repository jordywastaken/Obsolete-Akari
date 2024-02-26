
#include "Main.hpp"
#include "../../../Akari.hpp"

void TabMain()
{
	g_Menu.Submenu(L"Console manager", []
	{
		g_Menu.Option(L"Beep boop").Action([]
		{
			g_Keyboard.Open([](std::wstring const& text)
			{

				vsh::ShowNavigationMessage(text.c_str());

			}, L"Quack quack", L"am a duck", 32);
		});
	});
}