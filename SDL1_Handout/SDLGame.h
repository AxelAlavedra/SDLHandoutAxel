#ifndef __SDLGAME_H__
#define __SDLGAMEC_H__

#include "Module.h"
#include "Globals.h"
#include "../SDL/include/SDL.h"
#include "../SDL_Image/include/SDL_image.h"
#include "../SDL_Mixer/include/SDL_mixer.h"
#pragma comment(lib,"../SDL/libx86/SDL2.lib")
#pragma comment(lib,"../SDL/libx86/SDL2main.lib")
#pragma comment(lib,"../SDL_Image/libx86/SDL2_image.lib")
#pragma comment(lib,"../SDL_Mixer/libx86/SDL2_mixer.lib")

#define WIDTH 640
#define HEIGHT 480

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define BACKGROUND 0
#define SHIP 1
#define BULLET 2

#define SHOTS 10

struct ship {
	int x, y;
	bool directions[4];
	bool shooting;
	int speed;
	int shotTimer;
	int shotCD;

	ship() {
		x = WIDTH / 6;
		y = HEIGHT / 6;
		speed = 5;
		shotTimer = 0;
		shotCD = 10;
		shooting = false;
		for (bool &d : directions) {
			d = false;
		}
	}
	bool canShoot() {
		if (shotTimer > 0) return false;
		else return true;
	}
	void cooldown() {
		if (shotTimer > 0) shotTimer--;
	}
};
struct bullet {
	int x, y;
	bool active, shot;
	int speed;

	bullet() {
		x = 0;
		y = 0;
		active = false;
		speed = 25;
	}
};

struct globals
{
	SDL_Renderer* renderer = nullptr;
	Mix_Music* music = nullptr;
	Mix_Chunk* blaster = nullptr;
	SDL_Window* window = nullptr;
	SDL_Texture* textures[3];


	ship* player = new ship();
	bullet bullets[SHOTS];
	SDL_Rect bckgdestRect = { 0,0,WIDTH,HEIGHT };
} g;

class SDLGame : public Module
{
	bool Init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		g.window = SDL_CreateWindow("SDLTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
		g.renderer = SDL_CreateRenderer(g.window, -1, 0);

		IMG_Init(IMG_INIT_PNG);
		g.textures[BACKGROUND] = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/Images/background.png"));
		g.textures[SHIP] = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/Images/ship.png"));
		g.textures[BULLET] = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/Images/bullet.png"));

		Mix_Init(MIX_INIT_OGG);
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_U8, 2, 1024);
		g.music = Mix_LoadMUS("Assets/Sounds/song.ogg");
		Mix_VolumeMusic(50);
		Mix_PlayMusic(g.music, -1);
		g.blaster = Mix_LoadWAV("Assets/Sounds/blaster.wav");

		for (int i = 0; i < SHOTS; i++) {
			g.bullets[i] = bullet();
		}
		return true;
	}

	update_status PreUpdate()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				return update_status::UPDATE_STOP;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					g.player->directions[UP] = true;
					break;
				case SDLK_DOWN:
					g.player->directions[DOWN] = true;
					break;
				case SDLK_LEFT:
					g.player->directions[LEFT] = true;
					break;
				case SDLK_RIGHT:
					g.player->directions[RIGHT] = true;
					break;
				case SDLK_SPACE:
					g.player->shooting = true;
					break;
				case SDLK_ESCAPE:
					return update_status::UPDATE_STOP;
					break;
				}
			}
			else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					g.player->directions[UP] = false;
					break;
				case SDLK_DOWN:
					g.player->directions[DOWN] = false;
					break;
				case SDLK_LEFT:
					g.player->directions[LEFT] = false;
					break;
				case SDLK_RIGHT:
					g.player->directions[RIGHT] = false;
					break;
				case SDLK_SPACE:
					g.player->shooting = false;
					break;
				}
			}
		}
		return update_status::UPDATE_CONTINUE;
	}

	update_status Update()
	{
		if (g.player->directions[UP]) g.player->y -= g.player->speed;
		if (g.player->directions[DOWN]) g.player->y += g.player->speed;
		if (g.player->directions[LEFT]) g.player->x -= g.player->speed;
		if (g.player->directions[RIGHT]) g.player->x += g.player->speed;

		if (g.player->shooting) {
			if (g.player->canShoot()) {
				for (int i = 0; i < SHOTS; i++) {
					if (!g.bullets[i].active) {
						Mix_PlayChannel(-1, g.blaster, 0);
						g.bullets[i].active = true;
						g.bullets[i].x = g.player->x + 32;
						g.bullets[i].y = g.player->y + 16;
						g.player->shotTimer = g.player->shotCD;
						break;
					}
				}
			}
		}
		g.player->cooldown();

		for (int i = 0; i<SHOTS; i++)
		{
			if (g.bullets[i].active) {
				g.bullets[i].x += g.bullets[i].speed;
				if (g.bullets[i].x > WIDTH) g.bullets[i].active = false;
			}
		}
		return update_status::UPDATE_CONTINUE;
	}

	update_status PostUpdate()
	{
		SDL_Rect destRect;
		destRect = { 0, 0, WIDTH, HEIGHT };

		SDL_RenderCopy(g.renderer, g.textures[BACKGROUND], nullptr, &destRect);

		for (int i = 0; i<10; i++)
		{
			if (g.bullets[i].active) {
				destRect = { g.bullets[i].x, g.bullets[i].y, 32, 32 };
				SDL_RenderCopy(g.renderer, g.textures[BULLET], nullptr, &destRect);
			}
		}
		destRect = { g.player->x, g.player->y, 64, 64 };
		SDL_RenderCopy(g.renderer, g.textures[SHIP], nullptr, &destRect);

		SDL_RenderPresent(g.renderer);
		return update_status::UPDATE_CONTINUE;
	}

	bool CleanUp()
	{
		delete g.player;
		Mix_FreeMusic(g.music);
		Mix_CloseAudio();
		Mix_Quit();
		IMG_Quit();
		SDL_DestroyRenderer(g.renderer);
		SDL_DestroyWindow(g.window);
		SDL_Quit();
		return true;
	}
};

#endif // __SDLGAME_H__

