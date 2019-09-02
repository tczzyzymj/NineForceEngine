#pragma once
#include "NFUtility/NFUtility.h"
#include "NFSingleton.h"


namespace NineForceEngine
{
    class NFTimer : public NFSingleton<NFTimer>
    {
    public:
        bool Init();


        double GetDeltaTime() const;


        void BeginRecord();


        void EndRecord();


    private:

        friend class NFSingleton<NFTimer>;


        NFTimer() = default;


        double mNewDeltaTime = 0;


        double mSecondsPerTime = 0.0;


        _int64 mBeginQueryTimeCount = 0;
    };
}
