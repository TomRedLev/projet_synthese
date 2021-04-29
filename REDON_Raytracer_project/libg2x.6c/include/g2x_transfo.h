/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr 
       - Librairie G2X - Fonctions de base d'accès public -           
                    version 6.0 - Mar.2020                                           
  ============================================================= */

#ifdef __cplusplus
   extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G2X_TRANSFO_H
  #define _G2X_TRANSFO_H

  #include <g2x_types.h>

  /*=****************************************=*/
  /*=   Identité                             =*/
  /*=****************************************=*/
  G2Xhmat   g2x_Identity(void);
  /*=****************************************=*/
  /*=   MATRICES DE TRANSFORMATION STANDARD  =*/
  /*=****************************************=*/
  G2Xhmat   g2x_Translation1v(G2Xvector t);
  G2Xhmat   g2x_Translation2d(double tx, double ty);
  G2Xhmat   g2x_Homothetie1v(G2Xvector h);
  G2Xhmat   g2x_Homothetie2d(double hx, double hy);
  G2Xhmat   g2x_Rotation(double a_radians);
  /*=****************************************=*/
  /*=  Fonctions utiltaires sur les matrices =*/
  /*=****************************************=*/
  /* Produit Matrice(A)*Vecteur(V) -> Vecteur */
  G2Xvector g2x_Mat_x_Vector(G2Xhmat A, G2Xvector V);
  /* Produit Matrice(A)*Point(P) -> Point     */
  G2Xpoint  g2x_Mat_x_Point(G2Xhmat A, G2Xpoint P);
  /* version générique des 2 précédentes      */
  G2Xcoord  g2x_Mat_x_Coord2d(G2Xhmat A, double x, double y, double delta);
  /* Produit Matrice(A)*Matrice(B)->Matrice   */
  G2Xhmat   g2x_Mat_x_Mat(G2Xhmat A,G2Xhmat B);
  /* Affichage de la matrice */
  void g2x_Mat_print(G2Xhmat A);
  /* Creation de la matrice 3x3 de rotation autour du G2Xpoint C, d'angle alpha */
  G2Xhmat g2x_RotationC(G2Xpoint C, float alpha);
  G2Xhmat g2x_RotationCXY(double Cx, double Cy, float alpha) ;
   
#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
