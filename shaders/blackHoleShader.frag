#version 410 core

const float PI = 3.14159265358979323846f;
const float TwoPI = 2.f * PI;

in vec2 fragPos;

uniform vec3 cameraPos;
uniform mat4 invViewMat;
uniform mat4 invPersMat;
uniform float blackHoleMass;
uniform samplerCube sampler;

out vec4 color;

struct Ray {
    vec3 origin;
    vec3 direction;
};


Ray RayFromCameraThroughPixel()
{
    Ray result;

    result.origin = cameraPos;

    vec4 ray_clip = vec4(fragPos, -1.0, 1.0);
    vec4 ray_eye = invPersMat * ray_clip;
    ray_eye = vec4(ray_eye.xy, -1.0, 0.0);
    vec3 ray_wor = (invViewMat * ray_eye).xyz;

    result.direction = normalize(ray_wor);

    return result;
}


mat3 RotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);

    float s = sin(angle);
    float c = cos(angle);
    float OneMCos = 1.0 - c;

    return mat3(
        axis.x * axis.x * OneMCos + c,           axis.x * axis.y * OneMCos - axis.z * s,  axis.z * axis.x * OneMCos + axis.y * s,
        axis.x * axis.y * OneMCos + axis.z * s,  axis.y * axis.y * OneMCos + c,           axis.y * axis.z * OneMCos - axis.x * s,
        axis.z * axis.x * OneMCos - axis.y * s,  axis.y * axis.z * OneMCos + axis.x * s,  axis.z * axis.z * OneMCos + c
    );
}


vec3 Rotate(vec3 vec, vec3 axis, float angle) {
    mat3 rot = RotationMatrix(axis, angle);
    return rot * vec;
}


float fun(float w, float massByDis){
    return (1 + massByDis * w) / sqrt(1 + (2.0*massByDis - w)*w);
}


float LightDiffractionAngle(float distanceFromBlackHole){
    const int N = 1024;

    float massDisRatio = blackHoleMass / distanceFromBlackHole;

    float w1 = massDisRatio + sqrt(pow(massDisRatio, 2) + 1);

    // Integral aproximation
    float h = w1 / float(N);

    float result = 0.f;
    for (int i = 1; i < N - 1; i++) {
        float w = float(i) * h;
        result += fun(w, massDisRatio);
    }

    result *= 2.f;
    result += fun(0.f, massDisRatio) + fun(w1 - h/2.f, massDisRatio);
    result *= h;

    if(result > TwoPI)
        result -= TwoPI * floor(result / TwoPI);
    else if(result < 0)
        result += TwoPI * floor(-result / TwoPI);

    return result - PI;
}


void main() {
    Ray ray = RayFromCameraThroughPixel();

    vec3 toBlackHole = -ray.origin;

    // Angle between toBlackHole and direction vectors
    float angle = acos(dot(normalize(toBlackHole), ray.direction));

    float b = sin(angle) * length(toBlackHole);
    if(b < sqrt(27*blackHoleMass))
    {
        color = vec4(0, 0, 0, 1.0);
        return;
    }

    float lightDiffAngle = LightDiffractionAngle(b);

    vec3 rotationAxis = cross(normalize(toBlackHole), ray.direction);
    vec3 dir = normalize(Rotate(ray.direction, rotationAxis, lightDiffAngle));
    color = texture(sampler, dir);

    return;
}
