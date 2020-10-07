#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stack>
#include <sstream>
#include <unistd.h>
#include <conio.h>
#include <math.h>
#include <float.h>
#include <cfloat>
using namespace std;

// Estrutura para representar pontos
typedef struct {int x, y;} Point;

// Variáveis necessárias para o SDL
unsigned int * pixels;
int width, height;
SDL_Surface * window_surface;
SDL_Renderer * renderer;

// Título da janela
std::string titulo = "SDL Trabalho";

// Valores RGB para a cor de funco da janela
const int VERMELHO = 255;
const int VERDE = 255;
const int AZUL = 255;
const int MiddleX = 475;
const int MiddleY = 295;

int tamGrafico = 525;

double pontoX[1000];
double pontoY[1000];

double XINI = - 10.0;
double XFIM = + 10.0;
double YINI = + 10.0;
double YFIM = - 10.0;
double lstY = 0.0, lstX = 0.0;

int pos_x = 0;
int pos_y = 0;
int PONTOS = 21;
int ESPACO = PONTOS * 10;
int MOVEX = 0;
int MOVEY = 0;

typedef struct
{
    double ponto;
    double indice;
} Coordenada;

Coordenada coordenadaX[1000];
Coordenada coordenadaY[1000];

// Gera uma estrutura Point a partir de valores para x e y
Point getPoint(int x, int y)
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

bool AreSame(double a, double b)
{
    return fabs(a - b) < 0.1 ;
}

// Obtém a cor de um pixel de uma determinada posição
Uint32 getPixel(int x, int y)
{
    if((x>=0 && x<=width) && (y>=0 && y<=height))
        return pixels[x + width * y];
    else
        return -1;
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g, b, e alpha (transparência)
// r, g, b e a variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b, int a)
{
    pixels[x + y * width] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g e b
// r, g, e b variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b)
{
    setPixel(x, y, r, g, b, 255);
}

// Mostra na barra de título da janela a posição
// corrente do mouse
void showMousePosition(SDL_Window * window, int x, int y)
{
    std::stringstream ss;
    ss << titulo << " X: " << x << " Y: " << y;
    SDL_SetWindowTitle(window, ss.str().c_str());
}

// Imprime na console a posição corrente do mouse
void printMousePosition(int x, int y)
{
    printf("Mouse on x = %d, y = %d\n",x,y);
}

// Seta um pixel em uma determinada posição,
// através de um Uint32 representando
// uma cor RGB
void setPixel(int x, int y, Uint32 color)
{
    if((x<0 || x>=width || y<0 || y>=height)) {
        printf("Coordenada inválida : (%d,%d)\n",x,y);
        return;
    }
    pixels[x + y * width] = color;
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, b e a(transparência)
// informadas. r, g, b e a variam de 0 até 255
Uint32 RGB(int r, int g, int b, int a) {
    return SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, e b
// informadas. r, g e b variam de 0 até 255
// a transparência é sempre 255 (imagem opaca)
Uint32 RGB(int r, int g, int b) {
    return SDL_MapRGBA(window_surface->format, r, g, b, 255);
}

// Retorna um componente de cor de uma cor RGB informada
// aceita os parâmetros 'r', 'R','g', 'G','b' e 'B',
Uint8 getColorComponent( Uint32 pixel, char component ) {

    Uint32 mask;

    switch(component) {
        case 'b' :
        case 'B' :
            mask = RGB(0,0,255);
            pixel = pixel & mask;
            break;
        case 'r' :
        case 'R' :
            mask = RGB(255,0,0);
            pixel = pixel & mask;
            pixel = pixel >> 16;
            break;
        case 'g' :
        case 'G' :
            mask = RGB(0,255,0);
            pixel = pixel & mask;
            pixel = pixel >> 8;
            break;
    }
    return (Uint8) pixel;
}

void bresenham (int x1, int y1, int x2, int y2)
{
    bool steep = abs(y2 - y1) > abs(x2 - x1);
    if(steep) {
        // swap x1 and y1
        int tmp = x1;
        x1 = y1;
        y1 = tmp;
        // swap x2 and y2
        tmp = x2;
        x2 = y2;
        y2 = tmp;
    }
    if(x1 > x2) {
        // swap x1 and x2
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
        // swap y1 and y2
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    int dx, dy, error, ystep, y;

    dx = x2 - x1;
    dy = abs(y2 - y1);
    error = dx / 2;
    y = y1;

    if(y1 < y2)
        ystep = 1;
    else
        ystep = -1;

    for (int x = x1; x <= x2; x++) {
        if(steep)
            setPixel( y, x, RGB(0,0,0) );
        else
            setPixel( x, y, RGB(0,0,0) );
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void linha(int x, int y,int direcao, int tam){
    int i=0;
    //vertical
    if (direcao){
        for(i=0;i<tam;i++){
           setPixel(x + i,y, RGB(0,0,0));
        }
    }
    else{
        //horizontal
        for(i=0;i<tam;i++){
           setPixel(x,y + i,RGB(0,0,0));
        }
    }
}

void linha_tracado(int x, int y,int direcao, int tam){
    int i=0;
    //vertical
    if (direcao){
        for(i=0;i<tam;i+=2){
           setPixel(x + i,y, RGB(0,0,0));
        }
    }
    else{
        //horizontal
        for(i=0;i<tam;i+=2){
           setPixel(x,y + i,RGB(0,0,0));
        }
    }
}

void initPontosX(){
    double pt = (tamGrafico / (PONTOS - 1)) / 10.0;
    int indice = 0;

    memset(pontoX,0,1000);

    for(double x = XINI;x < (XFIM + 0.1);x+=0.1){
        if (indice == 0) pontoX[indice] = pontoX[indice] = 200;
        else pontoX[indice] = pontoX[indice-1] + pt;

        coordenadaX[indice].ponto  = x;
        coordenadaX[indice].indice = indice;
        indice++;
    }
}

void initPontosY(){
    double pt = ((tamGrafico / (PONTOS - 1)) / 10.0);
    int indice = 0;

    memset(pontoY,0,1000);

    for(double y = YINI;y > (YFIM - 0.1);y-=0.1){
        if (indice == 0) pontoY[indice] = pontoY[indice] = 20;
        else pontoY[indice] = pontoY[indice-1] + pt;

        coordenadaY[indice].ponto  = y;
        coordenadaY[indice].indice = indice;
        indice++;
    }

}

void initPontos(){
    initPontosX();
    initPontosY();

}

void bezierCurve(Point *ponteiros){
    double xu = 0.0 , yu = 0.0 , u = 0.0 ;
    int i = 0 ;
    for(u = 0.0 ; u <= 1.0 ; u += 0.0001)
    {
        xu = pow(1-u,3)*ponteiros[0].x+3*u*pow(1-u,2)*ponteiros[1].x+3*pow(u,2)*(1-u)*ponteiros[2].x
             +pow(u,3)*ponteiros[3].x;
        yu = pow(1-u,3)*ponteiros[0].y+3*u*pow(1-u,2)*ponteiros[1].x+3*pow(u,2)*(1-u)*ponteiros[2].x
            +pow(u,3)*ponteiros[3].x;
        setPixel(xu, yu,0);
    }
}

void getColor(int fnct,int &R, int &G, int &B){
    switch(fnct){
        case 1:
            //R = 0,G = 255, B = 0;
            R = 0,G = 0, B = 0;
            break;
        case 2:
            R = 255,G = 0, B = 0;
            break;
        case 3:
            R = 0,G = 0, B = 255;
            break;
        case 4:
            R = 115,G = 115, B = 115;
            break;
        case 5:
            R = 0,G = 0, B = 0;
            break;
    }
}

double getFunctionValue(double x, int fnct){

    switch(fnct){
        case 1:
            return x;
        case 2:
            return -x;
        case 3:
            return 2 * pow(2,x) - 6 * x + 1;
        case 4:
            return sin(x);
        case 5:
            return - pow(x,2) + 4;
    }
}

void reviseDraw(int fnct, int ptX, int ptY){

    switch(fnct){
        case 2:
            for(int px=0;px < 25.0;px+=2.5){
                setPixel( pontoX[ptX] + px, pontoY[ptY] + px, RGB(0,0,0));
            }
            break;
    }
}

void printFunction(double x, int fnct){
    int ptX, ptY;
    int R,G,B;
    double y;

    getColor(fnct,R,G,B);

    ptX = -1, ptY = -1, y = 0;

    y = getFunctionValue(x,fnct);

    if (y < YFIM || y > YINI)
        return;

    for(int j=0;j < ESPACO;j++){
        if (AreSame(coordenadaX[j].ponto,x))
            ptX = coordenadaX[j].indice;
        if (AreSame(coordenadaY[j].ponto,y))
            ptY = coordenadaY[j].indice;

        if (ptX != -1 && ptY != -1){
            break;
        }
    }

    if (ptX == -1 || ptY == -1)
        return;

    if ((pontoX[ptX] > (lstX)) || (pontoY[ptY] > (lstY)))
        return;

    if ((pontoX[ptX] < 200) || (pontoY[ptY] < 20))
        return;

    if (fnct == 1) setPixel( pontoX[ptX], pontoY[ptY] /*+ 25*/, RGB(R,G,B));
    else if (fnct == 5) setPixel( pontoX[ptX], pontoY[ptY] /*+ 25*/, RGB(R,G,B));
         else setPixel( pontoX[ptX], pontoY[ptY], RGB(R,G,B));
}


void drawFunction(int fnct){

    for(double x=XINI;x <= XFIM;x+=0.01){
        printFunction(x,fnct);
    }
}


void drawDots(){
    int x = 220;
    int y = 20;

    for(int i=0;i < PONTOS; i++){
        printf("%d \n",pontoY[i]);
    }

    for(int i=0;i < PONTOS; i++){
        setPixel( x, pontoY[i], RGB(0,0,0) );
    }
    for(int i=0;i < PONTOS; i++){
        setPixel( pontoX[i], y, RGB(0,0,0) );
    }
}

void sandbox(){
    int TAM = 10;

    for(int y=0;y < PONTOS;y++)
        lstY = pontoY[y * 10] - MOVEY,linha(200,pontoY[y * 10],1,TAM);


    linha(200,20,0,lstY - 20);
    for(int x=0; x < PONTOS;x++)
        lstX = pontoX[x * 10] - MOVEX,linha(pontoX[x * 10],lstY - TAM,0,TAM);

    linha(200,lstY,1,lstX - 200);
}

// Aqui ocorrem as chamadas das funções a ser exibidas na janela
void display()
{
    initPontos();
    sandbox();
    drawFunction(1);
    drawFunction(2);
    drawFunction(3);
    drawFunction(4);
    drawFunction(5);
}




// Inicializa o SDL, abre a janela e controla o loop
// principal do controle de eventos
int main()
{
    // Inicializações iniciais obrigatórias
    int xvel = 0;
    int yvel = 0;
    setlocale(LC_ALL, NULL);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow(titulo.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_RESIZABLE);

    window_surface = SDL_GetWindowSurface(window);

    pixels = (unsigned int *) window_surface->pixels;
    width = window_surface->w;
    height = window_surface->h;


    // Fim das inicializações

    printf("Pixel format: %s\n",
        SDL_GetPixelFormatName(window_surface->format->format));

     while (1)
     {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }

            switch( event.type ){
            /* Look for a keypress */
                case SDL_KEYDOWN:
                    /* Check the SDLKey values and move change the coords */
                    switch( event.key.keysym.sym ){
                        case SDLK_LEFT:
                            printf("Pressionada a tecla LEFT\n");
                            XINI += 1, XFIM +=1;
                            break;
                        case SDLK_RIGHT:
                            printf("Pressionada a tecla RIGHT\n");
                            XINI -= 1, XFIM +=1;
                            break;
                        case SDLK_UP:
                            printf("Pressionada a tecla UP\n");
                            YINI -= 1, YFIM -=1;
                            break;
                        case SDLK_DOWN:
                            printf("Pressionada a tecla DOWN\n");
                            YINI += 1, YFIM -=1;
                            break;
                        case SDLK_EQUALS:
                            printf("Pressionada a tecla + \n");
                            XINI -= 1, XFIM +=1;
                            YINI += 1, YFIM -=1;
                            PONTOS +=2, ESPACO = PONTOS * 10;
                            break;
                        case SDLK_MINUS:
                            printf("Pressionada a tecla - \n");
                            XINI += 1, XFIM -=1;
                            YINI -= 1, YFIM +=1;
                            PONTOS -=2, ESPACO = PONTOS * 10;
                            break;

                        default:
                            break;
                    }

                    //printf("XINI %2.f XFIM %2.f YINI %2.f YFIM %2.f \n",XINI,XFIM,YINI,YFIM);
                break;
            }


            /* Update the alien position */
            pos_x += xvel;
            pos_y += yvel;

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    window_surface = SDL_GetWindowSurface(window);
                    pixels = (unsigned int *) window_surface->pixels;
                    width = window_surface->w;
                    height = window_surface->h;
                    printf("Size changed: %d, %d\n", width, height);
                }
            }

            // Se o mouse é movimentado
            if(event.type == SDL_MOUSEMOTION)
            {
                // Mostra as posições x e y do mouse
                showMousePosition(window,event.motion.x,event.motion.y);
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
				/*Se o botão esquerdo do mouse é pressionado */
                if(event.button.button == SDL_BUTTON_LEFT)
				{
					printf("Mouse pressed on (%d,%d)\n",event.motion.x,event.motion.y) ;
				}
            }
        }

        // Seta a cor de fundo da janela para a informada nas
        // constantes VERMELHO, VERDE e AZUL
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                setPixel(x, y, RGB(VERMELHO,VERDE,AZUL));
            }
        }

        display();

        SDL_UpdateWindowSurface(window);
    }

    system("pause");
}
