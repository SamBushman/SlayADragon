#include "ScreenManager.h"

ScreenManager* ScreenManager::_mSingleton = NULL;

ScreenManager* ScreenManager::GetInstance()
{
	if(_mSingleton == NULL)
	{
		_mSingleton = new ScreenManager();
	}
	return _mSingleton;
}

ScreenManager::ScreenManager()
{
	_mScreensToRemove = std::vector<IScreen*>();
	_mCurScreen = NULL;
}

void ScreenManager::SetScreen(IScreen* screenToSet)
{
	_mCurScreen = screenToSet;
}

void ScreenManager::Update(Ogre::Real timeSinceLastFrame)
{
	if(_mCurScreen != NULL)
	{
		_mCurScreen->Update(timeSinceLastFrame);
	}
	for(int i = _mScreensToRemove.size() - 1; i >= 0; i--)
	{
		delete _mScreensToRemove[i];
		_mScreensToRemove.pop_back();
	}
}

void ScreenManager::RegisterForRemoval(IScreen* screenToRemove)
{
	_mScreensToRemove.push_back(screenToRemove);
}

void ScreenManager::OnKeyPressed(const OIS::KeyEvent &arg)
{
	if(_mCurScreen != NULL)
	{
		_mCurScreen->OnKeyPressed(arg);
	}
}

void ScreenManager::OnKeyReleased(const OIS::KeyEvent &arg)
{
	if(_mCurScreen != NULL)
	{
		_mCurScreen->OnKeyReleased(arg);
	}
}

void ScreenManager::OnMouseMoved(const OIS::MouseEvent &arg)
{
	if(_mCurScreen != NULL)
	{
		_mCurScreen->OnMouseMoved(arg);
	}
}

void ScreenManager::OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(_mCurScreen != NULL)
	{
		_mCurScreen->OnMousePressed(arg, id);
	}
}

void ScreenManager::OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(_mCurScreen != NULL)
	{
		_mCurScreen->OnMouseReleased(arg, id);
	}
}