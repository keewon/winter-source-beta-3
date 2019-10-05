#include"Sound.h"

LPDIRECTSOUND Sound::DirectSound = NULL;
LPDIRECTSOUNDBUFFER Sound::DirectSoundBuffer = NULL;
LPDIRECTSOUND3DLISTENER Sound::DirectSound3DListener= NULL;

bool
Sound::
init(HWND hWnd)
{
	HRESULT hResult;
	DSBUFFERDESC DSBufferDesc;
	DS3DLISTENER DS3DListener;
	WAVEFORMATEX WaveFormatEx;

	hResult = DirectSoundCreate(NULL, & DirectSound, NULL);

	if (hResult != DS_OK)
	{
		MessageBox(hWnd, "DrectSoundCreate 에러", "Direct Sound", MB_OK);
		return false;
	}

	hResult = DirectSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL);

	if (hResult != DS_OK)
	{
		MessageBox(hWnd, "SetCooperativeLevel 에러", "Direct Sound", MB_OK);
		return false;
	}

	memset(&DSBufferDesc, 0, sizeof(DSBUFFERDESC));
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	//DSBufferDesc.dwBufferBytes = 0;
	//DSBufferDesc.lpwfxFormat = NULL;

	hResult = DirectSound->CreateSoundBuffer(& DSBufferDesc, & DirectSoundBuffer, NULL); 
	
	if (hResult != DS_OK)
	{
		MessageBox(hWnd, "CreateSoundBuffer 에러", "Direct Sound", MB_OK);
		return false;
	}

	hResult = DirectSoundBuffer->QueryInterface(IID_IDirectSound3DListener, (void * *) & DirectSound3DListener);

	if (hResult != DS_OK)
	{
		MessageBox(hWnd, "QueryInterface 에러", "Direct Sound", MB_OK);
		return false;
	}

	DS3DListener.dwSize = sizeof(DS3DLISTENER);
	DirectSound3DListener->GetAllParameters(& DS3DListener);

	memset(& WaveFormatEx, 0, sizeof(WAVEFORMATEX));
    WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
    WaveFormatEx.nChannels = 2;
    WaveFormatEx.nSamplesPerSec = 44100;
    WaveFormatEx.wBitsPerSample  = 16; 
    WaveFormatEx.nBlockAlign = WaveFormatEx.wBitsPerSample / 8 * WaveFormatEx.nChannels;
    WaveFormatEx.nAvgBytesPerSec = WaveFormatEx.nSamplesPerSec * WaveFormatEx.nBlockAlign;

    DirectSoundBuffer->SetFormat(& WaveFormatEx);

	return true;
}

// 프로그램의 종료시 생성된 DirectSound 객체들을 해제시킨다.
void
Sound::
end(void)
{
	if (DirectSound != NULL)
	{
		if (DirectSoundBuffer != NULL)
		{
			DirectSoundBuffer->Release();
			DirectSoundBuffer = NULL;
		}

		if (DirectSound3DListener != NULL)
		{
			DirectSound3DListener->Release();
			DirectSound3DListener = NULL;
		}

		DirectSound->Release();
		DirectSound = NULL;
	}
}

void
Sound::
SetListenerPosition(const D3DVALUE x, const D3DVALUE y, const D3DVALUE z, const DWORD Apply)
{
	DirectSound3DListener->SetPosition(x, y, z, Apply);
}

void 
Sound::
SetListenerVelocity(const D3DVALUE x, const D3DVALUE y, const D3DVALUE z, const DWORD Apply)
{
	DirectSound3DListener->SetVelocity(x, y, z, Apply);
}

void
Sound::
SetListenerOrient(const D3DVALUE xFront, const D3DVALUE yFront, const D3DVALUE zFront, const D3DVALUE xTop, const D3DVALUE yTop, const D3DVALUE zTop, const DWORD Apply)
{
	DirectSound3DListener->SetOrientation(xFront, yFront, zFront, xTop, yTop, zTop, Apply);
}

void
Sound::
GetListenerPosition(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z)
{
	D3DVECTOR D3DVector;

	DirectSound3DListener->GetPosition(& D3DVector);

	x = D3DVector.x;
	y = D3DVector.y;
	z = D3DVector.z;
}

void
Sound::
GetListenerVelocity(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z)
{
	D3DVECTOR D3DVector;

	DirectSound3DListener->GetVelocity(& D3DVector);

	x = D3DVector.x;
	y = D3DVector.y;
	z = D3DVector.z;
}

void
Sound::
GetListenerOrient(D3DVALUE & xFront, D3DVALUE & yFront, D3DVALUE & zFront, D3DVALUE & xTop, D3DVALUE & yTop, D3DVALUE & zTop)
{
	D3DVECTOR Front, Top;

	DirectSound3DListener->GetOrientation(& Front, & Top);

	xFront = Front.x;
	yFront = Front.y;
	zFront = Front.z;
	xTop = Top.x;
	yTop = Top.y;
	zTop = Top.z;
}

HRESULT 
Sound::
CommitDeferredSettings(void)
{
	return DirectSound3DListener->CommitDeferredSettings();
}

int
Sound::
ReadWave(char * FileName, WAVEFORMATEX & WaveFormatEx)
{
	MMRESULT mmResult;
	MMCKINFO mmCkRiff;
	MMCKINFO mmCkChunk;
	HMMIO hMmio;
	int returnValue;

	hMmio = mmioOpen(FileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

	if (hMmio == NULL)
		return -1;

	mmCkRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmResult = mmioDescend(hMmio, & mmCkRiff, NULL, MMIO_FINDRIFF);

	if (mmResult != MMSYSERR_NOERROR)
		return -1;

	mmCkChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmResult = mmioDescend(hMmio, & mmCkChunk, & mmCkRiff, MMIO_FINDCHUNK);
	
	if (mmResult != MMSYSERR_NOERROR)
		return -1;

	returnValue = mmioRead(hMmio, (char *) & WaveFormatEx, sizeof(WAVEFORMATEX));

	if (returnValue == -1)
		return -1;

	mmResult = mmioAscend(hMmio, & mmCkChunk, 0);

	if (mmResult != MMSYSERR_NOERROR)
		return -1;

	mmCkChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = mmioDescend(hMmio, & mmCkChunk, & mmCkRiff, MMIO_FINDCHUNK);

	if (mmResult != MMSYSERR_NOERROR)
		return -1;

	returnValue = mmCkChunk.cksize;
	WaveData = new char[returnValue];

	if (mmioRead(hMmio, WaveData, returnValue) == -1) 
		return -1;

	mmioClose(hMmio, 0);

	return returnValue;
}

Sound::
Sound(void): buffer(NULL), buffer3D(NULL), WaveData(0)
{
}

Sound::
Sound(char * FileName): buffer(NULL), buffer3D(NULL), WaveData(0)
{
	update(FileName);
}

Sound::
~Sound(void)
{
	if (buffer != NULL)
	{
		if (buffer3D != NULL)
		{
			buffer3D->Release();
			buffer3D = NULL;
		}
		buffer->Release();
		buffer = NULL;
	}
	if (WaveData != 0)
	{
		delete [] WaveData;
		WaveData = 0;
	}
}

void Sound::
update(char * FileName)
{
	HRESULT hResult;
	DSBUFFERDESC DSBufferDesc;
	DS3DBUFFER DS3DBuffer;
	WAVEFORMATEX WaveFormatEx;
	LPVOID AudioPtr1, AudioPtr2;
	DWORD AudioBytes1, AudioBytes2;

	if (buffer != NULL)
	{
		if (buffer3D != NULL)
		{
			buffer3D->Release();
			buffer3D = NULL;
		}
		buffer->Release();
		buffer = NULL;
	}
	if (WaveData != 0)
	{
		delete [] WaveData;
		WaveData = 0;
	}

	int BufferSize = ReadWave(FileName, WaveFormatEx);

	memset(& DSBufferDesc, 0, sizeof(DSBUFFERDESC));
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
	DSBufferDesc.dwBufferBytes = BufferSize;
	DSBufferDesc.lpwfxFormat = & WaveFormatEx;
//	DSBufferDesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;

	hResult = DirectSound->CreateSoundBuffer(& DSBufferDesc, & buffer, NULL);

	if (hResult != DS_OK)
		return;

	hResult = buffer->Lock(0, BufferSize, & AudioPtr1, & AudioBytes1, & AudioPtr2, & AudioBytes2, DSBLOCK_ENTIREBUFFER);

	if (hResult != DS_OK)
		return;

	memcpy(AudioPtr1, WaveData, BufferSize);
	buffer->Unlock(AudioPtr1, AudioBytes1, AudioPtr2, AudioBytes2);

	hResult = buffer->QueryInterface(IID_IDirectSound3DBuffer, (void * *) & buffer3D); 
	if (hResult != DS_OK)
		return;

    DS3DBuffer.dwSize = sizeof(DS3DBUFFER);
	buffer3D->GetAllParameters(& DS3DBuffer);

    // Set new 3D buffer parameters
	DS3DBuffer.dwMode = DS3DMODE_HEADRELATIVE;
    buffer3D->SetAllParameters(& DS3DBuffer, DS3D_IMMEDIATE);
}

void
Sound::
Play(DWORD Flags)
{
	buffer->SetCurrentPosition(0);
	buffer->Play(0, 0, Flags);
}

void
Sound::
Stop(void)
{
	buffer->Stop();
}

void
Sound::
SetVolume(const long volume)
{
	buffer->SetVolume(volume);
}

void
Sound::
GetVolume(long & volume)
{
	buffer->GetVolume(& volume);
}

void
Sound::
SetPosition(const D3DVALUE x, const D3DVALUE y, const D3DVALUE z, const DWORD Apply)
{
	buffer3D->SetPosition(x, y, z, Apply);
}

void
Sound::
SetVelocity(const D3DVALUE x, const D3DVALUE y, const D3DVALUE z, const DWORD Apply)
{
	buffer3D->SetVelocity(x, y, z, Apply);
}

void
Sound::
GetPosition(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z)
{
	D3DVECTOR D3DVector;

	buffer3D->GetPosition(& D3DVector);
	x = D3DVector.x;
	y = D3DVector.y;
	z = D3DVector.z;
}

void
Sound::
GetVelocity(D3DVALUE & x, D3DVALUE & y, D3DVALUE & z)
{
	D3DVECTOR D3DVector;

	buffer3D->GetVelocity(& D3DVector);
	x = D3DVector.x;
	y = D3DVector.y;
	z = D3DVector.z;
}

void
Sound::
SetConeAngle(const DWORD InsideConeAngle, const DWORD OutsideConeAngle, const DWORD Apply)
{
	buffer3D->SetConeAngles(InsideConeAngle, OutsideConeAngle, Apply);
}

void
Sound::
SetConeOrientation(const D3DVECTOR orientation, const DWORD Apply)
{
	buffer3D->SetConeOrientation(orientation.x, orientation.y, orientation.z, Apply);
}

void
Sound::
GetConeAngle(DWORD & InsideConeAngle, DWORD & OutsideConeAngle)
{
	buffer3D->GetConeAngles(& InsideConeAngle, & OutsideConeAngle);
}

void
Sound::
GetConeOrientation(D3DVECTOR & orientation)
{
	buffer3D->GetConeOrientation(& orientation);
}


void
Sound::
SetMinDistance(const D3DVALUE MinDistance, const DWORD Apply)
{
	buffer3D->SetMinDistance(MinDistance, Apply);
}

void
Sound::
SetMaxDistance(const D3DVALUE MaxDistance, const DWORD Apply)
{
	buffer3D->SetMaxDistance(MaxDistance, Apply);
}

void
Sound::
GetMinDistance(D3DVALUE & MinDistance)
{
	buffer3D->GetMinDistance(& MinDistance);
}

void
Sound::
GetMaxDistance(D3DVALUE & MaxDistance)
{
	buffer3D->GetMaxDistance(& MaxDistance);
}