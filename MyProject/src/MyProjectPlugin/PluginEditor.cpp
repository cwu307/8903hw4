/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceDemoPluginAudioProcessorEditor::JuceDemoPluginAudioProcessorEditor (JuceDemoPluginAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      //midiKeyboard (ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard),
      infoLabel (String::empty),
      title("","vibrato"),
      gainLabel ("", "Width:"),
      delayLabel ("", "Delay:"),
      ModFreqLabel("","ModFreqency:")
{
    // add some sliders..
    addAndMakeVisible (gainSlider);
    gainSlider.setSliderStyle (Slider::Rotary);
    gainSlider.addListener (this);
    gainSlider.setRange (0.0, 1.0, 0.01);

    addAndMakeVisible (delaySlider);
    delaySlider.setSliderStyle (Slider::Rotary);
    delaySlider.addListener (this);
    delaySlider.setRange (0.0, 1.0, 0.01);

    addAndMakeVisible (ModFreqSlider);
    ModFreqSlider.setSliderStyle (Slider::Rotary);
    ModFreqSlider.addListener (this);
    ModFreqSlider.setRange (0.0, 10.0, 0.1);
    
    addAndMakeVisible(bypassButton);
    bypassButton.setButtonText("ByPass");
    bypassButton.addListener(this);
    bypass = false;
    
    // add some labels for the sliders..
    gainLabel.attachToComponent (&gainSlider, true);
    gainLabel.setFont (Font (11.0f));

    delayLabel.attachToComponent (&delaySlider, true);
    delayLabel.setFont (Font (11.0f));
    
    ModFreqLabel.attachToComponent(&ModFreqSlider, true);

    // add a label that will display the current timecode and status..
    addAndMakeVisible (infoLabel);
    infoLabel.setColour (Label::textColourId, Colours::blue);
    
    addAndMakeVisible(title);
    title.setColour(Label::textColourId, Colours::blue);
    title.setFont(Font(12.0f));

    // add the triangular resizer component for the bottom-right of the UI
    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
    resizeLimits.setSizeLimits (150, 150, 800, 300);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (ownerFilter->lastUIWidth,
             ownerFilter->lastUIHeight);

    startTimer (50);
}

JuceDemoPluginAudioProcessorEditor::~JuceDemoPluginAudioProcessorEditor()
{
}

void JuceDemoPluginAudioProcessorEditor::buttonClicked (Button* button)
{
    if (button == &bypassButton) {
        if (bypass) {
            getProcessor()->setParameterNotifyingHost(JuceDemoPluginAudioProcessor::buttonPara,
                                                1.0F);
            bypass = false;
        }
        else {
            getProcessor()->setParameterNotifyingHost(JuceDemoPluginAudioProcessor::buttonPara,
                                                      0.0F);
            bypass = true;
        }
        
    }
}

//==============================================================================
void JuceDemoPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.setGradientFill (ColourGradient (Colours::white, 0, 0,
                                       Colours::grey, 0, (float) getHeight(), false));
    g.fillAll();
}

void JuceDemoPluginAudioProcessorEditor::resized()
{
    infoLabel.setBounds (10, 4, 400, 25);
    gainSlider.setBounds (40, 60, 140, 40);
    delaySlider.setBounds (220, 60, 140, 40);
    ModFreqSlider.setBounds(100, 120, 150, 40);
    bypassButton.setBounds(250, 120, 40, 20);
    title.setBounds(150, 20, 50, 35);


    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);

    getProcessor()->lastUIWidth = getWidth();
    getProcessor()->lastUIHeight = getHeight();
}

//==============================================================================
// This timer periodically checks whether any of the filter's parameters have changed...
void JuceDemoPluginAudioProcessorEditor::timerCallback()
{
    JuceDemoPluginAudioProcessor* ourProcessor = getProcessor();

    gainSlider.setValue (ourProcessor->gain, sendNotificationSync);
    delaySlider.setValue (ourProcessor->delay, sendNotificationSync);
    ModFreqSlider.setValue(ourProcessor->Modfreq,sendNotificationSync);
    
}

// This is our Slider::Listener callback, when the user drags a slider.
void JuceDemoPluginAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &gainSlider)
    {
        // It's vital to use setParameterNotifyingHost to change any parameters that are automatable
        // by the host, rather than just modifying them directly, otherwise the host won't know
        // that they've changed.
        getProcessor()->setParameterNotifyingHost (JuceDemoPluginAudioProcessor::gainParam,
                                                   (float) gainSlider.getValue());
    }
    else if (slider == &delaySlider)
    {
        getProcessor()->setParameterNotifyingHost (JuceDemoPluginAudioProcessor::delayParam,
                                                   (float) delaySlider.getValue());
    }
    else if (slider == &ModFreqSlider)
    {
        getProcessor()->setParameterNotifyingHost(JuceDemoPluginAudioProcessor::ModFreqParam, (float)ModFreqSlider.getValue());
    }
}

//==============================================================================
// quick-and-dirty function to format a timecode string
static const String timeToTimecodeString (const double seconds)
{
    const double absSecs = fabs (seconds);

    const int hours =  (int) (absSecs / (60.0 * 60.0));
    const int mins  = ((int) (absSecs / 60.0)) % 60;
    const int secs  = ((int) absSecs) % 60;

    String s (seconds < 0 ? "-" : "");

    s << String (hours).paddedLeft ('0', 2) << ":"
      << String (mins) .paddedLeft ('0', 2) << ":"
      << String (secs) .paddedLeft ('0', 2) << ":"
      << String (roundToInt (absSecs * 1000) % 1000).paddedLeft ('0', 3);

    return s;
}

// quick-and-dirty function to format a bars/beats string
static const String ppqToBarsBeatsString (double ppq, double /*lastBarPPQ*/, int numerator, int denominator)
{
    if (numerator == 0 || denominator == 0)
        return "1|1|0";

    const int ppqPerBar = (numerator * 4 / denominator);
    const double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * numerator;

    const int bar    = ((int) ppq) / ppqPerBar + 1;
    const int beat   = ((int) beats) + 1;
    const int ticks  = ((int) (fmod (beats, 1.0) * 960.0 + 0.5));

    String s;
    s << bar << '|' << beat << '|' << ticks;
    return s;
}

// Updates the text in our position label.
void JuceDemoPluginAudioProcessorEditor::displayPositionInfo (const AudioPlayHead::CurrentPositionInfo& pos)
{
    lastDisplayedPosition = pos;
    String displayText;
    displayText.preallocateBytes (128);

    displayText << String (pos.bpm, 2) << " bpm, "
                << pos.timeSigNumerator << '/' << pos.timeSigDenominator
                << "  -  " << timeToTimecodeString (pos.timeInSeconds)
                << "  -  " << ppqToBarsBeatsString (pos.ppqPosition, pos.ppqPositionOfLastBarStart,
                                                    pos.timeSigNumerator, pos.timeSigDenominator);

    if (pos.isRecording)
        displayText << "  (recording)";
    else if (pos.isPlaying)
        displayText << "  (playing)";

    infoLabel.setText (displayText, dontSendNotification);
}
