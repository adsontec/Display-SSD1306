#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

// Define o Pinos GPIO
#define UART_ID uart0 // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0 // Pino GPIO usado para TX
#define UART_RX_PIN 1 // Pino GPIO usado para RX
#define GPIO_R_LED 13
#define GPIO_B_LED 12
#define GPIO_G_LED 11
#define BUTTON_A 5
#define BUTTON_B 6
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Conexão dos pinos GPIO
const uint8_t GPIOs[] = {GPIO_R_LED, GPIO_B_LED, GPIO_G_LED, BUTTON_A, BUTTON_B};

// Variáveis globais
volatile absolute_time_t last_press_time = 0;
ssd1306_t ssd; 
bool cor = true;

// Protótipo de Função
void inicializar_GPIOs();
void init_gpio();
void init_display();
void ssd1306();
void gpio_irq_handler(uint gpio, uint32_t events);
void uart_gpio();

int main(){
  inicializar_GPIOs();
  i2c_init(I2C_PORT, 400 * 1000);
  init_gpio();
  init_display();
  uart_gpio();

  // Mensagem inicial
  const char *init_message = "UART Demo - RP2\r\n"
                               "Digite algo e veja o eco:\r\n";
  uart_puts(UART_ID, init_message);

  // Configuração da interrupção com callback
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

  while (true){
    if (uart_is_readable(UART_ID)) {
      // Lê um caractere da UART
      char c = uart_getc(UART_ID), letra = c;
      char texto[2] = {letra, '\0'}; // Criamos uma string com um único caractere

      ssd1306_fill(&ssd, !cor); // Limpa o display
      ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
      ssd1306_draw_string(&ssd, "INSIRA O DADO" , 8, 10); // Desenha uma string
      ssd1306_draw_string(&ssd, texto, 20, 30); // Desenha uma string     
      ssd1306_send_data(&ssd);
    }
  }
}

void init_gpio(){
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
}

void init_display(){
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
}

// Função para inicializar saídas dos LEDs e as entradas dos Buttons
void inicializar_GPIOs(){
    for(int i = 0; i < 5; i++){
        gpio_init(GPIOs[i]);
        if( i < 3){
            gpio_set_dir(GPIOs[i], GPIO_OUT);
        }else{
            gpio_set_dir(GPIOs[i], GPIO_IN);
            // Habilita o resistor pull-up interno
            gpio_pull_up(GPIOs[i]);
        }
    }
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events){
  bool btn_last_state = 0;

    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    bool btn_pressed = !gpio_get(gpio);

    // Verifica se passou tempo suficiente desde o último evento
    if (btn_pressed && !btn_last_state && (absolute_time_diff_us(last_press_time, get_absolute_time()) > 200000)){
        last_press_time = get_absolute_time();
        btn_last_state = 1;
        if (gpio == BUTTON_A){
          gpio_put(GPIO_G_LED, !gpio_get(GPIO_G_LED));
        }
    }else if (!btn_pressed){
        btn_last_state = 0;
    }
}

void uart_gpio(){
  uart_init(UART_ID, BAUD_RATE);
  // Configura os pinos GPIO para a UART
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para RX
}