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

#ifndef _G2X_TYPES_H
  #define _G2X_TYPES_H
  
  /*!           DEFINITION DE QUELQUES TYPES           !*/
  /* definition et macros pour le type <unsigned char>  */ 
  #ifndef _UCHAR_
    #define _UCHAR_
    #include <limits.h>
    typedef unsigned char uchar;
    #define UCHARSIZE CHAR_BIT*sizeof(uchar)
  #endif
  /* definition et macros pour le type <unsigned short> */ 
  #ifndef _USHORT_
    #define _USHORT_
    #include <limits.h>
    typedef unsigned short ushort;
    #define USHORTSIZE CHAR_BIT*sizeof(ushort)
  #endif
  /* definition et macros pour le type <unsigned int> */  
  #ifndef _UINT_
    #define _UINT_
    #include <limits.h>
    typedef unsigned int uint;
    #define UINTSIZE CHAR_BIT*sizeof(uint)
  #endif
  /* definition et macros pour le type <bool> */  
  #ifndef _BOOL_
    #define _BOOL_
    #ifndef __cplusplus
      typedef enum {false=0x0,true=0x1} bool;
    #endif 
  #endif

  /*==================================================================*/
  /* les types primaires :                                            */
  /* Couleur, Point, Vecteur, Matrice et Quaternions                  */
  /*==================================================================*/
  /* Couleur RGBA    - Attention OpenGl gère ça en <float rgba[4]>    */
  typedef struct { float  r,g,b,a; } G2Xcolor;
  /* Coordonnées XYZ - Attention OpenGl gère ça en <double xyz[3]>    */
  typedef struct { double x,y; }   G2Xcoord, G2Xpoint, G2Xvector;
  /*= MATRICE EN COORDONNEES HOMOGENE     =*/
  /* type matrice en coordonnées homogènes */
  /* ATTENTION  : OpenGl gère ça directement en <double mat[16]       */
  /*              et traite ça par colonne plutôt que par ligne       */   
  typedef struct { double m[9]; } G2Xhmat;
  /* | 00 03 | 06 | */
  /* | 01 04 | 07 | */
  /* | 02 05 | 08 | */

   /* 1° colonne */
  #define c00  0
  #define c10  1
  #define c20  2
  /* 2° colonne */
  #define c01  3
  #define c11  4
  #define c21  5  
  /* 3° colonne */
  #define c02  6
  #define c12  7
  #define c22  8

  /* type quaternion (reel/vecteur) */   
  typedef struct
  {
    double    r;
    G2Xvector v;
  } G2Xquat;  


  /*==================================================================*/
  /* les types plus évolués Image, Caméra, Lumière, Horloge           */
  /*==================================================================*/
  /* Le type "image" basé sur le format PNM */
  typedef struct
  {
    char    mode;         /* P[1-6]                    */
    int     width,height; /* largeur, hauteur          */
    int     layer,depth;  /* nbre de plans, profondeur */
    uchar  *map,*end;     /* début et fin du pixmap    */
  } G2Xpnm;  
  
  /* horloge */  
  typedef struct
  {
    uint hh; /* heure [0,23]     */
    uint mm; /* minutes [0,59]   */
    uint ss; /* secondes [0,59]  */
    uint ds; /* dizieme  de sec. */
    uint cs; /* centieme de sec. */
    uint ms; /* millieme de sec. */
  } G2Xclock;

#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
