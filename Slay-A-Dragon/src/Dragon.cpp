#include <Dragon.h>

Dragon::Dragon()
	: CollidableObject(NULL)
{
	//A default constructor to facilitate copy initialization
}

Dragon::Dragon(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node)
	: CollidableObject(parent_node)
{
	Ogre::String num_str = Ogre::StringConverter::toString(parent_node->numChildren());
	Ogre::String node_name = "DragonNode"+num_str;
	mSceneNode = parent_node->createChildSceneNode();
	Ogre::Entity* dragonMesh = scene_manager->createEntity("Dragon"+num_str, "Dragon.mesh");
	mSceneNode->attachObject(dragonMesh);
	Ogre::ParticleSystem* particleSystem = scene_manager->createParticleSystem("DragonFire"+num_str, "SlayADragon/Fire");
	Ogre::SceneNode* particleNode = mSceneNode->createChildSceneNode("DragonParticleNode"+num_str);
	particleNode->attachObject(particleSystem);
	mParticleEmitter = (Ogre::PointEmitter*)particleSystem->getEmitter(0);
	mParticleEmitter->setDuration(0.5f);
	mParticleEmitter->setEnabled(false);
	mPopHeight = 1.0f;
	mPopSpeed = 3.0f; //World units per second
	isPopping = false;
	mStartHeight = mSceneNode->getPosition().y;
	mTimeWaiting = 0.0f;
	mMaxFireWaitTime = 1.5f;
	mMaxWaitTime = mMaxFireWaitTime + 0.5f;
	scene_manager->_updateSceneGraph(scene_manager->getCamera("PlayerCam"));
	Ogre::AxisAlignedBox boundBox = mSceneNode->_getWorldAABB();
	particleNode->translate(0.0f,  boundBox.getSize().y * 0.6f, 0.0f);
	SetCollideShapeBox(boundBox.getSize().x, boundBox.getSize().y, boundBox.getSize().z * 0.8f);
	geomOffset = Ogre::Vector3(0.0f, boundBox.getSize().y / 2.0f, 0.0f);
	SetCanCollide(false);
	mDidBreathFire = false;
}

void Dragon::SetPosition(Ogre::Vector3 pos)
{
	mSceneNode->setPosition(pos);
	mStartHeight = pos.y;
}

bool Dragon::Popup()
{
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
		mSceneNode->translate(0.0f, mPopSpeed * timeSinceLastFrame * mMoveDirection, 0.0f);
		float curY = mSceneNode->getPosition().y;
		if(curY  <= mStartHeight)
		{
			//Reset the dragon to its start position
			mSceneNode->translate(0.0f, mStartHeight - curY, 0.0f);
			isPopping = false;
		}
		else if(curY >= mStartHeight + mPopHeight && mMoveDirection > 0)
		{
			//Set the dragon at his max height and start waiting
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