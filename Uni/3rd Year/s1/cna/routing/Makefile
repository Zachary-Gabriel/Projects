CXX=g++
CPPFLAGS= -std=c++11 
FILES=configFileName changedConfigName
SRCS1=DistanceVector.cpp
SRCS2=PoisonedReverse.cpp

all: cl
all: DistanceVector PoisonedReverse

DistanceVector:
	$(CXX) $(CPPFLAGS) -o DistanceVector $(SRCS1)

PoisonedReverse:
	$(CXX) $(CPPFLAGS) -o PoisonedReverse $(SRCS2)
cl:
	rm DistanceVector PoisonedReverse