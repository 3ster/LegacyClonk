/* Copyright (C) 1998-2000  Matthes Bender  RedWolf Design */

/* Move liquids in the landscape using individual transport spots */

#pragma once

const int32_t C4MassMoverChunk = 10000;

class C4MassMoverSet;

class C4MassMover
{
	friend class C4MassMoverSet;

protected:
	int32_t Mat, x, y;

protected:
	void Cease();
	bool Execute();
	bool Init(int32_t tx, int32_t ty);
	bool Corrosion(int32_t dx, int32_t dy);
};

class C4MassMoverSet
{
public:
	C4MassMoverSet();
	~C4MassMoverSet();

public:
	int32_t Count;
	int32_t CreatePtr;

protected:
	C4MassMover Set[C4MassMoverChunk];

public:
	void Copy(C4MassMoverSet &rSet);
	void Synchronize();
	void Default();
	void Clear();
	void Execute();
	bool Create(int32_t x, int32_t y, bool fExecute = false);
	bool Load(C4Group &hGroup);
	bool Save(C4Group &hGroup);

protected:
	void Consolidate();
};