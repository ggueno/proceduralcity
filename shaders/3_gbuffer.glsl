#version 150
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


#ifdef _VERTEX_

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexUv;


out vec2 uv;
out vec3 normal;
out vec3 position;
out vec4 vEyeSpacePos;


vec4 DoTwist( vec4 pos, float t )
{
    float st = sin(t);
    float ct = cos(t);
    vec4 new_pos;

    new_pos.x = pos.x*ct - pos.z*st;
    new_pos.z = pos.x*st + pos.z*ct;

    new_pos.y = pos.y;
    new_pos.w = pos.w;

    return( new_pos );
}

void main(void)
{
	uv = vertexUv;

    // float angle_deg = 20*sin(Time);
    // float angle_rad = angle_deg * 3.14159 / 180.0;

    // float ang = (1.0*0.5 + VertexPosition.y)/1.0 * angle_rad;

    // vec4 twistedPosition = DoTwist(vec4(VertexPosition,1.0), ang);
    // vec4 twistedNormal = DoTwist(vec4(VertexNormal,1.0), ang);

    // float grid_width = 3;
    // twistedPosition.x += mod(gl_InstanceID, grid_width) * 100;
    // twistedPosition.y += sin(Time)*gl_InstanceID;
    // twistedPosition.z += int(gl_InstanceID/grid_width) * 100;

    //twistedPosition = vec4(twistedPosition.xyz + vec3(grid_width, 0.0, grid_width), 1.0);

    vec4 vEyeSpacePosVertex = view * model * vec4(vertexPosition,1.0);
    vEyeSpacePos = vEyeSpacePosVertex;

  gl_Position = projection * vEyeSpacePosVertex;
	normal = vertexNormal;
}

#endif

#ifdef _FRAGMENT_
//uniform vec3 CameraPosition;

in vec2 uv;
in vec3 position;
in vec3 normal;
in vec4 vEyeSpacePos;

uniform sampler2D textureUnitDiffuse;
uniform sampler2D textureUnitSpecular;
uniform sampler2D textureUnitNormal;

out vec4  Color;
out vec4  Normal;


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
	vec3 diffuse = texture(textureUnitDiffuse, uv).rgb;
    //Color = vec4(uv.s, uv.t, 0.0, 1.0);
	float spec = texture(textureUnitSpecular, uv).r;
  vec3 nn = normalize(texture(textureUnitNormal, uv).rgb * 2.0 - 1.0);  
  vec3 light_pos = normalize(vec3(10.0, 10.0, 10.5));  
  
  float normal_map = max(dot(nn, light_pos), 0.0);

    float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);

    FogParameters fparams;
    fparams.vFogColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    fparams.fStart = 10.0f;
    fparams.fEnd = 100.0f;
    fparams.fDensity = 0.005f;
    fparams.iEquation = 2;

    //vec4 color_light = vec4(1.0-uv.x*0.5, 1.0-uv.y*0.5, 1.0, spec);
    //vec4 color_light = vec4(diffuse, spec);
    vec4 color_light = vec4(diffuse, spec);
    //Color = color_light;
   //Color = mix(color_light,fparams.vFogColor, getFogFactor(fparams,fFogCoord));
   //Color = vec4(uv, 1.0, 1.0);
   //Color = color_light;

  Color = vec4(uv, 1.0, spec);
  Normal = vec4(normal, spec);
}

#endif
