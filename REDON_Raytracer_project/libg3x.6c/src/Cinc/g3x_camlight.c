/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr  
       - Librairie G3X - Fonctions de base d'accès public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */

/*====================================================================*/
/*=                               LA CAMERA                          =*/
/*====================================================================*/
static G3Xpoint  cam_default_pos={5.,5.,5.};
static G3Xpoint  cam_default_tar={0.,0.,0.};
static GLfloat   _near=20.0,
                 _far =100.0,
                 _open=0.1;

static GLdouble  theta_cam_init=+0.25*PI,
                 phi_cam_init  =+0.25*PI,
                 dist_cam_init = 5.;

static G3Xcamera cam;

extern G3Xcamera* g3x_GetCamera(void) { return &cam; }
extern double     g3x_GetCameraDist(void) { return cam.dist; }

extern void g3x_SetCameraTracking(G3Xpoint *position, G3Xpoint *target)
{
  if (position) cam.pos=position;
  if (target  ) cam.tar=target;
}

extern void g3x_SetCameraSpheric(double theta, double phi, double dist, G3Xpoint target)  
{ 
  cam.theta=theta; 
  cam.phi  =phi; 
  cam.dist =dist; 
  cam_default_tar   = target;
  cam_default_pos.x = cam.dist*cos(cam.theta)*cos(cam.phi);
  cam_default_pos.y = cam.dist*sin(cam.theta)*cos(cam.phi);
  cam_default_pos.z = cam.dist*sin(cam.phi);
  cam.upv.z = cos(cam.phi)>0.?1.0:-1.0;
}

extern void g3x_SetCameraCartesian(G3Xpoint position, G3Xpoint target)  
{ 
  cam_default_pos = position;
  cam_default_tar = target;
  cam.dist  = g3x_Norm(position);
  cam.phi   = (G3Xiszero(cam.dist  ) ? 0.     : asin(position.z/cam.dist   ));
  cam.theta = (G3Xiszero(position.y) ? 0.5*PI : atan(position.x/position.y));
  g3x_SetCameraSpheric(cam.theta, cam.phi, cam.dist, target);
}

extern void g3x_SetCameraTheta(double   theta) { cam.theta=theta; }
extern void g3x_SetCameraPhi  (double   phi  ) { cam.phi=phi;}
extern void g3x_SetCameraDist (double   dist ) { cam.dist=dist; }
extern void g3x_SetCameraTar  (G3Xpoint tar  ) { *(cam.tar)=tar; }

/* paramètres d'ouverture de la caméra 3D OpenGl (cf. gluLookAt() */
extern void g3x_SetPerspective(float near, float far, float open) 
{ 
  _near=near; 
  _far=far; 
  _open=open;
}

/*====================================================================*/
/*=                               LA LUMIERE                         =*/
/*====================================================================*/
static G3Xpoint  spot_default_pos={5.,5.,5.};

static GLdouble  theta_spot_init=+0.25*PI,
                 phi_spot_init  =+0.25*PI,
                 dist_spot_init = 5.;

static G3Xlight  spot;

extern G3Xlight* g3x_GetLight(void) { return &spot; }

extern void g3x_SetLightSpheric(double theta, double phi, double dist)  
{ 
  spot.theta=theta; 
  spot.phi  =phi; 
  spot.dist =dist; 
  spot.pos.x = spot.dist*cos(spot.theta)*cos(spot.phi);
  spot.pos.y = spot.dist*sin(spot.theta)*cos(spot.phi);
  spot.pos.z = spot.dist*sin(spot.phi);
}

extern void g3x_SetLightCartesian(G3Xpoint position)
{ 
  spot.pos   = position;
  spot.dist  = g3x_Norm(spot.pos);
  spot.phi   = (G3Xiszero(spot.dist ) ? 0.     : asin(spot.pos.z/spot.dist   ));
  spot.theta = (G3Xiszero(spot.pos.y) ? 0.5*PI : atan(spot.pos.x/spot.pos.y ));
  g3x_SetLightSpheric(spot.theta, spot.phi, spot.dist);
}

extern void g3x_SetSpotSteps(GLfloat dx, GLfloat dy, GLfloat dz)
{
  if (dx!=0.) spot.dx=dx;
  if (dy!=0.) spot.dy=dy;
  if (dz!=0.) spot.dz=dz;
}

extern void g3x_SetLightAmbient  (float r, float g, float b){ spot.ambi=(G3Xcolor){r,g,b,0.}; }
extern void g3x_SetLightDiffuse  (float r, float g, float b){ spot.diff=(G3Xcolor){r,g,b,0.}; }
extern void g3x_SetLightSpecular (float r, float g, float b){ spot.spec=(G3Xcolor){r,g,b,0.}; }
extern void g3x_SetLightPosition (float x, float y, float z){ spot.pos =(G3Xpoint){x,y,z}   ; }
