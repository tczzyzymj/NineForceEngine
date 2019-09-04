#include "NFManager/NFEventManager.h"
#include "NFManager/NFLogManager.h"


void NineForceEngine::NFEventManager::RegisterEvent(int messageID, void** callbackHandler)
{
    auto _findIT = mEventMap.find(messageID);

    if (_findIT == mEventMap.end())
    {
        std::list<void**> _newValue;

        _newValue.push_back(callbackHandler);

        mEventMap.insert(std::pair<int, std::list<void**>>(messageID, _newValue));
    }
    else
    {
        // check if it's same call back address
        auto _targetVector = _findIT->second;
        for (auto _value : _targetVector)
        {
            if (_value == callbackHandler)
            {
                // show error here
                NFLogManager::Instance()->LogError(L"Error! Same event callback already exist!");

                return;
            }
        }

        _targetVector.push_back(callbackHandler);
    }
}


void NineForceEngine::NFEventManager::UnRegisterEvent(int messageID, void** callbackHandler)
{
    auto _findIT = mEventMap.find(messageID);

    if (_findIT == mEventMap.end())
    {
        return;
    }

    auto _targetVector = _findIT->second;

    _targetVector.remove(callbackHandler);
}


void NineForceEngine::NFEventManager::NotifyEvent(const int messagID, NFEventInterface* targetEventPtr)
{
}
