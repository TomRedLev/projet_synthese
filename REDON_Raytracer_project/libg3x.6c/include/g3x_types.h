/* ===============================================================
  E.Incerti - Universit� Gustave Eiffel - eric.incerti@univ-eiffel.fr 
       - Librairie G3X - Fonctions de base d'acc�s public -           
                    version 6.0 - Mar.2020                                           
  ============================================================= */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G3X_TYPES_H
  #define _G3X_TYPES_H
  
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
  /* Couleur RGBA    - Attention OpenGl g�re �a en <float rgba[4]>    */
  typedef struct { float  r,g,b,a; } G3Xcolor;
  /* Coordonn�es XYZ - Attention OpenGl g�re �a en <double xyz[3]>    */
  typedef struct { double x,y,z; }   G3Xcoord, G3Xpoint, G3Xvector;
  /*= MATRICE EN COORDONNEES HOMOGENE     =*/
  /* type matrice en coordonn�es homog�nes */
  /* ATTENTION  : OpenGl g�re �a directement en <double mat[16]       */
  /*              et traite �a par colonne plut�t que par ligne       */   
  typedef struct { double m[16]; } G3Xhmat;
  /* | 00 04 08 | 12 | */
  /* | 01 05 09 | 13 | */
  /* | 02 06 10 | 14 | */
  /* | 03 07 11 | 15 | */

   /* 1� colonne */
  #define a00  0
  #define a10  1
  #define a20  2
  #define a30  3
  /* 2� colonne */
  #define a01  4
  #define a11  5
  #define a21  6  
  #define a31  7
  /* 3� colonne */
  #define a02  8
  #define a12  9
  #define a22 10
  #define a32 11
  /* 4� colonne */
  #define a03 12
  #define a13 13
  #define a23 14
  #define a33 15

  /* type quaternion (reel/vecteur) */   
  typedef struct
  {
    double    r;
    G3Xvector v;
  } G3Xquat;  


  /*==================================================================*/
  /* les types plus �volu�s Image, Cam�ra, Lumi�re, Horloge           */
  /*==================================================================*/
  /* Le type "image" bas� sur le format PNM */
  typedef struct
  {
    char    mode;         /* P[1-6]                    */
    int     width,height; /* largeur, hauteur          */
    int     layer,depth;  /* nbre de plans, profondeur */
    uchar  *map,*end;     /* d�but et fin du pixmap    */
  } G3Xpnm;  
  
  /* Le type Cam�ra  */
  typedef struct
  {
    G3Xpoint  *pos; /* point d'attache (point de la sc�ne)    */
    G3Xpoint  *tar; /* point de vis�e  (point de la sc�ne)    */
    G3Xvector  upv; /* verticale                              */
    /* coord. sph�riques dans le rep�re local                */
    double    theta,phi,dist;
    /* parametres d'ouverture (-> man gluLookAt)             */
    double    near,far,open;
  } G3Xcamera;

  /* Le type Lumi�re */
  typedef struct
  {
    G3Xpoint   pos;            /* position  cartesienne        */
    double    theta,phi,dist; /* coordonn�es sph�riques       */
    /* couleurs de la lumiere      */    
    G3Xcolor   ambi;  /* ambiante   */
    G3Xcolor   diff;  /* diffuse    */
    G3Xcolor   spec;  /* sp�culaire */
    double    dx,dy,dz; /* pas de d�placement clavier (x/X,y/Y,z/Z)*/
  } G3Xlight;

  /* horloge */  
  typedef struct
  {
    uint hh; /* heure [0,23]     */
    uint mm; /* minutes [0,59]   */
    uint ss; /* secondes [0,59]  */
    uint ds; /* dizieme  de sec. */
    uint cs; /* centieme de sec. */
    uint ms; /* millieme de sec. */
  } G3Xclock;

#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
