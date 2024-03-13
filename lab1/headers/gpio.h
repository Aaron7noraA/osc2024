#ifndef GPIO_H
#define GPIO_H

// Spec BCM2835 P.6
#define PERIPHERAL_PHY_BASE (0x3F000000)
#define PERIPHERAL_BUS_BASE (0x7E000000)

#define BUS2PHY(a) (( a & 0x00FFFFFF) | PERIPHERAL_PHY_BASE)
#define PHY2BUS(a) (( a & 0x00FFFFFF) | PERIPHERAL_BUS_BASE)

#define GPIO_OFFSET BUS2PHY(0x7E200000)

// define all the pins
// spec given in bus address

#define GPFSEL0     (( volatile unsigned int *)(GPIO_OFFSET + 0x0000))
#define GPFSEL1     (( volatile unsigned int *)(GPIO_OFFSET + 0x0004))
#define GPFSEL2     (( volatile unsigned int *)(GPIO_OFFSET + 0x0008))
#define GPFSEL3     (( volatile unsigned int *)(GPIO_OFFSET + 0x000C))
#define GPFSEL4     (( volatile unsigned int *)(GPIO_OFFSET + 0x0010))
#define GPFSEL5     (( volatile unsigned int *)(GPIO_OFFSET + 0x0014))

#define GPSET0      (( volatile unsigned int *)(GPIO_OFFSET + 0x001C))
#define GPSET1      (( volatile unsigned int *)(GPIO_OFFSET + 0x0020))

#define GPCLR0      (( volatile unsigned int *)(GPIO_OFFSET + 0x0028))//0028
#define GPCLR1      (( volatile unsigned int *)(GPIO_OFFSET + 0x002C))//002C

#define GPLEV0      (( volatile unsigned int *)(GPIO_OFFSET + 0x0034))//0034
#define GPLEV1      (( volatile unsigned int *)(GPIO_OFFSET + 0x0038))//0038

#define GPEDS0      (( volatile unsigned int *)(GPIO_OFFSET + 0x0040))//0040
#define GPEDS1      (( volatile unsigned int *)(GPIO_OFFSET + 0x0044))//0044

#define GPREN0      (( volatile unsigned int *)(GPIO_OFFSET + 0x004C))//004C
#define GPREN1      (( volatile unsigned int *)(GPIO_OFFSET + 0x0050))//0050

#define GPFEN0      (( volatile unsigned int *)(GPIO_OFFSET + 0x0058))//0058
#define GPFEN1      (( volatile unsigned int *)(GPIO_OFFSET + 0x005C))//005C

#define GPHEN0      (( volatile unsigned int *)(GPIO_OFFSET + 0x0064))//0064
#define GPHEN1      (( volatile unsigned int *)(GPIO_OFFSET + 0x0068))//0068

#define GPLEN0      (( volatile unsigned int *)(GPIO_OFFSET + 0x0070))//0070
#define GPLEN1      (( volatile unsigned int *)(GPIO_OFFSET + 0x0074))//0074

#define GPAREN0     (( volatile unsigned int *)(GPIO_OFFSET + 0x007C))//007C
#define GPAREN1     (( volatile unsigned int *)(GPIO_OFFSET + 0x0080))//0080

#define GPAFEN0     (( volatile unsigned int *)(GPIO_OFFSET + 0x0088))//0088
#define GPAFEN1     (( volatile unsigned int *)(GPIO_OFFSET + 0x008C))//008C

#define GPPUD       (( volatile unsigned int *)(GPIO_OFFSET + 0x0094))//0094
#define GPPUDCLK0   (( volatile unsigned int *)(GPIO_OFFSET + 0x0098))//0098
#define GPPUDCLK1   (( volatile unsigned int *)(GPIO_OFFSET + 0x009C))//009C

#endif