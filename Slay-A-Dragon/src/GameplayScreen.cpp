#include "GameplayScreen.h"

GameplayScreen::GameplayScreen()
{
	Ogre::SceneManager* mSceneManager = Ogre::Root::getSingletonPtr()->getSceneManager("ApplicationSceneManager");
	//Set up the initial scene
	mSceneManager->setSkyBox(true, "SlayADragon/SkyBox");
	Ogre::Light* directionalLight = mSceneManager->createLight("directionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setPosition(Ogre::Vector3(0, 0, 250));
	directionalLight->setDirection(Ogre::Vector3( 0, 1, -1 ));
	mStageEntity = mSceneManager->createEntity("Stage", "Stage.mesh");
	mStageNode = mSceneManager->getRootSceneNode()->createChildSceneNode("StageNode");
	mStageNode->attachObject(mStageEntity);
	mStageNode->setPosition(0.0f, -6.0f, -6.5f);
	//Set up game objects
	mWhacker = Whacker(mSceneManager, mSceneManager->getRootSceneNode());
	mWhacker.SetMovePlane(Ogre::Vector3(0.0f, 2.0f, 1.0f), Ogre::Vector3(0.0f, -0.5f, -6.5f), 3.8f, 2.5f);
	float dragonPositions[] = { -3.0f, -1.5f, -7.2f,
								-1.5f, -1.0f, -8.2f,
								-1.5f, -2.0f, -6.0f,
								0.0f, -1.5f, -7.2f,
								1.5f, -1.0f, -8.2f,
								1.5f, -2.0f, -6.0f,
								3.0f, -1.5f, -7.2f };
	mDragons = std::vector<Dragon>();
	for (int i=0; i < 7; i++)
	{
		mDragons.push_back(Dragon(mSceneManager, mSceneManager->getRootSceneNode()));
		mDragons[i].SetPosition(Ogre::Vector3(dragonPositions[i*3], dragonPositions[(i*3)+1], dragonPositions[(i*3)+2]));
	}
}

GameplayScreen::~GameplayScreen()
{

}

void GameplayScreen::OnKeyPressed(const OIS::KeyEvent &arg)
{
	
}

void GameplayScreen::OnKeyReleased(const OIS::KeyEvent &arg)
{

}

void GameplayScreen::OnMouseMoved(const OIS::MouseEvent &arg)
{
	mWhacker.Move(arg.state.X.rel * 0.01f, arg.state.Y.rel * 0.01f);
}

void GameplayScreen::OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(arg.state.buttonDown(OIS::MouseButtonID::MB_Left))
	{
		mWhacker.Whack();
	}
}

void GameplayScreen::OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void GameplayScreen::Update(Ogre::Real timeSinceLastFrame)
{
	mWhacker.Update(timeSinceLastFrame);
}