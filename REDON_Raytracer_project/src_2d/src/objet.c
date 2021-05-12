#include "objet.h"
#include <g2x.h>

extern void draw_cercle(Objet *c) {
	static G2Xpoint P[721];
	static bool flag = false;

	if (!flag) {
		double _angle = PI/360.;
		int i;
		for (i = 0; i < 720; i++) {
			P[i].x = cos(i*_angle);
			P[i].y = sin(i*_angle);
		}
		P[i].x = 1;
		P[i].y = 0;
		flag = true;
	}
	double x, y;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	g2x_Color4fv(c->col);
	glBegin(GL_POLYGON);
	int i;
	G2Xpoint Q;
	for (i = 0; i < 720; i++) {
		Q = g2x_Mat_x_Point(c->Md, P[i]);
		glVertex2d(Q.x, Q.y);
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static bool rayon_inter_cercle_canonique(G2Xpoint* J, G2Xvector* N, G2Xpoint A, G2Xvector u) {
	double t = -g2x_ProdScal(u, (G2Xvector) A);
	if (t < 0.) {
		return false;
	}
	double d = -g2x_ProdVect(u, (G2Xvector) A);
	d = d*d;
	if (d > 1.) {
		return false;
	}
	t = t - sqrt(1. - d);
	J->x = A.x + t * u.x;
	J->y = A.y + t * u.y;
	*N = (G2Xvector) (*J);

	return true;
}

extern Objet cree_cercle_can(G2Xcolor col, Matiere mat) {
	Objet c;
	c.id = _CRC_;
	reset_objet(&c);
	c.col = col;
	c.mat = mat;
	c.inter = rayon_inter_cercle_canonique;
	c.draw = draw_cercle;
	c.next = NULL;
	return c;
}

extern void draw_carre(Objet *c) {
	G2Xpoint P;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	g2x_Color4fv(c->col);
	glBegin(GL_POLYGON);
	P = g2x_Mat_x_Point(c->Md, (G2Xpoint) {-1., -1.});
	glVertex2d(P.x, P.y);
	P = g2x_Mat_x_Point(c->Md, (G2Xpoint) {+1., -1.});
	glVertex2d(P.x, P.y);
	P = g2x_Mat_x_Point(c->Md, (G2Xpoint) {+1., +1.});
	glVertex2d(P.x, P.y);
	P = g2x_Mat_x_Point(c->Md, (G2Xpoint) {-1., +1.});
	glVertex2d(P.x, P.y);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static double dist_L2(G2Xpoint A, G2Xpoint B) {
	double x = fabs(A.x - B.x);
	double y = fabs(A.y - B.y);
	return MAX(x, y);
}

static bool rayon_inter_carre_canonique(G2Xpoint* J, G2Xvector* N, G2Xpoint A, G2Xvector u) {
	bool flag[4] = {false, false, false, false};
	static G2Xcoord CN[4] = {{+1., 0.}, {0., +1.}, {-1., 0.}, {0., -1.}};
	flag[0] = (bool)(A.x > +1.);
	flag[1] = (bool)(A.y > +1.);
	flag[2] = (bool)(A.x < -1.);
	flag[3] = (bool)(A.y < -1.);
	int i;
	for (i = 0; i < 4; i++) {
		if (flag[i] == false) {
			continue;
		}
		double ps1 = g2x_ProdScal(g2x_SetVect(A, CN[i]), CN[i]);
		if (ps1 >= 0.) {
			continue;
		}
		double ps2 = g2x_ProdScal(u, CN[i]);
		if (ps2 >= 0.) {
			continue;
		}
		double t = ps1/ps2;
		J->x = A.x + t * u.x;
		J->y = A.y + t * u.y;
		if (dist_L2(CN[i], *J) > 1.) {
			continue;
		}
		*N = CN[i];
		return true;
	}
	return false;
}

extern Objet cree_carre_can(G2Xcolor col, Matiere mat) {
	Objet c;
	c.id = _SQR_;
	reset_objet(&c);
	c.col = col;
	c.mat = mat;
	c.inter = rayon_inter_carre_canonique;
	c.draw = draw_carre;
	c.next = NULL;
	return c;
}



extern void reset_objet(Objet *c) {
	c->Md = g2x_Identity();
	c->Mn = g2x_Identity();
	c->Mi = g2x_Identity();
}

extern void rescale_objet(Objet* obj, double a, double b) {
	obj->Md = g2x_Mat_x_Mat(g2x_Homothetie2d(a, b), obj->Md);
	obj->Mn = g2x_Mat_x_Mat(g2x_Homothetie2d(b, a), obj->Mn);
	obj->Mi = g2x_Mat_x_Mat(obj->Mi, g2x_Homothetie2d(1./a, 1./b));
}

extern void zrotate_objet(Objet* obj, double arad) {
	obj->Md = g2x_Mat_x_Mat(g2x_Rotation(arad), obj->Md);
	obj->Mn = g2x_Mat_x_Mat(g2x_Rotation(arad), obj->Mn);
	obj->Mi = g2x_Mat_x_Mat(obj->Mi, g2x_Rotation(-arad));
}

extern void translate_objet(Objet* obj, double x, double y) {
	obj->Md = g2x_Mat_x_Mat(g2x_Translation2d(x, y), obj->Md);
	obj->Mi = g2x_Mat_x_Mat(obj->Mi, g2x_Translation2d(-x, -y));
}
