/*=================================================================*/
/*= E.Incerti - eric.incerti@univ-eiffel.fr                       =*/
/*= Université Gustave Eiffel                                     =*/
/*= Code "squelette" pour prototypage avec libg3x.6c              =*/
/*=================================================================*/

#include <g3x.h>

/* tailles de la fenêtre (en pixel) */
static int WWIDTH=512, WHEIGHT=512;

/* la fonction d'initialisation */
static void init(void)
{  
}

/* la fonction de dessin */
static void draw()
{
}

/* la fonction d'animation (facultatif) */
static void anim()
{
}

/* la fonction de sortie  (facultatif) */
static void quit()
{
}

/***************************************************************************/
/* La fonction principale : NE CHANGE JAMAIS                               */
/***************************************************************************/
int main(int argc, char **argv)
{
	/* creation de la fenetre - titre et tailles (pixels) */
	g3x_InitWindow(*argv,WWIDTH,WHEIGHT);
	g3x_SetInitFunction(init); /* fonction d'initialisation */
	g3x_SetDrawFunction(draw); /* fonction de dessin        */
	g3x_SetAnimFunction(anim); /* fonction d'animation      */
	g3x_SetExitFunction(quit); /* fonction de sortie        */
	/* lancement de la boucle principale */
  return g3x_MainStart();
	/* RIEN APRES CA */
}
