
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#define pointsNumber 10

const int width=800;
const int height=600;
const int pos_y=100;
const int pos_x=100;

typedef struct point{
double x[pointsNumber];
double y[pointsNumber];
struct point *next;
}point;


typedef struct List{
    point *first;
    point *last;
}List;

typedef struct RectangleElement{
    SDL_Rect *rect;
    struct RectangleElement *next;
} RectangleElement;

typedef struct Model{
 double a[pointsNumber];
 double b[pointsNumber];
 double learning_rate;
}Model;

List *creatList();

void insertBgeniing(RectangleElement **rectangles,SDL_Rect *rect );
RectangleElement*createRectangleElement(SDL_Rect *rect);
void drawRecttangles(SDL_Renderer *renderer,RectangleElement *rectangles );

Model *findModel(List *list);
point *creatNewElem(double x[], double y[]);
void addPOINT(double x[], double y[],List **l);
void onClick(double x[], double y[], List *list,SDL_Renderer *renderer);
void drawLine(double a[], double b[],SDL_Renderer *renderer);

int main(int argc, char* argv[]){
    List *l=NULL;
    l=creatList();
    double x[pointsNumber],y[pointsNumber];
    int i,j;

    //initialisation.
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){ printf("%s\n",SDL_GetError()); exit(-1);}

    RectangleElement *rectangles;rectangles=NULL;

    SDL_Window *window;
    window=SDL_CreateWindow("Linear regression",pos_x,pos_y,width,height,SDL_WINDOW_OPENGL);
    SDL_Event windowEvent;
    SDL_Renderer*renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    while(1)
    {
        if(SDL_PollEvent(&windowEvent))
        {
            if(windowEvent.type==SDL_QUIT)
            {
                break;
            }

            for(j=0;j<5;j++){
                printf("x%d= [",j);
                for(i=0;i<pointsNumber;i++){
                    x[i]= rand() % 800;
                    y[i]= height-rand()% 600;
                    printf("%f,",x[i]);
                }
                printf("]\n");
                // model...
                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                onClick( x,  y, l,renderer);
                drawRecttangles(renderer,rectangles);
            }
        }

        SDL_RenderPresent(renderer);
        if(getchar()) break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

return 0;
}



void onClick(double x[], double y[], List *list,SDL_Renderer *renderer){
    addPOINT(x,y,&list);
    Model *model ;
    model=findModel(list);
    double a[pointsNumber];double b[pointsNumber];

    printf("a=[");
    for(int i=0;i<pointsNumber;i++){
        a[i]=model->a[i];
        printf("%f,",a[i]);
        b[i]=model->b[i];
    }
    printf("]\n");

    drawLine(a,b,renderer);
}

Model* findModel(List*list){
    double learning_rate=0.01; /*vitesse d'apprentissage*/
    double real_yi,xi,predicted_y[pointsNumber],a[pointsNumber],b[pointsNumber],error[pointsNumber];
    point *p;
    Model *model;
    model=(Model*)malloc(sizeof(Model));
    for(int i=0;i<pointsNumber;i++){
            a[i] = 1;
            b[i] = 1;
    }
   /*a et be peuvent etre choisis de mani�re al�atoire au d�but*/
    for(p=list->first;p!=NULL;p=p->next){
        for(int i=0;i<pointsNumber;i++){
            xi=p->x[i];
            real_yi=p->y[i];
            printf("%.2f * %.2f \n",a[i],b[i]);
            predicted_y[i] = a[i] * xi + b[i];
            error[i] = real_yi - predicted_y[i];
            if(error[i]<0) error[i] =- error[i];
            a[i]=a[i]+1/xi*learning_rate*error[i];
            b[i]=b[i]+error[i]*learning_rate;
        }
    }
    for(int i=0;i<pointsNumber;i++){
        model->a[i]=a[i];
        model->b[i]=b[i];
    }
    model->learning_rate=learning_rate;
    return model;
}

List *creatList(){
    List *list;
    list=(List*)malloc(sizeof(List));
    list->first=NULL;
    list->last=NULL;
    return list;

}

point  *creatNewElem(double x[], double y[]){
    point  *pt;
    pt=(point*)malloc(sizeof(point));
    for(int i=0;i<pointsNumber;i++){
        pt->x[i] = x[i];
        pt->y[i] = y[i];
    }
    pt->next=NULL;
    return pt;
}

void addPOINT(double x[], double y[],List **l){
    point *E;
    E=creatNewElem(x,y);
    if((*l)->first==NULL){/*si la liste est vide*/
            (*l)->last=E;
            (*l)->first=E;
            return;
    }
/*si la liste est plein on ajoute a la fin de la liste*/
    (*l)->last->next=E;
    (*l)->last=E;

}

void insertBgeniing(RectangleElement **rectangles,SDL_Rect *rect )
{
    RectangleElement *newElement;
    newElement=createRectangleElement(rect);
    newElement->next=*rectangles;
    *rectangles=newElement;

}

void drawRecttangles(SDL_Renderer *renderer,RectangleElement *rectangles )
{
    RectangleElement *pt;
    for(pt=rectangles; pt!=NULL; pt=pt->next)
    {
        SDL_RenderFillRect(renderer,pt->rect);
    }
}
RectangleElement*createRectangleElement(SDL_Rect *rect)
{
    RectangleElement *newElement;
    newElement=(RectangleElement*)malloc(sizeof(RectangleElement));
    newElement->next=NULL;
    newElement->rect=rect;
    return newElement;
}


void drawLine(double a[], double b[],SDL_Renderer *renderer){
    double x1,x2,y1,y2;
    double a_means = 0.0,b_means = 0.0;
    for(int i=0;i<pointsNumber;i++){
        a_means += a[i];
        b_means += b[i];
    }
    a_means = a_means/pointsNumber ;
    b_means = b_means/pointsNumber;
    x1 =height/5;
    y1= height-(a_means*x1+b_means);
    x2= height-height/7;
    y2 = height - (a_means*x2+b_means);
    //printf("\ny1=%f, y2=%f\n",y1,y2);
    SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
}



