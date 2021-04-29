/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr 
       - Librairie G2X - Fonctions de base d'accès public -           
                    version 6.0 - Mar.2020                                           
  ============================================================= */

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/ 

#define MAXBUTLEN  15

typedef struct
{
  int  num; /* numéro attribué au bouton      */
  int  x,y; /* coordonnées du bouton          */
  int  len;
  char name[MAXBUTLEN+1];
	char info[128];
	void (*action)(void);
	bool on;
} G2Xpopup;

static G2Xpopup *_POPUP_=NULL,*ThePopUp=NULL;
static int       popnum=0;

static int _popup_id_on_=0,_popup_id_off_=0;

/*=  Attribue un numéro et un texte au bouton  =*/
extern bool g2x_CreatePopUp(const char *name, void (*action)(void), const char *info)
{
  G2Xpopup *pop;
  char   *c;

  if (!_POPUP_)
  {
    if (!(_POPUP_=(G2Xpopup*)calloc(BLOCSIZE,sizeof(G2Xpopup)))) return false;
  }

  pop = _POPUP_+popnum;

  strncpy(pop->name,name,MAXBUTLEN);
  if (info) strncpy(pop->info,info,127);
  pop->len = 0;
  c=pop->name;
  while (*c) { pop->len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c); c++; }
  
  Ydialwidth  = MAX(Ydialwidth,pop->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;
  
  pop->num    = popnum;
  pop->action = action;
	pop->on     = false;
	pop->x      = Xbutpos;
  pop->y      = Ybutpos; Ybutpos+=22; 
  popnum++;
  return true;
}

/*=  Renvoie le numéro du bouton sélectionné =*/
extern int g2x_GetPopUp(void) { return ThePopUp?(int)ThePopUp->num:-1; }


/*= Si un bouton est sélectionné, son numero =*/
/*= est mis dans la var. glob. <ThePopUp>   =*/
static __inline__ bool g2x_SelectPopUp(int x, int y)
{
  G2Xpopup *pop=_POPUP_;
  while (pop<_POPUP_+popnum)
  {
    if (abs(pop->x-x)<2*Xbutw && abs(pop->y-y)<10)
    { 
      pushmode=GLUT_DOWN;
      ThePopUp=pop; 
			ThePopUp->on=true;
      return true; 
    }
    pop++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g2x_FreePopUps()
{ 
  if (_POPUP_) 
  { 
    free(_POPUP_); 
    ThePopUp=_POPUP_=NULL; 
  }
}


static __inline__ void g2x_InitPopOn()
{
	_popup_id_on_ = glGenLists(1);
  glNewList(_popup_id_on_, GL_COMPILE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);  
      g2x_Color4fv(G2Xk_a);
      glVertex2i(0,0);
      g2x_Color4fv(G2Xw_a);
      glVertex2i(-Xbutw,-8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(-Xbutw,-8);
    glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEndList();
}

static __inline__ void g2x_InitPopOff()
{
	_popup_id_off_ = glGenLists(1);
  glNewList(_popup_id_off_, GL_COMPILE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);  
      g2x_Color4fv(G2Xw_a);
      glVertex2i(0,0);
      g2x_Color4fv(G2Xk_a);
      glVertex2i(-Xbutw,-8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(-Xbutw,-8);
    glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEndList();
}


/*=  dessinne tous les boutons  =*/
static __inline__ void g2x_DrawPopUps(void)
{
  G2Xpopup  *pop=_POPUP_;
  G2Xcolor   col;
	char *c;
  while (pop<_POPUP_+popnum)
	{
		glPushMatrix();
      glTranslatef(Xbutpos,curheight-pop->y,0.);
			glCallList(pop->on?_popup_id_on_:_popup_id_off_);
      col = G2Xk;
      col.a = 1.-col.a;
      g2x_Color4fv(col);
    	glRasterPos2i(12,-4); 
			
      col = pop->on?G2Xw:G2Xk;
      col.a = 1.-col.a;
      g2x_Color4fv(col);
    	glRasterPos2i(-pop->len/2,-4); 
    	for (c=pop->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    	glRasterPos2i(1-pop->len/2,-4); 
    	for (c=pop->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    glPopMatrix();		
		pop++;
	}
}



