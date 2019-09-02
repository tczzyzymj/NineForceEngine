#pragma once
#include "NFCore/NFInclude.h"


namespace NineForceEngine
{
#ifdef UNICODE
    #define NFString std::wstring
#else
    #define NFString std::string
#endif

    class NFUtility
    {
    public:
        static void ReleaseCOM(IUnknown** targetPtr);

        static void ShowMessageBox(const char * targetContent);
    };
}
