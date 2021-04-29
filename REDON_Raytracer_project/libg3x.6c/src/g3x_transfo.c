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

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <g3x_types.h>
#include <g3x_transfo.h>

extern G3Xhmat g3x_Identity(void) 
{
  G3Xhmat Id;
  memset(Id.m,0,16*sizeof(double));
  Id.m[a00] = Id.m[a11] = Id.m[a22] = Id.m[a33] = 1.0;
  return Id;
}

extern G3Xhmat g3x_MakeTranslationV(G3Xvector t) 
{
  G3Xhmat A=g3x_Identity();
  A.m[a03] = t.x;
  A.m[a13] = t.y;
  A.m[a23] = t.z;
  return A;
}

extern G3Xhmat g3x_MakeTranslationXYZ(double tx, double ty, double tz)
{
  G3Xhmat A=g3x_Identity();
  A.m[a03] = tx;
  A.m[a13] = ty;
  A.m[a23] = tz;
  return A;
}

extern G3Xhmat g3x_MakeHomothetieV(G3Xvector h) 
{
  G3Xhmat A=g3x_Identity();
  A.m[a00] = h.x;
  A.m[a11] = h.y;
  A.m[a22] = h.z;
  return A;
}

extern G3Xhmat g3x_MakeHomothetieXYZ(double hx, double hy, double hz) 
{
  G3Xhmat A=g3x_Identity();
  A.m[a00] = hx;
  A.m[a11] = hy;
  A.m[a22] = hz;
  return A;
}

extern G3Xhmat g3x_MakeRotationX(double a_radians) 
{
  G3Xhmat A=g3x_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[a11] = +c; A.m[a12] = -s;
  A.m[a21] = +s; A.m[a22] = +c;
  return A;
}

extern G3Xhmat g3x_MakeRotationY(double a_radians) 
{
  G3Xhmat A=g3x_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[a00] = +c; A.m[a02] = +s;
  A.m[a20] = -s; A.m[a22] = +c;
  return A;
}

extern G3Xhmat g3x_MakeRotationZ(double a_radians) 
{
  G3Xhmat A=g3x_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[a00] = +c; A.m[a01] = -s;
  A.m[a10] = +s; A.m[a11] = +c;
  return A;
}

extern G3Xvector g3x_ProdHMatVector(G3Xhmat A, G3Xvector V) 
{	
	G3Xvector U;
  U.x = A.m[a00]*V.x + A.m[a01]*V.y + A.m[a02]*V.z;
  U.y = A.m[a10]*V.x + A.m[a11]*V.y + A.m[a12]*V.z;
  U.z = A.m[a20]*V.x + A.m[a21]*V.y + A.m[a22]*V.z;
	return U;
}

extern G3Xpoint g3x_ProdHMatPoint(G3Xhmat A, G3Xpoint P)
{
	G3Xpoint M;
  M.x = A.m[a00]*P.x + A.m[a01]*P.y + A.m[a02]*P.z + A.m[a03];
  M.y = A.m[a10]*P.x + A.m[a11]*P.y + A.m[a12]*P.z + A.m[a13];
	M.z = A.m[a20]*P.x + A.m[a21]*P.y + A.m[a22]*P.z + A.m[a23];
	return M;
}

extern G3Xcoord g3x_ProdHMatXYZ(G3Xhmat A, double x, double y, double z, double delta)
{
	G3Xcoord M;
  M.x = A.m[a00]*x + A.m[a01]*y + A.m[a02]*z + A.m[a03]*delta;
  M.y = A.m[a10]*x + A.m[a11]*y + A.m[a12]*z + A.m[a13]*delta;
	M.z = A.m[a20]*x + A.m[a21]*y + A.m[a22]*z + A.m[a23]*delta;
	return M;
}

extern G3Xhmat g3x_ProdHMat(G3Xhmat A,G3Xhmat B) 
{
	G3Xhmat D;
  D.m[a00] = A.m[a00]*B.m[a00] + A.m[a01]*B.m[a10] + A.m[a02]*B.m[a20] + A.m[a03]*B.m[a30];
  D.m[a10] = A.m[a10]*B.m[a00] + A.m[a11]*B.m[a10] + A.m[a12]*B.m[a20] + A.m[a13]*B.m[a30];
  D.m[a20] = A.m[a20]*B.m[a00] + A.m[a21]*B.m[a10] + A.m[a22]*B.m[a20] + A.m[a23]*B.m[a30];
  D.m[a30] = A.m[a30]*B.m[a00] + A.m[a31]*B.m[a10] + A.m[a32]*B.m[a20] + A.m[a33]*B.m[a30];
  D.m[a01] = A.m[a00]*B.m[a01] + A.m[a01]*B.m[a11] + A.m[a02]*B.m[a21] + A.m[a03]*B.m[a31];
  D.m[a11] = A.m[a10]*B.m[a01] + A.m[a11]*B.m[a11] + A.m[a12]*B.m[a21] + A.m[a13]*B.m[a31];
  D.m[a21] = A.m[a20]*B.m[a01] + A.m[a21]*B.m[a11] + A.m[a22]*B.m[a21] + A.m[a23]*B.m[a31];
  D.m[a31] = A.m[a30]*B.m[a01] + A.m[a31]*B.m[a11] + A.m[a32]*B.m[a21] + A.m[a33]*B.m[a31];
  D.m[a02] = A.m[a00]*B.m[a02] + A.m[a01]*B.m[a12] + A.m[a02]*B.m[a22] + A.m[a03]*B.m[a32];
  D.m[a12] = A.m[a10]*B.m[a02] + A.m[a11]*B.m[a12] + A.m[a12]*B.m[a22] + A.m[a13]*B.m[a32];
  D.m[a22] = A.m[a20]*B.m[a02] + A.m[a21]*B.m[a12] + A.m[a22]*B.m[a22] + A.m[a23]*B.m[a32];
  D.m[a32] = A.m[a30]*B.m[a02] + A.m[a31]*B.m[a12] + A.m[a32]*B.m[a22] + A.m[a33]*B.m[a32];
  D.m[a03] = A.m[a00]*B.m[a03] + A.m[a01]*B.m[a13] + A.m[a02]*B.m[a23] + A.m[a03]*B.m[a33];
  D.m[a13] = A.m[a10]*B.m[a03] + A.m[a11]*B.m[a13] + A.m[a12]*B.m[a23] + A.m[a13]*B.m[a33];
  D.m[a23] = A.m[a20]*B.m[a03] + A.m[a21]*B.m[a13] + A.m[a22]*B.m[a23] + A.m[a23]*B.m[a33];
  D.m[a33] = A.m[a30]*B.m[a03] + A.m[a31]*B.m[a13] + A.m[a32]*B.m[a23] + A.m[a33]*B.m[a33];
	return D;
}

/* conversion (transposition) au format de amatrice OpenGl */
extern  G3Xhmat g3x_FormatGLmat(G3Xhmat src)
{
  G3Xhmat dst;
  dst.m[a00]=src.m[a00]; dst.m[a10]=src.m[a01]; dst.m[a20]=src.m[a02]; dst.m[a30]=src.m[a03];
  dst.m[a01]=src.m[a10]; dst.m[a11]=src.m[a11]; dst.m[a21]=src.m[a12]; dst.m[a31]=src.m[a13];
  dst.m[a02]=src.m[a20]; dst.m[a12]=src.m[a21]; dst.m[a22]=src.m[a22]; dst.m[a32]=src.m[a23];
  dst.m[a03]=src.m[a30]; dst.m[a13]=src.m[a31]; dst.m[a23]=src.m[a32]; dst.m[a33]=src.m[a33];
  return dst;
}


extern void g3x_PrintHMat(G3Xhmat A) 
{
  fprintf(stderr,"Matrice :\n");
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[a00],A.m[a01],A.m[a02],A.m[a03]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[a10],A.m[a11],A.m[a12],A.m[a13]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[a20],A.m[a21],A.m[a22],A.m[a23]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[a30],A.m[a31],A.m[a32],A.m[a33]);
  fprintf(stderr,"\n");
}

#ifdef __cplusplus
	}
#endif
/*=============================================================================*/

