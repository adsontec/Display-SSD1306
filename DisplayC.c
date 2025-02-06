#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

ssd1306_t ssd; // Inicializa a estrutura do display
bool cor = true;

void init_gpio();
void init_display();
void ssd1306();

int main(){
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);
  init_gpio();
  init_display();

  while (true){
    cor = !cor;
    ssd1306();
    sleep_ms(1000);
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

void ssd1306(){
    // Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Cepedi Tic37", 8, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "Embarcatech", 20, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "Prof Wilton", 15, 48); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
}