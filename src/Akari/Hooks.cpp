
#include <vsh/paf.hpp>
#include <vsh/sys_io.hpp>

#include "../Akari.hpp"
#include "../Utils/Memory/Detours.hpp"
#include "../Utils/Memory/Patterns.hpp"

Detour* pafFrameworkRun_Detour;
Detour* cellPadGetDataExtraInternal_Detour;


/***
* paf::Framework::Run()
* As the name indicates, this function will run the Virtual Shell framework,
* rendering widgets, handling the controller(s) input and do a lot more.
*/
int pafFrameworkRun_Hook(paf::Framework const* framework, float frameTime)
{
	// Update our utilities/framework
	g_Timer.OnFrameUpdate();
	g_Helpers.OnFrameUpdate();
	g_Render.OnFrameUpdate();
	g_Keyboard.OnFrameUpdate();

	// Update our user interface
	g_Menu.OnFrameUpdate();

	return pafFrameworkRun_Detour->GetOriginal<int>(framework, frameTime);
}


/***
* cellPadGetDataExtraInternal() // Not sure what the real name is.
* Internal function called in sys_io_3733EA3C(), a copy of the cellPadGetDataExtra
* export only used by the Virtual Shell framework.
*/
int cellPadGetDataExtraInternal_Hook(void* r3, unsigned int portNum, int* deviceType, CellPadData* data, bool returnWithoutClearing)
{
	int returnValue = cellPadGetDataExtraInternal_Detour->GetOriginal<int>(r3, portNum, deviceType, data, returnWithoutClearing);
	
	// The game pad handling class I wrote only support one controller for now
	// so we will only process the data of the first controller.
	if (portNum == 0) 
		g_Pad.Process(data);

	return returnValue;
}


void InstallHooks()
{
	pafFrameworkRun_Detour = new Detour(((opd_s*)paf::paf_85D1D23B)->sub, pafFrameworkRun_Hook);
	cellPadGetDataExtraInternal_Detour = new Detour(ResolveBranch(((opd_s*)sys_io::sys_io_3733EA3C)->sub + 0x6C), cellPadGetDataExtraInternal_Hook);
}

void RemoveHooks()
{
	delete pafFrameworkRun_Detour;
	delete cellPadGetDataExtraInternal_Detour;
}