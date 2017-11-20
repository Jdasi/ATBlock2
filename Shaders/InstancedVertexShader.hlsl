cbuffer ConstantBuffer
{
    float4x4 obj_world;
    float4x4 obj_rot;
    float4 obj_color;

    float4x4 view;
    float4x4 proj;
};

struct Input
{
    float4 position : POSITION;
    float3 instance_pos : INSTANCEPOS;
    float4 instance_col : INSTANCECOLOR;
    uint instance_id : SV_InstanceID;
};

struct Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

Output main(Input input)
{
    Output output;

    input.position += float4(input.instance_pos, 0.0f);

    output.position = mul(input.position, obj_world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);

    output.color = input.instance_col;

    return output;
}
