#pragma once

#ifdef WINDOWS
#include <windows.h>
BOOLEAN nanosleep(LONGLONG ns);
#endif // WINDOWS