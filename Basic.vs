#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat3 normMat;
out vec4 vertexColor;
out vec4 viewPos;
out vec3 viewNormal;


void main()
{		
	// Get position of vertex (object space)
	vec4 pos = vec4(position, 1.0);
    vec4 vpos = viewMat*modelMat*pos;
    viewPos = vpos;

	// For now, just pass along vertex position (no transformations)
    
    gl_Position = projMat*vpos;
    
	// Output per-vertex color
	vertexColor = color;
    viewNormal = normMat*normal;
}


