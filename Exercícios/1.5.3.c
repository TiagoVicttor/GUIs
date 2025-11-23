#include <SDL2/SDL.h>
#include <assert.h>

// ---------------- AUXILIAR DO EXERCÍCIO 1.5.2 ----------------
int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms)
{
    if (*ms == 0)
        return 0;

    Uint32 antes = SDL_GetTicks();
    int ret = SDL_WaitEventTimeout(evt, *ms);
    Uint32 depois = SDL_GetTicks();

    Uint32 gasto = depois - antes;

    if (gasto >= *ms) *ms = 0;
    else *ms -= gasto;

    return ret;
}

// ----------------------------------------------------------------

int main(int argc, char* args[])
{
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win = SDL_CreateWindow("Ex 1.3.1 (eventos + timeout)",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       1920, 950, SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_Rect n1 = {400,125,100,50};
    SDL_Rect n2 = {700,75,100,50};
    SDL_Rect n3 = {1000,175,100,50};
    SDL_Rect n4 = {1300,125,100,50};
    SDL_Rect n5 = {1600,50,100,50};
    SDL_Rect sol = {100,75,100,100};

    Uint32  timerCorSol= 0;

    while (!SDL_QuitRequested())
    {
        Uint32 timeout = 16; // ~60 FPS

        SDL_Event evt;
        int ret = AUX_WaitEventTimeout(&evt, &timeout);

        // ---------------------------------------
        // 1. PROCESSA EVENTOS
        // ---------------------------------------
        if (ret == 1)
        {
            if (evt.type == SDL_KEYDOWN) {
                timerCorSol = 1000;
            }
        }

        // ---------------------------------------
        // 2. ATUALIZA ANIMAÇÃO (independente de eventos)
        // ---------------------------------------

        if (timerCorSol > timeout)
            timerCorSol -= timeout;
        else
            timerCorSol = 0;

        SDL_Rect* arr[5] = {&n1,&n2,&n3,&n4,&n5};

        for (int i = 0; i < 5; i++) {
            arr[i]->x += 1;  
            if (arr[i]->x >= 1920)
                arr[i]->x = -50;
        }

        // ---------------------------------------
        // 3. DESENHO
        // ---------------------------------------
        SDL_SetRenderDrawColor(ren, 0xbb,0xf3,0xf9,255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0xd0,0x9d,0x6a,255);
        SDL_Rect terra = {0,700,1920,250};
        SDL_RenderFillRect(ren, &terra);

        SDL_SetRenderDrawColor(ren, 0x05,0xee,0x07,255);
        SDL_Rect grama = {0,700,1920,50};
        SDL_RenderFillRect(ren, &grama);

        if (timerCorSol > 0)
            SDL_SetRenderDrawColor(ren, 0xff, 0x90, 0x20, 255);
        else
            SDL_SetRenderDrawColor(ren, 0xf3, 0xf0, 0x63, 255);

        SDL_RenderFillRect(ren, &sol);

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &n1);
        SDL_RenderFillRect(ren, &n2);
        SDL_RenderFillRect(ren, &n3);
        SDL_RenderFillRect(ren, &n4);
        SDL_RenderFillRect(ren, &n5);

        SDL_RenderPresent(ren);
    }

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
