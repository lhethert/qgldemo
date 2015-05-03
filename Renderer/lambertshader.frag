#version 120

/**
 * Calculates the radiance contribution for a single light source L. Uses the lambertian
 * BRDF, and assumes that the light irradiance has been pre-multiplied by PI.
 *
 * \pre Assumes n has already been normalized.
 * \pre Assumes l has already been normalized.
 * \pre Assumes El has already been multiplied by 1/PI (lambertian BRDF)
 */
vec4 calcRadianceLambert(in vec4 cDiff, in vec3 n, in vec3 l, in vec4 El)
{
    return cDiff * El * clamp( dot(n, l), 0.0, 1.0 );
}

uniform vec4 diffuseColor;
uniform vec3 lightPos;

varying vec3 worldViewPos;
varying vec3 worldViewNormal;

void main()
{
    // Re-normalize the normal, as it has been interpolated across the primitive
    vec3 normal = normalize(worldViewNormal);
    vec3 lightVec = normalize(lightPos - worldViewPos);
    vec4 color = calcRadianceLambert(diffuseColor, normal, lightVec, vec4(1.0,1.0,1.0,1.0));
    gl_FragColor = color;
    gl_FragDepth = gl_FragCoord.z;
}
