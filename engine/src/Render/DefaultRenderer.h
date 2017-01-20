#pragma once
#include "GL/GLProgram.h"
/**
 * The Default renderer performs a phong rendering
 * of the entire scene, using MaterialInstance.tint's 
 * property to color objects. The renderer
 * does not switch programs, or perform
 * any binning.
 */
class DefaultRenderer : public I_Renderer
{
public:
	virtual void setup(Scene * scene);
	virtual void render(Scene * scene);
	virtual void shutdown();

private:
	GLProgram basicPhongProgram;
	Camera * mainCamera;
}