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
  Mostra as horas e a data (com o dia da semana).
  
  - Mudar hora e data:
  Mostra a hora e data anterior e a depois de mudada.
  
  - Muda hora do alarme:
  Mostra a hora anterior do alarme e a depois de mudada.
  
  - Cronômetro:
  Cronometragem começando de zero.
  
  Os 3 botões têm funções distintas:
  - Botão 1 (PA0): Este é o botão principal, que vai trocando os modos no relógio.
  
  - Botão 2 (PA3): Este é o botão para alterar os valores de hora, data e alarme. No menu este é o botão para entrar no modo de mudar a hora do alarme.
  
  - Botão 3(
  
  
  
  
 
 
  Limitações:
  
  A implementação desse projeto apresentou algumas limitações, principalmente no que diz respeito a memória flash, pois a memória teve grande parte consumida para o display 128x64 e pela inicialização do RTC interno. Além Disso, o RTC do microprocessador utilizado é um pouco impreciso, aproximadamente 20% de adiatamento em relação ao tempo real. 
  
  
  Código de terceiros:
  Os códigos para configuração do display e suas funções para utilização foram: https://www.controllerstech.com/oled-display-using-i2c-stm32/
  
  
  
   
   
