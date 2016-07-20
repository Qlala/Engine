#version 330

in PixelData
{
	vec3 TexCoord;
} Pixel;


uniform sampler2D Tex1;
uniform float ColorTest;
layout(location = 0) out vec4 PixelColor;

int FrameHeight=1366;
int FrameWidth=768;





void main()
{
PixelColor=texture2D(Tex1,Pixel.TexCoord.xy);
//PixelColor=vec4(ColorTest,0.0,1.0,1.0);
//ShadowRatioOut=1;
//on maintinen l'alpha de la couleur tout de meme
}
