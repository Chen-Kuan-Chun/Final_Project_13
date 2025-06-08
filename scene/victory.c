/* ============================================================
   victory.c – victory screen (triggered when Boss HP <= 0)
   ------------------------------------------------------------ */
   #include <allegro5/allegro.h>
   #include <allegro5/allegro_audio.h>
   #include <allegro5/allegro_acodec.h>
   #include <allegro5/allegro_font.h>
   #include <allegro5/allegro_ttf.h>
   #include "scene.h"
   #include "sceneManager.h"
   
   typedef struct _Victory {
       ALLEGRO_FONT   *font;
       ALLEGRO_SAMPLE *song;
       ALLEGRO_SAMPLE_INSTANCE *sinst;
       int cx, cy;
   } Victory;
   
   static void victory_update(Scene *self){
       if(key_state[ALLEGRO_KEY_ESCAPE]){   /* press Enter to return menu */
           self->scene_end=true;
           window = 0;                     /* assume 0 = menu window */
       }
   }
   static void victory_draw(Scene *self){
       Victory *d=(Victory*)self->pDerivedObj;
       al_clear_to_color(al_map_rgb(0,0,0));
       al_draw_text(d->font,al_map_rgb(50,255,50),d->cx,d->cy-20,ALLEGRO_ALIGN_CENTRE,"YOU ARE VICTORY");
       al_draw_text(d->font,al_map_rgb(255,255,255),d->cx,d->cy+20,ALLEGRO_ALIGN_CENTRE,"Press ESC to return menu");
       al_play_sample_instance(d->sinst);
   }
   static void victory_destroy(Scene *self){
       Victory *d=(Victory*)self->pDerivedObj;
       al_destroy_font(d->font);
       al_destroy_sample(d->song);
       al_destroy_sample_instance(d->sinst);
       free(d); free(self);
   }
   
   Scene *New_Victory(int label){
       Victory *d=calloc(1,sizeof(Victory));
       Scene *s=New_Scene(label);
       /* font & sound */
       al_init_font_addon(); al_init_ttf_addon();
       d->font = al_load_ttf_font("assets/font/pirulen.ttf",24,0);
       if(!d->font) d->font = al_create_builtin_font();
       al_reserve_samples(8);
       d->song = al_load_sample("assets/sound/victory.mp3");
       d->sinst = al_create_sample_instance(d->song);
       al_set_sample_instance_playmode(d->sinst, ALLEGRO_PLAYMODE_LOOP);
       al_attach_sample_instance_to_mixer(d->sinst, al_get_default_mixer());
       al_set_sample_instance_gain(d->sinst,0.4);
       d->cx = WIDTH/2; d->cy = HEIGHT/2;
   
       s->pDerivedObj=d;
       s->Update = victory_update;
       s->Draw   = victory_draw;
       s->Destroy= victory_destroy;
       return s;
   }
   