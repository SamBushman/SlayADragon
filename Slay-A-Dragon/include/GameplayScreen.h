#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H

#include <ScreenManager.h>
#include <Whacker.h>
#include <Dragon.h>

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
	Ogre::Entity* mStageEntity;
	Ogre::SceneNode* mStageNode;
	Whacker mWhacker;
	std::vector<Dragon> mDragons;
	float timeBetweenPopups;
	float timeSinceLastPopup;
	unsigned int score;
	int numPeasants;
};

#endif