//This header and cpp define the main application state where
//the game takes place

#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H

#include <ScreenManager.h>
#include <Whacker.h>
#include <Dragon.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <GameOverScreen.h>

class GameplayScreen : public IScreen
{
public:
	GameplayScreen();
	~GameplayScreen();
	void OnKeyPressed(const OIS::KeyEvent &arg);
	void OnKeyReleased(const OIS::KeyEvent &arg);
	void OnMouseMoved(const OIS::MouseEvent &arg);
	void OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void Update(Ogre::Real timeSinceLastFrame);
private:
	//Ogre 3D scene objects
	Ogre::SceneNode* mStageNode;
	Ogre::Overlay* mOverlay;
	Ogre::TextAreaOverlayElement* mScoreText;
	Ogre::TextAreaOverlayElement* mPeasantText;
	//Game objects
	Whacker* mWhacker;
	std::vector<Dragon*> mDragons;
	//Game rule variables
	float timeBetweenPopups;
	float timeSinceLastPopup;
	unsigned int score;
	int numPeasants;
	float subtractPopTime;
	unsigned int scorePerPopTimeSub;
	float minPopTime;
	float mWhackerMoveScalar;
};

#endif