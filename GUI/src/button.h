#ifndef button_h
#define button_h


typedef struct Button {
	int x, y;
	int width, height;
	int border;
	int border_radius;
	SDL_Color* background_color;
	SDL_Color* border_color;
	int font_size;
	const char* text;
	bool mouse_on;
} Button;


void Button_create(Button* button, int x, int y, int width, int height, int border, int border_radius, SDL_Color* background_color, SDL_Color* border_color, int font_size, const char* text);
bool Button_collide(Button* button, int mouse_x, int mouse_y);

void Button_draw(Button* button);


#endif // button_h
