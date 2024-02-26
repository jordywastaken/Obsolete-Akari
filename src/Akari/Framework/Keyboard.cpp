
#include "Keyboard.hpp"
#include "Renderer.hpp"

Keyboard g_Keyboard;

void Keyboard::Open(KeyboardFn cb, const wchar_t* title, const wchar_t* initText, int maxLength)
{
	// Check if widgets can be drawn
	if (!g_Helpers.system_plugin || !g_Helpers.page_autooff_guide)
		return;

	if (m_IsRunning)
		return;

	if (!m_KeyboardTextField)
	{
		m_KeyboardTextField = new paf::PhText(g_Helpers.page_autooff_guide);
		if (!m_KeyboardTextField)
			return;

		m_KeyboardTextField->SetName("KeyboardTextField")
			.SetColor(paf::vec4())
			.SetStyle(0x13, int(0x70));
	}

	m_CallbackExecuted = false;
	m_Callback = cb;

	// Set init text
	m_InitText = initText;
	m_KeyboardTextField->SetText(m_InitText);

	// Set keyboard callback
	m_KeyboardTextField->SetCallBack(0x2000F, [](paf::PhWidget* pWidget, paf::PhEvent* pEvent, void* unknown)
	{
		if (g_Keyboard.m_IsRunning && g_Keyboard.m_KeyboardTextField)
		{
			g_Keyboard.m_IsRunning = false;

			g_Keyboard.m_KeyboardTextField->GetText(g_Keyboard.m_Result);
			if (g_Keyboard.m_Result != g_Keyboard.m_InitText)
				g_Keyboard.m_Callback(g_Keyboard.m_Result);
		}

		g_Keyboard.m_CallbackExecuted = true;

	}, nullptr);

	paf::PhEditText::OskParam oskParam = paf::PhEditText::OskParam::OskParam();

	// Set keyboard parameters
	oskParam.title = title;
	*(int*)(m_KeyboardTextField + 0x24) |= 0x1B0;
	m_KeyboardTextField->SetStyle(52, true);
	m_KeyboardTextField->SetStyle(63, true);
	//oskParam.dword_0x4 = 0x19000000;
	//oskParam.dword_0x8 = 0x10000000;
	//oskParam.dword_0x10 = 3;

	// Set the max length of the keyboard output
	m_KeyboardTextField->SetStyle(62, maxLength - 1);

	// Spawn keyboard
	int ret = m_KeyboardTextField->BeginEditText(oskParam, 0);
	if (ret == 0)
		m_IsRunning = true;

	vsh::printf("[Keyboard]: BeginEditText=%p\n", ret);
}

void Keyboard::Close()
{
	if (m_KeyboardTextField)
	{
		m_KeyboardTextField->EndEditText();
		m_CallbackExecuted = false;
		m_IsRunning = false;

		delete m_KeyboardTextField;
		m_KeyboardTextField = nullptr;

		vsh::vshmain_6D5FC398(-1, -1, 0);
	}
}

void Keyboard::OnFrameUpdate()
{
	if (m_CallbackExecuted)
		Close();
}