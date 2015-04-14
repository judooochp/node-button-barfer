#include <stdint.h>        //  Needed for other Libs
#include <TFTv2.h>         //  Seeed TFT TouchScreen Lib,
                           //  modified to include extra colors.
#include <SPI.h>           //  Included in u/electronicchicken's original
#include <EthernetV2_0.h>  //  Seeed Ethernet Shield Lib

// Your MAC address; do not change
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
// Your IP configuration
IPAddress ip(10,0,0,13);          //  IP of the card(s)
IPAddress gateway(10,0,0,1);      //  IP of router
IPAddress subnet(255,255,255,0);  

// Server info
IPAddress server(10,0,0,3);       //  IP of connected PC
int port = 8000;                  //  Node's "Listening on ####:* ..."

// Labels which map to values (below)
char names[10][13] = {
  "Pressers:",
  "/r/thebutton",
  "Greys:",
  "Purps:",
  "Blues:", 
  "Greens:",
  "Yellows:",
  "Oranges:",
  "Reds:",
  "n00bs:"
};

// This will be populated with data from the server
char values[10][10] = {
  "0,000,000", // Presses
  "00", // Timer
  "0000", // Greys
  "0000", // Purps
  "0000", // Blues
  "0000", // Greens
  "0000", // Yellows
  "0000", // Oranges
  "0000", // Reds
  "0000" // Noobs
};

// Ethernet (W5200) and SD pin assignments. Don't let these
// pins conflict with the TFT shield's pins.
#define W5200_CS  10
#define SDCARD_CS  4

// THE TFT HAS A VERY SLOW REFRESH RATE. TO ACCOMODATE A LITTLE,
// WE ARE GOING TO COVER UP ONLY THE TEXT THAT NEEDS UPDATED BY
// USING A SEPARATE FUNCTION PER SECTION.

// Update the counter
void cycleCount() {
// serve() returns CONST CHAR [2]; Switch needs INT or LONG INT
  long b = atol(values[1]);
// Blank the counter, or it'll pile on the counts
  Tft.fillRectangle(85,20,70,50,BLACK);
// Detect the counter's value and draw it in the appropriate color
  switch (b) {
    case 60:
    case 59:
    case 58:
    case 57:
    case 56:
    case 55:
    case 54:
    case 53:
    case 52:
// PURPLE is not defined in the original TFTv2 Library.
// 0x8808
      Tft.drawString(values[1],80,25,6,PURPLE);
      break;
    case 51:
    case 50:
    case 49:
    case 48:
    case 47:
    case 46:
    case 45:
    case 44:
    case 43:
    case 42:
      Tft.drawString(values[1],80,25,6,BLUE);
      break;
    case 41:
    case 40:
    case 39:
    case 38:
    case 37:
    case 36:
    case 35:
    case 34:
    case 33:
    case 32:
      Tft.drawString(values[1],80,25,6,GREEN);
      break;
    case 31:
    case 30:
    case 29:
    case 28:
    case 27:
    case 26:
    case 25:
    case 24:
    case 23:
    case 22:
      Tft.drawString(values[1],80,25,6,YELLOW);
      break;
    case 21:
    case 20:
    case 19:
    case 18:
    case 17:
    case 16:
    case 15:
    case 14:
    case 13:
    case 12:
// ORANGE is not defined in the original TFTv2 Library.
// 0xfb00
      Tft.drawString(values[1],80,25,6,ORANGE);
      break;
    case 11:
    case 10:
    case 9:
    case 8:
    case 7:
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
      Tft.drawString(values[1],80,25,6,RED);
      break;
    case 1:
      Tft.fillScreen();
// I haven't checked to see what this looks like.
      Tft.drawString("1",20,25,15,WHITE);
      break;
    default:
      Tft.drawString(values[1],80,25,6,WHITE);
  }
}

// Update the "Pressers"
void cycleParts() {
// Blank the counter
  Tft.fillRectangle(115,299,110,40,BLACK);
// Draw that shit, updated
  Tft.drawString(values[0],120,300,2,WHITE); 
}

// Refresh the whole display
void cycleDisplay() {
// Blank the whole screen. In future versions,
// I would blank just the counters, but meh; when the
// whole screen goes black, it kind of calls attention; 
// I like that, kinda.
  Tft.fillScreen();
// Draw the titles and counts creatively
  for(int i = 2; i < 10; i++) {
// Draw 'participants' (pressers) label and count
    Tft.drawString(names[0],1,300,2,WHITE);
    Tft.drawString(values[0],120,300,2,WHITE);
// Draw the timer label and value
    Tft.drawString(names[1],1,1,2,GRAY2);
    Tft.drawString(values[1],80,25,6,WHITE);
// Initiate color variable as WHITE to start with.
    int color;
// Display the 'active users' data
    switch (i) {
      case 2:
        color = GRAY1;
        break;
      case 3:
        color = PURPLE;
        break;
      case 4:
        color = BLUE;
        break;
      case 5:
        color = GREEN;
        break;
      case 6:
        color = YELLOW;
        break;
      case 7:
        color = ORANGE;
        break;
      case 8:
        color = RED;
        break;
      case 9:
// PALE is not defined in the original TFTv2 Library.
// 0xc618
        color = PALE;
        break;
    }
// Space and color that shit automatically. 
      Tft.drawString(names[i],1,(i*24+40),2,color); 
      Tft.drawString(values[i],135,(i*24+40),2,color);
  }
}

// Start the Ethernet code.
EthernetClient client;

// Self-explanatory?
void setup() {
// turn off the SD card reader. HIGH is off, for some reason.
  pinMode(SDCARD_CS,OUTPUT);
  digitalWrite(SDCARD_CS,HIGH);
  
// Start the screen.
  Tft.TFTinit();
// Turn the backlight on.
  TFT_BL_ON;
  
// In later versions, I would like to use the comic this
// references. For now, though, this is an unnecessary joke:
  Tft.drawString("I made this.",0,120,3,ORANGE);
// Display it for a whole second (1000 milliseconds)
  delay(1000);
  
// Display everything
  cycleDisplay();
   
// Configure our internets
  Ethernet.begin(mac, ip, gateway, subnet);

// give the Ethernet shield a second to initialize:
  delay(1000);
// Now we'll connect to the node-button-barfer server
  client.connect(server, port);
// An extra delay here prevents several frames of "00", etc.
  delay(1000);
}

// Rather than get the info once every second, we'll have to
// refresh the info more often. This facilitates that.
void serve() {
  
  /*  We will naively assume that if any data is available from
   the server, that a complete set is waiting to be read.
   This is not great, but so far it hasn't borked on me.  */
  if(client.available()) {

// Read the participants count
    for(int n = 0; n < 10; n++) {
      char c = client.read();
      // Stop at a newline
      if(c == '\n') {
        // Overwrite any remaining chars
        while(n < 10) {
          values[0][n] = 32;
          n++;
        }
        break;
      }
      // If we're still here, append the char to this value
      values[0][n] = c;
    }
    
    // Read the timer value
    for(int n = 0; n < 2; n++) {
      char c = client.read();
      if(c == '\n') {
        while(n < 2) {
          values[1][n] = 32;
          n++;
        }
        break;
      }
      values[1][n] = c;
    }
    
// Read and dispense with the space behind the timer
      char c = client.read();
      if(c == '\n') {
    // Do nothing with that extra space.
        }
    
    // Read each line of 'active user' data
    for(int i = 2; i < 10; i++) {
      for(int n = 0; n < 5; n++) {
        char c = client.read();
        if(c == '\n') {
          while(n < 5) {
            values[i][n] = 32;
            n++;
          }
          break;
        }
        values[i][n] = c;
      }
    }
    
  }
}

// Over and over and over and ...
void loop() {

// Get the info
  serve();
  
// Redraw the counters with the most recent info
  cycleDisplay();

// Every 50 seconds, 
  for (int o = 1; o < 1000; o++) {
    serve();
// Refresh the counter
    cycleCount();
// Every 1/20th of a second
    delay(50);
// And then the Participants counter, as doing them
// at the same time would make the two parts of the
// screen glaringly obviously blinking
    cycleParts();  
}
    

  }

