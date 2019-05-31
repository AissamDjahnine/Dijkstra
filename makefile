#make file overview :
CXX=g++
CXXFLAGS= -Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -std=c++11
Network: Network.cpp
	$(CXX) $(CXXFLAGS) Network.cpp -o Network
clean:
	rm Network
