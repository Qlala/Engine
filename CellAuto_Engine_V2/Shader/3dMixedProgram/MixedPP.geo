#version 330
layout(points) in;
layout (triangle_strip, max_vertices=6 )out;




out PixelData
{
	vec3 TexCoord;
} Pixel;




void main(){
    gl_Position=vec4(1.0,1.0,0.0,1.0);
    Pixel.TexCoord=vec3(1.0,1.0,0.0);
    EmitVertex();
    gl_Position=vec4(-1.0,1.0,0.0,1.0);
    Pixel.TexCoord=vec3(0.0,1.0,1.0);
    EmitVertex();
    gl_Position=vec4(1.0,-1.0,0.0,1.0);
    Pixel.TexCoord=vec3(1.0,0.0,0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position=vec4(-1.0,-1.0,0.0,1.0);
    Pixel.TexCoord=vec3(0.0,0.0,0.0);
    EmitVertex();
    gl_Position=vec4(-1.0,1.0,0.0,1.0);
    Pixel.TexCoord=vec3(0.0,1.0,0.0);
    EmitVertex();
    gl_Position=vec4(1.0,-1.0,0.0,1.0);
    Pixel.TexCoord=vec3(1.0,0.0,0.0);
    EmitVertex();
   EndPrimitive();

}

