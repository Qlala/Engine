#version 420
layout(triangles) in;
layout (triangle_strip, max_vertices=20) out;
in VertexData
{
    vec4 Position;
    vec4 FrontColor;
    vec3 TexCoord;
}Vertex[];
out gl_PerVertex//à rajouté pour corrigé le probleme
{
    vec4 gl_Position;
};

out PixelData
{
	vec4 Position;
	vec3 TexCoord;
	vec4 FrontColor;
	vec4 OriginalPos;
}Pixel;

uniform mat4 ProjMatrix;
uniform mat4 CamMatrix;
uniform mat4 InvCamMatrix;
uniform mat4 ModelMat;
mat4 TransformMat;
void ScreenQuad(void){
    gl_Position=vec4(1.0,1.0,1.0,1.0);
    Pixel.Position=vec4(1.0,1.0,1.0,1.0);
    Pixel.FrontColor=vec4(1.0,1.0,0.0,1.0);
    EmitVertex();
    gl_Position=vec4(-1.0,1.0,1.0,1.0);
    Pixel.Position=vec4(-1.0,1.0,1.0,1.0);
    Pixel.FrontColor=vec4(0.0,1.0,1.0,1.0);
    EmitVertex();
    gl_Position=vec4(1.0,-1.0,1.0,1.0);
    Pixel.Position=vec4(1.0,-1.0,1.0,1.0);
    Pixel.FrontColor=vec4(1.0,0.0,0.0,1.0);
    EmitVertex();
    EndPrimitive();

    gl_Position=vec4(-1.0,-1.0,1.0,1.0);
    Pixel.Position=vec4(-1.0,-1.0,1.0,1.0);
    Pixel.FrontColor=vec4(0.0,0.0,0.0,1.0);
    EmitVertex();
    gl_Position=vec4(-1.0,1.0,1.0,1.0);
    Pixel.Position=vec4(-1.0,1.0,1.0,1.0);
    Pixel.FrontColor=vec4(0.0,1.0,0.0,1.0);
    EmitVertex();
    gl_Position=vec4(1.0,-1.0,1.0,1.0);
    Pixel.Position=vec4(1.0,-1.0,1.0,1.0);
    Pixel.FrontColor=vec4(1.0,0.0,0.0,1.0);
    EmitVertex();
   EndPrimitive();
}

void TestMatrix(){
    TransformMat=ProjMatrix*(CamMatrix*ModelMat);
    Pixel.Position=TransformMat*vec4(0.0,1.0,0.0,1.0);
    gl_Position=Pixel.Position;
    Pixel.FrontColor=vec4(0.1,0.0,0.0,1.0);
    EmitVertex();
    Pixel.Position=TransformMat*vec4(0.87,-0.5,0.0,1.0);
    gl_Position=Pixel.Position;
    Pixel.FrontColor=vec4(0.1,1.0,0.0,1.0);
    EmitVertex();
    Pixel.Position=TransformMat*vec4(-0.87,-0.5,0.0,1.0);
    gl_Position=Pixel.Position;
    Pixel.FrontColor=vec4(1.0,0.0,0.0,1.0);
    EmitVertex();
   EndPrimitive();
   /*
    Pixel.Position=vec4(0.0,1.0,-1.0,1.0);
    gl_Position=Pixel.Position;
    Pixel.FrontColor=vec4(0.1,0.0,0.0,1.0);
    EmitVertex();
    Pixel.Position=vec4(0.87,-0.5,-1.0,1.0);
    gl_Position=Pixel.Position;
    Pixel.FrontColor=vec4(0.1,1.0,0.0,1.0);
    EmitVertex();
    Pixel.Position=vec4(-0.87,-0.5,-1.0,1.0);
    gl_Position=Pixel.Position;
    Pixel.FrontColor=vec4(1.0,0.0,0.0,1.0);
    EmitVertex();
   EndPrimitive();*/
}

void UsualRender(void){
    TransformMat=ProjMatrix*(CamMatrix*ModelMat);
    for(int i=0;i<3;i++){
        vec4 RealPosition=TransformMat*Vertex[i].Position;
        gl_Position=RealPosition;
        Pixel.Position=RealPosition;
        Pixel.OriginalPos=Vertex[i].Position;
        Pixel.FrontColor=Vertex[i].FrontColor;
        Pixel.TexCoord=Vertex[i].TexCoord;
        EmitVertex();
    }
    EndPrimitive();

}

void main(){
    TestMatrix();
}

