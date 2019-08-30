#pragma once
#include "NFUtility/NFUtility.h"
#include "NFSingleton.h"


namespace NineForceEngine
{
    class NFTimer : public NFSingleton<NFTimer>
    {
    public:
        bool Init();


        float GetDeltaTime() const;


        void BeginRecord();


        void EndRecord();


    private:

        friend class NFSingleton<NFTimer>;


        NFTimer() = default;


        float mDeltaTime = 0.0f;


        long mBeginRecordTime = 0;


        double mSecondsPerTime = 0.0;
    };
}
