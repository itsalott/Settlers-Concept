// MakePatternsWork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <list>
#include <vector>
#include "World.h"
#include "Server.h"

int main()
{
	Server server = Server();
	World::Instance()->init();

	for(int i = 0; i < 30; ++i) {
		char c;
		std::cout << "\nPress enter to continue..." << std::endl;
		std::cin.get();

		World::Instance()->update();
	}

	//IWeapon *weapon = new Weapon();
	//std::cout << "Basic weapon has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	//weapon = new GunpowderUpgrade(weapon);
	//std::cout << "Weapon upgraded with gunpowder has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	//weapon = new BarrelUpgrade(weapon);
	//std::cout << "Weapon upgraded with barrel has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	//std::cout <<  "I'm a Settler of type " << settler.type << std::endl;
	//std::cout <<  "I'm a Settler of type " << settler2.type << std::endl;

    return 0;
}
