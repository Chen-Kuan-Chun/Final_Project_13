/* ============================================================
   count_points.c – single‑step clear + gravity + refill
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
   
   /* ===== extern from bead.c ============================================ */
   extern void move_bead_to(Bead *b,int row,int col);
   
   /* ===== font single‑ton =============================================== */
   static ALLEGRO_FONT *ensure_font(void){
       static ALLEGRO_FONT *f=NULL;
       if(!f){ al_init_font_addon(); al_init_ttf_addon();
           f = al_load_ttf_font("assets/font/Consolas.ttf",28,0);
           if(!f) f = al_create_builtin_font(); }
       return f; }
   
   /* ===== helper temporary grids ======================================== */
   static Elements *ele_grid[GRID_ROWS][GRID_COLS];
   static int       type_grid[GRID_ROWS][GRID_COLS];
   
   static inline void clear_grids(void){
       for(int r=0;r<GRID_ROWS;r++)
           for(int c=0;c<GRID_COLS;c++){
               ele_grid [r][c]=NULL;
               type_grid[r][c]=-1; }}
   
   static void build_grids(Scene *scene){
       clear_grids();
       ElementVec beads = _Get_label_elements(scene,Bead_L);
       for(int i=0;i<beads.len;i++){
           Elements *e = beads.arr[i]; if(e->dele) continue;
           Bead *b = e->pDerivedObj;
           int row=b->row, col=b->col;  /* 直接用 row/col, 不靠 x/y */
           if(row>=0&&row<GRID_ROWS&&col>=0&&col<GRID_COLS){
               ele_grid [row][col]=e;
               type_grid[row][col]=b->type; }} }
   
   /* ===== match marking ================================================== */
   static int mark_matches(void){
       int removed=0;
       /* horizontal */
       for(int r=0;r<GRID_ROWS;r++){
           int run_t=-1,run_s=0,run_l=0;
           for(int c=0;c<=GRID_COLS;c++){
               int cur=(c==GRID_COLS)?-2:type_grid[r][c];
               if(cur==run_t) run_l++; else{
                   if(run_l>=3&&run_t!=-1){
                       for(int k=0;k<run_l;k++){
                           Elements *e=ele_grid[r][run_s+k];
                           if(e&&!e->dele){ e->dele=true; removed++; }
                           ele_grid [r][run_s+k]=NULL;
                           type_grid[r][run_s+k]=-1; }
                   }
                   run_t=cur; run_s=c; run_l=1; }} }
       /* vertical */
       for(int c=0;c<GRID_COLS;c++){
           int run_t=-1,run_s=0,run_l=0;
           for(int r=0;r<=GRID_ROWS;r++){
               int cur=(r==GRID_ROWS)?-2:type_grid[r][c];
               if(cur==run_t) run_l++; else{
                   if(run_l>=3&&run_t!=-1){
                       for(int k=0;k<run_l;k++){
                           Elements *e=ele_grid[run_s+k][c];
                           if(e&&!e->dele){ e->dele=true; removed++; }
                           ele_grid [run_s+k][c]=NULL;
                           type_grid[run_s+k][c]=-1; }
                   }
                   run_t=cur; run_s=r; run_l=1; }} }
       return removed; }
   
   /* ===== gravity collapse ============================================== */
   static void collapse_column(int col){
       int write=GRID_ROWS-1;
       for(int r=GRID_ROWS-1;r>=0;r--){
           if(type_grid[r][col]!=-1){  /* 有珠子 */
               if(r!=write){
                   /* 更新珠子的 row/col 以及 grids */
                   Elements *e=ele_grid[r][col]; Bead *b=e?e->pDerivedObj:NULL;
                   ele_grid[write][col]=e; type_grid[write][col]=type_grid[r][col];
                   ele_grid[r][col]=NULL;  type_grid[r][col]=-1;
                   if(b) move_bead_to(b,write,col);
               }
               write--; }
       }}
   static void collapse_board(void){ for(int c=0;c<GRID_COLS;c++) collapse_column(c);} 
   
   /* ===== refill empty slots (after collapse) ============================ */
   static void refill_empty(Scene *scene){
       for(int c=0;c<GRID_COLS;c++){
           for(int r=0;r<GRID_ROWS;r++){
               if(type_grid[r][c]==-1){
                   int type = rand()%NUM_BEAD_TYPES;
                   Elements *nb = New_Bead(Bead_L,c,r,type);
                   _Register_elements(scene,nb);
                   ele_grid [r][c]=nb;
                   type_grid[r][c]=type; } } } }
   
   /* ===== manager struct & methods ====================================== */
   typedef struct { bool prev_e; int total; } MatchManager;
   
   static void mm_update(Elements *self){ (void)self; }
   
   static void mm_interact(Elements *self){
       MatchManager *mm=self->pDerivedObj; extern Scene *scene;
       bool e_down=key_state[ALLEGRO_KEY_E];
       if(e_down && !mm->prev_e){
           build_grids(scene);
           int removed=mark_matches();
           if(removed>0){
               mm->total+=removed;
               ElementVec bosses=_Get_label_elements(scene,Boss1_L);
               for(int i=0;i<bosses.len;i++) Boss1_take_damage(bosses.arr[i],removed);
               al_rest(0.25);
               collapse_board();
               refill_empty(scene);
           }
           mm->prev_e=true;
       }
       if(!e_down) mm->prev_e=false; }
   
   static void mm_draw(Elements *self){
       MatchManager *mm=self->pDerivedObj;
       ALLEGRO_FONT *f=ensure_font();
       char buf[32]; sprintf(buf,"count:%d",mm->total);
       al_draw_text(f,al_map_rgb(255,255,0),WIDTH-150,10,0,buf); }
   
   static void mm_destroy(Elements *self){ free(self->pDerivedObj); free(self);} 
   
   Elements *New_MatchManager(int label){
       static bool seeded=false; if(!seeded){ srand(time(NULL)); seeded=true; }
       MatchManager *mm=calloc(1,sizeof(MatchManager));
       Elements *e=New_Elements(label);
       e->pDerivedObj=mm;
       e->Update = mm_update;
       e->Interact = mm_interact;
       e->Draw = mm_draw;
       e->Destroy = mm_destroy;
       return e; }
   