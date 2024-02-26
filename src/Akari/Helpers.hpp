
#pragma once

#include <vshlib.hpp>

class Helpers
{
public:
	Helpers() = default;
	void OnFrameUpdate();

	void PlaySystemSound(const char* soundAlias);
	
private:
	void UpdatePointers();

public:
	paf::View* system_plugin;
	paf::PhWidget* page_autooff_guide;
};

extern Helpers g_Helpers;