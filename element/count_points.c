/* ============================================================
   count_points.c – chain clear with elemental advantage
   ------------------------------------------------------------ */
   #include "count_points.h"
   #include "bead.h"
   #include "boss_1.h"              /* need boss->element */
   #include "../scene/gamescene.h"
   #include <allegro5/allegro.h>
   #include <allegro5/allegro_font.h>
   #include <allegro5/allegro_ttf.h>
   #include <stdlib.h>
   #include <time.h>
   
   /* ===== extern from bead.c ============================================ */
   extern void move_bead_to(Bead *b,int row,int col);
   
   /* ===== element table (all neutral =1) ================================ */
   /* 若日後想要屬性相剋，可在此調整；目前全部 1 */
   static const int elem_table[NUM_BEAD_TYPES][NUM_BEAD_TYPES] = {
   /* boss↓ / bead→  水 心 木 火 */
   /* 水 */  {1,1,1,1},
   /* 心 */  {1,1,1,1},
   /* 木 */  {1,1,1,1},
   /* 火 */  {1,1,1,1}};
   
   /* ===== bead 自身倍率：木×2，火×0.5 ================================ */
   static const float type_mult[NUM_BEAD_TYPES] = {
   /* 水 */ 1.0f,
   /* 心 */ 1.0f,
   /* 木 */ 2.0f,
   /* 火 */ 0.5f};
   
   /* ===== font singleton ================================================ */
   static ALLEGRO_FONT *ensure_font(void){
       static ALLEGRO_FONT *f=NULL;
       if(!f){ al_init_font_addon(); al_init_ttf_addon();
           f = al_load_ttf_font("assets/font/Consolas.ttf",28,0);
           if(!f) f = al_create_builtin_font(); }
       return f; }
   
   /* ===== helper grids =================================================== */
   static Elements *ele_grid[GRID_ROWS][GRID_COLS];
   static int       type_grid[GRID_ROWS][GRID_COLS];
   static inline void clear_grids(void){
       for(int r=0;r<GRID_ROWS;r++)
           for(int c=0;c<GRID_COLS;c++) ele_grid[r][c]=NULL,type_grid[r][c]=-1; }
   
   static void build_grids(Scene *scene){
       clear_grids();
       ElementVec beads=_Get_label_elements(scene,Bead_L);
       for(int i=0;i<beads.len;i++){
           Elements *e=beads.arr[i]; if(e->dele) continue;
           Bead *b=e->pDerivedObj;
           int row=b->row,col=b->col;
           if(row>=0&&row<GRID_ROWS&&col>=0&&col<GRID_COLS){
               ele_grid[row][col]=e; type_grid[row][col]=b->type; }} }
   
   /* ===== mark matches =================================================== */
   static int mark_matches(int *type_count){
       int removed = 0; bool mark[GRID_ROWS][GRID_COLS] = {{false}};
       /* horizontal */
       for(int r=0;r<GRID_ROWS;r++){int t=-1,s=0,l=0;for(int c=0;c<=GRID_COLS;c++){int cur=(c==GRID_COLS)?-2:type_grid[r][c];if(cur==t)l++;else{if(l>=3&&t!=-1)for(int k=0;k<l;k++)mark[r][s+k]=true;t=cur;s=c;l=1;}}}
       /* vertical */
       for(int c=0;c<GRID_COLS;c++){int t=-1,s=0,l=0;for(int r=0;r<=GRID_ROWS;r++){int cur=(r==GRID_ROWS)?-2:type_grid[r][c];if(cur==t)l++;else{if(l>=3&&t!=-1)for(int k=0;k<l;k++)mark[s+k][c]=true;t=cur;s=r;l=1;}}}
       /* apply */
       for(int r=0;r<GRID_ROWS;r++)
           for(int c=0;c<GRID_COLS;c++) if(mark[r][c]){
               int tp=type_grid[r][c]; if(tp>=0) type_count[tp]++;
               Elements *e=ele_grid[r][c]; if(e&&!e->dele){e->dele=true; removed++;}
               ele_grid[r][c]=NULL; type_grid[r][c]=-1; }
       return removed; }
   
   /* ===== gravity / refill ============================================== */
   static void collapse_column(int col){int w=GRID_ROWS-1;for(int r=GRID_ROWS-1;r>=0;r--){if(type_grid[r][col]!=-1){if(r!=w){Elements *e=ele_grid[r][col];Bead*b=e?e->pDerivedObj:NULL;ele_grid[w][col]=e;type_grid[w][col]=type_grid[r][col];ele_grid[r][col]=NULL;type_grid[r][col]=-1;if(b)move_bead_to(b,w,col);}w--;}}}
   static void collapse_board(void){for(int c=0;c<GRID_COLS;c++)collapse_column(c);} 
   static void refill_until_full(Scene*scene){while(1){bool gap=false;for(int c=0;c<GRID_COLS&&!gap;c++)for(int r=0;r<GRID_ROWS;r++)if(type_grid[r][c]==-1){gap=true;break;}if(!gap)break;for(int c=0;c<GRID_COLS;c++)for(int r=GRID_ROWS-1;r>=0;r--)if(type_grid[r][c]==-1){int type=rand()%NUM_BEAD_TYPES;Elements*nb=New_Bead(Bead_L,c,r,type);_Register_elements(scene,nb);ele_grid[r][c]=nb;type_grid[r][c]=type;}collapse_board();}}
   
   /* ===== manager ======================================================== */
   typedef struct { bool prev_e; int total; } MatchManager;
   
   static void chain_clear(Scene *scene, MatchManager *mm){
       int round_removed=0; int type_count[NUM_BEAD_TYPES]={0};
       do{
           build_grids(scene);
           int removed = mark_matches(type_count);
           if(!removed) break;
           /* move deleted beads out */
           ElementVec bead_vec=_Get_label_elements(scene,Bead_L);
           for(int i=0;i<bead_vec.len;i++) if(bead_vec.arr[i]->dele){Bead*b=bead_vec.arr[i]->pDerivedObj;move_bead_to(b,-1,-1);} 
           round_removed += removed;
           collapse_board(); refill_until_full(scene);
       }while(1);
   
       if(round_removed){
           mm->total += round_removed;
           ElementVec bosses=_Get_label_elements(scene,Boss1_L);
           for(int i=0;i<bosses.len;i++){
               Boss1 *bs=bosses.arr[i]->pDerivedObj; int battr=bs->element; int dmg=0;
               for(int t=0;t<NUM_BEAD_TYPES;t++){
                   int eff = elem_table[t][battr];
                   float part = type_count[t] * eff * type_mult[t];
                   dmg += (int)(part + 0.5f);
               }
               if(dmg==0) dmg=1; Boss1_take_damage(bosses.arr[i], dmg);
           }
       }
   }
   
   static void mm_update(Elements *self){ (void)self; }
   static void mm_interact(Elements *self){
       MatchManager *mm=self->pDerivedObj; extern Scene *scene;
       bool e_down=key_state[ALLEGRO_KEY_E];
       if(e_down && !mm->prev_e){ chain_clear(scene,mm); mm->prev_e=true; }
       if(!e_down) mm->prev_e=false; }
   static void mm_draw(Elements *self){
       MatchManager *mm=self->pDerivedObj; ALLEGRO_FONT *f=ensure_font();
       char buf[32]; sprintf(buf,"消除：%d",mm->total);
       al_draw_text(f,al_map_rgb(255,255,0),WIDTH-150,10,0,buf); }
   static void mm_destroy(Elements *self){ free(self->pDerivedObj); free(self);} 
   Elements *New_MatchManager(int label){
       static bool seeded=false; if(!seeded){ srand(time(NULL)); seeded=true; }
       MatchManager *mm=calloc(1,sizeof(MatchManager)); Elements *e=New_Elements(label);
       e->pDerivedObj=mm; e->Update=mm_update; e->Interact=mm_interact; e->Draw=mm_draw; e->Destroy=mm_destroy; return e; }
   