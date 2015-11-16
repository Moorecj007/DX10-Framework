////////////////////////////////////////////////////////////////////////////////
// Filename: softshadow.fx
////////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "LightStructures.fx"

// Defines
#define MAX_LIGHTS 2

//////////////
// MATRICES //
//////////////
float4x4 worldMatrix;
float4x4 viewMatrix;
float4x4 projectionMatrix;


//////////////
// TEXTURES //
//////////////
Texture2D shaderTexture;
Texture2D shadowTexture1;
Texture2D shadowTexture2;


/////////////
// GLOBALS //
/////////////
Light g_lights[MAX_LIGHTS];
int g_lightCount;
float4 ambientColor;
float4 diffuseColor;


///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypeClamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState SampleTypeWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float3 position : POSITION;	
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 viewPosition : TEXCOORD1;
	float3 lightPos[MAX_LIGHTS] : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType SoftShadowVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the position of the vertice as viewed by the camera in a separate variable.
	output.viewPosition = output.position;

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
	worldPosition = mul(float4(input.position, 1.0f), worldMatrix);

	// Determine the light position based on the position of the light and the position of the vertex in the world.
	[unroll]
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		output.lightPos[i] = 0;
		if (i < g_lightCount)
		{
			output.lightPos[i] = g_lights[i].pos_range.xyz - worldPosition.xyz;
			output.lightPos[i] = normalize(output.lightPos[i]);
		}
	}

	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SoftShadowPixelShader(PixelInputType input) : SV_Target
{
	float4 color = 0;
	float lightIntensity;
	float4 textureColor;
	float2 projectTexCoord;
	float shadowValue[MAX_LIGHTS];
	float averageShadow = 0;

	[unroll]
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		shadowValue[i] = 0;

		if (i < g_lightCount)
		{
			// Set the default output color to the ambient light value for all pixels.
			color = g_lights[i].ambient;

			// Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, input.lightPos[i]));
			if (lightIntensity > 0.0f)
			{
				// Determine the light color based on the diffuse color and the amount of light intensity.
				color += (g_lights[i].diffuse * lightIntensity);

				// Saturate the light color.
				color = saturate(color);
			}

			// Sample the pixel color from the texture using the sampler at this texture coordinate location.
			textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

			// Combine the light and texture color.
			color = color * textureColor;

			// Calculate the projected texture coordinates to be used with the shadow texture.
			projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
			projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;

			// Sample the shadow value from the shadow texture using the sampler at the projected texture coordinate location.
			if (i == 0)
			{
				shadowValue[i] = shadowTexture1.Sample(SampleTypeClamp, projectTexCoord).r;
			}
			else
			{
				shadowValue[i] = shadowTexture2.Sample(SampleTypeClamp, projectTexCoord).r;
			}
		}
		averageShadow = averageShadow + shadowValue[i];
	}
	averageShadow = averageShadow / g_lightCount;

	// Combine the shadows with the final color.
	color = color * averageShadow;

	return color;
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 SoftShadowTech
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, SoftShadowVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, SoftShadowPixelShader()));
		SetGeometryShader(NULL);
	}
}