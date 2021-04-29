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

#ifndef _G3X_TOOLS_H_
  #define _G3X_TOOLS_H_

#include <g3x_types.h>

  char*  g3x_ProcTimer(void);
  char*  g3x_Chrono(void);
  void   g3x_GetTime(G3Xclock* clock);
  
  double g3x_Rand_Percent(double root, double percent);
  double g3x_Rand_Delta(double root, double delta);
  double g3x_Rand_MinMax(double min, double max);    
  
  double g3x_Radcos(double rad);
  double g3x_Radsin(double rad);
  double g3x_Degcos(double deg);
  double g3x_Degsin(double deg);
  
  /*= cartes de couleurs -- ATTENTION:allouent un tableau (float* renvoyé) => à libérer !!!! =*/
  /* Basé sur carte HSV : */
  /*    0°= rouge         */
  /*   60°= jaune         */
  /*  120°= vert          */
  /*  180°= cyan          */
  /*  240°= bleu          */
  /*  300°= magenta       */
  /*  360°= rouge         */
  /* n teintes étalonnées du rouge (0°) au rouge (360°) */
  void   g3x_FillColorMap(G3Xcolor *map, int n);
  void   g3x_FillColorMap_va(G3Xcolor *map, int n, float v, float a);
  float* g3x_GetColorIndex(int idx);
  /* change la couleur n°<idx> de la table <map> */
  void   g3x_SetIndexColor(G3Xcolor* map, int idx, G3Xcolor col);
  
  


 #endif

#ifdef __cplusplus
  }
#endif
/*===============================================================*/
