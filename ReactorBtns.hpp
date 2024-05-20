#ifndef THERMODYNAMICS_SIMULATION_REACTORBTNS_HPP
#define THERMODYNAMICS_SIMULATION_REACTORBTNS_HPP

#include "Reactor.hpp"
#include "Buttons.hpp"

const double DEFAULT_VEL{ 400 };

class ReactorBtn : public ImageButton {
public:
	Reactor* rctr;

	explicit ReactorBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);
};

class AddCircleBtn : public ReactorBtn {
public:
	explicit AddCircleBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);

	void Press() override;

	void Pressed (double dt) override {}

	void Release() override {}
};


class AddSquareBtn : public ReactorBtn {
public:
	explicit AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);

	void Press() override;

	void Pressed (double dt) override {}

	void Release() override {}
};


class HeatWallsBtn : public ReactorBtn {
private:
	double temp_change_v;

public:
	explicit HeatWallsBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double temp_change_v_);

	void Press() override {}

	void Pressed (double dt) override;

	void Release() override {};
};


class MovePistonBtn : public ReactorBtn {
private:
	double vel;

public:
	explicit MovePistonBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double vel_);

	void Press() override;

	void Pressed (double dt) override;

	void Release() override;
};


#endif //THERMODYNAMICS_SIMULATION_REACTORBTNS_HPP
