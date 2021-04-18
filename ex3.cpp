/*
Created by HU Chuxuan on 18.04.21
*/
//algorithm implemented in search of perfect matches from K.D.



#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <iostream>
using namespace std;
using std::vector; 
using std::set; 
using std::pair;

//Created a new class type according to basic features of bipartite graphs
class bipartite{
    public:
       void insert(int boy,int girl); //insertion

       //primary function
       bool check_perfect();

       //helper functions
       set<pair<int,int>> girls_to_boys(set<set<int>> list);
       set<pair<int,int>> boys_to_girls(set<set<int>> list);
       set<set<int>> unordered(set<pair<int,int>> list);
       bool reachable(int boy, set<pair<int,int>> digraph, int girl, set<pair<int,int>>& path, set<pair<int,int>>& visited);

    private:
       set<int> BOYS;
       set<int> GIRLS;
       set<set<int>> Edges;
       set<set<int>> partial_match;
};


//INSERTION
void bipartite::insert(int boy,int girl){
    set<int> edge;
    edge.insert(boy);
    edge.insert(girl);
    Edges.insert(edge);

    BOYS.insert(boy);
    GIRLS.insert(girl);
}


bool bipartite::check_perfect(){
    //if the number are not equal, this definitely cannot form a perfect match
    if(BOYS.size()!=GIRLS.size()) return false;

    //check every round
    while(1){
        //EXAMINE
        set<int>::iterator iter1=BOYS.begin();
        int boy_check;
        //we look for the unmatched boy
        while(iter1!=BOYS.end()){
            boy_check=*iter1;
            bool matched=0;
            set<int>::iterator iter2=GIRLS.begin();
            set<int> check;
            check.insert(boy_check);
            while(iter2!=GIRLS.end()){
                check.insert(*iter2);
                if(partial_match.find(check)!=partial_match.end()) matched=1;
                check.erase(*iter2);
                iter2++;
            }
            if(matched==0) break;
            iter1++;
        }
        //if there's no such boy, we have already found a perfect match
        if(iter1==BOYS.end()) return 1;

        //BUILD-DIGRAPH
        set<pair<int,int>> digraph;
        set<pair<int,int>> gtb=girls_to_boys(partial_match);
        set<set<int>> diff;
        set_difference(Edges.begin(),Edges.end(),partial_match.begin(),partial_match.end(),inserter(diff,diff.begin()));
        set<pair<int,int>> btg=boys_to_girls(diff);
        set_union(gtb.begin(),gtb.end(),btg.begin(),btg.end(),inserter(digraph,digraph.begin()));

        //BUILD PATH
        /*
        We first look for the un-matched girl
        */
        set<int>::iterator iter3=GIRLS.begin();
        int girl_check;
        while(iter3!=GIRLS.end()){
            girl_check=*iter3;
            bool matched=0;
            set<int>::iterator iter4=BOYS.begin();
            set<int> check;
            check.insert(girl_check);
            while(iter4!=BOYS.end()){
                check.insert(*iter4);
                if(partial_match.find(check)!=partial_match.end()) matched=1;
                check.erase(*iter4);
                iter4++;
            }
            if(matched==0) break;
            iter3++;
        }
        //set up the recording path
        set<pair<int,int>> path;
        set<pair<int,int>> visited;
        //check if reachable
        //fill up the path
        if(reachable(boy_check,digraph,girl_check,path,visited)==0) return 0;

        //MODIFICATION
        set<set<int>> u_path=unordered(path);
        set<set<int>> diff1;
        set<set<int>> diff2;
        set_difference(partial_match.begin(),partial_match.end(),u_path.begin(),u_path.end(),inserter(diff1,diff1.begin()));
        set_difference(u_path.begin(),u_path.end(),partial_match.begin(),partial_match.end(),inserter(diff2,diff2.begin()));
        set_union(diff1.begin(),diff1.end(),diff2.begin(),diff2.end(),inserter(partial_match,partial_match .begin()));
    }

}

//For these two helper function, we simply transform pairs into un-ordered sets
set<pair<int,int>> bipartite:: girls_to_boys(set<set<int>> list){
    set<pair<int,int>> ret;
    set<set<int>>::iterator iter1=list.begin();
    while(iter1!=list.end()){
        set<int> check=*iter1;
        set<int>::iterator iter2=check.begin();
        iter2++;
        if(GIRLS.find(*check.begin())!=GIRLS.end()){
            ret.insert(make_pair(*check.begin(),*iter2));
        }else{
            ret.insert(make_pair(*iter2,*check.begin()));
        }
        iter1++;
    }
    return ret;
}

set<pair<int,int>> bipartite:: boys_to_girls(set<set<int>> list){
    set<pair<int,int>> ret;
    set<set<int>>::iterator iter1=list.begin();
    while(iter1!=list.end()){
        set<int> check=*iter1;
        set<int>::iterator iter2=check.begin();
        iter2++;
        if(GIRLS.find(*check.begin())==GIRLS.end()){
            ret.insert(make_pair(*check.begin(),*iter2));
        }else{
            ret.insert(make_pair(*iter2,*check.begin()));
        }
        iter1++;
    }
    return ret;
}

//This helper function transform pairs into sets
set<set<int>> bipartite::unordered(set<pair<int,int>> list){
    set<set<int>> ret;
    set<pair<int,int>>::iterator iter1=list.begin();
    while(iter1!=list.end()){
        pair<int,int> check=*iter1;
        set<int> add;
        add.insert(check.first);
        add.insert(check.second);
        ret.insert(add);
        iter1++;
    }
    return ret;
}


//This healper function applies DFS to find if there exists a path from a boy to a specific girl
bool bipartite::reachable(int boy, set<pair<int,int>> digraph, int girl, set<pair<int,int>>& path, set<pair<int,int>>& visited){
    //means we have reached to the girl
    if(boy==girl) return true;
    //start recursives
    bool ret=0;
    set<pair<int,int>>::iterator itr=digraph.begin();
    //look for every path reachable from this one
    while(itr!=digraph.end()){
        pair<int,int> check= *itr;
        //we check this if not checked and reachable from previous
        if(check.first==boy&&visited.find(check)==visited.end()){
            path.insert(check);
            visited.insert(check);
            //OR the answers together
            ret|=reachable(check.second,digraph,girl,path,visited);
            if(ret) return ret;
            path.erase(check);
        }
        itr++;
    }
    return ret;
}

int main(){

    //TEST 1: simplest case
    bipartite my_graph1;
    my_graph1.insert(1,3);
    my_graph1.insert(2,4);
    if(my_graph1.check_perfect()) cout<<"TEST1:SUCCESS!"<<endl;

    //TEST2: cross occurs
    my_graph1.insert(1,4);
    if(my_graph1.check_perfect()) cout<<"TEST2:SUCCESS!"<<endl;

    //TEST3: two crosses(automatically chose an answer)
    my_graph1.insert(2,3);
    if(my_graph1.check_perfect()) cout<<"TEST3:SUCCESS!"<<endl;

    //TEST4: failures
    bipartite my_graph2;
    my_graph2.insert(1,4);
    my_graph2.insert(2,4);
    my_graph2.insert(3,5);
    my_graph2.insert(3,6);
    my_graph2.insert(3,4);
    if(!my_graph2.check_perfect()) cout<<"TEST4:UNMATCHED!"<<endl;

    return 0;
}


