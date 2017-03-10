#pragma once
/**
 * Various tags for game object querying
 */
namespace MoonEngine
{
    enum Tag
    {
        T_NONE = 0x00,
        T_Player = 0x01,
        T_Camera = 0x02,
        T_Terrain = 0x04,
        T_Detail = 0x08,
        T_FX = 0x10,
        T_Dynamic = 0x12,
        T_GUI = 0x14,
        T_ALL = 0xFF
    };
}