#version 150

uniform ivec2 u_Dimensions;

//in vec4 gl_FragCoord;
//in vec2 fs_UV;

out vec3 color;

//uniform sampler2D u_RenderedTexture;

void main()
{
    /*
    vec4 diffuseColor = vec4(0, 0, 1, 1);//texture(u_RenderedTexture, fs_UV);

    float grey = 0.21 * diffuseColor.x + 0.72 * diffuseColor.y + 0.07 * diffuseColor.z;

    vec2 center = vec2(u_Dimensions.x / 2.f, u_Dimensions.y / 2.f);

    float distance = abs(sqrt(pow(gl_FragCoord.x - center.x, 2) + pow(gl_FragCoord.y - center.y, 2)));

    */
    color = vec3(0, 0, 1);//vec3(grey, grey, grey) - (distance / 1000);
}
