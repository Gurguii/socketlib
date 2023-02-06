#include <gsocket/templates>
using namespace gsocket;
int main(){
    /* THIS IS BASICLY THE SAME BUT WITH POINTERS
    AND DIFFERENT SYNTAX HeHe*/
    /* THIS IS THE CLIENT PART */
    Socket s(inet,tcp,BLOCK);
    templates::connect(&s,"127.0.0.1", 8080);
    templates::send(&s,"hellooo w templates");
    s.close();
    /* THIS IS THE SERVER PART */
    Socket server(inet,tcp,BLOCK);
    templates::bind(&server,"127.0.0.1",8080);
    templates::listen(&server);
    Address _client;
    /*Address is not required if we don't want to retrieve incoming host port*/
    auto client = templates::acceptConnection(&server,&_client);
    std::cout << "CONN FROM " << _client.host << " : " << _client.port << "\n";
    std::string buff(1024,'\x00');
    /* Givin default size is not required, but might improve performance
    since it won't have to resize that often(depends on input size though)*/
    templates::awaitData(&client,&buff);
    std::cout << "received: " << buff << "\n";
    client.close();
    server.close();
}