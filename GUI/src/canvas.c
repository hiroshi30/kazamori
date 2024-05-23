#include <stdbool.h>
#include <math.h>

#include <SDL.h>

#include <canvas.h>


extern SDL_Renderer* renderer;


void Canvas_create(Canvas* canvas, int x, int y, int width, int height, SDL_Color* background_color) {
	canvas->x = x;
	canvas->y = y;

	canvas->width = width;
	canvas->height = height;

	canvas->background_color = background_color;
}


void Canvas_clear(Canvas* canvas) {
	SDL_SetRenderDrawColor(renderer, canvas->background_color->r, canvas->background_color->g, canvas->background_color->b, canvas->background_color->a);
    SDL_RenderFillRect(renderer, &(SDL_Rect){canvas->x, canvas->y, canvas->width, canvas->height});
}


bool Canvas_collide(Canvas* canvas, int mouse_x, int mouse_y) {
	canvas->mouse_on = false;
	if (mouse_x >= canvas->x && mouse_x <= canvas->x + canvas->width) {
		if (mouse_y >= canvas->y && mouse_y <= canvas->y + canvas->height) {
			canvas->mouse_on = true;
		}
	}
	return canvas->mouse_on;
}


void Canvas_draw_point(Canvas* canvas, int x, int y, SDL_Color* color) {
	if (x >= canvas->x && x <= canvas->x + canvas->width) {
		if (y >= canvas->y && y <= canvas->y + canvas->height) {
		    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
		    SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}


void Canvas_draw_line(Canvas* canvas, int x_begin, int y_begin, int x_end, int y_end, SDL_Color* color) {
	// if (x_begin >= canvas->x && x_begin <= canvas->x + canvas->width) {
	// 	if (y_begin >= canvas->y && y_begin <= canvas->y + canvas->height) {
	// 		if (x_end >= canvas->x && x_end <= canvas->x + canvas->width) {
	// 			if (y_end >= canvas->y && y_end <= canvas->y + canvas->height) {
				    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
				    SDL_RenderDrawLine(renderer, x_begin, y_begin, x_end, y_end);
	// 			}
	// 		}
	// 	}
	// }
}


void Canvas_draw_circle(Canvas* canvas, double center_x, double center_y, double radius, SDL_Color* color) {
	SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    for (int y = fmax(center_y - radius, canvas->y) + 1; y < fmin(center_y + radius, canvas->y + canvas->height); ++y) {
        for (int x = fmax(center_x - radius, canvas->x) + 1; x < fmin(center_x + radius, canvas->x + canvas->width); ++x) {
            if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}


void Canvas_draw_rect(Canvas* canvas, int x, int y, int width, int height, SDL_Color* color) {
	SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    SDL_RenderFillRect(renderer, &(SDL_Rect){x, y, width, height});
}


void Canvas_draw_vignette(Canvas* canvas, int border, SDL_Color* color) {
	SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
	// up
    SDL_RenderFillRect(renderer, &(SDL_Rect){canvas->x, canvas->y, canvas->width, border});
    // left
    SDL_RenderFillRect(renderer, &(SDL_Rect){canvas->x, canvas->y + border, border, canvas->height - 2 * border});
    // down
    SDL_RenderFillRect(renderer, &(SDL_Rect){canvas->x, canvas->y + canvas->height - border, canvas->width, border});
    // right
    SDL_RenderFillRect(renderer, &(SDL_Rect){canvas->x + canvas->width - border, canvas->y + border, border, canvas->height - 2 * border});
}
