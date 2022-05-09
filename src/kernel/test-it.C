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
  this is a short test for tracking memory leaks in the kernel
*/

#include <stdio.h>
#include <stdlib.h>

#include "MLKernel.h"
#include "MLNode.h"
#include "MLProperty.h"
#include "MLPString.h"

#include <new.h>

void h()
{
  char* p= 0;
  *p= 0;
}

int main( int argc, char* argv[] )
{
  set_new_handler(h);

  MLKernel* k= new MLKernel();
  k->init(argc,argv);

  MLNode* root= k->getRootNode();

  MLNode* scene= root->newChild("scene");

  scene->setName("scene");
  MLProperty* p= scene->newString("vertices","");

  k->getRootNode()->print();

  const int max_props= 10;
  const int n_nodes= 30;
  int ids[n_nodes];
  memset( (void*)ids,0,n_nodes*sizeof(int) );

  int nloops= 30000;//atoi(argv[1]);

  printf( "creating %d nodes in a table of %d ...\n", nloops, n_nodes );

//for(;;) delete root->newInteger("i",1);
//for(;;) delete root->newChild("");

  for( int i= 0; i < nloops; ++i )
    {
    if( (i % 500) == 0 )
      printf( "%d/%d\n",i,nloops);

    int ii= rand() % n_nodes;
    if( ids[ii] != 0 )
      {
      delete k->getNode(ids[ii]);
      ids[ii]= 0;
      }

    int ok= 0;
    for( int jj= 0; jj < n_nodes; ++jj )
      if( k->getNode( ids[jj] ) != 0 )
        { ok= 1; break; }

    MLNode* root0= 0;
    if( ok )
      {
      while( root0 == 0 )
        root0= k->getNode( ids[rand() % n_nodes] );
      }
     else
      root0= root;

    MLNode* node= root0->newChild("");
    ids[ii]= node->getID();

    int n_props= rand() % 10;
    while( n_props-- > 0 )
      node->newInteger("",n_props);
    }

  k->getRootNode()->print();

//  k->dump_nodes_ids();

  printf( "freeing kernel\n");

  delete k;

  printf( "kernel successfully freed\n");

  return 0;
}
