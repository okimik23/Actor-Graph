/*
 * ActorGraph.hpp
 * Author: Adrian Guthals
 * Date: 2/24/2015
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

/*
 * Jae Song
 * A12042160, jys023
 * Date: 3/10/16
 * CSE 100 HW 4
 */
#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include "ActorNode.hpp"
#include "ActorEdge.hpp"
#include "Movie.hpp"
#include "Disjoint.hpp"

using namespace std;

/*
 * Description:
 * 		header for ActorGraph, the class design of the
 * 		Graph Data structure
 */
class ActorGraph {
protected:
  
  //member variables
  std::unordered_map<string, ActorNode*> actors;
  std::unordered_map<string, Movie*> movies;
  std::vector<Disjoint*> set;
  int min_year = 9999;

  /**
   * Name: BFSTraverse
   * Return: true if path is found, false otherwise
   * Description:
   * 	It will use the BFS algorithm to find the shortest
   * 	unweighted path between two ActorNodes
   */
  bool BFSTraverse(ActorNode* start, ActorNode* end, int year);

  /*
   * Name: DijkTraverse
   * Return: true if path is found, false otherwise
   * Description:
   * 	It will use Dijkstra's algorithm to find the shortest
   * 	weighted path between two ActorNodes
   *
   */
  bool DijkTraverse(ActorNode* start, ActorNode* end);

  /*
   * Name: BFSSearch
   * Return: int, year at which they become connected
   * Description:
   * 	It will check if the two actors are connected at a
   * 	certain year and return that year using BFS
   */
  int BFSSearch(ActorNode* start, ActorNode* end);

  /*
   * Name: UnionFind
   * Return: int, year at which the two ActorNodes become
   * 		 connected
   * Description:
   * 	It will check if th two actors are connect4ed at a
   * 	certain year and return that year using union find and 
   * 	disjoint sets
   */
  int UnionFind(ActorNode*, ActorNode* end);

public:
  ActorGraph(void);

  ~ActorGraph();

  /** You can modify this method definition as you wish
   *
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
   *
   * return true if file was loaded sucessfully, false otherwise
   */
  bool loadFromFile(const char* in_filename); 

  /** 
   * Name: findPath
   * Parameter: in_filename, out_filename. use_weighted
   * Return: true if found, false otherwise, or if in_filename is valid
   * Description:
   * 	will find the path using either BFS or Dijkstras
   */
  bool findPath(const char* in_filename, const char* out_filename, 
  				bool use_weighted);
  
  /**
   * Name: moviespan
   * Return: true if read in_filename is valid false if not
   * Description:
   * 	will find the year that connects the pair using either
   * 	BFS or Union find
   */
  bool moviespan(const char* in_filename, const char* out_filename,
  				 bool ufind);
  
  /*
   * Name: averageDist
   * Return: true if read in_filename is valid false otherwise
   * Description:
   * 	will calculate the average shortest unweighted path of a
   * 	certain actor to all possible actors
   */
  bool averageDist(const char* out_filename, std::string actorName);
  
};


#endif // ACTORGRAPH_HPP
