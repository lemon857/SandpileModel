#version 330
out vec4 frag_color;

uniform vec4 sourceColor;

void main() 
{
   frag_color = sourceColor;
}