#pragma once
#include "NFUtility/NFUtility.h"


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
    };
}
