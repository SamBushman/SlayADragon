#include "GameplayScreen.h"

GameplayScreen::GameplayScreen()
{
	Ogre::SceneManager* mSceneManager = Ogre::Root::getSingletonPtr()->getSceneManager("ApplicationSceneManager");
	mSceneManager->setSkyBox(true, "SlayADragon/SkyBox");
	Ogre::Light* directionalLight = mSceneManager->createLight("directionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setPosition(Ogre::Vector3(0, 0, 250));
	directionalLight->setDirection(Ogre::Vector3( 0, 1, -1 ));
	mStageEntity = mSceneManager->createEntity("Stage", "Stage.mesh");
	mStageNode = mSceneManager->getRootSceneNode()->createChildSceneNode("StageNode");
	mStageNode->attachObject(mStageEntity);
	mStageNode->setPosition(0.0f, -6.0f, -6.5f);
	Ogre::SceneNode foo = Ogre::SceneNode(mSceneManager);
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

}

void GameplayScreen::OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void GameplayScreen::OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void GameplayScreen::Update(Ogre::Real timeSinceLastFrame)
{

}