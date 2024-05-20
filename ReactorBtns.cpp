#include "ReactorBtns.hpp"


ReactorBtn::ReactorBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
		ImageButton (x_, y_, w_, h_),
		rctr (rctr_)
{}


AddCircleBtn::AddCircleBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
		ReactorBtn (x_, y_, w_, h_, rctr_)
{}

void AddCircleBtn::Press() {
	for (size_t i = 0; i < 10; i++) rctr -> AddCircle (DEFAULT_VEL);
}


AddSquareBtn::AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
		ReactorBtn (x_, y_, w_, h_, rctr_)
{}

void AddSquareBtn::Press() {
	for (size_t i = 0; i < 10; i++) rctr -> AddSquare (DEFAULT_VEL);
}


HeatWallsBtn::HeatWallsBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double temp_change_v_):
		ReactorBtn (x_, y_, w_, h_, rctr_),
		temp_change_v (temp_change_v_)
{}

void HeatWallsBtn::Pressed (double dt) {
	rctr -> HeatWalls (dt * temp_change_v);
}


MovePistonBtn::MovePistonBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double vel_):
		ReactorBtn (x_, y_, w_, h_, rctr_),
		vel (vel_)
{}

void MovePistonBtn::Press() {
	rctr -> LockPiston();
}

void MovePistonBtn::Pressed (double dt) {
	rctr -> MovePiston (vel * dt);
}

void MovePistonBtn::Release() {
	rctr -> UnlockPiston();
}