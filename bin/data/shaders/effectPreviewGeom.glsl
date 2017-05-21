#version 150

#define SPEED 5.0
#define OFFSET 0.01

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

uniform float time;
uniform float boxSize;
uniform mat4 modelViewProjectionMatrix;

in Vertex {
    vec4 normal;
    vec4 color;
    vec2 texCoord;
} vertex[];

out vec4 vColor;

void main() {
    
    float boxSizeHalf = boxSize*0.5;
    
    for(int j = 0; j < gl_in.length(); j++) {
        
        vec3 newVertex = gl_in[j].gl_Position.xyz;
        //newVertex = vec3(0.0f,1.0f,0.0f) * cos(time * SPEED + gl_in[j].gl_Position.x) * OFFSET + vec3(gl_in[j].gl_Position.xyz);
        
        // -z
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,-boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,-boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        EndPrimitive();
        
        // +z
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,-boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,-boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        EndPrimitive();
        
        // +y
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        EndPrimitive();
        
        // -y
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,-boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,-boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,-boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,-boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        EndPrimitive();
        
        // +x
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,-boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,-boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(boxSizeHalf,boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        EndPrimitive();
        
        // -x
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,-boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,-boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,boxSizeHalf,-boxSizeHalf), 1.0);
        EmitVertex();
        
        vColor = vertex[j].color;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex + vec3(-boxSizeHalf,boxSizeHalf,boxSizeHalf), 1.0);
        EmitVertex();
        EndPrimitive();
    }
    
    
}
