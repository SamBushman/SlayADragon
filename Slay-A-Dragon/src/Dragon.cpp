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
}

void Dragon::SetPosition(Ogre::Vector3 pos)
{
	mSceneNode->setPosition(pos);
}

bool Dragon::Popup()
{
	return true;
}

void Dragon::OnCollide(CollidableObject* other)
{

}

void Dragon::Update(Ogre::Real timeSinceLastFrame)
{

}

void Dragon::OnMiss()
{

}