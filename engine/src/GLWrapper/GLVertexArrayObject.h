#pragma once
#include "OpenGL.h"
#include "GLBuffer.h"
namespace MoonEngine
{
	class GLVertexArrayObject
	{
	public:
		/**
		 * Create an empty VAO
		 */
		GLVertexArrayObject();
		/**
		 * Remove a VAO
		 */
		~GLVertexArrayObject();

		/**
		 * VAO's cannot be copied
		 */
		GLVertexArrayObject(const GLVertexArrayObject & other) = delete;
		GLVertexArrayObject &operator=(const GLVertexArrayObject &) = delete;

		/**
		 * Move constructors
		 */
		GLVertexArrayObject(GLVertexArrayObject && other);
		GLVertexArrayObject &operator=(GLVertexArrayObject && other);

		/**
		 * Bind a vertex buffer to the array object
		 * @param attribute  Attribute index
		 * @param buffer     Buffer to bind to the VAO
		 * @param size       Number of components in each data type
		 * @param type       Type of the data (GL_Float)
		 * @param normalized Should the data be normalized by default?
		 * @param stride     Number of bytes between data objects	
		 * @param offset     Pointer to start of the vertex buffer.
		 */
		void bindVertexBuffer(
			GLuint attribute, 
			const GLBuffer &  buffer, 
			GLint size = 3, 
			GLenum type = GL_FLOAT, 
			GLboolean normalized = false, 
			GLsizei stride = sizeof(GLfloat) * 3, 
			GLvoid * offset = (GLvoid *)0);
	

		/**
		 * Bind EBO to VBO 
		 */
		void bindElementBuffer(const GLBuffer & buffer);

		/**
		 * Bind the VAO
		 */
		void bind() const;

		/**
		 * unbind all VAO's
		 */
		static void Unbind();

		private:
			GLuint _vaoID;

			GLuint reset(GLuint newId = 0);

			GLuint release();


	};
};