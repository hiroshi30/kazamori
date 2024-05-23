#ifndef GUI_h
#define GUI_h


#include <SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

#include <canvas.h>
#include <button.h>


int init(const char* window_title, int window_width, int window_height);
void quit();
void update();


#endif // GUI_h
