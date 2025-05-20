# 🎮 Jogo da Forca com Arduino / Hangman Game with Arduino

Este projeto implementa uma versão simplificada do clássico **Jogo da Forca** usando um **Arduino Mega**, três **botões físicos**, três **LEDs** e um **buzzer ativo**. A interação do usuário acontece via **monitor serial** e botões físicos.

This project implements a simplified version of the classic **Hangman game** using an **Arduino Mega**, three **push buttons**, three **LEDs**, and a **active buzzer**. User interaction occurs through the **serial monitor** and physical buttons.

---

## ⚙️ Requisitos / Requirements

- Arduino Mega (pode ser adaptado para Uno) / Arduino Mega (can be adapted to Uno)
- 3 botões / 3 push buttons
- 3 LEDs
- 1 buzzer ativo / 1 active buzzer
- Resistores adequados / Suitable resistors
- Arduino IDE para subir o código / Arduino IDE to upload the code

---

## 🖼️ Circuito & Simulação / Circuit & Simulation

Uma **imagem de sugestão de montagem** do circuito foi incluída na pasta do projeto, criada com o **Tinkercad**.

A **circuit diagram image** is included in the project folder, created with **Tinkercad**.

> ⚠️ Nota: a simulação utiliza um **Arduino Uno** e um **buzzer passivo** por limitações do simulador.  
> O projeto real utiliza um **Arduino Mega** e um **buzzer ativo**.

> ⚠️ Note: The simulation uses an **Arduino Uno** and an **passive buzzer** due to simulator limitations.  
> The actual project uses an **Arduino Mega** and a **active buzzer**.

---

## 🕹️ Controles / Controls

| Botão / Button | Função no Menu / Menu Function          | Função no Jogo / In-Game Function           |
|----------------|------------------------------------------|----------------------------------------------|
| 1              | Selecionar opção 1 / Select option 1     | Adivinhar a palavra / Guess the whole word   |
| 2              | Selecionar opção 2 / Select option 2     | Adivinhar uma letra / Guess a letter         |
| 3              | Selecionar opção 3 / Select option 3     | Próxima palavra (modo médio / difícil apenas) / Next word (medium and hard mode only) |

---

## 📌 Funcionalidades / Features

- Seleção de dificuldade: Fácil, Médio, Difícil / Difficulty selection: Easy, Medium, Hard
- Tentativas limitadas por palavra / Limited number of tries per word
- Escolha entre chutar letra ou palavra / Choose between guessing a **letter** or the **full word**
- LEDs indicam qual botão foi pressionado e acendem na vitória / LEDs indicate which button was pressed and light up on victory
- Tempo limite por dificuldade / Time limit based on selected difficulty
- Buzzer soa na derrota / Buzzer sounds on defeat
- Leitura com debounce dos botões / Debounced button reading

---

## 🚦 LEDs e Buzzer / LEDs and Buzzer

- **LED1, LED2, LED3**: Acendem conforme o botão pressionado / Light up based on which button was pressed
- **Todos LEDs acendem**: Vitória / **All LEDs light up**: Victory
- **Buzzer soa**: Derrota / **Buzzer sounds**: Defeat

---

## 🧠 Lógica do Jogo / Game Logic

1. O jogador escolhe a dificuldade (botões 1, 2 ou 3)  
   The player selects a difficulty level (buttons 1, 2, or 3)
2. Escolhe iniciar ou voltar ao menu  
   Chooses whether to start the game or go back
3. Em cada rodada, escolhe:  
   In each round, chooses to:
   - Adivinhar a palavra / Guess the full word
   - Adivinhar uma letra / Guess a single letter
4. O jogo verifica se acertou / The game checks if the attempt is correct
5. O jogo termina com vitória (todas palavras certas) ou derrota (tentativas esgotadas)  
   Game ends with a victory (all words guessed) or a defeat (tries exhausted)

---

## ⏲️ Tempo por Dificuldade / Time Limit Per Difficulty

| Dificuldade / Difficulty | Tempo / Time Limit |
|--------------------------|--------------------|
| Fácil / Easy             | 120 segundos       |
| Médio / Medium           | 60 segundos        |
| Difícil / Hard           | 30 segundos        |

---

## 🧪 Testado com / Tested With

- Arduino Mega
- Monitor serial da IDE Arduino / Arduino IDE Serial Monitor
- Botões com resistores de pull-up / Push buttons with pull-up resistors
- Resistores de 220Ω nos LEDs / 220Ω resistors for LEDs
- Buzzer ativo / Active buzzer
- Simulação Tinkercad com Uno e buzzer passivo / Tinkercad simulation with Uno and passive buzzer

---

## 🗃️ Estrutura do Código / Code Structure

- `setup()`: Inicializa os pinos e comunicação serial  
  Initializes pins and serial communication
- `loop()`: Implementa a máquina de estados do jogo  
  Implements the game state machine
- Funções auxiliares / Helper functions:
  - `readButtonWithDebounce(...)`: Leitura com debounce  
  Reads buttons with debounce logic
  - `startTimeout(...)`, `isTimeoutExpired()`, `cancelTimeout()`: Controle de tempo limite  
  Timeout control