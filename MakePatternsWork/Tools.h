#ifndef TOOL_H
#define TOOL_H
#include <iostream>

class ITool {
public:
	virtual void useTool() {}
};

class ToolImp : public ITool {
public:
	ToolImp() { }

	virtual void useTool() override {
		std::cout << "I don't do anything" << std::endl;
	}
};

class WeaponImp : public ToolImp {
public:
	WeaponImp() { }

	void useTool() override {
		std::cout << "Attacked" << std::endl;
	}
};

class AxeImp : public ToolImp {
public:
	AxeImp() { }

	void useTool() override {
		std::cout << "Harvested wood" << std::endl;
	}
};

class FishingNetImp : public ToolImp {
public:
	FishingNetImp() { }

	void useTool() override {
		std::cout << "Catched fish" << std::endl;
	}
};

class IWeapon : ITool {
public:
	virtual int getFireRange() { return 0; }
	virtual int getPreciseness() { return 0; }
};

class Tool : public ITool {
public:
	Tool() { imp = new ToolImp(); }
	~Tool() {}

	void useTool() override {
		imp->useTool();
	}

protected:
	ToolImp* imp;
};

class Weapon : public Tool, public IWeapon {
public:
	Weapon() { imp = new WeaponImp(); }
	~Weapon() {}

	int getFireRange() override { return fireRange; }
	int getPreciseness() override { return preciseness; }

protected:
	int fireRange = 5;
	int preciseness = 10;
};

class Axe : public Tool {
public:
	Axe() { imp = new AxeImp(); }
	~Axe() {}
};

class FishingNet : public Tool {
public:
	FishingNet() { imp = new FishingNetImp(); }
	~FishingNet() {}
};

class WeaponUpgrade : public IWeapon {
public:
	WeaponUpgrade(IWeapon *baseWeapon) : weapon(baseWeapon) {}
	~WeaponUpgrade() {}

	int getFireRange() override {
		return weapon->getFireRange();
	}

	int getPreciseness() override {
		return weapon->getPreciseness();
	}

protected:
	IWeapon* weapon;
};

class GunpowderUpgrade : public WeaponUpgrade {
public:
	GunpowderUpgrade(IWeapon *baseWeapon) : WeaponUpgrade(baseWeapon) {}
	~GunpowderUpgrade() {}

	int getFireRange() override {
		//std::cout << weapon.getFireRange() << std::endl;
		return WeaponUpgrade::getFireRange() + 10;
	}
};

class BarrelUpgrade : public WeaponUpgrade {
public:
	BarrelUpgrade(IWeapon *baseWeapon) : WeaponUpgrade(baseWeapon) {}
	~BarrelUpgrade() {}

	int getPreciseness() override {
		return WeaponUpgrade::getPreciseness() + 8;
	}
};
#endif
