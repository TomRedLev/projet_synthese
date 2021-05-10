#include <g2x.h>
#include "objet.h"
#include "camera.h"

/* tailles de la fenêtre (en pixel) */
static int WWIDTH=512, WHEIGHT=512;
/* limites de la zone reelle associee a la fenetre */
static double   wxmin=-2.,wymin=-2.,wxmax=+2.,wymax=+2.;

static bool SHADOW_FLAG = false;
static bool RAY_FLAG = false;
static double n = 1.;

#define NC 5
Objet CTab[NC], *CList = NULL;

#define NBLIGHT 1
static G2Xpoint SpotLight[NBLIGHT];

Cam cam;

/* des points controlables     */
static G2Xpoint A,B,C,D,E;

/* parametres pour l'animation */
static double   xa1,ya1,xa2,ya2,pasxa1,pasya1,pasxa2,pasya2;

/* des parametres controlables pour les scrollbars */
static double   rA=0.2,rE=0.2,cE=0.5,bkg=1.;
/* une couleur controlable     */
static G2Xcolor col={0.,0.,1.};

/* des flags booleen pour les boutons 'switch' */
static bool _DISKFLAG_=false;
static bool _SQUAREFLAG_=false;

/* un fonction associee a une touche clavier */
static void toggle_square(void) { _SQUAREFLAG_=!_SQUAREFLAG_; }

/* des num. de boutons */
static int  SPOTBUT,TRIGBUT;

/* un fonction associee a un bouton 'popup' : */
/* remise aux positions initiales             */
static void reset(void)
{
	xa1=0.5*wxmin; ya1=0.5*wymin;
	xa2=0.5*wxmax; ya2=0.5*wymax;

	A.x=xa1; A.y=ya1;
	B.x=xa1; B.y=ya2;
	C.x=xa2; C.y=ya1;
	D.x=xa2; D.y=ya2;
	E.x=0.0; E.y=0.0;
}

/* la fonction d'initialisation */
static void init(void)
{
	/* zone graphique reelle associee a la fenetre                 */
  /* si cette fonction n'est pas appelée, les valeurs par défaut */
  /* sont (-1.,-1)->(+1.,+1.)                                    */
	g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

	reset();
	pasxa1=pasxa2=(xa2-xa1)*0.004;
	pasya1=pasya2=(ya2-ya1)*0.004;

	/* les 5 points controlables */
	g2x_SetControlPoint(&A,10);
	g2x_SetControlPoint(&B,10);
	g2x_SetControlPoint(&C,10);
	g2x_SetControlPoint(&D,10);
	g2x_SetControlPoint(&E,10);

  /* les boutons 'switch' */
	g2x_CreateSwitch("D.",&_DISKFLAG_  ,"affiche/masque le disque de centre A");
  g2x_CreateSwitch("S.",&_SQUAREFLAG_,"affiche/masque le 'quad' <ABDC>     ");

	/* les boutons selecteurs exclusifs */
	SPOTBUT=g2x_CreateButton("spot","affiche/masque le 'spot' de centre E      ");
  TRIGBUT=g2x_CreateButton("tri.","affiche/masque le triangle tricolore <BCD>");

  /* une fonction "popup" */
	g2x_CreatePopUp("reset",reset,"reset positions");

	/* les scrollbars : 1 horiz. / 2 vertic. */
	int id=g2x_CreateScrollh_d("rA" ,&rA,0.1,1.0,1.0,"rayon du disque de centre A      ");
  g2x_SetScrollColor(id,G2Xrb_c);
	id=g2x_CreateScrollv_d("rE" ,&rE,0.1,1.0,1.0,"rayon du 'spot' de centre E      ");
  g2x_SetScrollColor(id,G2Xrb_c);
	id=g2x_CreateScrollv_d("col",&cE,0.0,1.0,1.0,"couleur du spot 0.:bleu->1.:rouge");
  g2x_SetScrollColor(id,G2Xgb_c);
	/* un parametre controlable au pave fleche */
	g2x_SetControlParameter_d(&bkg,"bkg",0.01,0.,1.,'+');

	/* une action attachee a une touce clavier */
	g2x_SetKeyAction('s',toggle_square,"affiche/masque le 'quad' <ABDC>");

  /* pour récupérer les bornes de la fenêtre */
  fprintf(stderr,"(%lf,%lf)->(%lf,%lf)\n",g2x_GetXMin(),g2x_GetYMin(),g2x_GetXMax(),g2x_GetYMax());
}

/* La fonction de dessin */
static void draw()
{
  static char msg[32]="";

	/* les boutons selecteurs exclusifs */
	switch (g2x_GetButton())
	{
	  case 0 :  col.r=cE; col.g=1.-cE; g2x_Spot(E.x,E.y,rE,col); break;
	  case 1 :  g2x_FillTriangle_3col(B.x,B.y,G2Xr_b ,C.x,C.y,G2Xg_b ,D.x,D.y,G2Xb_b); break;
	}

  /* les boutons 'switch' */
	if (_SQUAREFLAG_) g2x_FillQuad(A.x,A.y,B.x,B.y,D.x,D.y,C.x,C.y,G2Xya_b);
	if (_DISKFLAG_)   g2x_FillCircle(A.x,A.y,rA,G2Xca_b);


	g2x_Line(A.x,A.y,E.x,E.y,G2Xr,2);

  g2x_Plot(A.x,A.y,G2Xr,4);
  g2x_Print(A.x,A.y,G2Xk,'r',"  A:%+.2lf,%+.2lf",A.x,A.y);

  g2x_Plot(B.x,B.y,G2Xr,4);
  g2x_Print(B.x,B.y,G2Xk,'r',"  B:%+.2lf,%+.2lf",B.x,B.y);

  g2x_Plot(C.x,C.y,G2Xr,4);
  g2x_Print(C.x,C.y,G2Xk,'l',"  C:%+.2lf,%+.2lf",C.x,C.y);

  g2x_Plot(D.x,D.y,G2Xr,4);
  g2x_Print(D.x,D.y,G2Xk,'l',"  D:%+.2lf,%+.2lf",D.x,D.y);

  g2x_Plot(E.x,E.y,G2Xr,4);
  g2x_Print(E.x,E.y,G2Xk,'l',"  E:%+.2lf,%+.2lf",E.x,E.y);

  g2x_Print(0.1,-0.6,G2Xr,'c',"TEXTE EN COORD. FENETRE");
  g2x_StaticPrint(10,10,G2Xr,'l',"TEXTE EN COORD. PIXEL :  NE BOUGE PAS");


  g2x_Axes();
}

/* la fonction d'animation */
static void anim(void)
{
	/* avancement des parametres */
	xa1+=pasxa1; ya1+=pasya1;
	xa2-=pasxa2; ya2-=pasya2;
	/* change de direction sur les bords de la fenetre */
	if (xa1>g2x_GetXMax() || xa1<g2x_GetXMin()) pasxa1=-pasxa1;
	if (xa2>g2x_GetXMax() || xa2<g2x_GetXMin()) pasxa2=-pasxa2;
	if (ya1>g2x_GetYMax() || ya1<g2x_GetYMin()) pasya1=-pasya1;
	if (ya2>g2x_GetYMax() || ya2<g2x_GetYMin()) pasya2=-pasya2;
	/* mise a jour des points */
	A.x=xa1; A.y=ya1;
	B.x=xa1; B.y=ya2;
	C.x=xa2; C.y=ya1;
	D.x=xa2; D.y=ya2;
}

/* la fonction de sortie */
static void quit(void)
{
  fprintf(stderr,"Bye !\n\n");
}

/***************************************************************************/
/* La fonction principale : NE CHANGE JAMAIS                               */
/***************************************************************************/
int main(int argc, char **argv)
{
	/* creation de la fenetre - titre et tailles (pixels) */
	g2x_InitWindow(*argv,WWIDTH,WHEIGHT);
	g2x_SetInitFunction(init); /* fonction d'initialisation */
	g2x_SetDrawFunction(draw); /* fonction de dessin        */
	g2x_SetAnimFunction(anim); /* fonction d'animation      */
	g2x_SetExitFunction(quit); /* fonction de sortie        */
	/* lancement de la boucle principale */
  return g2x_MainStart();
	/* RIEN APRES CA */
}
