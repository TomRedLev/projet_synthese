#ifndef _CAMERA_H_
	#define _CAMERA_H_

	#include <g2x.h>

	typedef struct {
		G2Xpoint org;
		G2Xvector dir;
		struct _obj_* obj;
		double dis;
		G2Xcolor col;
	} Ray;

	typedef struct {
		G2Xhmat Md;
		int nbc;
		G2Xcolor *col;
	} Cam;

	G2Xvector Reflechi(int orient, G2Xvector u, G2Xvector N);
	G2Xvector Transmis(int orient, G2Xvector u, G2Xvector N, double n);

	Ray cree_ray(G2Xpoint A, G2Xvector u);
	Ray draw_ray(Ray* R);
	Cam cree_cam(int nbc);
	void set_up_camera(Cam* camera, G2Xpoint pos, double phi, double foc);
	void draw_camera(Cam* camera);

#endif
