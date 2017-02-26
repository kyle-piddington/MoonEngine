#include "PostProcessStep.h"
#include "Libraries/Library.h"
using namespace MoonEngine;

void PostProcessStep::renderToScreen()
{
	if(_quadInfo == nullptr)
	{
		_quadInfo = Library::MeshLib->getInfoForMeshNamed("quad_full",false);
	}
	assert(_quadInfo != nullptr);
	_quadInfo->bind();
	glDrawElementsBaseVertex(GL_TRIANGLES,
				_quadInfo->numTris,
				GL_UNSIGNED_SHORT,
				_quadInfo->indexDataOffset,
				_quadInfo->baseVertex);

}