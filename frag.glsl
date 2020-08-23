#version 420

in vec4 gl_FragCoord;

out vec4 fragcol;

uniform vec2 window;

uniform vec2 center;
uniform float radius;
uniform int num_iters;

void main()
{
    float re = ((gl_FragCoord.x / window.x) * 2 - 1);
    float im = ((gl_FragCoord.y / window.y) * 2 - 1) * (window.y / window.x);

    re = re * radius + center.x;
    im = im * radius + center.y;
    
    float zr = 0, zi = 0;

    float i = 0;
    while (i < num_iters && zr * zr + zi * zi < 100000.0) {
        float tzr = zr * zr - zi * zi + re;
        zi = 2.0 * zr * zi + im;
        zr = tzr;
        i = i + 1;
    }

    if (i < num_iters) {
        float log_zn = log(zr * zr + zi * zi) / 2.0;
        float nu = log(log_zn / log(2.0)) / log(2.0);
        i = i + 1 - nu;
    } else {
        //fragcol = vec4(0, 0, 0, 1);
    }
        fragcol = vec4(vec3(i / float(num_iters)), 1.0);

}
