#ifndef WHACKER_H
#define WHACKER_H

#include <CollisionDetection.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMath.h>

class Whacker : public CollidableObject
{
public:
	Whacker();
	Whacker(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node);
	void SetMovePlane(Ogre::Vector3 normal, Ogre::Vector3 origin, Ogre::Real width, Ogre::Real height);
	void Move(Ogre::Real delta_x, Ogre::Real delta_y);
	bool Whack();
	void OnCollide(CollidableObject* other);
	void Update(Ogre::Real timeSinceLastFrame);

private:
	//Whack Animation Vars
	bool isWhacking;
	float targetPitch;
	float curPitch;
	float whacksPerSecond;
	//Mouse Movement Vars
	float curPlaneX;
	float curPlaneY;
	Ogre::Vector3 rightAxis;
	Ogre::Vector3 towardAxis;
	Ogre::Real planeWidth;
	Ogre::Real planeHeight;
};

#endif