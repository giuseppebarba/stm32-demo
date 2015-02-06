#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

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


int main(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	 * Enable APB2 clock
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/*
	 * Configure PC9 pin
	 */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*
	 * Configure the system clock to generate an interrupt every 1 msec
	 */
	SysTick_Config(SystemCoreClock / 1000);
	while(1) {
		delay(1000);
		toggle_led();
	}

}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
	while(1) {
	}
}
#endif
