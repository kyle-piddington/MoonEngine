#pragma once
#include "OpenGL.h"
/**
 * RAII Style vertex and element buffer array. Provide a vertex format to the 
 * buffer to create the buffer.
 *
 * RAII principles taken from
 * http://stackoverflow.com/documentation/opengl/7556/encapsulating-opengl-objects-with-c-raii#t=201701180611576446929
 */
namespace MoonEngine
{


	class GLBuffer
	{
	public:
	/**
	 * Construct and allocate an empty vertex buffer.
	 */
		GLBuffer(GLenum target);
	/**
	 * Construct and add data to a vertex buffer.
	 */
		GLBuffer(GLenum target, GLsizeiptr size, const void * data, GLenum usage);

	/**
	 * Destroy a vertex buffer.
	 */
		~GLBuffer();

	/**
	 * Copy constructor(s) is disallowed for vertex buffers.
	 */
		GLBuffer(const GLBuffer & other) = delete;
		GLBuffer &operator=(const GLBuffer &) = delete;

    /**
    Move Constructor(s)
     */
		GLBuffer(GLBuffer &&other);
		GLBuffer &operator=(GLBuffer &&other);

	/**
	 * Set the data in the buffer
	 * @param target Buffer target (Vertex/index)
	 * @param size   size in bytes of the data
	 * @param data   data
	 * @param usage  Dynamic or static
	 */
	void setData(GLsizeiptr size, const void * data, GLenum usage) const;

	/**
	 * Bind a vertex buffer to the GPU
	 */
	void bind() const;

	/**
	 * Retrieve the objectId of the vertex buffer.
	 * @return objectId
	 */
	GLuint getObject() const;

	GLenum getType() const;


	private:
		GLenum _type;
		GLuint _objectId;
		GLuint release();
		GLuint reset(GLuint newObject = 0);


	};
};