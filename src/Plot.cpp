#include "Plot.hpp"

Plot::Plot (double x_, double y_, double w_, double h_, double max_t_, double max_f_):
		x (x_),
		y (y_),
		w (w_),
		h (h_),
		max_t (max_t_),
		max_f (max_f_),
		t_unit_size (max_t_ / 2),
		f_unit_size (max_f_ / 2),
		bg_color   (sf::Color::Black),
		plot_color (sf::Color::Red),
		axis_color (sf::Color::Green),
		txt_color  (sf::Color::Green),
		t0 (0),
		cur_t (0),
		points ()
{}

Plot::Plot (double x_, double y_, double w_, double h_, double max_t_, double max_f_, double t_unit_size_, double f_unit_size_):
		x (x_),
		y (y_),
		w (w_),
		h (h_),
		max_t (max_t_),
		max_f (max_f_),
		t_unit_size (t_unit_size_),
		f_unit_size (f_unit_size_),
		bg_color   (sf::Color::Black),
		plot_color (sf::Color::Green),
		axis_color (sf::Color (64, 64, 64)),
		txt_color  (sf::Color (64, 64, 64)),
		t0 (0),
		cur_t (0),
		points ()
{}

void Plot::Draw (sf::RenderWindow& window) {
	sf::RectangleShape rect (sf::Vector2f (static_cast<float>(w), static_cast<float>(h)));
	rect.setPosition (static_cast<float>(x), static_cast<float>(y));
	rect.setFillColor (bg_color);
	window.draw (rect);

	rect.setFillColor (axis_color);
	rect.setSize (sf::Vector2f (static_cast<float>(w + 18), 6));
	rect.setPosition (static_cast<float>(x - 18), static_cast<float>(y + h));
	window.draw (rect);

	rect.setSize (sf::Vector2f (6, static_cast<float>(h + 18)));
	rect.setPosition (static_cast<float>(x - 6), static_cast<float>(y));
	window.draw (rect);

	char buf [PLOT_TXT_BUF_SIZE] = "";
	sf::Text txt (buf, font, 20);
	txt.setFillColor (txt_color);

	rect.setSize (sf::Vector2f (6, 15));
	for (double t = 0; t <= max_t; t += t_unit_size) {
		rect.setPosition (static_cast<float>(x - 6 + t / max_t * w), static_cast<float>(y + h));
		window.draw (rect);

		snprintf (buf, 127, "%.0lf", t0 + t <= cur_t ? t0 + t : t0 + t - max_t);
		txt.setString (buf);
		txt.setPosition (static_cast<float>(x - 12 + t / max_t * w), static_cast<float>(y + h + 15));
		window.draw (txt);
	}

	rect.setSize (sf::Vector2f (15, 6));
	for (double f = 0; f <= max_f; f += f_unit_size) {
		rect.setPosition (static_cast<float>(x - 15), static_cast<float>(y + h - f / max_f * h));
		window.draw (rect);

		snprintf (buf, 127, "%.0lf", f);
		txt.setString (buf);
		txt.setPosition (static_cast<float>(x) - 18 * static_cast<float>(strlen(buf) + 1), static_cast<float>(y + h - f / max_f * h - 12));
		window.draw (txt);
	}

	sf::CircleShape pnt (2);
	pnt.setFillColor (plot_color);
	for (auto & point : points) {
		double f = point.f;
		if (f > max_f) continue;
		double t = point.t - t0;
		if (t < 0) t += max_t;
		pnt.setPosition (static_cast<float>(x + w * (t / max_t) - 2), static_cast<float>(y + h * (1 - f / max_f) - 2));
		window.draw (pnt);
	}

	rect.setSize (sf::Vector2f (4, static_cast<float>(h)));
	rect.setFillColor (sf::Color::White);
	rect.setPosition (static_cast<float>(x + w * (cur_t - t0) / max_t), static_cast<float>(y));
	window.draw (rect);

	txt.setString (title);
	txt.setCharacterSize (30);
	txt.setPosition (static_cast<float>(x), static_cast<float>(y - 35));
	window.draw (txt);
}

void Plot::AddPoint (double val, double dt) {
	cur_t += dt;
	if (cur_t >= t0 + max_t) t0 += max_t;

	while (!points.empty() && points.front().t + max_t <= cur_t) points.pop_front();

	points.emplace_back(cur_t, val);
}

void Plot::SetBGColor (const sf::Color& clr) {
	bg_color = clr;
}

void Plot::SetPlotColor (const sf::Color& clr) {
	plot_color = clr;
}

void Plot::SetAxisColor (const sf::Color& clr) {
	axis_color = clr;
}

void Plot::SetTxtColor (const sf::Color& clr) {
	txt_color = clr;
}

void Plot::SetFont (const sf::Font& fnt) {
	font = fnt;
}

void Plot::SetTitle (const char* const str) {
	strncpy (title, str, PLOT_TXT_BUF_SIZE - 1);
}