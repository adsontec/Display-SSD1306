## Display-SSD1306 🔠
Este projeto demonstra como usar o Raspberry Pi Pico W com o display SSD1306 que possui 8192 pixels para fazer a comunicação serial I2C, passando informacão para o usuário via USB, além de usar um display númerico com a matriz de LEDs (WS2812) usando dois butões para discar o LED RGB, consolidando os conceitos sobre o uso de interfaces de comunicação serial no RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab
## Funcionalidade: 📊
Ao pressionar o botão A alternar o estado do LED verde (ligado/desligado) e ao pressionar o botão B alternar o estado do LED azul (ligado/desligado) via interrupção callback, tambem tem um display SSD1306 que informa sobre o estado do LED pelo monitor serial via comunicação I2C(Inter-Integrated Circuit), utilizando o serial monitor do VS Code para exibir os caracteres (A até Z) maiúscula e minúscula e os números (0 até 9) que deve aparecer na matriz de LEDs (WS2812).
## Requisitos de Hardware: 👨‍🔧
- Microcontrolador Raspberry Pi Pico W
- Display SSD1306
- Matriz 5x5 de LEDs (WS2812)
- 1 LED (RGB)
- 1 Resistores de 220 Ω
- 2 Buttton (Pushbutton)
## Requisitos de Software: ☑️
- Instalar o Visual Studio Code
- Instalar a extensão Wokwi no VSCode
- Instalar os pacotes do Pico SDK
## Pinos GPIO dos LEDs ⏰
- Display SSD1306 conectado via I2C: GPIO 14 e GPIO 15
- Matriz 5x5 (WS2812): GPIO 7 
- LED (RGB): GPIO 11, GPIO 12 e GPIO 13
- Button A: GPIO 5
- Button B: GPIO 6 
## Explicação do Código: 🕵️
- **stdio_init_all()**: Inicializar comunicação padrão
- **inicializar_GPIOs()**: Inicializar as configurações dos pinos dos LEDs e dos putões
- **init_display()**: Inicializa o display SSD1306
- **init_gpio()**: Define a função do pino GPIO 14(SDA) e 15(SCL) para I2C
- **ssd1306()**: Atualiza o display com os novos dados
## Link com a produção do vídeo 📀

https://drive.google.com/file/d/1RSreU6B2R3uX2dnI6HYKA6BVo8zJvbeF/view?usp=drive_link

## Imagem do Projeto ✔️
![image](https://github.com/user-attachments/assets/a01a0bc1-c4dc-4a71-9bd1-191b35a02d21)
