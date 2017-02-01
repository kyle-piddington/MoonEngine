#version 330 core
//---------IN------------
in vec3 pos;
//in vec3 sun_norm;

//---------UNIFORM------------
uniform sampler2D tint;//the color of the sky on the half-sphere where the sun is. (time x height)
uniform sampler2D tint2;//the color of the sky on the opposite half-sphere. (time x height)
uniform sampler2D sun;//sun texture (radius x time)

uniform sampler2D clouds1;//light clouds texture (spherical UV projection)
uniform sampler2D clouds2;//heavy clouds texture (spherical UV projection)
//uniform float weather;//mixing factor (0.5 to 1.0)
//uniform float time;
//---------OUT------------
out vec3 color;

//---------MAIN------------
void main(){
    float weather = 1.0;
    vec3 sun_norm = vec3(1.0, 5.0, 1.0);
    float time = 1.0;

    vec3 pos_norm = normalize(pos);
    float dist = dot(sun_norm,pos_norm);

    //We read the tint texture according to the position of the sun and the weather factor
    vec3 color_wo_sun = texture(tint2, vec2((sun_norm.y + 1.0) / 2.0,max(0.01,pos_norm.y))).rgb;
    vec3 color_w_sun = texture(tint, vec2((sun_norm.y + 1.0) / 2.0,max(0.01,pos_norm.y))).rgb;
    color = weather*mix(color_wo_sun,color_w_sun,dist*0.5+0.5);

    //Computing u and v for the clouds textures (spherical projection)
    float u = 0.5 + atan(pos_norm.z,pos_norm.x)/(2*3.14159265);
    float v = - 0.5 + asin(pos_norm.y)/3.14159265;

    //Cloud color
    //color depending on the weather (shade of grey) *  (day or night) ?
    vec3 cloud_color = vec3(min(weather*3.0/2.0,1.0))*(sun_norm.y > 0 ? 0.95 : 0.95+sun_norm.y*1.8);

    //Reading from the clouds maps
    //mixing according to the weather (1.0 -> clouds1 (sunny), 0.5 -> clouds2 (rainy))
    //+ time translation along the u-axis (horizontal) for the clouds movement
    float transparency = mix(texture(clouds2,vec2(u+time,v)).r,texture(clouds1,vec2(u+time,v)).r,(weather-0.5)*2.0);


    //Sun
    float radius = length(pos_norm-sun_norm);
    if(radius < 0.05){//We are in the area of the sky which is covered by the sun
        float time = clamp(sun_norm.y,0.01,1);
        radius = radius/0.05;
        if(radius < 1.0-0.001){//< we need a small bias to avoid flickering on the border of the texture
            //We read the alpha value from a texture where x = radius and y=height in the sky (~time)
            vec4 sun_color = texture(sun,vec2(radius,time));
            color = mix(color,sun_color.rgb,sun_color.a);
        }
    }



    //Final mix
    //mixing with the cloud color allows us to hide things behind clouds (sun, stars, moon)
    color = mix(color,cloud_color,clamp((2-weather)*transparency,0,1));

}
