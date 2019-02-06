#pragma once

#ifdef ITEMLOGGER_ARK
#include "../Public/ArkItemLogger.h"
#else
#include "../Public/AtlasItemLogger.h"
#endif

class IDatabase
{
public:
	virtual ~IDatabase() = default;

	virtual bool SaveItemLog(uint16 action, uint64 sourceSteamId, std::string sourceName, uint64 destSteamId, std::string destName, uint32 itemId1, uint32 itemId2, std::string type, int quantity, int quality, bool isBlueprint, std::string itemname, std::string blueprintPath) = 0;
};
