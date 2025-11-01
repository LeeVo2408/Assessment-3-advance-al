#ifdef KKT_HPP_
#define KKT_HPP_

//Boruvka step to return the chosen edges and the connected components (supernode) for next round
Graph boruvkaStep(const Graph& G) {}

//random function to choose edges with probability 1/2
int randomChoice();

//KKT MST algorithm
Graph kktMST(const Graph& G);

#endif      //KKT_HPP_