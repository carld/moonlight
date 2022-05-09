/*
 * MUIBase.C (c) 2002 Gregor Mueckl
 */

#include "MUIBase.h"

namespace MetaUI {
  Widget::Widget()
  {
    parent=0;
    children.clear();
  }

  Widget::~Widget()
  {
    if(parent!=0)
      parent->unregisterChild(this);
  }

  void Widget::registerChild(Widget *w)
  {
    children.push_back(w);
    cout << "MetaUI::Widget::registerChild() registration successful; number of children: " << children.size() << endl;
  }

  void Widget::unregisterChild(Widget *w)
  {
    vector<Widget *>::iterator vi;

    for(vi=children.begin();vi!=children.end();++vi) {
      if(*vi==w) {
	children.erase(vi);
	cout << "MetaUI::Widget::unregisterChild() removal successful" << endl;
	return;
      }
    }
  }

  void Widget::redrawAll()
  {
    vector<Widget *>::iterator vi;
    int i;
    
    cout << "MetaUI::Widget::redrawAll() redrawing myself" << endl;

    redraw();
    cout << "MetaUI::Widget::redrawAll() going through " << children.size() << " children" << endl;
    for(i=0;i<children.size();++i) {
      cout << "MetaUI::Widget::redrawAll() redrawAll on a child" << endl;
      children[i]->redrawAll();
    }
  }

  void Widget::setParent(Widget *p)
  {
    if(parent!=0)
      parent->unregisterChild(this);

    parent=p;

    if(parent!=0)
      parent->registerChild(this);
  }

  Widget *Widget::getParent()
  {
    return parent;
  }
}
