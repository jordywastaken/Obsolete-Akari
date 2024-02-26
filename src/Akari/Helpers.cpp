
#include "Helpers.hpp"

Helpers g_Helpers;

void Helpers::OnFrameUpdate()
{
	UpdatePointers();
}

void Helpers::UpdatePointers()
{
	system_plugin = paf::View::Find("system_plugin");
	page_autooff_guide = system_plugin ? system_plugin->FindWidget("page_autooff_guide") : nullptr;
}

void Helpers::PlaySystemSound(const char* soundAlias)
{
	if (!system_plugin)
		return;

	system_plugin->PlaySound(soundAlias);
}