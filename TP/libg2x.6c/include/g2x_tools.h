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

#ifndef _G2X_TOOLS_H_
  #define _G2X_TOOLS_H_

#include <g2x_types.h>

  char*  g2x_ProcTimer(void);
  char*  g2x_Chrono(void);
  void   g2x_GetTime(G2Xclock* clock);
  
  double g2x_Rand_Percent(double root, double percent);
  double g2x_Rand_Delta(double root, double delta);
  double g2x_Rand_MinMax(double min, double max);    
  
  double g2x_Radcos(double rad);
  double g2x_Radsin(double rad);
  double g2x_Degcos(double deg);
  double g2x_Degsin(double deg);
  
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
  void   g2x_FillColorMap(G2Xcolor *map, int n);
  void   g2x_FillColorMap_va(G2Xcolor *map, int n, float v, float a);
  float* g2x_GetColorIndex(int idx);
  /* change la couleur n°<idx> de la table <map> */
  void   g2x_SetIndexColor(G2Xcolor* map, int idx, G2Xcolor col);
  
  


 #endif

#ifdef __cplusplus
  }
#endif
/*===============================================================*/
