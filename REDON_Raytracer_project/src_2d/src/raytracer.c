#include <g2x.h>
#include "objet.h"
#include "camera.h"

/* tailles de la fenêtre (en pixel) */
static int WWIDTH=264, WHEIGHT=264;
/* limites de la zone reelle associee a la fenetre */
static double   wxmin=-2.,wymin=-2.,wxmax=+2.,wymax=+2.;

static bool SHADOW_FLAG = false;
static bool RAY_FLAG = true;
static double n = 1.;

#define NC 5
Objet CTab[NC], *CList = NULL;

#define NBLIGHT 1
static G2Xpoint SpotLight[NBLIGHT];

Cam cam;

static void ray_tracer(Ray *Ri, Objet *CList, G2Xpoint *L, int nbl, int rec) {
	if (rec == 0) {
		return ;
	}
	rec = rec - 1;

	G2Xpoint I;
	G2Xvector Ni;

	Objet* C = CList;

	do {
		G2Xpoint A = g2x_Mat_x_Point(C->Mi, Ri->org);
		G2Xvector u = g2x_Mat_x_Vector(C->Mi, Ri->dir);
		g2x_Normalize(&u);
		G2Xpoint Ic;
		G2Xvector Nc;
		if (C->inter(&Ic, &Nc, A, u)) {
			double d = g2x_SqrDist(g2x_Mat_x_Point(C->Md, Ic), Ri->org);
			if (d < Ri->dis) {
				Ri->obj = C;
				Ri->dis = d;
				Ri->col = C->col;
				I = g2x_Mat_x_Point(C->Md, Ic);
				Ni = g2x_Mat_x_Vector(C->Mn, Nc);
				g2x_Normalize(&Ni);
			}
		}
	} while ((C = C->next) != NULL);

	if (Ri->obj == NULL) {
		return ;
	}

	Objet* Ci = Ri->obj;

	int i;
	for (i = 0; i < nbl; i++) {
		G2Xvector IL = g2x_SetNormalVect(I, L[i]);
		double x = Ci->mat.diff * g2x_ProdScal(Ni, IL);
		if (x > 0. && Ci->mat.shin > 0.) {
			x = pow(x, 1./Ci->mat.shin) * Ci->mat.diff * (1.-Ci->mat.shin);
			Ri->col.r += x*1.;
			Ri->col.g += x*1.;
			Ri->col.b += x*1.;
		}
	}

	if (!RAY_FLAG) {
		return ;
	}
	/* Ri->col.a = 0.66; */
	g2x_Line(I.x, I.y, Ri->org.x, Ri->org.y, G2Xk, 1);
	Ri->col.a = 0.;
}

static void cam_tracer(Cam* camera, Objet* CList, G2Xpoint* L, int nbl, int rec) {
	G2Xpoint E = g2x_Mat_x_Point(camera->Md, (G2Xpoint){0., 1.});
	double xpas = 2./camera->nbc;
	int x;
	G2Xcolor *rgb = camera->col;
	for (x = 0; x < camera->nbc; x++) {
		G2Xpoint pix = (G2Xpoint) {(-1. + x * xpas), 0.};
		G2Xpoint A = g2x_Mat_x_Point(camera->Md, pix);
		G2Xvector u = g2x_SetVect(E, A);
		/* g2x_Normalize(&u); */
		Ray R = cree_ray(A, u);
		ray_tracer(&R, CList, L, nbl, rec);
		*rgb = R.col;
		rgb++;
	}
}


void display_list(Cam* camera) {
	G2Xcolor* rgb = camera->col;
	int x;
	double step = 4./camera->nbc;
	for (x = 0; x < camera->nbc; x++) {
		g2x_SetColor(*rgb);
		g2x_Line(-2 + (x * step), -1.5, -2 + (x * step), -2., *rgb, step);
		rgb++;
	}
}






/* la fonction d'initialisation */
static void init(void)
{
	cam = cree_cam(1000);
	G2Xpoint pos_cam = g2x_Point(1., 0.);
	set_up_camera(&cam, pos_cam, 0., .4);

	SpotLight[0] = (G2Xpoint){2, 2};

	Matiere mat = (Matiere){1., 1., 1., 1., 1., 1.};
	CTab[0] = cree_cercle_can(G2Xr, mat);
	rescale_objet(&CTab[0], 0.2, 0.2);
	translate_objet(&CTab[0], -1, -1);
	CList = &CTab[0];
	Objet *elem = CList;

	CTab[1] = cree_carre_can(G2Xg, mat);
	rescale_objet(&CTab[1], 0.2, 0.2);
	translate_objet(&CTab[1], 0., 1.);
	elem->next = &CTab[1];
	elem = elem->next;

	CTab[2] = cree_cercle_can(G2Xb, mat);
	rescale_objet(&CTab[2], 0.2, 0.2);
	translate_objet(&CTab[2], -0.25, 0.25);
	elem->next = &CTab[2];
	elem = elem->next;

	CTab[3] = cree_carre_can(G2Xb, mat);
	rescale_objet(&CTab[3], 0.2, 0.2);
	translate_objet(&CTab[3], -0.25, -0.25);
	elem->next = &CTab[3];
}

/* La fonction de dessin */
static void draw()
{
	draw_camera(&cam);
	draw_cercle(&CTab[0]);
	draw_carre(&CTab[1]);
	draw_cercle(&CTab[2]);
	draw_carre(&CTab[3]);
	g2x_Plot(SpotLight[0].x, SpotLight[1].y, G2Xy, 15);
	cam_tracer(&cam, CList, SpotLight, 1, 1);
	display_list(&cam);
}

/* la fonction d'animation */
static void anim(void)
{

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
