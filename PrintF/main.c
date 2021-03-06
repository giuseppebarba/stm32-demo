#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <xprintf.h>
#include <uart.h>


//Use __IO to avoid compiler optimizations
static __IO uint32_t TimingDelay;

void delay(uint32_t nTime) {
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void) {
	if(TimingDelay != 0x0)
		TimingDelay--;
}
void toggle_led() {
	static uint8_t ledval = 0;

	GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
	ledval = 1 - ledval;
}

void putchar(unsigned char c) {
	uart_putc(c, USART1);
}

unsigned char getchar() {
	return uart_getc(USART1);
}

int main(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	int counter = 0;

	/*
	 * Enable clock for GPIOC
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/*
	 * Enable clock for USART1
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO |
				RCC_APB2Periph_GPIOA, ENABLE);

	/*
	 * Configure PC9 pin
	 */
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*
	 * Configure UART1 TX
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*
	 * Configure UART1 RX
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*
	 * Configure USART parameter to 9600 baud, 8 data bits, 1 stop bit,
	 * no parity and no flow control
	 */
	uart_open(USART1, 9600, USART_Mode_Rx | USART_Mode_Tx);
	xfunc_in = getchar;
	xfunc_out = putchar;

	/*
	 * Configure the system clock to generate an interrupt every 1 msec
	 */
	SysTick_Config(SystemCoreClock / 1000);
	while(1) {
		delay(1000);
		toggle_led();
		xprintf("Test %d\n", counter++);

	}

}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
	while(1) {
	}
}
#endif
