#include "Object.h"

ostream & operator<<(ostream &os , Object & o ){
  os << o.toString();
  return os;
}
ostream & operator<<(ostream &os , Object * o ){
  os << o->toString();
  return os;
}

