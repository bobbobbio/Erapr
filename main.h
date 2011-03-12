/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#import <Allegro/Allegro.h>
#include "Sprite.h"
#include "data1.h"
#include "map editor/mapdata.h"

#ifdef _WIN32
#include "win32.h"
#else
#include "macosx.h"
#endif

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (int)(sizeof((x)) / sizeof((x)[0]))
#endif

