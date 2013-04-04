#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Ogre.h>
#include <OIS.h>
#include <vector>
class IScreen
{
	//Methods
public:
	virtual ~IScreen() {};
	virtual void OnKeyPressed(const OIS::KeyEvent &arg) = 0;
	virtual void OnKeyReleased(const OIS::KeyEvent &arg) = 0;
	virtual void OnMouseMoved(const OIS::MouseEvent &arg) = 0;
	virtual void OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
	virtual void OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
	virtual void Update(Ogre::Real timeSinceLastFrame) = 0;
};

class ScreenManager
{
	//Methods
public:
	static ScreenManager* GetInstance();
	void SetScreen(IScreen* screenToSet);
	void Update(Ogre::Real timeSinceLastFrame);
	void RegisterForRemoval(IScreen* screenToRemove);
	void OnKeyPressed(const OIS::KeyEvent &arg);
	void OnKeyReleased(const OIS::KeyEvent &arg);
	void OnMouseMoved(const OIS::MouseEvent &arg);
	void OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
	ScreenManager();

	//Attributes
	static ScreenManager* _mSingleton;
	IScreen* _mCurScreen;
	std::vector<IScreen*> _mScreensToRemove;
};

#endif