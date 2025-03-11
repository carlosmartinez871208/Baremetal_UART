/*
** main.c file made by Carlos Martinez
*/

#ifndef EXIT_SUCCESS
 #define EXIT_SUCCESS 0u
#endif

/* USART6 Baremetal. */

/*                                    Peripherals                                   */
/* Peripheral base address. */
#define PERIPHERAL_BASE (0x40000000ul)

/* AHB1 base address. */
#define AHB1_PERIPHERAL_OFFSET (0x00020000ul)
#define AHB1_PERIPHERAL_BASE (PERIPHERAL_BASE + AHB1_PERIPHERAL_OFFSET)

/* APB2 base address. */
#define APB2_PERIPHERAL_OFFSET (0x10000ul)
#define APB2_PERIPHERAL_BASE (PERIPHERAL_BASE + APB2_PERIPHERAL_OFFSET)

/* GPIOA base address. */
#define GPIOA_OFFSET (0x0000ul)
#define GPIOA_BASE (AHB1_PERIPHERAL_BASE + GPIOA_OFFSET)

/* RCC base address. */
#define RCC_OFFSET (0x3800ul)
#define RCC_BASE (AHB1_PERIPHERAL_BASE + RCC_OFFSET)

/* USART6 base address. */
#define USART6_OFFSET (0x1400ul)
#define USART6_BASE (APB2_PERIPHERAL_BASE + USART6_OFFSET)

/*                                    Registers                                    */
/* RCC AHB1 enable clock register. */
#define RCC_AHB1ENR_OFFSET (0x30ul)
#define RCC_AHB1ENR (*(volatile unsigned int*)(RCC_BASE + RCC_AHB1ENR_OFFSET))

/* RCC APB2 enable clock register. */
#define RCC_APB2ENR_OFFSET (0x44ul)
#define RCC_APB2ENR (*(volatile unsigned int*)(RCC_BASE + RCC_APB2ENR_OFFSET))

/* GPIOA port mode register. */
#define GPIOA_MODER_OFFSET (0x0ul)
#define GPIOA_MODER (*(volatile unsigned int*)(GPIOA_BASE + GPIOA_MODER_OFFSET))

/* GPIOA port pull-up/pull-down register */
#define GPIOA_PUPDR_OFFSET (0xCul)
#define GPIOA_PUPDR (*(volatile unsigned int*)(GPIOA_BASE + GPIOA_PUPDR_OFFSET))

/* GPIOA alternate function high register. */
#define GPIOA_AFRH_OFFSET (0x24ul)
#define GPIOA_AFRH (*(volatile unsigned int*)(GPIOA_BASE + GPIOA_AFRH_OFFSET))

/* USART6 status register. */
#define USART6_SR_OFFSET (0x0ul)
#define USART6_SR (*(volatile unsigned int*)(USART6_BASE + USART6_SR_OFFSET))

/* USART6 Data register. */
#define USART6_DR_OFFSET (0x4ul)
#define USART6_DR (*(volatile unsigned int*)(USART6_BASE + USART6_DR_OFFSET))

/* Baudrate register */
#define USART6_BRR_OFFSET (0x8ul)
#define USART6_BRR (*(volatile unsigned int*)(USART6_BASE + USART6_BRR_OFFSET))

/* Control Register 1 */
#define USART6_CR1_OFFSET (0xCul)
#define USART6_CR1 (*(volatile unsigned int*)(USART6_BASE + USART6_CR1_OFFSET))

/* Control Register 2 */
#define USART6_CR2_OFFSET (0x10ul)
#define USART6_CR2 (*(volatile unsigned int*)(USART6_BASE + USART6_CR2_OFFSET))

/* Control Register 3 */
#define USART6_CR3_OFFSET (0x14ul)
#define USART6_CR3 (*(volatile unsigned int*)(USART6_BASE + USART6_CR3_OFFSET))

/*                              Configuration values                                    */
/* Enable clock to IO Port A */
#define RCC_AHB1ENR_GPIOAEN  (1ul << 0)
/* Set GPIOA pins 11 and 12 as alternate function  */
#define GPIOA_MODER11        (2ul << 22)
#define GPIOA_MODER12        (2ul << 24)
/* Configure GPIO AFR(Alternate Function register) for USART6 */
#define GPIOA_AFRH11_AF8     (8ul << 12)
#define GPIOA_AFRH12_AF8     (8ul << 16)
/* USART lines must be held high */
#define GPIOA_PUPDR11        (1ul << 22)
#define GPIOA_PUPDR12        (1ul << 24)
/* Enable APB1 peripheral clock. */
#define RCC_APB2ENR_USART6EN (1ul << 5)
/* Baudrate configuration */
#define USART6_CR1_OVER8     (1ul << 15)
#define USART6_BRR_MANTISSA  (45ul << 4) /* Mantissa = SystemClock/(8*(2-OVER8)*BAUDRATE) = 84000000/(16*115200) = 45.652 = 45*/
#define USART6_BRR_FRACTION  (9ul << 0)  /* Fraction = 0.5625 (page 527 Reference manual), 16 * 0.5625 = 9 */
/* Enable transciver and receiver */
#define USART6_CR1_TE        (1ul << 3)
#define USART6_CR1_RE        (1ul << 2)
/* Configure word length */
#define USART6_CR1_M         (1ul << 12)
/* Configure parity control */
#define USART6_CR1_PCE       (1ul << 10)
/* Configure stop bits */
#define USART6_CR2_STOP      (3ul << 12)
/* Disable RTS*/
#define USART6_CR3_RTSE      (1ul << 8)
/* Disable CTS */
#define USART6_CR3_CTSE      (1ul << 9)
/* Enable USART6 module */
#define USART6_CR1_UE        (1ul << 13)
/* Check data register empty */
#define USART6_SR_TXE        (1ul << 7)
/* MASK Data register */
#define USART6_DR_MASK       (0xFFul << 0)
/* Check transmission complete */
#define USART6_SR_TC         (1ul << 6)

/* Send char function */
int __io_putchar(int ch)
{
    while(USART6_SR_TXE!=(USART6_SR & USART6_SR_TXE)); /* make sure transmit data is empty */
    USART6_DR = (USART6_DR_MASK & ch);
    while(USART6_SR_TC!=(USART6_SR & USART6_SR_TC));
    return ch;
}

int main (void)
{
    /* Enable clock access to GPIOA. */
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* Set PA11 and PA12 as alternate function mode. */
    GPIOA_MODER |= GPIOA_MODER11;
    GPIOA_MODER |= GPIOA_MODER12;
    /* Set function alternate type: AF7 USART6. See Microcontroller Reference Manual, page 151.*/
    GPIOA_AFRH |= GPIOA_AFRH11_AF8;
    GPIOA_AFRH |= GPIOA_AFRH12_AF8;
    /* USART lines must be held high, it is needed to pull-up resistors. */
    GPIOA_PUPDR |= GPIOA_PUPDR11;
    GPIOA_PUPDR |= GPIOA_PUPDR12;
    /* Enable clock access to UART6. */
    RCC_APB2ENR |= RCC_APB2ENR_USART6EN;
    /* Configure UART baudrate: SysClock= 84 MHz, AHB1= 84 MHz, APB2= 42 Mhz, Oversample= 16, Baudrate= 115200, data= 8 bits, Parity= none, stop bit= 1. */
    /* Oversampling 16 */
    USART6_CR1 &= ~USART6_CR1_OVER8;
    /* Configure baudrate */
    USART6_BRR |= USART6_BRR_MANTISSA;
    USART6_BRR |= USART6_BRR_FRACTION;
    /* Configure transfer direction. */
    USART6_CR1 |= USART6_CR1_TE;
    /* Configure receive direction. */
    USART6_CR1 |= USART6_CR1_RE;
    /* Configure word length */
    USART6_CR1 &= ~USART6_CR1_M;
    /* Configure parity control */
    USART6_CR1 &= ~USART6_CR1_PCE;
    /* Configure stop bits: 1 stop bit */
    USART6_CR2 &= ~USART6_CR2_STOP;
    /* Disable RTS */
    USART6_CR3 &= ~USART6_CR3_RTSE;
    /* Disable CTS */
    USART6_CR3 &= ~USART6_CR3_CTSE;
    /* Enable UART Module. */
    USART6_CR1 |= USART6_CR1_UE;
    while(1)
    {
        __io_putchar('H');
        __io_putchar('i');
        __io_putchar(' ');
        __io_putchar('U');
        __io_putchar('A');
        __io_putchar('R');
        __io_putchar('T');
        __io_putchar('\n');
        for(int i=0ul;i<2000000;i++){}
    }
    return EXIT_SUCCESS;
}