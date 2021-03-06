//Antony Saputra D00197615
//Conor Byrne D00185566

#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "MusicID.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

class MusicPlayer : private sf::NonCopyable
{
public:
	MusicPlayer();
	void play(MusicID theme);
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);

private:
	sf::Music mMusic;
	std::map<MusicID, std::string> mFilenames;
	float mVolume;
};