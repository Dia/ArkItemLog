#define _CRT_SECURE_NO_WARNINGS

#include "json.hpp"

#include "Database/SqlLiteDB.h"
#include "Database/MysqlDB.h"

#include "Main.h"

#include <fstream>

#ifdef ITEMLOGGER_ARK
#include "../Public/ArkItemLogger.h"
#else
#include "../Public/AtlasItemLogger.h"
#endif

#include "Hooks.h"
#include "Helper.h"

#ifdef ITEMLOGGER_ARK
#pragma comment(lib, "ArkApi.lib")
#else
#pragma comment(lib, "AtlasApi.lib")
#endif

namespace ItemLogger
{
    nlohmann::json config;

    void ReadConfig()
    {
        const std::string config_path = GetConfigPath();
        std::ifstream file{ config_path };
        if (!file.is_open())
            throw std::runtime_error("Can't open config.json");

        file >> config;
        file.close();
    }

    void Load()
    {
        Log::Get().Init("ItemLogger");
        try
        {
            ReadConfig();
        }
        catch (const std::exception& error)
        {
            Log::GetLog()->error(error.what());
            throw;
        }

        if (config.value("Database", "sqlite") == "mysql")
        {
            database = std::make_unique<MySql>(config.value("MysqlHost", ""),
                config.value("MysqlUser", ""),
                config.value("MysqlPass", ""),
                config.value("MysqlDB", ""));
        }
        else
        {
            database = std::make_unique<SqlLite>(config.value("DbPathOverride", ""));
        }

        Hooks::Init();
    }

    void Unload()
    {
        Hooks::Unload();
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ItemLogger::Load();
        break;
    case DLL_PROCESS_DETACH:
        ItemLogger::Unload();
        break;
    }

    return TRUE;
}