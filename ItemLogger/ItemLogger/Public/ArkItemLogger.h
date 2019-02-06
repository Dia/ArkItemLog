#pragma once

#include <API/Ark/Ark.h>

#ifdef ARK_EXPORTS
#define ARK_API __declspec(dllexport)
#else
#define ARK_API __declspec(dllimport)
#endif

namespace ItemLogger
{
}
