cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
}

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut _vOut;

    _vOut.PosH = mul(float4(vIn.PosL, 1.0f), gWorldViewProj);
    _vOut.Color = vIn.Color;

    return _vOut;
}

float4 PS(VertexOut pIn) : SV_Target
{
    return pIn.Color;
}