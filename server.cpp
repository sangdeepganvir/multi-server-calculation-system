#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
using namespace std;

#define int long long
#define pb push_back
#define ppb pop_back
#define nl '\n'
#define fi first
#define se second
#define f(n) for (int i = 0; i < n; i++)

int server, subServer, mulServer, divServer;

int performOperation(string operation, int val1, int val2)
{
    int port;
    if (operation == "add")
    {
        port = 9999;
    }
    else if (operation == "sub")
    {
        port = 8889;
    }
    else if (operation == "mul")
    {
        port = 7777;
    }
    else
    {
        port = 6666;
    }
    int tempServer = socket(AF_INET, SOCK_STREAM, 0);
    if (tempServer < 0)
    {
        cerr << "Error creating addition client socket" << endl;
        return LLONG_MAX;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    int err = connect(tempServer, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (err < 0)
    {
        cerr << "Error connecting to server" << endl;
        close(server);
        return LLONG_MAX;
    }
    string dataToSend = to_string(val2) + " " + to_string(val1);
    send(tempServer, dataToSend.c_str(), dataToSend.size(), 0);

    // Receive and print the result
    stringstream resultStream;
    char buffer[256];
    int sz = recv(tempServer, buffer, 256, 0);
    string result = buffer;
    result.substr(0, sz);
    return stoll(result);
}

int prec(char c)
{
    if (c == '/' || c == '*')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

int evaluatePostfix(vector<string> &exp)
{
    stack<int> st;
    for (int i = 0; i < exp.size(); ++i)
    {
        string s = exp[i];
        if (!(s == "*" || s == "/" || s == "+" || s == "-" || s == "(" || s == ")"))
            st.push(stoll(s));

        else
        {
            if (st.size() < 2)
                return LLONG_MIN; // Use a more appropriate value for representing an error
            int val1 = st.top();
            st.pop();
            int val2 = st.top();
            st.pop();
            switch (exp[i][0])
            {
                int res;
            case '+':
                res = performOperation("add", val1, val2);
                st.push(res);
                // st.push(val2 + val1);
                break;
            case '-':
                res = performOperation("sub", val1, val2);
                st.push(res);
                // st.push(val2 - val1);
                break;
            case '*':
                res = performOperation("mul", val1, val2);
                st.push(res);
                // st.push(val2 * val1);
                break;
            case '/':
                if (val1 == 0)
                    return LLONG_MIN; // Division by zero error
                res = performOperation("div", val1, val2);
                st.push(res);
                // st.push(val2 / val1);
                break;
            default:
                if (res == LLONG_MAX)
                {
                    return LLONG_MAX;
                }
            }
        }
    }
    if (st.size() == 1)
        return st.top();
    return LLONG_MIN; // Indicate an error
}

string infixToPostfixAndEvaluate(vector<string> &vs)
{
    stack<string> st;
    vector<string> result;

    for (int i = 0; i < vs.size(); i++)
    {
        string s = vs[i];

        if (!(s == "*" || s == "/" || s == "+" || s == "-" || s == "(" || s == ")"))
            result.pb(s);

        else if (s == "(")
            st.push(s);

        else if (s == ")")
        {
            while (!st.empty() && st.top() != "(")
            {
                result.push_back(st.top());
                st.pop();
            }
            if (st.empty())
                return "Invalid Expression"; // Mismatched parentheses
            st.pop();                        // Pop '('
        }

        else
        {
            while (!st.empty() && prec(s[0]) <= prec(st.top()[0]))
            {
                result.push_back(st.top());
                st.pop();
            }
            st.push(s);
        }
    }

    while (!st.empty())
    {
        result.push_back(st.top());
        st.pop();
    }

    int res = evaluatePostfix(result);
    if (res == LLONG_MIN)
        return "Invalid Expression.";
    else if (res == LLONG_MAX)
    {
        return "Server isn't working properly.";
    }
    return to_string(res);
}

void evaluateAndSend(int client, string s)
{
    string cur;
    vector<string> vs;
    string reply;
    f(s.size())
    {
        if (isdigit(s[i]))
        {
            cur.pb(s[i]);
        }
        else if (s[i] == '*' || s[i] == '/' || s[i] == '+' || s[i] == '-' || s[i] == '(' || s[i] == ')')
        {
            if (cur.size() > 0)
                vs.pb(cur);
            cur.clear();
            vs.pb(string(1, s[i]));
        }
        else if (s[i] != ' ')
        {
            reply = "Invalid Expression";
            break;
        }
    }
    if (cur.size() > 0)
        vs.pb(cur);
    cur.clear();
    if (reply.empty())
    {
        if (s == "exit")
        {
            reply = "Connection closed by client.";
            send(client, reply.c_str(), reply.size(), 0);
            close(client);
            return;
        }
        else
        {
            reply = infixToPostfixAndEvaluate(vs);
        }
    }
    send(client, reply.c_str(), reply.size(), 0);
}

void run(int client)
{
    while (1)
    {
        char buff[256];
        int sz = recv(client, buff, 256, 0);
        if (sz <= 0)
        {
            break; // Handle connection closure or error
        }
        string req = buff;
        req = req.substr(0, sz);
        cout << "Client" << client << ": " << req << '\n';
        evaluateAndSend(client, req);
    }
    close(client);
}

signed main()
{
    int port = 8888;
    struct sockaddr_in address;
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(server >= 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
    int err = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    assert(err >= 0);

    err = ::bind(server, (struct sockaddr *)&address, sizeof(address));
    assert(err >= 0);

    err = listen(server, 5);
    assert(err >= 0);
    cout << "Listening to client at port 8888" << endl;
    vector<thread> threads;

    while (1)
    {
        int client = accept(server, nullptr, nullptr);
        if (client < 0)
        {
            cout << "Could not connect" << endl;
            continue;
        }
        cout << "client" << client << " connected..." << '\n';
        threads.push_back(thread(run, client));
    }

    for (auto &th : threads)
    {
        th.join();
    }
    close(server);
    return 0;
}
