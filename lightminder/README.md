# LightMinder

Projeto de sistema embarcado desenvolvido para monitorar o uso ineficiente de iluminação em ambientes internos. A solução detecta quando há **luz acesa sem presença de pessoas** e emite alertas visuais e sonoros para estimular o desligamento da luz, promovendo economia de energia.

---

## ⚙️ Visão Geral do Funcionamento

O sistema é composto por dois microcontroladores que se comunicam via UART:

### Arduino Mega
- Utiliza um **sensor LDR** para detectar iluminação ambiente.
- Utiliza um **sensor PIR** para detectar presença humana.
- Quando detecta **luz acesa sem presença**, inicia uma contagem regressiva.
- Se após o tempo determinado a situação permanecer a mesma, envia o caractere `'A'` via UART para o PIC.
- Ao receber o sinal de retorno `'R'`, o Arduino reinicia o monitoramento.

### PIC18F4520 (Microgenios)
- Recebe o sinal `'A'` via UART.
- Ativa **alertas visuais (LED, ventoinha)** e **sonoros (Buzzer)**.
- Exibe o estado no **display LCD** e conta o tempo de alerta.
- O alerta pode ser encerrado:
  - Ao pressionar um botão físico.
  - Ou automaticamente após um tempo limite.
- Ao encerrar, envia `'R'` via UART para o Arduino.

---

## 📁 Arquivos incluídos nesta pasta

- `Arduino_master.ino`: Código-fonte que roda no Arduino Mega.
- `PIC_slave.c`: Código em C para o PIC18F4520 (kit PICGenios).

---

## 🔌 Comunicação UART

- **Baud rate:** 9600 bps
- **Protocolo simples com dois comandos:**
  - `'A'`: alerta de luz acesa sem presença.
  - `'R'`: sinal de retorno para retomar o monitoramento.

---

## 🛠️ Requisitos

- Arduino Mega 2560
- PIC18F4520 (Microgenios)
- Sensor LDR
- Sensor PIR HC-SR501
- Display LCD (comunicação I2C)
- Comunicação serial UART entre os microcontroladores

---

## 🧰 Ferramentas e Softwares Utilizados

- **Arduino IDE**: escrita do código para Arduino e uso do monitor serial.
- **MikroC PRO for PIC**: compilação do código em C para gerar o `.hex` do PIC.
- **PICKit2 Programmer**: para gravar o `.hex` no microcontrolador PIC.
- **PICSimLab**: simulação do circuito e testes do código do PIC.

---

## 📷 Imagens e Esquemas

### 🔌 Esquema de ligação do Arduino (baseado em Arduino Uno):

> ![Arduino Schematic](assets/arduino.png)

*Obs: O esquema foi feito com Arduino Uno, mas o código foi executado no Arduino Mega.*

### 🧭 Configuração das chaves do kit PIC para os componentes usados:

> ![PIC DIP Switch Config](assets/pic.png)

### 🔌 Esquema de ligação da comunicação UART:

> ![UART Schematic](assets/uart.png)

---

*🌐 [English Version](README-EN.md)*

