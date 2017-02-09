#pragma once
namespace MoonEngine
{
/**
 * Interface for a heightmap
 */
    class IHeightmapSource
    {
    public:
    /**
     * Get the number of unique data points
     * in the X dimension
     * @return number of points
     */
        virtual int getSizeX() = 0;
    /**
     * Get the number of unique data points in the
     * Z dimension
     * @return the number of points
     */
        virtual int getSizeZ() = 0;
    /**
     * Get the height at a point

     * @param  x [description]
     * @param  y [description]
     * @return   [description]
     * @note returned value is converted to height using following formula:
     * @note 'WorldHeight = WorldMinZ + GetHeightAt(,) * WorldSizeZ / 65535.0f'
     */
        virtual unsigned short getHeightAt(int x, int z) = 0;

        virtual float getHeightAtFloat(float x, float z);


    /**
     * Get the maxaimum and minimum size for an area
     * @param  x     lower left
     * @param  y     lower right
     * @param  sizeX width
     * @param  sizeZ depth 
     * @param  minY  returned minumum height
     * @param  maxY  returned maximum height
     * @note returned value is converted to height using following formula:
     * @note 'WorldHeight = WorldMinZ + GetHeightAt(,) * WorldSizeZ / 65535.0f;
     */
        virtual void           getAreaMinMaxY( int x, int z, int sizeX, int sizeZ, unsigned short & minY, unsigned short & maxY );
    };
}
