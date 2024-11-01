#include "BloomEffect.h"
BloomEffect::BloomEffect()
	:mShaders()
	, mBrightnessTexture()
	, mSecondPassTexture()
	, mFirstPassTexture()
{
	mShaders.load(Shaders::BrightnessPass, "data/Fullpass.vert","data/Brightness.frag");
	mShaders.load(Shaders::DownSamplePass,"data/Fullpass.vert","data/DownSample.frag");
	mShaders.load(Shaders::GuassianBlurPass, "data/Fullpass.vert","data/GuassianBlur.frag");
	mShaders.load(Shaders::AddPass, "data/Fullpass.vert", "data/Add.frag");
	sf::Shader& a = mShaders.get(Shaders::BrightnessPass);
}
void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	prepareTextures(input.getSize());


	filterBright(input, mBrightnessTexture);

	downsample(mBrightnessTexture, mFirstPassTexture[0]);
	blurMultipass(mFirstPassTexture);

	downsample(mFirstPassTexture[0], mSecondPassTexture[0]);
	blurMultipass(mSecondPassTexture);

	add(mFirstPassTexture[0], mSecondPassTexture[0], mFirstPassTexture[1]);
	mFirstPassTexture[1].display();
	add(input, mFirstPassTexture[1], output);
}

void::BloomEffect::prepareTextures(sf::Vector2u size)
{
	if (mBrightnessTexture.getSize() != size)
	{
		mBrightnessTexture.create(size.x, size.y);
		mBrightnessTexture.setSmooth(true);

		mFirstPassTexture[0].create(size.x / 2, size.y / 2);
		mFirstPassTexture[0].setSmooth(true);
		mFirstPassTexture[1].create(size.x / 2, size.y / 2);
		mFirstPassTexture[1].setSmooth(true);

		mSecondPassTexture[0].create(size.x / 4, size.y / 4);
		mSecondPassTexture[0].setSmooth(true);
		mSecondPassTexture[1].create(size.x / 4, size.y / 4);
		mSecondPassTexture[1].setSmooth(true);

	}
}
void::BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	sf::Shader& brightness = mShaders.get(Shaders::BrightnessPass);
	brightness.setUniform("source", input.getTexture());
	applyShader(brightness, output);
	output.display();
}

void::BloomEffect::blurMultipass(RenderTextureArray& renderTextures)
{
	sf::Vector2u textureSize = renderTextures[0].getSize();
	for (std::size_t count = 0; count < 2; count++)
	{
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f( 1.f / textureSize.x,0.f));
	}
}
void::BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
	sf::Shader& gaussianBlur = mShaders.get(Shaders::GuassianBlurPass);

	gaussianBlur.setUniform("source", input.getTexture());
	gaussianBlur.setUniform("offsetFactor", offsetFactor);
	applyShader(gaussianBlur, output);
	output.display();
}
void::BloomEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	sf::Shader& downsampler = mShaders.get(Shaders::DownSamplePass);

	downsampler.setUniform("source", input.getTexture());
	downsampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
	applyShader(downsampler, output);
	output.display();
}
void::BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
	sf::Shader& adder = mShaders.get(Shaders::AddPass);
	adder.setUniform("source", source.getTexture());
	adder.setUniform("bloom", bloom.getTexture());
	applyShader(adder, output);
}