#if defined(VERTEX)

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Object;
uniform mat4 Camera;

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

out vec2 uv;
out vec3 position;

void main(void)
{	
	uv = VertexTexCoord;
	position = vec3(Object * vec4(VertexPosition, 1.0));
    mat3 scaleMat = mat3( 200, 0, 0
                        , 0, 200, 0
                        , 0, 0, 200 );

    float theta = 90 * 3.1415927 / 180.0;
    mat3 rotMat = mat3(   1,     0,        0
                        , 0, cos(theta), -sin(theta)
                        , 0, sin(theta), cos(theta) );

    position*=scaleMat;
    position*=rotMat;
    position.y += 20;
	gl_Position = Projection * View * Object * vec4(position, 1.0);
}

#endif

#if defined(FRAGMENT)
in vec2 uv;
//in vec3 position;
//in vec3 normal;

uniform sampler2D SkyTexture;

out vec4  Color;

void main(void){

    float alpha = texture(SkyTexture, uv).r;
	//Color = vec4( alpha, alpha,1.0 , alpha );
    Color = vec4( 1.0, 1.0, 1.0, alpha );
}

#endif
