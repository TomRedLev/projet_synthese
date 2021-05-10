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
		G2Xpoint eye;
		G2Xpoint ctr;
		int res;
		G2Xpoint* pix;
		G2Xcolor* col;
		G2Xpoint A, B;
		double len;
	} Cam;

	G2Xvector Reflechi(int orient, G2Xvector u, G2Xvector N);
	G2Xvector Transmis(int orient, G2Xvector u, G2Xvector N, double n);

	Ray cree_ray(G2Xpoint A, G2Xvector u);
	Cam cree_cam(G2Xpoint ctr, G2Xpoint eye, double w, int res);
	void set_up_camera(Cam* camera);
	void draw_camera(Cam* camera);

#endif
