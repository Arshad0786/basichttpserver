Required environment : Windows
Make sure to installed Mingw-w64 as compiler and add it into PATH.

This program is implemented with winsock2.h.

Steps:

1. Extract server.c, page1.html and page2.html into locl folder. (make sure they're all in the same folder.)
2. Open up command prompt and change directory to said folder.
3. Run "gcc server.c -o server -lws2_32" to generate server.exe file.
4. Execute server.exe file.
5. Run "ipconfig" on command prompt.
6. Copy your IPv4 address into browser and add ":1024" as port then send. (example:"http://192.168.0.195:1024")

Accepted requests are:
"/", "/index.html", "/page1.html", "/page2.html"
