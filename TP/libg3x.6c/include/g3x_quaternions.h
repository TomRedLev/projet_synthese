/* ===============================================================
  E.Incerti - Universit� Gustave Eiffel - eric.incerti@univ-eiffel.fr 
       - Librairie G3X - Fonctions de base d'acc�s public -       
                    version 6.0 - Mar.2020                        
  ============================================================= */


#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G3X_QUATERNION_H
  #define _G3X_QUATERNION_H

  #include <g3x_types.h>

  void    g3x_QuatIdentity(G3Xquat A);
  G3Xquat g3x_QuatSet(double r, G3Xvector v); 
  G3Xquat g3x_QuatSet4(double r, double x, double y, double z); 
  G3Xquat g3x_QuatAdd(G3Xquat A, G3Xquat B); 
  G3Xquat g3x_QuatProd(G3Xquat A, G3Xquat B); 
  G3Xquat g3x_QuatConj(G3Xquat A); 
  double  g3x_QuatSqrNorm(G3Xquat A); 
  double  g3x_QuatNorm(G3Xquat A); 
  G3Xquat g3x_QuatNormalize(G3Xquat A); 
  G3Xquat g3x_QuatScalMap(G3Xquat A, double a); 
  G3Xquat g3x_QuatInv(G3Xquat A); 
  void    g3x_QuatToHmat(G3Xquat A, G3Xhmat M);
  void    g3x_QuatPrint(G3Xquat A);
  void    g3x_QuatRot(G3Xquat Qrot, G3Xcoord src, G3Xcoord dest);
  void    g3x_AxeRadRot(G3Xvector v, double rad, G3Xcoord src, G3Xcoord dest);
  G3Xquat g3x_QuatAlign(G3Xvector v, G3Xvector const cible);
  
#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
