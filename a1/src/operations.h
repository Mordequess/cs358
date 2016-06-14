#ifndef OPERATIONS
#define OPERATIONS

#define ALLKEYS 'a'
#define ADD_PEER '0'
#define REMOVE_PEER '1'
#define ADD_CONTENT '2'
#define REMOVE_CONTENT '3'
#define LOOKUP_CONTENT '4'

#define MOVE_CONTENT '5'
#define CHANGE_NUMPEERS_CONTENT '6'
#define GET_NUMPEERS_CONTENT '7'
#define CHANGE_NEIGHBOUR_NEIGHBOUR '8'

#endif




//Define new message types that start with unique characters followed by
//sizeof(sockaddr_in) bytes representing who received the god call
//before passing the message on to the next peer, check if the next peer is
//exactly that sockaddr, if it is, go back in reverse order and say can't be found.

//eg, get and change num peers and content are non-god calls