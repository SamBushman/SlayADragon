#ifndef DRAGON_H
#define DRAGON_H

#include <CollisionDetection.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMath.h>

class Dragon : public CollidableObject
{
public:
	Dragon();
	Dragon(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node);
	void SetPosition(Ogre::Vector3 pos);
	bool Popup();
	void OnCollide(CollidableObject* other);
	void Update(Ogre::Real timeSinceLastFrame);
	void OnMiss();

private:

};


#endif