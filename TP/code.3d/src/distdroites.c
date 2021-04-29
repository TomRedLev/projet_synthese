#include <g3x.h>

/* des couleurs prédéfinies */  
static G3Xcolor rouge  ={1.0,0.0,0.0,1.0};
static G3Xcolor jaune  ={1.0,1.0,0.0,1.0};
static G3Xcolor vert   ={0.0,1.0,0.0,1.0};
static G3Xcolor cyan   ={0.0,1.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0,1.0};


G3Xpoint  A,B,I,K;
G3Xvector u,v;
bool      flag=false;

bool distdroites_2(G3Xpoint A, G3Xvector u, G3Xpoint *I, G3Xpoint B, G3Xvector v, G3Xpoint *K)
{
  double c = g3x_ProdScal(u,v);
  double d = (1.-c*c);
  if (fabs(d)<1.E-8) return false;
  d = 1./d;
  G3Xvector AB=g3x_SetVect(A,B);
  double a =+g3x_ProdScal(AB,u);
  double b =-g3x_ProdScal(AB,v);
  double ta = (a+b*c)*d;
  double tb = (b+a*c)*d;
  I->x = A.x+ta*u.x;
  I->y = A.y+ta*u.y;
  I->z = A.z+ta*u.z;
  K->x = B.x+tb*v.x;
  K->y = B.y+tb*v.y;
  K->z = B.z+tb*v.z;
  return true;
}


/* La fonction de dessin : ne fait aucun calcul, que du dessin */
void draw(void)
{
  glDisable(GL_LIGHTING);    /* <BALISE.GL>  "débranche" la lumière, pour permettre le tracé en mode point/ligne */
    glPointSize(3.0);        /* <VAR.ENV.GL> taille de référence du point                                        */  
    g3x_Color4fv(jaune);     /* fonction g3x, emule la fonction glColor4fv(GLfloat [4])             */
    glBegin(GL_POINTS);      /* <BALISE.GL>  début du bloc de tracé, en mode "point"                             */
      g3x_Vertex3dv(A);      /* fonction g3x, emule la fonction glVertex3dv(GLdouble [3])           */
      g3x_Vertex3dv(B);
    glEnd();                 /* <BALISE.GL>  fin du bloc                                                         */

    g3x_Color4fv(jaune);
    glBegin(GL_LINES);
      glVertex3d(A.x,A.y,A.z);
      glVertex3d(A.x+u.x,A.y+u.y,A.z+u.z);
    glEnd();

    glBegin(GL_LINES);
      glVertex3d(B.x,B.y,B.z);
      glVertex3d(B.x+v.x,B.y+v.y,B.z+v.z);
    glEnd();

    g3x_Color4fv(bleu);       
    glLineWidth(2.);          /* taille de référence du point (en pixel)                             */
    glBegin(GL_LINES);        /* début du bloc de tracé, en mode ligne                               */
      glVertex3d(A.x-10*u.x,A.y-10*u.y,A.z-10*u.z); /* les points, à relier par des lignes */
      glVertex3d(A.x+10*u.x,A.y+10*u.y,A.z+10*u.z);
    glEnd();                  /* fin du bloc de tracé                                                */

    g3x_Color4fv(vert);
    glLineWidth(2.);
    glBegin(GL_LINES);
      glVertex3d(B.x-10*v.x,B.y-10*v.y,B.z-10*v.z);
      glVertex3d(B.x+10*v.x,B.y+10*v.y,B.z+10*v.z);
    glEnd();

  if (!flag) { glDisable(GL_LIGHTING); return; }
  
    g3x_Color4fv(rouge);
    glPointSize(3.0);  
    glBegin(GL_POINTS);
      g3x_Vertex3dv(I);
      g3x_Vertex3dv(K);
    glEnd();
    glLineWidth(2.);
    glBegin(GL_LINES);
      glVertex3d(I.x,I.y,I.z);
      glVertex3d(K.x,K.y,K.z);
    glEnd();

  glEnable(GL_LIGHTING);
}


void init(void)
{
  A=(G3Xpoint ){-1.0,-1.0,-1.0};
  u=(G3Xvector){+0.5,+0.4,+0.2};
  g3x_Normalize(&u);
  B=(G3Xpoint ){+1.0,+1.0,+1.0};
  v=(G3Xvector){-0.2,-0.4,-0.5};
  g3x_Normalize(&v);
  /* l'algo. qui détermine les points I,K */
  flag = distdroites_2(A,u,&I,B,v,&K);
}

int main(int argc, char** argv)
{   
  /* initialisation de la fenêtre graphique et paramétrage Gl */
  g3x_InitWindow(*argv,768,512);

  g3x_SetInitFunction(init);
  g3x_SetDrawFunction(draw);

  return g3x_MainStart();
}
