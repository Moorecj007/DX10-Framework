//=============================================================================
// LitTex.fx                                                                                                   
//
// Transforms, lights, and textures geometry.
//=============================================================================

// Local Includes
#include "LightStructures.fx"

// Defines
#define MAX_LIGHTS 5

cbuffer cbPerFrame
{
	float3 g_eyePosW;
	float4x4 g_matView;
	float4x4 g_matProj;
};

cbuffer cbPerObject
{
	// Standard
	Light g_light[MAX_LIGHTS];
	int g_lightCount;
	float4x4 g_matWorld;
	float4x4 g_matTex;

	// Fade + BlendTex
	float	 g_reduceAlpha;

	// Refraction and Reflection
	float4 g_plane;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D g_mapDiffuse;
Texture2D g_mapDiffuse2;
Texture2D g_mapSpec;

SamplerState g_triLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

struct VS_OUT
{
	float4 positionH    : SV_POSITION;
	float3 position     : POSITION;
	float3 normal		: NORMAL;
    float2 texCoord     : TEXCOORD;
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
 
VS_OUT VS_Standard(VS_IN _inputVS)
{
	VS_OUT outputVS;
	
	// Transform to world space space.
	outputVS.position = mul(float4(_inputVS.position, 1.0f), g_matWorld).xyz;
	outputVS.normal = mul(float4(_inputVS.normal, 0.0f), g_matWorld).xyz;
		
	// Transform to homogeneous clip space.
	outputVS.positionH = mul(float4(_inputVS.position, 1.0f), g_matWorld);
	outputVS.positionH = mul(outputVS.positionH, g_matView);
	outputVS.positionH = mul(outputVS.positionH, g_matProj);
	
	// Output vertex attributes for interpolation across triangle.
	outputVS.texCoord = mul(float4(_inputVS.texCoord, 0.0f, 1.0f), g_matTex).xy;
	
	return outputVS;
}

float4 PS_Standard(VS_OUT _inputPS) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);
	
	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;
	
	// Interpolating normal can make it not be of unit length so normalize it.
	float3 normal = normalize(_inputPS.normal);
    
	// Compute the lit color for this pixel.
	SurfaceInfo surface = { _inputPS.position, normal, diffuse, spec };

	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < g_lightCount; i++)
	{
		if (g_light[i].active == true)
		{
			if (g_light[i].type == 0)
			{
				litColor += ParallelLight(surface, g_light[i], g_eyePosW);
			}
			else if (g_light[i].type == 1)
			{
				litColor += PointLight(surface, g_light[i], g_eyePosW);
			}
			else if (g_light[i].type == 2)
			{
				litColor += SpotLight(surface, g_light[i], g_eyePosW);
			}
			else if (g_light[i].type == 3)
			{
				float glowLerp = GlowLight(surface, g_light[i]);

				if (glowLerp > 0.0f)
				{
					litColor = lerp(litColor, g_light[i].diffuse.xyz, glowLerp);
				}
			}
		}
	}

	return float4(litColor, diffuse.a);
}

technique10 StandardTech
{
    pass P0
    {
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
       	SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS_Standard()));
    }
}

//--------------------------------------------------------------
// Fade
//--------------------------------------------------------------

float4 PS_Fade(VS_OUT _inputPS) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);

	diffuse.a = diffuse.a - g_reduceAlpha;

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Interpolating normal can make it not be of unit length so normalize it.
	float3 normal = normalize(_inputPS.normal);

	// Compute the lit color for this pixel.
	SurfaceInfo surface = { _inputPS.position, normal, diffuse, spec };
	float3 litColor = ParallelLight(surface, g_light[0], g_eyePosW);

	return float4(litColor, diffuse.a);
}

technique10 FadeTech
{
	pass P0
	{
		// For transparency values
		SetBlendState(SrcAlphaBlendingAdd, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Fade()));
	}
}

//--------------------------------------------------------------
// Blend 2 Textures + Fades
//--------------------------------------------------------------

float4 PS_BlendTex2(VS_OUT _inputPS) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);

	diffuse.a = diffuse.a - g_reduceAlpha;

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Interpolating normal can make it not be of unit length so normalize it.
	float3 normal = normalize(_inputPS.normal);

	// Compute the lit color for this pixel.
	SurfaceInfo surface = { _inputPS.position, normal, diffuse, spec };

	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < g_lightCount; i++)
	{
		if (g_light[i].active == true)
		{
			if (g_light[i].type == 0)
			{
				litColor += ParallelLight(surface, g_light[i], g_eyePosW);
			}
			else if (g_light[i].type == 1)
			{
				litColor += PointLight(surface, g_light[i], g_eyePosW);
			}
			else if (g_light[i].type == 2)
			{
				litColor += SpotLight(surface, g_light[i], g_eyePosW);
			}
			else if (g_light[i].type == 3)
			{
				float glowLerp = GlowLight(surface, g_light[i]);

				if (glowLerp > 0.0f)
				{
					litColor = lerp(litColor, g_light[i].diffuse.xyz, glowLerp);
				}
			}
		}
	}

	return float4(litColor, diffuse.a);
}

technique10 BlendTex2Tech
{
	pass P0
	{
		// For transparency values
		SetBlendState(SrcAlphaBlendingAdd, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_BlendTex2()));
	}
}

//--------------------------------------------------------------
// Refraction Shading
//--------------------------------------------------------------

// Refraction Structure for the Vertex Shader Input
struct VS_IN_REFRACT
{
	float4 position : POSITION;
	float3 normal	: NORMAL;
	float2 texCoord	: TEXCOORD0;
};

// Refraction Structure for the Vertex Shader Output and Pixel Shader Input
struct VS_OUT_REFRACT
{
	float4 position : SV_POSITION;
	float2 texCoord	: TEXCOORD0;
	float3 normal	: NORMAL;
	float  clipDist : SV_ClipDistance0;
};

VS_OUT_REFRACT VS_Refract(VS_IN_REFRACT _inputVS)
{
	VS_OUT_REFRACT outputVS;

	// Homogenise the Input position vector
	_inputVS.position.w = 1.0f;

	// Calculate the Position of the vertex in world space
	outputVS.position = mul(_inputVS.position, g_matWorld);
	outputVS.position = mul(outputVS.position, g_matView);
	outputVS.position = mul(outputVS.position, g_matProj);

	// Pass the Texture Coordinates through to the Pixel Shader
	outputVS.texCoord = _inputVS.texCoord;

	// Calculate the normal vector and normalise it
	outputVS.normal = mul(_inputVS.normal, (float3x3)g_matWorld);
	outputVS.normal = normalize(outputVS.normal);

	// Set the clipping distance
	outputVS.clipDist = dot(mul(_inputVS.position, g_matWorld), g_plane);

	return outputVS;
}

float4 PS_Refract(VS_OUT_REFRACT _inputPS) : SV_Target
{
	// Retrieve the color of the pixel at the texture coordinates on the texture maps
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Calculate the lit color for this pixel using the lighting Structure
	SurfaceInfo surface = { _inputPS.position.xyz, _inputPS.normal, diffuse, spec };
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < g_lightCount; i++)
		{
			if (g_light[i].active == true)
			{
				if (g_light[i].type == 0)
				{
					litColor += ParallelLight(surface, g_light[i], g_eyePosW);
				}
				else if (g_light[i].type == 1)
				{
					litColor += PointLight(surface, g_light[i], g_eyePosW);
				}
				else if (g_light[i].type == 2)
				{
					litColor += SpotLight(surface, g_light[i], g_eyePosW);
				}
				else if (g_light[i].type == 3)
				{
					float glowLerp = GlowLight(surface, g_light[i]);

					if (glowLerp > 0.0f)
					{
						litColor = lerp(litColor, g_light[i].diffuse.xyz, glowLerp);
					}
				}
			}
		}

	return float4(litColor, diffuse.a);
}

technique10 RefractTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Refract()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Refract()));
	}
}

//--------------------------------------------------------------
// Reflection Shading
//--------------------------------------------------------------

float4 PS_Reflect(VS_OUT _inputPS) : SV_Target
{
	// Calculate the distance from the plane
	float numerator = g_plane.x * _inputPS.position.x + g_plane.y * _inputPS.position.y + g_plane.z * _inputPS.position.z + g_plane.w;
	float denom = sqrt(g_plane.x * g_plane.x + g_plane.y * g_plane.y + g_plane.z * g_plane.z);
	float distance = numerator / denom;

	// Retrieve the color of the pixel at the texture coordinates on the texture maps
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
		float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);

		// If the distance is greater than zero then the pixel is not within the reflective surface boundarys so clip it
		if (distance > 0.0f)
		{
			// Guarantees Clipping
			clip(diffuse.a - 256);
		}

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Normalise the normal
	float3 normal = normalize(_inputPS.normal);

	// Calculate the lit color for this pixel using the lighting Structure
	SurfaceInfo surface = { _inputPS.position, normal, diffuse, spec };
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < g_lightCount; i++)
		{
			if (g_light[i].active == true)
			{
				if (g_light[i].type == 0)
				{
					litColor += ParallelLight(surface, g_light[i], g_eyePosW);
				}
				else if (g_light[i].type == 1)
				{
					litColor += PointLight(surface, g_light[i], g_eyePosW);
				}
				else if (g_light[i].type == 2)
				{
					litColor += SpotLight(surface, g_light[i], g_eyePosW);
				}
				else if (g_light[i].type == 3)
				{
					float glowLerp = GlowLight(surface, g_light[i]);

					if (glowLerp > 0.0f)
					{
						litColor = lerp(litColor, g_light[i].diffuse.xyz, glowLerp);
					}
				}
			}
		}

	return float4(litColor, diffuse.a);
}

technique10 ReflectTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Reflect()));
	}
}

//--------------------------------------------------------------
// Star Shading
//--------------------------------------------------------------

// Star Structure for the Vertex Shader Output and Pixel Shader Input

struct VS_OUT_STAR
{
	float3 position		: POSITION;
};

struct GS_OUT_STAR
{
	float4 positionH    : SV_POSITION;
};

VS_OUT_STAR VS_Star(VS_IN _inputVS)
{
	VS_OUT_STAR outputVS;

	// Transform to world space space.
	outputVS.position = mul(float4(_inputVS.position, 1.0f), g_matWorld).xyz;

	return outputVS;
}

[maxvertexcount(6)]
void GS_Star(point VS_OUT_STAR gInput[1], inout TriangleStream<GS_OUT_STAR> TriStream)
{
	// Calculate a look, right and up vector from the camera position to the vertex position (Billboard effect)
	float3 vecLook = normalize(g_eyePosW.xyz - gInput[0].position);
		float3 vecRight = normalize(cross(float3(0, 1, 0), vecLook));
		float3 vecUp = cross(vecLook, vecRight);

		// Calculate a new world matrix
		float4x4 matWorld;
	matWorld[0] = float4(vecRight, 0.0f);
	matWorld[1] = float4(vecUp, 0.0f);
	matWorld[2] = float4(vecLook, 0.0f);
	matWorld[3] = float4(gInput[0].position, 1.0f);

	// Calculate the new World, View, Projection Matrix using the new world matrix
	float4x4 matViewProj = mul(g_matView, g_matProj);
		float4x4 matWVP = mul(matWorld, matViewProj);

		// Create two triangles around the vertex point position
		float4 starPoints[6];
	// Triangle One
	starPoints[0] = float4(gInput[0].position.x + 0.0f, gInput[0].position.y - (1.732 * 6), 0, 1.0f);
	starPoints[1] = float4(gInput[0].position.x + 9.0f, gInput[0].position.y + (1.732 * 3.0f), 0, 1.0f);
	starPoints[2] = float4(gInput[0].position.x - 9.0f, gInput[0].position.y + (1.732 * 3.0f), 0, 1.0f);

	// Triangle Two
	starPoints[3] = float4(gInput[0].position.x - 0.0f, gInput[0].position.y + (1.732 * 6), 0, 1.0f);
	starPoints[4] = float4(gInput[0].position.x - 9.0f, gInput[0].position.y - (1.732 * 3.0f), 0, 1.0f);
	starPoints[5] = float4(gInput[0].position.x + 9.0f, gInput[0].position.y - (1.732 * 3.0f), 0, 1.0f);

	// Add each point of the star to the triangleStream
	GS_OUT_STAR outputGS;
	[unroll]
	for (int j = 0; j < 6; j++)
	{
		outputGS.positionH = mul(starPoints[j], matWVP);	// Calculate the position in world space of the star point
		TriStream.Append(outputGS);	// Append the star point

		if (j == 2)
		{
			TriStream.RestartStrip();	// First triangle ended
		}
	}
	TriStream.RestartStrip();	// Second triangle ended
}

float4 PS_Star(GS_OUT_STAR _inputPS) : SV_Target
{
	// return yellow a the color
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

technique10 StarTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Star()));
		SetGeometryShader(CompileShader(gs_4_0, GS_Star()));
		SetPixelShader(CompileShader(ps_4_0, PS_Star()));
	}
}