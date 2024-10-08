#pragma once

struct cbuffer abstract
{
private:
	static inline int regIndex;
public:
	static int GetRegIndex() { return regIndex; }
};

struct vs_cbuffer : cbuffer
{
	vs_cbuffer();
	~vs_cbuffer();

private:

};

struct ps_cbuffer : cbuffer
{
	ps_cbuffer();
	~ps_cbuffer();

private:

};

struct shared_cbuffer : cbuffer
{
	shared_cbuffer();
	~shared_cbuffer();

private:

};
