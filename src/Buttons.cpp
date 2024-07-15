#include "Buttons.hpp"

Button::Button (double x_, double y_, double w_, double h_):
		x (x_),
		y (y_),
		w (w_),
		h (h_),
		state (btnNormal)
{}

Button::Button():
		x (0),
		y (0),
		w (100),
		h (100),
		state (btnNormal)
{}

void Button::SetPos (double x_, double y_) {
	x = x_;
	y = y_;
}

void Button::SetSize (double w_, double h_) {
	w = w_;
	h = h_;
}

void Button::SetState (double mouse_x, double mouse_y, bool mouse_pressed, double dt) {
	if (state == btnDisabled) return;

	if (MouseOnButton (mouse_x, mouse_y)) {
		if (mouse_pressed) {
			if (state == btnNormal || state == btnFocused) {
				state = btnPressed;
				Press();
			}
			else Pressed (dt);
		}
		else {
			if (state == btnNormal)
				state = btnFocused;
			else if (state == btnPressed) {
				state = btnFocused;
				Release();
			}
		}
	}
	else {
		if (state == btnFocused)
			state = btnNormal;
		else if (state == btnPressed) {
			state = btnNormal;
			Release();
		}
	}
}

bool Button::MouseOnButton (double mouse_x, double mouse_y) const {
	return (mouse_x >= x && mouse_x <= x + w && mouse_y >= y && mouse_y <= y + h);
}


TextButton::TextButton (double x_, double y_, double w_, double h_, const sf::Font& font, const char* str):
		Button (x_, y_, w_, h_),
		text (str, font)
{
	text.setFillColor (sf::Color::Black);
}

void TextButton::SetText (const char* str) {
	text.setString (str);
}

void TextButton::SetFont (const sf::Font& font) {
	text.setFont (font);
}

void TextButton::Draw (sf::RenderWindow& window) {
	sf::RectangleShape rect (sf::Vector2f (static_cast<float>(w), static_cast<float>(h)));
	rect.setFillColor (sf::Color (255 - state * 80, 255 - state * 80, 255 - state * 80));
	rect.setPosition (static_cast<float>(x), static_cast<float>(y));
	text.setPosition (static_cast<float>(x), static_cast<float>(y));

	window.draw (rect);
	window.draw (text);
}


ImageButton::ImageButton (double x_, double y_, double w_, double h_):
		Button (x_, y_, w_, h_),
		textures ()
{}

ImageButton::ImageButton():
		Button (),
		textures ()
{}

void ImageButton::SetTextures (const sf::Texture* t0, const sf::Texture* t1, const sf::Texture* t2, const sf::Texture* t3) {
	textures[0] = t0;
	textures[1] = t1;
	textures[2] = t2;
	textures[3] = t3;
}

void ImageButton::Draw (sf::RenderWindow& window) {
	sf::Sprite sprite;
	sf::Texture texture;

	if (textures[state] == nullptr) {
		sf::Image whiteImg;
		whiteImg.create (1, 1, sf::Color::White);
		texture.loadFromImage (whiteImg);
	}
	else texture = *(textures[state]);

	sprite.setTexture (texture);
	sprite.setScale (static_cast<float>(w / texture.getSize().x), static_cast<float>(h / texture.getSize().y));
	sprite.setPosition (static_cast<float>(x), static_cast<float>(y));
	window.draw (sprite);
}


ButtonManager::~ButtonManager() {
	for (auto & btn : btns) {
		delete btn;
	}
}

void ButtonManager::AddButton (Button *btn) {
	btns.push_back (btn);
}

void ButtonManager::DrawButtons (sf::RenderWindow& window) {
	for (auto & btn : btns) {
		btn -> Draw (window);
	}
}

void ButtonManager::SetStates (double mouse_x, double mouse_y, bool mouse_pressed, double dt) {
	for (auto & btn : btns) {
		btn -> SetState (mouse_x, mouse_y, mouse_pressed, dt);
	}
}