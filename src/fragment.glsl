# version 460 core

layout(location = 0) out vec4 FragColor;

in vec4 vertexNormal; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexPos; // the input variable from the vertex shader (same name and same type)  
in vec2 texcoords; // the input variable from the vertex shader (same name and same type)  

vec4 lerp(vec4 a, vec4 b, float alpha){

    return ((b - a) * alpha) + a;
}

float lerp(float a, float b, float alpha){

    return ((b - a) * alpha) + a;
}


void main()
{
    FragColor = vec4(0.8,0.8,0.8,1.0);
}