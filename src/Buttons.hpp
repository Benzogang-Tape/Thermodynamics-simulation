#ifndef THERMODYNAMICS_SIMULATION_BUTTONS_HPP
#define THERMODYNAMICS_SIMULATION_BUTTONS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>

const size_t TXTBTN_BUFSIZE = 128;

enum ButtonState {
	btnNormal,
	btnPressed,
	btnFocused,
	btnDisabled,
};


class Button {
public:
	double x;
	double y;
	double w;
	double h;
	ButtonState state;

	explicit Button (double x_, double y_, double w_, double h_);

	explicit Button();

	void SetPos (double x_, double y_);

	void SetSize (double w_, double h_);

	void SetState (double mouse_x, double mouse_y, bool mouse_pressed, double dt);

	[[nodiscard]] bool MouseOnButton (double mouse_x, double mouse_y) const;

	virtual void Press() = 0;

	virtual void Pressed(double dt) = 0;

	virtual void Release() = 0;

	virtual void Draw (sf::RenderWindow& window) = 0;
};


class TextButton : public Button {
	sf::Text text;

public:

	explicit TextButton (double x_, double y_, double w_, double h_, const sf::Font& font, const char* str = "");

	void SetText (const char* str);

	void SetFont (const sf::Font& font);

	void Draw (sf::RenderWindow& window) override;
};


class ImageButton : public Button {
private:
	const sf::Texture* textures[4];

public:
	explicit ImageButton (double x_, double y_, double w_, double h_);

	explicit ImageButton();

	void SetTextures (const sf::Texture* t0, const sf::Texture* t1, const sf::Texture* t2, const sf::Texture* t3);

	void Draw (sf::RenderWindow& window) override;
};

class ButtonManager {
private:
	std::vector<Button*> btns;

public:

	~ButtonManager();

	void AddButton (Button *btn);

	void DrawButtons (sf::RenderWindow& window);

	void SetStates (double mouse_x, double mouse_y, bool mouse_pressed, double dt);
};


#endif //THERMODYNAMICS_SIMULATION_BUTTONS_HPP
