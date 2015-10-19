//=============================================================================
// Sprite.fx                                                                                                   
// Author: Juran Griffith.
// Handles all sprite rendering.
//=============================================================================

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix World			: WORLD;
matrix View				: VIEW;
matrix Projection		: PROJECTION;

float DeltaTime			: TIME;

Texture2D Texture;

//--------------------------------------------------------------------------------------
// Sample States 
//--------------------------------------------------------------------------------------
SamplerState SampleType // The sampler state allows us to modify how the pixels are written to the polygon face when shaded.
{
	Filter = MIN_MAG_MIP_LINEAR; // Filter will determine how it decides which pixels will be used or combined to create the final look of the texture on the polygon face
	AddressU = Wrap;
	AddressV = Wrap;
};

//--------------------------------------------------------------------------------------
// Structs
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texC : TEXCOORD0;
};

// For transparency values
BlendState SrcAlphaBlendingAdd
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS_Default(float3 position : POSITION, float2 texC : TEXCOORD0)
{
	PS_INPUT output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(float4(position, 1.0f), World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);

	output.texC = texC;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_Default(PS_INPUT _input) : SV_Target
{
	float4 textureColor;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = Texture.Sample(SampleType, _input.texC);

	return textureColor;
}

//--------------------------------------------------------------------------------------
// Render Technique
//--------------------------------------------------------------------------------------
technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Default()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Default()));

		// For transparency values
		SetBlendState(SrcAlphaBlendingAdd, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}