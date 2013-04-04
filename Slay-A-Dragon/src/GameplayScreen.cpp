#include "GameplayScreen.h"

GameplayScreen::GameplayScreen()
{
	Ogre::Root::getSingletonPtr()->getSceneManager("ApplicationSceneManager")->setSkyBox(true, "SlayADragon/SkyBox");
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