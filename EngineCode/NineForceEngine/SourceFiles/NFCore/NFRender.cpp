#include "NFCore/NFRender.h"


NineForceEngine::NFRender::NFRender()
{
}


NineForceEngine::NFRender::~NFRender()
{
}


bool NineForceEngine::NFRender::Init()
{
	D3D_FEATURE_LEVEL _featureLevel;

    const auto _result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &mDevice,
        &_featureLevel,
        &mContext
    );

    if (FAILED(_result))
    {
		MessageBox(nullptr, L"Create D3D device failed!", L"Warning", 0);
        
        return false;
    }

    if(_featureLevel != D3D_FEATURE_LEVEL_11_0)
    {
		MessageBox(nullptr, L"D3D11 not supported!", L"Error", 0);

		return false;
    }

    return true;
}


void NineForceEngine::NFRender::Update(float deltaTime)
{
}


void NineForceEngine::NFRender::Clean()
{
}


void NineForceEngine::NFRender::Render()
{
}
