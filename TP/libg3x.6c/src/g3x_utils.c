/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr  
       - Librairie G3X - Fonctions de base d'accès public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */

#include <math.h>
#include <g3x_types.h>
#include <g3x_utils.h>

/* 02.04.2020
 * FONCTIONS BASIQUES OpenGl : conversion G3Xcoord -> GLdouble[3] et G3Xcolor -> GLfloat[4]
 * */

/* (A,B) -> AB */
extern G3Xvector g3x_SetVect(G3Xpoint A, G3Xpoint B)
{
  return (G3Xvector){(B.x-A.x),(B.y-A.y),(B.z-A.z)};
}

/* (U,V) -> U+V */
extern G3Xvector g3x_AddVect(G3Xvector U, G3Xvector V)
{
  return (G3Xvector){(U.x+V.x),(U.y+V.y),(U.z+V.z)};
}

/* (U,V) -> U-V */
extern G3Xvector g3x_SubVect(G3Xvector U, G3Xvector V)
{
  return (G3Xvector){(U.x-V.x),(U.y-V.y),(U.z-V.z)};
}

/* normalise le vecteur V */
extern void g3x_Normalize(G3Xvector *V)
{
  double norm = SQR(V->x)+SQR(V->y)+SQR(V->z);
  if (norm<1.E-20) norm=1.;
  else norm = 1./sqrt(norm);
  V->x*=norm;
  V->y*=norm;
  V->z*=norm;
}
/* produit scalaire  : crée le scalaire U°V */
extern double g3x_ProdScal(G3Xvector U, G3Xvector V)
{ 
  return (U.x*V.x + U.y*V.y + U.z*V.z);
} 

/*! 04/10/2020 !*/
/* produit scalaire : crée le scalaire AB°U */
extern double g3x_ProdScal2P1V(G3Xpoint A, G3Xpoint B, G3Xvector U)
{ 
  return ((B.x-A.x)*U.x + (B.y-A.y)*U.y + (B.x-A.x)*U.z);
}

/* produit scalaire : crée le scalaire AB°AC */
extern double g3x_ProdScal3Points(G3Xpoint A, G3Xpoint B, G3Xpoint C)
{ 
  return ((B.x-A.x)*(C.x-A.x) + (B.y-A.y)*(C.y-A.y) + (B.x-A.x)*(C.z-A.z));
}



/* produit vectoriel : crée le vecteur U^V */
extern G3Xvector g3x_ProdVect(G3Xvector U, G3Xvector V)
{ 
  return (G3Xvector){ U.y*V.z - U.z*V.y,
                      U.z*V.x - U.x*V.z,
                      U.x*V.y - U.y*V.x };
}

/* produit vectoriel : crée le vecteur AB^AC */
extern G3Xvector g3x_ProdVect3Points(G3Xpoint A, G3Xpoint B, G3Xpoint C)
{ 
  return g3x_ProdVect(g3x_SetVect(A,B),g3x_SetVect(A,C));
}

/* renvoie le milieu du segment (AB) */
extern G3Xpoint g3x_Milieu(G3Xpoint A, G3Xpoint B)
{                     
  return (G3Xpoint){(A.x+B.x)*0.5,
                    (A.y+B.y)*0.5,
                    (A.z+B.z)*0.5};
}

/* renvoie le barycentre du triangle (ABC) */
extern G3Xpoint g3x_Baryc3(G3Xpoint A, G3Xpoint B, G3Xpoint C)
{                     
  return (G3Xpoint){(A.x+B.x+C.x)/3.,
                    (A.y+B.y+C.y)/3.,
                    (A.z+B.z+C.z)/3.};
} 

/* distance euclidienne au carré */
extern double g3x_SqrDist(G3Xpoint A,G3Xpoint B)
{
  return (SQR(B.x-A.x)+SQR(B.y-A.y)+SQR(B.z-A.z));
}
  
/* distance euclidienne */
extern double g3x_Dist(G3Xpoint A,G3Xpoint B)
{
  return sqrt((SQR(B.x-A.x)+SQR(B.y-A.y)+SQR(B.z-A.z)));
}

/* norme du vecteur V au carré */
extern double g3x_SqrNorm(G3Xvector V)
{
  return (SQR(V.x)+SQR(V.y)+SQR(V.z));
}

/* norme du vecteur V  */
extern double g3x_Norm(G3Xvector V)
{
  return sqrt(SQR(V.x)+SQR(V.y)+SQR(V.z));
}

/* norme L2 du vecteur V  */
extern double g3x_NormL2(G3Xvector V)
{
  return MAX3(fabs(V.x),fabs(V.y),fabs(V.z));
}

extern void g3x_Vertex3dv(G3Xpoint p)
{
  glVertex3d(p.x,p.y,p.z);
};

extern void g3x_Normal3dv(G3Xvector v)
{
  glNormal3d(v.x,v.y,v.z);
};

extern void g3x_Color4fv(G3Xcolor c)
{
  glColor4f(c.r,c.g,c.b,c.a);
};

extern void g3x_Materialfv(GLenum face,GLenum pname, G3Xcolor col)
{
  GLfloat GLCOL[4] = {col.r,col.g,col.b,col.a};
  glMaterialfv(face,pname,GLCOL);
}

extern void g3x_Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float alpha)
{
  GLfloat tmp[4];
  tmp[3]=alpha;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  tmp[0]=ambi*col.r;
  tmp[1]=ambi*col.g;
  tmp[2]=ambi*col.b;
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,tmp);
  tmp[0]=diff*col.r;
  tmp[1]=diff*col.g;
  tmp[2]=diff*col.b;
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,tmp);
  tmp[0]=spec*col.r;
  tmp[1]=spec*col.g;
  tmp[2]=spec*col.b;
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,tmp);
  glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,shine*128.);
}


extern G3Xcoord g3x_mapscal3(G3Xcoord C, double map)
{
  return (G3Xcoord){map*C.x,map*C.y,map*C.z};
}

extern G3Xcoord g3x_mapvect3(G3Xcoord C, G3Xcoord map)
{
  return (G3Xcoord){map.x*C.x,map.y*C.y,map.z*C.z};
}

extern G3Xcolor g3x_mapscal4(G3Xcolor C, float map)
{
  return (G3Xcolor){map*C.r,map*C.g,map*C.b,map*C.a};
}

extern G3Xcolor g3x_mapvect4(G3Xcolor C, G3Xcolor map)
{
  return (G3Xcolor){map.r*C.r,map.g*C.g,map.b*C.b,map.a*C.a};
}
