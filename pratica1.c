unsigned char timer0msb;        // Variável a ser carregada ao TMR0H
unsigned char timer0lsb;        // Variável a ser carregada ao TMR0L


// Disparo de T0
void T0_disp () {
  T0CON.TMR0ON = 1;                 // Começa contagem do Timer0
  while (INTCON.TMR0IF == 0){       // Aguarda o overflow do Timer0
  
    /* Enquanto não houver o overflow, o programa checa se algum dos botões
    foi pressionado e ajusta a temporização de acordo*/
    if (portb.rb1 == 0) {
      timer0msb = 0xF0;
      timer0lsb = 0xBE;
    } else if (portb.rb0 == 0) {
      timer0msb = 0xC2;
      timer0lsb = 0xF7;
    }
  }
  T0CON.TMR0ON = 0;   // Para Timer0
  INTCON.TMR0IF = 0;  // Zera bit do flag de overflow do Timer0
}

// Carrega TMR0H e TMR0L com os parâmetros passados à função
void carregaT0 (unsigned char msb, unsigned char lsb) {
  TMR0H = msb;
  TMR0L = lsb;
}

// Função para contagem em loop
void conta () {

  // Obs: o ponto do display troca de estado a cada ciclo para medirmos
  // o período da contagem no simulIDE
  while (1){
    latd = 0b11111100;                 // display = 0
    carregaT0 (timer0msb, timer0lsb);  // Recarrega TMR0H e TMR0L
    T0_disp ();                        // Dispara Timer0
    latd = 0b01100001;                 // display = 1
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b11011010;                 // display = 2
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b11110011;                 // display = 3
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b01100110;                 // display = 4
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b10110111;                 // display = 5
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b10111110;                 // display = 6
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b11100001;                 // display = 7
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b11111110;                 // display = 8
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
    latd = 0b11110111;                 // display = 9
    carregaT0 (timer0msb, timer0lsb);
    T0_disp ();
  }
}

//////////////////////// MAIN ////////////////////////
void main () {
  ADCON1 = 0x0F;           // Configura todas as portas do PIC como digitais
  trisb.trisb0 = 1;        // Configura a Porta RB0 como entrada
  trisb.trisb1 = 1;        // Configura a Porta RB1 como entrada
  trisd = 0;               // Configura todas as portas RD<7-0> como saídas
  T0CON = 0x06;            // PS = 1:128, modo de 16 bits, clock interno,
/*// GIE =  1, PEIE = 1
  INTCON.GIE = 1;       // Habilita interrupção do Timer0
  INTCON.PEIE = 1;
  INTCON.TMR0IE = 1;
  //INTCON2.TMR0IP = 1;
  IPEN_bit = 0;            //desabilita prioridades*/
  latd = 0x00;             // Apaga display
  
  // Loop que aguarda algum dos botões ser pressionado para iniciar contagem
  while (1) {
    if (portb.rb1 == 0) { // Se o botão em RB1 for pressionado, inicia contagem
      timer0msb = 0xF0;   // com período de 0,25s
      timer0lsb = 0xBE;
      conta ();   // Começa contagem
    } else if (portb.rb0 == 0) { // Se o botão em RB1 for pressionado, inicia
      timer0msb = 0xC2;          // contagem com período de 1s
      timer0lsb = 0xF7;
      conta ();   // Começa contagem
    }
  }
}