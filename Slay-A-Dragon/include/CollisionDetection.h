//This header and cpp define an interface with the collision detection
//system in the Open Dynamics Engine.

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#define dSINGLE
#include <ode\ode.h>
#include <string>
#include <map>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>

//The CollidableObject class acts as an abstract class that defines
//an object in the 3D environment that interfaces with the ODE
//collision detection system. The management of the object's
//interactions with the collision detection system is abstracted
//to the CollidableObject's constructor and its few convenience
//functions.
class CollidableObject
{
protected:
	dGeomID mGeom;
	Ogre::SceneNode* mSceneNode;
	bool isInWorld;
	Ogre::Vector3 geomOffset;
	void SetCollideShapeCylinder(dReal radius, dReal length);
	void SetCollideShapeBox(dReal lx, dReal ly, dReal lz);
	void SetCollideShapeSphere(dReal radius);
	void SetCollideShapeCapsule(dReal radius, dReal length);
	void SetCanCollide(bool canCollide);
	bool GetCanCollide();

public:
	CollidableObject(Ogre::SceneNode* parent_node);
	virtual ~CollidableObject();
	virtual void OnCollide(CollidableObject* other)=0;
	virtual void Update(Ogre::Real timeSinceLastFrame);
};

//This class is a singleton class that wraps essential funcitons
//from ODE's world and space classes. The stepping and configuration
//of ODE's simulation happens here.
class PhysWorld
{
public:
    //Attributes
    dWorldID mWorld;
    dSpaceID mSpace;
	bool isInitialized;

    //Methods
	void Initialize();
	void DeInitialize();
	static PhysWorld* GetSingletonPtr();
    ~PhysWorld();
    void Update(double timeSinceLastFrame);
	static void mCallback(void * data, dGeomID g1, dGeomID g2);
	dGeomID AddCylinder(dReal radius, dReal length);
	dGeomID AddBox(dReal lx, dReal ly, dReal lz);
	dGeomID AddSphere(dReal radius);
	dGeomID AddCapsule(dReal radius, dReal length);
private:
	static PhysWorld* _mSingleton;
	PhysWorld();
};

#endif