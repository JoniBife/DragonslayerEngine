#include "Controller.h"
using namespace core;

Controller::Controller(const std::string& name) : Component(name + " (Controller)") { }

core::Controller::~Controller()
{
}

bool core::Controller::getType() const
{
	return false;
}

bool core::Controller::isSingular() const
{
	return false;
}
