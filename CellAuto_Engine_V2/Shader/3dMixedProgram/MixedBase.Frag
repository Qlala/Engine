#version 420

in PixelData
{
	vec4 Position;
	vec3 TexCoord;
	vec4 FrontColor;
	vec4 OriginalPos;
} Pixel;

uniform float test=1;

layout(location = 0) out vec4 PixelColor;
layout(location = 1) out vec4 Normal;
out float gl_FragDepth;

void main(void)
{
    gl_FragDepth=Pixel.Position.z/Pixel.Position.w;
    PixelColor.rgb=Pixel.FrontColor.rgb;
    Normal.rgb=vec3(1.0,1.0,1.0)-Pixel.FrontColor.rgb;
    Normal.a=1.0;
    //ShadowRatioOut=1;
    PixelColor.a=1.0;//on maintinen l'alpha de la couleur tout de meme
}
