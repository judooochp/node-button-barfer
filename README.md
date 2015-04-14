# node-button-barfer
A node.js server that barfs /r/thebutton data to clients. (For an Arduino project of echicken, forked by judooochp.)

### Hardware

Arduino UNO R3

Seeed TFT Touch Shield

Seeed Ethernet Shield

### Software

echicken set out to do everything natively on the Arduino, but after a couple of frustrating hours of messing with various websocket and JSON parsing libraries, they decided to go another route (wanted to have something working before the timer runs out.) 

The Arduino connects to a custom server and listens for data.  The server barfs out ten lines of data at a time, which are:

- Participants (how many people have pressed the button)
- The current timer value (seconds remaining)
- Non-pressers currently active in the subreddit
- Purple pressers currently active in the subreddit
- Blue pressers currently active in the subreddit
- Green pressers currently active in the subreddit
- Yellow pressers currently active in the subreddit
- Orange pressers currently active in the subreddit
- Red pressers currently active in the subreddit
- Can't-pressers currently active in the subreddit

The Arduino reads these ten lines, then updates its display.  This happens roughly twenty times per second, so updating the timer takes care of itself.

The *button.ino* file contains the Arduino sketch, and depends on [TFTv2](https://github.com/Seeed-Studio/TFT_Touch_Shield_V2) and [EthernetV2_0](https://github.com/Seeed-Studio/Ethernet_Shield_W5200).

The server component was written for node.js by echicken.  It maintains a connection to the button's websocket service and receives updates from there.  When an update is received, the server parses the data and then spits out the lines described above to all connected clients.

The 'active user' data is not included in the websocket feed, so at a (configurable) interval the server reloads the [/r/thebutton](http://www.reddit.com/r/thebutton/) front page and then parses that info out of the sidebar.  Variables are updated and included in the next broadcast to clients.  This HTML-scraping method is a bit slow, so it happens in a background process thanks to [backgrounder](http://jolira.github.io/backgrounder/).  The HTML parsing is done via [jsdom](https://github.com/tmpvar/jsdom/tree/3.x) and [jQuery](https://jquery.com/).

### Server Installation

echicken only tested this on Ubuntu 14.04, with rconsole, but this version works on Windows 8.1 without rconsole.

Assuming you've got [node.js](https://nodejs.org/), [npm](https://www.npmjs.com/), and [git](http://git-scm.com/downloads) installed, and assuming judooochp didn't completely mess up, the following should do the trick:

```sh
git clone https://github.com/judooochp/node-button-barfer.git
cd node-button-barfer
npm install
```

Running the server is just a matter of:
```sh
node index.js
```

### Client (Arduino) Installation

As previously mentioned, *button.ino* is the Arduino sketch.  You'll need to make a few changes to this file to suit your configuration.  There are some configuration variables on lines 11-18 that you will need to modify to set your IP address and the address of the server to connect to.

The calls to *drawString()* in the *cycleDisplay()* function specify ( "String to display", x, y, fontSize, COLOR ). You may need to adjust the locations of stuff and things in order to fit them on your screen, and possibly drop certain items altogether.
