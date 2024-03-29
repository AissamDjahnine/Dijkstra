# Dijkstra Metro-Problem- 

Implementation of Dijkstra's algorithm to compute the shortest path on Paris subway network :station: --> :train: --> :station: 

# Related Work 

You can find a python-based implementation for the same database here : https://github.com/IemProg/ParisPathFinder.

## Getting Started & Prerequisites 

**Step #1: Install C/C++ compiler and related tools**

If you are using Fedora, Red Hat, CentOS, or Scientific Linux, use the following yum command to install GNU c/c++ compiler:

           yum groupinstall 'Development Tools'
           
If you are using Debian or Ubuntu Linux, type the following apt-get command to install GNU c/c++ compiler:

           $ sudo apt-get update
           $ sudo apt-get install build-essential manpages-dev

**Step #2: Verify installation**

Type the following command to display the version number and location of the compiler on Linux:

           $ whereis gcc
           $ which gcc
           $ gcc --version
           
## Running 

* Using of compilerflags  :checkered_flag: :

      -Wall -Wextra -Werror -pedantic -pedantic -errors -O3
    
of g++ to garanteeC++ norms in the code implementation, optimizing compiling.

* Using C++ 11 by adding the compilerflag :checkered_flag: : 

      std=c++11 
      
* Dynamic memory using STL containers


## Execution & tests

**Use of the csv files as database :**

In order to execute the code , two csv files  :page_facing_up: are provided as database :

**s.csv : contains all the stations of paris subway** :
       
![alt text ](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/files./stations.jpg)
       
**c.csv : contains all the connections between the subway stations ( transfer time in seconds )**
       
![alt text ](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/files./connections.jpg)


**Use the makefile to compile the cpp files :**

Example :

![alt text](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/files./makefile.jpg)


You can check the [makefile](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/makefile)  :ballot_box_with_check:

**Execute the program :**

There are two ways to excute the Network program  : 

**1/ Using Stations Ids :** 

You can find the :id: of the stations in : **s.csv** file 

        ./Network s.csv c.csv Start_id End_id        
**Example :** 

         ./Network s.csv c.csv 1722 2062
**Output :**

![alt text ](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/files./SaintLazare_Bastille.jpg)

**2/ Using Stations Names :**

You can find the names :abcd: of stations in : **s.csv** file

Note : the code is resistant to spelling errors :warning: ( Using the levenshtein Distance - Check the code -)


        ./Network s.csv c.csv Start_name End_name
        
**Example :**

        ./Network s.csv c.csv Bastille Jussieu        

**Output :**

![alt text](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/files./BastilleJussieu.jpg)

## Authors

* **Djahnine Aissam**  - [Profile](https://github.com/AissamDjahnine)
