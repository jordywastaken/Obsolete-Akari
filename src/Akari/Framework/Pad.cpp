
#include "Pad.hpp"
#include "../../Utils/Timer.hpp"
#include <vsh/sys_io.hpp>
#include <vsh/stdc.hpp>

Pad g_Pad;

void Pad::Process(CellPadData* padData)
{
	if (!padData)
		return;

	vsh::memcpy(&m_PadData, padData, sizeof(CellPadData));

	// Update buttons data
	m_PreviousButtonsData = m_CurrentButtonsData;
	m_CurrentButtonsData = (m_PadData.button[CELL_PAD_BTN_OFFSET_DIGITAL1] << 8) | m_PadData.button[CELL_PAD_BTN_OFFSET_DIGITAL2];

	m_CurrentTime = Timer::GetTimeNow();

	ProcessDisabledButtons(padData);
	ProcessDisabledAnalogs(padData);
	ProcessButtonsPressed();
}

void Pad::ProcessDisabledButtons(CellPadData* padData)
{
	uint32_t& buttonData = *(uint32_t*)&padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1];
	for (int i = 0; i < BUTTONS_COUNT; i++)
	{
		if (m_IsButtonDisabled[i])
			buttonData &= (i < 8) ? ~(1 << i) : ~(256 << i);
	}
}

void Pad::ProcessDisabledAnalogs(CellPadData* padData)
{
	for (int i = 0; i < ANALOGS_COUNT; i++)
	{
		uint16_t& analogInput = *(uint16_t*)&padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X + i];
		if (m_IsAnalogDisabled[i])
			analogInput = 128;
	}
}

void Pad::ProcessButtonsPressed()
{
	for (int i = 0; i < BUTTONS_COUNT; i++)
	{
		if (IsButtonPressed((Buttons)i))
			m_ButtonLastTimePressed[i] = m_CurrentTime;
	}
}

void Pad::DisableSystemButton(Buttons btn, bool disable)
{
	m_IsButtonDisabled[btn] = disable;
}

void Pad::DisableSystemAnalog(Analogs analog, bool disable)
{
	m_IsAnalogDisabled[analog] = disable;
}

bool Pad::IsButtonDown(Buttons btn)
{
	return m_CurrentButtonsData & (1 << btn);
}

bool Pad::IsButtonDownFor(Buttons btn, uint64_t millis)
{
	if ((m_CurrentTime - m_ButtonLastTimePressed[btn]) >= millis)
	{
		if (IsButtonDown(btn))
		{
			m_ButtonLastTimePressed[btn] = m_CurrentTime;
			return true;
		}
	}
	return false;
}

bool Pad::IsButtonDownWithDelay(Buttons btn, uint64_t delay)
{
	if (m_CurrentTime - m_ButtonLastTimeDelay[btn] >= delay)
	{
		if (IsButtonDown(btn))
		{
			m_ButtonLastTimeDelay[btn] = m_CurrentTime;
			return true;
		}
	}
	return false;
}

bool Pad::IsButtonPressed(Buttons btn)
{
	bool isCurrentButtonPressed = (m_CurrentButtonsData & (1 << btn)),
		isPreviousButtonPressed = (m_PreviousButtonsData & (1 << btn));
	return isCurrentButtonPressed && !isPreviousButtonPressed;
}

bool Pad::IsButtonReleased(Buttons btn)
{
	bool isCurrentButtonPressed = (m_CurrentButtonsData & (1 << btn)),
		isPreviousButtonPressed = (m_PreviousButtonsData & (1 << btn));
	return !isCurrentButtonPressed && isPreviousButtonPressed;
}

bool Pad::IsButtonReleasedAfter(Buttons btn, uint64_t millis)
{
	if (IsButtonReleased(btn))
		return (m_CurrentTime - m_ButtonLastTimePressed[btn]) >= millis;

	return false;
}

bool Pad::IsButtonBinds(Buttons btn1, Buttons btn2)
{
	return IsButtonDown(btn1) && IsButtonDown(btn2);
}

bool Pad::IsButtonBindsFor(Buttons btn1, Buttons btn2, uint64_t millis)
{
	return IsButtonDownFor(btn1, millis) && IsButtonDownFor(btn2, millis);
}

double Pad::GetAnalogInput(Analogs analog)
{
	int16_t analogInput = m_PadData.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X + analog] - 128;
	if ((analogInput > -m_AnalogDeadzone) && (analogInput < m_AnalogDeadzone))
		return 0.0;

	return static_cast<double>(analogInput) / 128.0;
}

double Pad::GetRightAnalogX()
{
	return GetAnalogInput(ANALOG_RIGHT_X);
}

double Pad::GetRightAnalogY()
{
	return GetAnalogInput(ANALOG_RIGHT_Y);
}

double Pad::GetLeftAnalogX()
{
	return GetAnalogInput(ANALOG_LEFT_X);
}

double Pad::GetLeftAnalogY()
{
	return GetAnalogInput(ANALOG_LEFT_Y);
}

void Pad::SetAnalogDeadzone(int16_t deadzone)
{
	m_AnalogDeadzone = deadzone;
}