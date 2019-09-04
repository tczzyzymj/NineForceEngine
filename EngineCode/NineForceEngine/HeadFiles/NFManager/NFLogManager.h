#pragma once
#include <NFCore/NFSingleton.h>


namespace NineForceEngine
{
    class NFLogManager : public NFSingleton<NFLogManager>
    {
    public:
        void LogError(const wchar_t *targetMsg);


        void LogWarning(const wchar_t *targetMsg);


        void LogTag(const wchar_t *targetMsg);


    private:
    };
}
