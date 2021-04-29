/*=================================================================*/
/*= E.Incerti - eric.incerti@univ-eiffel.fr                       =*/
/*= Universite Gustave Eiffel                                     =*/
/*= Code d'illustration pour prototypage avec libg3x.6c           =*/
/*= cet exemple sert de "manuel" pour la libg3x.                  =*/
/*= il illustre la plupart des fonctionalites disponibles         =*/
/*=================================================================*/

#include <g3x.h>

/* tailles de la fenetre (en pixel) */
static int WWIDTH=768, WHEIGHT=512;

/* BEAUCOUP DE VARIABLES GLOBALES */

/* des couleurs predefinies */
static G3Xcolor rouge  ={1.0,0.0,0.0,1.0};
static G3Xcolor jaune  ={1.0,1.0,0.0,1.0};
static G3Xcolor vert   ={0.0,1.0,0.0,1.0};
static G3Xcolor cyan   ={0.0,1.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0,1.0};
/* parametres geometriques */
static double angle= 0.00;
static double rayon= 0.66;
/* parametres de lumiere/matiere */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;
/* une carte de couleurs */
#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];
/* variables pour l'anim. */
static G3Xvector W={1.,2.,3.};
static double    b=0.1;

/* flag d'affichag/masquage */
static bool FLAG_TEAPOT=true;
static bool FLAG_TORUS =true;
static bool FLAG_CONE  =true;
static bool FLAG_ICOS  =true;

/*= action : variation de couleur =*/
static void action1(void)
{
  rouge.r-=0.01;  rouge.g+=0.01;
  vert.g -=0.01;  vert.b +=0.01;
  bleu.b -=0.01;  bleu.r +=0.01;
}
/*= action : variation de couleur =*/
static void action2(void)
{
  rouge.r+=0.01;  rouge.g-=0.01;
  vert.g +=0.01;  vert.b -=0.01;
  bleu.b +=0.01;  bleu.r -=0.01;
}

/*= action : affiche les parametre camera dans le terminal =*/
static void camera_info(void)
{
	G3Xcamera* cam=g3x_GetCamera();
	fprintf(stderr,"position (x:%lf,y:%lf,z:%lf)\n",cam->pos->x,cam->pos->y,cam->pos->z);
	fprintf(stderr,"cible    (x:%lf,y:%lf,z:%lf)\n",cam->tar->x,cam->tar->y,cam->tar->z);
	fprintf(stderr,"coord. spheriques (d:%lf,theta:%lf,phi:%lf)\n",cam->dist,cam->theta,cam->phi);
}


/* la fonction d'initialisation */
void Init(void)
{
	/* parametres camera */
  /* param. geometrique de la camera. cf. gluLookAt(...) */
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la camera */
  g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});

  /* fixe les param. colorimetriques du spot lumineux */
	/* lumiere blanche (c'est les valeurs par defaut)   */
  g3x_SetLightAmbient (1.,1.,1.);
  g3x_SetLightDiffuse (1.,1.,1.);
  g3x_SetLightSpecular(1.,1.,1.);

  /* fixe la position et la direction du spot lumineux */
	/* (c'est les valeurs par defaut)                    */
  g3x_SetLightPosition (10.,10.,10.);

  /* definition des scrollbars */
  int id;
  /* pour regler la largeur (valeur par defaut 4) */
  g3x_SetScrollWidth(6);
  /* un premier scrollbar */
  id=g3x_CreateScrollv_d("ray",&rayon,0.1,  2.0,1,"rayon sphere     ");
  /* pour changer la "couleur" du scrollbar */
  g3x_SetScrollColor(id,G3Xrb_c);
  id=g3x_CreateScrollv_d("ang",&angle,0.0,360.0,1,"angle rotation   ");
  g3x_SetScrollColor(id,G3Xgb_c);
	id=g3x_CreateScrollv_d("alf",&alpha,0.0,  1.0,1,"transparence cube");
  g3x_SetScrollColor(id,G3Xbb_c);

	g3x_SetScrollWidth(4);
	id=g3x_CreateScrollh_d("shin",&shin,0.0,1.0,1,"intensite lumiere brillance ");
  g3x_SetScrollColor(id,G3Xrc_c);
	id=g3x_CreateScrollh_d("spec",&spec,0.0,1.0,1,"intensite lumiere speculaire");
  g3x_SetScrollColor(id,G3Xgc_c);
	id=g3x_CreateScrollh_d("diff",&diff,0.0,1.0,1,"intensite lumiere diffuse   ");
  g3x_SetScrollColor(id,G3Xbc_c);
	id=g3x_CreateScrollh_d("ambi",&ambi,0.0,1.0,1,"intensite lumiere ambiante  ");

  /* action attachees a des touches */
  g3x_SetKeyAction('g',action1,"variation de couleur");
  g3x_SetKeyAction('G',action2,"variation de couleur");
  g3x_SetKeyAction('c',camera_info,"pos./dir. de la camera => terminal");

	/* initialisation d'une carte de couleurs */
	g3x_FillColorMap(colmap,MAXCOL);
}

/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Dessin(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		/* une sphere */
  	g3x_Material(rouge,ambi,diff,spec,shin,1.);
  	glutSolidSphere(rayon,40,40);
  	/* un cube transparent */
		/*--------------LIMITES DE LA TRANSPARENCE Gl ---------*/
		/* -> TESTER :  DEFINIR LE CUBE TRANSP. AVANT LA SPHERE */
		/* -> TESTER :  RENDRE LA SPHERE TRANSPARENTE           */
  	g3x_Material(vert,ambi,diff,spec,shin,alpha);
  	glutSolidCube(1.);
	glDisable(GL_BLEND);
}

/*= FONCTION D'ANIMATION =*/
void Anim(void)
{
	static double pas=0.1;
	b+=pas;
	W.x+=pas;
	W.y-=pas;
	W.z+=pas;
	if (W.x<-10. || W.x>+10.) pas=-pas;
}

/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= liberation de memoire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void)
{
  /* rien a faire ici puisqu'il n'y a pas d'allocation dynamique */
	fprintf(stdout,"\nbye !\n");
}

int main(int argc, char** argv)
{
  /* initialisation de la fen�tre graphique et parametrage Gl */
  g3x_InitWindow(*argv,WWIDTH,WHEIGHT);


  /* definition des fonctions */
	g3x_SetInitFunction(Init  );  /* fonction d'initialisation */
  g3x_SetDrawFunction(Dessin);  /* la fonction de Dessin     */
	g3x_SetAnimFunction(Anim  );  /* la fonction d'animation   */
  g3x_SetExitFunction(Exit  );  /* la fonction de sortie     */


	/* boucle d'execution principale */
  return g3x_MainStart();
  /* rien apres ca */
}
