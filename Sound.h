#ifndef _Sound_
#define _Sound_

#include <dsound.h>

class Sound
{
private:
public:
	static LPDIRECTSOUND DirectSound;
	static LPDIRECTSOUNDBUFFER DirectSoundBuffer;
	static LPDIRECTSOUND3DLISTENER DirectSound3DListener;

	LPDIRECTSOUNDBUFFER buffer;
	LPDIRECTSOUND3DBUFFER buffer3D;
	char * WaveData;

	int ReadWave(char * FileName, WAVEFORMATEX & WaveFormatEx);

public:
	static bool init(HWND hWnd); // Direct Sound initialize
	static void end(void); // Direct Sound release
	static void SetListenerPosition(const D3DVALUE x = 0, const D3DVALUE y = 0, const D3DVALUE z = 0, const DWORD Apply = DS3D_IMMEDIATE);
	static void SetListenerVelocity(const D3DVALUE x, const D3DVALUE y, const D3DVALUE z, const DWORD Apply = DS3D_IMMEDIATE);
	static void SetListenerOrient(const D3DVALUE xFront = 0, const D3DVALUE yFront = 0, const D3DVALUE zFront = 0, const D3DVALUE xTop = 0, const D3DVALUE yTop = 0, const D3DVALUE zTop = 0, const DWORD Apply = DS3D_IMMEDIATE);
	static void GetListenerPosition(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z);
	static void GetListenerVelocity(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z);
	static void GetListenerOrient(D3DVALUE & xFront, D3DVALUE & yFront, D3DVALUE & zFront, D3DVALUE & xTop, D3DVALUE & yTop, D3DVALUE & zTop);
	static HRESULT CommitDeferredSettings(void);

	Sound(void);
	Sound(char * FileName);
	~Sound(void);
	void update(char * FileName);
	void Play(DWORD Flags = 0);
	void Stop(void);
	void SetVolume(const long volume);
	void GetVolume(long & volume);
	void SetPosition(const D3DVALUE x, const D3DVALUE y, const D3DVALUE z, const DWORD Apply = DS3D_IMMEDIATE);
	void SetVelocity(const D3DVALUE x = 0, const D3DVALUE y = 0, const D3DVALUE z = 0, const DWORD Apply = DS3D_IMMEDIATE);
	void GetPosition(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z);
	void GetVelocity(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z);
	void SetConeAngle(const DWORD InsideConeAngle = 360, const DWORD OutsideConeAngle = 360, const DWORD Apply = DS3D_IMMEDIATE);
	void SetConeOrientation(const D3DVECTOR orientation, const DWORD Apply = DS3D_IMMEDIATE);
	void GetConeAngle(DWORD & InsideConeAngle, DWORD & OutsideConeAngle);
	void GetConeOrientation(D3DVECTOR & orientation);
	void SetMinDistance(const D3DVALUE MinDistance, const DWORD Apply = DS3D_IMMEDIATE);
	void SetMaxDistance(const D3DVALUE MaxDistance, const DWORD Apply = DS3D_IMMEDIATE);
	void GetMinDistance(D3DVALUE & MinDistance);
	void GetMaxDistance(D3DVALUE & MaxDistance);
};

#endif