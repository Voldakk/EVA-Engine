#version 150 core
#define MAX_LIGHTS 10
#define MAX_MATERIALS 8
const float PI = 3.14159265359;

in vec2 uvFrag;
in vec3 posFrag;
in vec3 tangentFrag;
in vec4 posLightSpaceFrag [MAX_LIGHTS];

out vec4 fragColor;

// Terrain
uniform int tbnRange;
uniform sampler2D normalmap;
uniform sampler2D splatmap0;
uniform sampler2D splatmap1;

// Materials
uniform int numMaterials;
uniform struct Material
{
	vec4 tint;
	vec2 tiling;
	float heightScale;

    sampler2D albedoMap;
    sampler2D metRouAoMap;
    sampler2D normalHeightMap;
} materials[MAX_MATERIALS];

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// Lights
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 color;
   float farPlane;
   int hasShadows;
   mat4 lightSpaceMatrix;
   sampler2D shadowMap;
   samplerCube shadowCubeMap;
} allLights[MAX_LIGHTS];

uniform vec3 cameraPosition;

// ----------------------------------------------------------------------------
float ShadowCalculation(vec3 normal, vec3 lightDir, sampler2D shadowMap, vec4 fragPosLightSpace)
{
	// Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.005);
	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
// ----------------------------------------------------------------------------
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCubeCalculation(vec3 fragPos, int lightIndex)
{
    vec3 fragToLight = fragPos - allLights[lightIndex].position.xyz;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(cameraPosition - fragPos);
    float diskRadius = (1.0 + (viewDistance / allLights[lightIndex].farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(allLights[lightIndex].shadowCubeMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= allLights[lightIndex].farPlane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   
// ----------------------------------------------------------------------------
void main()
{
	float dist = length(cameraPosition - posFrag);
	float height = posFrag.y;

	vec3 normal = normalize(texture(normalmap, uvFrag).rbg);
	
	vec4 blendValues0 = texture(splatmap0, uvFrag).rgba;
    vec4 blendValues1 = texture(splatmap1, uvFrag).rgba;
	float[MAX_MATERIALS] blendValuesArray = float[](blendValues0.r, blendValues0.g, blendValues0.b, blendValues0.a, blendValues1.r, blendValues1.g, blendValues1.b, blendValues1.a);
	
    vec2[MAX_MATERIALS] UVs;
    for(int i = 0; i < numMaterials; ++i)
	    UVs[i] = uvFrag * materials[i].tiling;

	if (dist < tbnRange - 50)
	{
		float attenuation = clamp(-dist / (tbnRange - 50) + 1, 0.0, 1.0);
		
		vec3 bitangent = normalize(cross(tangentFrag, normal));
		mat3 TBN = mat3(bitangent,normal,tangentFrag);
		
		vec3 bumpNormal;
		for (int i = 0; i < numMaterials; i++)
		{			
			bumpNormal += (2*(texture(materials[i].normalHeightMap, UVs[i]).rbg) - 1) * blendValuesArray[i];
		}
		
		bumpNormal = normalize(bumpNormal);
		bumpNormal.xz *= attenuation;
		normal = normalize(TBN * bumpNormal);
	}
	
    vec3 albedo;
    vec3 metRouAo;
	for(int i = 0; i < numMaterials; ++i)
	{
        albedo    += pow(texture(materials[i].albedoMap, UVs[i]).rgb, vec3(2.2)) * materials[i].tint.rgb * blendValuesArray[i];
        metRouAo  += texture(materials[i].metRouAoMap, UVs[i]).rgb * blendValuesArray[i];
	}
	float metallic = metRouAo.r;
    float roughness = metRouAo.g;
    float ao = metRouAo.b;

	// PBR
    vec3 N = normal;
    vec3 V = normalize(cameraPosition - posFrag);
    vec3 R = reflect(-V, N); 
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numLights; ++i) 
    {
        vec3 L;
        vec3 radiance;
        float shadow = 0;

        // Point light
        if(allLights[i].position.w == 1.0)
        {
            L = normalize(allLights[i].position.xyz - posFrag);
            float distance = length(allLights[i].position.xyz - posFrag);
            float attenuation = 1.0 / (distance * distance);
            radiance = allLights[i].color * attenuation;
            if(allLights[i].hasShadows == 1)
                shadow = ShadowCubeCalculation(posFrag, i);
        }
        else // Directional light
        {
            L =  normalize(allLights[i].position.xyz);
            radiance = allLights[i].color;
            if(allLights[i].hasShadows == 1)
                shadow = ShadowCalculation(N, L, allLights[i].shadowMap, posLightSpaceFrag[i]);
        }

        radiance *= 1 - shadow;

        vec3 H = normalize(V + L);

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G = GeometrySmith(N, V, L, roughness);    
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    fragColor = vec4(color, 1.0);
}
