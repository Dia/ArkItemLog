#pragma once

#include <SQLiteCpp/Database.h>

#include "IDatabase.h"
#include "../Main.h"

class SqlLite : public IDatabase
{
public:
	explicit SqlLite(const std::string& path)
		: db_(path.empty()
			      ? ItemLogger::GetDbPath()
			      : path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
	{
		try
		{
            db_.exec("PRAGMA journal_mode=WAL;");

            db_.exec("create table if not exists ItemLog ("
                "Id integer primary key autoincrement not null,"
                "SteamId integer default 0,"
                ");");
		}
		catch (const std::exception& exception)
		{
			Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
		}
	}

    bool SaveItemLog(uint16 action, uint64 sourceSteamId, std::string sourceName, uint64 destSteamId, std::string destName, uint32 itemId1, uint32 itemId2, std::string type, int quantity, int quality, bool isBlueprint, std::string itemname, std::string blueprintPath) override
    {
        SQLite::Statement query(db_, "INSERT INTO ItemLog (SteamId) VALUES (?);");
        query.bind(1, static_cast<int64>(sourceSteamId));
        query.exec();
        return true;
    }

private:
	SQLite::Database db_;
};
