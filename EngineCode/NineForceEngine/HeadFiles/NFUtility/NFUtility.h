#pragma once
#include "NFCore/NFInclude.h"


namespace NineForceEngine
{
    template <class T> class NFSingleton
    {
    public:
        static T* Ins()
        {
            return mIns;
        }


    private:
        NFSingleton()
        {
        }


        NFSingleton& operator =(const NFSingleton& other)
        {
            return other;
        }


        NFSingleton(const NFSingleton& other)
        {
        }


        static T* mIns;
    };


    template <class T> T* NFSingleton<T>::mIns = new T;
}
