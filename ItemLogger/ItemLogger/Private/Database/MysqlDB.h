#pragma once

#include <mysql++11.h>

#include "IDatabase.h"

#pragma comment(lib, "mysqlclient.lib")

class MySql : public IDatabase
{
public:
    explicit MySql(std::string server, std::string username, std::string password, std::string db_name)
    {
        try
        {
            daotk::mysql::connect_options options;
            options.server = move(server);
            options.username = move(username);
            options.password = move(password);
            options.dbname = move(db_name);
            options.autoreconnect = true;
            options.timeout = 30;

            bool result = db_.open(options);
            if (!result)
            {
                Log::GetLog()->critical("Failed to open connection!");
                return;
            }

            result = db_.query(fmt::format("CREATE TABLE IF NOT EXISTS ItemLog ("
                "id INT NOT NULL AUTO_INCREMENT,"
                "action SMALLINT UNSIGNED NOT NULL,"
                "sourceSteamId BIGINT(11) UNSIGNED NULL,"
                "sourceName VARCHAR(100) NULL,"
                "destSteamId BIGINT(11) UNSIGNED NOT NULL,"
                "destName VARCHAR(100) NOT NULL,"
                "itemId1 INT UNSIGNED NOT NULL,"
                "itemId2 INT UNSIGNED NOT NULL,"
                "type VARCHAR(100) NOT NULL,"
                "quantity INT UNSIGNED NOT NULL,"
                "quality INT UNSIGNED NOT NULL,"
                "isBlueprint BIT NOT NULL,"
                "itemName VARCHAR(100) NOT NULL,"
                "blueprintPath VARCHAR(250) NOT NULL,"
                "PRIMARY KEY(id),"
                "INDEX sourceSteamId (sourceSteamId ASC),"
                "INDEX destSteamId (destSteamId ASC));"));

            if (!result)
            {
                Log::GetLog()->critical("({} {}) Failed to create table!", __FILE__, __FUNCTION__);
            }
        }
        catch (const std::exception& exception)
        {
            Log::GetLog()->critical("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
        }
    }

    bool SaveItemLog(uint16 action, uint64 sourceSteamId, std::string sourceName, uint64 destSteamId, std::string destName, uint32 itemId1, uint32 itemId2, std::string type, int quantity, int quality, bool isBlueprint, std::string itemName, std::string blueprintPath) override
    {
        if (!sourceSteamId)
        {
            sourceSteamId = 0;
        }

        if (!destSteamId)
        {
            destSteamId = 0;
        }

        bool result = db_.query(fmt::format("INSERT INTO ItemLog (action, sourceSteamId, sourceName, destSteamId, destName, itemId1, itemId2, type, quantity, quality, isBlueprint, itemName, blueprintPath) VALUES ({}, {}, '{}', {}, '{}', {}, {}, '{}', {}, {}, {}, '{}', '{}');", action, sourceSteamId, sourceName, destSteamId, destName, itemId1, itemId2, type, quantity, quality, isBlueprint, itemName, blueprintPath));
        if (!result)
        {
            Log::GetLog()->critical("({} {}) Failed to insert item log table! {}", __FILE__, __FUNCTION__, db_.error_message());
        }

        return result;
    }

private:
    daotk::mysql::connection db_;
};
