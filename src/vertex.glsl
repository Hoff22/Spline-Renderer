#version 460 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec4 vNormal; // the position variable has attribute position 0
layout (location = 2) in vec2 uv; // the color variable has attribute position 1

uniform mat4 mode_m;
uniform mat4 view_m;
uniform mat4 proj_m;

#define PI 3.14159265359
#define f 100
#define n 0.1

out vec4 vertexNormal;
out vec4 vertexPos;
out vec2 texcoords; // specify a color output to the fragment shader

void main()
{
    // model matrix

    // view matrix

    // projection matrix

    texcoords = uv;
    vertexPos = mode_m * vec4(aPos,1.0);
    vertexNormal.w = 0.0;
    vertexNormal = inverse(transpose(mode_m)) * vNormal;
    gl_Position = proj_m * view_m * mode_m * vec4(aPos,1.0); // see how we directly give a vec3 to vec4's constructor
}
