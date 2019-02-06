#pragma once

namespace ItemLogger
{
    inline std::string GetDbPath()
    {
#ifdef ITEMLOGGER_ARK
        return ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/ItemLogger/ArkDB.db";
#else
        return ArkApi::Tools::GetCurrentDir() + "/AtlasApi/Plugins/ItemLogger/AtlasDB.db";
#endif
    }

    inline std::string GetConfigPath()
    {
#ifdef ITEMLOGGER_ARK
        return ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/ItemLogger/config.json";
#else
        return ArkApi::Tools::GetCurrentDir() + "/AtlasApi/Plugins/ItemLogger/config.json";
#endif
    }
}
