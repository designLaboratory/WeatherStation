#define LED_GREEN           PTD,5
#define LED_RED             PTE,29
#define SW_1                PTC,3
#define SW_2                PTC,12

static void Clock_Config(void);
static void GPIO_Config(void);
static void I2C_Init(void);
