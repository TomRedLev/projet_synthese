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

#ifndef _G2X_BASIX_H_
  #define _G2X_BASIX_H_

  #include <g2x_types.h>
  #include <g2x_colors.h>

  /********************************************************************************/
   /* variables d'environnement                                                    */
  double g2x_GetXMin(void);      /* coordonnee courante en X du coin inf. gauche  */
  double g2x_GetYMin(void);      /* coordonnee courante en Y du coin inf. gauche  */
  double g2x_GetXMax(void);      /* coordonnee courante en X du coin sup. droite  */
  double g2x_GetYMax(void);      /* coordonnee courante en Y du coin sup. droite  */
  double g2x_GetXScale(void);    /* echelle pixel/reel en X                       */
  double g2x_GetYScale(void);    /* echelle pixel/reel en Y                       */
  double g2x_GetXPixSize(void);  /* taille reelle du pixel en X                   */
  double g2x_GetYPixSize(void);  /* taille reelle du pixel en Y                   */
  int    g2x_GetPixWidth(void);  /* largeur courante (en pixels)                  */
  int    g2x_GetPixHeight(void); /* hauteur courante (en pixels)                  */
  void   g2x_SelectDrawWin(void);/* inactive dans cette version                   */ 

  /*!-----------------------------------------------!*/
  /*!           LES FONCTIONALITES DE BASE          !*/
  /*!-----------------------------------------------!*/
  void g2x_AutoSnap_jpg(void);
  void g2x_AutoSnap_png(void);
  
  /*= fixe / récupère la couleur de fond de la fenêtre               =*/
  void   g2x_SetBkgCol(double bkg);
  double g2x_GetBkgCol(void      );

  /* trace les axes (0,x,y) */
  void g2x_Axes(void);
  /* trace une grilles des coordonnées */
  void g2x_Grid(G2Xcolor col1, G2Xcolor col2);

  /*= Fonction de gestion des événements =*/
  bool g2x_Running(void);   /* renvoie true si la boucle d'anim. est active            */
  void g2x_Show(void);      /* force l'affichage dans la fonction de dessin (DrawFunc) */ 
  void g2x_Stop(void);      /* arrête l'animation (arrêt sur image)                    */
  void g2x_Continue(void);  /* relance l'animation                                     */

  /*= initialisation de la fenêtre et mise en place des paramètres Gl =*/
  void g2x_InitWindow(char* windname, int w, int h);
  /*= définit la zone graphique réelle associée à la fenêtre          =*/
  bool g2x_SetWindowCoord(double xmin, double ymin, double xmax, double ymax);
  /*!-----------------------------------------------!*/
  /*!             dessin et calculs                 !*/
  /*!-----------------------------------------------!*/
  /* initialisation des donnees                                                   */
  void  g2x_SetInitFunction(void (*f)(void));
  /* sélectionne la fonction associée aux actions de dessin                       */
  void g2x_SetDrawFunction(void (*f)(void));
  /* sélectionne la fonction associée aux actions d'animation / calculs           */
  void  g2x_SetAnimFunction(void (*f)(void));
  /* affichage/anim : 1 affichage pour <freq> passes de calcul                    */
  void  g2x_SetRefreshFreq(int freq);
    
  /*= lance la boucle principale (glutMainLoop) =*/
  int  g2x_MainStart(void);
  /*= - ExitFunc   : procédure de libération de mémoire, appelée à la sortie     =*/
  void g2x_SetExitFunction(void (*ExitFunc)(void));
  /*= pour sortir =*/
  void g2x_Quit(void);

  /* démarre automatiquement l'enregistrement vidéo */
  void g2x_AutoStartVideo(void);
  
  /********************************************************************************/
  /* TEXTE                                                                        */
  /* selectionne la taille de la fonte : seules valeurs acceptees 10, 12, 18      */
  /* taille par defaut 10                                                         */
  void g2x_SelectFont(int size);

  /* écrit le message <msg> en coordonnees reelles (x,y) avec la couleur <col>    */
  /* <pos> : 'l','r' ou 'c' positionne le texte a gauche/droite/centre de (x,y)   */
  void g2x_Print(double x, double y, G2Xcolor col, char pos, const char *msg, ...);

  /* meme chose en coordonnees "pixel" -- entiers --(x,y)                         */
  /* garde une position fixe, independante des zoom et redimensionnement          */
  void g2x_StaticPrint(int x, int y, G2Xcolor col, char pos, const char *msg, ...);
  /*!-----------------------------------------------!*/
  /*!             FONCTIONS DE CONTROLE             !*/
  /*!-----------------------------------------------!*/
  /* **************************************************************************** */
  /* Controle de parametre de type <int> et <double> avec le pave fleche          */
  /* param : adresse du parametre (doit etre de type <int> ou <double>            */
  /* nom   : identification du parametre qui apparait dans le menu deroulant      */
  /* min,max : borne inf. et sup. de la zone de variation du parametre            */
  /* mode : '+' pour mode arithmetique (*param += pas)                            */
  /*        '*' pour mode geometrique  (*param *= pas)                            */
  /* pas  : pas d'avancement (exp. 0.01 en mode '+' , 1.01 en mode '*'            */
  /* retour : <false> en cas d'echec d'enregistrement                             */
  bool g2x_SetControlParameter_i(int    *param, char* name, int    pas, int    min,    int max, char mode);
  bool g2x_SetControlParameter_d(double *param, char* name, double pas, double min, double max, char mode);
  /* teste si le parametre controlable <param> a ete modifie                       */
  bool g2x_ChangeControlParameter(void *param);  

  /* Point de contrôle à la souris*/
  bool g2x_SetControlPoint(G2Xpoint* P, int  ray);
  
  /* **************************************************************************** */
  /* gestion des mouvements de la souris (sans click)                             */
  /* execute l'action en fonction de la position de la souris                     */
  void g2x_SetMouseMoveAction(void (*action)(G2Xpoint _CLIC_POSITION_));
  /* Retourne les coord. de la souris.                                            */
  G2Xpoint g2x_GetMousePosition(void);
  /* Retourne les coord. du clic souris. Retourne NULL si aucun clic détecté.     */
  G2Xpoint* g2x_GetClic(void);


  /* **************************************************************************** */
  /* gestion des boutons poussoirs                                                */
  /*                                                                              */
  /* bouton renvoyant un simple numéro                                            */
  /* à utiliser avec switch (g2x_GetButton()) { case 1 : ..... break; ... }       */ 
  int g2x_CreateButton(const char *text, const char *info);
  /* retourne le num. du boutton courant                                          */
  int g2x_GetButton(void);

  /* bouttons 'ON/OFF' : bascule la variable flag false<->true                    */
  bool g2x_CreateSwitch(const char *txt, bool *flag, const char *info);
  /* retourne l'id du popup courant                                               */
  int  g2x_GetSwitch(void);

  /* lance l'action associée, une seule fois (pop-up)                             */
  /*équivalent à l'attachement de l'action à une touche clavier (cf. en dessous)  */
  bool g2x_CreatePopUp(const char *name, void (*action)(void), const char *info);
  int  g2x_GetPopUp(void);

  /* **************************************************************************** */
  /* gestion des actions associées à une touche clavier                           */
  /* ATTENTION : les touches predefinies ne sont pas reattribuables               */
  /* predefinies : <ESP><ESC><q><Q><Ctrl+F><+><-><=><?><fleches>                  */
  /* **************************************************************************** */
  bool g2x_SetKeyAction(char key, void (*action)(void), const char *info);

  /* **************************************************************************** */
  /* gestion des scrollbars                                                       */  
  /* crée un Scroll Vertical directement attache au parametre reel <param>          
     nom :  nom du scroll affiché sur la fenêtre                                    
     prm :  pointeur sur la variable (TYPE DOUBLE) attachee au scroll               
     min,max : bornes inférieurs et supérieure de parcours du scroll                
     info :  facultatif (0/NULL/"") texte apparaissant dans la fenetre d'aide       
     retour  : id (numéro) du scroll, -1 en cas d'échec                           */
  /* **************************************************************************** */
  int g2x_CreateScrollh_d(const char *nom, double* prm, double min, double max, double mapfactor, const char *info);
  /* meme chose pour un parametre entier (TYPE INT)                               */
  int g2x_CreateScrollh_i(const char *nom, int*    prm, int    min, int    max, double mapfactor, const char *info);
  /* crée un Scroll anonyme [0.,1.]  - initilise en position <init>               */
  int g2x_CreateAnonymousScrollv(const char *name, double init, const char *info);
  
  /* memes choses pour les scroll verticaux                                       */
  int g2x_CreateScrollv_d(const char *nom, double* param, double min, double max, double mapfactor, const char *info);
  int g2x_CreateScrollv_i(const char *nom, int*    prm, int      min, int    max, double mapfactor, const char *info);
  int g2x_CreateAnonymousScrollv(const char *name, double init, const char *info);

  /* position courante du curseur dans l'intevalle [min,max] du scroll  n° <id>   */
  /* utile surtout avec les scroll anonymes                                       */
  double g2x_GetScrollCursor(int id);

  /* pour changer la couleur du scroll                                            */
  void   g2x_SetScrollColor(int id, G2Xcolor col);
  /* ajuste la largeur des scrolls (4<=w<=16) defaut:8                            */
  void   g2x_SetScrollWidth(int width) ;

  /* temporisation, en seconde                                                    */
  int    g2x_tempo(double tempo);

 #endif

#ifdef __cplusplus
  }
#endif
/*===============================================================*/
