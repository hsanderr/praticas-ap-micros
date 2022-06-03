// Configuração da pinagem do LCD
sbit LCD_RS at RD2_bit; // Pino 2 do PORTD = RS do LCD
sbit LCD_EN at RD3_bit; // Pino 3 do PORTD = EN do LCD
sbit LCD_D7 at RD7_bit; // Pino 7 do PORTD = D7 do LCD
sbit LCD_D6 at RD6_bit; // Pino 6 do PORTD = D6 do LCD
sbit LCD_D5 at RD5_bit; // Pino 5 do PORTD = D5 do LCD
sbit LCD_D4 at RD4_bit; // Pino 4 do PORTD = D4 do LCD

// Direção do fluxo de dados dos pinos do PORTD
sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D7_Direction at TRISD7_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD4_bit;

// Botões de entrada para alterar o duty cycle
sbit velocidade1 at RA1_bit; // Duty cycle = 0%
sbit velocidade2 at RA2_bit; // Duty cycle = 25%
sbit velocidade3 at RA3_bit; // Duty cycle = 50%
sbit velocidade4 at RA4_bit; // Duty cycle = 75%
sbit velocidade5 at RA5_bit; // Duty cycle = 100%

// Configura o pino 0 do PORTC para receber informação do sensor infravermelho
sbit sensor at RC0_bit;

// Variável que guarda o estado anterior do sensor
bit sensor_anterior;

// Variável para contar os pulsos do sensor infravermelho
unsigned int cont;

// Variável para armazenar o númeo de voltas da ventoinha
unsigned int voltas;

// Variável que receberá o número de voltas em forma de string
char voltas_string [8];

// Bit para indicar ao main que houve interrupção
bit aux;

// Rotina de interrupção
void interrupt_high() iv 0x0008 ics ICS_AUTO {
     //TMR0IE_bit = 1;                        // Habilita interrupção do Timer0
     T0CON.TMR0ON = 0;                      // Para Timer0
     INTCON.TMR0IF = 0;                     // Zera flag de overflow do Timer0
     TMR0H = 0x85;                          // Recarrega TMR0H
     TMR0L = 0xEE;                          // Recarrega TMR0L
     // RC1_bit = !RC1_bit;
     
     /* A variável "cont", recebe o número de vezes que o sensor troca
     de estado, portanto deve-se dividir por 2 para se conseguir o número
     de pulsos. Depois, divide-se por 7 pois a ventoinha possui 7 pás,
     obtendo-se o número de voltas.*/
     voltas = cont / 14;
     
     /* Arredonda cont / 14 para o próximo inteiro caso o restante de cont / 14
     seja maior que 14 / 2 */
     if (cont % 14 > 14 / 2) {
        voltas++;
     }
     
     // Transforma o número de voltas em uma string
     IntToStr(voltas*60, voltas_string);

     // Seta "aux" para avisar ao main da interrupção
     aux = 1;

     cont = 0;           // Reinicia contagem
     T0CON.TMR0ON = 1;   // Dispara timer
}

void main()
{
     ADCON1=0x0F;                   // Configura todos os pinos como digitais
     Lcd_Init();                    // Inicializa biblioteca do LCD
     Lcd_Cmd(_LCD_CLEAR);           // Limpa LCD
     Lcd_Cmd(_LCD_CURSOR_OFF);      // Desliga cursor
     PWM1_Init(5000);               // Inicializa PWM1 (CCP1) em 5kHz
     TRISA = 0xFF;                  // Configura pinos do PORTA como entrada
     RC2_bit = 0;                   // Zera RC2
     TRISC2_bit = 0;                // Configura pino 2 do PORTC como saída
     cont = 0;                      // Inicia contagem do sensor em zero
     T0CON = 0x05;                  // PS = 1:64/modo de 16 bits/clock interno
     TMR0H = 0x85;                  // Carrega TMR0H para temporização de 1s
     TMR0L = 0xEE;                  // Carrega TMR0L para temporização de 1s
     GIE_bit = 1;                   // Habilita interrupções
     TMR0IE_bit = 1;                // Habilita interrupção do Timer0
     sensor_anterior = sensor;      // Salva estado atual do sensor
     T0CON.TMR0ON = 1;              // Dispara Timer0

     // Laço de repetição infinito
     while (1) 
     {
           // Incrementa "cont" quando o estado do sensor muda
           if (sensor != sensor_anterior) {
              cont++;
              sensor_anterior = sensor; // Salva estado do sensor
           }
           // Checa se houve interrupção
           if (aux == 1) {
               // Escreve no LCD a velocidade da ventoinha em rpm
               Lcd_Out(2, 2, voltas_string);
               Lcd_Out(2, 10, "rpm");
               
               // Zera "aux" para aguardar próxima interrupção
               aux = 0;
           }
           if (velocidade1 == 0) {
              PWM1_Stop();
              PWM1_Set_Duty(0);                    // Duty cycle = 0%
              Lcd_Out(1,1,"Duty cycle= 0% ");
              PWM1_Start();
           } 
           else if (velocidade2 == 0) {
                PWM1_Stop();
                
                // 25*255/100 = 64
                PWM1_Set_Duty(64);                 // Duty cycle = 25%
                Lcd_Out(1,1,"Duty cycle=25% ");
                PWM1_Start();
           } 
           else if (velocidade3 == 0) {
                PWM1_Stop();

                // 50*255/100 = 128
                PWM1_Set_Duty(128);                // Duty cycle = 50%
                Lcd_Out(1,1,"Duty cycle=50% ");
                PWM1_Start();
           } 
           else if (velocidade4 == 0) {
                PWM1_Stop();
                
                // 75*255/100 = 191
                PWM1_Set_Duty(191);                // Duty cycle = 75%
                Lcd_Out(1,1,"Duty cycle=75% ");
                PWM1_Start();
           } 
           else if (velocidade5 == 0) {
                PWM1_Stop();
                
                // 100*255/100 = 255
                PWM1_Set_Duty(255);                // Duty cycle = 100%
                Lcd_Out(1,1,"Duty cycle=100%");
                PWM1_Start();
           }
     }
}