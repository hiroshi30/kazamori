#ifdef Debug
#include <stdio.h>
#endif // Debug
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include <button.h>


extern SDL_Renderer* renderer;


void Button_create(Button* button, int x, int y, int width, int height, int border, int border_radius, SDL_Color* background_color, SDL_Color* border_color, int font_size, const char* text) {
	button->x = x;
	button->y = y;

	button->width = width;
	button->height = height;

	button->border = border;
	button->border_radius = border_radius;

	button->background_color = background_color;
	button->border_color = border_color;

	button->font_size = font_size;
	button->text = text;

	button->mouse_on = false;
}


// bool Button_collide(Button* button, int mouse_x, int mouse_y) {
// 	if (button->x <= mouse_x && mouse_x <= button->x + button->width) {
// 		if (button->y <= mouse_y && mouse_y <= button->y + button->height) {
// 			button->mouse_on = true;
// 			return true;
// 		}
// 	}
// 	button->mouse_on = false;
// 	return false;
// }


bool Button_collide(Button* button, int mouse_x, int mouse_y) {
	button->mouse_on = false;

	if (mouse_x >= button->x + button->border_radius && mouse_x <= button->x + button->width - button->border_radius) {
		if (mouse_y >= button->y && mouse_y <= button->y + button->border_radius) {
			button->mouse_on = true;
		}
	}

	if (mouse_x >= button->x && mouse_x <= button->x + button->width) {
		if (mouse_y >= button->y + button->border_radius && mouse_y <= button->y + button->height - button->border_radius) {
			button->mouse_on = true;
		}
	}

	if (mouse_x >= button->x + button->border_radius && mouse_x <= button->x + button->width - button->border_radius) {
		if (mouse_y >= button->y + button->height - button->border_radius && mouse_y <= button->y + button->height) {
			button->mouse_on = true;
		}
	}

	int center_x, center_y;
	center_x = button->x + button->border_radius;
	center_y = button->y + button->border_radius;
	if ((mouse_x - center_x) * (mouse_x - center_x) + (mouse_y - center_y) * (mouse_y - center_y) <= button->border_radius * button->border_radius) {
		button->mouse_on = true;
	}

	center_x = button->x + button->width - button->border_radius;
	center_y = button->y + button->border_radius;
	if ((mouse_x - center_x) * (mouse_x - center_x) + (mouse_y - center_y) * (mouse_y - center_y) <= button->border_radius * button->border_radius) {
		button->mouse_on = true;
	}

	center_x = button->x + button->border_radius;
	center_y = button->y + button->height - button->border_radius;
	if ((mouse_x - center_x) * (mouse_x - center_x) + (mouse_y - center_y) * (mouse_y - center_y) <= button->border_radius * button->border_radius) {
		button->mouse_on = true;
	}

	center_x = button->x + button->width - button->border_radius;
	center_y = button->y + button->height - button->border_radius;
	if ((mouse_x - center_x) * (mouse_x - center_x) + (mouse_y - center_y) * (mouse_y - center_y) <= button->border_radius * button->border_radius) {
		button->mouse_on = true;
	}

	return button->mouse_on;
}


// void Button_draw_border(Button* button) {
// 	double k = 1;
// 	if (button->mouse_on) {
// 		k = 0.5;
// 	}

// 	SDL_SetRenderDrawColor(renderer,
// 		k * button->border_color->r,
// 		k * button->border_color->g,
// 		k * button->border_color->b,
// 		255
// 	);
// 	SDL_RenderFillRect(renderer, &(SDL_Rect){
// 		button->x,
// 		button->y,
// 		button->width,
// 		button->height
// 	});
// }


// void Button_draw_background(Button* button) {
// 	double k = 1;
// 	if (button->mouse_on) {
// 		k = 0.5;
// 	}

// 	SDL_SetRenderDrawColor(renderer,
// 		k * button->background_color->r,
// 		k * button->background_color->g,
// 		k * button->background_color->b,
// 		255
// 	);
// 	SDL_RenderFillRect(renderer, &(SDL_Rect){
// 		button->x + button->border,
// 		button->y + button->border,
// 		button->width - 2 * button->border,
// 		button->height - 2 * button->border
// 	});
// }


void Button_draw_border(Button* button) {
	double k = 1;
	if (button->mouse_on) {
		k = 0.5;
	}

	SDL_SetRenderDrawColor(renderer,
		k * button->border_color->r,
		k * button->border_color->g,
		k * button->border_color->b,
		255
	);

	SDL_RenderFillRect(renderer, &(SDL_Rect){
		button->x + button->border_radius,
		button->y,
		button->width - 2 * button->border_radius,
		button->border_radius
	});
	SDL_RenderFillRect(renderer, &(SDL_Rect){
		button->x,
		button->y + button->border_radius,
		button->width,
		button->height - 2 * button->border_radius
	});
	SDL_RenderFillRect(renderer, &(SDL_Rect){
		button->x + button->border_radius,
		button->y + button->height - button->border_radius,
		button->width - 2 * button->border_radius,
		button->border_radius
	});

	for (int y = 0; y < button->border_radius; ++y) {
		for (int x = 0; x < button->border_radius; ++x) {
			if (x * x + y * y <= button->border_radius * button->border_radius) {
                SDL_RenderDrawPoint(renderer, button->x + button->border_radius - x, button->y + button->border_radius - y);
                SDL_RenderDrawPoint(renderer, button->x + button->border_radius - x, button->y + button->height - button->border_radius + y);
                SDL_RenderDrawPoint(renderer, button->x + button->width - button->border_radius + x, button->y + button->border_radius - y);
                SDL_RenderDrawPoint(renderer, button->x + button->width - button->border_radius + x, button->y + button->height - button->border_radius + y);
            }
		}
	}
}


void Button_draw_background(Button* button) {
	double k = 1;
	if (button->mouse_on) {
		k = 0.5;
	}

	SDL_SetRenderDrawColor(renderer,
		k * button->background_color->r,
		k * button->background_color->g,
		k * button->background_color->b,
		255
	);

	SDL_RenderFillRect(renderer, &(SDL_Rect){
		button->x + button->border_radius,
		button->y + button->border,
		button->width - 2 * button->border_radius,
		button->border_radius - button->border
	});
	SDL_RenderFillRect(renderer, &(SDL_Rect){
		button->x + button->border,
		button->y + button->border_radius,
		button->width - 2 * button->border,
		button->height - 2 * button->border_radius
	});
	SDL_RenderFillRect(renderer, &(SDL_Rect){
		button->x + button->border_radius,
		button->y + button->height - button->border_radius,
		button->width - 2 * button->border_radius,
		button->border_radius - button->border
	});

	for (int y = 0; y < button->border_radius; ++y) {
		for (int x = 0; x < button->border_radius; ++x) {
			if (x * x + y * y <= (button->border_radius - button->border) * (button->border_radius - button->border)) {
                SDL_RenderDrawPoint(renderer, button->x + button->border_radius - x, button->y + button->border_radius - y);
                SDL_RenderDrawPoint(renderer, button->x + button->border_radius - x, button->y + button->height - button->border_radius + y);
                SDL_RenderDrawPoint(renderer, button->x + button->width - button->border_radius + x, button->y + button->border_radius - y);
                SDL_RenderDrawPoint(renderer, button->x + button->width - button->border_radius + x, button->y + button->height - button->border_radius + y);
            }
		}
	}
}


void Button_draw_text(Button* button) {
	TTF_Font* font = TTF_OpenFont("GUI/Arial.ttf", button->font_size);

    if (font) {
	    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

	    SDL_Surface* text_surface = TTF_RenderText_Solid(font, button->text, (SDL_Color){0, 0, 0, 255});
	    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

		int text_width = 0;
		int text_height = 0;
		SDL_QueryTexture(text_texture, NULL, NULL, &text_width, &text_height);
		SDL_RenderCopy(renderer, text_texture, NULL, &(SDL_Rect){
			button->x + (button->width - text_width) / 2,
			button->y + (button->height - text_height) / 2,
			text_width,
			text_height
		});

	    SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);
    }
	#ifdef Debug
	else {
    	printf("!!! ERROR in TTF_OpenFont(): %s !!!\n", TTF_GetError());
	}
	#endif // Debug

	TTF_CloseFont(font);
}


void Button_draw(Button* button) {
	Button_draw_border(button);
	Button_draw_background(button);
    Button_draw_text(button);
}
