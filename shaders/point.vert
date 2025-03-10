#version 150 core
uniform float aspect;
uniform vec2 size;
uniform float scale;
in vec4 position;
void main()
{
	// gl_Position = vec4(1.0 / aspect, 1.0, 1.0, 1.0) * position;
	gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position;
}
