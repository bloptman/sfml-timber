#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	//Create video mode object.
	VideoMode vm(1920, 1080);

	// Create and open a window for the game.
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	Texture textureBackground;

	textureBackground.loadFromFile("assets/graphics/background.png");

	Sprite spriteBackground;

	spriteBackground.setTexture(textureBackground);

	spriteBackground.setPosition(0, 0);
	/*
	* ************************************
	* Handle the player's input.
	* ************************************
	*/
	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		/*
		****************************************
		* Update the scene.
		****************************************
		*/

		/*
		*****************************************
		* Draw the scene.
		*****************************************
		*/

		window.clear();

		window.draw(spriteBackground);

		// Draw our game scene here.

		// Show everything we just drew.

		window.display();
	}

	return 0;
}