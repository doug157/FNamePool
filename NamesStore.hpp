#pragma once

namespace UE4
{
#pragma region GlobalNames
	struct FNameEntryHandle
	{
		uint32_t Block = 0;
		uint32_t Offset = 0;
		FNameEntryHandle(uint32_t block, uint32_t offset) : Block(block), Offset(offset) {};
		FNameEntryHandle(uint32_t id) : Block(id >> 16), Offset(id & 65535) {};
		operator uint32_t() const { return (Block << 16 | Offset); }
	};

	struct FNameEntryHeader
	{
		uint16_t bIsWide : 1;
#if WITH_CASE_PRESERVING_NAME
		uint16_t Len : 15;
#else
		static constexpr uint32_t ProbeHashBits = 5;
		uint16_t LowercaseProbeHash : ProbeHashBits;
		uint16_t Len : 10;
#endif
	};

	struct FNameEntryId
	{
		uint32_t Value;
	};

	struct FNameEntry
	{
	public:
#if WITH_CASE_PRESERVING_NAME
		struct FNameEntryId ComparisonId;
#endif
		struct FNameEntryHeader Header;
		union {
			char AnsiName[1024];
			wchar_t WideName[1024];
		};
		uint16_t Size();
		std::string ToString();
		FORCEINLINE bool IsWide() const { return Header.bIsWide; }
		FORCEINLINE int32_t GetNameLength() const { return Header.Len; }
	};

	class FNameEntryAllocator
	{
	private:
		static size_t NumEntries;

	public:
		static std::unordered_map<uint32_t, std::string> Cache;

		enum { Stride = alignof(FNameEntry) };
		enum { BlockSizeBytes = Stride * 65536 };

		mutable unsigned char Lock[0x08];
		uint32_t CurrentBlock = 0;
		uint32_t CurrentByteCursor = 0;
		uint8_t* Blocks[8192] = {};

		FNameEntry* GetEntry(FNameEntryHandle Handle) const;
		void DumpBlock(uint32_t BlockId, uint32_t BlockSize) const;
		void Dump() const;
		size_t Num();
	};

	class FNamePool
	{
	public:
		enum { MaxENames = 512 };
		class FNameEntryAllocator Entries;
	};

	struct FName
	{
		static FNamePool* GNames;
		struct FNameEntryId ComparisonIndex;
#if WITH_CASE_PRESERVING_NAME
		struct FNameEntryId DisplayIndex;
#endif
		uint32_t Number;

		FORCEINLINE std::string GetName() const
		{
			return GNames->Entries.GetEntry(ComparisonIndex.Value)->ToString();
		}
	};
#pragma endregion
}
