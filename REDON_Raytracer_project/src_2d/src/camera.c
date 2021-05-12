#include "camera.h"

extern Ray cree_ray(G2Xpoint A, G2Xvector u) {
	Ray R;
	R.org = A;
	R.dir = u;
	g2x_Normalize(&R.dir);
	R.col = G2Xk;
	R.obj = NULL;
	R.dis = 10.;
	return R;
}

extern Ray draw_ray(Ray* R) {
	G2Xpoint vec = (G2Xpoint) {(R->org.x) + (R->dir.x), (R->org.y) + (R->dir.y)};
	printf("%f %f\n", vec.x, vec.y);
	g2x_Line(R->org.x, R->org.y, (R->org.x) - (vec.x) * (R->dis), (R->org.y) - (vec.y) * (R->dis), R->col, 5);
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
	G2Xpoint camera = g2x_Mat_x_Point(cam->Md, (G2Xpoint){1., 0.});
	G2Xpoint camera2 = g2x_Mat_x_Point(cam->Md, (G2Xpoint){-1., 0.});
	G2Xpoint middle = g2x_Mat_x_Point(cam->Md, (G2Xpoint){0., 0.});
	G2Xpoint foc = g2x_Mat_x_Point(cam->Md, (G2Xpoint){0., 1.});
	g2x_Triangle(camera.x, camera.y, camera2.x, camera2.y, foc.x, foc.y, *cam->col, 5);
	g2x_Line(middle.x, middle.y, foc.x, foc.y, *cam->col, 5);
	g2x_Plot(middle.x, middle.y, *cam->col, 5);
	g2x_Plot(foc.x, foc.y, *cam->col, 5);
}
