//=============================================================================
// Water.fx                                                                                                   
//
// Renders scrolling water with refraction and reflection
//=============================================================================

cbuffer cbPerFrame
{
	float4x4 g_matProj;
};

cbuffer cbPerObject
{
	float4x4 g_matWorld;
	float4x4 g_matView;
	float2   g_texTranslation;
	float	 g_rippleScale;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D g_mapNormal;
Texture2D g_mapRefractTex;
Texture2D g_mapReflectTex;

// Sampler using Linear for all filters and Wrapping address mode
SamplerState g_triLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// Standard Structure for the Vertex Shader Input
struct VS_IN
{
	float4 position : POSITION;
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD0;
};

// Standard Structure for the Vertex Shader Output and Pixel Shader Input
struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 reflectionPos : TEXCOORD1;
	float4 refractionPos : TEXCOORD2;
};

VS_OUT VS_Standard(VS_IN _inputVS)
{
	VS_OUT outputVS;

	// Homogenise the Input position vector
	_inputVS.position.w = 1.0f;

	// Calculate the Position of the vertex in world space
	outputVS.position = mul(_inputVS.position, g_matWorld);
	outputVS.position = mul(outputVS.position, g_matView);
	outputVS.position = mul(outputVS.position, g_matProj);

	// Pass the Texture Coordinates through to the Pixel Shader
	outputVS.texCoord = _inputVS.texCoord; 

	// Create the View, Projection, World Matrix for the Refraction and Reflection
	float4x4 viewProjWorld = mul(g_matView, g_matProj);
	viewProjWorld = mul(g_matWorld, viewProjWorld);

	// Calculate the Refraction and Reflection Position using the ViewProjWorld matrix
	outputVS.refractionPos = mul(_inputVS.position, viewProjWorld);
	outputVS.reflectionPos = mul(_inputVS.position, viewProjWorld);

	return outputVS;
}

float4 PS_Standard(VS_OUT _inputPS) : SV_Target
{
	// Translate the Texture coord by the current translation for the animated scrolling effect
	_inputPS.texCoord += g_texTranslation;

	// Calculate the projected ripple texture coordinates.
	float2 rippleTexCoord;
	rippleTexCoord.x = _inputPS.reflectionPos.x / _inputPS.reflectionPos.w / 2.0f + 0.5f;
	rippleTexCoord.y = -_inputPS.reflectionPos.y / _inputPS.reflectionPos.w / 2.0f + 0.5f;

	// Sample the normal from the normal map texture.
	float4 normalMap = g_mapNormal.Sample(g_triLinearSam, _inputPS.texCoord);

	// Expand the range of the normal from (0,1) to (-1,+1).
	float3 normal = (normalMap.xyz * 2.0f) - 1.0f;

	// Re-position the texture coordinate sampling position by the normal map value scaled by the rippleScale to simulate the rippling wave effect.
	rippleTexCoord = rippleTexCoord + (normal.xy * g_rippleScale);

	// Sample the texture pixels from the textures using the updated texture coordinates.
	float4 reflectionColor = g_mapReflectTex.Sample(g_triLinearSam, rippleTexCoord);
		float4 refractionColor = g_mapRefractTex.Sample(g_triLinearSam, rippleTexCoord);
	refractionColor.b += 0.1f;

	// Combine the reflection and refraction results for the final color.
	float4 color = lerp(reflectionColor, refractionColor, 0.6f);

	return color;
}

technique10 StandardTech
{
	pass P0
	{	
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Standard()));
	}
}
	
