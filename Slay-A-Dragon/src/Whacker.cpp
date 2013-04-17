#include <Whacker.h>

Whacker::Whacker(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node)
	: CollidableObject(parent_node)
{
	//3D scene objects
	mEntity = scene_manager->createEntity("Whacker", "Hammer.mesh");
	mSceneNode->attachObject(mEntity);
	mParticleSystem = scene_manager->createParticleSystem("Whacker", "SlayADragon/Whack");
	mParticleNode = parent_node->createChildSceneNode("WhackerParticleNode");
	mParticleNode->attachObject(mParticleSystem);
	mParticleEmitter = (Ogre::PointEmitter*)mParticleSystem->getEmitter(0);
	mParticleEmitter->setDuration(0.3f);
	mParticleEmitter->setEnabled(false);
	//Animation variables
	isWhacking = false;
	targetPitch = 0.0f;
	curPitch = 0.0f;
	whacksPerSecond = 4.0f;
	//This is an easy way to dynamicaly get an approx. for the hammer mesh's dimensions
	scene_manager->_updateSceneGraph(scene_manager->getCamera("PlayerCam")); //Necessary to get an accurate AABB
	Ogre::AxisAlignedBox boundBox = mSceneNode->_getWorldAABB();
	//Set up our mesh-size dependent variables
	SetCollideShapeBox(boundBox.getSize().x, boundBox.getSize().y * 0.1f, boundBox.getSize().z);
	geomOffset = Ogre::Vector3(0.0f, boundBox.getSize().y * 0.95f, 0.0f);
	mEmitterOffset = Ogre::Vector3(0.0f, 0.0f, -boundBox.getSize().y * 0.95f);
	mDidHitDragon = false;
}

Whacker::~Whacker()
{
	//We only remove OGRE objects that were created in the Whacker class.
	//The CollidableObject base class will take care of the rest
	Ogre::SceneManager* mSceneManager = Ogre::Root::getSingletonPtr()->getSceneManager("ApplicationSceneManager");
	mSceneManager->destroyEntity(mEntity->getName());
	mSceneManager->destroyParticleSystem(mParticleSystem->getName());
	mSceneNode->getParentSceneNode()->removeAndDestroyChild(mParticleNode->getName());
}

void Whacker::SetMovePlane(Ogre::Vector3 normal, Ogre::Vector3 origin, Ogre::Real width, Ogre::Real height)
{
	//The whacker moves along 2 dimensions on a defined plane. This function sets that
	//plane and sets the whacker to the center of it.
	normal.normalise();

	float normXDot = normal.dotProduct(Ogre::Vector3::UNIT_X);
	if(1.0f - Ogre::Math::Abs(normXDot) > 0.13f)
	{
		//The supplied normal is NOT colinear to the unit X axis. We can use
		//the X axis as a reference vector.
		towardAxis = Ogre::Vector3::UNIT_X.crossProduct(normal);
		towardAxis.normalise();
		rightAxis = normal.crossProduct(towardAxis);
		rightAxis.normalise();
	}
	else
	{
		//The supplied normal is colinear to the unit X axis (or close enough).
		//We should use the Y axis as a reference vector.
		towardAxis = normal.crossProduct(Ogre::Vector3::UNIT_Y);
		towardAxis.normalise();
		rightAxis = normal.crossProduct(towardAxis);
		rightAxis.normalise();
	}
	planeWidth = width;
	planeHeight = height;
	mSceneNode->setPosition(origin);
	curPlaneX = 0.0f;
	curPlaneY = 0.0f;
	mSceneNode->setOrientation(Ogre::Quaternion(rightAxis, normal, towardAxis));
}

void Whacker::Move(Ogre::Real delta_x, Ogre::Real delta_y)
{
	//We move the whacker along the defined movement plane my adding our base direction vectors.
	if((curPlaneX + delta_x) > -planeWidth && (curPlaneX + delta_x) < planeWidth && !isWhacking)
	{
		mSceneNode->translate(rightAxis * delta_x);
		curPlaneX += delta_x;
	}
	if((curPlaneY + delta_y) > -planeHeight && (curPlaneY + delta_y) < planeHeight && !isWhacking)
	{
		mSceneNode->translate(towardAxis * delta_y);
		curPlaneY += delta_y;
	}
}

bool Whacker::Whack()
{
	//Start to bring the hammer down
	//This function returns whether the Whacker
	//actually started whacking or not.
	if(!isWhacking)
	{
		targetPitch = -90.0f;
		isWhacking = true;
		return true;
	}
	else
	{
		return false;
	}
	
}

void Whacker::OnCollide(CollidableObject* other)
{
	mDidHitDragon = true;
}

void Whacker::Update(Ogre::Real timeSinceLastFrame)
{
	mDidHitDragon = false;
	if(isWhacking)
	{
		float pitchTimeStep;
		if(targetPitch == 0.0f)
		{
			//The whacker is currently bringing the hammer up to its starting position
			pitchTimeStep = 90.0f * timeSinceLastFrame * (whacksPerSecond * 2.0f); //whacksPerSecond is doubled since this is only half the swing
			if((pitchTimeStep) < (targetPitch - curPitch))
			{
				mSceneNode->pitch(Ogre::Degree(pitchTimeStep));
				curPitch += pitchTimeStep;
			}
			else
			{
				//We are close enough to our goal to just set the orientation to the upright pose
				mSceneNode->pitch(Ogre::Degree((targetPitch - curPitch)));
				curPitch = 0;
				isWhacking = false;
			}
			
		}
		else
		{
			//The whacker is currently bringing the hammer down to hit something
			pitchTimeStep = -90.0f * timeSinceLastFrame * (whacksPerSecond * 2.0f); //whacksPerSecond is doubled since this is only half the swing
			float particleDelayStep = -90.0f * 0.15f * (whacksPerSecond * 2.0f);
			if((pitchTimeStep) > (targetPitch - curPitch))
			{
				mSceneNode->pitch(Ogre::Degree(pitchTimeStep));
				curPitch += pitchTimeStep;
				if(curPitch + particleDelayStep <= targetPitch && !mParticleEmitter->getEnabled())
				{
					//Lets start emitting BEFORE the hammer collides
					mParticleNode->setPosition(mEmitterOffset + mSceneNode->getPosition());
					mParticleEmitter->setEnabled(true);
				}
			}
			else
			{
				//We are close enough to our goal to just set the orientation to the down pose
				mSceneNode->pitch(Ogre::Degree((targetPitch - curPitch)));
				curPitch = targetPitch;
				targetPitch = 0.0f;
			}
		}
	}
	CollidableObject::Update(timeSinceLastFrame);
}

bool Whacker::didHitDragon()
{
	return mDidHitDragon;
}