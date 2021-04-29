/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr  
       - Librairie G3X - Fonctions de base d'accès public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */

/* 02.04.2020
 * FONCTIONS BASIQUES OpenGl : conversion G3Xcoord -> GLdouble[3] et G3Xcolor -> GLfloat[4]
 * */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G3X_UTILS_H
  #define _G3X_UTILS_H
  
  #include <g3x_types.h>
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
  #define G3Xiszero(x) (((x)<ZERO && (x)>-ZERO)?true:false)

  /* Quelques constantes numériques toujours utiles.. */    
  #define PI        3.1415926535897932384626433832795
  #define DegToRad  0.0174532925199433
  #define RadToDeg 57.29577951308232
  #define Racin2    1.414213562373095

  /* FONCTIONS BASIQUES OpenGl : conversion G3Xcoord -> GLdouble[3] et G3Xcolor -> GLfloat[4] */
  void g3x_Vertex3dv(G3Xpoint p);  /* équiv. glVertex3dv */
  void g3x_Normal3dv(G3Xvector p); /* équiv. glNormal3dv */
  void g3x_Color4fv(G3Xcolor c);   /* équiv. glColor4fv  */
  void g3x_Materialfv(GLenum face,GLenum pname, G3Xcolor col); /* équiv. glMaterialfv */

  /*= ALTERNATIVE à la gestion de couleurs OpenGl                              =*/ 
  /*= mise en forme des paramètre de couleur/matière des objets                =*/
  /* tous les paramètres dans [0,1]                                             */
  /* col    : teinte RGB                                                        */
  /* ambi   : réaction à la lumière ambiante (blanche par défaut)               */
  /* diff   : réaction à la lumière diffuse (blanche par défaut)                */
  /* spec   : réaction à la lumière de spéculosité (blanche par défaut)         */
  /* shine  : coeff. de spéculosité (max par défaut -- 256.    )                */
  /* transp : valeur de transparence (0.: objet totalement transp. / 1.:opaque) */
  void g3x_Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float alpha);

  /* Opérations basiques sur les objets G3Xpoint et G3Xvector                   */
  G3Xvector g3x_SetVect(G3Xpoint  A, G3Xpoint  B); /* return AB                 */
  G3Xvector g3x_AddVect(G3Xvector U, G3Xvector V); /* return U+V                */ 
  G3Xvector g3x_SubVect(G3Xvector U, G3Xvector V); /* return U-V                */
  /* produits scalaire et vectoriel */
  double    g3x_ProdScal(G3Xvector U, G3Xvector V); /*            return U°V    */
  double    g3x_ProdScal2P1V(G3Xpoint A, G3Xpoint B, G3Xvector U);/*     AB°U   */ /*! 04/10/2020 !*/     
  double    g3x_ProdScal3Points(G3Xpoint A, G3Xpoint B, G3Xpoint C); /*  AB°AC  */
  G3Xvector g3x_ProdVect(G3Xvector U, G3Xvector V); /*            return U^V    */
  G3Xvector g3x_ProdVect3Points(G3Xpoint A, G3Xpoint B, G3Xpoint C); /*  AB^AC  */
  /* milieu, barycentre */
  G3Xpoint  g3x_Milieu(G3Xpoint A, G3Xpoint B);            /* return (A+B)/2    */
  G3Xpoint  g3x_Baryc3(G3Xpoint A, G3Xpoint B, G3Xpoint C);/* return (A+B+C)/3  */
  /* norme et distance */
  double    g3x_SqrDist(G3Xpoint A,G3Xpoint B); /* distance au carré ||AB||²    */
  double    g3x_Dist(G3Xpoint A,G3Xpoint B);
  double    g3x_SqrNorm(G3Xvector V);           /* norme au carré ||V||²        */
  double    g3x_Norm(G3Xvector V);
  double    g3x_NormL2(G3Xvector V);            /* norme L2 : max(|x|,|y|,|z|)  */
  void      g3x_Normalize(G3Xvector *V);
  /* mapping coordonnées, couleurs */
  G3Xcoord  g3x_mapscal3(G3Xcoord C, double   map); /* return (map*C)            */
  G3Xcoord  g3x_mapvect3(G3Xcoord C, G3Xcoord map);
  G3Xcolor  g3x_mapscal4(G3Xcolor C, float    map);
  G3Xcolor  g3x_mapvect4(G3Xcolor C, G3Xcolor map);

#endif

#ifdef __cplusplus
  }
#endif
