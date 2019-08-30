#pragma once
#include "NFCore/NFInclude.h"


namespace NineForceEngine
{
    class NFUtility
    {
    public:
        static void ReleaseCOM(IUnknown** targetPtr);
    };
}
