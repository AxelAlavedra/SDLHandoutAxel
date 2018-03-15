#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "DummyESC.h"
#include "SDLGame.h"

#define NUM_MODULES 3

class Application
{
public:

	Module* modules[NUM_MODULES];

public:

	Application()
	{
		modules[0] = new ModuleDummy();
		modules[1] = new DummyESC();
		modules[2] = new SDLGame();
		// TODO 7: Create your new module "DummyESC"
		// it should check if player it ESC key use kbhit()
		// http://www.cprogramming.com/fod/kbhit.html
	}

	// INIT all modules
	bool Init() 
	{
		for (int i = 0; i < NUM_MODULES; ++i)
			if (!modules[i]->Init()) return false;
		
		// TODO 5: Make sure that if Init() / PreUpdate/Update/PostUpdate/CleanUP return
		// an exit code App exits correctly.
		return true; 
	}

	// TODO 4: Add PreUpdate and PostUpdate calls

	// UPDATE all modules
	// TODO 2: Make sure all modules receive its update
	update_status Update() {
		update_status status;

		for (int i = 0; i < NUM_MODULES; ++i) {
			status = modules[i]->PreUpdate();
			if (status != UPDATE_CONTINUE) return status;
		}

		for (int i = 0; i < NUM_MODULES; ++i) {
			status = modules[i]->Update();
			if (status != UPDATE_CONTINUE) return status;
		}

		for (int i = 0; i < NUM_MODULES; ++i) {
			status = modules[i]->PostUpdate();
			if (status != UPDATE_CONTINUE) return status;
		}
		return status;
	}

	// EXIT Update 
	// TODO 3: Make sure all modules have a chance to cleanup
	bool CleanUp()
	{
		for (int i = NUM_MODULES - 1; i >= 0; --i)
		{
			if (!modules[i]->CleanUp()) return false;
			delete modules[i];
		}
		return true;
	}

};

#endif // __APPLICATION_H__