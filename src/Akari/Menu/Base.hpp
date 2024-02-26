
#pragma once

#include <vshlib.hpp>

using Function = void(*)();

class Menu
{
public:
	Menu() = default;
	Menu(Function mainMenu, Function drawHelper);
	~Menu();

	bool IsOpened();
	bool IsFullyOpened();
	void OnFrameUpdate();

	void MenuTab(const wchar_t* name, Function callback);
	void Submenu(const wchar_t* name, Function callback);
	Menu& Option(const wchar_t* option);
	Menu& Action(Function callback);
	Menu& Toggle(bool& var);
	Menu& Toggle(bool& var, Function onEnable, Function onDisable);
	Menu& Slider(float& var, float min, float max, float step, unsigned int delay = 100, const wchar_t* format = L"%.2f");
	Menu& Slider(int& var, int min, int max, int step, unsigned int delay = 100, const wchar_t* format = L"%i");
	Menu& ColorPreview(paf::vec4 color);
	Menu& InternalSubmenu(Function callback);

private:
	bool IsOpenPressed();
	bool IsEnterPressed();
	bool IsBackPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsShoulderLeftPressed();
	bool IsShoulderRightPressed();
	bool IsHovered();
	bool IsPressed();

	void OnOpen();
	void OnClose();
	void OnCancel();
	void OnScrollUp();
	void OnScrollDown();
	void OnShoulderLeft();
	void OnShoulderRight();

	void UpdateButtons();
	void UpdateFadeColor();
	void UpdateUI();

	void EnterSubmenu(Function submenu);

	void DrawHeader();
	void DrawMenuTab(const wchar_t* name);
	void DrawSubmenu(const wchar_t* name);
	void DrawOption(const wchar_t* name);
	void DrawToggle(bool var);
	void DrawSlider(const wchar_t* format, float progressFactor);
	void DrawRightText(const wchar_t* text);
	void DrawRightColor(paf::vec4 color);

	template <typename T>
	void processOptionItemControls(T& var, T min, T max, T step)
	{
		if (IsHovered() && m_StateInSubmenu)
		{
			if (IsLeftPressed())
			{
				if (var <= min)
					var = min;
				else
					var -= step;
			}

			if (var < min)
				var = min;

			if (IsRightPressed())
			{
				if (var >= max)
					var = max;
				else
					var += step;
			}

			if (var > max)
				var = max;
		}
	}

public:
	paf::vec4 m_ColorFade{ 255, 0, 0, 255 };

private:
	bool m_StateInSubmenu{};

	static const int m_MaxSubmenuLevel = 5;
	Function m_FnMenu{};
	Function m_FnHelper{};
	Function m_FnCurrentMenuTab{};
	Function m_FnCurrentSubmenu{};
	Function m_FnPreviousSubmenu[m_MaxSubmenuLevel]{};
	int m_PreviousSubmenuOption[m_MaxSubmenuLevel]{};

	int m_CurrentSubmenuLevel{};
	int m_CurrentPrintingMenuTab{};
	int m_CurrentPrintingOptionInMenu{};
	int m_CurrentPrintingOptionInSubmenu{};

	int m_CurrentMenuTab = 1;
	int m_CurrentOptionInMenu = 1;
	int m_CurrentOptionInSubmenu = 1;

	int m_TotalMenuTabs{};
	int m_TotalOptionsInMenu{};
	int m_TotalOptionsInSubmenu{};

	float m_OpacityHeader = 0.0;
	float m_OpacityMenuTab = 0.0;
	float m_OpacitySubmenu = 0.0;
	float m_OpacityOption = 0.0;
	float m_OpacityTitle = 0.0;

	float m_SizeWidth = 0.0;
	float m_SizeHeader = 50.0;
	float m_SizeHighlightbarWidth = 3.0;
	float m_SizeSpacing = 5.0;
	float m_SizeOptionStart = 25.0;
	float m_SizeBorderThickness = 2.0;

	int m_TimeSlider = 200;
};

extern Menu g_Menu;