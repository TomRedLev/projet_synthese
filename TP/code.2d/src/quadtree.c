/*==================================================================
 *  E.Incerti - eric.incerti@univ-eiffel.fr
 *  2020.04.14  - QuadTree sur nuage de points
 *  des points sont distribués dans le 
 *  carré unité (bas du Quadtree) et on subdivise jusqu'à ce qu'il
 *  n'y ait plus qu'un seul point -- ou zero -- dans chaque noeud
 *==================================================================*/
#include <g2x.h>

#define DIM 4

/* le type d'objet à stocker : ici un point et sa vitesse */
typedef struct _objlist_
{
  G2Xpoint  pos;
  G2Xvector vit;
  struct _objlist_ *prev,*next;
} ObjList;

/* un noeud de l'Octree */
typedef struct _4node_
{
  int       lev;            /* niveau de subdivision      */
  G2Xpoint  DL,UR,C;        /* 2 coins opposés et centre  */
  G2Xvector dim;            /* les 2 dimensions du "cube" */
  struct _4node_* son[DIM]; /* les 4 fils            */
  struct _4node_* dad;      /* le noeud père         */
  ObjList*  ptl;
  int       nbp;
} QuadNode, *QuadTree;

/* Variables globales */
QuadNode QT;
#define N 600
ObjList  PTL[N];
G2Xcolor PCOL[N];
G2Xcolor QCOL[N];

static void change_node_list(ObjList *p, QuadNode *dad, QuadNode *son)
{ /* passe le point <p> dela liste du père à la liste du fils */
  /* si p est la tête de liste de <dad> */
  if (p==dad->ptl) dad->ptl=p->next;
  /* p est enlevé de sa liste initiale */
  if (p->prev) p->prev->next = p->next;
  if (p->next) p->next->prev = p->prev;
  dad->nbp--;
  /* p est inséré en tete sur la liste de <son> */
  p->next  = son->ptl;
  p->prev  = NULL;
  if (son->ptl) son->ptl->prev=p;
  son->ptl = p;
  son->nbp++;
}

static bool point_in_4node(QuadNode *node, ObjList *p)
{ /* renvoie True si le point <p->pos> est dans le noeud <node> */
  double x=fabs(node->C.x-p->pos.x); if (x > node->dim.x) return false;
  double y=fabs(node->C.y-p->pos.y); if (y > node->dim.y) return false;
  return true;
}

static void set_sons_pt(QuadNode *dad, QuadNode *son)
{ /* crée la liste des points dans <node> à partir de */
  ObjList *p=dad->ptl,*next=NULL;

  while (p)
  {
    next=p->next;
    if (point_in_4node(son,p))
    {     
      change_node_list(p,dad,son);
    }
    p=next;
  }
}

static void reset_4node(QuadNode* node)
{
  if (!node) return;

  int i;
  node->ptl = NULL;
  node->nbp = 0;
  for (i=0;i<DIM;i++)
  {
    reset_4node(node->son[i]);
  }
}

static void init_4node(QuadNode* node, G2Xpoint DL, G2Xpoint UR, int lev)
{ /* initialise le _4node_ */
  node->DL  = DL;
  node->UR  = UR;
  node->C   = g2x_Milieu(DL,UR);
  node->dim = (G2Xvector){0.5*fabs(UR.x-DL.x),0.5*fabs(UR.y-DL.y)};
  node->ptl = NULL;
  node->nbp = 0;
  node->lev = lev;
}

static bool split_4node(QuadNode* node)
{/* subdivise le _4node_ => 4 fils */

  /* si il y a 0 ou 1 point dans le noeud on arrête */
  if (node->nbp<=1) return true;
  /* si il y en a plus de 1, on subdivise           */
  int i;
  for (i=0;i<DIM;i++)
  {
    if (!node->son[i])
    {
      if (!(node->son[i]=(QuadNode*)calloc(1,sizeof(QuadNode)))) return false;
      node->son[i]->dad = node;
      /* création des nouveaux "coins" */
      G2Xpoint DLi = (G2Xpoint){ node->DL.x + ((i>>0)&1)*node->dim.x,
                                 node->DL.y + ((i>>1)&1)*node->dim.y };
      G2Xpoint URi = (G2Xpoint){ DLi.x+node->dim.x,
                                 DLi.y+node->dim.y };
      /* initialisation */
      init_4node(node->son[i],DLi,URi,node->lev+1);
    }
    /* remplissage des points */
    set_sons_pt(node,node->son[i]);
    /* sudiv récursive si nécessaire */
    split_4node(node->son[i]);
  }
  return true;
}

static void check_4node(QuadNode *node)
{
  static int nbn=0,nbp=0;
  static char tab[256]="";

  if (!node) return;
  fprintf(stderr,"%snode(%2d) [%+.2lf,%+.2lf]->[%+.2lf,%+.2lf] [%+.2lf,%+.2lf]: %d points\t",
                  tab,nbn,node->DL.x,node->DL.y,node->UR.x,node->UR.y,node->C.x,node->C.y,node->nbp);
  if (node->nbp==1)
  {
    fprintf(stderr,"[%+.2lf,%+.2lf] [%d]\n",node->ptl->pos.x ,node->ptl->pos.y, nbp);
    nbp++;
  }
  else
    fprintf(stderr,"\n");
  nbn++;
  int i;
  for (i=0;i<DIM;i++)
  {
    check_4node(node->son[i]);
  }
}

static void check_List(QuadNode *node)
{
  ObjList *p = node->ptl;
  while (p)
  {
    fprintf(stderr,"%3d->",(int)(p-PTL));
    p=p->next;
  }
  fprintf(stderr,"\n");
}

/*=====================================*/
/*=                                   =*/
/*=====================================*/
static void Init(void)
{
  ObjList *p;
  init_4node(&QT,(G2Xpoint){-1.,-1.},(G2Xpoint){+1.,+1.},0);
  
  /* les points créés sont tous dans le noeud racine */
  for (p=PTL; p<PTL+N; p++)
  {
    p->pos.x = g2x_Rand_MinMax(-1.,+1.);
    p->pos.y = g2x_Rand_MinMax(-1.,+1.);
    p->vit.x = g2x_Rand_MinMax(-.0005,+.0005);
    p->vit.y = g2x_Rand_MinMax(-.0005,+.0005);
    p->next  = QT.ptl;
    if (QT.ptl) QT.ptl->prev=p;
    QT.ptl   = p;
    QT.nbp++;
  }
  /* le découpage récursif */
  split_4node(&QT);

  g2x_FillColorMap(PCOL,N);
  g2x_FillColorMap_va(QCOL,N,1.,0.9);
}

/*=====================================*/
/*=                                   =*/
/*=====================================*/
static void Anim(void)
{
  reset_4node(&QT);
  ObjList *p;
  for (p=PTL; p<PTL+N; p++)
  {
    p->pos.x += p->vit.x; if (fabs(p->pos.x)>1.) { p->vit.x*=-1.; p->pos.x += p->vit.x; }
    p->pos.y += p->vit.y; if (fabs(p->pos.y)>1.) { p->vit.y*=-1.; p->pos.y += p->vit.y; }
    p->next  = QT.ptl;
    if (QT.ptl) QT.ptl->prev=p;
    QT.ptl   = p;
    QT.nbp++;
  }
  /* le découpage récursif */
  split_4node(&QT);
}


static void Show_Points(void)
{
  ObjList *p;
  for (p=PTL; p<PTL+N; p++) g2x_Plot(p->pos.x,p->pos.y,PCOL[(int)(p-PTL)],4);
}

static void Show_Octree(QuadNode *node)
{
  G2Xcolor col;
  if (!node) return;
  if (node->nbp==1)
  {
    col = QCOL[(int)(node->ptl-PTL)];
    g2x_FillRectangle(node->DL.x,node->DL.y,node->UR.x,node->UR.y,col);
    g2x_Rectangle(node->DL.x,node->DL.y,node->UR.x,node->UR.y,G2Xwa,1);
  } 
  int i;
  for (i=0;i<DIM;i++)
    Show_Octree(node->son[i]);
}

/*=====================================*/
/*=                                   =*/
/*=====================================*/
static void Draw(void)
{
  Show_Octree(&QT);
  Show_Points();
}

/*=====================================*/
/*=                                   =*/
/*=====================================*/
static void Exit(void)
{
  return;
}

/*=====================================*/
/*=                                   =*/
/*=====================================*/
int main(int argc, char** argv)
{ 
  /* initialisation de la fenêtre graphique et paramétrage Gl */
  g2x_InitWindow(*argv,1024,1024);
 /* définition des fonctions */
  g2x_SetInitFunction(Init);     /* la fonction de sortie */
  g2x_SetExitFunction(Exit);     /* la fonction de sortie */
  g2x_SetDrawFunction(Draw);     /* la fonction de Dessin */
  g2x_SetAnimFunction(Anim);
	/* boucle d'exécution principale */
  return g2x_MainStart();
  /* rien après ça */
}

