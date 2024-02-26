
#include "Patterns.hpp"
#undef vector
#include <vector>
#include <vsh/stdc.hpp>

bool DataCompare(const uint8_t* pbData, const uint8_t* pbMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pbData, ++pbMask)
		if (*szMask == 'x' && *pbData != *pbMask)
			return false;

	return (*szMask == 0);
}

uintptr_t FindPattern(uintptr_t address, uint32_t length, const char* bytes, const char* mask, int occurancesIndex)
{
	uint32_t occurancesFound = 0;
	for (uint32_t i = 0; i < length; i++)
	{
		if (DataCompare((uint8_t*)(address + i), (uint8_t*)bytes, mask))
		{
			if (occurancesFound == occurancesIndex)
				return (uintptr_t)(address + i);
			occurancesFound++;
		}
	}

	return 0;
}

uintptr_t FindPatternInTextSegment(const char* bytes, const char* mask, int occurancesIndex)
{
	return FindPattern(gBaseAddress, gInfo_SizeOfImage, bytes, mask, occurancesIndex);
}

uintptr_t FindSignature(uintptr_t address, uint32_t length, const char* sig, int occurancesIndex)
{
	static auto sigToBytes = [](const char* sig) -> std::vector<short>
	{
		auto bytes = std::vector<short>{};
		char* start = const_cast<char*>(sig);
		char* end = const_cast<char*>(sig) + vsh::strlen(sig);

		for (char* current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else
				bytes.push_back(vsh::strtoul(current, &current, 16));
		}

		return bytes;
	};

	auto bytes = sigToBytes(sig);
	uint32_t bytesCount = bytes.size();
	uint32_t occurancesFound = 0;

	for (uint32_t i = 0; i < length - bytesCount; ++i)
	{
		bool sigFound = true;
		uint8_t* data = reinterpret_cast<uint8_t*>(address + i);

		for (uint32_t j = 0; j < bytesCount; ++j)
		{
			if (data[j] != bytes[j] && bytes[j] != -1)
			{
				sigFound = false;
				break;
			}
		}

		if (sigFound)
		{
			if (occurancesFound == occurancesIndex)
				return address + i;

			occurancesFound++;
		}
	}

	return 0;
}

uintptr_t FindSignatureInTextSegment(const char* sig, int occurancesIndex)
{
	return FindSignature(gBaseAddress, gInfo_SizeOfImage, sig, occurancesIndex);
}

uintptr_t ReadHighLow(uintptr_t address, uint32_t highAdditive, uint32_t lowAdditive)
{
	uintptr_t returnAddr = (((uint16_t)(*(uint32_t*)(address + highAdditive)) << 16) | (uint16_t)(*(uint32_t*)(address + lowAdditive)));
	uintptr_t returnFinal = (returnAddr & 0x8000) ? returnAddr - 0x10000 : returnAddr;
	return returnFinal;
}

uintptr_t ResolveBranch(uintptr_t branchAddress)
{
	uint32_t instruction = *(uint32_t*)(branchAddress);
	int32_t offset = instruction & 0x3FFFFFC;

	if (offset & (1 << 25))
		offset |= ~0x03FFFFFF;

	return branchAddress + offset;
}