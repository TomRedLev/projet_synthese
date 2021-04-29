/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr 
       - Librairie G3X - Fonctions de base d'accès public -           
                    version 6.0 - Mar.2020                                           
  ============================================================= */

#ifdef __cplusplus
   extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G3X_TRANSFO_H
  #define _G3X_TRANSFO_H

  #include <g3x_types.h>

  /*=****************************************=*/
  /*=   Identité                             =*/
  /*=****************************************=*/
  G3Xhmat   g3x_Identity(void);
  /*=****************************************=*/
  /*=   MATRICES DE TRANSFORMATION STANDARD  =*/
  /*=****************************************=*/
  G3Xhmat   g3x_MakeTranslationV(G3Xvector t);
  G3Xhmat   g3x_MakeTranslationXYZ(double tx, double ty, double tz);
  G3Xhmat   g3x_MakeHomothetieV(G3Xvector h);
  G3Xhmat   g3x_MakeHomothetieXYZ(double hx, double hy, double hz);
  G3Xhmat   g3x_MakeRotationX(double a_radians);
  G3Xhmat   g3x_MakeRotationY(double a_radians);
  G3Xhmat   g3x_MakeRotationZ(double a_radians);
  /*=****************************************=*/
  /*=  Fonctions utiltaires sur les matrices =*/
  /*=****************************************=*/
  /* Produit Matrice(A)*Vecteur(V) -> Vecteur */
  G3Xvector g3x_ProdHMatVector(G3Xhmat A, G3Xvector V);
  /* Produit Matrice(A)*Point(P) -> Point     */
  G3Xpoint  g3x_ProdHMatPoint(G3Xhmat A, G3Xpoint P);
  /* version générique des 2 précédentes      */
  G3Xcoord  g3x_ProdHMatXYZd(G3Xhmat A, double x, double y, double z, double delta);
  /* Produit Matrice(A)*Matrice(B)->Matrice   */
  G3Xhmat   g3x_ProdHMat(G3Xhmat A,G3Xhmat B);

  /* Affichage de la matrice */
  void g3x_PrintHMat(G3Xhmat A);

  /* conversion (transposition) au format de amatrice OpenGl */
  G3Xhmat g3x_FormatGLmat(G3Xhmat src);  

#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
