//This header and cpp define the application's means of managing its states (called "screens")
//and defines the base behavior for a screen.

#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Ogre.h>
#include <OIS.h>
#include <vector>

//This class is an interface for any state in the application. By
//overriding the interface's functions, the state creates its hooks into
//Ogre's update loop and OIS's input handling
class IScreen
{
public:
	virtual ~IScreen() {};
	virtual void OnKeyPressed(const OIS::KeyEvent &arg) = 0;
	virtual void OnKeyReleased(const OIS::KeyEvent &arg) = 0;
	virtual void OnMouseMoved(const OIS::MouseEvent &arg) = 0;
	virtual void OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
	virtual void OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
	virtual void Update(Ogre::Real timeSinceLastFrame) = 0;
};

//The ScreenManager is a singleton that links the application to the current state.
//Registration of new states and removal of current states also happens here.
class ScreenManager
{
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
	static ScreenManager* _mSingleton;
	IScreen* _mCurScreen;
	std::vector<IScreen*> _mScreensToRemove;
};

#endif