/*=================================================================*/
/*= E.Incerti - eric.incerti@univ-eiffel.fr                       =*/
/*= Université Gustave Eiffel                                     =*/
/*= Code "squelette" pour prototypage avec libg2x.6c              =*/
/*=================================================================*/

#include <g2x.h>

/* tailles de la fenêtre (en pixel) */
static int WWIDTH=512, WHEIGHT=512;
/* limites de la zone reelle associee a la fenetre */
static double wxmin=-10.,wymin=-10.,wxmax=+10.,wymax=+10.;

/* la fonction d'initialisation */
static void init(void)
{  
	/* zone graphique reelle associee a la fenetre                 */
  /* si cette fonction n'est pas appelée, les valeurs par défaut */
  /* sont (-1.,-1)->(+1.,+1.)                                    */
	g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);
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
	g2x_InitWindow(*argv,WWIDTH,WHEIGHT);
	g2x_SetInitFunction(init); /* fonction d'initialisation */
	g2x_SetDrawFunction(draw); /* fonction de dessin        */
	g2x_SetAnimFunction(anim); /* fonction d'animation      */
	g2x_SetExitFunction(quit); /* fonction de sortie        */
	/* lancement de la boucle principale */
  return g2x_MainStart();
	/* RIEN APRES CA */
}
