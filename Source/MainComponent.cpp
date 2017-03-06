/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
    physicscontainer = new PhysicsContainer();
    physicscontainer->setSize(display.userArea.getWidth(), display.userArea.getHeight());
    addAndMakeVisible(physicscontainer);
    resized();
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
    setSize (display.userArea.getWidth(), display.userArea.getHeight());
    physicscontainer->setSize(this->getWidth(), this->getHeight());
}