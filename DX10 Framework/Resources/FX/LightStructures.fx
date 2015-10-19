//=============================================================================
// LightStructures.fx                                                                                                   
//
// Structures and functions for lighting calculations.
//=============================================================================

// Defines
#define LIGHT_DIRECTIONAL	0
#define LIGHT_POINT			1
#define LIGHT_SPOT			2
#define LIGHT_GLOW			3

struct Light
{
	float4 pos_range;
	float4 dir_spotPow;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float4 att;

	int type;
	bool active;
	float pad1;
	float pad2;
};

struct SurfaceInfo
{
	float3 pos;
	float3 normal;
	float4 diffuse;
	float4 spec;
};

float3 ParallelLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

		// The light vector aims opposite the direction the light rays travel.
		float3 lightVec = -L.dir_spotPow.xyz;

		// Add the ambient term.
		litColor += (float3)(v.diffuse * L.ambient);

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if (diffuseFactor > 0.0f)
	{
		float specPower = max(v.spec.a, 1.0f);
		float3 toEye = normalize(eyePos - v.pos);
			float3 R = reflect(-lightVec, v.normal);
			float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * (float3)(v.diffuse * L.diffuse);
		litColor += specFactor * (float3)(v.spec * L.spec);
	}

	return litColor;
}

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

		// The vector from the surface to the light.
		float3 lightVec = L.pos_range.xyz - v.pos;

		// The distance from surface to light.
		float d = length(lightVec);

	if (d > L.pos_range.w)
	{
		return float3(0.0f, 0.0f, 0.0f);
	}

	// Normalize the light vector.
	lightVec /= d;

	// Add the ambient light term.
	litColor += (float3)(v.diffuse * L.ambient);

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if (diffuseFactor > 0.0f)
	{
		float specPower = max(v.spec.a, 1.0f);
		float3 toEye = normalize(eyePos - v.pos);
			float3 R = reflect(-lightVec, v.normal);
			float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * (float3)(v.diffuse * L.diffuse);
		litColor += specFactor * (float3)(v.spec * L.spec);
	}

	// attenuate
	return litColor / dot(L.att.xyz, float3(1.0f, d, d*d));
}

float3 SpotLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = PointLight(v, L, eyePos);

		// The vector from the surface to the light.
		float3 lightVec = normalize(L.pos_range.xyz - v.pos);

		float s = pow(max(dot(-lightVec, L.dir_spotPow.xyz), 0.0f), L.dir_spotPow.w);

	// Scale color by spotlight factor.
	return litColor*s;
}

float GlowLight(SurfaceInfo _surface, Light _light)
{
	float3 litColor = _light.diffuse.xyz;

		// The vector from the surface to the light.
		float3 lightVec = _light.pos_range.xyz - _surface.pos;

		// The distance from surface to light.
		float dist = length(lightVec);

	if (dist > _light.pos_range.w || dist < 3.0f)
	{
		// Outside Glow Light range
		return 0.0f;
	}

	float ratio = 1 - (dist / _light.pos_range.w);

	ratio = ratio * 3.5f;

	if (ratio > 1.0f)
	{
		ratio = 1.0f;
	}

	return ratio;
}


