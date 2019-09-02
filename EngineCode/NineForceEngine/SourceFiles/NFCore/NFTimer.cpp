#include "NFCore/NFTimer.h"
#include <ctime>


bool NineForceEngine::NFTimer::Init()
{
    mNewDeltaTime = 1.0f / 60;

    long _countPerSecond;

    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_countPerSecond));

    mSecondsPerTime = 1.0 / static_cast<double>(_countPerSecond);

    return true;
}


double NineForceEngine::NFTimer::GetDeltaTime() const
{
    return mNewDeltaTime;
}


void NineForceEngine::NFTimer::BeginRecord()
{
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mBeginQueryTimeCount));
}


void NineForceEngine::NFTimer::EndRecord()
{
    __int64 _tempCount;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_tempCount));

    auto _countSpan = _tempCount - mBeginQueryTimeCount;

    mNewDeltaTime = _countSpan * mSecondsPerTime;
}
