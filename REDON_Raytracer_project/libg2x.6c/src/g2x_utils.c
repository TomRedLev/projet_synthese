/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr  
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */

#include <math.h>
#include <g2x_types.h>
#include <g2x_utils.h>

/* 02.04.2020
 * FONCTIONS BASIQUES OpenGl : conversion G2Xcoord -> GLdouble[3] et G2Xcolor -> GLfloat[4]
 * */

/* (A,B) -> AB */
extern G2Xvector g2x_SetVect(G2Xpoint A, G2Xpoint B)
{
  return (G2Xvector){(B.x-A.x),(B.y-A.y)};
}

extern G2Xvector g2x_SetNormalVect(G2Xpoint A, G2Xpoint B)
{
  G2Xvector V=(G2Xvector){(B.x-A.x),(B.y-A.y)};
  g2x_Normalize(&V);
  return V;
}

/* (U,V) -> U+V */
extern G2Xvector g2x_AddVect(G2Xvector U, G2Xvector V)
{
  return (G2Xvector){(U.x+V.x),(U.y+V.y)};
}

/* (U,V) -> U-V */
extern G2Xvector g2x_SubVect(G2Xvector U, G2Xvector V)
{
  return (G2Xvector){(U.x-V.x),(U.y-V.y)};
}

/* normalise le vecteur V */
extern void g2x_Normalize(G2Xvector *V)
{
  double norm = SQR(V->x)+SQR(V->y);
  if (norm<1.E-20) norm=1.;
  else norm = 1./sqrt(norm);
  V->x*=norm;
  V->y*=norm;
}
/* produit scalaire  : crée le scalaire U°V */
extern double g2x_ProdScal(G2Xvector U, G2Xvector V)
{ 
  return (U.x*V.x + U.y*V.y);
} 

/* produit scalaire : crée le scalaire AB°AC */
extern double g2x_ProdScal3Points(G2Xpoint A, G2Xpoint B, G2Xpoint C)
{ 
  return g2x_ProdScal(g2x_SetVect(A,B),g2x_SetVect(A,C));
}

/* produit vectoriel : crée le vecteur U^V */
/* en 2D c'est un scalaire !!!!            */
extern double g2x_ProdVect(G2Xvector U, G2Xvector V)
{ 
  return (U.x*V.y - U.y*V.x);
}

/* produit vectoriel : crée le vecteur AB^AC */
/* en 2D c'est un scalaire !!!!              */
extern double g2x_ProdVect3Points(G2Xpoint A, G2Xpoint B, G2Xpoint C)
{ 
  return g2x_ProdVect(g2x_SetVect(A,B),g2x_SetVect(A,C));
}

/* renvoie le milieu du segment (AB) */
extern G2Xpoint g2x_Milieu(G2Xpoint A, G2Xpoint B)
{                     
  return (G2Xpoint){(A.x+B.x)*0.5,
                    (A.y+B.y)*0.5 };
}

/* renvoie le barycentre du triangle (ABC) */
extern G2Xpoint g2x_Baryc3(G2Xpoint A, G2Xpoint B, G2Xpoint C)
{                     
  return (G2Xpoint){(A.x+B.x+C.x)/3.,
                    (A.y+B.y+C.y)/3. };
} 

/* distance euclidienne au carré */
extern double g2x_SqrDist(G2Xpoint A,G2Xpoint B)
{
  return (SQR(B.x-A.x)+SQR(B.y-A.y));
}
  
/* distance euclidienne */
extern double g2x_Dist(G2Xpoint A,G2Xpoint B)
{
  return sqrt((SQR(B.x-A.x)+SQR(B.y-A.y)));
}

/* norme du vecteur V au carré */
extern double g2x_SqrNorm(G2Xvector V)
{
  return (SQR(V.x)+SQR(V.y));
}

/* norme du vecteur V  */
extern double g2x_Norm(G2Xvector V)
{
  return sqrt(SQR(V.x)+SQR(V.y));
}

/* norme L2 du vecteur V  */
extern double g2x_NormL2(G2Xvector V)
{
  return MAX(fabs(V.x),fabs(V.y));
}

extern void g2x_Color4fv(G2Xcolor c)
{
  glColor4f(c.r,c.g,c.b,c.a);
}

extern G2Xcoord g2x_mapscal2(G2Xcoord C, double map)
{
  return (G2Xcoord){map*C.x,map*C.y};
}

extern G2Xcoord g2x_mapvect2(G2Xcoord C, G2Xcoord map)
{
  return (G2Xcoord){map.x*C.x,map.y*C.y};
}

extern G2Xcolor g2x_mapscal4(G2Xcolor C, float map)
{
  return (G2Xcolor){map*C.r,map*C.g,map*C.b,map*C.a};
}

extern G2Xcolor g2x_mapvect4(G2Xcolor C, G2Xcolor map)
{
  return (G2Xcolor){map.r*C.r,map.g*C.g,map.b*C.b,map.a*C.a};
}
