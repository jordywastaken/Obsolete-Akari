#pragma once
#include <stdint.h>
#include <sys/process.h>
#include <vshlib.hpp>

#define CcxCall uint64_t __attribute__((naked, noinline))

enum ConsoleIdType
{
    Idps = 0,
    Psid = 1,
};

struct ConsoleId
{
    uint8_t value[16];
};

struct ProcessName
{
    char value[512];
};

enum class ShutdownFlags
{
    shutdown = 0x100
    // to be continued...
};

typedef enum ConsoleIdType ConsoleIdType;
typedef struct ConsoleId ConsoleId;

typedef struct
{
    void* unk_0; // ptr to some funcs
    uint64_t unk_8;
    uint32_t unk_10;
    uint32_t unk_14;
    void* unk_18;
    void* unk_20; // same as unk_18? :S
    uint64_t unk_28[3];
    void* unk_40; // same as unk_0?
    // ...
} thread_t;

bool DoesConsoleHaveCCAPI();
uint8_t* CCAPIStringToArray(const char* s, uint8_t* id);
CcxCall sys_ccapi_call_extra(uint64_t num, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8);
CcxCall sys_ccapi_call_extra2(uint64_t num, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8);
void ReplaceAllOccurrencesOfScBlrUsingFunctionOrder(uint32_t ccapiEnableSysCallFuncAddr);
int CCAPIEnableSysCall();
int CCAPIGetProcessList(uint32_t* npid, uint32_t* pids);
int CCAPIGetProcessName(uint32_t pid, ProcessName* name);
int CCAPISetMemory(sys_pid_t pid, void* destination, size_t size, const void* source);
/*
* @Returns
* 0x80000100 if buffer is invalid or failed to write?
* 0x80001000 failed process doesnt not exist
* 0x80000600 invalid read
* 0x80001000 if process isn't acive
* 0 if sucessful
*/
int CCAPIGetMemory(sys_pid_t pid, void* destination, size_t size, void* source);
int CCAPIAllocatePage(sys_pid_t pid, uint64_t size, uint64_t page_size, uint64_t flags, uint64_t is_executable, uint64_t* kernel_page_adr, uint64_t* game_page_adr);
int CCAPICreateProcessThread(sys_pid_t pid, thread_t* thread, void* entry, uint64_t arg, int prio, size_t stacksize, const char* threadname);
int CCAPIDisableTmapiAndSyscall8();
int CCAPIConsoleShutDown(ShutdownFlags shutdownFlags);
int CCAPISpoofOFW();
int CCAPISetConsoleIds(ConsoleIdType type, ConsoleId* id);
int CCAPISetConsoleIdsString(ConsoleIdType idType, const char* id);
int CCAPIUnknownSyscall_0x450();
int CCAPIUnknownSyscall_0x750();
int CCAPIUnknownSyscall_0x211();
int WritePayload(uint64_t startAddr, const char* fileName);