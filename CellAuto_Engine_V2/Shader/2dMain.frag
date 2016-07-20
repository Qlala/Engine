#version 330

in PixelData
{
	vec4 Position;
	vec3 TexCoord;
	vec4 FrontColor;
} Pixel;

uniform float test=1;

layout(location = 0) out vec4 PixelColor;







void main()
{
PixelColor.rgb=vec3(test,0,0);
//ShadowRatioOut=1;
PixelColor.a=Pixel.FrontColor.a;//on maintinen l'alpha de la couleur tout de meme
}
