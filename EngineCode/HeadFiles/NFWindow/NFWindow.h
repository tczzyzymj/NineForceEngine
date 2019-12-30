#pragma once
#include "NFCommonInclude.h"

class NFWindow
{
public:
    NFWindow();


    // init for window
    bool Init(HINSTANCE hIns);


    // update per frame
    int Update();


    void Draw();
private:
};