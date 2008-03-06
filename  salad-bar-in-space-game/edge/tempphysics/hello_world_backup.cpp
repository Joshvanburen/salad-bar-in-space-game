//include irrnewt header
#include "../IrrNewt_SDK_0_4/include/irrnewt.hpp"

#include "irrlicht.h"
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib,"irrlicht.lib")
#pragma comment(lib,"irrnewt.lib")

using namespace irr;


IrrlichtDevice* device;
irr::SIrrlichtCreationParameters deviceSettings;

video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;

//the physics world
irr::newton::ICharacterController* sphere_body;
irr::newton::IWorld* p_world;
//irr::newton::IBody* sphere_body;

const float move_force=10.0f;
const float jump_force=20.0f;
const float gravity_force=-20.0f;

bool debug_info;
bool fps_cam=false;

bool left_=false,right_=false,jump=false;


//IRRLICHT EVENT RECEIVER
//see irrlicht examples
class MyeventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(SEvent event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == true) {

			if(event.KeyInput.Key==KEY_KEY_A)
				left_=true;
			if(event.KeyInput.Key==KEY_KEY_D)
				right_=true;
			if(event.KeyInput.Key==KEY_KEY_W) 
				jump=true;
		}

		if(event.KeyInput.PressedDown == false) {

			if(event.KeyInput.Key==KEY_KEY_A)
				left_=false;
			if(event.KeyInput.Key==KEY_KEY_D)
				right_=false;
			if(event.KeyInput.Key==KEY_KEY_W) 
				jump=false;
			if(event.KeyInput.Key==KEY_SPACE)
				debug_info=!debug_info;
		
		}
	}
	return false;
	}
}my_event_receiver;



void GetDeviceSettings() {
	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5"\
		"\n (otherKey) exit\n");

	char i;
	std::cin>>i;

	switch(i)
	{
		case 'a': deviceSettings.DriverType = video::EDT_DIRECT3D9;break;
		case 'b': deviceSettings.DriverType = video::EDT_DIRECT3D8;break;
		case 'c': deviceSettings.DriverType = video::EDT_OPENGL;   break;
		default: exit(0);
	}

	deviceSettings.Bits = 16;
	deviceSettings.Fullscreen = false;
	deviceSettings.WindowSize = core::dimension2d<s32>(640,480);
}

int main(int argc, char** argv) {

	//create irrlicht device
	GetDeviceSettings();
	device = createDeviceEx(deviceSettings);

	//change working dir
	device->getFileSystem()->changeWorkingDirectoryTo("../IrrNewt_SDK_0_4/");

	//set event receiver
	device->setEventReceiver(&my_event_receiver);
	//get scene manager to avoid to write device->getSceneManager every time
	smgr=device->getSceneManager();
	//get video driver for the same reason
	driver=device->getVideoDriver();

	//load a map as an oct tree scene node
	//add a zip file
	device->getFileSystem()->addZipFileArchive("media/level/map-20kdm2.pk3");

	//get model mesh
	scene::IAnimatedMesh* world_mesh_anim = smgr->getMesh("media/level/20kdm2.bsp");

	if(world_mesh_anim == NULL) {
		int c = 0;
		int s = 5/c;
	}


	//create the scene node
	scene::ISceneNode* world_node =	smgr->addOctTreeSceneNode(world_mesh_anim);

	world_node->setMaterialFlag(video::EMF_LIGHTING, false);

	//position the scene node
	world_node->setPosition(core::vector3df(45,0,0));


	//TEMP
	/*
	video::ITexture* tex = driver->getTexture("media/rigell.jpg");
	world_node->setMaterialTexture(0, tex);`
	world_node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    world_node->setScale( core::vector3df(1, 15, 1) ); 
    world_node->setPosition(core::vector3df(200, -10, 0)); 
    world_node->setRotation(core::vector3df(0, 0, 90)); 
    */


	//create ellipsoid
	//the ellipsoid is only a scaled sphere
	//create the sphere scene node
	scene::ISceneNode* sphere_node = smgr->addSphereSceneNode(22);

	//the node is not affected by lights
	sphere_node->setMaterialFlag(video::EMF_LIGHTING, false);

	//scale the sphere to make it an ellipsoid
	sphere_node->setScale(core::vector3df(1.0f,1.0f,1.0f));

	//set a texture on it
	sphere_node->setMaterialTexture(0,driver->getTexture("media/bowling_ball_texture.jpg"));

	//set position of the ellipsoid
	sphere_node->setPosition(core::vector3df(1469.6f,300.474f,1363.34f));

	//create an FPS camera
	camera = smgr->addCameraSceneNodeFPS();

	camera->setPosition(core::vector3df(1469.6f,106.474f,1063.34f));

	//PHYSICS
	//Here are your first IrrNewt example!!!!!!!!! Let's add physics to our project
	//We want that the ellipsoid bounce on the map.
	//Let's create physics world first
	//create the physics world. a physics world indicates the world
	//where our bodies is placed
	p_world = irr::newton::createPhysicsWorld(device);

	//let's create the body for our map
	//if we don't do this, the ellipsoid trought the map
	//the creation methos is the same
	irr::newton::SBodyFromNode mapData;
	mapData.Node = world_node;
	//because the node is a oct tree scene node and not a simple scene node we need to pass the mesh also
	mapData.Mesh = world_mesh_anim->getMesh(0);

	mapData.Type = newton::EBT_TREE;

	//create the body
	irr::newton::IBody* world_p_node = p_world->createBody(mapData);


	//then create a body relative to the sphere scene node
	//let's instantiate a irr::newton::SBodyFromNode object
	//to store our body data (like mass, node, mesh,exc..)
	irr::newton::SBodyFromNode sphereData;
	//set the node attached to the body as the sphere scene node
	sphereData.Node = sphere_node;
	sphereData.Type = newton::EBT_PRIMITIVE_ELLIPSOID;

	//sphereData.BodyOffsetFromSceneNode.setScale(core::vector3df(1,2,1));


	//create the body relative to the sphere
	sphere_body = p_world->createCharacterController(p_world->createBody(sphereData));
	sphere_body->setRotationUpdate(true);
	sphere_body->setContinuousCollisionMode(true);

	//materials!! see materials and shapes example
	irr::newton::IMaterial* sphere_material;
	irr::newton::IMaterial* level_material;

	sphere_material = p_world->createMaterial();
	level_material  = p_world->createMaterial();

	sphere_body->setMaterial(sphere_material);
	world_p_node->setMaterial(level_material);

	//rigell<->level comportaments
	sphere_material->setElasticity(level_material, 0.8f);
	sphere_material->setFriction(level_material, 1.0f,0.9f);
	sphere_material->setSoftness(level_material, 0.3f);

	//add gravity
	sphere_body->addForceContinuous(core::vector3df(0,
		gravity_force,
		0));


	//END OF INIT PHISICS

	//add the gravity force to the ellipsoid
	//we use addForceContinuous and not addForce because addForce add the force one time
	//and addForceContinuous add the force recursive (always), and a body is always affected by gravity


	while(device->run()) {
		if(device->isWindowActive()) {

		//STEP 1: MOVING
		//core::vector3df velocity = rigell_body->getVelocity();
		//velocity.X=velocity.Z=0;

		if(left_) //give force left
			sphere_body->addForce(core::vector3df(-move_force,0,0));
		if(right_)//give force right
			sphere_body->addForce(core::vector3df(move_force,0,0));

		core::vector3df pos_ = 	sphere_node->getPosition();
		//pos_.Y = pos_.Y;
		pos_.Z = pos_.Z - 300.00;

		camera->setPosition(pos_);

		//sphere_body->setVelocity(velocity);
		
		//STEP 2:JUMPING
		//give force up
		if(jump)
			//sphere_body->addForce(core::vector3df(0,jump_force,0));
			sphere_body->jump(6);



		//update physics
		//it's very important that you cann this function inside game loop
		p_world->update();

		//clear z buffer
		driver->beginScene(true,true,video::SColor(0,0,0,0));
		//draw the scene
		smgr->drawAll();

		if(debug_info)
			//this function will show all newton body as a wireframe rappresentation (the lines are yellow). 
			//Generally newton bodies are transparent
			p_world->drawAllDebugInfos();		
	
		//put the drawed scene on the screen
		driver->endScene();

           	} //if(isWindowActive)
	} 

	device->drop();

	return 0;
}




