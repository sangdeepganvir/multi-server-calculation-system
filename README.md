# multi-server-calculation-system

**Problem Statement :**

Design and implement a multi-server calculation system using socket programming. Refer to
the figure below as the basis for your system architecture. Develop a client socket that assigns
tasks involving simple arithmetic equations (e.g., 2 + 4 * 5) to the server cluster.

**Explanation :**

The calculation server (Calc Server) serves as the central server responsible for receiving inputs
from the client. Upon receiving an input equation, the Calc Server should prioritize the
operations based on operator precedence (e.g., multiplication and division before addition and
subtraction). Subsequently, the Calc Server will assign tasks to specialized servers: ADD
Server, MUL Server, SUB Server, and DIV Server, each dedicated to a specific arithmetic
operation.

Tasks should be distributed efficiently among the respective servers in serial manner. Once all
servers complete their assigned tasks, the Calc Server will consolidate the results based on the
original input equation and reply to the client with the final result.
Your implementation should consider effective communication between the client and servers
and proper synchronization to ensure accurate results. Additionally, handle potential errors or
invalid inputs gracefully

![cnl4](https://github.com/sangdeepganvir/multi-server-calculation-system/assets/41817210/9e0caa74-817b-4856-8ea5-46a935980ae7)


**How to Run :**

1) Open 6 terminals for server , mulServer, divServer, subServer, addServer and client.
2) Compile all files by : **g++ -std=c++11 -pthread server.cpp -o server**
3) Order of compiling and running will be : server -> operations servers -> client
4) Done.

**Screenshots :**

![photo_2024-01-31_10-42-40](https://github.com/sangdeepganvir/multi-server-calculation-system/assets/41817210/4d6ac0b7-2f72-47b0-879f-cfdb4940dcb2)

![photo_2024-01-31_10-42-38](https://github.com/sangdeepganvir/multi-server-calculation-system/assets/41817210/2a763478-9451-48f7-ba17-6fe922465f59)

![photo_2024-01-31_10-42-32](https://github.com/sangdeepganvir/multi-server-calculation-system/assets/41817210/0f25d15d-f1ce-48c0-b7a5-c66b4d8293d1)
