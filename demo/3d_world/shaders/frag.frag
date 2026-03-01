#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 texColor = texture(texSampler, fragTexCoord);

    float epsilon = 0.9;

    if (all(greaterThanEqual(texColor.rgb, vec3(1.0 - epsilon))) &&
        all(lessThanEqual(texColor.rgb, vec3(1.0 + epsilon))) &&
        abs(texColor.a - 1.0) < epsilon) {

        outColor = vec4(0.0, 0.0, 0.0, 0.0); // green
    } else {
        outColor = texColor;
    }
}