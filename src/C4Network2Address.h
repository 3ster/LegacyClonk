/*
 * LegacyClonk
 *
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de/
 * Copyright (c) 2013-2016, The OpenClonk Team and contributors
 * Copyright (c) 2019, The LegacyClonk Team and contributors
 *
 * Distributed under the terms of the ISC license; see accompanying file
 * "COPYING" for details.
 *
 * "Clonk" is a registered trademark of Matthes Bender, used with permission.
 * See accompanying file "TRADEMARK" for details.
 *
 * To redistribute this file separately, substitute the full license texts
 * for the above references.
 */

#pragma once

#include "C4Network2IO.h"

class C4Network2Address
{
public:
	C4Network2Address()
			: eProtocol(P_NONE)
	{ std::memset(&addr, 0, sizeof(addr)); }

	C4Network2Address(C4NetIO::addr_t addr, C4Network2IOProtocol eProtocol)
			: addr(addr), eProtocol(eProtocol)
	{ }

	C4Network2Address(const C4Network2Address &addr)
			: addr(addr.getAddr()), eProtocol(addr.getProtocol())
	{ }

	void operator= (const C4Network2Address &addr)
	{ SetAddr(addr.getAddr()); SetProtocol(addr.getProtocol()); }

	bool operator== (const C4Network2Address &addr) const;

protected:
	C4NetIO::addr_t addr;
	C4Network2IOProtocol eProtocol;

public:
	const C4NetIO::addr_t &getAddr() const { return addr; }
	in_addr               getIPAddr() const { return addr.sin_addr; }
	bool                  isIPNull() const { return !addr.sin_addr.s_addr; }
	uint16_t              getPort() const { return htons(addr.sin_port); }
	C4Network2IOProtocol  getProtocol() const { return eProtocol; }

	StdStrBuf toString() const;

	void SetAddr(C4NetIO::addr_t naddr) { addr = naddr; }
	void SetIP(in_addr ip) { addr.sin_addr = ip; }
	void SetPort(uint16_t iPort) { addr.sin_port = htons(iPort); }
	void SetProtocol(C4Network2IOProtocol enProtocol) { eProtocol = enProtocol; }

	void CompileFunc(StdCompiler *pComp);
};
