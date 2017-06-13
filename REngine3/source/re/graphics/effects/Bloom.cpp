//
//  Bloom.cpp
//  REngine3
//
/*
#####################
SFML Game Development
License
#####################

This file describes the licenses for material accompanying the book "SFML Game Development".
Copyright(c) 2013 Artur Moreira, Henrik Vogelius Hansson, Jan Haller



========== =
Source code
========== =

This software is provided 'as-is', without any express or implied
warranty.In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
excluding commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. Redistributions may not be sold, nor may they be used in a commercial
product or activity.

4. This notice may not be removed or altered from any source distribution.
*/

// ALTERED SOURCE

#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/utility/ResourceManager.hpp"

#include "Bloom.hpp"

namespace re
{
	void BloomEffect::load()
	{
		
	}

	void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
	{
		prepareTextures(input.getSize());

		filterBright(input, mBrightnessTexture);

		downsample(mBrightnessTexture, mFirstPassTextures[0]);
		blurMultipass(mFirstPassTextures);

		downsample(mFirstPassTextures[0], mSecondPassTextures[0]);
		blurMultipass(mSecondPassTextures);

		add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
		mFirstPassTextures[1].display();
		add(input, mFirstPassTextures[1], output);
	}

	void BloomEffect::prepareTextures(sf::Vector2u size)
	{
		if (mBrightnessTexture.getSize() != size)
		{
			mBrightnessTexture.create(size.x, size.y);
			mBrightnessTexture.setSmooth(true);

			mFirstPassTextures[0].create(size.x / 2, size.y / 2);
			mFirstPassTextures[0].setSmooth(true);
			mFirstPassTextures[1].create(size.x / 2, size.y / 2);
			mFirstPassTextures[1].setSmooth(true);

			mSecondPassTextures[0].create(size.x / 4, size.y / 4);
			mSecondPassTextures[0].setSmooth(true);
			mSecondPassTextures[1].create(size.x / 4, size.y / 4);
			mSecondPassTextures[1].setSmooth(true);
		}
	}

	void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
	{
		sf::Shader& brightness = Locator::get<ResourceManager<sf::Shader>>()->get("BrightnessPass");

		brightness.setUniform("source", input.getTexture());
		applyShader(brightness, output);
		output.display();
	}

	void BloomEffect::blurMultipass(RenderTextureArray& renderTextures)
	{
		sf::Vector2u textureSize = renderTextures[0].getSize();

		for (std::size_t count = 0; count < 2; ++count)
		{
			blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
			blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
		}
	}

	void BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
	{
		sf::Shader& gaussianBlur = Locator::get<ResourceManager<sf::Shader>>()->get("GaussianBlurPass");

		gaussianBlur.setUniform("source", input.getTexture());
		gaussianBlur.setUniform("offsetFactor", offsetFactor);
		applyShader(gaussianBlur, output);
		output.display();
	}

	void BloomEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
	{
		sf::Shader& downSampler = Locator::get<ResourceManager<sf::Shader>>()->get("DownSamplePass");

		downSampler.setUniform("source", input.getTexture());
		downSampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
		applyShader(downSampler, output);
		output.display();
	}

	void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
	{
		sf::Shader& adder = Locator::get<ResourceManager<sf::Shader>>()->get("AddPass");

		adder.setUniform("source", source.getTexture());
		adder.setUniform("bloom", bloom.getTexture());
		applyShader(adder, output);
	}
}