/*=================================================================*/

/*= Tom REDON - M1 INFORMATIQUE                                   =*/
/*= Université Gustave Eiffel - Master Informatique 1             =*/
/*= Synthèse d'Image - Raytracer Project                          =*/
/*= camera.c													  =*/
/*=================================================================*/

#include <g3x.h>

/* tailles de la fenêtre graphique (en pixels)     */
#define WWIDTH  1000
#define WHEIGHT 1000

/* tailles max de l'image (nb de pixels) */
#define NBL 100
#define NBC 100


typedef struct
{
  G3Xhmat   Md;      /* matrice directe (la seule utile)             */
  int       nbc,nbl; /* tailles courantes du pixmap                  */
  G3Xcolor *col;     /* la "carte" des pixels                        */
} Cam;


/* la Caméra globale */
Cam        camera;
/* paramètres réglables de la Caméra : position, orientation, focale, résolution */
double     cam_the=0., cam_phi=PI/4., cam_dis=2., cam_foc=2., cam_res=0.25;
G3Xpoint   cam_tar={1.,1.,-1.};

/*! création de la caméra, allocation des tableau de pixel/couleur  !*/
static Cam Cree_camera_can(void)
{
  Cam camera;
  camera.Md  = g3x_Identity();
  camera.nbc = NBC;
  camera.nbl = NBL;

 /* allocation de l'image aux tailles max */
  if (! (camera.col = (G3Xcolor*)calloc(NBC*NBL,sizeof(G3Xcolor))) ) g3x_Quit();

  return camera;
}

/*! mise à jour de la caméra (Coordonnées sphériques dans le repère de la cible <target>) !*/
static void Camera_setup(Cam* camera, double theta, double phi, double dist, G3Xpoint target, double foc, double resol)
{
  /* la position déduite des coor. sphériques et le vecteur de visée pos->tar */
  G3Xpoint  pos = (G3Xpoint){ target.x + dist*cos(theta)*sin(phi),
                              target.y + dist*sin(theta)*sin(phi),
                              target.z + dist*cos(phi) };
  G3Xvector u   = g3x_SetVect(pos,target);
  g3x_Normalize(&u);
  /*                         |cos(rot_z)*sin(rot_y)   *
   * en coord. sphérique : u |sin(rot_z)*sin(rot_y)   *
   *                         |cos(rot_y)           */
  /* angle principal  (Y) */
  double rot_y  = acos(u.z);
  /* angle secondaire (Z) */
  double rot_z = ( G3Xiszero(rot_y) ? PI : (u.y<0.?-1.:+1.)*acos(u.x/sin(rot_y)) );


  /* (reset) */
  camera->Md = g3x_Identity();
  /* échelle (attention à la symétrie sur l'axe des x) */
  camera->Md = g3x_ProdHMat(g3x_MakeHomothetieXYZ(-camera->nbc/(1.*camera->nbl) , 1., foc),camera->Md);
  /* alignement      */
  camera->Md = g3x_ProdHMat(g3x_MakeRotationZ(PI/2.),camera->Md);

  /* Orientation : le vecteur directeur de la cam. canonique (0.,0.,-1) s'aligne sur U */
  camera->Md = g3x_ProdHMat(g3x_MakeRotationY(+rot_y),camera->Md);
  camera->Md = g3x_ProdHMat(g3x_MakeRotationZ(+rot_z),camera->Md);

  /* translation : positionnement    */
  camera->Md = g3x_ProdHMat(g3x_MakeTranslationV(pos),camera->Md);


  /* remplissage du pixmap, ici avec une bête procédure créant un dégradé */
  camera->nbc = NBC*resol;
  camera->nbl = NBL*resol;
  int x,y;
  double xpas = 2./camera->nbl;
  double ypas = 2./camera->nbc;
  G3Xcolor *col = camera->col;
  for (x=0;x<camera->nbl; x++)
    for (y=0;y<camera->nbc; y++)
    {
      /*(x=0  ,y=0  ) : coin (-1.,-1.) : vert    */
      /*(x=nbl,y=0  ) : coin (+1.,-1.) : bleu    */
      /*(x=0  ,y=nbc) : coin (-1.,+1.) : rouge   */
      /*(x=nbl,y=nbc) : coin (+1.,+1.) : magenta */
     *col = (G3Xcolor){x*xpas, 1.-0.5*(x*xpas+y*ypas), y*ypas,1.};
      col++;
    }
}

/*! Affichage de la caméra !*/
static void Draw_camera(Cam* camera)
{
  /* chargement de la matrice de la caméra dans la pile GL */
  glPushMatrix();
  glMultMatrixd(camera->Md.m);

  /* caméra canonique */

  /*= Chargement Pixmap => Texture Gl =*/
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,camera->nbl,camera->nbc,GL_RGBA,GL_FLOAT,camera->col);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1.,1.,1.);
    glBegin(GL_QUADS);
      glTexCoord2d(0,0); glVertex3d(-1.,-1.,0.);
      glTexCoord2d(1,0); glVertex3d(-1.,+1.,0.);
      glTexCoord2d(1,1); glVertex3d(+1.,+1.,0.);
      glTexCoord2d(0,1); glVertex3d(+1.,-1.,0.);
    glEnd();
  glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

  /* cadre */
  glDisable(GL_LIGHTING);
  glColor3f(1.,1.,1.);
  glBegin(GL_LINE_LOOP);
    glVertex3d(-1.,-1.,0.);
    glVertex3d(-1.,+1.,0.);
    glVertex3d(+1.,+1.,0.);
    glVertex3d(+1.,-1.,0.);
  glEnd();
  glColor3f(.5,.5,.5);
  glBegin(GL_LINES);
    glVertex3d(-1.,-1., 0.);
    glVertex3d( 0., 0.,-1.);
    glVertex3d(-1.,+1., 0.);
    glVertex3d( 0., 0.,-1.);
    glVertex3d(+1.,+1., 0.);
    glVertex3d( 0., 0.,-1.);
    glVertex3d(+1.,-1., 0.);
    glVertex3d( 0., 0.,-1.);
  glEnd();
  glColor3f(1.,0.,0.);
  glBegin(GL_LINES);
    glVertex3d(0., 0.,-1.);
    glVertex3d(0., 0.,0.5*cam_dis);
  glEnd();
	glEnable(GL_LIGHTING);
  /* dépilage de la matrice */
  glPopMatrix();
}


/*===================================================================*/
/*! Le corps du programme                                           !*/
/*===================================================================*/
/*! la fonction d'initialisation !*/
static void init(void)
{
  /* la camera */
  camera = Cree_camera_can();

  g3x_CreateScrollh_d("dF" ,&cam_foc, 0.0, 30., 1.,"distance focale");
  g3x_CreateScrollh_d("Di" ,&cam_dis, 0.0, 30., 1.,"distance de vue");
  g3x_CreateScrollh_d("Ph" ,&cam_phi, -PI, +PI, 1.,"angle de vue (vertical)");
  g3x_CreateScrollh_d("Th" ,&cam_the, -PI, +PI, 1.,"angle de vue (horizontal)");

  g3x_CreateScrollv_d("res",&cam_res, 0.0,1.0,1.,"résolution");
}

/*! la fonction d'initialisation !*/
static void draw()
{
  Camera_setup(&camera,cam_the,cam_phi, cam_dis,cam_tar, cam_foc, cam_res);
  glPushMatrix();
    g3x_Material(G3Xo,0.1,0.5,0.5,0.1,1.);
    glTranslated(cam_tar.x,cam_tar.y,cam_tar.z);
    glutSolidCube(0.25);
  glPopMatrix();
  Draw_camera(&camera);
}

/* fonction de sortie */
static void quit()
{
  free(camera.col); camera.col=NULL;
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/
int main(int argc, char **argv)
{ /* creation de la fenetre - titre et tailles (pixels) */
	g3x_InitWindow(*argv,WWIDTH,WHEIGHT);
  g3x_SetBkgCol(0.3);
  /* param. géométrique de la caméra 3D cf. gluLookAt(...)    */
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la caméra 3D                    */
  g3x_SetCameraSpheric(0.,PI/4.,5.,(G3Xpoint){0.,0.,0.});

	g3x_SetInitFunction(init); /* fonction d'initialisation */
	g3x_SetDrawFunction(draw); /* fonction de dessin        */
	g3x_SetExitFunction(quit); /* fonction de sorite        */

	/* lancement de la boucle principale */
  return g3x_MainStart();
	/* RIEN APRES CA */
}
