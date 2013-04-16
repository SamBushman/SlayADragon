#include <Whacker.h>

Whacker::Whacker()
	: CollidableObject(NULL)
{
	//A default constructor to facilitate copy initialization
}

Whacker::Whacker(Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node)
	: CollidableObject(parent_node)
{
	mSceneNode = parent_node->createChildSceneNode("WhackerNode_"+parent_node->getName());
	Ogre::Entity* hammerMesh = scene_manager->createEntity("Whacker"+parent_node->getName(), "Hammer.mesh");
	mSceneNode->attachObject(hammerMesh);
	Ogre::ParticleSystem* particleSystem = scene_manager->createParticleSystem("Whacker", "SlayADragon/Whack");
	mParticleNode = parent_node->createChildSceneNode("ParticleNode");
	mParticleNode->attachObject(particleSystem);
	mParticleEmitter = (Ogre::PointEmitter*)particleSystem->getEmitter(0);
	mParticleEmitter->setDuration(0.3f);
	mParticleEmitter->setEnabled(false);
	isWhacking = false;
	targetPitch = 0.0f;
	curPitch = 0.0f;
	whacksPerSecond = 2.0f;
	scene_manager->_updateSceneGraph(scene_manager->getCamera("PlayerCam"));
	Ogre::AxisAlignedBox boundBox = mSceneNode->_getWorldAABB();
	SetCollideShapeBox(boundBox.getSize().x, boundBox.getSize().y * 0.1f, boundBox.getSize().z);
	geomOffset = Ogre::Vector3(0.0f, boundBox.getSize().y * 0.95f, 0.0f);
	mEmitterOffset = Ogre::Vector3(0.0f, 0.0f, -boundBox.getSize().y * 0.95f);
	mDidHitDragon = false;
}

Whacker::Whacker(Whacker& other)
	: CollidableObject(NULL)
{
	mParticleEmitter = other.mParticleEmitter;
	mParticleNode = other.mParticleNode;
	mEmitterOffset = other.mEmitterOffset;
	isWhacking = other.isWhacking;
	targetPitch = other.targetPitch;
	curPitch = other.curPitch;
	whacksPerSecond = other.whacksPerSecond;
	mDidHitDragon = other.mDidHitDragon;
	CollidableObject::CollidableObject(other);
}

Whacker& Whacker::operator=( const Whacker& rhs ) {
	mParticleEmitter = rhs.mParticleEmitter;
	mParticleNode = rhs.mParticleNode;
	mEmitterOffset = rhs.mEmitterOffset;
	isWhacking = rhs.isWhacking;
	targetPitch = rhs.targetPitch;
	curPitch = rhs.curPitch;
	whacksPerSecond = rhs.whacksPerSecond;
	mDidHitDragon = rhs.mDidHitDragon;
	CollidableObject::operator=(rhs);
	return *this;
}

void Whacker::SetMovePlane(Ogre::Vector3 normal, Ogre::Vector3 origin, Ogre::Real width, Ogre::Real height)
{
	normal.normalise();

	float normXDot = normal.dotProduct(Ogre::Vector3::UNIT_X);
	if(1.0f - Ogre::Math::Abs(normXDot) > 0.13f)
	{
		//The supplied normal is NOT colinear to the unit X axis
		towardAxis = Ogre::Vector3::UNIT_X.crossProduct(normal);
		towardAxis.normalise();
		rightAxis = normal.crossProduct(towardAxis);
		rightAxis.normalise();
	}
	else
	{
		//The supplied normal is colinear to the unit X axis (or close enough)
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
			pitchTimeStep = 90.0f * timeSinceLastFrame * (whacksPerSecond * 2.0f); //whacksPerSecond is doubled since this is only half the swing
			if((pitchTimeStep) < (targetPitch - curPitch))
			{
				mSceneNode->pitch(Ogre::Degree(pitchTimeStep));
				curPitch += pitchTimeStep;
			}
			else
			{
				mSceneNode->pitch(Ogre::Degree((targetPitch - curPitch)));
				curPitch = 0;
				isWhacking = false;
			}
			
		}
		else
		{
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