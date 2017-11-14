cbuffer ConstantBuffer
{
    float4x4 wvp;
};

struct Input
{
    float4 position : POSITION;
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

    output.position = mul(input.position, transpose(wvp));
    output.color = input.color;

    return output;
}
