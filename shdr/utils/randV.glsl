
float rand(in float p, out vec4 col) {
    return fract(sin(p*7.0) * 43758.5453);
}
vec2 rand2(in vec2 p) {
    vec2 x = vec2(7.0, 57.0);
    return fract(sin(vec2(
            dot(p, x.xy),
            dot(p, x.yx)
        )) *
    43758.5453);
}
vec3 rand3(in vec3 p) {
    vec3 x = vec3(7.0, 57.0, 113.0);
    return fract(sin(vec3(
            dot(p, x.xyz),
            dot(p, x.yzx), 
            dot(p, x.zyx)
        )) *
    43758.5453);
}
vec4 rand4(in vec4 p) {
    vec4 x = vec4(7.0, 57.0, 113.0, 607.0);
    return fract(sin(vec4(
            dot(p, x.xyzw),
            dot(p, x.ywzx), 
            dot(p, x.wzyx),
            dot(p, x.wyxz)
        )) *
    43758.5453);
}