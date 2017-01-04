/* imagekeys.c -- Andrew Cashner, 2017/01/04
 * Letter game for kids
 * Flashes rainbow colors until key is pressed;
 * then shows an image matching the key (e.g., a = aardvark)
 * Put .bmp image files in local directory with alphabetic filenames
 * like so: ./img/a.bmp, ./img/b.bmp.
 * Can specify different directory with command-line option.
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

#define MAX_STRING 120

/* Lookup tables */
/*   Command-line messages */
const enum {
    HELP,
    VERSION
} cli_message_types;
const char *cli_message[] = {
    /* HELP */
    "imagekeys -- Colorful letter-typing game for children\n"
     "By Andrew cashner, 2017\n"
     "Usage: image_keys [-d IMAGE_DIRECTORY]\n"
     "Default image directory is ./img/\n",

    /* VERSION */
    "imagekeys v1.0\n"
};

/*   SDL error messages */
const enum {
    INITIALIZE_ERROR,
    WINDOW_RENDERER_ERROR,
    SURFACE_ERROR,
    TEXTURE_ERROR,
    UNKNOWN_ERROR
} error_types;
const char *error_msg[] = {
    "Couldn't initialize SDL",
    "Couldn't create window and renderer",
    "Couldn't create surface from image",
    "Couldn't create texture from surface",
    "Unknown error\n"
};


/* Function prototypes */
void exit_SDL_error(int error_code);
void display_picture(char *image_filename, SDL_Renderer *renderer);


int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    char image_filename[MAX_STRING];
    char image_base_filename[] = "a.bmp";
    char image_directory[MAX_STRING] = "img/"; /* Default */
    int  image_directory_length = 4;

    bool quit = false;
    bool show_picture = false;

    int i;
    int rgb_rainbow[6][3] = { 
        {255, 0, 0},
        {255, 127, 0},
        {255, 255, 0},
        {0, 255, 0},
        {0, 0, 255},
        {75, 0, 130} 
    };

    int c;
    while ((c = getopt(argc, argv, "hvd:")) != -1) {
        switch (c) {
            case 'h':
                printf("%s", cli_message[HELP]);
                exit(0);
                break;
            case 'v':
                printf("%s", cli_message[VERSION]);
                exit(0); 
                break;
            case 'd':
                strcpy(image_directory, optarg);
                image_directory_length = strlen(image_directory);
                break;
            default:
                printf("%s", cli_message[HELP]);
                exit(EXIT_FAILURE);
        }
    }
    if (argc != optind) {
        printf("%s", cli_message[HELP]);
        exit(EXIT_FAILURE);
    }

    strcpy(image_filename, image_directory);
    strcat(image_filename, image_base_filename);

    /* TODO Check if directory and all bmp files a-z are present */

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        exit_SDL_error(INITIALIZE_ERROR);
    }
    if (SDL_CreateWindowAndRenderer(640, 580, SDL_WINDOW_RESIZABLE,
                &window, &renderer)) {
        exit_SDL_error(WINDOW_RENDERER_ERROR);
    }

    while (quit == false) {
        /* Loop through colors for background */
        for (i = 0; i < 6; ++i) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_WINDOWEVENT:
                        switch (event.window.event) {
                            case SDL_WINDOWEVENT_CLOSE:
                                quit = true;
                                break;
                            default:
                                ; /* Do nothing */
                        }
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            quit = true;
                            break;
                        } else if (event.key.keysym.sym >= SDLK_a &&
                                event.key.keysym.sym <= SDLK_z) {
                            /* Select image file based on key pressed */
                            image_filename[image_directory_length]
                                = (char)(event.key.keysym.sym - SDLK_a + 'a');
                            show_picture = true;
                            break;
                        }
                    default:
                        ; /* Do nothing */
                }
            }
            if (quit == true) {
                break;
            }

            /* Set up colored background */
            SDL_SetRenderDrawColor(renderer,
                    rgb_rainbow[i][0],
                    rgb_rainbow[i][1],
                    rgb_rainbow[i][2], 255);
            SDL_RenderClear(renderer);

            if (show_picture == true) {
                display_picture(image_filename, renderer);
                show_picture = false;
            } else {
                display_picture(NULL, renderer);
            }
        }
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return(0);
}


void exit_SDL_error(int error_code)
{
    if (error_code >= UNKNOWN_ERROR) {
        fprintf(stderr, "%s", error_msg[UNKNOWN_ERROR]);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "%s: %s", error_msg[error_code], SDL_GetError());
    }
    exit(EXIT_FAILURE);
}

void display_picture(char *image_filename, SDL_Renderer *renderer)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    if (image_filename == NULL) {
        /* just show colored background */
        SDL_RenderPresent(renderer);
        SDL_Delay(500);
    } else {
        surface = SDL_LoadBMP(image_filename);
        if (!surface) {
            exit_SDL_error(SURFACE_ERROR);
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            exit_SDL_error(TEXTURE_ERROR);
        }
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        SDL_DestroyTexture(texture);
    }
    return;
}


