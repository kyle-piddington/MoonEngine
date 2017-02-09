#pragma once
#include "glm/glm.hpp"

namespace MoonEngine {

    //Basic Colors
    glm::vec3 COLOR_WHITE       (1.00f, 1.00f, 1.00f);
    glm::vec3 COLOR_RED         (1.00f, 0.00f, 0.00f);
    glm::vec3 COLOR_LIME        (0.00f, 1.00f, 0.00f);
    glm::vec3 COLOR_BLUE        (0.00f, 0.00f, 1.00f);
    glm::vec3 COLOR_YELLOW      (1.00f, 1.00f, 0.00f);
    glm::vec3 COLOR_CYAN        (0.00f, 1.00f, 1.00f);
    glm::vec3 COLOR_MAGENTA     (1.00f, 0.00f, 1.00f);
    glm::vec3 COLOR_SILVER      (0.75f, 0.75f, 0.75f);
    glm::vec3 COLOR_GRAY        (0.50f, 0.50f, 0.50f);
    glm::vec3 COLOR_MAROON      (0.50f, 0.00f, 0.00f);
    glm::vec3 COLOR_OLIVE       (0.50f, 0.50f, 0.00f);
    glm::vec3 COLOR_GREEN       (0.00f, 0.50f, 0.00f);
    glm::vec3 COLOR_PURPLE      (0.50f, 0.00f, 0.50f);
    glm::vec3 COLOR_TEAL        (0.00f, 0.50f, 0.50f);
    glm::vec3 COLOR_NAVY        (0.00f, 0.00f, 0.50f);


    //Complex Colors
#ifdef COLORS_MORE
    glm::vec3 COLOR_ALICEBLUE           ( 0.941f, 0.973f, 1.000f );
    glm::vec3 COLOR_ANTIQUEWHITE        ( 0.980f, 0.922f, 0.843f );
    glm::vec3 COLOR_AQUA                ( 0.000f, 1.000f, 1.000f );
    glm::vec3 COLOR_AQUAMARINE          ( 0.498f, 1.000f, 0.831f );
    glm::vec3 COLOR_AZURE               ( 0.941f, 1.000f, 1.000f );
    glm::vec3 COLOR_BEIGE               ( 0.961f, 0.961f, 0.863f );
    glm::vec3 COLOR_BISQUE              ( 1.000f, 0.894f, 0.769f );
    glm::vec3 COLOR_BLUEVIOLET          ( 0.541f, 0.169f, 0.886f );
    glm::vec3 COLOR_BROWN               ( 0.647f, 0.165f, 0.165f );
    glm::vec3 COLOR_BURLYWOOD           ( 0.871f, 0.722f, 0.529f );
    glm::vec3 COLOR_CADETBLUE           ( 0.373f, 0.620f, 0.627f );
    glm::vec3 COLOR_CHARTREUSE          ( 0.498f, 1.000f, 0.000f );
    glm::vec3 COLOR_CHOCOLATE           ( 0.824f, 0.412f, 0.118f );
    glm::vec3 COLOR_CORAL               ( 1.000f, 0.498f, 0.314f );
    glm::vec3 COLOR_CORNFLOWERBLUE      ( 0.392f, 0.584f, 0.929f );
    glm::vec3 COLOR_CORNSILK            ( 1.000f, 0.973f, 0.863f );
    glm::vec3 COLOR_CRIMSON             ( 0.863f, 0.078f, 0.235f );
    glm::vec3 COLOR_DARKBLUE            ( 0.000f, 0.000f, 0.545f );
    glm::vec3 COLOR_DARKCYAN            ( 0.000f, 0.545f, 0.545f );
    glm::vec3 COLOR_DARKGOLDENROD       ( 0.722f, 0.525f, 0.043f );
    glm::vec3 COLOR_DARKGRAY            ( 0.663f, 0.663f, 0.663f );
    glm::vec3 COLOR_DARKGREEN           ( 0.000f, 0.392f, 0.000f );
    glm::vec3 COLOR_DARKGREY            ( 0.663f, 0.663f, 0.663f );
    glm::vec3 COLOR_DARKKHAKI           ( 0.741f, 0.718f, 0.420f );
    glm::vec3 COLOR_DARKMAGENTA         ( 0.545f, 0.000f, 0.545f );
    glm::vec3 COLOR_DARKOLIVEGREE       ( 0.333f, 0.420f, 0.184f );
    glm::vec3 COLOR_DARKORANGE          ( 1.000f, 0.549f, 0.000f );
    glm::vec3 COLOR_DARKORCHID          ( 0.600f, 0.196f, 0.800f );
    glm::vec3 COLOR_DARKRED             ( 0.545f, 0.000f, 0.000f );
    glm::vec3 COLOR_DARKSALMON          ( 0.914f, 0.588f, 0.478f );
    glm::vec3 COLOR_DARKSEAGREEN        ( 0.561f, 0.737f, 0.561f );
    glm::vec3 COLOR_DARKSLATEBLUE       ( 0.282f, 0.239f, 0.545f );
    glm::vec3 COLOR_DARKSLATEGRAY       ( 0.184f, 0.310f, 0.310f );
    glm::vec3 COLOR_DARKSLATEGREY       ( 0.184f, 0.310f, 0.310f );
    glm::vec3 COLOR_DARKTURQUOISE       ( 0.000f, 0.808f, 0.820f );
    glm::vec3 COLOR_DARKVIOLET          ( 0.580f, 0.000f, 0.827f );
    glm::vec3 COLOR_DEEPPINK            ( 1.000f, 0.078f, 0.576f );
    glm::vec3 COLOR_DEEPSKYBLUE         ( 0.000f, 0.749f, 1.000f );
    glm::vec3 COLOR_DIMGRAY             ( 0.412f, 0.412f, 0.412f );
    glm::vec3 COLOR_DIMGREY             ( 0.412f, 0.412f, 0.412f );
    glm::vec3 COLOR_DODGERBLUE          ( 0.118f, 0.565f, 1.000f );
    glm::vec3 COLOR_FIREBRICK           ( 0.698f, 0.133f, 0.133f );
    glm::vec3 COLOR_FLORALWHITE         ( 1.000f, 0.980f, 0.941f );
    glm::vec3 COLOR_FORESTGREEN         ( 0.133f, 0.545f, 0.133f );
    glm::vec3 COLOR_GAINSBORO           ( 0.863f, 0.863f, 0.863f );
    glm::vec3 COLOR_GHOSTWHITE          ( 0.973f, 0.973f, 1.000f );
    glm::vec3 COLOR_GOLD                ( 1.000f, 0.843f, 0.000f );
    glm::vec3 COLOR_GOLDENROD           ( 0.855f, 0.647f, 0.125f );
    glm::vec3 COLOR_GREENYELLOW         ( 0.678f, 1.000f, 0.184f );
    glm::vec3 COLOR_IVORY               ( 1.000f, 1.000f, 0.941f );
    glm::vec3 COLOR_KHAKI               ( 0.941f, 0.902f, 0.549f );
    glm::vec3 COLOR_LAVENDER            ( 0.902f, 0.902f, 0.980f );
    glm::vec3 COLOR_LAWNGREEN           ( 0.486f, 0.988f, 0.000f );
    glm::vec3 COLOR_LEMONCHIFON         ( 1.000f, 0.980f, 0.804f );
    glm::vec3 COLOR_LIGHTBLUE           ( 0.678f, 0.847f, 0.902f );
    glm::vec3 COLOR_LIGHTCORAL          ( 0.941f, 0.502f, 0.502f );
    glm::vec3 COLOR_LIGHTCYAN           ( 0.878f, 1.000f, 1.000f );
    glm::vec3 COLOR_LIGHTGRAY           ( 0.827f, 0.827f, 0.827f );
    glm::vec3 COLOR_LIGHTGREEN          ( 0.565f, 0.933f, 0.565f );
    glm::vec3 COLOR_LIGHTPINK           ( 1.000f, 0.714f, 0.757f );
    glm::vec3 COLOR_LIGHTYELLOW         ( 1.000f, 1.000f, 0.878f );
    glm::vec3 COLOR_LIMEGREEN           ( 0.196f, 0.804f, 0.196f );
    glm::vec3 COLOR_MIDNIGHTBLUE        ( 0.098f, 0.098f, 0.439f );
    glm::vec3 COLOR_MINTCREAM           ( 0.961f, 1.000f, 0.980f );
    glm::vec3 COLOR_MISTYROSE           ( 1.000f, 0.894f, 0.882f );
    glm::vec3 COLOR_ORANGE              ( 1.000f, 0.647f, 0.000f );
    glm::vec3 COLOR_ORANGERED           ( 1.000f, 0.271f, 0.000f );
    glm::vec3 COLOR_ORCHID              ( 0.855f, 0.439f, 0.839f );
    glm::vec3 COLOR_PALEGREEN           ( 0.596f, 0.984f, 0.596f );
    glm::vec3 COLOR_PEACHPUFF           ( 1.000f, 0.855f, 0.725f );
    glm::vec3 COLOR_PINK                ( 1.000f, 0.753f, 0.796f );
    glm::vec3 COLOR_PLUM                ( 0.867f, 0.627f, 0.867f );
    glm::vec3 COLOR_POWDERBLUE          ( 0.690f, 0.878f, 0.902f );
    glm::vec3 COLOR_PURPLE              ( 0.502f, 0.000f, 0.502f );
    glm::vec3 COLOR_ROSYBROWN           ( 0.737f, 0.561f, 0.561f );
    glm::vec3 COLOR_ROYALBLUE           ( 0.255f, 0.412f, 0.882f );
    glm::vec3 COLOR_SALMON              ( 0.980f, 0.502f, 0.447f );
    glm::vec3 COLOR_SANDYBROWN          ( 0.957f, 0.643f, 0.376f );
    glm::vec3 COLOR_SEAGREEN            ( 0.180f, 0.545f, 0.341f );
    glm::vec3 COLOR_SEASHELL            ( 1.000f, 0.961f, 0.933f );
    glm::vec3 COLOR_SIENNA              ( 0.627f, 0.322f, 0.176f );
    glm::vec3 COLOR_SKYBLUE             ( 0.529f, 0.808f, 0.922f );
    glm::vec3 COLOR_TAN                 ( 0.824f, 0.706f, 0.549f );
    glm::vec3 COLOR_TOMATO              ( 1.000f, 0.388f, 0.278f );
    glm::vec3 COLOR_TURQUOISE           ( 0.251f, 0.878f, 0.816f );
    glm::vec3 COLOR_VIOLET              ( 0.933f, 0.510f, 0.933f );
    glm::vec3 COLOR_WHEAT               ( 0.961f, 0.871f, 0.702f );
    glm::vec3 COLOR_YELLOW              ( 1.000f, 1.000f, 0.000f );
    glm::vec3 COLOR_YELLOWGREEN         ( 0.604f, 0.804f, 0.196f );
#endif

}