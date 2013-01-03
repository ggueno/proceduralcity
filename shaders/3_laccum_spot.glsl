#version 150

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Object;

#ifdef _VERTEX_

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexUv;

out vec4 vEyeSpacePos;
out vec2 uv;

void main(void)
{
    uv = vertexUv.xy;
    //vec4 vEyeSpacePosVertex = View * Object * vec4(vertexPosition,1.0);
    //vEyeSpacePos = vEyeSpacePosVertex;
	gl_Position = Projection * vec4(vertexPosition, 1.0);
}

#endif

#ifdef _FRAGMENT_


in vec2 uv;
in vec4 vEye;

uniform sampler2D Material;
uniform sampler2D Normal;
uniform sampler2D Depth;
uniform sampler2D Shadow;

uniform vec3  CameraPosition;
uniform vec3  LightPosition;
uniform vec3  LightDirection;
uniform vec3  LightColor;
uniform float LightIntensity;
uniform mat4  InverseViewProjection;
uniform mat4  ProjectionLight;
uniform float Time;
uniform float ShadowBias;
uniform float ShadowSamples;
uniform float ShadowSampleSpread;

vec2 poissonDisk[16] = vec2[](
        vec2( -0.94201624, -0.39906216 ),
        vec2( 0.94558609, -0.76890725 ),
        vec2( -0.094184101, -0.92938870 ),
        vec2( 0.34495938, 0.29387760 ),
        vec2( -0.91588581, 0.45771432 ),
        vec2( -0.81544232, -0.87912464 ),
        vec2( -0.38277543, 0.27676845 ),
        vec2( 0.97484398, 0.75648379 ),
        vec2( 0.44323325, -0.97511554 ),
        vec2( 0.53742981, -0.47373420 ),
        vec2( -0.26496911, -0.41893023 ),
        vec2( 0.79197514, 0.19090188 ),
        vec2( -0.24188840, 0.99706507 ),
        vec2( -0.81409955, 0.91437590 ),
        vec2( 0.19984126, 0.78641367 ),
        vec2( 0.14383161, -0.14100790 )
);

out vec4  Color;

vec3 spotLight(in vec3 lcolor, in float intensity, in vec3 ldir, in vec3 lpos, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
	vec3 l =  lpos - fpos;
	float cosTs = dot( normalize(-l), normalize(ldir) ); 
	float thetaP =  radians(30.0);
	float cosTp = cos(thetaP);      
	vec3 v = fpos - cpos;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(n, l), 0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0, 1.0);
	float d = distance(l, fpos);
	vec3 color = vec3(0.0, 0.0, 0.0);
	if (cosTs > cosTp) 
		color = pow(cosTs, 30.0) * lcolor * intensity * (diffuse * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
	color = pow(cosTs, 30.0) * lcolor * intensity * (diffuse * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));

	return color;
}

float random(vec4 seed)
{
	float dot_product = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}


struct FogParameters
{
   vec4 vFogColor; // Fog color
   float fStart; // This is only for linear fog
   float fEnd; // This is only for linear fog
   float fDensity; // For exp and exp2 equation

   int iEquation; // 0 = linear, 1 = exp, 2 = exp2
} fogParams;

float getFogFactor(FogParameters params, float fFogCoord)
{
   float fResult = 0.0;
   if(params.iEquation == 0)
      fResult = (params.fEnd-fFogCoord)/(params.fEnd-params.fStart);
   else if(params.iEquation == 1)
      fResult = exp(-params.fDensity*fFogCoord);
   else if(params.iEquation == 2)
      fResult = exp(-pow(params.fDensity*fFogCoord, 2.0));

   fResult = 1.0-clamp(fResult, 0.0, 1.0);

   return fResult;
}


void main(void)
{
	vec4  material = texture(Material, uv).rgba;
	vec3  normal = texture(Normal, uv).rgb;
	float depth = texture(Depth, uv).r * 2.0 -1.0;

	vec2  xy = uv * 2.0 -1.0;
	vec4  wPosition =  vec4(xy, depth, 1.0 ) * InverseViewProjection;
	vec3  position = vec3(wPosition/wPosition.w);

	vec4 wlightSpacePosition = ProjectionLight * vec4(position, 1.0);
	vec3 lightSpacePosition = vec3(wlightSpacePosition/wlightSpacePosition.w);

	vec3 diffuse = material.rgb;
	float spec = material.a;

	vec3 n = normalize(normal);

	vec3 cspotlight1 = spotLight(LightColor, LightIntensity, LightDirection, LightPosition, n, position, diffuse, spec, CameraPosition );

	float fFogCoord = abs(vEye.z/vEye.w);

    FogParameters fparams;
    fparams.vFogColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    fparams.fStart = 20.0f;
    fparams.fEnd = 100.0f;
    fparams.fDensity = 0.01f;
    fparams.iEquation = 2;


	vec4 color_light;

	if (wlightSpacePosition.w > 0.0  && lightSpacePosition.x > 0.0 && lightSpacePosition.x < 1.0 && lightSpacePosition.y > 0.0 && lightSpacePosition.y < 1.0 )
	{
		
		float visibility = 1.0;
		float visibilityOffset = 1.0 / ShadowSamples;
		for (int i=0;i<float(ShadowSamples);i++)
		{
			int index = int(16.0*random(vec4(gl_FragCoord.xyy, i)))%16;
		  	if ( texture(Shadow, lightSpacePosition.xy + poissonDisk[index]/ShadowSampleSpread).r  + ShadowBias < lightSpacePosition.z  )
		  	{
			    visibility-=visibilityOffset;
			}
		}
		color_light = vec4(cspotlight1 * visibility, 1.0);

	}
	else
	{
		color_light = vec4(cspotlight1, 1.0);
	} 
	
	//Color = mix(color_light,fparams.vFogColor, getFogFactor(fparams,fFogCoord));
	Color = color_light;

	/* 
	float shadowDepth = texture(Shadow, lightSpacePosition.xy).r;
	Color = vec4(lightSpacePosition.z - shadowDepth , 0.0, 0.0, 1.0);


	if (shadowDepth + 0.0001 < lightSpacePosition.z )
		Color = vec4(0.0, 0.0, 0.0, 1.0);
	else
		Color = vec4(cspotlight1, 1.0);

		*/
	//Color = vec4(lightSpacePosition.z, 0.0, 0.0, 1.0);
	//Color = vec4(lightSpacePosition.xy, -lightSpacePosition.z, 1.0);

	//Color = vec4(shadowDepth , 0.0, 0.0, 1.0);


	//Color = vec4(cspotlight1, 1.0);
	//Color = vec4(texture(Shadow, uv).z, 0.0 , 0.0, 1.0);

	//Color = vec4(shadowDepth, 0.0, 0.0, 1.0);
	//Color = vec4(depth, 0.0 , 0.0, 1.0);
}

#endif
