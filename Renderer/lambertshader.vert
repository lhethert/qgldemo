#version 120

uniform mat4 matWorldView;
uniform mat4 matWorldViewProj;
uniform mat4 matWorldViewInvTranspose;

attribute vec4 vertPosition;
attribute vec3 vertNormal;

varying vec3 worldViewPos;
varying vec3 worldViewNormal;

void main()
{
    // Set the built-in position variable used in some fixed-functionality between shaders
    gl_Position = matWorldViewProj * vertPosition;
    worldViewPos = (matWorldView * vertPosition).xyz;
    worldViewNormal = normalize((matWorldViewInvTranspose * vec4(vertNormal, 0)).xyz);
}
