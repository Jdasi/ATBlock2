cbuffer ConstantBuffer
{
    float4x4 wvp;
};

struct Input
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

Output main(Input input)
{
    Output output;

    float4 pos = float4(input.position.x, input.position.y, input.position.z, 1);
    output.position = pos;
    //output.position = mul(pos, wvp);
    output.color = input.color;

    return output;
}
