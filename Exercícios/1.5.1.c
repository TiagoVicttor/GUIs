#include <SDL2/SDL.h>

int main(int argc, char* args[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Tres Retangulos",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        640, 480, SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // --- Retângulos ---
    SDL_Rect r1 = {50, 50, 40, 40};   // Automático
    SDL_Rect r2 = {200, 200, 40, 40}; // Teclado
    SDL_Rect r3 = {0, 0, 40, 40};     // Mouse

    int rodando = 1;
    Uint32 ultimoTempo = SDL_GetTicks();

    while (rodando)
    {
        // Espera por UMA mensagem (não varre fila)
        SDL_Event evt;
        SDL_WaitEvent(&evt);

        // --- PROCESSA O EVENTO RECEBIDO ---
        if (evt.type == SDL_QUIT)
            rodando = 0;

        if (evt.type == SDL_KEYDOWN) {
            switch (evt.key.keysym.sym) {
                case SDLK_UP:    r2.y -= 5; break;
                case SDLK_DOWN:  r2.y += 5; break;
                case SDLK_LEFT:  r2.x -= 5; break;
                case SDLK_RIGHT: r2.x += 5; break;
            }
        }

        if (evt.type == SDL_MOUSEMOTION) {
            r3.x = evt.motion.x - r3.w / 2;
            r3.y = evt.motion.y - r3.h / 2;
        }

        // --- MOVIMENTO AUTOMÁTICO DO RETÂNGULO 1 ---
        Uint32 agora = SDL_GetTicks();
        float dt = (agora - ultimoTempo) / 1000.0f;
        ultimoTempo = agora;

        r1.x += (int)(50 * dt);
        if (r1.x > 640) r1.x = -40;

        // --- DESENHA TUDO ---
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);   SDL_RenderFillRect(ren, &r1);
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);   SDL_RenderFillRect(ren, &r2);
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);   SDL_RenderFillRect(ren, &r3);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
