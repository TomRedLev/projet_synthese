/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr  
       - Librairie G3X - Fonctions de base d'accès public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <sys/times.h>
#include <unistd.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <g3x.h>

#define BLOCSIZE 32
#define MAXBLOC  32

/* tailles par defaut de la fenetre gl */
#define DFT_PIXHEIGHT (GLint)512
#define DFT_PIXWIDTH  (GLint)512

/* tailles des bandeaux */
#define DFT_DIALWIDTH ((GLint)12)

static GLint   xdialwin,ydialwin,cornwin,drawwin,mainwin;
static bool    xdialflag=false, ydialflag=false, cornflag=false;
static char    _WINDNAME_[128]="";

/* tailles de la fenetre : dim. en pixels     */
static GLint     curheight =DFT_PIXHEIGHT; /* */
static GLint     pixheight =DFT_PIXHEIGHT; /* */
static GLint     curwidth  =DFT_PIXWIDTH;  /* */
static GLint     pixwidth  =DFT_PIXWIDTH;  /* */
static GLint     fullwidth,fullheight;

/* PARAMETRES DU BANDEAU LATERAL : position des boutons et scrollbars */
static GLint     Ydialwidth=0,Xdialheight=0; 
static GLint     Xbutpos   =0,Ybutpos=12;
static GLint     Xbutw     =0;

/*static int       KeyMode=PARAM;*/
static GLuint   pushmode=GLUT_UP;
static GLint    moving;

static bool  _IDLE_       = false;
static bool  _RUN_        = false ;
static bool  _AUTODISPLAY_= true ;
static bool  _FULLSCREEN_ = false;
static bool  _REPERE_     = false;
static bool  _FILL_       = true ;
static bool  _INFO_       = false;

/*=================================================*/
/*=           fonctions d'écriture                =*/
/*=================================================*/
static void* font =  GLUT_BITMAP_HELVETICA_10;
extern void g3x_SelectFont(int size)
{
  if (size<11) { font=GLUT_BITMAP_HELVETICA_10; return; }
  if (size<15) { font=GLUT_BITMAP_HELVETICA_12; return; }
  font=GLUT_BITMAP_HELVETICA_18;  
}

extern void g3x_Print(int x, int y, const char *msg)
{
  int i=0;
  glRasterPos2i(x,y); 
  while (msg[i]!='\0')
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,msg[i]); 
    i++;
  }
}

extern void g3x_Write(const char *msg, int x, int y, G3Xcolor col)
{
  int i=0;
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5,639.5,-0.5,479.5,-1.0,1.0);
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  glColor4f(col.r,col.g,col.b,col.a);
  glRasterPos2i(x,y); 
  while (msg[i]!='\0')
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,msg[i]); 
    i++;
  }
  
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
}

/*=================================================*/
#include "Cinc/g3x_camlight.c"
#include "Cinc/g3x_ctrlprm.c"
#include "Cinc/g3x_keys.c"
#include "Cinc/g3x_popup.c"
#include "Cinc/g3x_button.c"
#include "Cinc/g3x_switch.c"
#include "Cinc/g3x_scroll.c"

/*=================================================*/

/*= prototypes =*/
static void (*g3x_Init)(void)=NULL;             /*! fonction d'initialisation !*/
static void (*g3x_Idle)(void)=NULL;             /*! fonction d'animation      !*/
static void (*g3x_Draw)(void)=NULL;             /*! fonction de dessin        !*/
static void (*g3x_GarbageCollector)(void)=NULL; /*! fonction de liberation    !*/

extern void g3x_Quit(void);                     /*! fonction de sortie        !*/

extern int  g3x_GetPixWidth(void) { return curwidth;  } /*! largeur courante  !*/
extern int  g3x_GetPixHeight(void){ return curheight; } /*! hauteur coutante  !*/


/*=======================================*/
/*= FONCTIONS DE GESTION DES EVENEMENTS =*/
/*=======================================*/
extern bool g3x_Running(void)  { return _RUN_; } 
extern void g3x_Stop(void)     { _RUN_=false;  }
extern void g3x_Continue(void) { _RUN_=true;   }
extern void g3x_Show(void)     { glutPostWindowRedisplay(drawwin); }

/*= automatisation - rarement utilisée =*/
extern void g3x_SetAutoDisplay(bool flag) { _AUTODISPLAY_=flag; }
extern void g3x_ToggleAutoDisplay(void)   { _AUTODISPLAY_=!_AUTODISPLAY_; }
static int   _VIDEO_    = 0;
extern void g3x_AutoStartVideo(void)      { _RUN_=true; _VIDEO_=true; }
/*= snapshot attaché à des touches ('j'/'p')  =*/
extern void g3x_AutoSnap_jpg(void) { g3x_Snapshot("jpg",_WINDNAME_,curwidth,curheight); }
extern void g3x_AutoSnap_png(void) { g3x_Snapshot("png",_WINDNAME_,curwidth,curheight); }

/*= - DrawFunc   : procedure d'affichage, associee a <glutPostRedisplay>             =*/
extern void g3x_SetDrawFunction(void (*f)(void)) { g3x_Draw = f; }
/*= - FreeFunc   : procedure de liberation de memoire, appelee a la sortie           =*/
extern void g3x_SetExitFunction(void (*f)(void)) { g3x_GarbageCollector = f; }
/*= - IdleFunc   : procedure de la boucle d'action Gl, passee a <glutIdleFunc>       =*/
extern void g3x_SetAnimFunction(void (*f)(void)) { g3x_Idle=f; _IDLE_=true; }
/*= - Fonction d'initialisation                                                      =*/
extern void g3x_SetInitFunction(void (*f)(void)) { g3x_Init=f; }

/* fréquence d'affichage */
static int  _FAFF_=1;
/* par défaut : tous les pas de calcul sont affichés */
static void _idle_0_(void) 
{ 
  g3x_Idle();
  glutPostWindowRedisplay(drawwin);
}

/* affichage temporisé                               */
static void _idle_F_(void) 
{ 
  static int t=0;
  g3x_Idle();
  if (t==_FAFF_) { glutPostWindowRedisplay(drawwin); t=0; }
  t++;
}
/* règle la fréquence d'affichage et sélctionne la bonne <idle func>  */
extern void g3x_SetRefreshFreq(int freq) 
{ 
  if (_FAFF_==freq) return;
  _FAFF_=freq;
  glutIdleFunc((_IDLE_&&_RUN_)?(_FAFF_>1?_idle_F_:_idle_0_):NULL); 
}


/*==================================*/
/*=  RESHAPE                       =*/
/*==================================*/
static __inline__ void g3x_CornReshape(int w, int h)
{
  glViewport (0, 0, w, h);
}

static __inline__ void g3x_XDialReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  if (_SCROLLH_) g3x_SetUpScrollh();
}

static __inline__ void g3x_YDialReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  if (_SCROLLV_) g3x_SetUpScrollv();
}

static __inline__ void g3x_DrawReshape(int w, int h)
{ 
  glViewport (0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(_near,(GLfloat)w/(GLfloat)h,_open,_far );     
}

static __inline__ void g3x_MainReshape(int w, int h)
{
  glViewport (0, 0, w, h);

  curwidth  = w-Ydialwidth;
  curheight = h-Xdialheight; 

  if (cornflag)
  {
    glutSetWindow (cornwin);
    glutReshapeWindow(Ydialwidth,Xdialheight);
    glutPositionWindow(curwidth,curheight);
    glutShowWindow();
  }
  if (xdialflag)
  {
    glutSetWindow (xdialwin);
    glutReshapeWindow(curwidth,Xdialheight);
    glutPositionWindow(0,curheight);
    glutShowWindow();
  }
  if (ydialflag)
  {
    glutSetWindow (ydialwin);
    glutReshapeWindow(Ydialwidth,curheight);
    glutPositionWindow(curwidth,0);
    glutShowWindow();
  }
  glutSetWindow (drawwin);
  glutReshapeWindow(curwidth,curheight);
  glutPositionWindow(0,0);
  glutShowWindow();
}


/*==================================*/
/*=  DISPLAY                       =*/
/*==================================*/
static __inline__ void g3x_CornDisplay(void)
{
  glutSetWindow (cornwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)Ydialwidth,0.,(double)Xdialheight);
  glClearColor(.75,.80,.85,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  g3x_DrawInfoSwitch();
  glutSwapBuffers();
}

static __inline__ void g3x_XDialDisplay(void)
{
  glutSetWindow (xdialwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)curwidth,0.,(double)Xdialheight);

  glClearColor(.75,.80,.85,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  G3Xscroll *scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+scrollhnum) 
  {
    g3x_DrawScrollh(scroll);
    scroll++;
  }  
  glutSwapBuffers();
}

static __inline__ void g3x_YDialDisplay(void)
{
  glutSetWindow (ydialwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)Ydialwidth,0.,(double)curheight);
  glClearColor(.75,.80,.85,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  if (_BUTTON_) g3x_DrawButtons();
  if (_POPUP_ ) g3x_DrawPopUps();
  if (_SWITCH_) g3x_DrawSwitches();
  G3Xscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum) 
  {
    g3x_DrawScrollv(scroll);
    scroll++;
  }  
  glutSwapBuffers();
}

/*= Couleur de fond =*/
static GLfloat G3X_BKG=0.0;
extern void    g3x_SetBkgCol(double bkg) { G3X_BKG=(GLfloat)bkg;  }
extern double  g3x_GetBkgCol(void      ) { return (double)G3X_BKG; }

/*====================================================================*/
/*=                        tracé des axes                            =*/
/*====================================================================*/
extern void g3x_Arrow(GLfloat r, GLfloat h, G3Xcolor col)
{
  static GLUquadricObj *qobj=NULL;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glPushMatrix();
    if (!qobj) qobj=gluNewQuadric();
    g3x_Materialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(G3Xcolor){0.,0.,0.,0.});
    g3x_Materialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,col);
    glMaterialf(GL_FRONT_AND_BACK,GL_SPECULAR,0.);
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluCylinder(qobj,r,r,.8*h,8,1);
    glTranslatef(0.,0.,.8*h);
    glutSolidCone(4.*r,.2*h,4,1);
  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,_FILL_?GL_FILL:GL_LINE);
}

extern void g3x_ArrowVect(G3Xpoint O, G3Xvector V, GLfloat r, GLfloat h, G3Xcolor col)
{
  static GLUquadricObj *qobj=NULL;
  if (!qobj) qobj=gluNewQuadric();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  double l,theta,phi,sinphi;
  l=g3x_Norm(V);
  phi=acos(V.z/l);
  sinphi=sin(phi);
  theta=(G3Xiszero(sinphi)?0.:(V.y>0?+1.:-1.)*acos(V.x/(l*sinphi)));
  glPushMatrix();
    glTranslatef(O.x,O.y,O.z);
    glPushMatrix();
      glRotated(RadToDeg*theta,0.,0.,1.);
      glPushMatrix();
        glRotated(RadToDeg*phi,0.,1.,0.);
        glPushMatrix();
          g3x_Materialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(G3Xcolor){0.1, 0.4, 0.0, 1.0});
          col.a=1.;
          g3x_Materialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,col);
          glMaterialf(GL_FRONT_AND_BACK,GL_SPECULAR,0.0);
          gluQuadricDrawStyle(qobj,GLU_FILL);
          gluCylinder(qobj,r,r,.8*h,8,2);
          glTranslatef(0.,0.,.8*h);
          glutSolidCone(4.*r,.2*h,10,1);
        glPopMatrix();  
      glPopMatrix();  
    glPopMatrix();
  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,_FILL_?GL_FILL:GL_LINE);
}

/*= affichage du repère =*/
static __inline__ void g3x_Repere(void)
{
  static G3Xcolor rouge=(G3Xcolor){0.6,0.0,0.0,1.0};
  static G3Xcolor vert =(G3Xcolor){0.0,0.6,0.0,1.0};
  static G3Xcolor bleu =(G3Xcolor){0.0,0.0,0.6,1.0};
  glPushMatrix();
    g3x_Arrow(0.025,1.0,bleu); /*axe z (bleu) */
    glPushMatrix();
      glRotatef(-90.,1.,0.,0.);
      g3x_Arrow(0.025,1.0,vert); /* axe y (vert) */
    glPopMatrix();  
    glPushMatrix();
      glRotatef(+90.,0.,1.,0.);
      g3x_Arrow(0.025,1.0,rouge); /* axe x (rouge) */
    glPopMatrix();  
  glPopMatrix();  
}

static __inline__ void g3x_Info(bool i)
{
  static char tmp[128];
  static int k;
  if (i)
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5,639.5,-0.5,479.5,-1.0,1.0);
      glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.5,0.5,0.5,0.5);
    glRecti(10,10,630,470);
    glDisable(GL_BLEND);

    glColor3f(1.,1.,1.);

    int y=460;
    sprintf(tmp,"Param.  : ");  glColor3f(.33,.66,.99); g3x_Print(15,y,tmp);    
    glColor3f(1.,1.,1.);
    if (ThePrm!=_PRM_+prmnum && ThePrm->var.r)
      sprintf(tmp,"%s : %+3.3f<%+3.3f<%+3.3f" ,ThePrm->nom,ThePrm->min.r,*(ThePrm->var.r),ThePrm->max.r);  
    else if (ThePrm!=_PRM_+prmnum && ThePrm->var.i)
      sprintf(tmp,"%s : %+d<%+d<%+d" ,ThePrm->nom,ThePrm->min.i,*(ThePrm->var.i),ThePrm->max.i); 
    else 
      sprintf(tmp,"visee     [%+3.3f,%+3.3f,%+3.3f]",(cam.tar->x),cam.tar->y,cam.tar->z);
    g3x_Print(150,y,tmp);  y-=12;
    
    sprintf(tmp,"Camera  : "); glColor3f(.33,.66,.99); g3x_Print(15,y,tmp); 
    glColor3f(1.,1.,1.);
    sprintf(tmp,"pos.cart. "); g3x_Print(60,y,tmp); sprintf(tmp,"[%+3.3f,%+3.3f,%+3.3f]",cam.pos->x,cam.pos->y,cam.pos->z);    g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"pos.spher."); g3x_Print(60,y,tmp); sprintf(tmp,"[%+3.3f,%+3.3f,%+3.3f]",cam.theta ,cam.phi,cam.dist);                  g3x_Print(150,y,tmp);  y-=12;

    sprintf(tmp,"Lumiere : "); glColor3f(.33,.66,.99); g3x_Print(15,y,tmp);
    glColor3f(1.,1.,1.);
    sprintf(tmp,"pos.cart. "); g3x_Print(60,y,tmp); sprintf(tmp,"[%+3.3f,%+3.3f,%+3.3f]",spot.pos.x,spot.pos.y,spot.pos.z); g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"pos.spher."); g3x_Print(60,y,tmp); sprintf(tmp,"[%+3.3f,%+3.3f,%+3.3f]",spot.theta ,spot.phi,spot.dist);               g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"ambient   "); g3x_Print(60,y,tmp); sprintf(tmp,"[%+3.3f,%+3.3f,%+3.3f]",spot.ambi.r,spot.ambi.g,spot.ambi.b);       g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"diffuse   "); g3x_Print(60,y,tmp); sprintf(tmp,"[%+3.3f,%+3.3f,%+3.3f]",spot.diff.r,spot.diff.g,spot.diff.b);       g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"specular  "); g3x_Print(60,y,tmp); sprintf(tmp,"[%+3.3f,%+3.3f,%+3.3f0",spot.spec.r,spot.spec.g,spot.spec.b);       g3x_Print(150,y,tmp);  y-=12;

    sprintf(tmp,"Clavier : "); glColor3f(.33,.66,.99); g3x_Print(15,y,tmp); 
    glColor3f(1.,1.,1.);
    sprintf(tmp,"<Ctrl+q>/<esc> ");  g3x_Print(60,y,tmp); sprintf(tmp,"sortie du programme     ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"<tab>          ");  g3x_Print(60,y,tmp); sprintf(tmp,"mode plein/filaire      ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"<?>            ");  g3x_Print(60,y,tmp); sprintf(tmp,"affiche/masque l'aide   ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"<Ctrl+f>       ");  g3x_Print(60,y,tmp); sprintf(tmp,"plein ecran             ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"<Ctrl+r>       ");  g3x_Print(60,y,tmp); sprintf(tmp,"affiche/masque le repere");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"<Ctrl+w>       ");  g3x_Print(60,y,tmp); sprintf(tmp,"inverse la coul. de fond");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"4/6            ");  g3x_Print(60,y,tmp); sprintf(tmp,"pos. camera theta (-/+) ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"2/8            ");  g3x_Print(60,y,tmp); sprintf(tmp,"pos. camera phi   (-/+) ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"3/9            ");  g3x_Print(60,y,tmp); sprintf(tmp,"pos. camera dist. (-/+) ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"d/D            ");  g3x_Print(60,y,tmp); sprintf(tmp,"pos. lumiere en z (-/+) ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"x/X            ");  g3x_Print(60,y,tmp); sprintf(tmp,"pos. lumiere en x (-/+) ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"y/Y            ");  g3x_Print(60,y,tmp); sprintf(tmp,"pos. lumiere en y (-/+) ");  g3x_Print(150,y,tmp);  y-=12;
    sprintf(tmp,"z/Z            ");  g3x_Print(60,y,tmp); sprintf(tmp,"pos. lumiere en z (-/+) ");  g3x_Print(150,y,tmp);  y-=12;
    if (keynumb>0)
    {
      sprintf(tmp,"Actions : ");    glColor3f(.33,.66,.99); g3x_Print(15,y,tmp);
      glColor3f(1.,1.,1.);
      for (k=0; k<keynumb; k++)
      {  
      sprintf(tmp,"'%c'       %s",_KEY_[k].key,_KEY_[k].info);  g3x_Print(150,y,tmp);  y-=12;
      }
    }
  }  
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  
  g3x_DrawReshape(curwidth,curheight);
}

static __inline__ void g3x_Info_Xterm(bool i)
{
  static char tmp[128];
  static int k,l=0;
  if (i)
  {
    if (ThePrm!=_PRM_+prmnum && ThePrm->var.r)
    {l++;fprintf(stderr,"\e[1;33mParam.  : \e[0;36m%s : \e[0;31m%+3.3f<\e[0;41m%+3.3f<\e[0;31m%+3.3f                  \n"
                       ,ThePrm->nom,ThePrm->min.r,*(ThePrm->var.r),ThePrm->max.r); 
    }
    else if (ThePrm!=_PRM_+prmnum && ThePrm->var.i)
    {l++;fprintf(stderr,"\e[1;33mParam.  : \e[0;36m\"%s\" : \e[0;31m %+d<\e[0;41m%+d\e[0;31m<%+d                       \n"
                       ,ThePrm->nom,ThePrm->min.i,*(ThePrm->var.i),ThePrm->max.i); 
    }
    else
    {l++;fprintf(stderr,"\e[1;33mParam   : \e[0;36mvisee     \e[0;31m[%+3.3f,%+3.3f,%+3.3f]                   \n"
                       ,(cam.tar->x),cam.tar->y,cam.tar->z);
    }                   
    l++;fprintf(stderr,"\e[1;33m                                                                             \n\e[0;0m");
    l++;fprintf(stderr,"\e[1;33mCamera  : \e[0;35mpos.cart.     \e[0;31m[%+3.3f,%+3.3f,%+3.3f]                   \n",(cam.pos->x),cam.pos->y,cam.pos->z);
    l++;fprintf(stderr,"\e[1;33m          \e[0;35mpos.spher.    \e[0;31m[%+3.3f,%+3.3f,%+3.3f]                   \n",cam.theta ,cam.phi,cam.dist);
    l++;fprintf(stderr,"\e[1;33mLumiere : \e[0;35mposition      \e[0;31m[%+3.3f,%+3.3f,%+3.3f]                   \n",spot.pos.x,spot.pos.y,spot.pos.z);
    l++;fprintf(stderr,"\e[1;33m          \e[0;35mambient       \e[0;31m[%+3.3f,%+3.3f,%+3.3f]                   \n",spot.ambi.r,spot.ambi.g,spot.ambi.b);
    l++;fprintf(stderr,"\e[1;33m          \e[0;35mdiffuse       \e[0;31m[%+3.3f,%+3.3f,%+3.3f]                   \n",spot.diff.r,spot.diff.g,spot.diff.b);
    l++;fprintf(stderr,"\e[1;33m          \e[0;35mspecular      \e[0;31m[%+3.3f,%+3.3f,%+3.3f]                   \n" ,spot.spec.r,spot.spec.g,spot.spec.b);
    l++;fprintf(stderr,"\e[1;33m                                                                             \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33mClavier : \e[0;35m<Ctrl+q>/<esc>\e[0;2msortie du programme                       \e[0;0m\n");
    l++;fprintf(stderr,"\e[133m;          \e[0;35m<Ctrl+w>      \e[0;2minverse la couleur de fond                \e[0;0m\n");
    l++;fprintf(stderr,"\e[133m;          \e[0;35m<tab>         \e[0;2mmode plein/filaire                        \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35m<ent>         \e[0;2maffiche/masque le repere                  \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35m?/i           \e[0;2maffiche/masque l'aide                     \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35m4/6           \e[0;2mpos. camera theta (-/+)                   \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35m2/8           \e[0;2mpos. camera phi   (-/+)                   \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35m4/6           \e[0;2mpos. camera dist. (-/+)                   \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35md/D           \e[0;2mpos. lumiere en z (-/+)                   \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35mx/X           \e[0;2mpos. lumiere en x (-/+)                   \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35my/Y           \e[0;2mpos. lumiere en y (-/+)                   \e[0;0m\n");
    l++;fprintf(stderr,"\e[1;33m          \e[0;35mz/Z           \e[0;2mpos. lumiere en z (-/+)                   \e[0;0m\n");
    if (keynumb>0)
    {
      l++;fprintf(stderr,"\e[1;33m Actions : \e[0;36m'%c'       \e[0;2m%s\e[0;0m                                      \n",_KEY_[0].key,_KEY_[0].info);
      for (k=1; k<keynumb; k++)
      {  
        l++;fprintf(stderr,"\e[1;33m           \e[0;36m'%c'       \e[0;2m%s\e[0;0m                                      \n",_KEY_[k].key,_KEY_[k].info);
      }
      l++;fprintf(stderr,"\e[1;33m                                                                                 \e[0;0m\n");
    }
  }  
  else
    for (k=0; k<l; k++) fprintf(stderr,"                                                                                                \n");

  sprintf(tmp,"\e[0m\e[%dA\n",l);
  fprintf(stderr,"%s",tmp);
}

extern int g3x_tempo(double tempo) 
{
  char command[64]="";
  sprintf(command,"sleep %lf",tempo);
  return system(command);  
}


/*========================================================*/
/*=                       FENETRE 3D                     =*/
/*========================================================*/
static __inline__ void g3x_DrawDisplay(void)
{
  glutSetWindow (drawwin);
  glClearColor(G3X_BKG,G3X_BKG,G3X_BKG,1.);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cam.pos->x,cam.pos->y,cam.pos->z,
            cam.tar->x,cam.tar->y,cam.tar->z,
            cam.upv .x,cam.upv .y,cam.upv .z);

  glEnable(GL_LIGHTING);
  glPolygonMode(GL_FRONT_AND_BACK,_FILL_?GL_FILL:GL_LINE);
  GLfloat _POS[4]={spot.pos.x,spot.pos.y,spot.pos.z,1. };
  glLightfv(GL_LIGHT0,GL_POSITION,_POS);
  
  glPushMatrix();
    g3x_Draw();
  glPopMatrix();
  if (_info_switch_) g3x_Info(true);
  if (_REPERE_)      g3x_Repere();
  
  glFlush();
  glutSwapBuffers();
  if (_VIDEO_ && !g3x_FilmFrame()) g3x_Quit();
}

static __inline__ void g3x_MainDisplay(void)
{
  if (cornflag ) g3x_CornDisplay();
  if (xdialflag) g3x_XDialDisplay();
  if (ydialflag) g3x_YDialDisplay();
  g3x_DrawDisplay();
}

/*====================================================================*/
/*=      Gestion des 'clic' souris                                   =*/
/*====================================================================*/
static __inline__ void g3x_CornMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(cornwin);
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  _info_switch_=!_info_switch_;
  glutPostRedisplay();
  glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
  return;
}

static __inline__ void g3x_XDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(xdialwin);
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  /*! clic Scrollbar!*/
  G3Xscroll *scroll=_SCROLLH_;
  y = Xdialheight-y;
  while (scroll<_SCROLLH_+scrollhnum && pushmode==GLUT_UP)
  {
    if (abs((int)scroll->ycurs-y)<scroll->w && x>(Xscrollstart-2*scroll->w) && x<(Xscrollend  +2*scroll->w))
    {
      moving = true+button;
      pushmode=GLUT_DOWN;
      TheScrollh=scroll;
      TheScrollh->xcurs  = CLIP(Xscrollstart,x,Xscrollend);
      TheScrollh->cursor = (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
      if (TheScrollh->dprm)
         *TheScrollh->dprm   = (double)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
      else
      if (TheScrollh->iprm)
         *TheScrollh->iprm   = (   int)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
      glutPostRedisplay();
      glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
      return;
    }
    scroll++;
  }
  return;
}

static __inline__ void g3x_YDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(ydialwin);
  if (button==GLUT_MIDDLE_BUTTON) { moving=true+button; return; }
  if (button!=GLUT_LEFT_BUTTON  ) return;

  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  /*! clic boutons!*/
  if (g3x_SelectPopUp(x,y) || g3x_SelectSwitch(x,y) || g3x_SelectButton(x,y))
  {
    if (ThePopUp && ThePopUp->on)
    {
      ThePopUp->action(); 
      ThePopUp->on=false; 
    }  
    ThePopUp =NULL;
    ThePopUp=NULL;
    glutPostRedisplay();
    glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
    return;
  }  
  /*! clic Scrollbar!*/
  y = curheight-y;
  G3Xscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum && pushmode==GLUT_UP)
  {
    if (abs((int)scroll->xcurs-x)<scroll->w && y>(Yscrollstart-2*scroll->w) 
                                            && y<(Yscrollend  +2*scroll->w) )
    {
      moving = true+button;
      pushmode=GLUT_DOWN;
      TheScrollv=scroll;
      TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
      TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
      if (TheScrollv->dprm)
         *TheScrollv->dprm   = (double)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
      else
      if (TheScrollv->iprm)
         *TheScrollv->iprm   = (   int)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
      glutPostRedisplay();
      glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
        return;
    }
    scroll++;
  }
  return;
}

static G3Xpoint _CLIC_POS_ = { 0., 0., 0. };
static GLint startx,starty;

static __inline__ void g3x_DrawMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(drawwin);
  if (state == GLUT_UP) return (void)(moving=false);
  startx = x;
  starty = y;
  moving = true+button;

  /* récupère les coord. sphérique de la caméra */
  theta_cam_init = cam.theta;
  phi_cam_init   = cam.phi;
  dist_cam_init  = cam.dist;
  /* récupère les coord. sphérique de la lumière */
  theta_spot_init = spot.theta;
  phi_spot_init   = spot.phi;
  dist_spot_init  = spot.dist;
}

/*====================================================================*/
/*=  Gestion des mouvements de la souris  (clic gauche)              =*/
/*====================================================================*/
static __inline__ void g3x_XDialMouseMove(int x, int y)
{ 
  glutSetWindow(xdialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+GLUT_LEFT_BUTTON   :
      if (TheScrollh) 
      {
        TheScrollh->xcurs = CLIP(Xscrollstart,x,Xscrollend);
        TheScrollh->cursor= (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
        if (TheScrollh->dprm)
           *TheScrollh->dprm   = (double)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
        else
        if (TheScrollh->iprm)
           *TheScrollh->iprm   = (   int)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
      }  
      glutPostRedisplay();
      glutPostWindowRedisplay(mainwin);
      return;
  }      
} 

static __inline__ void g3x_YDialMouseMove(int x, int y)
{ 
  glutSetWindow(ydialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+GLUT_LEFT_BUTTON   :
      if (TheScrollv) 
      {
        TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
        TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
        if (TheScrollv->dprm)
           *TheScrollv->dprm   = (double)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
        else
        if (TheScrollv->iprm)
           *TheScrollv->iprm   = (   int)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
      }  
      glutPostRedisplay();
      glutPostWindowRedisplay(mainwin);
      return;
  }
} 

/* pilotes mouvements de la souris : caméra ou lumière */
#define _CLIC_CONTROL_CAM_SPHR_  0
#define _CLIC_CONTROL_SPOT_SPHR_ 1
/* valeur par défaut : caméra */
static int _CLIC_CONTROLLER_ = _CLIC_CONTROL_CAM_SPHR_;

static __inline__ void g3x_DrawMouseMove(int x, int y)
{ 
 glutSetWindow(drawwin);
 switch (_CLIC_CONTROLLER_)
 { /* fonctionnalité mouvements attaché aux mouvements souris */
     case _CLIC_CONTROL_CAM_SPHR_ : /* Contrôle la Caméra    */
       switch (moving)
        {
          case true+GLUT_LEFT_BUTTON   :
           cam.theta = theta_cam_init - 0.005*(x-startx);
           cam.phi   = phi_cam_init   + 0.005*(y-starty);
        break;
        case true+GLUT_MIDDLE_BUTTON  :
           cam.dist = dist_cam_init+0.05*(y-starty);
        break;
      }
      /* mise à jour des coord. cartésiennes de la caméra */
      cam.pos->x = cam.dist*cos(cam.theta)*cos(cam.phi);
      cam.pos->y = cam.dist*sin(cam.theta)*cos(cam.phi);
      cam.pos->z = cam.dist*sin(cam.phi);
      cam.upv .z = cos(cam.phi)>0.?1.0:-1.0;
    break;
     case _CLIC_CONTROL_SPOT_SPHR_ :  /* Contrôle la Lumière */ 
       switch (moving)
        {
          case true+GLUT_LEFT_BUTTON   :
           spot.theta = theta_spot_init + 0.005*(x-startx);
           spot.phi   = phi_spot_init   - 0.005*(y-starty);
           /* mise à jour des coord. cartésiennes de la lumière */
           spot.pos.x = spot.dist*cos(spot.theta)*cos(spot.phi);
           spot.pos.y = spot.dist*sin(spot.theta)*cos(spot.phi);
           spot.pos.z = spot.dist*sin(spot.phi);
        break;
        case true+GLUT_MIDDLE_BUTTON  :
           spot.dist = dist_spot_init+0.05*(y-starty);
           /* mise à jour des coord. cartésiennes de la lumière */
           spot.pos.x = spot.dist*cos(spot.theta)*cos(spot.phi);
           spot.pos.y = spot.dist*sin(spot.theta)*cos(spot.phi);
           spot.pos.z = spot.dist*sin(spot.phi);
        break;
      }
    break;
  }                       
  glutPostRedisplay();
} 

/*= récupère la position de la souris =*/
extern G3Xpoint g3x_GetMousePosition(void)  { return _CLIC_POS_; }

/*=   PASSIVE MOUSE FUNCTIONS -- inutilisés =*/
/*!
static void (*MouseMoveAction)(G3Xpoint) = 0;
extern void g3x_SetMouseMoveAction(void (*action)(G3Xpoint)) { MouseMoveAction = action;}
static __inline__ void g3x_DrawPassiveMouseMove(int x, int y) 
{
  glutSetWindow(drawwin);
  y = curheight - y;
  _CLIC_POS_.x = G3XpixtowinX(x);
  _CLIC_POS_.y = G3XpixtowinY(y);
  if(MouseMoveAction) (*MouseMoveAction)(_CLIC_POS_);
}
!*/

/*====================================================
  fonction associee aux interruptions du pave fleche  
  parametres :                                        
  - c : caractere saisi                               
  - x,y : coordonnee du curseur dans la fenetre       
  ==================================================*/
static __inline__ void g3x_ArrowKeys(int c, int x, int y)
{ 
  if (!prmnum) return;
  glutSetWindow(drawwin);
  /* Ne sert plus ? */
  /*!
  if (ThePrm==_PRM_+prmnum)
  {
      startx = x;
      starty = y;
      switch(c)
      { 
        case GLUT_KEY_UP        : cam.tar->z+=0.01; break;
        case GLUT_KEY_DOWN      : cam.tar->z-=0.01; break;
        case GLUT_KEY_LEFT      : cam.tar->x+=0.01; break;
        case GLUT_KEY_RIGHT     : cam.tar->x-=0.01; break;
        case GLUT_KEY_PAGE_UP   : if (ThePrm!=_PRM_+prmnum) ThePrm++; 
                                  else ThePrm=_PRM_; break;
        case GLUT_KEY_PAGE_DOWN : if (ThePrm!=_PRM_       ) ThePrm--; 
                                  else ThePrm=_PRM_+prmnum; break; 
      }
      g3x_PrintParam(); 
      if (_AUTODISPLAY_) return glutPostWindowRedisplay(drawwin);
  }
  !*/
  if (!ThePrm) return glutPostWindowRedisplay(drawwin);
  
  switch(c)
  { 
    case GLUT_KEY_PAGE_UP   : if (ThePrm!=_PRM_+prmnum) ThePrm++; 
                              else ThePrm=_PRM_;   
                              g3x_PrintParam();  
                              if (_AUTODISPLAY_) return glutPostWindowRedisplay(drawwin);
    case GLUT_KEY_PAGE_DOWN : if (ThePrm!=_PRM_       ) ThePrm--; 
                              else ThePrm=_PRM_+prmnum; 
                              g3x_PrintParam();  
                              if (_AUTODISPLAY_) return glutPostWindowRedisplay(drawwin);
  }

  if (ThePrm->var.r)
  {
    switch (c)
    { 
      case GLUT_KEY_LEFT  : *ThePrm->var.r = ThePrm->min.r; break;
      case GLUT_KEY_RIGHT : *ThePrm->var.r = ThePrm->max.r; break;
      case GLUT_KEY_UP    : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.r += ThePrm->pas.r; break;
          case '*' : *ThePrm->var.r *= ThePrm->pas.r; break;
        } 
       *ThePrm->var.r=MIN(ThePrm->max.r,*ThePrm->var.r); 
        break;  
      case GLUT_KEY_DOWN  : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.r -= ThePrm->pas.r; break;
          case '*' : *ThePrm->var.r /= ThePrm->pas.r; break;
        } 
       *ThePrm->var.r=MAX(ThePrm->min.r,*ThePrm->var.r); 
        break;  
    }
    g3x_PrintParam();
    if (_AUTODISPLAY_) return glutPostWindowRedisplay(drawwin);
  }  
  if (ThePrm->var.i)
  {
    switch (c)
    { 
      case GLUT_KEY_LEFT  : *ThePrm->var.i = ThePrm->min.i; break;
      case GLUT_KEY_RIGHT : *ThePrm->var.i = ThePrm->max.i; break;
      case GLUT_KEY_UP    : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.i += ThePrm->pas.i; break;
          case '*' : *ThePrm->var.i *= ThePrm->pas.i; break;
        } 
       *ThePrm->var.i=MIN(ThePrm->max.i,*ThePrm->var.i);
        break;  
      case GLUT_KEY_DOWN  : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.i -= ThePrm->pas.i; break;
          case '*' : *ThePrm->var.i /= ThePrm->pas.i; break;
        } 
       *ThePrm->var.i=MAX(ThePrm->min.i,*ThePrm->var.i); 
        break;  
    }
    /*! g3x_PrintParam(); !*/
    if (_AUTODISPLAY_) return glutPostWindowRedisplay(drawwin);
  }  
}

/*==============================================================================*/
/*= fonction associee aux interruptions clavier : quelques touches predefinies =*/
/*= parametres :                                                               =*/
/*= - c : caractere saisi                                                      =*/
/*= - x,y : coordonnee du curseur dans la fenetre                              =*/
/*==============================================================================*/
static __inline__ void g3x_Keyboard(uchar c, int x, int y)
{ 
  glutSetWindow(mainwin);
    switch(c)
    { 
      case 6   : _FULLSCREEN_ = !_FULLSCREEN_;
                switch (_FULLSCREEN_) 
                { 
                  case true :
                    fullwidth=curwidth; 
                    fullheight=curheight; 
                    glutFullScreen(); 
                    break;
                  default :glutReshapeWindow(fullwidth,fullheight);
                }  
                /* on reaffiche TOUT */
                return glutPostRedisplay();
      case 17  : case 27 : g3x_Quit();                       /* <Ctrl+'q'> ou <ESC> */  
      /* inverser couleur de fond  : <Ctrl+w> */
      case 23  : G3X_BKG = 1.-G3X_BKG; break;
 
      case 18  : _REPERE_=!_REPERE_; break;                  /* <Ctrl+'r'>          */

      case ' ' : _RUN_=!_RUN_; 
               /* mise a jour de la fonction d'anim */
               glutIdleFunc((_IDLE_&&_RUN_)?(_FAFF_>1?_idle_F_:_idle_0_):NULL); 
               break;

      case '?' : _info_switch_=!_info_switch_; return glutPostRedisplay();

      case '\t': _FILL_=!_FILL_;     break;

      case 'X' : spot.pos.x+=spot.dx; break;
      case 'x' : spot.pos.x-=spot.dx; break;
      case 'Y' : spot.pos.y+=spot.dy; break;
      case 'y' : spot.pos.y-=spot.dy; break;
      case 'Z' : spot.pos.z+=spot.dz; break;
      case 'z' : spot.pos.z-=spot.dz; break;

      case 10  : g3x_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
   /*<Ctrl+j>*/  g3x_DrawDisplay(); 
                 g3x_Snapshot("jpg",_WINDNAME_,curwidth,curheight); break;
      case 16  : g3x_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
   /*<Ctrl+p>*/  g3x_DrawDisplay();
                 g3x_Snapshot("png",_WINDNAME_,curwidth,curheight); break;

      case '6' : 
        cam.theta+=0.01*PI;
        if (cam.theta>2.*PI) cam.theta-=2.*PI;
        cam.pos->x = cam.dist*cos(cam.theta)*cos(cam.phi);
        cam.pos->y = cam.dist*sin(cam.theta)*cos(cam.phi);
        break;
      case '4' : 
        cam.theta-=0.01*PI;   
        if (cam.theta<0.) cam.theta+=2.*PI;
        cam.pos->x = cam.dist*cos(cam.theta)*cos(cam.phi);
        cam.pos->y = cam.dist*sin(cam.theta)*cos(cam.phi);
        break;
      case '8' : 
        cam.phi+=0.01*PI;   
        if (cam.phi>2.*PI) cam.phi-=2*PI;
        cam.pos->x = cam.dist*cos(cam.theta)*cos(cam.phi);
        cam.pos->y = cam.dist*sin(cam.theta)*cos(cam.phi);
        cam.pos->z = cam.dist*sin(cam.phi);
          cam.upv .z = cos(cam.phi)>0.?1.0:-1.0;
        break;
      case '2' : 
        cam.phi-=0.01*PI;   
        if (cam.phi<0.) cam.phi+=2.*PI;
        cam.pos->x = cam.dist*cos(cam.theta)*cos(cam.phi);
        cam.pos->y = cam.dist*sin(cam.theta)*cos(cam.phi);
        cam.pos->z = cam.dist*sin(cam.phi);
          cam.upv .z = cos(cam.phi)>0.?1.0:-1.0;
        break;
      case '3' : 
        cam.dist-=0.1; 
        cam.pos->x = cam.dist*cos(cam.theta)*cos(cam.phi);
        cam.pos->y = cam.dist*sin(cam.theta)*cos(cam.phi);
        cam.pos->z = cam.dist*sin(cam.phi);
          cam.upv .z = cos(cam.phi)>0.?1.0:-1.0;
        break;
      case '9' : 
        cam.dist+=0.1; 
        cam.pos->x = cam.dist*cos(cam.theta)*cos(cam.phi);
        cam.pos->y = cam.dist*sin(cam.theta)*cos(cam.phi);
        cam.pos->z = cam.dist*sin(cam.phi);
          cam.upv .z = cos(cam.phi)>0.?1.0:-1.0;
        break;
  
      /* autres actions liees a une touche */
      default  : if (g3x_ExecuteKeyAction(c)) break;
                 else return (void)fprintf(stderr,"KEY '%c' (%d) : nothing attached\t\r",c,(int)c);
    }
    glutPostWindowRedisplay(drawwin);
    return;
}


/*==============================================================================*/
/*= Fonction d'initialisation de la fenetre et mise en place des parametres Gl =*/
/*= windname : nom de la fenetre                                               =*/
/*= w, h : largeur/hauteur de la denetre (en pixel)                            =*/
/*==============================================================================*/
extern void g3x_InitWindow(char* windname, int w, int h)
{
  /* juste pour que <glutInit> soit content... */
  int   argc=1;
  char* argv[]={""};  

  strncpy(_WINDNAME_,windname,127);
  /* initialisation des parametres gl */
  glutInit(&argc,argv);
  glutInitWindowSize(w,h); 
  glutInitWindowPosition(0,0);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

  if ((mainwin=glutCreateWindow(_WINDNAME_)) == GL_FALSE) 
  { fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  

  glutDisplayFunc(g3x_MainDisplay);
  glutReshapeFunc(g3x_MainReshape);
  pixwidth = curwidth  = w;
  pixheight= curheight = h;

  /* paramètres caméra */
  cam.pos   = &cam_default_pos;
  cam.tar   = &cam_default_tar;
  cam.upv.x= 0.;
  cam.upv.y= 0.;
  cam.upv.z= 1.;
  cam.theta = 0.25*PI;
  cam.phi   = 0.25*PI;
  cam.dist  = 10.;  

  /* paramètres lumière */
  spot.theta= 0.25*PI;
  spot.phi  = 0.25*PI;
  spot.dist = 10.;  
  g3x_SetSpotSteps(0.125,0.125,0.125);  
  g3x_SetLightSpheric(0.25*PI,+0.25*PI,10.);
  g3x_SetLightAmbient (1.,1.,1.);
  g3x_SetLightDiffuse (1.,1.,1.);
  g3x_SetLightSpecular(1.,1.,1.);

}


/*====================================================================*/
/*=  fonction associée aux événements de menu.                       =*/
/*=  - item : code associé au menu sélectionné                       =*/
/*====================================================================*/
#define _MENU_CTRL_ 100
#define _MENU_CLIC_ 200
#define _MENU_FORM_ 300
#define _MENU_MPEG_ 400
#define _MENU_EXIT_ 500

/*====================================================================*/
/*=  Gestion des menus de la souris (clic droit)                     =*/
/*====================================================================*/
static const char* _RIGHT_BUT_[] = {"cam","light",NULL};
static const char* _IMG_CODEC_[] = {"jpg","pnm","png","gif","eps","bmp","tif","ras",NULL};
static const char* _VID_CODEC_[] = {"mp4","flv","x264","mpg2",NULL};

extern G3Xprm *_PRM_,  /* liste des paramètres*/
              *ThePrm; /* paramètre courant   */
extern int     prmnum;

static __inline__ void g3x_MainMenu(int item)
{   
  if (prmnum && item<_MENU_CLIC_) /* les paramètres utilisateur */
  {
    item-=_MENU_CTRL_;
    ThePrm=_PRM_;
     while (item--) ThePrm++;
    return;
  }
  if (item<_MENU_FORM_)            /* souris : caméra ou lumière */
  { 
    const char **f=_RIGHT_BUT_;
    item-=_MENU_CLIC_;
    while (item--) f++;
    _CLIC_CONTROLLER_ = (int)(f-_RIGHT_BUT_);
    return;
  }  
  if (item<_MENU_MPEG_)            /* choix format snapshot */
  { 
    const char **f=_IMG_CODEC_;
    item-=_MENU_FORM_;
    while (item--) f++;
    /*if (!_FULLSCREEN_)*/ g3x_Snapshot(*f,_WINDNAME_,curwidth,curheight);
    return;
  }  
  if (item <_MENU_EXIT_)           /* choix format vidéo    */
  {
    const char **f=_VID_CODEC_;
    item-=_MENU_MPEG_;
    _VIDEO_++;
    while (item--) { f++; _VIDEO_++; }
    g3x_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
    return;
  }
  if (item==_MENU_EXIT_) return g3x_Quit(); /* exit */
}


/*==================================  
  ==================================*/
extern int g3x_MainStart(void)
{ 
  pixwidth = curwidth  = pixwidth -Ydialwidth;
  pixheight= curheight = pixheight-Xdialheight;

  if (g3x_Init) g3x_Init();

  if (_SCROLLH_) 
  { /* bandeau horizontal (si présence scrollbars horiz)            */
    xdialflag=true;
    if ((xdialwin=glutCreateSubWindow(mainwin,0,0,curwidth,Xdialheight)) == GL_FALSE) 
    { fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  
    glutReshapeFunc(g3x_XDialReshape);
    glutDisplayFunc(g3x_XDialDisplay);
    glutMouseFunc(g3x_XDialMouseClic);
    glutMotionFunc(g3x_XDialMouseMove);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  if (_SCROLLV_ || _SWITCH_ || _BUTTON_ || _POPUP_)
  { /* bandeau horizontal (si présence scrollbars vert. ou boutons) */
    ydialflag=true;
    if ((ydialwin=glutCreateSubWindow(mainwin,0,0,Ydialwidth,curheight)) == GL_FALSE) 
    { fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  
    glutReshapeFunc(g3x_YDialReshape);
    glutDisplayFunc(g3x_YDialDisplay);
    glutMouseFunc(g3x_YDialMouseClic);
    glutMotionFunc(g3x_YDialMouseMove);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (_SWITCH_)
    {
      g3x_InitSwitchOn();
      g3x_InitSwitchOff();
    }
    if (_BUTTON_ || _POPUP_)
    {
      g3x_InitPopOn();
      g3x_InitPopOff();
    }
  }
  if (xdialflag && ydialflag)
  { /* coin bas/gauche "?"                                          */
    cornflag=true;
    if ((cornwin=glutCreateSubWindow(mainwin,0,0,Ydialwidth,Xdialheight)) == GL_FALSE) 
    { fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  
    glutReshapeFunc(g3x_CornReshape);
    glutDisplayFunc(g3x_CornDisplay);
    glutMouseFunc(g3x_CornMouseClic);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    g3x_InitCornSwitchOn();
    g3x_InitCornSwitchOff();
  }
  
  if ((drawwin=glutCreateSubWindow(mainwin,0,0,curwidth,curheight)) == GL_FALSE) 
  { fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  

  /* fonctions principales */
  glutDisplayFunc(g3x_DrawDisplay);
  glutReshapeFunc(g3x_DrawReshape);
  glutMouseFunc(g3x_DrawMouseClic);
  glutMotionFunc(g3x_DrawMouseMove);
  glutKeyboardFunc(g3x_Keyboard);
  glutSpecialFunc(g3x_ArrowKeys);  
/*!  
 * glutPassiveMotionFunc(g3x_DrawPassiveMouseMove);
 * !*/
  
  /* initialisation des parametres 3D */
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glClearColor(0.,0.,0.,0.);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);  
  
  g3x_SetLightSpheric(spot.theta, spot.phi, spot.dist);
  GLfloat _POS[4]={spot.pos.x,spot.pos.y,spot.pos.z,1. };
  glLightfv(GL_LIGHT0,GL_POSITION      ,_POS);
  glLightfv(GL_LIGHT0,GL_AMBIENT       ,(GLfloat*)&spot.ambi);
  glLightfv(GL_LIGHT0,GL_DIFFUSE       ,(GLfloat*)&spot.diff);
  glLightfv(GL_LIGHT0,GL_SPECULAR      ,(GLfloat*)&spot.spec);

  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,(GLfloat*)&spot.ambi);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,(GLfloat*)&spot.diff);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,(GLfloat*)&spot.spec);
  glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,0.);


  /* CONSTRUCTION DU MENU SOURIS */
  const char **f;
  int   mp=_MENU_CTRL_ ,submenup,p;
  int   mm=_MENU_CLIC_ ,submenum;
  int   mf=_MENU_FORM_ ,submenuf;
  int   mv=_MENU_MPEG_ ,submenuv;
  submenup=glutCreateMenu(g3x_MainMenu);
  p=0;
  ThePrm=_PRM_;
  while (p<prmnum) glutAddMenuEntry(_PRM_[p++].nom,mp++);
  glutAddMenuEntry("visee",mp);
  /* fonctionalités clic droit (caméra ou lumière) */
  submenum=glutCreateMenu(g3x_MainMenu);
  f=_RIGHT_BUT_;
  while (*f) glutAddMenuEntry(*f++,mm++);
  /* choix format et snapshot        */
  submenuf=glutCreateMenu(g3x_MainMenu);
  f=_IMG_CODEC_;
  while (*f) glutAddMenuEntry(*f++,mf++);
  /* choix format et lancement vidéo */
  if (_IDLE_==true) 
  {
    submenuv=glutCreateMenu(g3x_MainMenu);
    f=_VID_CODEC_;
    while (*f) glutAddMenuEntry(*f++,mv++);
  }

  glutCreateMenu(g3x_MainMenu);
  if (prmnum) glutAddSubMenu("param",submenup);
  glutAddSubMenu("mouse",submenum);
  glutAddSubMenu("photo",submenuf);
  if (g3x_Idle)  glutAddSubMenu("video",submenuv);
  glutAddMenuEntry("exit ",_MENU_EXIT_);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  /*= LANCEMENT BOUCLE PRINCIPALE OpenGl =*/ 
  glutMainLoop();
  return 1;
}


/*==================================  
 OPERATION A REALISER AVANT DE QUITTER
  - fermer la fenetre graphique       
  - liberer les diverses listes       
  - fabrique le MPEG eventuel         
  ==================================*/
extern void g3x_Quit(void)
{
  if (g3x_GarbageCollector) g3x_GarbageCollector();
  g3x_FreeKeyAction();
  g3x_FreeCtrlPrm();
  g3x_FreeButtons();
  g3x_FreePopUps();
  g3x_FreeSwitches();
  g3x_FreeScrolls();
  glutDestroyWindow(mainwin);

  switch (_VIDEO_){ 
    case 0 : break;
    case 1 : g3x_MakeMpeg4(); break;
    case 2 : g3x_MakeFlv();   break;
    case 3 : g3x_MakeAvi();   break;
    case 4 : g3x_MakeMpeg();  break;
    default: break;
  }
  g3x_UnplugCapture();
  exit(0);   
}

#ifdef __cplusplus
  }
#endif

