//
// Created by DJ Tape on 01.05.2024.
//

#include "Reactor.hpp"

//namespace Reactor {
//} // Reactor

Piston::Piston (double x_, double y_, double width_, double height_, double min_y_, double max_y_, int mass_, double vy_):
		x (x_),
		y (y_),
		width (width_),
		height (height_),
		min_y (min_y_),
		max_y (max_y_ - height_ - 4 * BASE_MOL_RADIUS),
		mass (mass_),
		vy (vy_),
		locked (false)
{}

void Piston::Fall (double dt) {
	if (locked) {
		vy = 0;
		return;
	}
	if (y > max_y) {
		y = max_y;
		if (vy > 0) vy = 0;
	}
	if (y < min_y) {
		y = min_y;
		if (vy < 0) vy = 0;
	}
	vy += GRAV_ACC * dt;
	y += vy * dt;
}

void Piston::Move (double dy) {
	y += dy;
	if (y > max_y) y = max_y;
	if (y < min_y) y = min_y;
}

void Piston::Draw (sf::RenderWindow& window) const {
	sf::RectangleShape rect (sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
	rect.setPosition (static_cast<float>(x), static_cast<float>(y));
	rect.setFillColor (PISTON_COLOR);
	window.draw (rect);
}

double Piston::ReflectMol (Molecule* mol) {
	double v1 = mol -> velocity.y;
	double v2 = vy;
	double k = mass / mol -> mass;

	vy = (2 * v1 + v2 * (k - 1)) / (k + 1);
	double new_v1 = vy + v2 - v1;

	mol -> velocity.y = new_v1;
	mol -> pos.y = y + height + mol -> radius;

	if (locked) vy = 0;

	return mass * fabs (v2 - vy);
}

void Piston::Lock() {
	locked = true;
}

void Piston::Unlock() {
	locked = false;
}

Reactor::Reactor (double min_x_, double min_y_, double max_x_, double max_y_, size_t num_of_molecules):
		min_x (min_x_),
		min_y (min_y_),
		max_x (max_x_),
		max_y (max_y_),
		walls_temp (0),
		pressure (0),
		pist (min_x_, min_y_, max_x_ - min_x_, PISTON_HEIGHT, min_y_, max_y_, PISTON_MASS),
		gas ()
{
	for (size_t i = 0; i < num_of_molecules; i++) {

		Vec pos (min_x + std::rand() / (RAND_MAX / (max_x - min_x)),
				 min_y + std::rand() / (RAND_MAX / (max_y - min_y)));

		Vec vel (std::rand() / (RAND_MAX / 50.), 0);
		vel.RotateAroundZ (GetRandAngle());

		Molecule* mol = nullptr;
		if (i % 2 == 0) mol = new CircleMol (pos, vel, 1);
		else            mol = new SquareMol (pos, vel, 1);
		gas.AddMolecule (mol);
	}
}

void Reactor::Proceed (double dt) {
	gas.MoveMolecules (dt);
	pist.Fall (dt);
	gas.CollideMolecules();
	ReflectOffWalls();
	ReflectOffPiston();
}

double Reactor::GetPressure() {
	double ret = pressure;
	pressure = 0;
	return ret / 1e6;
}

double Reactor::GetTemperature() const {
	return gas.GetTemperature();
}

size_t Reactor::GetNumOfCircles() const {
	return gas.GetNumOfCircles();
}

size_t Reactor::GetNumOfSquares() const {
	return gas.GetNumOfSquares();
}

void Reactor::ReflectOffWalls() {
	for (size_t i = 0; i < gas.size; i++) {
		Molecule* mol = gas.molecules[i];

		if (mol -> pos.x + mol -> radius >= max_x) {
			mol -> pos.x = max_x - mol -> radius;
			if (mol -> velocity.x > 0) {
				mol -> velocity.x = -mol -> velocity.x;
				pressure += 2 * mol -> mass * fabs(mol -> velocity.x);
				if (walls_temp != 0)
					mol -> velocity = !(mol -> velocity) * ((mol -> velocity).GetLen() + 5 * walls_temp);
			}
		}
		if (mol -> pos.x - mol -> radius <= min_x) {
			mol -> pos.x = min_x + mol -> radius;
			if (mol -> velocity.x < 0) {
				mol -> velocity.x = -mol -> velocity.x;
				pressure += 2 * mol -> mass * fabs(mol -> velocity.x);
				if (walls_temp != 0)
					mol -> velocity = !(mol -> velocity) * ((mol -> velocity).GetLen() + 5 * walls_temp);
			}
		}
		if (mol -> pos.y + mol -> radius >= max_y) {
			mol -> pos.y = max_y - mol -> radius;
			if (mol -> velocity.y > 0) {
				mol -> velocity.y = -mol -> velocity.y;
				pressure += 2 * mol -> mass * fabs(mol -> velocity.y);
				if (walls_temp != 0)
					mol -> velocity = !(mol -> velocity) * ((mol -> velocity).GetLen() + 5 * walls_temp);
			}
		}
		if (mol -> pos.y - mol -> radius <= min_y) {
			mol -> pos.y = min_y + mol -> radius;
			if (mol -> velocity.y < 0) {
				mol -> velocity.y = -mol -> velocity.y;
				pressure += 2 * mol -> mass * fabs(mol -> velocity.y);
				if (walls_temp != 0)
					mol -> velocity = !(mol -> velocity) * ((mol -> velocity).GetLen() + 5 * walls_temp);
			}
		}
	}
}

void Reactor::ReflectOffPiston() {
	for (size_t i = 0; i < gas.size; i++) {
		Molecule* mol = gas.molecules[i];
		if (mol -> pos.y - mol -> radius <= pist.y + pist.height)
			pressure += pist.ReflectMol (mol);
	}
}

void Reactor::Draw (sf::RenderWindow& window) const {
	DrawWalls (window);
	pist.Draw(window);
	gas.DrawMolecules(window);
}

void Reactor::DrawWalls (sf::RenderWindow& window) const {
	sf::RectangleShape box (sf::Vector2f(static_cast<float>(max_x - min_x), static_cast<float>(max_y - min_y)));
	box.setPosition (static_cast<float>(min_x), static_cast<float>(min_y));
	box.setOutlineColor (GetWallsColor());
	box.setFillColor (GetBgColor());
	box.setOutlineThickness (static_cast<float>(REACTOR_WALLS_THICKNESS));
	window.draw (box);
}

void Reactor::AddCircle (double vel) {
	Vec velocity (vel, 0);
	velocity.RotateAroundZ (GetRandAngle() / -4);
	Molecule* mol = new CircleMol (Vec (min_x, max_y), velocity, 1, 100);
	gas.AddMolecule (mol);
}

void Reactor::AddSquare (double vel) {
	Vec velocity (vel, 0);
	velocity.RotateAroundZ (GetRandAngle() / 4 + M_PI);
	Molecule* mol = new SquareMol (Vec (max_x, max_y), velocity, 1, 100);
	gas.AddMolecule (mol);
}

void Reactor::HeatWalls (double tempChange) {
	walls_temp += tempChange;
}

void Reactor::MovePiston (double dy) {
	pist.Move (dy);
}

sf::Color Reactor::GetWallsColor() const {
	if (walls_temp ==   0) return {255, 255, 255};
	if (walls_temp <= -16) return {  0, 127, 255};
	if (walls_temp >=  16) return {255, 127, 127};

	if (walls_temp < 0)
		return {static_cast<sf::Uint8>(255 + 16 * walls_temp),
				static_cast<sf::Uint8>(255 + 8 * walls_temp),
				255
		};
	else
		return {255,
				static_cast<sf::Uint8>(255 - 8 * walls_temp),
				static_cast<sf::Uint8>(255 - 8 * walls_temp)
		};
}

sf::Color Reactor::GetBgColor() const {
	if (walls_temp ==   0) return {192, 192, 192};
	if (walls_temp <= -16) return {  0,  96, 192};
	if (walls_temp >=  16) return {192,  96,  96};

	if (walls_temp < 0)
		return {static_cast<sf::Uint8>(192 + 12 * walls_temp),
				static_cast<sf::Uint8>(192 + 6 * walls_temp),
				192
		};
	else
		return {192,
				static_cast<sf::Uint8>(192 - 6 * walls_temp),
				static_cast<sf::Uint8>(192 - 6 * walls_temp)
		};
}

void Reactor::LockPiston() {
	pist.Lock();
}

void Reactor::UnlockPiston() {
	pist.Unlock();
}

void Reactor::SetWallsTemp (double temp) {
	walls_temp = temp;
}