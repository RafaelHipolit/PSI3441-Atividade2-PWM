#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// semafaro

#define SLEEP_TIME_1_SEC 1000

// Define o LED usando Device Tree
#define LED0_NODE DT_ALIAS(led0) //green
#define LED1_NODE DT_ALIAS(led1) //blue 
#define LED2_NODE DT_ALIAS(led2) // red

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec ledVerde = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
static const struct gpio_dt_spec ledAzul = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#else
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

#if DT_NODE_HAS_STATUS(LED2_NODE, okay)
static const struct gpio_dt_spec ledVermelho = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
#else
#error "Unsupported board: led2 devicetree alias is not defined"
#endif

//estados
#define VERDE 0
#define AMARELO 1
#define VERMELHO 2

void mudaCor(char estado){
    if (estado == VERDE)
    {
        gpio_pin_set_dt(&ledVermelho, 0); 
        gpio_pin_set_dt(&ledVerde, 1);
        gpio_pin_set_dt(&ledAzul, 0);
    }
    else if (estado == AMARELO)
    {
        // AMARELO = Vermelho + Verde
        gpio_pin_set_dt(&ledVermelho, 1); 
        gpio_pin_set_dt(&ledVerde, 1);
        gpio_pin_set_dt(&ledAzul, 0);
    }
    else if (estado == VERMELHO)
    {
        gpio_pin_set_dt(&ledVermelho, 1); 
        gpio_pin_set_dt(&ledVerde, 0);
        gpio_pin_set_dt(&ledAzul, 0);
    }
}

void dorme(char estado){
    if (estado == VERDE)
    {
        k_msleep(SLEEP_TIME_1_SEC*5);
    }
    else if (estado == AMARELO)
    {
        k_msleep(SLEEP_TIME_1_SEC*2);
    }
    else if (estado == VERMELHO)
    {
        k_msleep(SLEEP_TIME_1_SEC*5);
    }
}

void main(void)
{
    int ret;
	char estado = VERMELHO;

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&ledAzul)) {
        printk("Error: LED device %s is not ready\n", ledAzul.port->name);
        return;
    }
    if (!gpio_is_ready_dt(&ledVerde)) {
        printk("Error: LED device %s is not ready\n", ledVerde.port->name);
        return;
    }
    if (!gpio_is_ready_dt(&ledVermelho)) {
        printk("Error: LED device %s is not ready\n", ledVermelho.port->name);
        return;
    }

    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&ledAzul, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }
    ret = gpio_pin_configure_dt(&ledVerde, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }
    ret = gpio_pin_configure_dt(&ledVermelho, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    printk("Semafaro");
    printk("LED blue %s pin %d\n", ledAzul.port->name, ledAzul.pin);
    printk("LED green %s pin %d\n", ledVerde.port->name, ledVerde.pin);
    printk("LED red %s pin %d\n", ledVermelho.port->name, ledVermelho.pin);

    while (1) {
        if (estado == VERDE)
        {
            estado = AMARELO;
        }
        else if (estado == AMARELO)
        {
            estado = VERMELHO;
        }
        else if (estado == VERMELHO)
        {
            estado = VERDE;
        }
        mudaCor(estado);
        dorme(estado);
    }
}