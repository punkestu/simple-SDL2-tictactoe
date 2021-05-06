#include <SDL.h>
#include <iostream>

struct vect2d{
      int x ,y;
};

void layout(SDL_Renderer* renderer){
      for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                  SDL_Rect tile = {200*j,200*i,200,200};
                  SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                  SDL_RenderFillRect(renderer, &tile);
                  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                  if(j!=0){
                        SDL_RenderDrawLine(renderer, 200*j,0,200*j,600);
                  }
            }
            if(i!=0){
                  SDL_RenderDrawLine(renderer, 0,200*i,600,200*i);
            }
      }
}

vect2d getmousePos(){
      vect2d pos;
      SDL_GetMouseState(&pos.x, &pos.y);
      for (int i = 0; i < 3; i++)
      {
            if(pos.x > i*200 && pos.x < (i+1)*200){
                  pos.x = i;
                  break;
            }
      }
      for (int i = 0; i < 3; i++)
      {
            if(pos.y > i*200 && pos.y < (i+1)*200){
                  pos.y = i;
                  break;
            }
      }
      
      return pos;
}

bool mark(vect2d pos, int *tile, bool turn){
      if(tile[pos.y*3+pos.x]==0){
            tile[pos.y*3+pos.x] = turn?1:2;
            return true;
      }else{
            return false;
      }
      // SDL_Log("marked");
}

void drawMark(SDL_Renderer *renderer, int *tile){
      for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                  if(tile[i*3+j]==1){
                        //SDL_Log("have mark");
                        SDL_Rect _mark = {j*200+50, i*200+50, 100,100};
                        SDL_RenderDrawRect(renderer, &_mark);
                  }else if(tile[i*3+j]==2){
                        SDL_RenderDrawLine(renderer, j*200+50,i*200+50,j*200+150,i*200+150);
                        SDL_RenderDrawLine(renderer, j*200+150,i*200+50,j*200+50,i*200+150);
                  }
            }
      }
}

int check(int *tile){
      for(uint i = 0; i < 3; i++){
            if((tile[i*3] == tile[i*3+1] && tile[i*3] == tile[i*3+2] && tile[i*3]!=0)){
                  return tile[i*3];
            }
            if((tile[i] == tile[1*3+i] && tile[i] == tile[2*3+i] && tile[i]!=0)){
                  return tile[i];
            }
            if((tile[0] == tile[4] && tile[0] == tile[8] && tile[0]!=0)){
                  return tile[0];
            }
            if((tile[2] == tile[4] && tile[2] == tile[6] && tile[2]!=0)){
                  return tile[2];
            }
      }
      return 0;
}

int main(int argc, char* argv[]){
      SDL_Init(SDL_INIT_EVERYTHING);

      int tile[9] = {0,0,0,
                     0,0,0,
                     0,0,0};

      SDL_Window* window = SDL_CreateWindow("tictactoe",0,0,600,600,false);
      SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

      bool turn = true;
      bool finish = false;
      int winner = 0;
      while(!finish){
            SDL_Event e;
            SDL_PollEvent(&e);
            if(e.type == SDL_QUIT){
                  break;
            }

            for(uint i = 0; i < 9; i++){
                  if(tile[i]==0){
                        break;
                  }else{
                        if(i==8){
                              finish = true;
                        }
                  }
            }

            winner = check(tile);
            if(winner != 0){
                  finish = true;
            }

            if(finish){
                  SDL_Delay(250);
            }

            vect2d pos = getmousePos();
            // SDL_Log("%d : %d",pos.x,pos.y);
            if(e.type == SDL_MOUSEBUTTONDOWN){
                  if(mark(pos, tile, turn)){
                        turn = turn?false:true;
                  }
            }


            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            SDL_RenderClear(renderer);

            layout(renderer);
            drawMark(renderer, tile);

            SDL_RenderPresent(renderer);
      }
      while (finish)
      {
            SDL_Event e;
            SDL_PollEvent(&e);
            if(e.type == SDL_QUIT){
                  break;
            }
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            if(winner == 1 || winner == 0){
                  SDL_Rect _mark = {250, 250, 100,100};
                  SDL_RenderDrawRect(renderer, &_mark);
            }
            if(winner == 2 || winner == 0){
                  SDL_RenderDrawLine(renderer, 250,250,350,350);
                  SDL_RenderDrawLine(renderer, 350,250,250,350);
            }
            SDL_RenderPresent(renderer);
      }
      

      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      SDL_Quit();
      
      return 0;
}