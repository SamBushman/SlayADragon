#include "CollisionDetection.h"

CollidableObject::CollidableObject(Ogre::SceneNode* parent_node)
{
	//Do stuff here to create any entities and adding geometry to the scene graph
	Ogre::String num_str = Ogre::StringConverter::toString(parent_node->numChildren());
	mSceneNode = parent_node->createChildSceneNode("CollidableObjectNode_"+num_str);
	isInWorld = false;
	geomOffset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
}

CollidableObject::~CollidableObject()
{
	//Disconnect any objects in the scene
	mSceneNode->removeAndDestroyAllChildren();
	mSceneNode->getParentSceneNode()->removeAndDestroyChild(mSceneNode->getName());
	if(isInWorld)
		dGeomDestroy(mGeom);
}

void CollidableObject::SetCollideShapeCylinder(dReal radius, dReal length)
{
	//Register a Cylinder geom with ODE and link it to the collidable object
	if(isInWorld)
		dGeomDestroy(mGeom);
	mGeom = PhysWorld::GetSingletonPtr()->AddCylinder(radius, length);
	dGeomSetData(mGeom, (void*)this);
	isInWorld = true;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

void CollidableObject::SetCollideShapeBox(dReal lx, dReal ly, dReal lz)
{
	//Register a Box geom with ODE and link it to the collidable object
	if(isInWorld)
		dGeomDestroy(mGeom);
	mGeom = PhysWorld::GetSingletonPtr()->AddBox(lx, ly, lz);
	dGeomSetData(mGeom, (void*)this);
	isInWorld = true;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

void CollidableObject::SetCollideShapeSphere(dReal radius)
{
	//Register a Sphere geom with ODE and link it to the collidable object
	if(isInWorld)
		dGeomDestroy(mGeom);
	mGeom = PhysWorld::GetSingletonPtr()->AddSphere(radius);
	dGeomSetData(mGeom, (void*)this);
	isInWorld = true;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

void CollidableObject::SetCollideShapeCapsule(dReal radius, dReal length)
{
	//Register a Capsule geom with ODE and link it to the collidable object
	if(isInWorld)
		dGeomDestroy(mGeom);
	mGeom = PhysWorld::GetSingletonPtr()->AddCapsule(radius, length);
	dGeomSetData(mGeom, (void*)this);
	isInWorld = true;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

void CollidableObject::SetCanCollide(bool canCollide)
{
	if(canCollide)
		dGeomEnable(mGeom);
	else
		dGeomDisable(mGeom);
}

bool CollidableObject::GetCanCollide()
{
	return dGeomIsEnabled(mGeom) != 0;
}

void CollidableObject::Update(Ogre::Real timeSinceLastFrame)
{
	//In the base case, the geom's orientation and position are synced with
	//the referenced scene node
	if(isInWorld)
	{
		Ogre::Quaternion curOrient = mSceneNode->getOrientation();
		Ogre::Vector3 curPos = mSceneNode->getPosition() + (curOrient * geomOffset);
		float dCurOrient[] = {curOrient.w, curOrient.x, curOrient.y, curOrient.z};
		dGeomSetPosition(mGeom, curPos.x, curPos.y, curPos.z);
		dGeomSetQuaternion(mGeom, dCurOrient);
	}
}

PhysWorld* PhysWorld::_mSingleton = NULL;
PhysWorld* PhysWorld::GetSingletonPtr()
{
	return _mSingleton ? _mSingleton : (_mSingleton = new PhysWorld);
}
PhysWorld::PhysWorld()
{
	Initialize();
}
PhysWorld::~PhysWorld()
{
	if (isInitialized)
	{
		DeInitialize();
	}
}

void PhysWorld::Initialize()
{
	dInitODE();
    mWorld = dWorldCreate();
	mSpace = dSimpleSpaceCreate(0);
	isInitialized = true;
}
void PhysWorld::DeInitialize()
{
    dSpaceDestroy(mSpace);
    dWorldDestroy(mWorld);
    dCloseODE();
	isInitialized = false;
}
void PhysWorld::Update(double timeSinceLastFrame)
{
	//Check for collisions (mCallback is called if a collision may occur)
	dSpaceCollide(mSpace, NULL, mCallback);
}

void PhysWorld::mCallback(void * data, dGeomID g1, dGeomID g2)
{
	//Get our CollidableObjects and tell them that a collision
	//has occured
	CollidableObject* colObj1 = (CollidableObject*)dGeomGetData(g1);
	CollidableObject* colObj2 = (CollidableObject*)dGeomGetData(g2);
	colObj1->OnCollide(colObj2);
	colObj2->OnCollide(colObj1);
}

dGeomID PhysWorld::AddCylinder(dReal radius, dReal length)
{
	return dCreateCylinder(mSpace, radius, length);
}

dGeomID PhysWorld::AddBox(dReal lx, dReal ly, dReal lz)
{
	return dCreateBox(mSpace, lx, ly, lz);
}

dGeomID PhysWorld::AddSphere(dReal radius)
{
	return dCreateSphere(mSpace, radius);
}

dGeomID PhysWorld::AddCapsule(dReal radius, dReal length)
{
	return dCreateCapsule(mSpace, radius, length);
}