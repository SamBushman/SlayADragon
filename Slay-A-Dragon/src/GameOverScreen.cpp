#include <GameOverScreen.h>

GameoverScreen::GameoverScreen(unsigned int score)
{
	//Lets set up our scoring overlay (Lots of uninteresting code :/ )
	//Fonts are loaded due to a resource loading bug in OGRE 1.8 http://www.ogre3d.org/forums/viewtopic.php?f=2&t=63652
	Ogre::FontManager::getSingleton().load( "YeOldShire", "General" );
	Ogre::FontManager::getSingleton().load( "DejaVuSans", "General" );
	//Title overlays
	Ogre::OverlayManager* overlayMgr = Ogre::OverlayManager::getSingletonPtr();
	mOverlay = overlayMgr->create("GameOver");
	Ogre::OverlayContainer* newContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "GameOverContainer"));
	newContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    newContainer->setDimensions(0.4f, 0.15f);
    newContainer->setPosition(0.5f, 0.5f);
	newContainer->setHorizontalAlignment(Ogre::GuiHorizontalAlignment::GHA_LEFT);
	Ogre::TextAreaOverlayElement* newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "GameOverText"));
	newElement->setCaption("Game Over");
	newElement->setFontName("YeOldShire");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
	newElement->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_TOP);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.028f);
	newContainer->addChild(newElement);
	newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "GameOverScoreText"));
	newElement->setCaption("Score");
	newElement->setFontName("YeOldShire");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Right);
	newElement->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_CENTER);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.028f);
	newContainer->addChild(newElement);
	newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "PlayAgainText"));
	newElement->setCaption("Press ENTER to play again");
	newElement->setFontName("YeOldShire");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
	newElement->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_BOTTOM);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.028f);
	newContainer->addChild(newElement);
	mOverlay->add2D(newContainer);

	newContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "GameOverScoreValueContainer"));
	newContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    newContainer->setDimensions(0.4f, 0.15f);
    newContainer->setPosition(0.5f, 0.5f);
	newContainer->setHorizontalAlignment(Ogre::GuiHorizontalAlignment::GHA_LEFT);
	newContainer->setColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 0.3f));
	newElement = static_cast<Ogre::TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea", "GameOverScoreValueText"));
	Ogre::String num_str = Ogre::StringConverter::toString(score);
	newElement->setCaption(":"+num_str);
	newElement->setFontName("DejaVuSans");
	newElement->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	newElement->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Left);
	newElement->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_CENTER);
	newElement->setMetricsMode(Ogre::GMM_RELATIVE);
    newElement->setPosition(0, 0);
    newElement->setDimensions(0.4f, 0.4f);
    newElement->setCharHeight(0.045f);
	newContainer->addChild(newElement);
	mOverlay->add2D(newContainer);

	mOverlay->show();
}

GameoverScreen::~GameoverScreen()
{
	//Remove our overlay
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

void GameoverScreen::OnKeyPressed(const OIS::KeyEvent &arg)
{
	if(arg.key == OIS::KC_RETURN)
	{
		ScreenManager::GetInstance()->SetScreen(new GameplayScreen());
		ScreenManager::GetInstance()->RegisterForRemoval(this);
	}
}

void GameoverScreen::OnKeyReleased(const OIS::KeyEvent &arg)
{

}

void GameoverScreen::OnMouseMoved(const OIS::MouseEvent &arg)
{

}

void GameoverScreen::OnMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void GameoverScreen::OnMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void GameoverScreen::Update(Ogre::Real timeSinceLastFrame)
{

}