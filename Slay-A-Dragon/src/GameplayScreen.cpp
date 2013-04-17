#include "GameplayScreen.h"

GameplayScreen::GameplayScreen()
{
	//Set up the 3D scene
	Ogre::SceneManager* mSceneManager = Ogre::Root::getSingletonPtr()->getSceneManager("ApplicationSceneManager");
	Ogre::Entity* stageEntity = mSceneManager->createEntity("Stage", "Stage.mesh");
	mStageNode = mSceneManager->getRootSceneNode()->createChildSceneNode("StageNode");
	mStageNode->attachObject(stageEntity);
	mStageNode->setPosition(0.0f, -6.0f, -6.5f);

	//Set up game objects
	mWhacker = new Whacker(mSceneManager, mSceneManager->getRootSceneNode());
	mWhacker->SetMovePlane(Ogre::Vector3(0.0f, 2.0f, 1.0f), Ogre::Vector3(0.0f, -0.5f, -6.5f), 3.8f, 3.0f);
	float dragonPositions[] = { -3.0f, -2.5f, -7.0f,
								-1.5f, -2.0f, -8.0f,
								-1.5f, -3.0f, -6.0f,
								0.0f, -2.5f, -7.0f,
								1.5f, -2.0f, -8.0f,
								1.5f, -3.0f, -6.0f,
								3.0f, -2.5f, -7.0f };
	mDragons = std::vector<Dragon*>();
	for (int i=0; i < 7; i++)
	{
		mDragons.push_back(new Dragon(mSceneManager, mSceneManager->getRootSceneNode()));
		mDragons[i]->SetPosition(Ogre::Vector3(dragonPositions[i*3], dragonPositions[(i*3)+1], dragonPositions[(i*3)+2]));
	}

	//Set up game variables
	timeBetweenPopups = 1.0f;
	timeSinceLastPopup = 0.0f;
	score = 0;
	numPeasants = 200;
	subtractPopTime = 0.05f;
	scorePerPopTimeSub = 15;
	minPopTime = 0.75f;
	mWhackerMoveScalar = 0.01f;

	//Lets set up our scoring overlay (Lots of uninteresting code :/ )
	//Fonts are loaded due to a resource loading bug in OGRE 1.8 http://www.ogre3d.org/forums/viewtopic.php?f=2&t=63652
	Ogre::FontManager::getSingleton().load( "YeOldShire", "General" );
	Ogre::FontManager::getSingleton().load( "DejaVuSans", "General" );
	//Title overlays
	Ogre::OverlayManager* overlayMgr = Ogre::OverlayManager::getSingletonPtr();
	mOverlay = overlayMgr->create("GameStats");
	Ogre::OverlayContainer* newContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "StatsTitleContainer")); //Breaks Here
	newContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    newContainer->setDimensions(0.4f, 0.15f);
    newContainer->setPosition(0.5f, 0.75f);
	newContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
	Ogre::TextAreaOverlayElement* newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "ScoreTitleText"));
	newElement->setCaption("Score");
	newElement->setFontName("YeOldShire");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Right);
	newElement->setVerticalAlignment(Ogre::GVA_TOP);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.028f);
	newContainer->addChild(newElement);
	newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "PeasantsTitleText"));
	newElement->setCaption("Peasants Left");
	newElement->setFontName("YeOldShire");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Right);
	newElement->setVerticalAlignment(Ogre::GVA_BOTTOM);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.028f);
	newContainer->addChild(newElement);
	mOverlay->add2D(newContainer);
	//Value Overlays
	newContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "StatsValueContainer"));
	newContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    newContainer->setDimensions(0.4f, 0.15f);
    newContainer->setPosition(0.5f, 0.75f);
	newContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
	newContainer->setColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 0.3f));
	newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "ScoreValueText"));
	Ogre::String num_str = Ogre::StringConverter::toString(score);
	newElement->setCaption(":"+num_str);
	newElement->setFontName("DejaVuSans");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Left);
	newElement->setVerticalAlignment(Ogre::GVA_TOP);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.045f);
	mScoreText = newElement;
	newContainer->addChild(newElement);
	newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "PeasantsValueText"));
	num_str = Ogre::StringConverter::toString(numPeasants);
	newElement->setCaption(":"+num_str);
	newElement->setFontName("DejaVuSans");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Left);
	newElement->setVerticalAlignment(Ogre::GVA_BOTTOM);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.045f);
	mPeasantText = newElement;
	newContainer->addChild(newElement);
	mOverlay->add2D(newContainer);

	mOverlay->show();
}

GameplayScreen::~GameplayScreen()
{
	//Destroy our game objects
	delete mWhacker;
	for(unsigned int i=0; i<mDragons.size(); i++)
	{
		delete mDragons[i];
	}
	//Destroy all entities created
	Ogre::SceneManager* mSceneManager = Ogre::Root::getSingletonPtr()->getSceneManager("ApplicationSceneManager");
	mSceneManager->destroyEntity("Stage");
	//Remove the stage mesh
	mStageNode->removeAndDestroyAllChildren();
	mStageNode->getParentSceneNode()->removeAndDestroyChild(mStageNode->getName());
	//Destroy our score overlay
	//To do this, we must traverse all elements contained within
	//the overlay and remove them one at a time.
	Ogre::OverlayManager* overlayMgr = Ogre::OverlayManager::getSingletonPtr();
	Ogre::Overlay::Overlay2DElementsIterator iter = mOverlay->get2DElementsIterator();
	for(;iter.hasMoreElements();)
	{
		Ogre::OverlayContainer * childContainer = iter.getNext();
		Ogre::OverlayContainer::ChildIterator contIter = childContainer->getChildIterator();
		for(;contIter.hasMoreElements();)
		{
			Ogre::OverlayElement* curElement = contIter.getNext();
			overlayMgr->destroyOverlayElement(curElement->getName());
		}
		overlayMgr->destroyOverlayElement(childContainer->getName());
	}
	overlayMgr->destroy(mOverlay->getName());
}

void GameplayScreen::OnKeyPressed(const OIS::KeyEvent &arg)
{
	
}

void GameplayScreen::OnKeyReleased(const OIS::KeyEvent &arg)
{

}

void GameplayScreen::OnMouseMoved(const OIS::MouseEvent &arg)
{
	mWhacker->Move(arg.state.X.rel * mWhackerMoveScalar, arg.state.Y.rel * mWhackerMoveScalar);
}

void GameplayScreen::OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(arg.state.buttonDown(OIS::MB_Left))
	{
		mWhacker->Whack();
	}
}

void GameplayScreen::OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void GameplayScreen::Update(Ogre::Real timeSinceLastFrame)
{
	//Update game components
	mWhacker->Update(timeSinceLastFrame);
	for (unsigned int i=0; i < mDragons.size(); i++)
	{
		mDragons[i]->Update(timeSinceLastFrame);
		if(mDragons[i]->didBreathFire())
		{
			numPeasants -= 5;
			Ogre::String num_str = Ogre::StringConverter::toString(numPeasants);
			mPeasantText->setCaption(":"+num_str);
			if(numPeasants <= 0)
			{
				//Its game over!
				ScreenManager::GetInstance()->SetScreen(new GameoverScreen(score));
				ScreenManager::GetInstance()->RegisterForRemoval(this);
			}
		}
	}
	//Randomly pop dragons
	timeSinceLastPopup += timeSinceLastFrame;
	if(timeSinceLastPopup >= timeBetweenPopups)
	{
		//We randomly pick a dragon and try to pop it.
		//If it doesn't pop, we iterate through all the
		//other dragons until we pop one or run out of dragons
		int randDragon = std::rand() % 7;
		for(int i=0; i < 7; i++)
		{
			//Try to pop a dragon
			if(mDragons[randDragon]->Popup())
			{
				timeSinceLastPopup = 0.0f;
				break;
			}
			randDragon += 1;
			randDragon = randDragon % 7;
		}
	}
	PhysWorld::GetSingletonPtr()->Update(timeSinceLastFrame);
	//Now that collision checking has happened, lets see if we need to update our score
	if(mWhacker->didHitDragon())
	{
		score += 1;
		Ogre::String num_str = Ogre::StringConverter::toString(score);
		mScoreText->setCaption(":"+num_str);
		if(score % scorePerPopTimeSub == 0)
		{
			//Time to increase the rate of dragon pops a bit
			timeBetweenPopups -= subtractPopTime;
		}
	}
}