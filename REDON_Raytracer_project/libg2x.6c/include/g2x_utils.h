/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr  
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */

/* 02.04.2020
 * FONCTIONS BASIQUES OpenGl : conversion G2Xcoord -> GLdouble[3] et G2Xcolor -> GLfloat[4]
 * */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G2X_UTILS_H
  #define _G2X_UTILS_H
  
  #include <g2x_types.h>
  #include <GL/gl.h>

  #define SQR(a) ((a)*(a))           
  
  #define MIN(a,b)        (((a)<(b))?(a):(b)) 
  #define MAX(a,b)        (((a)<(b))?(b):(a)) 
  #define MIN3(a,b,c)     (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c))) 
  #define MAX3(a,b,c)     (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c))) 
  #define CLIP(min,a,max) (((a)<(min)?(min):((a)>(max)?(max):(a))))

  /* pour remplacer les tests d'égalité sur les réels */
  /* 06/04/2020 : baissé de 1.e-8 à 1.e-20            */
  #define ZERO 1.e-20
  #define G2Xiszero(x) (((x)<ZERO && (x)>-ZERO)?true:false)

  /* Quelques constantes numériques toujours utiles.. */    
  #define PI        3.1415926535897932384626433832795
  #define DegToRad  0.0174532925199433
  #define RadToDeg 57.29577951308232
  #define Racin2    1.414213562373095

  /* pour compatibilité OpenGl : glColor4fv(GLfloat *c)                         */
  void      g2x_Color4fv(G2Xcolor c);
  /* Opérations basiques sur les objets G2Xpoint et G2Xvector                   */
  G2Xvector g2x_SetVect(G2Xpoint  A, G2Xpoint  B); /* return AB                 */
  G2Xvector g2x_SetNormalVect(G2Xpoint  A, G2Xpoint  B); /* return AB, NORME 1  */
  G2Xvector g2x_AddVect(G2Xvector U, G2Xvector V); /* return U+V                */ 
  G2Xvector g2x_SubVect(G2Xvector U, G2Xvector V); /* return U-V                */
  /* produits scalaire et vectoriel */
  double    g2x_ProdScal(G2Xvector U, G2Xvector V); /*            return U°V    */
  double    g2x_ProdScal3Points(G2Xpoint A, G2Xpoint B, G2Xpoint C); /*  AB°AC  */
  double    g2x_ProdVect(G2Xvector U, G2Xvector V); /*            return U^V    */
  double    g2x_ProdVect3Points(G2Xpoint A, G2Xpoint B, G2Xpoint C); /*  AB^AC  */
  /* milieu, barycentre */
  G2Xpoint  g2x_Milieu(G2Xpoint A, G2Xpoint B);            /* return (A+B)/2    */
  G2Xpoint  g2x_Baryc3(G2Xpoint A, G2Xpoint B, G2Xpoint C);/* return (A+B+C)/3  */
  /* norme et distance */
  double    g2x_SqrDist(G2Xpoint A,G2Xpoint B); /* distance au carré ||AB||²    */
  double    g2x_Dist(G2Xpoint A,G2Xpoint B);
  double    g2x_SqrNorm(G2Xvector V);           /* norme au carré ||V||²        */
  double    g2x_Norm(G2Xvector V);
  double    g2x_NormL2(G2Xvector V);            /* norme L2 : max(|x|,|y|,|z|)  */
  void      g2x_Normalize(G2Xvector *V);
  /* mapping coordonnées, couleurs */
  G2Xcoord  g2x_mapscal2(G2Xcoord C, double   map); /* return (map*C)            */
  G2Xcoord  g2x_mapvect3(G2Xcoord C, G2Xcoord map);
  G2Xcolor  g2x_mapscal4(G2Xcolor C, float    map);
  G2Xcolor  g2x_mapvect4(G2Xcolor C, G2Xcolor map);

#endif

#ifdef __cplusplus
  }
#endif
