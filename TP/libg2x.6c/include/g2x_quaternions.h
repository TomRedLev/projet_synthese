/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */


#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G2X_QUATERNION_H
  #define _G2X_QUATERNION_H

  #include <g2x_types.h>

  void    g2x_QuatIdentity(G2Xquat A);
  G2Xquat g2x_QuatSet(double r, G2Xvector v); 
  G2Xquat g2x_QuatSet4(double r, double x, double y); 
  G2Xquat g2x_QuatAdd(G2Xquat A, G2Xquat B); 
  G2Xquat g2x_QuatProd(G2Xquat A, G2Xquat B); 
  G2Xquat g2x_QuatConj(G2Xquat A); 
  double  g2x_QuatSqrNorm(G2Xquat A); 
  double  g2x_QuatNorm(G2Xquat A); 
  G2Xquat g2x_QuatNormalize(G2Xquat A); 
  G2Xquat g2x_QuatScalMap(G2Xquat A, double a); 
  G2Xquat g2x_QuatInv(G2Xquat A); 
  void    g2x_QuatToHmat(G2Xquat A, G2Xhmat M);
  void    g2x_QuatPrint(G2Xquat A);
  void    g2x_QuatRot(G2Xquat Qrot, G2Xcoord src, G2Xcoord dest);
  void    g2x_AxeRadRot(G2Xvector v, double rad, G2Xcoord src, G2Xcoord dest);
  G2Xquat g2x_QuatAlign(G2Xvector v, G2Xvector const cible);
  
#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
