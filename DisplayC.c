#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

// Define o Pinos GPIO
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

int main(){
  stdio_init_all();
  inicializar_GPIOs();
  i2c_init(I2C_PORT, 400 * 1000);
  init_gpio();
  init_display();

  // Configuração da interrupção com callback
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  printf("RP2040 inicializado\n");
  
  while (true){
    if(stdio_usb_connected()) {
      // Lê um caractere da UART
      char c, letra = c;
        if ((c = getchar()) != EOF) { // Lê um caractere da entrada padrão
            char texto[2] = {c, '\0'}; // Converte o caractere em string
            
            ssd1306_fill(&ssd, !cor); // Limpa o display antes de desenhar
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo na tela
            ssd1306_draw_string(&ssd, "INSIRA O DADO", 8, 10); // Exibe mensagens no display
            ssd1306_draw_string(&ssd, texto, 20, 30); // Exibe o caractere digitado
            ssd1306_send_data(&ssd); // Atualiza o display com os novos dados
        }
    }
    sleep_ms(40);
  }
  return 0;
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
          printf("LED verde %s\n", (gpio_get(GPIO_G_LED) == 0) ? "ligado" : "desligado");
          ssd1306_fill(&ssd, !cor); // Limpa o display
          ssd1306_draw_string(&ssd,(gpio_get(GPIO_G_LED) == 0) ? "G LIGADO" : "G DESLIGADO", 15, 48);
          ssd1306_send_data(&ssd);
          gpio_put(GPIO_G_LED, !gpio_get(GPIO_G_LED));
        }else if ((gpio == BUTTON_B)){
          printf("LED Azul %s\n", (gpio_get(GPIO_B_LED) == 0) ? "ligado" : "desligado");
          ssd1306_fill(&ssd, !cor); // Limpa o display
          ssd1306_draw_string(&ssd,(gpio_get(GPIO_B_LED) == 0) ? "B LIGADO" : "B DESLIGADO", 15, 48);
          ssd1306_send_data(&ssd);
          gpio_put(GPIO_B_LED, !gpio_get(GPIO_B_LED));
        }
    }else if (!btn_pressed){
        btn_last_state = 0;
    }
}
