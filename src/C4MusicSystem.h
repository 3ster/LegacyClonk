/*
 * LegacyClonk
 *
 * Copyright (c) 1998-2000, Matthes Bender (RedWolf Design)
 * Copyright (c) 2017-2019, The LegacyClonk Team and contributors
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

/* Handles Music.c4g and randomly plays songs */

#pragma once

#include <C4Group.h>

class C4MusicFileInfoNode;
class C4MusicFile;

class C4MusicSystem
{
	friend class C4SoundEffect;
	friend class C4SoundInstance;
	friend class C4SoundSystem;

public:
	C4MusicSystem();
	~C4MusicSystem();
	void Clear();
	int SetVolume(int);
	void Execute();
	void NotifySuccess();
	bool Init(const char *PlayList = nullptr);
	bool InitForScenario(C4Group &hGroup);
	bool Play(const char *szSongname = nullptr, bool fLoop = false);
	bool Stop();
	void FadeOut(int fadeout_ms);

	int SetPlayList(const char *szPlayList);

	bool ToggleOnOff(); // keyboard callback

protected:
	// song list
	C4MusicFile *Songs;
	int SongCount, ASongCount, SCounter;

	// play
	C4MusicFile *PlayMusicFile;
	int Volume; bool Loop;

	void LoadDir(const char *szPath); // load some music files (by wildcard / directory)
	void Load(const char *szFile); // load a music file
	void LoadMoreMusic(); // load music file names from MoreMusic.txt
	void ClearSongs();

	bool GrpContainsMusic(C4Group &rGrp); // return whether this group contains music files

	// FMod / SDL_mixer
	bool MODInitialized;
	bool InitializeMOD();
	void DeinitializeMOD();
};

// helper stuff

enum MusicType { MUSICTYPE_MID, MUSICTYPE_MOD, MUSICTYPE_MP3, MUSICTYPE_OGG, MUSICTYPE_UNKNOWN };

class C4MusicFileInfoNode // We need this for the MoreMusic.txt stuff
{
public:
	C4MusicFileInfoNode() { next = nullptr; str = nullptr; };
	~C4MusicFileInfoNode() { delete[] str; }
	char *str;
	MusicType type;
	C4MusicFileInfoNode *next;
};

MusicType GetMusicFileTypeByExtension(const char *ext);
