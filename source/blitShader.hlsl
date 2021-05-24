struct vs_in { 
    float2 position_local : POSITION;
    float2 uv : TEXCOORD0;
}; 

struct vs_out { 
    float4 position_clip : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D backBufferTexture : register(t0);
SamplerState backBufferSampler : register(s0);

vs_out vs_main(vs_in input) { 
    vs_out output; 
    output.position_clip = float4(input.position_local, 1.0, 1.0);
    output.uv = input.uv;
    return output; 
}

float4 ps_main(vs_out input) : SV_TARGET { 
    return backBufferTexture.Sample(backBufferSampler, input.uv);
}