/*
 * ActorGraph.cpp
 * Author: Adrian Guthals
 * Date: 2/24/2015
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include "ActorGraph.hpp"

using namespace std;

ActorGraph::ActorGraph(void) {}


bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted)
{
  
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;
  ActorNode* node = 0;
  Movie* movie = 0;
  ActorEdge* edge = 0;
  int index = 0;

  // keep reading lines until the end of file is reached
  while (infile)
  {
    string s;
    
    // get the next line
    if (!getline( infile, s )) break;
    
    if (!have_header)
    {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss)
    {
      string next;
      
      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;
      
      record.push_back( next );
    }
    
    if (record.size() != 3)
    {
      // we should have exactly 3 columns
      continue;
    }
    
    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);
	string movie_key = record[1] + record[2];
    
	if(actors.find(actor_name) == actors.end())
	{
	  node = new ActorNode(actor_name);
	  node->index = index++;
	  actors.insert({actor_name, node});
	  list.push_back(new ActorEdge());
	}
	else
	{
	  node = actors[actor_name];
	}

	if(movies.find(movie_key) == movies.end())
	{
	  movie = new Movie(movie_title, movie_year);
	  movies.insert({movie_key, movie});
	}
	else
	{
	  movie = movies[movie_key];
	}

	movie->cast.push_back(node);
	list[node->index]->connection.push_back(movie);
  }
  
  if (!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}

bool ActorGraph::findPath(const char* in_filename, const char* out_filename,
						  bool use_weighted)
{
  ifstream infile(in_filename);
  ofstream outfile(out_filename);

  bool have_header = false;
  
  ActorNode* start = 0;
  ActorNode* end = 0;
  
  while(infile)
  {
    string s;
	
	if(!getline(infile,s)) break;
    
	if(!have_header)
	{
	  have_header = true;
	  outfile << "(actor)--[movie#@year]-->(actor)--...\n";
	  continue;
	}

	istringstream ss(s);
	vector<string> record;

	while(ss)
	{
	  string next;
	  if(!getline(ss, next, '\t')) break;

	  record.push_back(next);
	}

	if(record.size() != 2)
	{
	  continue;
	}

	string source(record[0]);
	string dest(record[1]);
	bool find;

	start = actors[source];
	end = actors[dest];
	
	if(use_weighted)
	{
	  find = DijkTraverse(end,start);
	  if(find)
	  {
	    Movie* movTrav = start->prevMovie;
	    ActorNode* trav = start;
        while(1)
		{
		  outfile << "(" << trav->name << ")" << "--";
		  outfile << "[" << movTrav->name << "#@" 
		                 << movTrav->year << "]" << "-->";

		  trav = movTrav->prevActor;
		  if(trav == end)
		  {
		    outfile << "(" << trav->name << ")" << endl;
			break;
		  }
		  movTrav = movTrav->prev;
		}
	  }
	}
	else
	{
	  find = BFSTraverse(end, start);
	  if(find)
	  {
	    ActorNode* trav = start;
	  	while(1)
	  	{
	  	  outfile << "(" << trav->name << ")" << "--";
		  outfile << "[" << trav->prevMovie->name << "#@" 
		                 << trav->prevMovie->year << "]" << "-->";
		  trav = trav->prev;
		  if(trav == end)
		  {
		    outfile << "(" << trav->name << ")" << endl;
		    break;
		  }
	    }
	  }
	}
  }
  
  if(!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
	return false;
  }
  infile.close();
  outfile.close();

  return true;
}

bool ActorGraph::BFSTraverse(ActorNode* start, ActorNode* end)
{
  for(auto it = actors.begin(); it != actors.end(); ++it)
  {
	it->second->dist = std::numeric_limits<int>::max(); 
  }

  queue<ActorNode*> explore;
  start->dist = 0;
  
  explore.push(start);

  while(!explore.empty())
  {
    ActorNode* next = explore.front();
	explore.pop();
    ActorEdge* edge = list[next->index];
	vector<Movie*>::iterator movie_it = edge->connection.begin();
	for( ; movie_it != edge->connection.end(); ++movie_it)
	{
	  Movie* movie = *movie_it;
	  vector<ActorNode*>::iterator actor_it = movie->cast.begin();
	  for( ; actor_it != movie->cast.end(); ++actor_it)
	  {
	    ActorNode* actor = *actor_it;
	  	if(actor->dist == std::numeric_limits<int>::max())
	  	{
	  	  if(actor->index == end->index)
	  	  {
	        actor->prev = next;
			actor->prevMovie = movie;
	        return true;
	  	  }
	  	  else
	  	  {
	        actor->prev = next;
			actor->prevMovie = movie;
		    actor->dist = next->dist + 1;
	        explore.push(*actor_it);
	  	  }
        }
	  }
	}
  }

  return false;
}

bool ActorGraph::DijkTraverse(ActorNode* start, ActorNode* end)
{
  for(auto it = movies.begin(); it != movies.end(); ++it)
  {
    it->second->done = false;
	it->second->weight = 1 + 2015 - it->second->year;
	it->second->dist = 0; 
  }
  std::priority_queue<Movie*, std::vector<Movie*>, MovieComp> pq;
  ActorEdge* edge = list[start->index];
  vector<Movie*>::iterator movie_it = edge->connection.begin();
  
  for( ; movie_it != edge->connection.end(); ++movie_it)
  {
    (*movie_it)->prevActor = start;
	(*movie_it)->dist = (*movie_it)->weight;
    pq.push(*movie_it);
  }

  while(!pq.empty())
  {
    Movie* next = pq.top();
	pq.pop();
	if(!next->done)
	{
	  next->done = true;
	  vector<ActorNode*>::iterator actor_it = next->cast.begin();
	  for( ; actor_it != next->cast.end(); ++actor_it)
	  {
	  	ActorNode* actor = *actor_it;
		if(actor == end)
		{
		  end->prevMovie = next;
		  return true;
		}

	    vector<Movie*>::iterator movie_it = 
						list[actor->index]->connection.begin();
		for( ; movie_it != list[actor->index]->connection.end(); ++movie_it)
		{
		  Movie* movie = *movie_it;
		  if(!movie->done)
		  {
		    movie->dist = movie->weight + next->dist; 
			movie->prev = next;
		    movie->prevActor = actor;
		    pq.push(movie);
		  }
		}
	  }
	}
  }
  return false; 
}
