#include <SDL2/SDL.h>

#define MAX_RECTS 10

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Event evt;

    // Para armazenar os retângulos criados com o mouse
    SDL_Rect rects[MAX_RECTS];
    int colors[MAX_RECTS][3];
    int rectCount = 0;

    int running = 1;

    while (running) {

        /* ----- DESENHO ----- */
        SDL_SetRenderDrawColor(ren, 255,255,255,255);
        SDL_RenderClear(ren);

        // desenha retângulo móvel
        SDL_SetRenderDrawColor(ren, 0,0,255,255);
        SDL_RenderFillRect(ren, &r);

        // desenha retângulos fixos criados com o mouse
        for (int i = 0; i < rectCount; i++) {
            SDL_SetRenderDrawColor(ren, colors[i][0], colors[i][1], colors[i][2], 255);
            SDL_RenderFillRect(ren, &rects[i]);
        }

        SDL_RenderPresent(ren);

        /* ----- EVENTOS ----- */
        SDL_WaitEvent(&evt);

        if (evt.type == SDL_QUIT) {
            running = 0;
        }

        if (evt.type == SDL_KEYDOWN) {
            switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    r.y -= 5;
                    break;
                case SDLK_DOWN:
                    r.y += 5;
                    break;
                case SDLK_LEFT:
                    r.x -= 5;
                    break;
                case SDLK_RIGHT:
                    r.x += 5;
                    break;
            }

            // manter dentro da tela (200x100)
            if (r.x < 0) r.x = 0;
            if (r.y < 0) r.y = 0;
            if (r.x + r.w > 200) r.x = 200 - r.w;
            if (r.y + r.h > 100) r.y = 100 - r.h;
        }

        // Clique do mouse → novo retângulo
        if (evt.type == SDL_MOUSEBUTTONDOWN) {
            if (rectCount < MAX_RECTS) {
                int mx = evt.button.x;
                int my = evt.button.y;

                rects[rectCount].x = mx - 5;
                rects[rectCount].y = my - 5;
                rects[rectCount].w = 10;
                rects[rectCount].h = 10;

                // cores diferentes simples
                colors[rectCount][0] = (rectCount * 40) % 255;
                colors[rectCount][1] = (rectCount * 80) % 255;
                colors[rectCount][2] = (rectCount * 120) % 255;

                rectCount++;
            }
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
