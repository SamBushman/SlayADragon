#include "CollisionDetection.h"

CollidableObject::CollidableObject(Ogre::SceneNode* parent_node)
{
	//Do stuff here to create any entities and adding geometry to the scene graph
	isInWorld = false;
	geomOffset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
}
/*
CollidableObject::CollidableObject(CollidableObject& other)
{
	//PhysWorld::GetSingletonPtr()->UpdateObjPtr(other.mGeom, this);
	mGeom = other.mGeom;
	dGeomSetData(mGeom, (void*)this);
	geomOffset = other.geomOffset;
	isInWorld = other.isInWorld;
	mSceneNode = other.mSceneNode;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

CollidableObject& CollidableObject::operator=( const CollidableObject& rhs ) {
	//PhysWorld::GetSingletonPtr()->UpdateObjPtr(rhs.mGeom, this);
	mGeom = rhs.mGeom;
	dGeomSetData(mGeom, (void*)this);
	geomOffset = rhs.geomOffset;
	isInWorld = rhs.isInWorld;
	mSceneNode = rhs.mSceneNode;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
	return *this;
}
*/
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
	if(isInWorld)
		dGeomDestroy(mGeom);
	mGeom = PhysWorld::GetSingletonPtr()->AddCylinder(radius, length);
	dGeomSetData(mGeom, (void*)this);
	isInWorld = true;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

void CollidableObject::SetCollideShapeBox(dReal lx, dReal ly, dReal lz)
{
	if(isInWorld)
		dGeomDestroy(mGeom);
	mGeom = PhysWorld::GetSingletonPtr()->AddBox(lx, ly, lz);
	dGeomSetData(mGeom, (void*)this);
	isInWorld = true;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

void CollidableObject::SetCollideShapeSphere(dReal radius)
{
	if(isInWorld)
		dGeomDestroy(mGeom);
	mGeom = PhysWorld::GetSingletonPtr()->AddSphere(radius);
	dGeomSetData(mGeom, (void*)this);
	isInWorld = true;
	Update(0.0f); //Ensure the geom's position and orientation match the attached scene node
}

void CollidableObject::SetCollideShapeCapsule(dReal radius, dReal length)
{
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
	return (bool)dGeomIsEnabled(mGeom);
}

void CollidableObject::OnCollide(CollidableObject* other)
{
	//This is to act as a delegate of sorts. Proper action is taken
	//here by the child class and any registered callbacks are then called
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

const Ogre::SceneNode* CollidableObject::GetSceneNode()
{
	return mSceneNode;
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

	mSpace = dHashSpaceCreate(0);
	//Add 4 new subspaces
	mSubSpaces.push_back(dHashSpaceCreate(mSpace));
	mSubSpaces.push_back(dHashSpaceCreate(mSpace));
	mSubSpaces.push_back(dHashSpaceCreate(mSpace));
	mSubSpaces.push_back(dHashSpaceCreate(mSpace));
	
    mContactJoints = dJointGroupCreate(0);

	// Set some "universal" parameters
	dWorldSetGravity(mWorld, 0, -9.8f, 0);
	dWorldSetERP(mWorld, 0.2f);
	dWorldSetCFM(mWorld, 0.00001f);
	
	// Turn on auto-disable and set a small layer around all objects
	// that can be penetrated.
	dWorldSetAutoDisableFlag(mWorld, 1);
	dWorldSetContactSurfaceLayer(mWorld, 0.001f);

	isInitialized = true;

	numContactJoints = 0;
}
void PhysWorld::DeInitialize()
{
    dJointGroupDestroy(mContactJoints);
	for(unsigned int i = 0; i < mSubSpaces.size(); i++)
	{
		dSpaceDestroy(mSubSpaces[i]);
	}
    dSpaceDestroy(mSpace);
    dWorldDestroy(mWorld);
    dCloseODE();
	isInitialized = false;
	mSubSpaces.clear();
}
void PhysWorld::Update(double timeSinceLastFrame)
{
	//Check for collisions (mCallback is called if a collision May occur)
	dSpaceCollide(mSpace, (void *)this, mCallback);

	//Update the Physical Simulation
	if (timeSinceLastFrame > 0)
	{
		dWorldQuickStep(mWorld, (dReal)timeSinceLastFrame * 0.001f);

		//Also remove all generated joints for this frame
		dJointGroupEmpty(mContactJoints);
		numContactJoints = 0;
	}
}

void PhysWorld::mCallback(void * data, dGeomID g1, dGeomID g2)
{
	//We now have a reference to ourselves from the static function (ewww)
	if ((dGeomIsSpace (g1) && !dGeomIsSpace (g2)) || (dGeomIsSpace (g2) && !dGeomIsSpace (g1))) //!!Comment this if block out to allow for collisino between separate spaces
	{
		dSpaceCollide2 (g1, g2, data,&mCallback); 
	}
	if (dGeomIsSpace (g1) || dGeomIsSpace (g2))
	{ 
		// collide all geoms internal to the space(s)
		if (dGeomIsSpace (g1))
			dSpaceCollide ((dSpaceID)g1, data, &mCallback);
		if (dGeomIsSpace (g2))
			dSpaceCollide ((dSpaceID)g2, data, &mCallback);
	}
	else
	{
		// Call the geoms' callbacks
		PhysWorld* pWorld = (PhysWorld*)data;
		CollidableObject* colObj1 = (CollidableObject*)dGeomGetData(g1);
		CollidableObject* colObj2 = (CollidableObject*)dGeomGetData(g2);
		colObj1->OnCollide(colObj2);
		colObj2->OnCollide(colObj1);
	}
}

dReal PhysWorld::GetWorldERP()
{
    return dWorldGetERP(mWorld);
}

void PhysWorld::SetWorldERP(dReal val)
{
    dWorldSetERP(mWorld, val);
}

dReal PhysWorld::GetWorldCFM()
{
    return dWorldGetCFM(mWorld);
}

void PhysWorld::SetWorldCFM(dReal val)
{
    dWorldSetCFM(mWorld, val);
}

Ogre::Vector3 PhysWorld::GetWorldGravity()
{
    dVector3 g;
    dWorldGetGravity(mWorld, g);
    return Ogre::Vector3(g[0], g[1], g[2]);
}

void PhysWorld::SetWorldGravity(Ogre::Vector3 grav)
{
    dWorldSetGravity(mWorld, grav.x, grav.y, grav.z);
}

void PhysWorld::SetWorldGravity(dReal x, dReal y, dReal z)
{
    dWorldSetGravity(mWorld, x, y, z);
}

int PhysWorld::GetStepIterations()
{
    return dWorldGetQuickStepNumIterations(mWorld);
}

void PhysWorld::SetStepIterations(int val)
{
    dWorldSetQuickStepNumIterations(mWorld, val);
}

dReal PhysWorld::GetMaxCorrectionVel()
{
    return dWorldGetContactMaxCorrectingVel(mWorld);
}

void PhysWorld::SetMaxCorrectionVel(dReal val)
{
    dWorldSetContactMaxCorrectingVel(mWorld, val);
}

dReal PhysWorld::GetContactDepth()
{
    return dWorldGetContactSurfaceLayer(mWorld);
}

void PhysWorld::SetContactDepth(dReal d)
{
    dWorldSetContactSurfaceLayer(mWorld, d);
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