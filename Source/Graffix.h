#pragma once

#include <JuceHeader.h>

class Graffix : public juce::LookAndFeel_V4
{
public:
	Graffix();
	~Graffix();

	enum UIColour
	{
		main1 = 0,
		main2,
		main3,
		accent1,
		accent2,
		defaultFill,
		accent1low,
		highlightedFill,
		menuText,

		numColours
	};

	juce::Colour getUIColour(UIColour colourToGet) const noexcept;

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;

	juce::Font getLabelFont(juce::Label& label) override;
	void drawLabel(juce::Graphics& g, juce::Label& label) override;

private:
	std::uint32_t mpallette[7] =
	{
		0xFFDFDDE3, //window background  -- grey 1
		0xFF9E9DA0, //widget background  -- grey 2
		0xFF59595B, //menu background    -- grey 3
		0xFFAAA7DE, //outline            -- accent 1
		0xFF573E92, //default text       -- accent 2
		0xFF777777,
		0xFF8785C4  //                   -- accent 1low
	};
};