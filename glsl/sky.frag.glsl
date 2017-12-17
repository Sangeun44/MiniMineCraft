#version 150

uniform mat4 u_ViewProj;    // We're actually passing the inverse of the viewproj
                            // from our CPU, but it's named u_ViewProj so we don't
                            // have to bother rewriting our ShaderProgram class

uniform ivec2 u_Dimensions; // Screen dimensions

uniform vec3 u_Eye; // Camera pos

uniform float u_Time; //Time progress

in vec4 fs_Nor; //normal

out vec4 outColor4;
vec3 outColor;

const float PI = 3.14159265359;
const float TWO_PI = 6.28318530718;

// Sunset palette
const vec3 sunset[5] = vec3[](vec3(255, 229, 119) / 255.0,
                               vec3(254, 192, 81) / 255.0,
                               vec3(255, 137, 103) / 255.0,
                               vec3(253, 96, 81) / 255.0,
                               vec3(250, 32, 51) / 255.0);
// Dusk palette
const vec3 dusk[5] = vec3[](vec3(144, 96, 144) / 255.0,
                            vec3(96, 72, 120) / 255.0,
                            vec3(72, 48, 120) / 255.0,
                            vec3(48, 24, 96) / 255.0,
                            vec3(0, 24, 72) / 255.0);

const vec3 planet[11] = vec3[](vec3(33, 26, 20) / 255.0,
                        vec3(55, 52, 52) / 255.0,
                        vec3(63, 63, 62) / 255.0,
                        vec3(96, 111, 146) / 255.0,
                        vec3(105, 132, 190) / 255.0,
                        vec3(82, 72, 72) / 255.0,
                        vec3(80, 87, 89) / 255.0,
                        vec3(119, 88, 74) / 255.0,
                        vec3(119, 148, 195) / 255.0,
                        vec3(105, 94, 88) / 255.0,
                        vec3(180, 94, 88) / 255.0);

const vec3 sunColor = vec3(255, 255, 190) / 255.0;
const vec3 cloudColor = sunset[3];

vec2 sphereToUV(vec3);
vec3 uvToSunset(vec2);
vec3 uvToDusk(vec2);

//fbm
float fbm(const in vec2 uv);
float noise(in vec2 uv);
vec2 smoothF(vec2 uv);

vec3 uvToPlanet(vec2 uv);

//fractal noise
float fractal(vec2 n);
float makeNoise(vec2 n);
float rand(vec2 n);

//simplex noise
vec3 permute(vec3 x);
float snoise(vec2 v);

vec3 mod289(vec3 x);
vec4 mod289(vec4 x);
vec4 permute(vec4 x);
vec4 taylorInvSqrt(vec4 r);
float snoise2(vec3 v);

float pattern(in vec2 p);
void main()
{
    vec2 ndc = (gl_FragCoord.xy / vec2(u_Dimensions)) * 2.0 - 1.0; // -1 to 1 NDC

    vec4 p = vec4(ndc.xy, 1, 1); // Pixel at the far clip plane
    p *= 1000.0; // Times far clip plane value
    p = /*Inverse of*/ u_ViewProj * p; // Convert from unhomogenized screen to world

    vec3 rayDir = normalize(p.xyz - u_Eye); //look vector ray

    vec2 uv = sphereToUV(rayDir); //get sphere to uv coordinates from the ray direction

    vec3 skyHue = uvToSunset(uv);

    vec2 uvT1 = uv + vec2(u_Time * 0.0001);
    vec2 uvT2 = uv + vec2(-u_Time * 0.0002);

    float heightField = fractal(uv + vec2(u_Time * 0.001)); //create heightField with fbm and noise

    float fbmWarp1 = pattern(uvT1);
    float fbmWarp2 = pattern(uvT2);

    vec2 slope = vec2(fbm(uvT2 + vec2(1.0/u_Dimensions.x, 0)) - fbm(uvT2 - vec2(1.0/u_Dimensions.x, 0)),
                      fbm(uvT2 + vec2(0, 1.0/u_Dimensions.y)) - fbm(uvT2 - vec2(0, 1.0/u_Dimensions.y)));

    vec3 distortedDuskHue = uvToDusk(uv + fbmWarp2); // get color of the pixel relative to its position in uv
    vec3 distortedSkyHue = uvToSunset(uv + fbmWarp2); // get color of the pixel relative to its position in uv

    vec3 distortedPlanetHue = uvToPlanet(uv + fbmWarp2); //get color of the pixel in the planet

    vec3 planetDir = normalize(vec3(0, 0.5, 1.0)); //planet position
    float planetSize = 70; // planet can exist b/t 0 and 60 degrees from planetDir
    float planetAngle = fbmWarp1* acos(dot(rayDir, planetDir)) * 360.0 / PI;

    vec3 sunDir = normalize(vec3(0, 0.001, 1.0));  //Sun position
    float sunSize = 30; // Sun can exist b/t 0 and 30 degrees from sunDir
    float sunAngle = acos(dot(rayDir, sunDir)) * 360.0 / PI;

//    float n3 = snoise(vec2(fs_Nor.xy * 0.1)) * threshold;
//    float n = n1 + n2 + n3;

    // Draw the sun
//    if(sunAngle <= sunSize)
//        {
//            // 0 to 5 degrees is sun itself
//            if(sunAngle <= 7.5) {
//                //VVVVVVVVVVV reduces cloud cover over sun
//                outColor = mix(sunColor, cloudColor, heightField * 0.75 * sunAngle / 30);
//            }
//            // 7.5 to 30 degrees is sun halo
//            else {
//                vec3 sunBlur = mix(sunColor, distortedSkyHue, (sunAngle - 7.5) / 22.5);
//                outColor = mix(sunBlur, cloudColor, heightField * 0.75 * sunAngle / 30);
//            }
//        }

    // Draw the planet
    if(planetAngle <= planetSize) {
        // 0 to 5 degrees is planet itself
        if(planetAngle < 2) {
            outColor = planet[0];
        }
        else if(planetAngle < 3) {
            outColor = planet[1] + fbmWarp1;
        }
        else if(planetAngle < 5) {
            outColor = planet[2] + fbmWarp1 ;
        }
        else if(planetAngle < 6) {
            outColor = planet[3] + fbmWarp1;
        }
        else if(planetAngle < 8) {
            outColor = planet[4] + fbmWarp1;
        }
        else if(planetAngle < 10) {
            outColor = planet[5] + fbmWarp1;
        }
        else if(planetAngle < 12) {
            outColor = planet[6];
        }
        else if(planetAngle < 14) {
            outColor = planet[7];
        }
        else if(planetAngle < 16) {
            outColor = planet[8];
        }
        else if(planetAngle < 18) {
            outColor = planet[9];
        }
        else if(planetAngle < 19) {
            outColor = planet[10];
        }
        else if(planetAngle < 20) {
            //   VVVVVVVVVVV reduces cloud cover over sun
            outColor = cloudColor;
        }
        // 7.5 to 30 degrees is sun halo
        else {
            vec3 sunBlur = mix(sunColor, planet[8], fbmWarp1 *  (planetAngle - 20) / 22.5);
            outColor = mix(sunBlur, planet[3], fbmWarp1 * 0.75 * planetAngle / 60);
        }
    }

    // Draw the sky, blending from sunset to dusk
    else
    {
        float raySunDot = dot(rayDir, sunDir);
        // Sunset, between 1 and 0.75 dot
        if(raySunDot >= 0.75)
        {
            outColor = mix(distortedSkyHue, sunset[3], heightField * 0.75);
        }
        // LERP sunset and dusk, between 0.75 and -0.1 dot
        else if(raySunDot >= -0.1)
        {
            float t = (raySunDot - 0.75) / -0.85;
            vec3 skyMix = mix(distortedSkyHue, distortedDuskHue, t);
            outColor = mix(skyMix, sunset[3], heightField * 0.75);
        }
        // Pure dusk, less than 0.1 dot
        else
        {
            outColor = mix(distortedDuskHue, sunset[3], heightField * 0.75);
        }
    }

    vec4 fogColor = vec4(1);

    if(gl_FragCoord.z > 900) {
        outColor4 = mix(fogColor, vec4(outColor.x, outColor.y, outColor.z, 1), 0.1);
    }


    outColor4 = vec4(outColor, 1.0);

   // outColor = vec3(ndc * 0.5 + 0.5, 1);
   // outColor = abs(rayDir);
   // outColor = vec3(uv.xy, 0);
   // outColor = vec3(slope * 50, 1);
   // outColor = skyHue;
   // outColor = distortedSkyHue;
   // outColor = mix(skyHue, sunset[3], heightField);
   // outColor = mix(skyHue, vec3(1,1,1) - skyHue, heightField * 0.5);
   // outColor = mix(distortedSkyHue, sunset[3], heightField * 0.75);
   // outColor = mix(outColor, vec3(255, 137, 103) / 255.0, heightField);
   // outColor = distortedSkyHue;

}

//Mix between heights for the sunset colors
vec3 uvToSunset(vec2 uv)
{
    // Below horizon
    if(uv.y < 0.5)
    {
        return sunset[0];
    }
    else if(uv.y < 0.55) // 0.5 to 0.55
    {
        return mix(sunset[0], sunset[1], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6)// 0.55 to 0.6
    {
        return mix(sunset[1], sunset[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) // 0.6 to 0.65
    {
        return mix(sunset[2], sunset[3], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) // 0.65 to 0.75
    {
        return mix(sunset[3], sunset[4], (uv.y - 0.65) / 0.1);
    }
    return sunset[4]; // 0.75 to 1
}

vec3 uvToDusk(vec2 uv)
{
    // Below horizon
    if(uv.y < 0.5)
    {
        return dusk[0];
    }
    else if(uv.y < 0.55) // 0.5 to 0.55
    {
        return mix(dusk[0], dusk[1], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6)// 0.55 to 0.6
    {
        return mix(dusk[1], dusk[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) // 0.6 to 0.65
    {
        return mix(dusk[2], dusk[3], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) // 0.65 to 0.75
    {
        return mix(dusk[3], dusk[4], (uv.y - 0.65) / 0.1);
    }
    return dusk[4]; // 0.75 to 1
}

vec3 uvToPlanet(vec2 uv)
{
    // Below horizon
    if(uv.y < 0.3)
    {
        return planet[0];
    }
    else if(uv.y < 0.55) // 0.5 to 0.55
    {
        return mix(planet[0], planet[1], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6)// 0.55 to 0.6
    {
        return mix(planet[1], planet[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) // 0.6 to 0.65
    {
        return mix(planet[2], planet[3], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) // 0.65 to 0.75
    {
        return mix(planet[3], planet[4], (uv.y - 0.65) / 0.1);
    }
    return planet[4]; // 0.75 to 1
}

// Convert a point on a sphere to a UV coordinate
vec2 sphereToUV(vec3 p)
{
    float phi = atan(p.z, p.x); // Returns atan(z/x)
    if(phi < 0)
    {
        phi += TWO_PI; // [0, TWO_PI] range now
    }
    // ^^ Could also just add PI to phi, but this shifts where the UV loop from X = 1 to Z = -1.
    float theta = acos(p.y); // [0, PI]
    return vec2(1 - phi / TWO_PI, 1 - theta / PI);
}


vec2 smoothF(vec2 uv)
{
    return uv*uv*(3.-2.*uv);
}

//noise creator
float noise(in vec2 uv)
{
    const float k = 257.;
    vec4 l  = vec4(floor(uv),fract(uv));
    float u = l.x + l.y * k;
    vec4 v  = vec4(u, u+1.,u+k, u+k+1.);
    v       = fract(fract(1.23456789*v)*v/.987654321);
    l.zw    = smoothF(l.zw);
    l.x     = mix(v.x, v.y, l.z);
    l.y     = mix(v.z, v.w, l.z);
    return    mix(l.x, l.y, l.w);
}

//get fbm from noise creator
float fbm(const in vec2 uv)
{
    float a = 0.5;
    float f = 5.0;
    float n = 0.;
    int it = 8;
    for(int i = 0; i < 32; i++)
    {
        if(i<it)
        {
            n += noise(uv*f)*a;
            a *= .5;
            f *= 2.;
        }
    }
    return n;
}


vec3 permute(vec3 x) {
    return mod(((x*34.0)+1.0)*x, 289.0);
}

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;

  return 130.0 * dot(m, g);
}

float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float makeNoise(vec2 n) {
    vec2 floor = floor(n);
    vec2 fract = fract(n);
    float a = rand(floor);
    float b = rand(floor + vec2(1.0, 0.0));
    float c = rand(floor + vec2(0.0, 1.0));
    float d = rand(floor + vec2(1.0, 1.0));
    vec2 fractVec = fract * fract * (3.f - 2.f * fract);

    return mix(a, b, fractVec.x) + (c - a) * fractVec.y * (1.f - fractVec.x) +
            (d - b) * fractVec.x * fractVec.y;
}

float fractal(vec2 n) {
    float value = 0.0;
    float amp = .5;
    vec2 shift = vec2(100.0);
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));

    for (int i = 0; i < 6; i++) {
        value += amp * makeNoise(n);
        n = rot * n * 2.f + shift;
        amp*= .5;
    }
    return value;
}

//fractal noise
float noise(vec3 position, int octaves, float frequency, float persistence) {
   float total = 0.0; // Total value so far
   float maxAmplitude = 0.0; // Accumulates highest theoretical amplitude
   float amplitude = 1.0;
   for (int i = 0; i < octaves; i++) {
       // Get the noise sample
       total += snoise2(position * frequency) * amplitude;
       // Make the wavelength twice as small
       frequency *= 2.0;
       // Add to our maximum possible amplitude
       maxAmplitude += amplitude;
       // Reduce amplitude according to persistence for the next octave
       amplitude *= persistence;
   }
   // Scale the result by the maximum amplitude
   return total / maxAmplitude;
}

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise2(vec3 v)
  {
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i);
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
  }

//multiple warping fbm
float pattern(in vec2 p)
  {
      vec2 q = vec2(fbm(p + vec2(0.0,0.0)),
                     fbm(p + vec2(5.2,1.3)));

      vec2 r = vec2(fbm(p + 4.0*q + vec2(1.7,9.2)),
                     fbm(p + 4.0*q + vec2(8.3,2.8)));

      return fbm( p + 4.0 * r );
  }
