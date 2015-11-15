////////////////////////////////////////////////////////////////////////////////
// Filename: verticalblur.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
float4x4 g_matWorld;
float4x4 g_matView;
float4x4 g_matProj;
Texture2D g_texture;
float g_textureWidth;
float g_textureHeight;

///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleType
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
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VerticalBlurVertexShader(VertexInputType input)
{
	PixelInputType output;
	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, g_matWorld);
	output.position = mul(output.position, g_matView);
	output.position = mul(output.position, g_matProj);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / g_textureHeight;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.tex + float2(0.0f, texelSize * -4.0f);
	output.texCoord2 = input.tex + float2(0.0f, texelSize * -3.0f);
	output.texCoord3 = input.tex + float2(0.0f, texelSize * -2.0f);
	output.texCoord4 = input.tex + float2(0.0f, texelSize * -1.0f);
	output.texCoord5 = input.tex + float2(0.0f, texelSize *  0.0f);
	output.texCoord6 = input.tex + float2(0.0f, texelSize *  1.0f);
	output.texCoord7 = input.tex + float2(0.0f, texelSize *  2.0f);
	output.texCoord8 = input.tex + float2(0.0f, texelSize *  3.0f);
	output.texCoord9 = input.tex + float2(0.0f, texelSize *  4.0f);

	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 VerticalBlurPixelShader(PixelInputType input) : SV_Target
{
	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	float4 color;


	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine vertical pixels to the color by the specific weight of each.
	color += g_texture.Sample(SampleType, input.texCoord1) * weight4;
	color += g_texture.Sample(SampleType, input.texCoord2) * weight3;
	color += g_texture.Sample(SampleType, input.texCoord3) * weight2;
	color += g_texture.Sample(SampleType, input.texCoord4) * weight1;
	color += g_texture.Sample(SampleType, input.texCoord5) * weight0;
	color += g_texture.Sample(SampleType, input.texCoord6) * weight1;
	color += g_texture.Sample(SampleType, input.texCoord7) * weight2;
	color += g_texture.Sample(SampleType, input.texCoord8) * weight3;
	color += g_texture.Sample(SampleType, input.texCoord9) * weight4;

	// Set the alpha channel to one.
	color.a = 1.0f;

	return color;
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 BlurVerticalTech
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VerticalBlurVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, VerticalBlurPixelShader()));
		SetGeometryShader(NULL);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType HorizontalBlurVertexShader(VertexInputType input)
{
	PixelInputType output;
	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, g_matWorld);
	output.position = mul(output.position, g_matView);
	output.position = mul(output.position, g_matProj);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific width.
	texelSize = 1.0f / g_textureWidth;

	// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	output.texCoord1 = input.tex + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = input.tex + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = input.tex + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = input.tex + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = input.tex + float2(texelSize *  0.0f, 0.0f);
	output.texCoord6 = input.tex + float2(texelSize *  1.0f, 0.0f);
	output.texCoord7 = input.tex + float2(texelSize *  2.0f, 0.0f);
	output.texCoord8 = input.tex + float2(texelSize *  3.0f, 0.0f);
	output.texCoord9 = input.tex + float2(texelSize *  4.0f, 0.0f);

	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 HorizontalBlurPixelShader(PixelInputType input) : SV_Target
{
	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	float4 color;


	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine horizontal pixels to the color by the specific weight of each.
	color += g_texture.Sample(SampleType, input.texCoord1) * weight4;
	color += g_texture.Sample(SampleType, input.texCoord2) * weight3;
	color += g_texture.Sample(SampleType, input.texCoord3) * weight2;
	color += g_texture.Sample(SampleType, input.texCoord4) * weight1;
	color += g_texture.Sample(SampleType, input.texCoord5) * weight0;
	color += g_texture.Sample(SampleType, input.texCoord6) * weight1;
	color += g_texture.Sample(SampleType, input.texCoord7) * weight2;
	color += g_texture.Sample(SampleType, input.texCoord8) * weight3;
	color += g_texture.Sample(SampleType, input.texCoord9) * weight4;

	// Set the alpha channel to one.
	color.a = 1.0f;

	return color;
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 BlurHorizontalTech
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, HorizontalBlurVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, HorizontalBlurPixelShader()));
		SetGeometryShader(NULL);
	}
}