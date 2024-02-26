
#include "Base.hpp"
#include "../../Akari.hpp"

Menu g_Menu;

Menu::Menu(Function mainMenu, Function drawHelper)
	: m_FnMenu(mainMenu), m_FnHelper(drawHelper) 
{
	vsh::ShowNavigationMessage(L"Akari successfully loaded\r\nPress \uF889 + \uF88A to open the menu");
}

Menu::~Menu()
{
	m_OpacityOption = 0.f;
	m_OpacitySubmenu = 0.f;
	m_OpacityMenuTab = 0.f;
	m_OpacityTitle = 0.f;
	m_OpacityHeader = 0.f;

	Timer::Sleep(50);
}

bool Menu::IsOpened()
{
	return m_OpacityHeader > 0.0;
}

bool Menu::IsFullyOpened()
{
	return m_OpacityTitle == 255.0;
}

bool Menu::IsOpenPressed()
{
	if (g_Pad.IsButtonBinds(Pad::BUTTON_L2, Pad::BUTTON_L3))
	{
		g_Helpers.PlaySystemSound("snd_system_ok");
		return true;
	}
	return false;
}

bool Menu::IsEnterPressed()
{
	if (g_Pad.IsButtonDownWithDelay(Pad::BUTTON_CROSS, 200))
	{
		g_Helpers.PlaySystemSound("snd_system_ok");
		return true;
	}
	return false;
}

bool Menu::IsBackPressed()
{
	if (g_Pad.IsButtonDownWithDelay(Pad::BUTTON_CIRCLE, 200))
	{
		g_Helpers.PlaySystemSound("snd_cancel");
		return true;
	}
	return false;
}

bool Menu::IsUpPressed()
{
	if (g_Pad.IsButtonDownWithDelay(Pad::BUTTON_PAD_UP, 200))
	{
		g_Helpers.PlaySystemSound("snd_cursor");
		return true;
	}
	return false;
}

bool Menu::IsDownPressed()
{
	if (g_Pad.IsButtonDownWithDelay(Pad::BUTTON_PAD_DOWN, 200))
	{
		g_Helpers.PlaySystemSound("snd_cursor");
		return true;
	}
	return false;
}

bool Menu::IsLeftPressed()
{
	return g_Pad.IsButtonDownWithDelay(Pad::BUTTON_PAD_LEFT, m_TimeSlider);
}

bool Menu::IsRightPressed()
{
	return g_Pad.IsButtonDownWithDelay(Pad::BUTTON_PAD_RIGHT, m_TimeSlider);
}

bool Menu::IsShoulderLeftPressed()
{
	if (g_Pad.IsButtonDownWithDelay(Pad::BUTTON_L1, 200))
	{
		g_Helpers.PlaySystemSound("snd_cursor");
		return true;
	}
	return false;
}

bool Menu::IsShoulderRightPressed()
{
	if (g_Pad.IsButtonDownWithDelay(Pad::BUTTON_R1, 200))
	{
		g_Helpers.PlaySystemSound("snd_cursor");
		return true;
	}
	return false;
}

bool Menu::IsHovered()
{
	return (m_OpacityTitle == 255) && m_StateInSubmenu ? (m_CurrentPrintingOptionInSubmenu == m_CurrentOptionInSubmenu)
		: (m_CurrentPrintingOptionInMenu == m_CurrentOptionInMenu);
}

bool Menu::IsPressed()
{
	if (IsHovered())
		return IsEnterPressed();

	return false;
}

void Menu::OnOpen()
{
	g_Timer.Add(&m_SizeWidth, g_Config.menu.layout.width, 450);
	g_Timer.Add(&m_OpacityHeader, 255.f, 450);
	g_Timer.Add(&m_OpacityTitle, 255.f, 300, 700);

	g_Timer.Add(&m_OpacityMenuTab, 255.f, 300, 600);

	g_Timer.Add(&m_OpacitySubmenu, 255.f, 300, 700);
	g_Timer.Add(&m_OpacityOption, m_StateInSubmenu ? 255.f : 128.f, 300, 700);
}

void Menu::OnClose()
{
	g_Timer.Add(&m_OpacityOption, 0.f, 300);
	g_Timer.Add(&m_OpacitySubmenu, 0.f, 300);

	g_Timer.Add(&m_OpacityMenuTab, 0.f, 300, 200);

	g_Timer.Add(&m_OpacityTitle, 0.f, 300);
	g_Timer.Add(&m_OpacityHeader, 0.f, 400, 500);
	g_Timer.Add(&m_SizeWidth, 0.f, 400, 500);
}

void Menu::OnCancel()
{
	if (m_StateInSubmenu)
	{
		if (m_CurrentSubmenuLevel == 0)
		{
			m_StateInSubmenu = false;
			m_CurrentOptionInSubmenu = 1;
			g_Timer.Add(&m_OpacityOption, 128.f, 200);
		}

		if (m_CurrentSubmenuLevel > 0)
		{
			m_CurrentSubmenuLevel--;
			m_FnCurrentSubmenu = m_FnPreviousSubmenu[m_CurrentSubmenuLevel];
			m_CurrentOptionInSubmenu = m_PreviousSubmenuOption[m_CurrentSubmenuLevel];
		}
	}
	else
		OnClose();
}

void Menu::OnScrollUp()
{
	int* currentOption = m_StateInSubmenu ? &m_CurrentOptionInSubmenu : &m_CurrentOptionInMenu;
	int* totalOptions = m_StateInSubmenu ? &m_TotalOptionsInSubmenu : &m_TotalOptionsInMenu;

	(*currentOption)--;
	if (*currentOption < 1)
		*currentOption = *totalOptions;
}

void Menu::OnScrollDown()
{
	int* currentOption = m_StateInSubmenu ? &m_CurrentOptionInSubmenu : &m_CurrentOptionInMenu;
	int* totalOptions = m_StateInSubmenu ? &m_TotalOptionsInSubmenu : &m_TotalOptionsInMenu;

	(*currentOption)++;
	if (*currentOption > *totalOptions)
		*currentOption = 1;
}

void Menu::OnShoulderLeft()
{
	m_CurrentMenuTab--;
	if (m_CurrentMenuTab < 1)
		m_CurrentMenuTab = m_TotalMenuTabs;

	m_CurrentOptionInMenu = 1;
	m_CurrentOptionInSubmenu = 1;

	m_FnCurrentMenuTab = nullptr;
	m_FnCurrentSubmenu = nullptr;

	m_StateInSubmenu = false;
	m_CurrentSubmenuLevel = 0;

	m_OpacityOption = 128.0;
}

void Menu::OnShoulderRight()
{
	m_CurrentMenuTab++;
	if (m_CurrentMenuTab > m_TotalMenuTabs)
		m_CurrentMenuTab = 1;

	m_CurrentOptionInMenu = 1;
	m_CurrentOptionInSubmenu = 1;

	m_FnCurrentMenuTab = nullptr;
	m_FnCurrentSubmenu = nullptr;

	m_StateInSubmenu = false;
	m_CurrentSubmenuLevel = 0;

	m_OpacityOption = 128.0;
}

void Menu::UpdateButtons()
{
	if (!g_Keyboard.m_IsRunning)
	{
		if (!IsOpened())
		{
			if (IsOpenPressed())
				OnOpen();
		}
		else if (IsFullyOpened())
		{
			if (IsOpenPressed())
				OnClose();
			else if (IsBackPressed())
				OnCancel();
			else if (IsUpPressed())
				OnScrollUp();
			else if (IsDownPressed())
				OnScrollDown();
			else if (IsShoulderLeftPressed())
				OnShoulderLeft();
			else if (IsShoulderRightPressed())
				OnShoulderRight();
		}
	}

	g_Pad.DisableSystemButton(Pad::BUTTON_CROSS, IsOpened() && !g_Keyboard.m_IsRunning);
	g_Pad.DisableSystemButton(Pad::BUTTON_CIRCLE, IsOpened() && !g_Keyboard.m_IsRunning);
	g_Pad.DisableSystemButton(Pad::BUTTON_PAD_UP, IsOpened() && !g_Keyboard.m_IsRunning);
	g_Pad.DisableSystemButton(Pad::BUTTON_PAD_DOWN, IsOpened() && !g_Keyboard.m_IsRunning);
	g_Pad.DisableSystemButton(Pad::BUTTON_PAD_LEFT, IsOpened() && !g_Keyboard.m_IsRunning);
	g_Pad.DisableSystemButton(Pad::BUTTON_PAD_RIGHT, IsOpened() && !g_Keyboard.m_IsRunning);
	g_Pad.DisableSystemButton(Pad::BUTTON_L1, IsOpened() && !g_Keyboard.m_IsRunning);
	g_Pad.DisableSystemButton(Pad::BUTTON_R1, IsOpened() && !g_Keyboard.m_IsRunning);
}

void Menu::UpdateFadeColor()
{
	// Red to green
	if (m_ColorFade == paf::vec4(255, 0, 0, 255))
		g_Timer.Add(&m_ColorFade, paf::vec4(255, 255, 0, 255), 4000);
	if (m_ColorFade == paf::vec4(255, 255, 0, 255))
		g_Timer.Add(&m_ColorFade, paf::vec4(0, 255, 0, 255), 4000);

	// Green to blue
	if (m_ColorFade == paf::vec4(0, 255, 0, 255))
		g_Timer.Add(&m_ColorFade, paf::vec4(0, 255, 255, 255), 4000);
	if (m_ColorFade == paf::vec4(0, 255, 255, 255))
		g_Timer.Add(&m_ColorFade, paf::vec4(0, 0, 255, 255), 4000);

	// Blue to red
	if (m_ColorFade == paf::vec4(0, 0, 255, 255))
		g_Timer.Add(&m_ColorFade, paf::vec4(255, 0, 255, 255), 4000);
	if (m_ColorFade == paf::vec4(255, 0, 255, 255))
		g_Timer.Add(&m_ColorFade, paf::vec4(255, 0, 0, 255), 4000);
}

void Menu::UpdateUI()
{
	if (m_FnHelper)
		m_FnHelper();

	if (IsOpened())
	{
		m_TotalMenuTabs = m_CurrentPrintingMenuTab;
		m_TotalOptionsInMenu = m_CurrentPrintingOptionInMenu;
		m_TotalOptionsInSubmenu = m_CurrentPrintingOptionInSubmenu;

		m_CurrentPrintingMenuTab = 0;
		m_CurrentPrintingOptionInMenu = 0;
		m_CurrentPrintingOptionInSubmenu = 0;

		DrawHeader();

		if (m_OpacityTitle == 255.0)
			m_SizeWidth = g_Config.menu.layout.width;

		if (m_FnMenu)
			m_FnMenu();

		if (m_FnCurrentMenuTab)
			m_FnCurrentMenuTab();

		if (m_FnCurrentSubmenu)
			m_FnCurrentSubmenu();
	}
}

void Menu::OnFrameUpdate()
{
	UpdateButtons();
	UpdateFadeColor();
	UpdateUI();
}

void Menu::EnterSubmenu(Function submenu)
{
	m_FnPreviousSubmenu[m_CurrentSubmenuLevel] = m_FnCurrentSubmenu;
	m_PreviousSubmenuOption[m_CurrentSubmenuLevel] = m_CurrentOptionInSubmenu;
	m_CurrentSubmenuLevel++;
	m_FnCurrentSubmenu = submenu;

	m_CurrentOptionInSubmenu = 1;
}

void Menu::DrawHeader()
{
	// Background
	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2, g_Config.menu.layout.position.y),
		paf::vec2(m_SizeWidth, 50),
		paf::vec4(0, 0, 0, 0.5 * m_OpacityHeader),
		Renderer::Left,
		Renderer::Top);

	// Top bar
	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2, g_Config.menu.layout.position.y),
		paf::vec2(m_SizeWidth, m_SizeHighlightbarWidth * 2),
		g_Config.menu.color.outlineFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityHeader) : paf::vec4(g_Config.menu.color.outline.r, g_Config.menu.color.outline.g, g_Config.menu.color.outline.b, m_OpacityHeader),
		Renderer::Left,
		Renderer::Top);

	// Title
	g_Render.Text(
		L"Akari",
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + m_SizeOptionStart, g_Config.menu.layout.position.y + m_SizeHighlightbarWidth + m_SizeHeader / 2),
		34,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityMenuTab) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityMenuTab),
		Renderer::Left,
		Renderer::Centered);

	// Version
	g_Render.Text(
		L"Version: 2.0.0",
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart, g_Config.menu.layout.position.y + m_SizeHighlightbarWidth + m_SizeHeader / 2 - 9),
		15,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityMenuTab) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityMenuTab),
		Renderer::Right,
		Renderer::Centered);

	// Current option
	g_Render.Text(
		vsh::wva(L"Option: %i/%i", m_StateInSubmenu ? m_CurrentOptionInSubmenu : m_CurrentOptionInMenu, m_StateInSubmenu ? m_TotalOptionsInSubmenu : m_TotalOptionsInMenu),
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart, g_Config.menu.layout.position.y + m_SizeHighlightbarWidth + m_SizeHeader / 2 + 7),
		15,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityMenuTab) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityMenuTab),
		Renderer::Right,
		Renderer::Centered);
}

void Menu::DrawMenuTab(const wchar_t* name)
{
	float itemWidth = (m_SizeWidth - (m_TotalMenuTabs - 1) * (m_SizeSpacing * 2)) / m_TotalMenuTabs;

	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + (itemWidth + m_SizeSpacing * 2) * (m_CurrentPrintingMenuTab - 1), g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing),
		paf::vec2(itemWidth, g_Config.menu.layout.optionSize + m_SizeHighlightbarWidth),
		paf::vec4(0, 0, 0, 0.5 * m_OpacityMenuTab));

	if (m_CurrentMenuTab == m_CurrentPrintingMenuTab)
	{
		g_Render.Text(
			name,
			paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + (itemWidth + m_SizeSpacing * 2) * (m_CurrentPrintingMenuTab - 1) + m_SizeOptionStart / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth / 2 + g_Config.menu.layout.optionSize / 2),
			g_Config.menu.layout.optionSize * 0.9,
			g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityMenuTab) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityMenuTab),
			Renderer::Left,
			Renderer::Centered);

		g_Render.Rectangle(
			paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + (itemWidth + m_SizeSpacing * 2) * (m_CurrentPrintingMenuTab - 1), g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing),
			paf::vec2(itemWidth, m_SizeHighlightbarWidth),
			g_Config.menu.color.outlineFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityMenuTab) : paf::vec4(g_Config.menu.color.outline.r, g_Config.menu.color.outline.g, g_Config.menu.color.outline.b, m_OpacityMenuTab));
	}
	else
		g_Render.Text(
		name,
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + (itemWidth + m_SizeSpacing * 2) * (m_CurrentPrintingMenuTab - 1) + m_SizeOptionStart / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth / 2 + g_Config.menu.layout.optionSize / 2),
		g_Config.menu.layout.optionSize * 0.9,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityMenuTab) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityMenuTab),
		Renderer::Left,
		Renderer::Centered);
}

void Menu::DrawSubmenu(const wchar_t* name)
{
	float width = (m_SizeWidth - m_SizeSpacing) * 0.225;

	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInMenu - 1)),
		paf::vec2(width, g_Config.menu.layout.optionSize),
		paf::vec4(0, 0, 0, 0.5 * m_OpacitySubmenu));

	if (m_CurrentOptionInMenu == m_CurrentPrintingOptionInMenu)
	{
		g_Render.Text(
			name,
			paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + m_SizeOptionStart / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInMenu - 1) + g_Config.menu.layout.optionSize / 2),
			g_Config.menu.layout.optionSize * 0.9,
			g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacitySubmenu) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacitySubmenu),
			Renderer::Left,
			Renderer::Centered);

		g_Render.Rectangle(
			paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInMenu - 1)),
			paf::vec2(m_SizeHighlightbarWidth, g_Config.menu.layout.optionSize),
			g_Config.menu.color.outlineFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacitySubmenu) : paf::vec4(g_Config.menu.color.outline.r, g_Config.menu.color.outline.g, g_Config.menu.color.outline.b, m_OpacitySubmenu));
	}
	else
		g_Render.Text(
		name,
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + m_SizeOptionStart / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInMenu - 1) + g_Config.menu.layout.optionSize / 2),
		g_Config.menu.layout.optionSize * 0.9,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacitySubmenu) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacitySubmenu),
		Renderer::Left,
		Renderer::Centered);
}

void Menu::DrawOption(const wchar_t* name)
{
	float submenuWidth = (m_SizeWidth - m_SizeSpacing) * 0.225;
	float width = m_SizeWidth - submenuWidth - m_SizeSpacing;
	float startX = submenuWidth + m_SizeSpacing;

	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + startX, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1)),
		paf::vec2(width, g_Config.menu.layout.optionSize),
		paf::vec4(0, 0, 0, 0.5 * m_OpacityOption));

	if (m_CurrentOptionInSubmenu == m_CurrentPrintingOptionInSubmenu && m_StateInSubmenu)
	{
		g_Render.Text(
			name,
			paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + startX + m_SizeOptionStart / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2),
			g_Config.menu.layout.optionSize * 0.9,
			g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityOption) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityOption),
			Renderer::Left,
			Renderer::Centered);

		g_Render.Rectangle(
			paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + startX, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1)),
			paf::vec2(m_SizeHighlightbarWidth, g_Config.menu.layout.optionSize),
			g_Config.menu.color.outlineFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityOption) : paf::vec4(g_Config.menu.color.outline.r, g_Config.menu.color.outline.g, g_Config.menu.color.outline.b, m_OpacityOption));
	}
	else
		g_Render.Text(
		name,
		paf::vec2(g_Config.menu.layout.position.x - m_SizeWidth / 2 + startX + m_SizeOptionStart / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2),
		g_Config.menu.layout.optionSize * 0.9,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityOption) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityOption),
		Renderer::Left,
		Renderer::Centered);
}

void Menu::DrawToggle(bool var)
{
	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart - m_SizeBorderThickness - g_Config.menu.layout.optionSize * 0.4, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2 - g_Config.menu.layout.optionSize * 0.2),
		paf::vec2(g_Config.menu.layout.optionSize * 0.4, g_Config.menu.layout.optionSize * 0.4),
		var ? (g_Config.menu.color.outlineFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityOption) : paf::vec4(g_Config.menu.color.outline.r, g_Config.menu.color.outline.g, g_Config.menu.color.outline.b, m_OpacityOption)) : paf::vec4(40, 40, 40, m_OpacityOption));
}

void Menu::DrawSlider(const wchar_t* format, float progressFactor)
{
	float width = m_SizeWidth * 0.125;

	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart - m_SizeBorderThickness - width, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2 - g_Config.menu.layout.optionSize * 0.125),
		paf::vec2(width, g_Config.menu.layout.optionSize * 0.25),
		paf::vec4(40, 40, 40, m_OpacityOption));

	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart - m_SizeBorderThickness - width, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2 - g_Config.menu.layout.optionSize * 0.125),
		paf::vec2(width * progressFactor, g_Config.menu.layout.optionSize * 0.25),
		g_Config.menu.color.outlineFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, 0.5 * m_OpacityOption) : paf::vec4(g_Config.menu.color.outline.r, g_Config.menu.color.outline.g, g_Config.menu.color.outline.b, m_OpacityOption));

	g_Render.Text(
		format,
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart - m_SizeBorderThickness - width - m_SizeOptionStart / 2, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2),
		g_Config.menu.layout.optionSize * 0.9,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityOption) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityOption),
		Renderer::Right,
		Renderer::Centered);
}

void Menu::DrawRightText(const wchar_t* text)
{
	g_Render.Text(
		text,
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2),
		g_Config.menu.layout.optionSize,
		g_Config.menu.color.textFade ? paf::vec4(m_ColorFade.r, m_ColorFade.g, m_ColorFade.b, m_OpacityOption) : paf::vec4(g_Config.menu.color.text.r, g_Config.menu.color.text.g, g_Config.menu.color.text.b, m_OpacityOption),
		Renderer::Right,
		Renderer::Centered);
}

void Menu::DrawRightColor(paf::vec4 color)
{
	g_Render.Rectangle(
		paf::vec2(g_Config.menu.layout.position.x + m_SizeWidth / 2 - m_SizeOptionStart - g_Config.menu.layout.optionSize, g_Config.menu.layout.position.y + m_SizeHeader + m_SizeSpacing + m_SizeHighlightbarWidth + g_Config.menu.layout.optionSize + m_SizeSpacing + (g_Config.menu.layout.optionSize + g_Config.menu.layout.optionSpacing) * (m_CurrentPrintingOptionInSubmenu - 1) + g_Config.menu.layout.optionSize / 2 - g_Config.menu.layout.optionSize * 0.25),
		paf::vec2(g_Config.menu.layout.optionSize, g_Config.menu.layout.optionSize * 0.5),
		paf::vec4(color.r, color.g, color.b, color.a * m_OpacityOption / 255));
}

void Menu::MenuTab(const wchar_t* name, Function callback)
{
	m_CurrentPrintingMenuTab++;

	DrawMenuTab(name);

	if (m_CurrentPrintingMenuTab == m_CurrentMenuTab)
		m_FnCurrentMenuTab = callback;
}

void Menu::Submenu(const wchar_t* name, Function callback)
{
	m_CurrentPrintingOptionInMenu++;

	DrawSubmenu(name);

	if ((m_CurrentPrintingOptionInMenu == m_CurrentOptionInMenu) && (m_CurrentSubmenuLevel == 0))
		m_FnCurrentSubmenu = callback;

	if (IsPressed() && IsFullyOpened())
	{
		g_Timer.Add(&m_OpacityOption, 255.f, 200);
		m_StateInSubmenu = true;
	}
}

Menu& Menu::Option(const wchar_t* option)
{
	m_CurrentPrintingOptionInSubmenu++;
	DrawOption(option);
	return *this;
}

Menu& Menu::Action(Function callback)
{
	if (IsPressed() && m_StateInSubmenu && callback)
		callback();

	return *this;
}

Menu& Menu::Toggle(bool& var)
{
	DrawToggle(var);

	if (IsPressed() && m_StateInSubmenu)
		var ^= 1;

	return *this;
}

Menu& Menu::Toggle(bool& var, Function onEnable, Function onDisable)
{
	DrawToggle(var);

	if (IsPressed() && m_StateInSubmenu)
	{
		var ^= 1;

		if (var && onEnable)
			onEnable();

		if (!var && onDisable)
			onDisable();
	}

	return *this;
}

Menu& Menu::Slider(float& var, float min, float max, float step, unsigned int delay, const wchar_t* format)
{
	processOptionItemControls<float>(var, min, max, step);
	DrawSlider(vsh::wva(format, var), (var - min) / (max - min));

	if (IsHovered() && m_StateInSubmenu)
		m_TimeSlider = delay;

	return *this;
}

Menu& Menu::Slider(int& var, int min, int max, int step, unsigned int delay, const wchar_t* format)
{
	processOptionItemControls<int>(var, min, max, step);
	DrawSlider(vsh::wva(format, var), static_cast<float>(var - min) / static_cast<float>(max - min));

	if (IsHovered() && m_StateInSubmenu)
		m_TimeSlider = delay;

	return *this;
}

Menu& Menu::ColorPreview(paf::vec4 color)
{
	DrawRightColor(color);
	return *this;
}

Menu& Menu::InternalSubmenu(Function callback)
{
	DrawRightText(L">");

	if (IsPressed() && m_StateInSubmenu)
		EnterSubmenu(callback);

	return *this;
}