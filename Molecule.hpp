//
// Created by DJ Tape on 05.05.2024.
//

#ifndef THERMODYNAMICS_SIMULATION_MOLECULE_HPP
#define THERMODYNAMICS_SIMULATION_MOLECULE_HPP

enum MoleculeTypes {
	moleculeCircle = 0,
	moleculeSquare = 1,
};

class Molecule {
public:
	unsigned int mass;
	double potential_energy;
	double status;
	double radius;
	const MoleculeTypes type;

};


#endif //THERMODYNAMICS_SIMULATION_MOLECULE_HPP
