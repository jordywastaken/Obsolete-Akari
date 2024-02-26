
#include <vshlib.hpp>
#include <vsh/newDelete.hpp>

#include "Utils/Syscalls.hpp"
#include "Utils/Threads.hpp"

#include "Akari.hpp"


#pragma diag_suppress 77
SYS_MODULE_INFO(Akari, 0, 1, 0);
SYS_MODULE_START(module_start);
SYS_MODULE_STOP(module_stop);

Thread gModuleStartThread;

extern "C"
{
	int __cdecl module_start(size_t args, const void *argp)
	{
		gModuleStartThread = Thread(Akari::Initialize, &gModuleStartThread);

		ExitModuleThread();
		return 0;
	}

	int __cdecl module_stop(size_t args, const void *argp)
	{
		Thread moduleStopThread = Thread([]
		{
			Akari::Shutdown();

			// Prevents unloading too fast
			sys_ppu_thread_yield();
			Timer::Sleep(1000);

			gModuleStartThread.Join();

		}, &moduleStopThread);

		moduleStopThread.Join();

		Timer::Sleep(5);

		UnloadMyModule();
		ExitModuleThread();
		return 0;
	}
}