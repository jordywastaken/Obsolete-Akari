
#pragma once

#include <vshlib.hpp>

using KeyboardFn = void(*)(std::wstring const&);

class Keyboard
{
public:
	void Open(KeyboardFn cb, const wchar_t* title, const wchar_t* initText, int maxLength);
	void Close();

	void OnFrameUpdate();

public:
	KeyboardFn m_Callback;
	paf::PhText* m_KeyboardTextField;
	std::wstring m_InitText;
	std::wstring m_Result;
	bool m_IsRunning;
	bool m_CallbackExecuted;
};

extern Keyboard g_Keyboard;