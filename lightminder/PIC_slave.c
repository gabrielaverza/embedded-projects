#define BUZZ PORTC.RC1
#define LED PORTB.RB1
#define BT PORTB.RB0

// LCD pin configuration
sbit LCD_RS at RE2_bit;
sbit LCD_EN at RE1_bit;
sbit LCD_D7 at RD7_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D4 at RD4_bit;

// LCD pin direction configuration
sbit LCD_RS_Direction at TRISE2_bit;
sbit LCD_EN_Direction at TRISE1_bit;
sbit LCD_D7_Direction at TRISD7_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD4_bit;

typedef enum {
    ON,
    OFF,
    IDLE
} estados;

estados estado = IDLE;

unsigned int ucStatus_inc;
unsigned int uiValorAD;
unsigned int cont;
char valor_string[8];

void main() {
    // I/O configuration
    TRISB = 0;         // PORTB as output (LED, etc.)
    TRISC.RC1 = 0;     // BUZZER pin as output
    TRISB.RB0 = 1;     // Button pin as input

    LED = 0;
    BUZZ = 0;
    ucStatus_inc = 0;

    // ADC configuration
    PORTA = 255;
    TRISA = 255;          // PORTA as input

    ADCON0 = 0b00000001;  // Enable ADC, Channel 0
    ADCON1 = 0b11001110;  // AN0 as analog, others digital
    ADCON2 = 0b10111110;  // Right justify, Fosc/64

    // PWM setup - for cooler
    PWM1_Init(5000);     // 5kHz PWM
    PWM1_Start();

    // LCD setup
    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Cmd(_LCD_CLEAR);

    // UART setup
    UART1_Init(9600);    // Baud rate 9600

    while(1) {
        // Check UART for command
        if (UART1_Data_Ready()) {
            char caracter = UART1_Read();
            estado = caracter == 'A' ? ON : OFF;
        }

        switch(estado) {
            case ON:
                uiValorAD = ADC_Read(0);         // Read LDR value
                uiValorAD *= 0.24;               // Scale to duty cycle
                PWM1_Set_Duty(uiValorAD);        // Set PWM - for cooler

                LED = 1;
                BUZZ = 1;
                lcd_out(1, 1, "ON");

                delay_ms(1000);
                cont++;
                IntToStr(cont, valor_string);
                lcd_out(2, 1, valor_string);

                if (cont == 10) {
                    estado = OFF;
                }

                // Button pressed
                if ((BT == 0) && (ucStatus_inc == 0)) {
                    ucStatus_inc = 1;
                    estado = OFF;
                }

                // Button released
                if ((BT == 1) && (ucStatus_inc == 1)) {
                    ucStatus_inc = 0;
                }

                delay_ms(100);
                break;

            case OFF:
                PWM1_Set_Duty(0);
                LED = 0;
                BUZZ = 0;
                Lcd_Cmd(_LCD_CLEAR);
                lcd_out(1, 1, "OFF");
                UART1_Write('R');      // Send return signal to Arduino
                estado = IDLE;
                delay_ms(3000);
                break;

            case IDLE:
                lcd_out(1, 1, "IDLE");
                cont = 0;
                break;
        }
    }
}