#include "NFUtility/NFUtility.h"


void NineForceEngine::NFUtility::ReleaseCOM(IUnknown** targetPtr)
{
    if (targetPtr == nullptr || (*targetPtr) == nullptr)
    {
        return;
    }

    (*targetPtr)->Release();

    (*targetPtr) = nullptr;
}
