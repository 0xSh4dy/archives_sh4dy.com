### File Transfer Protocol (FTP)
FTP is an application layer protocol that can be used to move files between local and remote systems. It runs on the top of TCP. It uses two connections namely control connection and data connection to transfer a file.  Control connection is used for sending control information such as password, commands, etc. The control connection runs on port 21. The data connection is used to perform the actual file transfer. It is initiated on port 20. When an FTP connection is started, the clients sends control information to the server. The server verifies it and initiates a data connection. The control conneciton remains active throughout the session. Max file size can be 2GB in FTP. The data sent via control and data connection is not encrypted.

### Telnet
Telnet stands for teletype network. It is a client/server application protocol that provides access to virtual terminals of remote systems. It consists of two components: the protocol and the software application that provides the service. The default port for telnet is 23 and operates at the application layer. When a telnet connection is first established, each end is assumed to originate and terminate at a `Network Virtual Terminal` or NVT. An NVT is a virtual device which provides a standard representation of the terminal.

References
https://www.geeksforgeeks.org/file-transfer-protocol-ftp-in-application-layer/
https://en.wikipedia.org/wiki/Telnet