#pragma once
#include <NFCore/NFSingleton.h>
#include "NFEvent/NFEventInterface.h"
#include <map>
#include <vector>


namespace NineForceEngine
{
    class NFEventManager : NFSingleton<NFEventManager>
    {
    public:
        void RegisterEvent(int messageID, void** callbackHandler);


        void UnRegisterEvent(void** callbackHandler);


        void NotifyEvent(int messagID, NFEventInterface* targetEventPtr);


    private:
        std::map<int, std::vector<void**>> mEventMap;
    };
}
