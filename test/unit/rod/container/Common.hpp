#pragma once




class ComponentInterface
{};

class Component:
	public ComponentInterface
{
public:
	int
	method( int v )
	{
		return v;
	}
};


class Component2Interface
{};

class Component2:
	public Component2Interface
{};