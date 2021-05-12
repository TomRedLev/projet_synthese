#ifndef _OBJET_H_
	#define _OBJET_H_

	#include <g2x.h>
	#include "camera.h"

	#define _CRC_ 0
	#define _SQR_ 1

	typedef struct {
		double ambi, diff, spec, shin, alfa, indo;
	} Matiere;

	typedef struct _obj_ {
		int id;
		G2Xhmat Md, Mn, Mi;
		G2Xcolor col;
		Matiere mat;
		bool (* inter)(G2Xpoint*, G2Xvector*, G2Xpoint, G2Xvector);
		void (* draw)(struct _obj_ *);
		struct _obj_ *next;
	} Objet;

	Objet cree_cercle_can(G2Xcolor col, Matiere mat);
	void draw_cercle(Objet *c);
	Objet cree_carre_can(G2Xcolor col, Matiere mat);
	void draw_carre(Objet *c);

	void reset_objet(Objet *obj);
	void rescale_objet(Objet *obj, double a, double b);
	void rotate_objet(Objet *obj, double arad);
	void translate_objet(Objet *obj, double x, double y);

#endif
