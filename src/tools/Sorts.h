// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*
   Sorts.h

   Methodes de tris generiques

   Stephane Rehel
   24 octobre 1994
*/

#ifndef __Sorts_h
#define __Sorts_h

/////////////////////////////////////////////////////////////////////////////

template<class T>
class Sorts
{
private:
  T* t;
  void _quickSort( int g, int d );

  T* s;
  void fusion( int g, int d );
public:
  static void insertion( T* t, int n ); // O(n2)
  static void selection( T* t, int n ); // O(n2)
  void quickSort( T* _t, int n ) // O(n.log(n))
    {
    t= _t;
    _quickSort( 0, n-1 );
    }

  void fusion( T* t, int n ); // O(n.log(n))
  static void shell( T* t, int n ); // O(n.log(n))
};

////////////////////////////////////////////////////////////SORT_Insertion()/
// Tri par insertion
//
// static
template<class T>
void Sorts<T>::insertion( T* t, int n )
{
  int i, j;
  T v, *pi, *pj;

  pi= t+1;
  for( i= 1; i< n; ++i, ++pi )
    {
    v= *pi;
    pj= pi-1;
    j= i-1;
    while( *pj > v && j>=0 )
      {
      *(pj+1)= *pj;
      --j; --pj;
      }
    *(pj+1)= v;
    }
}


///////////////////////////////////////////////////////////SORT_Selection()/
// Tri par selection
//
// static
template<class T>
void Sorts<T>::selection( T* t, int n )
{
  T *pi, *pj, *pmin;
  T tmp;

  pi= t;
  for( int i= 0; i< n-1; ++i, ++pi )
    {
    pmin= pi;
    pj= pi+1;
    for( int j= i+1; j< n; ++j, ++pj )
      if( *pj < *pmin )
        pmin= pj;
    // swap( *pmin, *pi );
    tmp= *pmin;
    *pmin= *pi;
    *pi= tmp;
    }
}


///////////////////////////////////////////////////////////SORT_QuickSort()/
// Tri par segmentation (QuickSort) recursif
// Version recursive
//
// Source: Niklaus Wirth, p. 191
//
template<class T>
void Sorts<T>::_quickSort( int g, int d )
{
  int i= g;
  int j= d;
  T v= t[ (g+d) / 2 ];
  T tmp;

  do{
    while( t[i] < v ) ++i;
    while( v < t[j] ) --j;
    if( i <= j )
      {
      // swap( t[i], t[j] );
      tmp= t[i];
      t[i]= t[j];
      t[j]= tmp;
      ++i;
      --j;
      }
    }
  while( i <= j );

  if( g < j )
    _quickSort( g, j );
  if( i < d )
    _quickSort( i, d );
}


///////////////////////////////////////////////////////////////SORT_Fusion()/
// Tri par fusion
//
// Source: Robert Sedgewick, p. 176
//
template<class T>
void Sorts<T>::fusion( int g, int d )
{
  int m= (g+d)/2;
  if( m > g )
    fusion( g, m );
  if( d > m+1 )
    fusion( m+1, d );

  int i, j;
  for( i= m+1; i> g; --i )
    s[i-1]= t[i-1];
  for( j= m; j< d; ++j )
    s[d+m-j]= t[j+1];

  int k;
  for( k= g, i= g, j= d; k<= d; ++k )
    t[k]= (s[i] < s[j]) ? s[i++] : s[j--];
}


template<class T>
void Sorts<T>::fusion( T* _t, int n )
{
  if( n == 1 )
    return;

  t= _t;
  s= new T [ n ];
  assert( s != 0 );
  fusion( 0, n-1 );
  delete s;
}


////////////////////////////////////////////////////////////////SORT_Shell()/
// Tri Shell
//
// Source: Robert Sedgewick, p. 114
//
template<class T>
// static
void Sorts<T>::shell( T* t, int n )
{
  int h;

  for( h= 1; h<= n/9; h= 3*h+1 );

  for( ; h> 0; h/= 3 )
    for( int i= h; i< n; ++i )
      {
      T v= t[i];
      int j= i;
      while( j+1 > h && t[j-h] > v )
        {
        t[j]= t[j-h];
        j -= h;
        }
      t[j]= v;
      }
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Sorts_h
