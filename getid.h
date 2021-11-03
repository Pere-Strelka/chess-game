#ifndef GETID_H
#define GETID_H

class ID
{
public: 
   static unsigned int getNew() { static int id = -1; return ++id;  }
   static const unsigned int NoID = 31946184;
};

#endif // GETID_H
