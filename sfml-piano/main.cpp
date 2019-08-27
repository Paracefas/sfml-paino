#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <vector>
#include <ctime>

#define PI       3.14159265358979323846
#define TWOPI	 2.0 * PI

short sgn(double x) 
{
	if (x < 0) return -1;
	if (x == 0) return 0;
	if (x > 0) return 1;
}
double cotan(double x) 
{ 
	return 1 / tan(x); 
}
short sineWave(double t, double f, double amp)
{
	return amp * 32767 * sin((TWOPI * f * t) / 44100);
}
short squareWave(double t, double f, double amp)
{
	return amp * 32767 * sgn(sin((TWOPI * f * t) / 44100));
}
short sawToothWave(double t, double f, double amp)
{
	double dResult = 0.0;
	for (int n = 1; n < 100; ++n)
		dResult += (sin((n * f * TWOPI * t) / 44100)) / n;
	dResult *= (2 * amp * 32767) / PI;
	return dResult;
}

double getOctave(double dBase, int n)
{
	return dBase * pow(2, n);
}

struct Tile
{
	Tile(sf::RenderWindow& main, char const* spriteSrc, sf::Keyboard::Key code, short (*wave)(double, double, double), double dFrequency, double dAmp)
		: code{ code }, mainWindow{ main }, dFrequency{ dFrequency }, dPrevFreq{ dFrequency }, spriteSrc{ spriteSrc }, wave{ wave }, dAmp{ dAmp }
	{
		texture.loadFromFile(spriteSrc);
		sprite = sf::Sprite(texture);
		for (int i = 0; i < 44100; ++i)
		{
			notes.push_back(wave(i, dFrequency, dAmp));
		}
		buffer.loadFromSamples(&notes[0], notes.size(), 1, 44100);
	}
	void Check(sf::Event const& event)
	{
		if (dFrequency != dPrevFreq)
		{
			for (int i = 0; i < 44100; ++i)
			{
				notes[i] = (wave(i, dFrequency, dAmp));
			}
			buffer.loadFromSamples(&notes[0], notes.size(), 1, 44100);
			dPrevFreq = dFrequency;
		}
		if (event.key.code == code) 
		{
			Note.setBuffer(buffer);
			Note.play();
		}
		while (sf::Keyboard::isKeyPressed(code))
		{
			mainWindow.clear();
			mainWindow.draw(sprite);
			mainWindow.display();
		}
	}
	void UpdateOctave(int n)
	{
		dFrequency = getOctave(dFrequency, n);
	}
private:
	sf::Keyboard::Key code;
	sf::Sound Note;
	sf::SoundBuffer buffer;
	std::vector<sf::Int16> notes;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::RenderWindow& mainWindow;
	double dFrequency, dPrevFreq;
	double dAmp;
	char const* spriteSrc;
	short (*wave)(double, double, double);
};

int main()
{
	sf::SoundBuffer buffer;
	buffer.loadFromFile("sound.wav");

	sf::RenderWindow window(sf::VideoMode(534, 446), "Keyboard");

	sf::Texture texture;
	if (!texture.loadFromFile("img/keyboard.png")) return -1;
	sf::Sprite bg{ texture };

	Tile keyboard[] = {
		{ window, "img/h.png", sf::Keyboard::H, sineWave, 440.0, 1.0 },
		{ window, "img/u.png", sf::Keyboard::U, sineWave, 466.16, 1.0 },
		{ window, "img/j.png", sf::Keyboard::J, sineWave, 493.88, 1.0 },
		{ window, "img/a.png", sf::Keyboard::A, sineWave, 523.25, 1.0 },
		{ window, "img/w.png", sf::Keyboard::W, sineWave, 554.37, 1.0 },
		{ window, "img/s.png", sf::Keyboard::S, sineWave, 587.33, 1.0 },
		{ window, "img/e.png", sf::Keyboard::E, sineWave, 622.25, 1.0 },
		{ window, "img/d.png", sf::Keyboard::D, sineWave, 659.25, 1.0 },
		{ window, "img/f.png", sf::Keyboard::F, sineWave, 698.46, 1.0 },
		{ window, "img/t.png", sf::Keyboard::T, sineWave, 739.99, 1.0 },
		{ window, "img/g.png", sf::Keyboard::G, sineWave, 783.99, 1.0 },
		{ window, "img/y.png", sf::Keyboard::Y, sineWave, 830.61, 1.0 },
		{ window, "img/k.png", sf::Keyboard::K, sineWave, getOctave(523.25, 1), 1.0 }
	};

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
					for (Tile& tile : keyboard)
						tile.UpdateOctave(1);
				else if (event.key.code == sf::Keyboard::Down)
					for (Tile& tile : keyboard)
						tile.UpdateOctave(-1);
				else for (Tile& tile : keyboard)
					tile.Check(event);
			}
		}
		window.draw(bg);
		window.display();
	}
	
	return 0;
}