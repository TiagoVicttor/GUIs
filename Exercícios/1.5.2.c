#include <SDL2/SDL.h>
#include <stdio.h>

// --- Função pedida ---
int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms)
{
    if (*ms == 0)
        return 0;

    Uint32 antes = SDL_GetTicks();

    int ret = SDL_WaitEventTimeout(evt, *ms);

    Uint32 depois = SDL_GetTicks();
    Uint32 gasto = depois - antes;

    if (gasto >= *ms)
        *ms = 0;
    else
        *ms -= gasto;

    return ret;
}

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("1.5.2",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        400, 200, SDL_WINDOW_SHOWN);

    SDL_Event evt;

    Uint32 timeoutTotal = 5000; // 5 segundos

    printf("Timeout total: %u ms\n", timeoutTotal);

    while (1)
    {
        Uint32 restante = timeoutTotal;

        int ret = AUX_WaitEventTimeout(&evt, &restante);

        timeoutTotal = restante;

        if (ret == 0) {
            printf("Timeout total esgotado!\n");
            break;
        }

        printf("Evento recebido. Restante: %u ms\n", timeoutTotal);

        if (evt.type == SDL_QUIT)
            break;

        if (timeoutTotal == 0) {
            printf("Timeout chegou a zero.\n");
            break;
        }
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
