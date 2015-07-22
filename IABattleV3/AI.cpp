#include "stdafx.h"

//Parenthesis overloading for applying the AI on the unit provided in parameter
//Return the action which have to be done by the unit in this context
std::unique_ptr<Action> AI::operator()(Unit& unit, Army& allies, Army& opponents)
{
	return unit.getIAtree()->getAction(unit, allies, opponents);
}
