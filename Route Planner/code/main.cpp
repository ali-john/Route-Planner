#include <iostream>
#include "LinkedList.h"
#include<queue>
#include <list>
#include <iterator>
#include <cstring>
#include <stack>
#include <fstream>
#include <windows.h>
#define INF 10000000
using namespace std;

// Structure for maintaining edges of a town.

    struct Town_path_info
    {
        string second_town_name;
        int time;
        int distance;
        int cost;
    };


//structure for maintaining towns adjacency list


    struct TownInfo
    {
        string town_name;
        list<Town_path_info> town_connections_list;
    };


// structure for maintaining record table of towns used in shortest path computations


    struct town_record_table_node
    {
        string _town_name;
        int shortest_distance_from_source;
        string previous_source;
        bool visited=false;
    };


//structure for city node

template <class T>
    struct CityInfo
    {

        T city_name;
        LinkedList<T> city_connections;
        list<TownInfo> towns_list;
        CityInfo *next;
        list<town_record_table_node> record_table_towns;
    };


// structure for keeping record of visited cities in BFS implementation which is used to check whether
// path exist between two cities or not
template<typename T>
    struct visited_cities_list
    {
        bool visited;
        T name;
        visited_cities_list *next;
    };


// Serves as a node for shortest path calculation table.

    struct record_table
    {
        string _city_name;
        int shortest_distance_from_source;
        string previous_source;
        bool visited=false;
    };




template<typename T>
/*
 *  dynamic adjacency list implementation of graph
 */


    class Map_Navigation
{
    public:

// add a new town to graph

        void add_new_town(T city_for_town,string town_name)

        {
            CityInfo<T> *city_address=search_city_address(city_for_town);

            if(city_address)
            {
                TownInfo obj;
                obj.town_name=town_name;
                city_address->towns_list.push_back(obj);
            }

            else
            {
                cout<<"Invalid city address"<<endl;
            }

        }

// add new edges between different towns of a city.

        void add_new_paths_in_towns(T city_for_town,string town_one,string town_two,int cost_of_path,int _time_of_path,int dist)


        {
            CityInfo<T> *city_address=search_city_address(city_for_town);
            if(city_address== nullptr)
            {
                cerr<<"City in which  you want to add towns does not exist ."<<endl;
                return;
            }
            list<TownInfo> ::iterator  validation_iterator_one=search_for_town_address(city_for_town,town_one);
            list<TownInfo> ::iterator  validation_iterator_two=search_for_town_address(city_for_town,town_two);


                if(validation_iterator_one!=city_address->towns_list.end() && validation_iterator_two!=city_address->towns_list.end())// means both are valid town names
                {

                    Town_path_info obj;
                    obj.cost=cost_of_path;
                    obj.distance=dist;
                    obj.time=_time_of_path;
                    obj.second_town_name=town_two;
                    validation_iterator_one->town_connections_list.push_back(obj);
                    obj.second_town_name=town_one;
                    validation_iterator_two->town_connections_list.push_back(obj);
                }
                else
                {
                    cout<<"Towns does not exist"<<endl;
                }

        }

//Searches town address inside a city node

        list<TownInfo>::iterator  search_for_town_address(T city_for_T, string _town_name)


        {
            CityInfo<T> *city_address=search_city_address(city_for_T);
            list<TownInfo>::iterator  search_iterator_for_towns=city_address->towns_list.begin();
            bool found=false;


                for(search_iterator_for_towns;search_iterator_for_towns!=city_address->towns_list.end();search_iterator_for_towns++)
                {
                    if(search_iterator_for_towns->town_name==_town_name)
                    {
                        found=true;
                        break;
                    }
                }
                if(!found)
                {
                    search_iterator_for_towns=city_address->towns_list.end();
                    return search_iterator_for_towns;
                }
                else
                    return search_iterator_for_towns;
        }


// serves as a helper function in shortest path finding in towns of a city. Fills record table of towns

        void fill_table_for_towns(CityInfo<T> *city_ptr)


        {

            town_record_table_node trt;
             for ( auto  towns_itr=city_ptr->towns_list.begin(); towns_itr!=city_ptr->towns_list.end();towns_itr++)
                {
                    trt._town_name=towns_itr->town_name;
                    trt.previous_source="";
                    trt.shortest_distance_from_source=INF;
                    city_ptr->record_table_towns.push_back(trt);
                }


        }

//serves as a helper function in shortest path calculation for towns. Returns iterator of record table


        list<town_record_table_node>::iterator search_town_recordtable(CityInfo<T> *city_ptr, string town_Name)

                {
                        for (auto temp=city_ptr->record_table_towns.begin();temp!=city_ptr->record_table_towns.end();temp++)
                        {
                            if(temp->_town_name==town_Name)
                            {
                                return temp;
                            }
                        }

                }



// takes input of city name inside which towns are, and starting and ending city

        void computer_shortest_path_for_towns(T city, string starting_town, string ending_town,string wrt)


        {

            CityInfo<T> *city_address=search_city_address(city);
            fill_table_for_towns(city_address);//fill table for one particular city towns


            //save starting and ending cities

            string starting_town_saved=starting_town;
            string ending_town_saved=ending_town;



            //make some iterators

            //iterator for record table
            auto iterator_towns_one=city_address->record_table_towns.begin();


            //set source/starting town distance as 0;
            iterator_towns_one= search_town_recordtable(city_address, starting_town);

            //list<Town_path_info>::iterator iterator_town_two;

            iterator_towns_one->shortest_distance_from_source=0;


            //set minimum cost town string and iterator for traversing edges of a town.

            string minimum_cost_town="";

            //set iterator for traversing towns edges

            list<Town_path_info>::iterator town_paths_adjacencylist_itr;



                do
                {
                    //iterator of TownInfo type that will give specific town address.
                    //take that address and go inside its adjacency list
                    list<TownInfo>::iterator town_names_iterator=search_for_town_address(city, starting_town);



                    for (town_paths_adjacencylist_itr=town_names_iterator->town_connections_list.begin(); town_paths_adjacencylist_itr != town_names_iterator->town_connections_list.end(); town_paths_adjacencylist_itr++)
                    {

                        //search the name in table between which there is an edge
                        iterator_towns_one= search_town_recordtable(city_address,town_paths_adjacencylist_itr->second_town_name);

                        //search main town.
                        auto iterator_town_two=search_town_recordtable(city_address,town_names_iterator->town_name);

                        //add present cost of edge with the cost of taking new edge
                        int cost_of_edge=0;
                            if(wrt=="time")
                            {
                                cost_of_edge=iterator_town_two->shortest_distance_from_source + town_paths_adjacencylist_itr->time;
                            }
                            else if(wrt=="distance")
                            {
                                cost_of_edge=iterator_town_two->shortest_distance_from_source + town_paths_adjacencylist_itr->distance;
                            }
                            else if(wrt=="both")
                            {
                                cost_of_edge=iterator_town_two->shortest_distance_from_source + town_paths_adjacencylist_itr->cost;
                            }

                            if(iterator_towns_one->shortest_distance_from_source > cost_of_edge && !iterator_towns_one->visited)

                            {
                                //perform relaxation of weights
                                iterator_towns_one->shortest_distance_from_source=cost_of_edge;

                                //modify the source of reaching that cost
                                iterator_towns_one->previous_source=town_names_iterator->town_name;
                            }


                    }

                    iterator_towns_one=search_town_recordtable(city_address,town_names_iterator->town_name);
                    iterator_towns_one->visited=true;
                    starting_town="";
                    int smallest_cost_of_remaining_towns=INF;

                    //search table and select the unvisited vertex with minimum cost

                        for (iterator_towns_one=city_address->record_table_towns.begin();iterator_towns_one!=city_address->record_table_towns.end();iterator_towns_one++)


                        {
                            if(!iterator_towns_one->visited && iterator_towns_one->shortest_distance_from_source < smallest_cost_of_remaining_towns)


                            {

                                starting_town=iterator_towns_one->_town_name;
                                smallest_cost_of_remaining_towns=iterator_towns_one->shortest_distance_from_source;
                            }


                        }


                }while(starting_town!=ending_town);


                /*
                 *  backtracking and printing shortest path along with its cost
                 */


                iterator_towns_one=search_town_recordtable(city_address,starting_town);
            cout<<endl;
            cout<<endl;

                cout<<"shortest path from  "<<starting_town_saved<<" to  "<<ending_town_saved<<"  has a cost  wrt "<<wrt<<" "<<iterator_towns_one->shortest_distance_from_source;
                cout<<endl;
            cout<<endl;


                //stack for backtracking:


                stack<string> backtracking_towns;

                auto iterator_towns_two=city_address->record_table_towns.begin();
                    while(iterator_towns_one->_town_name!=starting_town_saved)
                    {
                        string temporary=iterator_towns_one->previous_source;
                        backtracking_towns.push(temporary);
                        iterator_towns_two=search_town_recordtable(city_address,temporary);
                        iterator_towns_one=iterator_towns_two;

                    }
                cout<<endl;
                cout<<endl;
                cout<<"FOLLOW THE SHORTEST PATH "<<endl;
                    while(!backtracking_towns.empty())
                    {
                        cout<<backtracking_towns.top()<<" ---> ";
                        backtracking_towns.pop();
                    }
                    cout<<ending_town_saved<<endl;

            cout<<endl;
            cout<<endl;
                    city_address->record_table_towns.clear();

        }



//only used for testing the code

        void view_towns(T city_for_town)

        {

            CityInfo<T> *city_address=search_city_address(city_for_town);
            list<TownInfo> ::iterator  itr_for_town_names;
            //city_address->itr.
            for (itr_for_town_names=city_address->towns_list.begin(); itr_for_town_names != city_address->towns_list.end(); itr_for_town_names++)
            {
                auto temp=itr_for_town_names->town_connections_list.begin();
                cout << "Town name: " << itr_for_town_names->town_name << endl;
                for (temp=itr_for_town_names->town_connections_list.begin();temp!=itr_for_town_names->town_connections_list.end();temp++)
                {
                    cout<<"town paths: "<<temp->second_town_name<<" with cost "<<temp->cost<<"with distance"<<temp->distance<<" and time "<<temp->time<<endl;
                }

            }

        }


        /*
         *  The implementation for towns inside the cities is completed, now complete the implementation
         *  for cities
         */

        //linked list implementation of graph of cities

        CityInfo<T> *cities_list_headptr;
        CityInfo<T> *cities_list_tailptr;

            Map_Navigation()//constructor


            {

                cities_list_headptr=nullptr;
                cities_list_tailptr=nullptr;

            }

// add a new city to graph

            void add_new_city(T city_Name)


            {



                CityInfo<T> *ptr= search_city_address(city_Name);

                    if(!ptr)
                    {
                        CityInfo<T> *City=new CityInfo<T>;
                        City->next=nullptr;
                        City->city_name=city_Name;

                            if (cities_list_headptr == nullptr)
                            {
                                    cities_list_headptr=City;
                                    cities_list_tailptr=City;
                            }

                            else
                            {
                                    cities_list_tailptr->next=City;
                                    cities_list_tailptr=City;
                            }
                    }
                    else
                        cout<<"City is already existing in data"<<endl;



            }



// adds an edge


            void add_path_between_cities(T c_1, T c_2,int time,int distance)


            {
                    CityInfo<T> *first_city= search_city_address(c_1);
                    CityInfo<T> *second_city= search_city_address(c_2);
                        if(first_city && second_city)
                        {
                                first_city->city_connections.InsertAtEnd(c_2,time,distance);
                                second_city->city_connections.InsertAtEnd(c_1,time, distance);

                        } else
                                cout<<"City does not exist in data."<<endl;


            }



// search address of a city inside the adjaceny list

        CityInfo<T> * search_city_address(T vertex)


        {
                if(cities_list_headptr == nullptr)//if vertex list is empty
                    return nullptr;
                else
                {
                    CityInfo<T> *temp=cities_list_headptr;
                    while(temp)
                    {
                        if (temp->city_name == vertex) {
                            return temp;
                        }
                        else
                            temp=temp->next;
                    }
                    return nullptr;
                }

        }



        int total_cities()

        {
                CityInfo<T> *temp=cities_list_headptr;
                int citices=0;
                    for(temp;temp!=nullptr;temp=temp->next)
                            citices++;
                return citices;

        }




    /*
     *  DIRECT EDGE IS CHECKED WHETHER ONE CITY HAS DIRECT ROAD LINKING
     *  TO OTHER CITY OR NOT OR WE HAVE TO MAKE SOME OTHER PATH.
     */









/*
 *  NOW CHECK IF THERE IS ANY PATH BETWEEN TWO CITIES VIA ANY ROUTE, IF PATH EXIST, ONLY THEN
 *  PROCEED FURTHER WITH SHORTEST PATHS. USED BFS TRAVERSAL TO FIND THIS.
 */


        bool check_if_path_exist_between_two_cities(T first_city, T second_city)


        {

                queue<T> queueobj;
                int size= total_cities();
                visited_cities_list<T> *visitedHead= new visited_cities_list<T>;
                visitedHead->next=nullptr;
                visitedHead->name=cities_list_headptr->city_name;
                visitedHead->visited=false;
                visited_cities_list<T> *temp=visitedHead;
                CityInfo<T> *ptr=cities_list_headptr;
                bool path_exists=false;


                //keep track of path by pushing path in stack and then backtracking

                /*
                 * use a linked list to keep track of all nodes that are visited.
                 * Array can work for integer data type but if nodes name change to string, it
                 * would not be feasible to maintain link between visited nodes and array indices
                 */

                        for(int i=1;i<size;i++)//linked list of vertices and there visited status
                        {
                            ptr=ptr->next;
                            visited_cities_list<T> *newNode= new visited_cities_list<T>;
                            temp->next=newNode;
                            newNode->next=nullptr;
                            newNode->visited=false;
                            newNode->name=ptr->city_name;
                            temp=newNode;
                        }


                 //bfs implementation.
                 //push starting node in queue

                queueobj.emplace(first_city);
                temp=visitedHead;

                    while(true)

                    {
                        if(temp->name == first_city)
                            break;
                        else
                            temp=temp->next;

                    }

                temp->visited=true;

                    while(!queueobj.empty())
                    {
                        T frontElement=queueobj.front();


                            if(frontElement == second_city)

                            {
                                path_exists=true;

                            }


                        /*
                         *  remove front element.
                         *now push all unvisited nodes of front element in queue
                         */

                        queueobj.pop();
                        ptr= search_city_address(frontElement);
                        Node<T> *ptrList=ptr->city_connections.head;

                            while(ptrList!=NULL)


                            {

                                temp=visitedHead;

                                    while(temp!=nullptr)

                                    {

                                        if(temp->name==ptrList->value)
                                            break;
                                        else
                                            temp=temp->next;

                                    }


                                if(!temp->visited)


                                {
                                    queueobj.emplace(temp->name);
                                    temp->visited=true;


                                }

                                else
                                    ptrList=ptrList->link;


                            }
                    }



                visited_cities_list<T> *currentNode=visitedHead;
                visited_cities_list<T> *nextNode= nullptr;


                        while (currentNode!=nullptr)


                        {

                            nextNode=currentNode->next;
                            delete currentNode;
                            currentNode=nextNode;


                        }
                delete currentNode;
                currentNode=nullptr;
                delete nextNode;
                nextNode=nullptr;
                cout<<endl;

                    if(path_exists)

                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }


        }


//used for testing the city network


        void testing()


        {
                    CityInfo<T> *ptr=cities_list_headptr;
                    Node<T> *lptr;

                        while(ptr!=nullptr)


                        {

                            if(ptr->city_connections.head!=NULL)
                                lptr=ptr->city_connections.head;

                            else
                                lptr=NULL;


                           while(lptr!=NULL)


                           {

                               cout<<"SOURCE CITY: "<<ptr->city_name<<" destination city "<<lptr->value<<" cost  "<<lptr->cost<<endl;
                               lptr=lptr->link;

                           }
                           cout<<"City name  "<<ptr->city_name<<endl;
                           ptr=ptr->next;


                        }

        }


        void view_cities()

        {
            CityInfo<T> *ptr=cities_list_headptr;
            while(ptr!=nullptr)


            {
                cout<<"CITY NAME :"<< ptr->city_name<<endl;
                ptr=ptr->next;
            }

        }




/*
 *  Dijkstra algorithm is implemented to find shortest distance of a weighted graph, where
 *  weight of each edge is product of time and distance between edge.
 *
 */


    list<record_table> shortest_path_record;
    list<record_table>::iterator itr=shortest_path_record.begin();
    list<record_table>::iterator itr_search=shortest_path_record.begin();
    list<record_table>::iterator itr_search_two=shortest_path_record.begin();
    CityInfo<T> *ptr;
    record_table obj;

//fill table of cities, helper in shortest path function


        void fill_record_table()


        {

            for (ptr=cities_list_headptr;ptr!=nullptr;ptr=ptr->next)


            {

                obj.shortest_distance_from_source=INF;
                obj._city_name=ptr->city_name;
                obj.previous_source="";
                shortest_path_record.push_back(obj);


            }

        }


//search record table function will give pointer to city node.


        list<record_table>::iterator  search_record_table(string name)


        {

            for (itr=shortest_path_record.begin();itr!=shortest_path_record.end();itr++)
            {
                if(itr->_city_name==name)
                     return itr;
            }


        }




//uses dijkstra algo to find shortest path


        void compute_shortest_path(T source_city, T destination_city, string with_respect_to)


                {


                    fill_record_table();

                    //set distance/time of source vertex as 0

                    itr_search=search_record_table(source_city);
                    itr_search->shortest_distance_from_source=0;


                    /*
                     * some variable and pointers for usage
                     */

                    T source_city_saved=source_city; // I manipulated this data so saved here for later use
                    T destination_city_saved=destination_city;


                    Node<T> *city_name_adjacenylist_ptr;
                    string minimum_cost_city;


                   do

                    {


                        CityInfo<T> *_city_name=search_city_address(source_city);
                            for(city_name_adjacenylist_ptr=_city_name->city_connections.head;city_name_adjacenylist_ptr!=NULL;city_name_adjacenylist_ptr=city_name_adjacenylist_ptr->link)
                            {
                                itr_search=search_record_table(city_name_adjacenylist_ptr->value);
                                itr_search_two=search_record_table(_city_name->city_name);
                                int cost=0;
                                    if(with_respect_to=="time")
                                    {
                                        cost=itr_search_two->shortest_distance_from_source + city_name_adjacenylist_ptr->time;
                                    }
                                    else if(with_respect_to=="distance")
                                    {
                                        cost=itr_search_two->shortest_distance_from_source + city_name_adjacenylist_ptr->distance;
                                    }


                                    if(itr_search->shortest_distance_from_source > cost && !itr_search->visited)

                                    {


                                        itr_search->shortest_distance_from_source=cost;
                                        itr_search->previous_source=_city_name->city_name;

                                    }

                            }
                        itr_search=search_record_table(_city_name->city_name);
                        itr_search->visited=true;
                        source_city="";
                        int smallest_cost_remaining=100000;


                            for (itr=shortest_path_record.begin();itr!=shortest_path_record.end();itr++)


                            {
                                if(!itr->visited && itr->shortest_distance_from_source < smallest_cost_remaining)

                                {

                                    source_city=itr->_city_name;
                                    smallest_cost_remaining=itr->shortest_distance_from_source;

                                }

                            }



                    } while(source_city!=destination_city );

                   /*
                    * backtracking and printing results
                    */


                    itr_search_two=search_record_table(source_city);
                    cout<<endl;
                    cout<<endl;
                    cout<<"Shortest path from  "<<source_city_saved<<" to " <<destination_city_saved<<" wrt  "<<with_respect_to<<" is "<<itr_search_two->shortest_distance_from_source;
                    cout<<endl;
                    cout<<endl;


                    stack<string> backtrack;

                        while(itr_search_two->_city_name!=source_city_saved)

                        {

                                string temp=itr_search_two->previous_source;
                                backtrack.push(temp);
                                itr_search=search_record_table(temp);
                                itr_search_two=itr_search;

                        }
                    cout<<endl;
                    cout<<endl;
                        while(!backtrack.empty())


                        {
                                cout<<backtrack.top()<<" --> ";
                                backtrack.pop();

                        }
                    cout<<destination_city_saved<<endl;
                    cout<<endl;
                    cout<<endl;

                shortest_path_record.clear();



                }



//destructor for destryoing whole network

void CompareCities(T c1, T c2)
{
            CityInfo<T> *temp1=search_city_address(c1);
            CityInfo<T> *temp2=search_city_address(c2);
            bool same=true;

            for (auto itr=temp1->city_connections.head; itr!=temp1->city_connections.end();itr++)
            {
                for (auto itr2=temp2->city_connections.head; itr2!=temp2->city_connections.end();itr2++)
                {
                    if (itr->value==itr2->value)
                    {

                    }
                    else
                    {
                        same=false;
                    }
                }
            }
            if(!same)
            {
                cout<<"diff"<<endl;
            }
}
        ~Map_Navigation()


        {

                CityInfo<T> *currentNode=cities_list_headptr;
                CityInfo<T> *nextNode= nullptr;


                    while (currentNode!= nullptr)

                    {

                        nextNode=currentNode->next;
                        delete currentNode;
                        currentNode=nextNode;


                    }
                delete currentNode;
                currentNode= nullptr;
                delete nextNode;
                nextNode=NULL;
                shortest_path_record.clear();

        }


};



/*
 *  MAIN THREAD FOR PROGRAMM EXECUTION
 */


    int main()
    {


        //system("Color 0B");
        cout<<"____             _         ____  _                             \n"
              "|  _ \\ ___  _   _| |_ ___  |  _ \\| | __ _ _ __  _ __   ___ _ __ \n"
              "| |_) / _ \\| | | | __/ _ \\ | |_) | |/ _` | '_ \\| '_ \\ / _ \\ '__|\n"
              "|  _ < (_) | |_| | ||  __/ |  __/| | (_| | | | | | | |  __/ |   \n"
              "|_| \\_\\___/ \\__,_|\\__\\___| |_|   |_|\\__,_|_| |_|_| |_|\\___|_|\n";


        cout<<"------------------------------------------------------------------------\n";
        cout<<"------------------------------------------------------------------------\n";
        cout<<endl;

        cout<<"             _____ _    _ _____ _____  ______ \n"
              "             / ____| |  | |_   _|  __ \\|  ____|\n"
              "            | |  __| |  | | | | | |  | | |__   \n"
              "            | | |_ | |  | | | | | |  | |  __|  \n"
              "            | |__| | |__| |_| |_| |__| | |____ \n"
              "             \\_____|\\____/|_____|_____/|______|\n";
        cout<<endl;

        cout<<"     -----------------------------------------------\n";
        cout<<"     ------------------------------------------------\n";






        cout<<"add_new_cities                ---> adds a new city to cities data set "<<endl;
        cout<<"add_path_between_cities       ---> add edges between already existing cities "<<endl;
        cout<<"computer_shortest_path        ---> calculates shortest distance between two cities "<<endl;
        cout<<"delete_city                   ---> deletes a city from record "<<endl;
        cout<<"add_new_towns                 ---> adds towns in a city "<<endl;
        cout<<"add_path_between_towns        ---> adds edges in towns of a city "<<endl;
        cout<<"compute_shortest_path_for_towns --> finds shortest path wrt time, distance and overall cost"<<endl;



        cout<<"------------------------------------------------------------------------\n";
        cout<<"------------------------------------------------------------------------\n";
        cout<<"             __                  _   _                 \n"
              "            / _|                | | (_)                \n"
              "           | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ \n"
              "           |  _| | | | '_ \\ / __| __| |/ _ \\| '_ \\/ __|\n"
              "           | | | |_| | | | | (__| |_| | (_) | | | \\__ \\\n"
              "           |_|  \\__,_|_| |_|\\___|\\__|_|\\___/|_| |_|___/"<<endl;
        cout<<"------------------------------------------------------------------------\n";
        cout<<"------------------------------------------------------------------------\n";





        int choice=-1;
        Map_Navigation<string> obj;
        ifstream cities_input;
        bool cities_added=false;
        bool cites_edges_added=false;
        bool towns_added=false;
        bool towns_edges_added=false;



        do

            {

                cout<<" 1--> add new city               2-->load cities data        3---> load cities edges \n"
                      " 4--> add new towns              5--> load towns data         6--->  load town edges  \n "
                      " 7--->shortest path(cities)      8---> shortest path(towns)                           \n"
                      " 9--->view all cities            10---> view all towns(of one city)                   \n"
                      " 11--> add path in cities        0---> quit                                           \n";

                cin>>choice;

                switch (choice)

                {
                    case 1:


                    {

                        string name;
                        cout<<"ENTER CITY NAME "<<endl;
                        cin>>name;
                        obj.add_new_city(name);
                        break;

                    }

                    case 2:

                    {

                        if(!cities_added)

                        {

                          //update path according to your location of file
                            cities_input.open(R"(C:\Users\abc\OneDrive\Desktop\C++\Data-Structures\SemesterProject\cities_data.txt)");
                            if(cities_input.fail())
                            {
                                cerr<<"Unable to open cities data file "<<endl;
                                break;
                            }
                            else

                            {

                                string cityName;
                                    while (cities_input >> cityName)
                                    {
                                        obj.add_new_city(cityName);
                                    }

                                cities_input.close();
                                cities_input.clear();
                                cities_added=true;
                                break;


                            }

                        }
                        else
                        {
                            cerr<<"Cities already added "<<endl;
                            break;
                        }




                    }


                    case 3:


                    {

                        if(!cites_edges_added && cities_added)


                        {


                            cities_input.open(R"(C:\Users\abc\OneDrive\Desktop\C++\Data-Structures\SemesterProject\cities_edges.txt)");
                            if(cities_input.fail())
                            {
                                cerr<<"Unable to open cities edges file "<<endl;
                                break;
                            }
                            else

                            {
                                string first_city,second_city;
                                int time,dist;
                                    while(cities_input>>first_city>>second_city>>time>>dist)
                                    {

                                        obj.add_path_between_cities(first_city,second_city,time,dist);
                                    }

                                cites_edges_added=true;
                                cities_input.close();
                                cities_input.clear();
                                break;

                            }



                        }

                        else

                        {
                            cerr<<" Cities edges already added or missing cities entries "<<endl;
                            break;
                        }


                    }


                    case 4:
                    {
                        string c_name,t;
                        cout<<"Enter city name for town insertion "<<endl;
                        cin>>c_name;

                        cout<<"Enter town name "<<endl;
                        cin>>t;
                        obj.add_new_town(c_name,t);
                        cout<<t<<" added in "<<c_name<<endl;
                        break;


                    }

                    case 5:

                    {


                        if(!towns_added && cities_added)

                        {
                            // load data file for lahore and rawalpindi towns


                                cities_input.open(R"(C:\Users\abc\OneDrive\Desktop\C++\Data-Structures\SemesterProject\lahore_towns.txt)");

                                if(cities_input.fail())
                                {
                                    cerr<<"Unable to open lahore cities data file "<<endl;
                                    break;
                                }

                                else

                                {

                                    string town_n;

                                        while(cities_input>>town_n)
                                        {
                                            obj.add_new_town("lahore",town_n);
                                        }


                                    cities_input.close();
                                    cities_input.clear();
                                }


                                //loading rawalpindi cities data:

                                cities_input.open(R"(C:\Users\abc\OneDrive\Desktop\C++\Data-Structures\SemesterProject\rawalpindi_towns.txt)");

                                if(cities_input.fail())

                                {
                                    cerr<<"Unable to open rawalpindi towns data file "<<endl;
                                    break;
                                }

                                else

                                {
                                    string rwp_towns;

                                        while(cities_input>>rwp_towns)

                                        {
                                            obj.add_new_town("rawalpindi",rwp_towns);
                                        }

                                    towns_added=true;
                                    cities_input.close();
                                    cities_input.clear();
                                    break;


                                }



                        }

                        else
                        {
                            cerr<<"Towns already added or missing cities data "<<endl;
                            break;
                        }


                    }


                    case 6:

                    {

                        if(towns_added )

                        {

                            //load lahore towns edges

                                cities_input.open(R"(C:\Users\abc\OneDrive\Desktop\C++\Data-Structures\SemesterProject\lahore_edges.txt)");


                                if(cities_input.fail())
                                {
                                    cerr<<"Unable to open edges information of lahore towns "<<endl;
                                    break;
                                }

                                else

                                {
                                    string city,first_c,sec_c;
                                    int cost_edge,time,dist;

                                        while(cities_input>>first_c>>sec_c>>cost_edge>>time>>dist)


                                        {

                                            obj.add_new_paths_in_towns("lahore",first_c,sec_c,cost_edge,time,dist);
                                        }

                                    cities_input.close();
                                    cities_input.clear();



                                }

                            //load rawalpindi towns edges:

                                cities_input.open(R"(C:\Users\abc\OneDrive\Desktop\C++\Data-Structures\SemesterProject\rawalpindi_edges.txt)");

                                if(cities_input.fail())

                                {
                                    cerr<<"Unable to open rawalpindi edges data set "<<endl;
                                    break;
                                }

                                else

                                {

                                    string f_towns,e_town;
                                    int cost,time,dist;

                                    while(cities_input>>f_towns>>e_town>>cost>>time>>dist)

                                    {

                                        obj.add_new_paths_in_towns("rawalpindi",f_towns,e_town,cost,time,dist);

                                    }
                                     towns_edges_added=true;
                                    cities_input.close();
                                    cities_input.clear();
                                    break;


                                }


                        }
                        else
                        {
                            cerr<<"Towns not added "<<endl;
                            break;
                        }


                    }

                    case 7:
                    {

                        string s,e,w;
                        cout<<"Enter Starting City"<<endl;
                        cin>>s;
                        cout<<"Enter Destination City "<<endl;
                        cin>>e;

                        //enter "time" wrt time, ""distance"" wrt dist ,, "cost" wrt cost. cost=time*dist;

                        cout<<"Optimal path with respect to time/ distance/ cost "<<endl;
                        cin>>w;
                        bool exists=obj.check_if_path_exist_between_two_cities(s,e);
                        if(exists)
                            obj.compute_shortest_path(s,e,w);
                        else
                            cout<<"Path does not exists between two cities "<<endl;
                        break;

                    }

                    case 8:

                    {


                        string c,s,e,w;
                        cout<<"Enter city name"<<endl;
                        cin>>c;
                        cout<<"Enter starting Town "<<endl;
                        cin>>s;
                        cout<<"Enter destination town "<<endl;
                        cin>>e;

                        //enter "time" wrt time, ""distance"" wrt dist ,, "cost" wrt cost. cost=time*dist;

                        cout<<"Optimal path with respect to time/ distance/ cost "<<endl;
                        cin>>w;

                        obj.computer_shortest_path_for_towns(c,s,e,w);
                        break;

                    }

                    case 9:
                    {
                        obj.view_cities();
                        break;
                    }

                    case 10:

                    {

                        string c;
                        cout<<"Enter city "<<endl;
                        cin>>c;
                        if(c=="lahore" || c=="rawalpindi")
                        {
                            obj.view_towns(c);
                            break;
                        }

                        else
                        {
                            cout<<c<<" town not available yet "<<endl;
                            break;
                        }

                    }

                    case 0:
                    {
                        choice=0;
                        break;
                    }
                    case 11:

                    {
                        string first_city,second_city;
                        int time,dist;

                        cout<<"ENTER CITY NAME "<<endl;
                        cin>>first_city;
                        cout<<"ENTER Second CITY"<<endl;
                        cin>>second_city;
                        cout<<"ENTER TIME AND DISTANCE TAKEN TO TAKE THIS PATH "<<endl;
                        cin>>time;
                        cin>>dist;

                        obj.add_path_between_cities(first_city,second_city,time,dist);
                        break;
                    }

                    default:
                        break;



                }


        }while(choice!=0);


            return 0;
    }

