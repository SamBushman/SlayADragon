#ifndef MDI_PHYS_HPP
#define MDI_PHYS_HPP

#define dSINGLE
#include <ode\ode.h>
#include <string>
#include <map>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreSceneNode.h>

enum BoundingShape { BOX, SPHERE, CAPSULE, CYLINDER};

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
	//CollidableObject(CollidableObject& other);
	//CollidableObject& operator=( const CollidableObject& rhs );
	virtual ~CollidableObject();
	const Ogre::SceneNode* GetSceneNode();
	virtual void OnCollide(CollidableObject* other);
	virtual void Update(Ogre::Real timeSinceLastFrame);
};

class PhysWorld
{
public:
    //Attributes
    dWorldID mWorld;
    dSpaceID mSpace;
	std::vector<dSpaceID> mSubSpaces;
    dJointGroupID mContactJoints;
	bool isInitialized;
	int numContactJoints;

    //Methods
	void Initialize();
	void DeInitialize();
	static PhysWorld* GetSingletonPtr();
    ~PhysWorld();
    void Update(double timeSinceLastFrame);
	static void mCallback(void * data, dGeomID g1, dGeomID g2);
    dReal GetWorldERP();
    void SetWorldERP(dReal val);
    dReal GetWorldCFM();
    void SetWorldCFM(dReal val);
    Ogre::Vector3 GetWorldGravity();
    void SetWorldGravity(Ogre::Vector3 grav);
    void SetWorldGravity(dReal x, dReal y, dReal z);
    int GetStepIterations();
    void SetStepIterations(int val);
    dReal GetMaxCorrectionVel();
    void SetMaxCorrectionVel(dReal val);
    dReal GetContactDepth();
    void SetContactDepth(dReal d);
	dGeomID AddCylinder(dReal radius, dReal length);
	dGeomID AddBox(dReal lx, dReal ly, dReal lz);
	dGeomID AddSphere(dReal radius);
	dGeomID AddCapsule(dReal radius, dReal length);
private:
	static PhysWorld* _mSingleton;
	PhysWorld();
};

#endif