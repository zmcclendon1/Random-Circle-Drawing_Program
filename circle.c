#define SDL_MAIN_USE_CALLBACKS 1  
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Initialize SDL
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_CreateWindowAndRenderer("Graphics Engine", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}


// Structs
typedef struct {
    float x;      
    float y;      
    float radius; 
} Circle;

typedef struct CircleNode {
    Circle circle;
    struct CircleNode *next;
} CircleNode;

CircleNode *circleList = NULL;

//Initializing Circles 
Circle circle = {0, 0, 12.0f};

// Global Variables
int mouseDown = 0;
int circleCount = 0;
float mx, my, mx2, my2;

//Drawing Functions
void SDL_RenderFillCircle(SDL_Renderer *renderer, Circle circle) {
    for (int w = -circle.radius; w <= circle.radius; w++) {
        for (int h = -circle.radius; h <= circle.radius; h++) {
            float distance = sqrt(w * w + h * h);  
            
            if (distance <= circle.radius) {
		SDL_RenderPoint(renderer, circle.x + w, circle.y + h);
            }
        }
    }
	
}

// Dynamically add circles
void AddCircle(float x, float y) {
    CircleNode *newNode = (CircleNode *)malloc(sizeof(CircleNode));
    if (!newNode) {
        SDL_Log("Memory allocation failed!");
        return;
    }

    newNode->circle.x = x;
    newNode->circle.y = y;
    newNode->circle.radius = 10.0f;
    newNode->next = circleList;  
    circleList = newNode;
}

void RenderCircles(SDL_Renderer *renderer) {
    CircleNode *current = circleList;
    while (current) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillCircle(renderer, current->circle);
        current = current->next;
    }
}
//Get Mouse Position and draw circle at that position
void SDL_PutCircleAtMousePosition(SDL_Renderer *renderer, Circle *circle) {
    SDL_GetMouseState(&mx, &my);
    circle->x = mx;
    circle->y = my;
    SDL_RenderFillCircle(renderer, *circle);  
}

// Event Handler
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        mouseDown = 1;  
    } 
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        mouseDown = 0;  
    }
    else if (event->type == SDL_EVENT_MOUSE_MOTION && mouseDown) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        AddCircle(mx, my);
    }
    return SDL_APP_CONTINUE;
}


// Main Loop
SDL_AppResult SDL_AppIterate(void *appstate)
{
    //Background stuff; fullscreen, bg color, clearing renderer to set new frames yada yada
    //SDL_SetWindowFullscreen(window, true);
    SDL_HideCursor();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    //Drawing the stationary circle
    RenderCircles(renderer);
    

    //Calling the circle to mouse function and setting the color of circle
    SDL_SetRenderDrawColor(renderer, 0, 65,170, 255);
    SDL_PutCircleAtMousePosition(renderer, &circle);

    //More SDL stuff I think this updates the screen to the latest frame idk man
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}


// Cleanup  
void FreeCircles() {
    CircleNode *current = circleList;
    while (current) {
        CircleNode *next = current->next;
        free(current);
        current = next;
    }
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    FreeCircles();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
}

    
