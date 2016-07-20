#version 330

layout(points) in;
layout (triangle_strip, max_vertices=6 )out;



in PerVertexData
{
	vec4 Position;
	vec3 TexCoord;
	vec4 FrontColor;
} Vertex[];


out PixelData
{
	vec4 Position;
	vec3 TexCoord;
	vec4 FrontColor;
} Pixel;




void main(){
    for(float i=-0.1;i<0.3;i+=0.2){
    gl_Position=vec4(Vertex[0].Position.x+i,Vertex[0].Position.y+i,Vertex[0].Position.zw);
    Pixel.FrontColor=Vertex[0].FrontColor;
	 EmitVertex();
    gl_Position=vec4(Vertex[0].Position.x+0.1,Vertex[0].Position.y-0.1,Vertex[0].Position.zw);
    Pixel.FrontColor=Vertex[0].FrontColor;
	 EmitVertex();
    gl_Position=vec4(Vertex[0].Position.x-0.1,Vertex[0].Position.y+0.1,Vertex[0].Position.zw);
    Pixel.FrontColor=Vertex[0].FrontColor;
	 EmitVertex();
    }
   EndPrimitive();

}



