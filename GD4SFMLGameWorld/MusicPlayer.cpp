//Antony Saputra D00197615
//Conor Byrne D00185566

#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer()
	: mMusic()
	, mFilenames()
	, mVolume(50.f)
{
	mFilenames[MusicID::MenuTheme] = "Media/Music/MenuTheme.ogg";
	mFilenames[MusicID::BattleTheme] = "Media/Music/BattleTheme.ogg";
}

void MusicPlayer::play(MusicID theme)
{
	std::string filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
	{
		throw std::runtime_error("Music " + filename + " could not be opened");
	}

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
	{
		mMusic.pause();
	}
	else {
		mMusic.play();
	}
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}
