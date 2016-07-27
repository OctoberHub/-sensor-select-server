#ifndef _RS485_SERIAL_H
#define _RS485_SERIAL_H
typedef unsigned int		uint;

#define DB120_CH_485_DE1       1
#define DB120_CH_485_DE2       2

/* RS485 ioctls: */
//#define TIOCGRS485      0x542E
//#define TIOCSRS485      0x542F

struct serial_rs485 {
	uint	flags;			/* RS485 feature flags */
#define SER_RS485_ENABLED		(1 << 0)	/* If enabled */
#define SER_RS485_RTS_ON_SEND		(1 << 1)	/* Logical level for
							   RTS pin when
							   sending */
#define SER_RS485_RTS_AFTER_SEND	(1 << 2)	/* Logical level for
							   RTS pin after sent*/
#define SER_RS485_RX_DURING_TX		(1 << 4)
	uint	delay_rts_before_send;	/* Delay before send (milliseconds) */
	uint	delay_rts_after_send;	/* Delay after send (milliseconds) */
	uint	padding[5];		/* Memory is cheap, new structs
					   are a royal PITA .. */
};

#endif /* _UAPI_LINUX_SERIAL_H */
