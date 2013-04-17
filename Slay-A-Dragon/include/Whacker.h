//This header and cpp define the hammer object that is to
//whack dragons in the game.

#ifndef WHACKER_H
#define WHACKER_H

#include <CollisionDetection.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMath.h>
#include <OgreParticleSystem.h>
#include <OgrePointEmitter.h>
#include <OgreRoot.h>

class Whacker : public CollidableObject
{
public:
	~Whacker();
	Whacker(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node);
	void SetMovePlane(Ogre::Vector3 normal, Ogre::Vector3 origin, Ogre::Real width, Ogre::Real height);
	void Move(Ogre::Real delta_x, Ogre::Real delta_y);
	bool Whack();
	void OnCollide(CollidableObject* other);
	void Update(Ogre::Real timeSinceLastFrame);
	bool didHitDragon();


private:
	bool mDidHitDragon;
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
	//Particle Vars
	Ogre::PointEmitter* mParticleEmitter;
	Ogre::SceneNode* mParticleNode;
	Ogre::Vector3 mEmitterOffset;
	Ogre::Entity* mEntity;
	Ogre::ParticleSystem* mParticleSystem;
};

#endif