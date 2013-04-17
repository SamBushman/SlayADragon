#include <Dragon.h>

Dragon::Dragon(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node)
	: CollidableObject(parent_node)
{
	//Set up our OGRE scene objects
	Ogre::String num_str = Ogre::StringConverter::toString(parent_node->numChildren());
	Ogre::String node_name = "DragonNode"+num_str;
	mEntity = scene_manager->createEntity("Dragon"+num_str, "Dragon.mesh");
	mSceneNode->attachObject(mEntity);
	mParticleSystem = scene_manager->createParticleSystem("DragonFire"+num_str, "SlayADragon/Fire");
	mParticleNode = mSceneNode->createChildSceneNode("DragonParticleNode"+num_str);
	mParticleNode->attachObject(mParticleSystem);
	mParticleEmitter = (Ogre::PointEmitter*)mParticleSystem->getEmitter(0);
	mParticleEmitter->setDuration(0.5f);
	mParticleEmitter->setEnabled(false);
	//Set up our "pop" animation variables
	mPopHeight = 1.0f;
	mPopSpeed = 3.0f; //World units per second
	mStartHeight = mSceneNode->getPosition().y;
	mTimeWaiting = 0.0f;
	mMaxFireWaitTime = 1.5f;
	mMaxWaitTime = mMaxFireWaitTime + 0.5f;
	//Set up general object state variables
	mDidBreathFire = false;
	isPopping = false;
	//This is an easy way to dynamicaly get an approx. for the dragon mesh's dimensions
	scene_manager->_updateSceneGraph(scene_manager->getCamera("PlayerCam")); //Necessary to get an accurate AABB
	Ogre::AxisAlignedBox boundBox = mSceneNode->_getWorldAABB();
	//Set up our mesh-size dependent variables
	mParticleNode->translate(0.0f,  boundBox.getSize().y * 0.6f, 0.0f);
	SetCollideShapeBox(boundBox.getSize().x, boundBox.getSize().y, boundBox.getSize().z * 0.8f);
	geomOffset = Ogre::Vector3(0.0f, boundBox.getSize().y / 2.0f, 0.0f);
	SetCanCollide(false);
}

Dragon::~Dragon()
{
	//We only remove OGRE objects that were created in the Dragon class.
	//The CollidableObject base class will take care of the rest
	Ogre::SceneManager* mSceneManager = Ogre::Root::getSingletonPtr()->getSceneManager("ApplicationSceneManager");
	mSceneManager->destroyEntity(mEntity->getName());
	mSceneManager->destroyParticleSystem(mParticleSystem->getName());
	mSceneNode->removeAndDestroyChild(mParticleNode->getName());
}

void Dragon::SetPosition(Ogre::Vector3 pos)
{
	mSceneNode->setPosition(pos);
	mStartHeight = pos.y;
}

bool Dragon::Popup()
{
	//Start the Dragon's Popping Up animation
	//This function returns whether the Dragon
	//actually started popping up or not.
	if(!isPopping)
	{
		isPopping = true;
		mMoveDirection = 1;
		SetCanCollide(true);
		return true;
	}
	else
	{
		return false;
	}
}

void Dragon::OnCollide(CollidableObject* other)
{
	//This function gets called when the Collision Detection
	//system determines that something has collided with the Dragon.
	//Set the dragon to its start height and "unpop" it
	float curY = mSceneNode->getPosition().y;
	mSceneNode->translate(0.0f, mStartHeight - curY, 0.0f);
	isPopping = false;
	SetCanCollide(false);
}

void Dragon::Update(Ogre::Real timeSinceLastFrame)
{
	mDidBreathFire = false;
	if(isPopping)
	{
		//Here we update the popping animation states
		mSceneNode->translate(0.0f, mPopSpeed * timeSinceLastFrame * mMoveDirection, 0.0f);
		float curY = mSceneNode->getPosition().y;
		if(curY  <= mStartHeight)
		{
			//We were moving down, now reset the dragon to its start position
			mSceneNode->translate(0.0f, mStartHeight - curY, 0.0f);
			isPopping = false;
		}
		else if(curY >= mStartHeight + mPopHeight && mMoveDirection > 0)
		{
			//We were moving up, now set the dragon at his max height and start waiting
			mSceneNode->translate(0.0f, mStartHeight + mPopHeight - curY, 0.0f);
			mMoveDirection = 0;
			mTimeWaiting = 0.0f;
		}
		else if(mMoveDirection == 0)
		{
			//We are currently waiting
			mTimeWaiting += timeSinceLastFrame;
			if(mTimeWaiting >= mMaxFireWaitTime && GetCanCollide())
			{
				//We have not been whacked, looks like we
				//need to kill some peasants!
				mParticleEmitter->setEnabled(true);
				SetCanCollide(false);
				mDidBreathFire = true;
			}
			else if(mTimeWaiting >= mMaxWaitTime)
			{
				//We have waited long enough, start moving back down
				mMoveDirection = -1;
			}
		}
	}
	CollidableObject::Update(timeSinceLastFrame);
}

bool Dragon::didBreathFire()
{
	return mDidBreathFire;
}