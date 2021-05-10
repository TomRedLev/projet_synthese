#include "camera.h"

extern Ray cree_ray(G2Xpoint A, G2Xvector u) {
	Ray R;
	R.org = A;
	R.dir = u;
	g2x_Normalize(&R.dir);
	R.col = G2Xk;
	R.obj = NULL;
	R.dis = 10000.;
	return R;
}

extern Cam cree_cam(int nbc) {
	Cam camera;
	camera.Md = g2x_Identity();
	camera.nbc = nbc;

	if (! (camera.col = (G2Xcolor*)calloc(nbc, sizeof(G2Xcolor)))) {
		g2x_Quit();
	}
	return camera;
}

extern void set_up_camera(Cam *camera, G2Xpoint pos, double phi, double foc) {
	camera->Md = g2x_Identity();
	camera->Md = g2x_Mat_x_Mat(g2x_Homothetie2d(-1., foc), camera->Md);
	camera->Md = g2x_Mat_x_Mat(g2x_Rotation(phi - PI/2), camera->Md);
	camera->Md = g2x_Mat_x_Mat(g2x_Translation2d(pos.x, pos.y), camera->Md);
}

extern void draw_camera(Cam* cam) {
	G2Xpoint E = g2x_Mat_x_Point(cam->Md, (G2Xpoint){0., -1.});
	g2x_Plot(E.x, E.y, *cam->col, 25);
}
