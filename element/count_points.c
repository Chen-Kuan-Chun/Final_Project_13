/* ============================================================
   count_points.c – implementation v6 (row/col‑based sync, no owner)
   ------------------------------------------------------------ */
   #include "count_points.h"
   #include "bead.h"
   #include "boss_1.h"
   #include "../scene/gamescene.h"
   #include <allegro5/allegro.h>
   #include <allegro5/allegro_font.h>
   #include <allegro5/allegro_ttf.h>
   #include <stdlib.h>
   #include <time.h>
   
   extern Bead *bead_grid[GRID_ROWS][GRID_COLS];
   extern void move_bead_to(Bead *b,int row,int col);
   
   /* Parallel grid holding the parent Elements* for each bead */
   static Elements *ele_grid[GRID_ROWS][GRID_COLS] = {{NULL}};
   
   /* -------- 單例字型 ---------------------------------------------------- */
   static ALLEGRO_FONT *ensure_font(void){
       static ALLEGRO_FONT *f=NULL;
       if(!f){
           al_init_font_addon(); al_init_ttf_addon();
           f=al_load_ttf_font("assets/font/Consolas.ttf",28,0);
           if(!f) f=al_create_builtin_font();
       }
       return f;
   }
   
   /* -------- Manager 結構 ------------------------------------------------ */
   typedef struct {
       bool prev_e_down;
       int  total_removed;
   } MatchManager;
   
   /* -------- 盤面同步 (依 row/col 而非 x/y) ------------------------------ */
   static void sync_bead_grid(Scene *scene)
   {
       /* 清陣列 */
       for(int r=0;r<GRID_ROWS;r++)
           for(int c=0;c<GRID_COLS;c++){
               bead_grid[r][c]=NULL;
               ele_grid [r][c]=NULL;
           }
   
       /* 重新填入 */
       ElementVec beads=_Get_label_elements(scene,Bead_L);
       for(int i=0;i<beads.len;i++){
           Elements *ele=beads.arr[i];
           Bead *b=ele->pDerivedObj;
           int row=b->row;
           int col=b->col;
           if(row>=0&&row<GRID_ROWS&&col>=0&&col<GRID_COLS){
               bead_grid[row][col]=b;
               ele_grid [row][col]=ele;
           }
       }
   }
   
   /* -------- Collapse & Refill ----------------------------------------- */
   static void collapse_column(int col){
       int write=GRID_ROWS-1;
       for(int r=GRID_ROWS-1;r>=0;r--){
           Bead *b=bead_grid[r][col];
           Elements *e=ele_grid[r][col];
           if(b){
               if(r!=write){
                   bead_grid[r][col]=NULL; ele_grid[r][col]=NULL;
                   move_bead_to(b,write,col);
                   bead_grid[write][col]=b;
                   ele_grid [write][col]=e;
               }
               write--; }
       }
   }
   static void collapse_board(void){ for(int c=0;c<GRID_COLS;c++) collapse_column(c);} 
   
   static void refill_empty(Scene *scene){
       collapse_board();
       for(int c=0;c<GRID_COLS;c++){
           for(int r=0;r<GRID_ROWS;r++){
               if(bead_grid[r][c]==NULL){
                   int type=rand()%NUM_BEAD_TYPES;
                   Elements *nb=New_Bead(Bead_L,c,r,type);
                   _Register_elements(scene,nb);
                   Bead *b=nb->pDerivedObj;
                   bead_grid[r][c]=b;
                   ele_grid [r][c]=nb;
               }
           }
       }
   }
   
   /* -------- 掃描並標記 -------------------------------------------------- */
   static int mark_matches_and_count(Scene *scene){ (void)scene;
       int removed=0;
       /* horizontal */
       for(int r=0;r<GRID_ROWS;r++){
           int run_type=-1,run_start=0,run_len=0;
           for(int c=0;c<=GRID_COLS;c++){
               int cur=(c==GRID_COLS)?-2:(bead_grid[r][c]?bead_grid[r][c]->type:-1);
               if(cur==run_type){run_len++;}
               else{
                   if(run_len>=3 && run_type!=-1){
                       for(int k=0;k<run_len;k++){
                           Elements *e=ele_grid[r][run_start+k];
                           if(e){ e->dele=true; bead_grid[r][run_start+k]=NULL; ele_grid[r][run_start+k]=NULL; removed++; }
                       }
                   }
                   run_type=cur; run_start=c; run_len=1;
               }
           }
       }
       /* vertical */
       for(int c=0;c<GRID_COLS;c++){
           int run_type=-1,run_start=0,run_len=0;
           for(int r=0;r<=GRID_ROWS;r++){
               int cur=(r==GRID_ROWS)?-2:(bead_grid[r][c]?bead_grid[r][c]->type:-1);
               if(cur==run_type){run_len++;}
               else{
                   if(run_len>=3 && run_type!=-1){
                       for(int k=0;k<run_len;k++){
                           Elements *e=ele_grid[run_start+k][c];
                           if(e){ e->dele=true; bead_grid[run_start+k][c]=NULL; ele_grid[run_start+k][c]=NULL; removed++; }
                       }
                   }
                   run_type=cur; run_start=r; run_len=1;
               }
           }
       }
       return removed;
   }
   
   /* -------- 連鎖 -------------------------------------------------------- */
   static int chain_clear(Scene *scene){
       int total=0;
       sync_bead_grid(scene);
       while(1){
           int removed=mark_matches_and_count(scene);
           if(removed==0) break;
           total+=removed;
           refill_empty(scene);
           /* 每輪補珠後重新同步格子，避免座標未更新 */
           sync_bead_grid(scene);
       }
       return total;
   }
   
   /* -------- 元件接口 ---------------------------------------------------- */
   static void MatchManager_update(Elements *self){ (void)self; }
   
   static void MatchManager_interact(Elements *self){
       MatchManager *mm=self->pDerivedObj;
       extern Scene *scene;
       bool e_down=key_state[ALLEGRO_KEY_E];
       if(e_down && !mm->prev_e_down){
           int removed=chain_clear(scene);
           if(removed>0){
               mm->total_removed+=removed;
               ElementVec bosses=_Get_label_elements(scene,Boss1_L);
               for(int i=0;i<bosses.len;i++) Boss1_take_damage(bosses.arr[i],removed);
           }
           mm->prev_e_down=true;
       }
       if(!e_down) mm->prev_e_down=false;
   }
   
   static void MatchManager_draw(Elements *self){
       MatchManager *mm=self->pDerivedObj;
       ALLEGRO_FONT *f=ensure_font();
       char buf[32]; sprintf(buf,"counts:%d",mm->total_removed);
       al_draw_text(f,al_map_rgb(255,255,0),WIDTH-150,10,0,buf);
   }
   
   static void MatchManager_destroy(Elements *self){ free(self->pDerivedObj); free(self);} 
   
   Elements *New_MatchManager(int label){
       static bool seeded=false; if(!seeded){ srand(time(NULL)); seeded=true; }
       MatchManager *mm=calloc(1,sizeof(MatchManager));
       Elements *e=New_Elements(label);
       e->pDerivedObj=mm;
       e->Update=MatchManager_update;
       e->Interact=MatchManager_interact;
       e->Draw=MatchManager_draw;
       e->Destroy=MatchManager_destroy;
       return e;
   }