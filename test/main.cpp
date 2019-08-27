#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>

int main(int argc, char* argv[]) {
	sf::Clock clock;
	Sleep(1000);
	sf::Time elapsed = clock.getElapsedTime();
	std::cout << elapsed.asMilliseconds();
	std::cin.get();
}