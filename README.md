# commDemo
commDemo is a brief and very rudimentary demo of using QTcpSockets in Qt, and is basically a super watered down netcat in graphical form.

## Status Notes
I have a lot of ideas for how I want to extend it and there's a long list of things that need to be implemented.

- Adding threads (split ui and processing threads)
- Multiple connection support
- File transfer support
- Encryption via QSslSockets
- QML-based interface for styling flexibility
- Multiple tabs (per connection, per connection group, etc)
- Let the user specify settings in a file or otherwise (logging, styling, etc)

On top of these things, there are a few things I'd definitely like to change about the very hacked-up current implementation.

- Singular programming interface for posting items to the QTextBrowser (something to manage formatting)
- Create a base class which Client/Server can inherit to cut down on code duplication
- Modularize the graphical / networking components to be able to add features and test everything easily
- Let the user specify a client timeout so you don't always have to start the server first

## How to build
This should build on any system with Qt 5.x+. The current implementation was built and tested using 5.10.1.

Linux (Debian-based):
I'm most familiar with Debian, and as such, the following makes use of Debian's package manager. Everything after setting up your core build environment (g++, toolchains, etc) should be fairly distribution-agnostic. You *may* need a graphics driver installed with libGL available, as Qt needs this for graphical applications.

Bare minimum development environment prep on your typical Debian-based installation:
- apt install build-essential libgl1-mesa-dev
- get Qt from https://www.qt.io , make sure you install the SDK and not just Qt Creator

Building the app:
- clone the repo to your machine
- open a terminal if you haven't already and cd to the repo dir
- create a build dir to keep the source dir clean
- change to your build dir and run the following
  - \<Qt-install-dir\>/\<Qt-version\>/gcc_64/bin/qmake ..
  - make
  
If all went well you should now have a commDemo executable.

## Usage
You can either run two instances on the same machine or run it across a LAN/WAN (if running across a WAN make sure your firewall isn't blocking the port you want to use). When running, make sure to run one instance as a server first, as this is set up to wait for a connection from a client. If you run the client first, it will not have anything to connect to and will fail to initialize. You can also test it out using netcat (nc -l <port>), but run netcat first.
  
The rest is relatively self-explanatory. Specify a mode, specify a host address to use, and specify a port. When specifying a host address for the server, make sure to use an address your machine actually has (0.0.0.0 for Any, 127.0.0.1 for localhost specifically, or the IPV4/IPV6 address of any of your local ethernet adapters). Also, make sure to use a port that isn't already being listened on for the server.
