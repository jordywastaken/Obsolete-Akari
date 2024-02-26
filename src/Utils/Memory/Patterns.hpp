
#pragma once

#include <stdint.h>

constexpr uintptr_t gBaseAddress = 0x00010200; // first sub in the elf in ida
constexpr size_t gInfo_SizeOfImage = 0x008FFFFF;

bool DataCompare(const uint8_t* pbData, const uint8_t* pbMask, const char* szMask);

/***
* usage:
* uintptr_t pExample = FindPattern(0x20000, 0x10000, "\x3F\x80\x00\x00", "xx?x");
*/
uintptr_t FindPattern(uintptr_t address, uint32_t length, const char* bytes, const char* mask, int occurancesIndex = 0);
uintptr_t FindPatternInTextSegment(const char* bytes, const char* mask, int occurancesIndex = 0);

/***
* usage:
* uintptr_t pExample = FindSignature(0x20000, 0x10000, "3F 80 ? 00");
*/
uintptr_t FindSignature(uintptr_t address, uint32_t length, const char* sig, int occurancesIndex = 0);
uintptr_t FindSignatureInTextSegment(const char* sig, int occurancesIndex = 0);

uintptr_t ReadHighLow(uintptr_t address, uint32_t highAdditive, uint32_t lowAdditive);
uintptr_t ResolveBranch(uintptr_t branchAddress);