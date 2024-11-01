#ifndef POSTEFFECT_H
#define POSTEFFECT_H
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
namespace
{
	class RenderTarget;
	class RenderTexture;
	class Shader;
}
class PostEffect : sf::NonCopyable
{
public:
	virtual ~PostEffect();
	virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;
	static bool isSupported();
protected:
	static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);

};
#endif // !POSTEFFECT_H

