#version 420

layout(location = 0)in vec3 InVertex;
layout(location = 3)in vec3 FrontColor;
layout(location = 4)in vec4 FrontColorRGBA;



out VertexData
{
    vec4 Position;
    vec4 FrontColor;
    vec3 TexCoord;

}Vertex;

void main(void){
    Vertex.Position=vec4(InVertex,1);
    Vertex.FrontColor=vec4(FrontColor,1.0);
    Vertex.TexCoord=vec3(0,0,0);
}
