#version 330

uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform float MaterialShiness;

in vec3 eyePosition;
in vec3 normalVector;
in vec3 lightVector;
uniform vec3 LightPosition;

out vec4 fragColor;

void main(void)
{
    float distance = length(LightPosition - eyePosition);

    vec3 n = normalize(normalVector);
    vec3 l = normalize(lightVector);

    float iDiffuse = clamp(dot(n, l), 0, 1);

    vec3 e = normalize(-eyePosition);
    vec3 r = reflect(-l, n);

    float iSpecular = pow(clamp(dot(e, r), 0, 1), MaterialShiness);
    float lightPower = 20;


    vec3 color = MaterialAmbient +
            lightPower * MaterialDiffuse * iDiffuse / (distance * distance) +
            lightPower * MaterialSpecular * iSpecular / (distance * distance);

    fragColor=vec4(color,1);
}
