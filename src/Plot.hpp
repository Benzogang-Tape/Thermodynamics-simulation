#ifndef THERMODYNAMICS_SIMULATION_PLOT_HPP
#define THERMODYNAMICS_SIMULATION_PLOT_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <deque>
#include <cstring>

const size_t PLOT_TXT_BUF_SIZE = 128;

struct Pair {
	double t;
	double f;
	explicit Pair (double t_, double f_):
			t (t_),
			f (f_)
	{}
};


class Plot {
private:
	double x;
	double y;
	double w;
	double h;

	double max_t;
	double max_f;
	double t_unit_size;
	double f_unit_size;

	double t0;
	double cur_t;

	sf::Color bg_color;
	sf::Color plot_color;
	sf::Color axis_color;
	sf::Color txt_color;
	sf::Font  font;
	char title[PLOT_TXT_BUF_SIZE];

	bool connect_points;

	std::deque<Pair> points;

public:

	Plot (double x_, double y_, double w_, double h_, double max_t_, double max_f_);

	Plot (double x_, double y_, double w_, double h_, double max_t_, double max_f_,
		  double t_unit_size_, double f_unit_size_);

	void Draw (sf::RenderWindow& window);

	void AddPoint (double val, double dt);

	void SetBGColor   (const sf::Color& clr);
	void SetPlotColor (const sf::Color& clr);
	void SetAxisColor (const sf::Color& clr);
	void SetTxtColor  (const sf::Color& clr);
	void SetFont      (const sf::Font&  fnt);
	void SetTitle     (const char* str);
};



#endif //THERMODYNAMICS_SIMULATION_PLOT_HPP
