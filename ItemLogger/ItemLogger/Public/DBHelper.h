#pragma once

#ifdef ARK_EXPORTS
#define ARK_API __declspec(dllexport)
#else
#define ARK_API __declspec(dllimport)
#endif

class FString;

namespace ItemLogger::DB
{
}
