/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_8D3F9C171D6898DC__
#define __JUCE_HEADER_8D3F9C171D6898DC__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "RigidBody.h"
#include "OpenGLDrawer/OpenGLDrawer.h"
#include "OpenGLDrawer/Renderer/ScreenRenderer.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PhysicsContainer  : public OpenGLAppComponent
{
public:
    //==============================================================================
    PhysicsContainer ();
    ~PhysicsContainer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void initialise() override;
    void render() override;
    void shutdown() override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel) override;
    bool keyPressed (const KeyPress& key) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void drawGraph(Graphics& g, String name, int x, int y, int w, int h, int num, float* v);
    OwnedArray<BaseObject> Bodys;
    ScopedPointer<ScreenRenderer> screenRenderer;
    Array<float> v;
    Array<float> a;
    Array<float> t;
    float startAzi;
    float startElv;

    float startYaw;
    float startPch;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhysicsContainer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_8D3F9C171D6898DC__
