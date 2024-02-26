
#include <cell/cell_fs.h>
#include "CCAPI.hpp"
#include "Utils/Syscalls.hpp"
#include "Utils/Memory/Common.hpp"
#include "Utils/Memory/Patterns.hpp"

bool DoesConsoleHaveCCAPI()
{
    CellFsStat sb;
    return (cellFsStat("/dev_flash/sys/internal/sys_plugin.sprx", &sb) == CELL_FS_SUCCEEDED);
}

uint8_t* CCAPIStringToArray(const char* s, uint8_t* id)
{
    uint32_t len = vsh::strlen(s);
    if (!len)
    {
        return id;
    }

    int j = 0;
    uint32_t i;
    for (i = 0; i < (len + 1); i += 2)
    {
        char b[3] = { 0,0,0 };
        vsh::strncpy(b, &s[i], 2);
        b[1] = b[1] ? b[1] : '0';
        id[j++] = vsh::strtoul(b, NULL, 16);
    }
    return id;
}

template <typename... Args>
CcxCall sys_ccapi_call(uint32_t num, Args... arg)
{
    __asm__
    (
        "sc;" // replace with ccsc = EF455314
        "blr;"
    );
}

CcxCall sys_ccapi_call_extra(uint64_t num, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8)
{
    __asm__
    (
        "mflr      %r0;"                    // 7C0802A6
        "std       %r0, 0x10(%r1);"         // F8010010
        "stdu      %r1, -0x70(%r1);"        // F821FF91
        "sc;"                               // replace with ccsc = EF455314
        "addi      %r1, %r1, 0x70;"         // 38210070
        "ld        %r0, 0x10(%r1);"         // E8010010
        "mtlr      %r0;"                    // 7C0803A6
        "blr;"                              // 4E800020
    );
}

CcxCall sys_ccapi_call_extra2(uint64_t num, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8)
{
    __asm__
    (
        "mflr       %r0;"                   // 7C0802A6
        "std        %r0, 0x10(%r1);"        // F8010010
        "stdu       %r1, -0x70(%r1);"       // F821FF91
        "mr.        %r3, %r3;"              // 7C631B79
        "beq        0x8;"                   // 41820008
        "sc;"                               // replace with ccsc = EF455314
        "addi       %r1, %r1, 0x70;"        // 38210070
        "ld         %r0, 0x10(%r1);"        // E8010010
        "mtlr       %r0;"                   // 7C0803A6
        "blr;"                              // 4E800020
    );
}

/**
* Doesn't seem to get accurate size.
* TODO: Try method 2 - scan for blr and count the bytes ???
* Or find a better hack to get function size because some functions don't have blr at the end
*/ 
uint32_t __attribute__((naked, noinline)) GetFunctionSize(uint32_t ccapiEnableSysCallFuncAddr)
{
    __asm__(
        "li %r31, 0;"				// uint32_t sizeRead = 0;
        "lis %r29, 0x6000;"			// 0x60000000
        "Loop:;"
        "lwzx %r30, %r3, %r31;"		// Add the pointer to the function to the sizeRead variable.
        "addi %r31, %r31, 4;"		// Add 4 for each opcode.
        "cmplw %cr6, %r30, %r29;"	// Compare to the nop opcode.
        "beq %cr6, Return;"			// If it equals to the nop opcode, return the amount of bytes it took to find it.
        "b Loop;"					// Branch back to start of the loop if we couldn't find it.
        "Return:;"
        "mr %r3, %r31;"				// return sizeRead;
    );
}

void ReplaceAllOccurrencesOfScBlrUsingFunctionOrder(uint32_t ccapiEnableSysCallFuncAddr)
{
    uint32_t bl_ccapi_syscall = 0;
    uint32_t funcSize = ccapiEnableSysCallFuncAddr + 0x28; // funcSize = ccapiEnableSysCallFuncAddr + GetFunctionSize(ccapiEnableSysCallFuncAddr)
    while (ccapiEnableSysCallFuncAddr < funcSize)
    {
        uint32_t instruction = *(uint32_t*)ccapiEnableSysCallFuncAddr;
        if ((instruction & (uint32_t)(63 << 26)) == ((uint32_t)(18 << 26))) // check if instruction is a branch
        {
            vsh::printf("instruction is a branch\n");
            bl_ccapi_syscall = ResolveBranch(ccapiEnableSysCallFuncAddr);
            break;
        }

        ccapiEnableSysCallFuncAddr += 4;
    }

    vsh::printf("bl_ccapi_syscall 0x%X\n", bl_ccapi_syscall);
    if (bl_ccapi_syscall)
    {
        /* 
        * @variable ccapiSyscallFuncSize
        * sys_ccapi_call only has 2 instructions so '8' bytes total **With -O2 Optimization**
        * '14' total number of sys_ccapi_call occurrences used in CCAPI.cpp
        */ 
        uint32_t allCcapiSyscallsFuncSize = bl_ccapi_syscall + (14 * 8);
        while (bl_ccapi_syscall < allCcapiSyscallsFuncSize)
        {
            uint64_t instructions = *(uint64_t*)bl_ccapi_syscall;

            if (instructions != 0x440000024E800020)
            {
                vsh::printf("we have replaced all occurrences of sys_ccapi_call\n");
                break;
            }

            /*
            * FIND
            * sc   0x44000002
            * blr  0x4E800020
            */ 
            if (instructions == 0x440000024E800020)
            {
                /*
                * REPLACE WITH
                * ccsc 0xEF455314
                * blr  0x4E800020
                */
                uint64_t replacement_syscall = 0xEF4553144E800020;
                WriteProcessMemory(sys_process_getpid(), (void*)bl_ccapi_syscall, &replacement_syscall, sizeof(replacement_syscall));
            }

            bl_ccapi_syscall += 8;
        }
    }
}

int CCAPIEnableSysCall()
{
    return sys_ccapi_call(0x241);
}

int CCAPIGetProcessList(uint32_t* npid, uint32_t* pids)
{
    return sys_ccapi_call(0x630, npid, pids);
}

int CCAPIGetProcessName(uint32_t pid, ProcessName* name)
{
    return sys_ccapi_call(0x700, pid, name);
}

int CCAPISetMemory(sys_pid_t pid, void* destination, size_t size, const void* source)
{
    return sys_ccapi_call(0x123, pid, destination, size, source);
}

int CCAPIGetMemory(sys_pid_t pid, void* destination, size_t size, void* source)
{
    return sys_ccapi_call(0x785, pid, destination, size, source);
}

int CCAPIAllocatePage(sys_pid_t pid, uint64_t size, uint64_t page_size, uint64_t flags, uint64_t is_executable, uint64_t* kernel_page_adr, uint64_t* game_page_adr)
{
    return sys_ccapi_call(0x977, pid, size, page_size, flags, is_executable, kernel_page_adr, game_page_adr);
}

int CCAPICreateProcessThread(sys_pid_t pid, thread_t* thread, void* entry, uint64_t arg, int prio, size_t stacksize, const char* threadname)
{
    return sys_ccapi_call(0x357, pid, thread, entry, arg, prio, stacksize, threadname);
}

int CCAPIDisableTmapiAndSyscall8()
{
    return sys_ccapi_call(0xCCC);
}

int CCAPIConsoleShutDown(ShutdownFlags shutdownFlags)
{
    return sys_ccapi_call(0x117, shutdownFlags);
}

int CCAPISpoofOFW()
{
    return sys_ccapi_call(0x530);
}

int CCAPISetConsoleIds(ConsoleIdType type, ConsoleId* id)
{
    return sys_ccapi_call(0x780, type, id);
}

int CCAPISetConsoleIdsString(ConsoleIdType idType, const char* id)
{
    ConsoleId cid{};
    CCAPIStringToArray(id, cid.value);
    return CCAPISetConsoleIds(idType, &cid);
}

int CCAPIUnknownSyscall_0x450()
{
    return sys_ccapi_call(0x450);
}

int CCAPIUnknownSyscall_0x750()
{
    return sys_ccapi_call(0x750);
}

int CCAPIUnknownSyscall_0x211()
{
    return sys_ccapi_call(0x211);
}

int WritePayload(uint64_t startAddr, const char* fileName) // uintptr_t startAddr
{
    constexpr int BYTES_PER_WRITE = 0x6000;
    int next_write = 0;

    int fd = 0; uint64_t read_amount = 1;

    if (cellFsOpen(fileName, CELL_FS_O_RDONLY, &fd, NULL, 0) == CELL_FS_SUCCEEDED)
    {
        while (read_amount)
        {
            char* data = (char*)vsh::malloc(BYTES_PER_WRITE);
            //char* data = (char*)sysPrxForUser::_sys_malloc(BYTES_PER_WRITE);
            if (cellFsRead(fd, (void*)(data), BYTES_PER_WRITE, &read_amount) == CELL_FS_SUCCEEDED)
            {
                CCAPISetMemory(vsh::GetGameProcessId(), reinterpret_cast<void*>((uint32_t)(startAddr + next_write)), (size_t)read_amount, data);
                next_write += read_amount;
                if (data)
                {
                    vsh::free(data);
                    //sysPrxForUser::_sys_free(data);
                }
            }
        }

        cellFsClose(fd);
    }
    else
    {
        return ENOENT;
    }

    return SUCCEEDED;
}