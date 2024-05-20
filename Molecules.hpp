#ifndef THERMODYNAMICS_SIMULATION_MOLECULES_HPP
#define THERMODYNAMICS_SIMULATION_MOLECULES_HPP

#include "Vec.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>


const sf::Color CIRCLE_MOL_COLOR{ sf::Color::Red };
const sf::Color SQUARE_MOL_COLOR{ sf::Color::Yellow };

const double BASE_MOL_RADIUS = 5;
const double MIN_REACTION_ENERGY{ 6000 };

const size_t BASE_GAS_CAPACITY{ 8 };

enum MoleculeTypes {
	moleculeCircle,
	moleculeSquare,
};

class Molecule {
public:
	Vec pos;
	Vec velocity;
	unsigned mass;
	double potentialEnergy;
	double status;
	double radius;
	const MoleculeTypes type;

	explicit Molecule (const Vec& pos_, const Vec& velocity_, unsigned int mass_,
					   MoleculeTypes type_, double status_);

	[[nodiscard]] double GetKineticEnergy() const;

	[[nodiscard]] double GetEnergy() const;

	[[nodiscard]] Vec GetMomentum() const;

	[[nodiscard]] bool CanReact() const;

	virtual void Draw (sf::RenderWindow& window) const = 0;

	void Move (double dt);

	void SetMass (unsigned int newMass);
};


class CircleMol : public Molecule {
public:
	explicit CircleMol (const Vec& pos_, const Vec& velocity_, unsigned int mass_, double status_ = 0);

	void Draw (sf::RenderWindow& window) const override;
};


class SquareMol : public Molecule {
public:
	explicit SquareMol (const Vec& pos_, const Vec& velocity_, unsigned int mass_, double status_ = 0);

	void Draw (sf::RenderWindow& window) const override;
};



class Gas {
public:
	Molecule** molecules;
	size_t size;
	size_t capacity;

	Gas();

	~Gas();

	void AddMolecule (Molecule *mol);

	void RemoveMolecule (size_t index);

	void MoveMolecules (double dt) const;

	void DrawMolecules (sf::RenderWindow& window) const;

	void CollideMolecules();

	void React (size_t index1, size_t index2);

	void ReactCircleCircle (size_t index1, size_t index2);

	void ReactCircleSquare (size_t index1, size_t index2);

	void ReactSquareCircle (size_t index1, size_t index2);

	void ReactSquareSquare (size_t index1, size_t index2);

	[[nodiscard]] double GetTemperature() const;

	[[nodiscard]] size_t GetNumOfCircles() const;

	[[nodiscard]] size_t GetNumOfSquares() const;
};


bool Intersect (Molecule* mol1, Molecule* mol2);

void ReflectMolecules (Molecule* mol1, Molecule* mol2);

void *Recalloc (void *memptr, size_t num, size_t size, size_t oldNum);


#endif //THERMODYNAMICS_SIMULATION_MOLECULES_HPP
