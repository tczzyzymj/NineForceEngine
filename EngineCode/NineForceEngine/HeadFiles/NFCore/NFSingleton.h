#pragma once


namespace NineForceEngine
{
    template <class T> class NFSingleton
    {
    public:
        static T* Instance()
        {
            static T* _instance = new T();

            return _instance;
        }


        virtual ~NFSingleton()
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
