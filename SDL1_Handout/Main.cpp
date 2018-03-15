#include "Application.h"
#include "Globals.h"
#include "MemLeaks.h"

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char* argv[])
{
	ReportMemoryLeaks();

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = nullptr;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
			case MAIN_CREATION:
			{
				LOG("\nApplication Creation --------------");
				App = new Application();
				state = MAIN_START;
			}	break;

			case MAIN_START:
			{
				LOG("\nApplication Start --------------");
				if(App->Init() == false)
				{
					LOG("\nApplication Init exits with error -----");
					state = MAIN_EXIT;
				}
				else
					state = MAIN_UPDATE;

			}	break;

			case MAIN_UPDATE:
			{
				update_status status = App->Update();

				if (status == update_status::UPDATE_ERROR)
				{
					LOG("\nApplication Update exits with error -----");
					state = MAIN_EXIT;
				}
				else if (status == update_status::UPDATE_STOP)
					state = MAIN_FINISH;

			} break;

			case MAIN_FINISH: {
				if (App->CleanUp()) main_return = EXIT_SUCCESS;
				state = MAIN_EXIT;
			}
			// TODO 1: Implement case MAIN_FINISH
			// Remember to quit the game with EXIT_SUCCESS
			// if everything went well :)
		}
	}

	LOG("\nBye :)\n");
	delete App;
	// TODO 6: Remove ALL memory leaks
	return main_return;
}