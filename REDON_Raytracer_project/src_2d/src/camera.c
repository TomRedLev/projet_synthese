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
	g2x_Line(R->org.x, R->org.y, (R->org.x) + (R->dis) * (vec.x), (R->org.y) + (R->dis) * (vec.y), R->col, 5);
	printf("%f %f\n", (R->org.x) + (R->dis) * (vec.x), (R->org.y) + (R->dis) * (vec.y));
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
	G2Xpoint camera = g2x_Mat_x_Point(cam->Md, (G2Xpoint){0., 0.});
	G2Xpoint foc = g2x_Mat_x_Point(cam->Md, (G2Xpoint){0., .4});
	g2x_Triangle(camera.x, camera.y + 0.2, camera.x, camera.y - 0.2, foc.x, foc.y, *cam->col, 5);
	g2x_Line(camera.x, camera.y, foc.x, foc.y, *cam->col, 5);
	g2x_Plot(camera.x, camera.y, *cam->col, 5);
	g2x_Plot(foc.x, foc.y, *cam->col, 5);

}
