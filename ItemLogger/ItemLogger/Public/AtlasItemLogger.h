#pragma once

#include <API/Atlas/Atlas.h>

#ifdef ARK_EXPORTS
#define ARK_API __declspec(dllexport)
#else
#define ARK_API __declspec(dllimport)
#endif

namespace ItemLogger
{
}
