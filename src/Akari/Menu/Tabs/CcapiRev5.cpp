
#include "../Base.hpp"
#include "Utils/CCAPI.hpp"

uint64_t g_PageTableKernel = 0;
uint64_t g_PageTableGame = 0;

#define MINI_LOG(message) \
vsh::ShowNavigationMessage(L##message); \
vsh::printf(message) 

void TabCcapiRev5()
{
	g_Menu.Submenu(L"CCAPI REV5", []
	{
		g_Menu.Option(L"Step 1 - Replace syscall").Action([]
		{
			ReplaceAllOccurrencesOfScBlrUsingFunctionOrder(*(uint32_t*)CCAPIEnableSysCall);	
		});

		g_Menu.Option(L"Step 2 - Enable CCAPI syscalls").Action([]
		{
			if (DoesConsoleHaveCCAPI())
				CCAPIEnableSysCall();
		});

		g_Menu.Option(L"Step 3 - Verify CCAPI syscalls are working").Action([]
		{
			if (DoesConsoleHaveCCAPI())
			{
				char readBytes[4];
				int ret = CCAPIGetMemory(sys_process_getpid(), (void*)0x10000, 0x4, &readBytes);
				if (ret == SUCCEEDED)
				{
					MINI_LOG("CCAPI syscalls are working\n");
				}
				else
				{
					MINI_LOG("you most likely crashed to illegal instruction\n");
				}
			}
		});

		g_Menu.Option(L"Step 4 - Allocate Page").Action([]
		{
			if (DoesConsoleHaveCCAPI() && vsh::GetCooperationMode() == vsh::CooperationMode::Game)
			{
				// Destiny
				//int ret = CCAPIAllocatePage(vsh::GetGameProcessId(), 0x9D000, 0x100, 0x2F, 0x1, &g_PageTableKernel, &g_PageTableGame);

				// Fury
				//int ret = CCAPIAllocatePage(vsh::GetGameProcessId(), 0x7C000, 0x100, 0x2F, 0x1, &g_PageTableKernel, &g_PageTableGame);

				// Fatality
				//int ret = CCAPIAllocatePage(vsh::GetGameProcessId(), 0x6F000, 0x100, 0x2F, 0x1, &g_PageTableKernel, &g_PageTableGame);

				// Reborn 
				//int ret = CCAPIAllocatePage(vsh::GetGameProcessId(), 0x23000, 0x100, 0x2F, 0x1, &g_PageTableKernel, &g_PageTableGame);

				// Fusion
				int ret = CCAPIAllocatePage(vsh::GetGameProcessId(), 0x27000, 0x100, 0x2F, 0x1, &g_PageTableKernel, &g_PageTableGame);

				vsh::printf("CCAPIAllocatePage returned = 0x%X\n", ret);
				vsh::printf("pageTableKernel 0x%016llX\n", g_PageTableKernel);
				vsh::printf("pageTableGame 0x%016llX\n", g_PageTableGame);
			}
			else
			{
				MINI_LOG("You are not in game\n");
			}
		});

		g_Menu.Option(L"Step 5 - Inject Payload To Page").Action([]
		{
			if (DoesConsoleHaveCCAPI() && vsh::GetCooperationMode() == vsh::CooperationMode::Game)
			{
				// Destiny
				//const char* fileName = "/dev_hdd0/plugins/RouLetteVshMenu/modmenus/enstone/destiny_by_enstone_120_patched.bin";

				// Fury
				//const char* fileName = "/dev_hdd0/plugins/RouLetteVshMenu/modmenus/enstone/fury_by_enstone_220_patched.bin";
				
				// Fatality
				//const char* fileName = "/dev_hdd0/plugins/RouLetteVshMenu/modmenus/enstone/fatality_by_enstone_102_patched.bin";

				// Reborn 
				//const char* fileName = "/dev_hdd0/plugins/RouLetteVshMenu/modmenus/enstone/reborn_by_enstone_446.bin";

				// Fusion
				const char* fileName = "/dev_hdd0/plugins/RouLetteVshMenu/modmenus/enstone/fusion_by_enstone_114.bin";

				int ret = WritePayload(g_PageTableGame, fileName);
				if (ret == SUCCEEDED)
				{
					MINI_LOG("successfully injected payload\n");
				}
				else
					vsh::printf("file %s NOT FOUND\n", fileName);
			}
			else
			{
				MINI_LOG("You are not in game\n");
			}
		});

		g_Menu.Option(L"Step 6 - Start Payload Thread").Action([]
		{
			if (DoesConsoleHaveCCAPI() && vsh::GetCooperationMode() == vsh::CooperationMode::Game)
			{
				uint32_t threadOpd[2]{};
				threadOpd[0] = g_PageTableGame;
				threadOpd[1] = 0x00000000; // does it need a proper TOC ???

				// is stack size and priority the same for all menus ???
				thread_t th;
				int ret = CCAPICreateProcessThread(vsh::GetGameProcessId(), &th, threadOpd, 0, 0x7D0, 0x4000, "ccapi_start_payload_thread");
				vsh::printf("CCAPICreateProcessThread returned = 0x%X\n", ret);
			}
			else
			{
				MINI_LOG("You are not in game\n");
			}
		});
	});
}