cbuffer MatBuff : register(b0)
{
	row_major float4x4 fMatrix;
};

cbuffer ShaderParam : register(b1)
{
    int4 g_int;
    float4 g_float;
    float4 g_mtrl;
};

cbuffer AnimParam : register(b2)
{
    float2 g_fLT;
    float2 g_fSize;
    int   g_bUse;
    float2 g_fOffset;
    int   g_fPadding;
};

struct VS_INPUT
{
	float3 vPos : POSITION;
	float4 vColor : COLOR;
	float2 vTex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
	float4 vOutColor : COLOR;
	float2 vOutTex : TEXCOORD;
};

struct GeoOut
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
    float2 vOutTex : TEXCOORD;
};

Texture2D g_Tex_0 : register(t0);
Texture2D g_Tex_1 : register(t1);
Texture2D g_Tex_2 : register(t2);
Texture2D g_Tex_3 : register(t3);
Texture2D g_Tex_4 : register(t4);
Texture2D g_Tex_5 : register(t5);
Texture2D g_Tex_6 : register(t6);
Texture2D g_Tex_7 : register(t7);

Texture2D g_Anim_Tex : register(t15);

RWTexture2D<float4> grwTex_0 : register(u0);

SamplerState g_Sample : register(s0);

groupshared float4 gChache[266];

VS_OUTPUT VS_Color(VS_INPUT _Input)
{
	VS_OUTPUT output = (VS_OUTPUT)0.f;
	
	output.vOutPos = mul(float4(_Input.vPos,1.f), fMatrix);
	output.vOutColor = _Input.vColor;

	return output;
}

float4 PS_Color(GeoOut _Input) : SV_Target
{
    return _Input.vOutColor;
}

// Èæ¹é if (g_int[0])
    //{
    //    float fColor = float(texColor.r + texColor.g + texColor.b) / 3.f;
    //    texColor.xyz = float3(fColor,fColor,fColor);
    //}

VS_OUTPUT VS_Std_2D(VS_INPUT _Input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 1.f), fMatrix);
    output.vOutPos.x += g_fOffset.x;
    output.vOutPos.y += g_fOffset.y;
    output.vOutTex = _Input.vTex;

    return output;
}

float4 PS_Std_2D(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4)0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float.x * _Input.vOutTex.x);
        fUV.y = (g_float.y * _Input.vOutTex.y);

        if (fUV.x >= 1.f)
            fUV.x = 1.f;

        if (fUV.y >= 1.f)
            fUV.y = 1.f;
            texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor *= g_mtrl;
    return texColor;
}

float4 PS_Std_Spectrum(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4) 0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float.x * _Input.vOutTex.x);
        fUV.y = (g_float.y * _Input.vOutTex.y);

        texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor.a = 0.8f;

    return texColor;
}

float4 PS_Std_Hit(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4) 0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float.x * _Input.vOutTex.x);
        fUV.y = (g_float.y * _Input.vOutTex.y);

        texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor.r = texColor.r + 0.4f;

    return texColor;
}

float4 PS_Std_Green(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4) 0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float.x * _Input.vOutTex.x);
        fUV.y = (g_float.y * _Input.vOutTex.y);

        texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor.r = 0.5f;
    texColor.b = 0.9f;

    return texColor;
}
float4 PS_Std_Color(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4) 0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float.x * _Input.vOutTex.x);
        fUV.y = (g_float.y * _Input.vOutTex.y);

        texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor = g_mtrl;

    return texColor;
}

VS_OUTPUT VS_Coll(VS_INPUT _Input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 1.f), fMatrix);
    output.vOutColor = _Input.vColor;

    return output;
}

float4 PS_Coll(VS_OUTPUT _Input) : SV_Target
{
    if (g_int[0])
        _Input.vOutColor = float4(1,0,0,0);
    else
        _Input.vOutColor = float4(0, 0, 1, 0);

    return _Input.vOutColor;
}

[maxvertexcount(3)]
void GS_Color(triangle VS_OUTPUT _Input[3],inout TriangleStream<GeoOut> Re)
{
    GeoOut geo[3];

    //for (int i = 0; i < 3; ++i)
    //{
    //    geo[i].vOutPos = _Input[0].vOutPos;
    //    geo[i].vOutColor = _Input[0].vOutColor;
    //    geo[i].vOutTex = _Input[0].vOutTex;
    //}

    for (int i = 0; i < 3; ++i)
    {
        geo[i].vOutPos = float4(_Input[i].vOutPos.x, _Input[i].vOutPos.y, _Input[i].vOutPos.z, _Input[i].vOutPos.w);
        geo[i].vOutColor = _Input[i].vOutColor;
        geo[i].vOutTex = _Input[i].vOutTex;
        Re.Append(geo[i]);
    }

    Re.RestartStrip();

    //Re.Append(geo[1]);
    //Re.Append(geo[2]);
       
    //Re.RestartStrip();
}

[numthreads(8,0,0)]
void CS_HorizenBlur(int3 dispatchThreadID : SV_DispatchThreadID)
{
}