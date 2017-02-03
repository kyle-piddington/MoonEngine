#pragma once
namespace MoonEngine
{
    class GLTextureConfiguration
    {
        int width;
        int height;
        //How the texture should be in out (GL_RGB, GL_RGBA, GL_SRGB etc)
        GLenum textureInputFmt;
        //How the texture should be read out
        GLenum textureOutputFmt;
        //How the texture should be stored
        GLenum textureDataType;

    public:
        /**
         * Create a new Texture configuration object, with no settings.
         */
        GLTextureConfiguration()
        {

        }

        /**
         * Create a new texture configuration object with a textue path.
         * The texture will use GL_RGB for it's input and storage types, and
         GL_Unsigned_Byte for it's data read type.
         */
        GLTextureConfiguration(int txWidth, int txHeight):
                width(txWidth),
                height(txHeight),
                textureInputFmt(GL_RGBA),
                textureOutputFmt(GL_RGBA),
                textureDataType(GL_UNSIGNED_BYTE)
        {

        }

        /**
         * Create a new Texture configuration with a texture path, and
         * internal storage details
         */
        GLTextureConfiguration(int txWidth, int txHeight, GLenum inputFmt, GLenum outputFmt, GLenum dataType):
                width(txWidth),
                height(txHeight),
                textureInputFmt(inputFmt),
                textureOutputFmt(outputFmt),
                textureDataType(dataType)
        {

        }

        /**
         * Return the input format specificed by this configuration
         * @return the format.
         */
        GLenum getInputFormat() const
        {
            return textureInputFmt;
        }

        /**
         * Return the output format of this texture specificed by the configuration
         * The output format describes what range of values and kinds of values
         * the GPU should expect when sampling the texture configured by this object.
         * @return the output format
         */
        GLenum getOutputFormat() const
        {
            return textureOutputFmt;
        }

        GLenum getDataType() const
        {
            return textureDataType;
        }

        int getWidth() const
        {
            return width;
        }

        int getHeight() const
        {
            return height;
        }

    };
}
