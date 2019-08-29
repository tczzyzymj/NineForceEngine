#include "NFCore/NFTimer.h"
#include <ctime>


bool NineForceEngine::NFTimer::Init()
{
    mDeltaTime = 0.0167f; // when init, assume it's 60 fps;

    return true;
}


float NineForceEngine::NFTimer::GetDeltaTime() const
{
    return mDeltaTime;
}


void NineForceEngine::NFTimer::BeginRecord()
{
    mBeginRecordTime = clock();
}


void NineForceEngine::NFTimer::EndRecord()
{
    const auto _currentTime = clock();

    mDeltaTime = static_cast<float>(_currentTime - mBeginRecordTime);
}
