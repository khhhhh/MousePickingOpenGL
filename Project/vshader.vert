#version 330

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;
uniform vec3 LightPosition;

uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform float MaterialShiness;

layout (location=0) in vec3 VertexPosition;
layout (location=2) in vec3 VertexColor;
layout (location=3) in vec3 VertexNormal;

out vec3 color;

void main(void)
{
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(VertexPosition, 1);
    vec3 eyePosition = vec4(ViewMat * ModelMat * vec4(VertexPosition, 1)).xyz;

    vec3 n = vec4(ViewMat * ModelMat * vec4(VertexNormal,0)).xyz;
    vec3 l = vec4(ViewMat * vec4(LightPosition, 1)).xyz - eyePosition;

    n = normalize(n);
    l = normalize(l);

    float iDiffuse = clamp(dot(n, l), 0, 1);

    vec3 e = normalize(-eyePosition);
    vec3 r = reflect(-l, n);

    float iSpecular = pow(clamp(dot(e, r), 0, 1), MaterialShiness);

    color = MaterialAmbient +
            MaterialDiffuse * iDiffuse +
            MaterialSpecular * iSpecular;
}
