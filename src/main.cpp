#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include "Constants.cpp"
#include "Boid.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui.h"

SDL_Texture* loadTexture(const char* fname, SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(fname);
    if(image == nullptr) {
        SDL_Log("Unable to load image; SDL Error: %s", SDL_GetError());
    }
    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(renderer, image);
    if(imgTexture == nullptr) {
        SDL_Log("Unable to create a texture from the image");
    }
    SDL_FreeSurface(image);
    return imgTexture;
}

int main() {
    srand (time(NULL));
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to init SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    SDL_Log("We compiled against SDL version %u.%u.%u ...\n",
        compiled.major, compiled.minor, compiled.patch);
    SDL_Log("But we are linking against SDL version %u.%u.%u.\n",
        linked.major, linked.minor, linked.patch);

    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 flags = SDL_RENDERER_ACCELERATED;
    SDL_Event event;

    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, flags, &window, &renderer)) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Couldn't create window and renderer: %s",
            SDL_GetError());
        return 1;
    }

    const char* title = "Babies";
    SDL_SetWindowTitle(window, title);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    SDL_Texture *babyTex = loadTexture("res/baby.png", renderer);
    std::vector<Boid*> boids;
    int numBoids = 300;
    for (int i = 0; i < numBoids; i++)  {
        float posX = rand() % WINDOW_WIDTH;
        float posY = rand() % WINDOW_HEIGHT;
        boids.push_back(new Boid(posX, posY));
    }

    // Controls
    float maxSpeed = 3.0f;
    float desiredSep = 25.0f;
    float alignRad = 50.0f;
    float coRad = 50.0f;
    bool showVelocityLines = false;

    while(true) {
        SDL_PollEvent(&event);
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Graphics Demo");
        ImGui::Text("Baby Controls");
        ImGui::Checkbox("Velocity Lines", &showVelocityLines);
        ImGui::SliderFloat("Separation", &desiredSep, 0.0f, 100.0f);
        ImGui::SliderFloat("Alignment", &alignRad, 0.0f, 100.0f);
        ImGui::SliderFloat("Cohesion", &coRad, 0.0f, 10000.0f);
        ImGui::SliderFloat("Max Speed", &maxSpeed, 0.0f, 6.0f);
        ImGui::ColorEdit3("Clear color", (float*)&clear_color);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        Boid::render(boids, renderer, babyTex, desiredSep, alignRad, coRad, maxSpeed, showVelocityLines);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    IMG_Quit();
    SDL_DestroyTexture(babyTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}




