/* Implemention of NewGfx - without gfx */

#pragma once

#include <StdDDraw2.h>

class CStdNoGfx : public CStdDDraw
{
public:
	CStdNoGfx();
	virtual ~CStdNoGfx();
	virtual bool CreateDirectDraw();

public:
	virtual bool PageFlip(RECT *pSrcRt = nullptr, RECT *pDstRt = nullptr, CStdWindow *pWindow = nullptr) { return true; }
	virtual bool BeginScene() { return true; }
	virtual void EndScene() {}
	virtual int GetEngine() { return GFXENGN_NOGFX; }
	virtual void TaskOut() {}
	virtual void TaskIn() {}
	virtual bool UpdateClipper() { return true; }
	virtual bool OnResolutionChanged() { return true; }
	virtual bool PrepareRendering(CSurface *) { return true; }
	virtual void FillBG(uint32_t dwClr = 0) {}
	virtual void PerformBlt(CBltData &, CTexRef *, uint32_t, bool, bool) {}
	virtual void DrawLineDw(CSurface *, float, float, float, float, uint32_t) {}
	virtual void DrawQuadDw(CSurface *, int *, uint32_t, uint32_t, uint32_t, uint32_t) {}
	virtual void DrawPixInt(CSurface *, float, float, uint32_t) {}
	virtual bool ApplyGammaRamp(CGammaControl &, bool) { return true; }
	virtual bool SaveDefaultGammaRamp(CStdWindow *) { return true; }
	virtual bool StoreStateBlock() { return true; }
	virtual void SetTexture() {}
	virtual void ResetTexture() {}
	virtual bool RestoreStateBlock() { return true; }
	virtual bool InitDeviceObjects() { return true; }
	virtual bool RestoreDeviceObjects() { return true; }
	virtual bool InvalidateDeviceObjects() { return true; }
	virtual bool DeleteDeviceObjects() { return true; }
	virtual bool DeviceReady() { return true; }
	virtual bool CreatePrimarySurfaces(bool, unsigned int);
	virtual bool SetOutputAdapter(unsigned int) { return true; }
};