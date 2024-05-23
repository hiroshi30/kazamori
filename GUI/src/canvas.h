#ifndef canvas_h
#define canvas_h


typedef struct Canvas {
	int x, y;
	int width, height;
	SDL_Color* background_color;
	bool mouse_on;
} Canvas;


void Canvas_create(Canvas* canvas, int x, int y, int width, int height, SDL_Color* background_color);
void Canvas_clear(Canvas* canvas);
bool Canvas_collide(Canvas* canvas, int mouse_x, int mouse_y);

void Canvas_draw_point(Canvas* canvas, int x, int y, SDL_Color* color);
void Canvas_draw_line(Canvas* canvas, int x_begin, int y_begin, int x_end, int y_end, SDL_Color* color);
void Canvas_draw_circle(Canvas* canvas, double center_x, double center_y, double radius, SDL_Color* color);
void Canvas_draw_rect(Canvas* canvas, int x, int y, int width, int height, SDL_Color* color);
void Canvas_draw_vignette(Canvas* canvas, int border, SDL_Color* color);


#endif // canvas_h
