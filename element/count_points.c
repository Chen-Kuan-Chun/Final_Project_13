/* ============================================================
   count_points.c – implementation
   ------------------------------------------------------------ */
   #include "count_points.h"
   #include "bead.h"
   #include "boss_1.h"                /* apply damage */
   #include "damage.h"                /* optional, if you want to reuse font */
   #include "../scene/sceneManager.h"
   #include "../scene/gamescene.h"
   #include <allegro5/allegro.h>
   #include <allegro5/allegro_font.h>
   #include <allegro5/allegro_ttf.h>
   #include <stdio.h>
   #include <stdlib.h>
   
   /* ------------ internal struct ---------------------------------------- */
   typedef struct _MatchManager {
       bool prev_e_pressed;
       int  total_removed;     /* running total for UI & damage */
   } MatchManager;
   
   /* ------------ forward decl ------------------------------------------- */
   static void MatchManager_update(Elements *self);
   static void MatchManager_interact(Elements *self);
   static void MatchManager_draw(Elements *self);
   static void MatchManager_destroy(Elements *self) { free(self->pDerivedObj); free(self); }
   
   /* ------------ helpers ------------------------------------------------- */
   static ALLEGRO_FONT *ensure_font(void)
   {
       static ALLEGRO_FONT *f = NULL;
       if (!f) {
           al_init_font_addon();
           al_init_ttf_addon();
           f = al_load_ttf_font("assets/font/NotoSans-Bold.ttf", 28, 0);
           if (!f) f = al_create_builtin_font();
       }
       return f;
   }
   
   static void build_grid(Scene *scene,
                          int type_grid[GRID_ROWS][GRID_COLS],
                          Elements *ele_grid[GRID_ROWS][GRID_COLS])
   {
       for (int r=0;r<GRID_ROWS;r++)
           for (int c=0;c<GRID_COLS;c++)
               type_grid[r][c] = -1, ele_grid[r][c] = NULL;
   
       ElementVec beads = _Get_label_elements(scene, Bead_L);
       for (int i=0;i<beads.len;i++) {
           Elements *e = beads.arr[i];
           Bead     *b = e->pDerivedObj;
           int col = (int)((b->x - 230)/72);
           int row = (int)((b->y - 300)/72);
           if (row>=0 && row<GRID_ROWS && col>=0 && col<GRID_COLS) {
               type_grid[row][col]=b->type;
               ele_grid[row][col]=e;
           }
       }
   }
   
   /* return how many beads were marked (>=3 runs) */
   static int mark_matches_and_count(Scene *scene)
   {
       int grid[GRID_ROWS][GRID_COLS];
       Elements *ele[GRID_ROWS][GRID_COLS];
       build_grid(scene, grid, ele);
   
       int removed = 0;
   
       /* horizontal */
       for (int r=0;r<GRID_ROWS;r++) {
           int run_type=-1, run_start=0, run_len=0;
           for(int c=0;c<=GRID_COLS;c++){
               int cur = (c==GRID_COLS)?-2:grid[r][c];
               if(cur==run_type){run_len++;}
               else{
                   if(run_len>=3 && run_type!=-1){
                       for(int k=0;k<run_len;k++){
                           Elements *e = ele[r][run_start+k];
                           if(e && !e->dele){e->dele=true; removed++;}
                       }
                   }
                   run_type=cur; run_start=c; run_len=1;
               }
           }
       }
   
       /* vertical */
       for (int c=0;c<GRID_COLS;c++) {
           int run_type=-1, run_start=0, run_len=0;
           for(int r=0;r<=GRID_ROWS;r++){
               int cur = (r==GRID_ROWS)?-2:grid[r][c];
               if(cur==run_type){run_len++;}
               else{
                   if(run_len>=3 && run_type!=-1){
                       for(int k=0;k<run_len;k++){
                           Elements *e = ele[run_start+k][c];
                           if(e && !e->dele){e->dele=true; removed++;}
                       }
                   }
                   run_type=cur; run_start=r; run_len=1;
               }
           }
       }
       return removed;
   }
   
   /* ------------ ctor ---------------------------------------------------- */
   Elements *New_MatchManager(int label)
   {
       MatchManager *mm  = calloc(1,sizeof(MatchManager));
       Elements     *ele = New_Elements(label);
       ele->pDerivedObj = mm;
       ele->Update   = MatchManager_update;
       ele->Interact = MatchManager_interact;
       ele->Draw     = MatchManager_draw;
       ele->Destroy  = MatchManager_destroy;
       return ele;
   }
   
   /* ------------ Update / Interact / Draw ------------------------------- */
   static void MatchManager_update(Elements *self) { (void)self; }
   
   static void MatchManager_interact(Elements *self)
   {
       MatchManager *mm = self->pDerivedObj;
       extern Scene *scene;
       bool e_down = key_state[ALLEGRO_KEY_E];
       if (e_down && !mm->prev_e_pressed) {
           int removed = mark_matches_and_count(scene);
           if (removed>0) {
               mm->total_removed += removed;
               /* 對Boss造成傷害：一次扣 removed HP */
               ElementVec bosses = _Get_label_elements(scene, Boss1_L);
               for(int i=0;i<bosses.len;i++) Boss1_take_damage(bosses.arr[i], removed);
           }
       }
       mm->prev_e_pressed = e_down;
   }
   
   static void MatchManager_draw(Elements *self)
   {
       MatchManager *mm = self->pDerivedObj;
       ALLEGRO_FONT *f = ensure_font();
       char buf[32]; snprintf(buf,sizeof(buf),"消除：%d", mm->total_removed);
       /* 右上角顯示 */
       al_draw_text(f, al_map_rgb(255,255,0), WIDTH-140, 10, 0, buf);
   }