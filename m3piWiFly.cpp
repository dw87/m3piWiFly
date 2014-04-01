#include "mbed.h"
#include "m3pi.h"

#define BUFFER_SIZE 256
#define ERROR_CHAR = -1

m3pi m3pi;

// Access the mbed's onboard LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Access the m3pi's onboard LEDs
DigitalOut m3pi_led1(p20);
DigitalOut m3pi_led2(p19);
DigitalOut m3pi_led3(p18);
DigitalOut m3pi_led4(p17);
DigitalOut m3pi_led5(p16);
DigitalOut m3pi_led6(p15);
DigitalOut m3pi_led7(p14);
DigitalOut m3pi_led8(p13);

 // Access the m3pi's configurable button to use as a reset command
InterruptIn m3pi_button(p21);
DigitalOut xbee_reset_pin(p26);

// Create the serial port on mbed pins 27 & 28 to talk to the Xbee
Serial xbeeport(p28,p27); //UART2 TX P28, RX P27

void rx_interrupt();
void read_line();

char rx_buffer[BUFFER_SIZE] = {0};
char rx_line[80];
volatile int rx_in = 0;
volatile int rx_out = 0;

void robot_parse();

Timer sysClock;
Ticker ticker;

void init();
void tick();
void reset_xbee();

int main() {
    init();

    wait(1.0);

    while(1){
        read_line();
        robot_parse();        
    }
}

void init(){
    m3pi_button.mode(PullUp);
    
    xbeeport.baud(9600);        //Configure the Xbee serial port to 9600 baud.
    xbeeport.attach(&rx_interrupt, Serial::RxIrq);  //Attach the function to the Serial Receiver Interrupt
    
    xbee_reset_pin = 1;
    
    m3pi_button.fall(&reset_xbee);

    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
    
    m3pi_led1 = 0;
    m3pi_led2 = 0;
    m3pi_led3 = 0;
    m3pi_led4 = 0;
    m3pi_led5 = 0;
    m3pi_led6 = 0;
    m3pi_led7 = 0;
    m3pi_led8 = 0;    
    
    m3pi.locate(0,0);
    m3pi.printf("mbedRNXV");
    m3pi.locate(0,1);
    m3pi.printf("        ");
    
    sysClock.reset();
    sysClock.start();
          
    ticker.attach(&tick, 0.5);  //Create a 2Hz tick function to blink a status LED.
    
    sysClock.stop();
    srand (sysClock.read_us());
}

void tick(){
    led1 = !led1; //Blink an LED to confirm program running.
    m3pi_led1 = rand()%2;
    m3pi_led2 = rand()%2;
    m3pi_led3 = rand()%2;
    m3pi_led4 = rand()%2;
    m3pi_led5 = rand()%2;
    m3pi_led6 = rand()%2;
    m3pi_led7 = rand()%2;
    m3pi_led8 = rand()%2;
}

void robot_parse(){
    int rx_x = 0;
    int rx_y = 0;
    char output [8];
    sscanf(rx_line,"$RP,X,%d,Y,%d;", &rx_x, &rx_y);
    sprintf(output, "%d,%d", rx_x, rx_y);
    m3pi.locate(0,1);
    m3pi.printf("        ");
    m3pi.locate(0,1);
    m3pi.printf(output);

    led4 = !led4;
    //Parse the data received and offer simple straight line forwards/backwards and
    //on the spot rotation/spinning of the m3pi.
    //This will expand to offer better control (e.g. curved paths) in the future.  
    if (rx_x == 0 && rx_y == 0){
        m3pi.stop();   
    }
    else if (rx_x > -15 && rx_x < 15){
        if (rx_y > 0){
            m3pi.forward(((float)rx_y/100));    
        }
        else if (rx_y < 0) {
            m3pi.backward(((float)rx_y/-100));    
        }
    }
    else if (rx_y > -15 && rx_y < 15){
        if (rx_x < 0) {
            m3pi.left(((float)rx_x/-100));    
        }
        else if (rx_x > 0){
            m3pi.right(((float)rx_x/100));    
        }
    }
    else {
        m3pi.stop();    
    }
}

void reset_xbee() {
    m3pi.locate(0,1);
    m3pi.printf(" RESET  ");
    xbee_reset_pin = 0;
    wait(2.00);
    xbee_reset_pin = 1;
    m3pi.locate(0,1);
    m3pi.printf(" DONE   ");
}

// Read a line from the large rx buffer from rx interrupt routine
void read_line() {
    int i;
    i = 0;
    // Start Critical Section - don't interrupt while changing global buffer variables
    NVIC_DisableIRQ(UART2_IRQn);
    // Loop reading rx buffer characters until end of line character
    led3 = 1;
    while ((i==0) || (rx_line[i-1] != '\n')) {
        // Wait if buffer empty
        if (rx_in == rx_out) {
            // End Critical Section - need to allow rx interrupt to get new characters for buffer
            NVIC_EnableIRQ(UART2_IRQn);
            while (rx_in == rx_out) {
            }
            // Start Critical Section - don't interrupt while changing global buffer variables
            NVIC_DisableIRQ(UART2_IRQn);
        }
        rx_line[i] = rx_buffer[rx_out];
        i++;
        rx_out = (rx_out + 1) % BUFFER_SIZE;
    }
    led3 = 0;
    // End Critical Section
    NVIC_EnableIRQ(UART2_IRQn);
    rx_line[i-1] = 0;
    return;
}

void rx_interrupt(){
    led2 = 1;
    while (xbeeport.readable() && (((rx_in + 1) % BUFFER_SIZE) != rx_out)){
        rx_buffer[rx_in] = xbeeport.getc();
        rx_in = (rx_in + 1) % BUFFER_SIZE;
    }
    led2 = 0;
    return;    
}