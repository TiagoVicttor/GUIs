#include <SDL2/SDL.h>
#include <assert.h>

int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int ret = SDL_WaitEventTimeout(evt, *ms);
    Uint32 depois = SDL_GetTicks();
    Uint32 gasto = depois - antes;

    if (gasto >= *ms) *ms = 0;
    else *ms -= gasto;

    return ret;
}

typedef enum {
    RODANDO,
    AGUARDANDO,
    RESET
} Estados;

void resetCorrida(SDL_Rect obj[3], int chegou[3], int* vencedorIndex) {
    obj[0] = (SDL_Rect){ 50, 200, 80, 40 };
    obj[1] = (SDL_Rect){ 50, 350, 80, 40 };
    obj[2] = (SDL_Rect){ 50, 500, 80, 40 };

    chegou[0] = chegou[1] = chegou[2] = 0;
    *vencedorIndex = -1;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "1.6",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect obj[3];
    int chegou[3];
    int vencedorIndex;

    resetCorrida(obj, chegou, &vencedorIndex);

    const int linhaChegada = 1100;
    Estados estado = RODANDO;

    Uint32 esperaStart = 0;     // usado no AGUARDANDO
    Uint32 duracaoEspera = 2000; // 2s

    while (!SDL_QuitRequested())
    {
        Uint32 timeout = 16;
        SDL_Event evt;
        AUX_WaitEventTimeout(&evt, &timeout);

        if (estado  == RODANDO)
        {
            for (int i = 0; i < 3; i++) {
                if (!chegou[i]) {
                    obj[i].x += (1 + i);

                    if (obj[i].x + obj[i].w >= linhaChegada) {
                        chegou[i] = 1;

                        if (vencedorIndex == -1)
                            vencedorIndex = i;
                    }
                }
            }

            // todos chegaram?
            if (chegou[0] && chegou[1] && chegou[2]) {
                estado  = AGUARDANDO;
                esperaStart = SDL_GetTicks();
            }
        }
        else if (estado  == AGUARDANDO)
        {
            Uint32 now = SDL_GetTicks();
            if (now - esperaStart >= duracaoEspera) {
                estado  = RESET;
            }
        }
        else if (estado  == RESET)
        {
            resetCorrida(obj, chegou, &vencedorIndex);
            estado  = RODANDO;
        }

        // ---- Desenho da tela ----
        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        // Objetos
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        for (int i = 0; i < 3; i++)
            SDL_RenderFillRect(ren, &obj[i]);

        // Linha de chegada
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderDrawLine(ren, linhaChegada, 0, linhaChegada, 720);

        // Quadrado dourado do vencedor
        if (vencedorIndex >= 0) {
            SDL_Rect medalha = { linhaChegada + 30, obj[vencedorIndex].y, 40, 40 };
            SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
            SDL_RenderFillRect(ren, &medalha);
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
