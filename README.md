# Metro-Problem-

 Implementation of Dijkstra's algorithm to compute the shortest path on Paris subway network.

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
## Running & tests :
**use of the csv files as database :**

In order to execute the code , two csv files are provided as database : 
       **s.csv : contains all the stations of paris subway**
       ![alt text ](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/stations.jpg)
       **c.csv : contains all the connections between the subway stations ( transfer time in seconds )**
       ![alt text ](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/connections.jpg)
     
**Use the makefile to compile the cpp files :**

Example :

![alt text](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/makefile.jpg)

You can check the [makefile](https://github.com/AissamDjahnine/Metro-Problem-/blob/master/makefile) 

**Execute the program :**
You can find the Ids of the stations in : **s.csv** file

        ./Network s.csv c.csv Start_id End_id
        
**Example :** 


## Authors

* **Djahnine Aissam**  - [Profile](https://github.com/AissamDjahnine)

## Acknowledgments

* Part of this job was supported by : Vincent Fraillon-maison


