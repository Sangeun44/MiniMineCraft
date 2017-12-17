#version 150

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.

in vec4 vs_Pos;

in vec4 vs_Nor;

out vec4 fs_Nor;

void main()
{
    gl_Position = vs_Pos;

    mat3 invTranspose = mat3(u_ModelInvTr);
    fs_Nor = vec4(invTranspose * vec3(vs_Nor), 0);          // Pass the vertex normals to the fragment shader for interpolation.

}
