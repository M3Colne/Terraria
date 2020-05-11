#pragma once

class Block
{
private:
	//Variables
private:
	//Functions
public:
	//Variables
	enum class Type
	{
		Air,
		Dirt,
		Grass,
		Stone,
		Tin,
		Copper,
		Iron,
		Lead,
		Gold,
		Platinum
	};
	Type type;
public:
	//Functions
	Block() = default;
	Block(Type t);
};