# STM32-Watch
Um smart watch feito utilizando uma STM32f030. Projeto de final da disciplina de Sistemas Microprocessados 2018.2 .

  Membros(Matrícula): Eduardo Viana (399016), Emerson Marques (398425), Samuel Façanha (398196).
  
  O STM32-Watch utiliza o micro-contolador STM32f030x6 para implementar um protótipo de Smart-Watch,ou seja, um dispositivo portátil , que possui ferramentas que visam facilitar o dia-a-dia do usuário.
 
 
   A princípio, o STM32-Watch, implementará as seguintes ferrmentas:
 -Relógio (Módulo Real Time Clock);
 -Despertador;
 -Dia da semana;
 -Data;
 -Cronômetro;
 -Estimador de batimento cardíaco;

  Materiáis:
 -STM32F0
 -Display OLED i2c SSD1306 128x64
 -3 botões
 -1x sensor ky-039 
 -1x buzzer.
 -Bateria 3V
 
  Configuração:
  O relógio tem 4 principais modos:
  
  - Menu:
  Mostra as horas e a data (com o dia da semana e ano de 4 dígitos).
  
  - Mudar hora e data:
  Mostra a hora e data anterior e a depois de mudada. Para entrar nesse modo basta apertar o botão 1 dentro do menu. Apertando o botão 2 pode-se aumentar os valores mostrados, apertando o botão 1, pode-se mudar entre horas, minutos, dia, mês e dia da semana. 
  
  - Muda hora do alarme:
  Mostra a hora anterior do alarme e a depois de mudada. Para entrar nesse modo basta apertar o botão 2 dentro do menu. Apertando o botão 2 novamente aumenta o valor das horas e minutos do alarme e apertando o botão 1 alterna entre minutos, horas e volta pro menu.
  
  - Cronômetro:
  Cronometragem começando de zero. Para entrar nesse modo basta apertar o botão 3 no menu. Apertando o botão 1 volta para o menu e apertando o 2 começa a contagem de tempo.
  
  Os 3 botões têm funções distintas:
  - Botão 1 (PA0): Este é o botão principal, que vai trocando os modos no relógio. Também desativa o alarme quando acionado
  
  - Botão 2 (PA1): Este é o botão para alterar os valores de hora, data e alarme. No menu este é o botão para entrar no modo de mudar a hora do alarme.
  
  - Botão 3(PA5): Este é o botão para entrar no modo cronômetro.
  
OBS: Os três botões são configurados como input e pull-down no Stm32Cubemx.

- Alarme: o buzzer está ligado no PA4 configurado como output e nopull no Cube.
Quando o as horas configuradas no alarme são iguais as horas mostradas no menu do relógio, o buzzer é ativado e deve ser desativado apertando o botão principal.

- Estimador de batimento cardíaco:
O pino PA6 foi configurado como ADC no Cube para ler os valores analógicos do sensor e converter para digital.
O ADC mede dois valores captados pelo sensor com um delay de 20ms entre eles, se os valores tiverem uma diferença muito grande, quer dizer que houve uma pulsação do coração, entrão pisca na tela um símbolo representando o coração. O tempo entre cada pulso estima uma fequência cardíaca.
  
  Limitações:
  
  A implementação desse projeto apresentou algumas limitações, principalmente no que diz respeito a memória flash, pois a memória teve grande parte consumida para o display 128x64 e pela inicialização do RTC interno. Além Disso, o RTC do microprocessador utilizado é um pouco impreciso, aproximadamente 20% de adiatamento em relação ao tempo real. 
  
  
  Código de terceiros:
  Os códigos para configuração do display e suas funções para utilização foram: https://www.controllerstech.com/oled-display-using-i2c-stm32/
  
  
  
   
   
