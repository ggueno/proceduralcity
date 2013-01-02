#version 150

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

#ifdef _VERTEX_

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexUv;

out vec2 uv;
out vec3 normal;
out vec3 position;

void main(void)
{	
	uv = vertexTexCoord;
	normal = vec3(model * vec4(vertexNormal, 1.0));; 
	position = vec3(model * vec4(vertexPosition, 1.0));
	//position.y += (gl_InstanceID * 1.5); 
	//position.x += (gl_InstanceID * 1.5); 
	gl_Position = projection * view * model * vec4(position, 1.0);
}

#endif

#ifdef _FRAGMENT_

out vec4  Color;

in vec3 normal;

void main(void)
{
}

#endif
