// Função para carregar TMRM0H e TMR0L
void carregaT0 (unsigned char msb, unsigned char lsb) {
  TMR0H = msb;
  TMR0L = lsb;
}

// Função de disparo do Timer0
void T0_disp () {
  T0CON.TMR0ON = 1;            // Dispara contagem do Timer0
  while (INTCON.TMR0IF == 0);  // Aguarda o flag de interrupção do Timer0
  INTCON = 0x20;               // Timer0 Off, flag de interrupção = 0, interrupção do Timer0 habilitada
}

void main() {
     ADCON1 = 0x0F;       // Define todas as portas como digitais
     TRISB = 0b00001111;  // Colunas(0000) = OUT; Linhas(1111) = IN
     TRISD.TRISD0 = 0;    // D0 (buzzer) = OUT
     INTCON = 0x20;       // Habilita interrupção do Timer0
     INTCON2 = 0x80;      // Desabilita resistores de pull-up
     T0CON = 0x08;        // PS desabilitado
     
     portd.rd0 = 0;       // Buzzer desligado
     
     // Coloca todas as colunas em nível lógico 1
     portb.rb4 = 1;
     portb.rb5 = 1;
     portb.rb6 = 1;
     portb.rb7 = 1;
     
     // Laço de repetição que faz a varredura dos botões indefinidamente
     while(1) {
              if (portb.rb3 == 1){
              // Algum botão da linha 1 foi apertado
              
                 // Testa se foi o botão da 1a coluna (1)
                 portb.rb4 = 0;
                 portb.rb5 = 0;
                 portb.rb6 = 0;
                 portb.rb7 = 1;
                 if (portb.rb3 == 1){
                 // Tecla 1
                    portd.rd0 = !portd.rd0;
                    carregaT0(0xFE, 0x41);
                    T0_disp();
                 } else {
                   // Testa se foi o botão da 2a coluna (2)
                   portb.rb4 = 0;
                   portb.rb5 = 0;
                   portb.rb6 = 1;
                   portb.rb7 = 0;
                   if (portb.rb3 == 1){
                   // Tecla 2
                      portd.rd0 = !portd.rd0;
                      carregaT0(0xFE, 0x81);
                      T0_disp();
                   } else {
                   
                     // Testa se foi o botão da 3a coluna (3)
                     portb.rb4 = 0;
                     portb.rb5 = 1;
                     portb.rb6 = 0;
                     portb.rb7 = 0;
                     if (portb.rb3 == 1){
                     // Tecla 3
                        portd.rd0 = !portd.rd0;
                        carregaT0(0xFE, 0xB7);
                        T0_disp();
                     } else {
                     
                       // Testa se foi o botão da 4a coluna (4)
                       portb.rb4 = 1;
                       portb.rb5 = 0;
                       portb.rb6 = 0;
                       portb.rb7 = 0;
                       if (portb.rb3 == 1){
                       // Tecla 4
                          portd.rd0 = !portd.rd0;
                          carregaT0(0xFE, 0xD3);
                          T0_disp();
                       } else {
                       
                         // Caso contrário, coloca 1 em todas as colunas e desliga buzzer
                         portb.rb4 = 1;
                         portb.rb5 = 1;
                         portb.rb6 = 1;
                         portb.rb7 = 1;
                         portd.rd0 = 0;
                       }
                     }
                   }
                 }
              } else if(portb.rb2 == 1) {
              // Algum botão da linha 2 foi apertado
              
                 // Testa se foi o botão da 1a coluna (5)
                 portb.rb4 = 0;
                 portb.rb5 = 0;
                 portb.rb6 = 0;
                 portb.rb7 = 1;
                 if (portb.rb2 == 1){
                 // Tecla 5
                    portd.rd0 = !portd.rd0;
                    carregaT0(0xFE, 0xE8);
                    T0_disp();
                 } else {
                 
                   // Testa se foi o botão da 2a coluna (6)
                   portb.rb4 = 0;
                   portb.rb5 = 0;
                   portb.rb6 = 1;
                   portb.rb7 = 0;
                   if (portb.rb2 == 1){
                   // Tecla 6
                      portd.rd0 = !portd.rd0;
                      carregaT0(0xFF, 0x11);
                      T0_disp();
                   } else {
                   
                     // Testa se foi o botão da 3a coluna (7)
                     portb.rb4 = 0;
                     portb.rb5 = 1;
                     portb.rb6 = 0;
                     portb.rb7 = 0;
                     if (portb.rb2 == 1){
                     // Tecla 7
                        portd.rd0 = !portd.rd0;
                        carregaT0(0xFF, 0x39);
                        T0_disp();
                     } else {
                     
                       // Testa se foi o botão da 4a coluna (8)
                       portb.rb4 = 1;
                       portb.rb5 = 0;
                       portb.rb6 = 0;
                       portb.rb7 = 0;
                       if (portb.rb2 == 1){
                       // Tecla 8
                          portd.rd0 = !portd.rd0;
                          carregaT0(0xFF, 0x4C);
                          T0_disp();
                       } else {
                       
                         // Caso contrário, coloca 1 em todas as colunas e desliga buzzer
                         portb.rb4 = 1;
                         portb.rb5 = 1;
                         portb.rb6 = 1;
                         portb.rb7 = 1;
                         portd.rd0 = 0;
                       }
                     }
                   }
                 }
              } else {
              /* Caso nenhum botão das duas primeiras linhas esteja apertado,
              coloca 1 em todas as colunas e desliga buzzer */
              
                portb.rb4 = 1;
                portb.rb5 = 1;
                portb.rb6 = 1;
                portb.rb7 = 1;
                portd.rd0 = 0;
              }
     }
}