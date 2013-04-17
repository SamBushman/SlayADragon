#ifndef DRAGON_H
#define DRAGON_H

#include <CollisionDetection.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMath.h>
#include <OgreParticleSystem.h>
#include <OgrePointEmitter.h>
#include <OgreRoot.h>

class Dragon : public CollidableObject
{
public:
	Dragon();
	Dragon(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node);
	Dragon::~Dragon();
	void SetPosition(Ogre::Vector3 pos);
	bool Popup();
	void OnCollide(CollidableObject* other);
	void Update(Ogre::Real timeSinceLastFrame);
	bool didBreathFire();

private:
	float mPopHeight;
	char mMoveDirection;
	float mTimeWaiting;
	float mMaxFireWaitTime;
	float mMaxWaitTime;
	float mPopSpeed;
	float mStartHeight;
	bool isPopping;
	bool mDidBreathFire;
	//Particle Vars
	Ogre::PointEmitter* mParticleEmitter;
	Ogre::Entity* mEntity;
	Ogre::ParticleSystem* mParticleSystem;
	Ogre::SceneNode* mParticleNode;
};


#endif