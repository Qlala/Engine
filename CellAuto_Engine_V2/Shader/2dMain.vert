#version 330



layout(location = 0)in vec3 InVertex;
layout(location = 3)in vec3 InColor;


out PerVertexData
{
	vec4 Position;
	vec3 TexCoord;
	vec4 FrontColor;
}Vertex;



void main(){
Vertex.FrontColor=vec4(InColor,1);
Vertex.Position=vec4(InVertex,1);
}

