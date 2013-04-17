#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <ScreenManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <GameplayScreen.h>

class GameoverScreen : public IScreen
{
public:
	GameoverScreen(unsigned int score);
	~GameoverScreen();
	void OnKeyPressed(const OIS::KeyEvent &arg);
	void OnKeyReleased(const OIS::KeyEvent &arg);
	void OnMouseMoved(const OIS::MouseEvent &arg);
	void OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void Update(Ogre::Real timeSinceLastFrame);
private:
	Ogre::Overlay* mOverlay;
};

#endif