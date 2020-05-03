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
		Stone
	};
	Type type;
public:
	//Functions
	Block() = default;
	Block(Type t);
};