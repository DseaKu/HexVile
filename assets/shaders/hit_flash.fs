#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Get texel color
    vec4 texel = texture(texture0, fragTexCoord);

    // If the pixel is not transparent, make it white
    if (texel.a > 0.0)
    {
        finalColor = vec4(1.0, 1.0, 1.0, texel.a);
    }
    else
    {
        finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
