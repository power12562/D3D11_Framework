#pragma once

struct cbuffer abstract
{
protected:
	static inline int regIndex{-1};
public:
	static int GetRegIndex() { return regIndex; }
};

struct vs_cbuffer : cbuffer
{
	vs_cbuffer();
	~vs_cbuffer();
};

struct ps_cbuffer : cbuffer
{
	ps_cbuffer();
	~ps_cbuffer();
};

struct shared_cbuffer : cbuffer
{
	shared_cbuffer();
	~shared_cbuffer();
};
