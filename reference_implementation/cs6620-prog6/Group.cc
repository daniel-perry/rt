
#include "Group.h"
#include "BoundingBox.h"
using namespace std;

Group::Group()
{
}

Group::~Group()
{
}

void Group::addObject(Object* object)
{
  objects.push_back(object);
}

void Group::preprocess()
{
  Object*const* begin = &objects[0];
  Object*const* end = &objects[objects.size()];
  while (begin != end)
    (*begin++)->preprocess();
}

void Group::getBounds(BoundingBox& bbox) const
{
  Object*const* begin = &objects[0];
  Object*const* end = &objects[objects.size()];
  while (begin != end)
    (*begin++)->getBounds(bbox);
}

void Group::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
#if 0
  for(vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); iter++)
    (*iter)->intersect(hit, context, ray);
#else
#if 0
  int s=objects.size();
  for(int i=0;i<s;i++)
    objects[i]->intersect(hit, context, ray);
#else
  Object*const* begin = &objects[0];
  Object*const* end = &objects[objects.size()];
  while (begin != end)
    (*begin++)->intersect(hit, context, ray);
#endif
#endif
}
