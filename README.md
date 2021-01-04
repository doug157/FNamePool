# FNamePool
#### Work in progress
FNamePool implementation for dumping UE4 SDK

Credit goes to guttir14 for his external implementation https://github.com/guttir14/UnrealDumper-4.25
https://www.unknowncheats.me/forum/unreal-engine-4-a/428680-unrealdumper-4-25-a.html#post3027279


```cpp
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\x74\x09\x48\x8D\x15\x00\x00\x00\x00\xEB\x16"), "xxxxx????xx");
	auto offset = *reinterpret_cast<uint32_t*>(address + 5);

	if (address == -1)
		return false;

	UE4::FName::GNames = reinterpret_cast<UE4::FNamePool*>(address + 9 + offset);

	for (auto entry : UE4::FName::GNames->Entries.Cache)
	{
		printf("[%-7d] %s\n", entry.first, entry.second.c_str());
	}

	printf("[%p] [%-6zu] GlobalNames\n", UE4::FName::GNames, UE4::FName::GNames->Entries.Num());
  ```
