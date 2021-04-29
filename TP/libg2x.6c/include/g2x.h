/* ===============================================================
  E.Incerti - Universit� Gustave Eiffel - eric.incerti@univ-eiffel.fr 
       - Librairie G2X - Fonctions de base d'acc�s public -           
                    version 6.0 - Mar.2020                                           
  ============================================================= */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

  #ifndef _G2X_H_
    #define _G2X_H_
  
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <math.h>
  #include <unistd.h>
  
  #include <GL/gl.h>
  #include <GL/glut.h>

  #include <g2x_types.h>      /* types primaires et macros diverses        */
  #include <g2x_utils.h>      /* macros et fonctions basiques Pts, Vect    */
  #include <g2x_tools.h>      /* utilitaires divers                        */
  #include <g2x_colors.h>     /* couleurs pr�d�finies                      */
  #include <g2x_capture.h>    /* routines de capture d'�cran image/vid�o   */
  #include <g2x_draw.h>       /* trac� en mode r�el                      */
  #include <g2x_geoalgo.h>    /* algo. g�om�triques en mode r�el         */
  #include <g2xi_geoalgo.h>   /* algo. g�om�triques                      */
  #include <g2x_polygon.h>    /* manipulation de polyg�nes (listes)      */
  #include <g2x_transfo.h>    /* transfo. en coordonn�es homog�nes 4x4     */
  #include <g2x_quaternions.h>
  #include <g2x_pnm.h>
  #include <g2x_window.h>     /* routines et fonctions de base            */
  
  #endif  

#ifdef __cplusplus
  }
#endif
/*===============================================================*/
