#version 330 core
out vec4 FragColor;

in vec3 position;

void main()
{
    float d = distance(position, vec3(0.0f, 0.0f, 0.0f));
    FragColor = vec4(sin(d * 100.0f) * asin(position.x / position.y),
                     sin(d * 100.0f) * asin(position.y / position.x),
                     0.0f,
                     1.0f);
}

