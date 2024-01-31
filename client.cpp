#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

bool connected = false; 

void post(int client) {
    while(connected) {
        string s; 
        getline(cin, s); 
        cout<<"User: "<<s<<'\n';
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        send(client, s.c_str(), s.size(), 0); 
    }
}

void recieve(int client) {
    while(connected) {
        char buff[256]; 
        int sz = recv(client, buff, 256, 0); 
        if(sz <= 0) {
            cout << "Server closed the connection." << endl; 
            connected=false;
            close(client);  
            break; 
        }
        string res = buff; 
        res = res.substr(0, sz); 
        cout<<"Server: "<<res<<'\n';
    }
}
 
int main(){
    sockaddr_in srv;
	int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
	srv.sin_family= AF_INET;
	srv.sin_port= htons(8888);
	srv.sin_addr.s_addr= inet_addr("127.0.0.1");
 
	int err= connect(client,(sockaddr*)&srv,sizeof(srv));
    assert(err>=0); 
    connected=true;
    thread t1(recieve, client); 
    thread t2(post, client); 
    t1.join(); 
    t2.join(); 
    close(client); 
	return 0;
}
