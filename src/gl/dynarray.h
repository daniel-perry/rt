#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>
#include <assert.h>

// Written by Pete Shirley
// a dynarray stores data in an ordered random access structure with
// no delete operations.  Items are added with append.

template <class T>
class dynarray { 
 public:

   dynarray();
   dynarray(int);
   ~dynarray();
   dynarray(const dynarray<T>&);
   dynarray<T>& operator=(const dynarray<T>&);

   bool append(T item); // always tagged to end
   bool truncate();     // make arraySize = nData;
   void clear() { nData = 0; }

   int length() const { return nData; }
   bool empty() const { return length() == 0; }
   
   const T &operator[] (int i) const 
   { 
     assert(i >= 0 && i < length()); 
     return data[i]; 
   }

   T &operator[] (int i) 
   { 
     assert(i >= 0 && i < length()); 
     return data[i]; 
   }

   dynarray sub( int index, int len ) const;
  //private:
   T *data;
   int nData;
   int arraySize;
};

template <class T>
dynarray<T> dynarray<T>::sub( int index, int len ) const {
  dynarray<T> temp(len);
  for ( int i=0 ; i< len ; i++ )
    temp.append((*this)[index+i]);
  return temp;
}

template <class T>
dynarray<T>::dynarray() {
   nData = 0;
   arraySize = 4;
   data = new T[arraySize];
}

template <class T>
dynarray<T>::dynarray(int a) {
   nData = 0;
   arraySize = a;
   data = new T[arraySize];
}

template <class T>
dynarray<T>::dynarray(const dynarray<T>& t) {
   nData = t.length();
   arraySize = t.arraySize;
   data = new T[arraySize];
   for (int i = 0; i < t.length(); i++)
      data[i] = t[i];
}

template <class T>
dynarray<T>::~dynarray() {
   nData = 0;
   delete [] data;
}

template <class T>
dynarray<T>& dynarray<T>::operator=(const dynarray<T>& t) {
   if (&t == this) return *this;
   if (data != 0) delete [] data;
   nData = t.length();
   arraySize = t.arraySize;
   data = new T[arraySize];
   for (int i = 0; i < t.length(); i++)
      data[i] = t[i];
   return *this;
}

template <class T>
bool dynarray<T>::truncate()
{
  if(nData != arraySize)
  {
    T *temp = data;
    arraySize = nData;
    if(!(data = new T[arraySize])) return false;
    for(int i = 0; i < nData; i++)
      data[i] = temp[i];
    delete [] temp;
  }
  return true;
}

template <class T>
bool dynarray<T>::append(T item) {
    if (nData == arraySize) {
       arraySize *= 2;
       T *temp = data;
       if (!(data = new T[arraySize])) return false;
       for (int i = 0; i < nData; i++)
           data[i] = temp[i];
       delete [] temp;
    }
    data[nData++] = item;
    return true;
}




#endif








