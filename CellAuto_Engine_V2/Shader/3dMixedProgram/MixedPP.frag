#version 440
layout(early_fragment_tests) in;
in PixelData
{
	vec3 TexCoord;
} Pixel;


uniform sampler2DMS FrameTexture_MS;
uniform sampler2D FrameTexture;
uniform sampler2D ZTexture;
uniform sampler2DMS Normal;
out float gl_FragDepth;
layout(location = 0) out vec4 PixelColor;

uniform ivec2 FrameResolution;
uniform int MSAA_Sample=0;
vec4 MSAASampleTex(in sampler2DMS Sam,in vec2 texCord){
    ivec2 ICoord=ivec2(texCord.x*FrameResolution.x,texCord.y*FrameResolution.y);
    vec4 FColor=vec4(0,0,0,0);
    for(int i=0;i<MSAA_Sample;i++){
        FColor+=texelFetch(Sam,ICoord,i);
    }
    return FColor/MSAA_Sample;
}
vec4 AmbiventSampling(in sampler2DMS MultiS, in sampler2D NormalS, in vec2 texcoord){
    if(MSAA_Sample==0){
        return texture(NormalS,texcoord);
    }else{
        return MSAASampleTex(MultiS,texcoord);
    }
}




void main()
{
    PixelColor=AmbiventSampling(FrameTexture_MS,FrameTexture,Pixel.TexCoord.xy);
    //PixelColor=vec4(1.0,0.0,0.0,1.0);
    PixelColor.a=1.0;
    gl_FragDepth=0;
    //PixelColor=vec4(ColorTest,0.0,1.0,1.0);
    //ShadowRatioOut=1;
    //on maintinen l'alpha de la couleur tout de meme
}
