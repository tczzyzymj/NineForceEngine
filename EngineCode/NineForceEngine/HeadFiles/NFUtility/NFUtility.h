#pragma once
#include "NFCore/NFInclude.h"


namespace NineForceEngine
{
    template <class T> class NFSingleton
    {
    public:
        static T* Instance() noexcept(std::is_nothrow_constructible<T>::value)
        {
            static T* _instance = new T();

            return _instance;
        }


        virtual ~NFSingleton() noexcept
        {
        }


    protected:
        explicit NFSingleton()
        {
        }


        explicit NFSingleton(const NFSingleton& other)
        {
        }


        NFSingleton& operator =(const NFSingleton& other)
        {
            return {};
        }
    };
}
