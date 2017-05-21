#version 150

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;
// this is the end of the default functionality

// this is something we're creating for this shader
out Vertex {
    vec4 normal;
    vec4 color;
    vec2 texCoord;
} vertex;

void main() {
    gl_Position = position;
    vertex.normal = normal;
    vertex.color =  color;
    vertex.texCoord = texcoord;
}
