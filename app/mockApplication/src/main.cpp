/** MockApplication is a pesudo-app demonstraing
	how a user might work with MoonEngine
 */
int main(int argc, char **argv) {

	/** Initialize GLFW */
	/* Begin by creating a new instance of the moonEngine*/
	EngineApp * app = new EngineApp(window, engineConfigs); 

	/*Create a new scene */
	Scene * scene = app->CreateScene();
	/* Scenes could also be loaded from a config file*/

	/* Add game objects to the scene */
	//
	scene->addObject( new DefaultFirstPersonPlayer(position));
	scene->addObject( new DefaultHeightmap("img.png"));
	scene->addObject( new BasicGameController());

	/* Create a renderer 
	   Materials must always have enough information
	   to perform basic rendering.

	   All meshes are guarenteed to have positions 
	   and normals, but not neccesarily texture coordinates
	*/
	DeferredRenderer * renderer = new DeferredRenderer(width, height, 
		"SSAO.program", "SSAOBlur.program",
		"shadow_maps.program", "deferred_stencil.program", 
		"deferred_pointL.program", "deferred_dirL.program");
	renderer->addPostProcessStep(std::make_shared<BasicProgramStep>("postprocess/post_passthrough.program",COMPOSITE_TEXTURE));
	renderer->addPostProcessStep(std::make_shared<BloomStep>(width, height));
	//renderer->addPostProcessStep(std::make_shared<GUIStep>(width, height));
	renderer->addPostProcessStep(std::make_shared<HDRStep>("postprocess/bloom/post_HDR_tonemap.program"));

	app->addCustomUpdate([&](float deltaTime)
	{
		LOG(Info, "NumGameObjects" + scene->getNumberOfObjects());
		// Custom update logic for the application 
		// (Anything that couldn't be created in the )
		// (actual app)
	});
	/**
	 * Hand control of the game to the engine.
	 * At this point
	 */
	app->run(scene,renderer);




	
	return 0;

}