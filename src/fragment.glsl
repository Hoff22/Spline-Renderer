# version 460 core

layout(location = 0) out vec4 FragColor;

in vec4 vertexNormal; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexTangent; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexPos; // the input variable from the vertex shader (same name and same type)  
in vec2 texcoords; // the input variable from the vertex shader (same name and same type)  

uniform vec4 solid_color;

vec4 lerp(vec4 a, vec4 b, float alpha){

    return ((b - a) * alpha) + a;
}

float lerp(float a, float b, float alpha){

    return ((b - a) * alpha) + a;
}


void main()
{
    FragColor = max(solid_color , vec4(abs(vertexTangent.xy), 0.0, 1.0));
    //FragColor = vec4(texcoords, 0.0, 1.0);
}