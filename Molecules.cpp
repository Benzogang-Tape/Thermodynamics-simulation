#include "Molecules.hpp"

Molecule::Molecule (const Vec& pos_, const Vec& velocity_, unsigned int mass_, MoleculeTypes type_, double status_):
		pos (pos_),
		velocity (velocity_),
		mass (mass_),
		potentialEnergy (0),
		type (type_),
		status (status_),
		radius (BASE_MOL_RADIUS + mass_)
{}

double Molecule::GetKineticEnergy() const {
	return mass * (velocity, velocity);
}

double Molecule::GetEnergy() const {
	return GetKineticEnergy() + potentialEnergy;
}

Vec Molecule::GetMomentum() const {
	return mass * velocity;
}

bool Molecule::CanReact() const {
	return status == 0;
}

void Molecule::Move (double dt) {
	pos += velocity * dt;

	if (status > 0) {
		status -= velocity.GetLen() * dt;
		if (status < 0) status = 0;
	}
}

void Molecule::SetMass (unsigned newMass) {
	mass = newMass;
	radius = BASE_MOL_RADIUS + mass;
}


CircleMol::CircleMol (const Vec& pos_, const Vec& velocity_, unsigned mass_, double status_):
		Molecule (pos_, velocity_, mass_, moleculeCircle, status_)
{}

void CircleMol::Draw (sf::RenderWindow& window) const {
	sf::CircleShape circle (static_cast<float>(radius));
	circle.setPosition (static_cast<float>(pos.x - radius), static_cast<float>(pos.y - radius));
	circle.setFillColor (CIRCLE_MOL_COLOR);
	window.draw (circle);
}


SquareMol::SquareMol (const Vec& pos_, const Vec& velocity_, unsigned mass_, double status_):
		Molecule (pos_, velocity_, mass_, moleculeSquare, status_)
{}

void SquareMol::Draw (sf::RenderWindow& window) const {
	sf::RectangleShape square (sf::Vector2f(static_cast<float>(radius) * 2, static_cast<float>(radius) * 2));
	square.setPosition (static_cast<float>(pos.x - radius), static_cast<float>(pos.y - radius));
	square.setFillColor (SQUARE_MOL_COLOR);
	window.draw (square);
}

bool Intersect (Molecule* mol1, Molecule* mol2) {
	if ((mol1 -> pos - mol2 -> pos).GetLen() <= mol1 -> radius + mol2 -> radius)
		return true;
	return false;
}

Gas::Gas() {
	size = 0;
	capacity = BASE_GAS_CAPACITY;
	molecules = (Molecule**) calloc (capacity, sizeof(molecules[0]));
	assert (molecules != nullptr);
}

Gas::~Gas() {
	for (size_t i = 0; i < size; i++) {
		delete molecules[i];
	}
	free (molecules);
}

void Gas::AddMolecule (Molecule *mol) {
	if (size >= capacity) {
		molecules = (Molecule**) Recalloc (molecules, capacity * 2, sizeof(molecules[0]), capacity);
		assert (molecules != nullptr);
		capacity *= 2;
	}
	molecules [size++] = mol;
}

void Gas::RemoveMolecule (size_t index) {
	delete molecules[index];
	molecules[index] = molecules[--size];
}

void Gas::DrawMolecules (sf::RenderWindow& window) const {
	for (size_t i = 0; i < size; i++) {
		molecules[i] -> Draw (window);
	}
}

void Gas::MoveMolecules (double dt) const {
	for (size_t i = 0; i < size; i++) {
		molecules[i] -> Move (dt);
	}
}

void Gas::CollideMolecules() {
	for (size_t i = 0; i < size; i++) {
		if (!(molecules[i] -> CanReact()))
			continue;
		for (size_t j = i + 1; j < size; j++) {
			if (!(molecules[j] -> CanReact()))
				continue;
			if (Intersect (molecules[i], molecules[j])) {
				if (molecules[i] -> GetEnergy() + molecules[j] -> GetEnergy() >= MIN_REACTION_ENERGY) {
					React (i, j);
					if (!(molecules[i] -> CanReact()))
						break;
				}
				else{
					ReflectMolecules (molecules[i], molecules[j]);
				}
			}
		}
	}

	for (int i = size - 1; i >= 0; i--) {
		if (molecules[i] -> status < 0)
			RemoveMolecule (i);
	}
}

void Gas::React (size_t index1, size_t index2) {
	typedef void (Gas::*ReactFunc) (size_t index1, size_t index2);
	static const ReactFunc ReactFuncTable [2][2] =
			{{&Gas::ReactCircleCircle, &Gas::ReactCircleSquare},
			 {&Gas::ReactSquareCircle, &Gas::ReactSquareSquare}};

	MoleculeTypes type1 = molecules[index1] -> type;
	MoleculeTypes type2 = molecules[index2] -> type;

	assert (0 <= type1 && type1 <= 1);
	assert (0 <= type2 && type2 <= 1);

	ReactFunc react = ReactFuncTable[type1][type2];

	(this ->* react) (index1, index2);
}

void Gas::ReactCircleCircle (size_t index1, size_t index2) {
	Molecule* mol1 = molecules[index1];
	Molecule* mol2 = molecules[index2];

	Vec newPos = (mol1 -> pos + mol2 -> pos) / 2;
	unsigned newMass = mol1 -> mass + mol2 -> mass;

	Vec newVel = (mol1 -> GetMomentum() + mol2 -> GetMomentum()) / newMass;

	Molecule* newMol = new SquareMol (newPos, newVel, newMass);

	newMol -> potentialEnergy += mol1 -> GetKineticEnergy() + mol2 -> GetKineticEnergy() - newMass * (newVel, newVel);

	AddMolecule (newMol);
	mol1 -> status = -1;
	mol2 -> status = -1;
}

void Gas::ReactCircleSquare (size_t index1, size_t index2) {
	ReactSquareCircle (index2, index1);
}

void Gas::ReactSquareCircle (size_t index1, size_t index2) {
	Molecule* mol1 = molecules[index1];
	Molecule* mol2 = molecules[index2];

	unsigned newMass = mol1 -> mass + mol2 -> mass;

	Vec newVel = (mol1 -> GetMomentum() + mol2 -> GetMomentum()) / newMass;

	mol1 -> potentialEnergy += mol1 -> GetKineticEnergy() + mol2 -> GetKineticEnergy() - newMass * (newVel, newVel);

	mol1 -> velocity = newVel;
	mol1 -> SetMass (newMass);

	mol2 -> status = -1;
}

void Gas::ReactSquareSquare (size_t index1, size_t index2) {
	Molecule* mol1 = molecules[index1];
	Molecule* mol2 = molecules[index2];

	unsigned newMolCount = mol1 -> mass + mol2 -> mass;
	Vec pos = (mol1 -> pos + mol2 -> pos) / 2;

	Vec vel (std::sqrt((mol1 -> GetEnergy() + mol2 -> GetEnergy()) / newMolCount), 0);

	double angle = 2 * M_PI / newMolCount;
	vel.RotateAroundZ (GetRandAngle());

	for (int i = 0; i < newMolCount; i++) {
		Molecule *mol = new CircleMol (pos, vel, 1, 30 + newMolCount * (BASE_MOL_RADIUS + 1) / 3);
		AddMolecule (mol);
		vel.RotateAroundZ (angle);
	}

	mol1 -> status = -1;
	mol2 -> status = -1;
}

double Gas::GetTemperature() const {
	double energy = 0;
	for (size_t i = 0; i < size; i++)
		energy += molecules[i] -> GetKineticEnergy();

	return energy / static_cast<double>(size) / 1e6;
}

size_t Gas::GetNumOfCircles() const {
	size_t count = 0;
	for (size_t i = 0; i < size; i++)
		if (molecules[i] -> type == moleculeCircle) count++;
	return count;
}

size_t Gas::GetNumOfSquares() const {
	size_t count = 0;
	for (size_t i = 0; i < size; i++)
		if (molecules[i] -> type == moleculeSquare) count++;
	return count;
}

void ReflectMolecules (Molecule* mol1, Molecule* mol2) {
	Vec dif = mol1 -> pos - mol2 -> pos;
	dif.Normalize();

	double v1 = (mol1 -> velocity, dif);
	double v2 = (mol2 -> velocity, dif);
	double k = mol2 -> mass / mol1 -> mass;

	double new_v2 = (2 * v1 + v2 * (k - 1)) / (k + 1);
	double new_v1 = new_v2 + v2 - v1;

	mol1 -> velocity += (dif * (new_v1 - v1));
	mol2 -> velocity += (dif * (new_v2 - v2));
}

void *Recalloc (void *memptr, size_t num, size_t size, size_t oldNum) {
	memptr = realloc (memptr, num * size);
	if (memptr == nullptr) return nullptr;

	if (num > oldNum) memset ((void *) ((char *) memptr + oldNum * size), 0, (num - oldNum) * size);

	return memptr;
}