#ifndef DISJOINT_HPP
#define DISJOINT_HPP

#include <vector>
#include "ActorNode.hpp"


class Disjoint
{
  public:
    Disjoint* sentinel;
	unsigned int size;
	int index;
    ActorNode* node;

    Disjoint(ActorNode* root)
	{
	  sentinel = this;
	  size = 1;
	  index = root->index;
	  node = root;
	}

  	void Union(Disjoint* other)
	{
	  if(sentinel == other->sentinel)
	  {
	    return;
	  }
	  if(size < other->size)
	  {
	    while(other->sentinel != other->sentinel->sentinel)
		{
		  other->sentinel = other->sentinel->sentinel;
		}
	    other->sentinel->size += size;
		other->size = size = other->sentinel->size;
	  	resetSentinel(other->sentinel);
	  }
	  else
	  {
	    while(sentinel != sentinel->sentinel)
		{
		  sentinel = sentinel->sentinel;
		}
		sentinel->size += other->sentinel->size;
		size = other->size = sentinel->size;
	    other->resetSentinel(sentinel);
	  }
	}

	void resetSentinel(Disjoint* newSent)
	{
	  if(sentinel == this)
	  {
	    sentinel = newSent;
		return;
	  }
	  sentinel->resetSentinel(newSent);
	  sentinel = newSent;
	}

	Disjoint* Find()
	{
	  while(sentinel != sentinel->sentinel)
	  {
	    sentinel = sentinel->sentinel;
	  }
	  return sentinel;
	}
};
#endif
