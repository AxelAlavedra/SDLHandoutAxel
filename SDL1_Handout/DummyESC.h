#ifndef __DUMMYESC_H__
#define __DUMMYESC_H__

#include "Module.h"
#include "Globals.h"
#include <conio.h>

#define ESC 27

class DummyESC : public Module
{
	bool Init()
	{
		LOG("DummyESC Init!");
		return true;
	}

	update_status PreUpdate()
	{
		LOG("DummyESC PreUpdate!");
		if (_kbhit()) {
			char ch = _getch();
			if (ch == ESC) return update_status::UPDATE_STOP;
		}
		return update_status::UPDATE_CONTINUE;
	}

	update_status Update()
	{
		LOG("DummyESC Update!");
		return update_status::UPDATE_CONTINUE;
	}

	update_status PostUpdate()
	{
		LOG("DummyESC PostUpdate!");
		return update_status::UPDATE_CONTINUE;
	}

	bool CleanUp()
	{
		LOG("DummyESC CleanUp!");
		return true;
	}
};

#endif // __DUMMYESC_H__
