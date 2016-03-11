/*
 * Jae Song
 * jys023, A12042160
 * Date: 3/10/16
 * CSE 100 HW 4
 * FileName: Disjoint.hpp
 *
 */
#ifndef DISJOINT_HPP
#define DISJOINT_HPP

#include <vector>
#include "ActorNode.hpp"

/*
 * Name: Disjoint
 * Description: 
 * 		a disjoint set representation of the actorNodes
 * 		implements the disjoint sets that will be used for
 * 		union find
 *
 */
class Disjoint
{
  public:

  	//member variables
    Disjoint* sentinel;
	unsigned int size;
	int index;
    ActorNode* node;

	/* CONSTRUCTOR     */
    Disjoint(ActorNode* root)
	{
	  sentinel = this;
	  size = 1;
	  index = root->index;
	  node = root;
	}

	/* DECONSTRUCTOR   */
	~Disjoint()
	{}
	
	/*
	 * Name: Union
	 * Parameter: Disjoint* other
	 * Return: void
	 * Description:
	 * 		this will essentially unifiy two different sets
	 *
	 */
  	void Union(Disjoint* other)
	{
	  //case if they are already connected...
	  if(sentinel == other->sentinel)
	  {
	    return;
	  }

	  //if not connected check the sizes
	  if(size < other->size)
	  {
		//reach the highest sentinel
	    while(other->sentinel != other->sentinel->sentinel)
		{
		  other->sentinel = other->sentinel->sentinel;
		}

		//reset sizes
	    other->sentinel->size += size;
		other->size = size = other->sentinel->size;

		//call recursive function to make optimal up-tree
	  	resetSentinel(other->sentinel);
	  }

	  else
	  {
	    //reach the highest sentinel
	    while(sentinel != sentinel->sentinel)
		{
		  sentinel = sentinel->sentinel;
		}

		//reset sizes
		sentinel->size += other->sentinel->size;
		size = other->size = sentinel->size;

		//recursive function to make optimal up-tree
	    other->resetSentinel(sentinel);
	  }
	}

	/*
	 * Name: Find
	 * Parameter: none
	 * Return: Disjoint*
	 * Description:
	 * 		will return the disjoint set that is representative
	 * 		of the whole set (sentinel)
	 */
	Disjoint* Find()
	{
	  //get the highest sentinel
	  while(sentinel != sentinel->sentinel)
	  {
	    sentinel = sentinel->sentinel;
	  }
	  return sentinel;
	}

private:
	
	/* 
	 * Name: resetSentinel
	 * Parameter: Disjoint* newSent
	 * Return: void
	 * Description:
	 * 		essentially make every disjoint set in an up-tree
	 * 		point to the same sentinel set
	 */
	void resetSentinel(Disjoint* newSent)
	{
	  //base case
	  if(sentinel == this)
	  {
	    sentinel = newSent;
		return;
	  }

	  //recursive call on one higher up
	  sentinel->resetSentinel(newSent);

	  //reset sentinel
	  sentinel = newSent;
	}
};
#endif
