#version 330

in vec2 uv;
uniform float i;
uniform float j;
out vec3 color;


uniform float seed;


float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 g(vec2 v) {
    float x = seed * rand(v);
    return vec2(cos(x), sin(x));
}

float c2(float t){
    return 6*pow(t,5) - 15*pow(t,4) + 10*pow(t, 3);
}

float generateNoise(vec2 pos) {

    vec2 p1 = floor(pos);
    vec2 p2 = p1 + vec2(1,0);
    vec2 p3 = p1 + vec2(0,1);
    vec2 p4 = p1 + vec2(1,1);

    vec2 a = pos - p1;
    vec2 b = pos - p2;
    vec2 c = pos - p3;
    vec2 d = pos - p4;


    float s = dot(g(p1), a);
    float t = dot(g(p2), b);
    float u = dot(g(p3), c);
    float v = dot(g(p4), d);

    float st = mix(s, t, c2(fract(pos.x)));
    float vuv = mix (u, v, c2(fract(pos.x)));

    return mix(st, vuv, c2(fract(pos.y)));
}
//fbm
float fbm(vec2 pos, float H, float lacunarity, int octave, bool simplex) {
    float value = 0.0f;

    vec2 p = pos;

    for (int i = 0; i < octave; i++) {
       value += generateNoise(p) * pow(lacunarity, -H*i);
        p = p * lacunarity;
    }

    return value;
}
//hybrid multifractal by Musgrave
float HybridMultifractal( vec2 coord, float H, float lacunarity, float offset )
{
        const int octaves = 6;
        vec3 point = vec3(coord, 0);// + vec2(generatenoise(point), noise(point+1.12))/3;
      float          frequency, result, signal, weight, remainder;
      int             i;
      int      first = 1;
      float exponent_array[octaves+1]; //octaves + 1

      /* precompute and store spectral weights */
      if ( first == 1 ) {
            /* seize required memory for exponent_array */

            frequency = 1.0;
            for (i=0; i<=octaves; i++) {
                  /* compute weight for each frequency */
                  exponent_array[i] = pow( frequency, -H);
                  frequency *= lacunarity;
            }
            first = 0;
      }

      /* get first octave of function */
      result = ( generateNoise( vec2(point) ) + offset ) * exponent_array[0];
      weight = result;
      /* increase frequency */
      point.x *= lacunarity;
      point.y *= lacunarity;
      point.z *= lacunarity;

      /* spectral construction inner loop, where the fractal is built */
      for (i=1; i<octaves; i++) {
            /* prevent divergence */
            if ( weight > 1.0 )  weight = 1.0;

            /* get next higher frequency */
            signal = ( generateNoise( vec2(point) ) + offset ) * exponent_array[i];
            /* add it in, weighted by previous freq's local value */
            result += weight * signal;
            /* update the (monotonically decreasing) weighting value */
            /* (this is why H must specify a high fractal dimension) */
            weight *= signal;

            /* increase frequency */
            point*=lacunarity;
      } /* for */

      return( result );
}

void main() {
   //  float noise = fbm((uv + vec2(i,j))*3.0f, 1.3f, 2.0f, 8, true)/1.5 - 0.1;
    float noise = HybridMultifractal((uv + vec2(i,j))*2.0f, 0.25f,2.1f,0.9)/6 -0.5;

    color = vec3(noise, noise, noise);

}
